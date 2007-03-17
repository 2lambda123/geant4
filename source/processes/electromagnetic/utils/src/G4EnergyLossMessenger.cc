//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4EnergyLossMessenger.cc,v 1.27 2007-03-17 19:24:39 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
// File name:     G4EnergyLossMessenger
//
// Author:        Michel Maire
//
// Creation date: 22-06-2000
//
// Modifications:
// 10-01-06 SetStepLimits -> SetStepFunction (V.Ivanchenko)
// 10-01-06 PreciseRange -> CSDARange (V.Ivanchenko)
// 10-05-06 Add command MscStepLimit (V.Ivanchenko) 
// 10-10-06 Add DEDXBinning command (V.Ivanchenko)
// 07-02-07 Add MscLateralDisplacement command (V.Ivanchenko)
// 12-02-07 Add SetSkin, SetLinearLossLimit (V.Ivanchenko)
// 15-03-07 Send a message "/run/physicsModified" if reinitialisation
//          is needed after the command (V.Ivanchenko)
// 16-03-07 modify /process/eLoss/minsubsec command (V.Ivanchenko)
//
// -------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4EnergyLossMessenger.hh"

#include "G4VEnergyLoss.hh"
#include "G4LossTableManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4EmProcessOptions.hh"
#include "G4UImanager.hh"

#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4EnergyLossMessenger::G4EnergyLossMessenger()
{
  eLossDirectory = new G4UIdirectory("/process/eLoss/");
  eLossDirectory->SetGuidance("Commands for G4VEnergyLoss.");

  RndmStepCmd = new G4UIcmdWithABool("/process/eLoss/rndmStep",this);
  RndmStepCmd->SetGuidance("Randomize the proposed step by eLoss.");
  RndmStepCmd->SetParameterName("choice",true);
  RndmStepCmd->SetDefaultValue(false);
  RndmStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EnlossFlucCmd = new G4UIcmdWithABool("/process/eLoss/fluct",this);
  EnlossFlucCmd->SetGuidance("Switch true/false the energy loss fluctuations.");
  EnlossFlucCmd->SetParameterName("choice",true);
  EnlossFlucCmd->SetDefaultValue(true);
  EnlossFlucCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SubSecCmd = new G4UIcmdWithABool("/process/eLoss/subsec",this);
  SubSecCmd->SetGuidance("Switch true/false the subcutoff generation.");
  SubSecCmd->SetParameterName("choice",true);
  SubSecCmd->SetDefaultValue(true);
  SubSecCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  MinSubSecCmd = new G4UIcmdWithADouble("/process/eLoss/minsubsec",this);
  MinSubSecCmd->SetGuidance("Set the ratio subcut/cut ");
  MinSubSecCmd->SetParameterName("rcmin",true);
  MinSubSecCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  StepFuncCmd = new G4UIcommand("/process/eLoss/StepFunction",this);
  StepFuncCmd->SetGuidance("Set the energy loss step limitation parameters.");
  StepFuncCmd->SetGuidance("  dRoverR   : max Range variation per step");
  StepFuncCmd->SetGuidance("  finalRange: range for final step");

  G4UIparameter* dRoverRPrm = new G4UIparameter("dRoverR",'d',false);
  dRoverRPrm->SetGuidance("max Range variation per step (fractional number)");
  dRoverRPrm->SetParameterRange("dRoverR>0. && dRoverR<=1.");
  StepFuncCmd->SetParameter(dRoverRPrm);

  G4UIparameter* finalRangePrm = new G4UIparameter("finalRange",'d',false);
  finalRangePrm->SetGuidance("range for final step");
  finalRangePrm->SetParameterRange("finalRange>0.");
  StepFuncCmd->SetParameter(finalRangePrm);

  G4UIparameter* unitPrm = new G4UIparameter("unit",'s',true);
  unitPrm->SetGuidance("unit of finalRange");
  unitPrm->SetDefaultValue("mm");
  G4String unitCandidates = 
    G4UIcommand::UnitsList(G4UIcommand::CategoryOf("mm"));
  unitPrm->SetParameterCandidates(unitCandidates);

  StepFuncCmd->SetParameter(unitPrm);
  StepFuncCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  MinEnCmd = new G4UIcmdWithADoubleAndUnit("/process/eLoss/minKinEnergy",this);
  MinEnCmd->SetGuidance("Set the min kinetic energy");
  MinEnCmd->SetParameterName("emin",true);
  MinEnCmd->SetUnitCategory("Energy");
  MinEnCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  MaxEnCmd = new G4UIcmdWithADoubleAndUnit("/process/eLoss/maxKinEnergy",this);
  MaxEnCmd->SetGuidance("Set the max kinetic energy");
  MaxEnCmd->SetParameterName("emax",true);
  MaxEnCmd->SetUnitCategory("Energy");
  MaxEnCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  IntegCmd = new G4UIcmdWithABool("/process/eLoss/integral",this);
  IntegCmd->SetGuidance("Switch true/false the integration of cross section over step.");
  IntegCmd->SetParameterName("integ",true);
  IntegCmd->SetDefaultValue(true);
  IntegCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  rangeCmd = new G4UIcmdWithABool("/process/eLoss/CSDARange",this);
  rangeCmd->SetGuidance("Switch true/false the precise range calculation.");
  rangeCmd->SetParameterName("range",true);
  rangeCmd->SetDefaultValue(true);
  rangeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  lpmCmd = new G4UIcmdWithABool("/process/eLoss/LPM",this);
  lpmCmd->SetGuidance("Switch true/false the LPM effect calculation.");
  lpmCmd->SetParameterName("lpm",true);
  lpmCmd->SetDefaultValue(true);
  lpmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  latCmd = new G4UIcmdWithABool("/process/eLoss/MscLateralDisplacement",this);
  latCmd->SetGuidance("Switch true/false sampling of latra dislacent.");
  latCmd->SetParameterName("lat",true);
  latCmd->SetDefaultValue(true);
  latCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  dedxCmd = new G4UIcmdWithAnInteger("/process/eLoss/binsDEDX",this);
  dedxCmd->SetGuidance("Set number of bins for DEDX tables.");
  dedxCmd->SetParameterName("binsDEDX",true);
  //  dedxCmd->SetParameterRange("binsDEDX>59");
  dedxCmd->SetDefaultValue(120);
  dedxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  lbCmd = new G4UIcmdWithAnInteger("/process/eLoss/binsLambda",this);
  lbCmd->SetGuidance("Set number of bins for Lambda tables.");
  lbCmd->SetParameterName("binsL",true);
  //  lbCmd->SetParameterRange("binsL>59");
  lbCmd->SetDefaultValue(120);
  lbCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  verCmd = new G4UIcmdWithAnInteger("/process/eLoss/verbose",this);
  verCmd->SetGuidance("Set verbose level for EM physics.");
  verCmd->SetParameterName("verb",true);
  verCmd->SetDefaultValue(1);
  verCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  mscCmd = new G4UIcommand("/process/eLoss/MscStepLimit",this);
  mscCmd->SetGuidance("Set msc step limit flag and facRange value.");
  G4UIparameter* facRange = new G4UIparameter("facRange",'d',false);
  facRange->SetGuidance("msc parameter facRange");
  facRange->SetParameterRange("facRange>0.");
  mscCmd->SetParameter(facRange);
  G4UIparameter* msc = new G4UIparameter("algMsc",'s',true);
  msc->SetGuidance("msc step algorithm flag");
  msc->SetDefaultValue("true");
  mscCmd->SetParameter(msc);
  mscCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  lllCmd = new G4UIcmdWithADouble("/process/eLoss/linLossLimit",this);
  lllCmd->SetGuidance("Set linearLossLimit parameter.");
  lllCmd->SetParameterName("linlim",true);
  lllCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  skinCmd = new G4UIcmdWithADouble("/process/eLoss/MscSkin",this);
  skinCmd->SetGuidance("Set skin parameter for multiple scattering.");
  skinCmd->SetParameterName("skin",true);
  skinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4EnergyLossMessenger::~G4EnergyLossMessenger()
{
  delete RndmStepCmd;
  delete EnlossFlucCmd;
  delete SubSecCmd;
  delete MinSubSecCmd;
  delete StepFuncCmd;
  delete eLossDirectory;
  delete MinEnCmd;
  delete MaxEnCmd;
  delete IntegCmd;
  delete rangeCmd;
  delete lpmCmd;
  delete latCmd;
  delete verCmd;
  delete mscCmd;
  delete dedxCmd;
  delete lbCmd;
  delete lllCmd;
  delete skinCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4EnergyLossMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  G4LossTableManager* lossTables = G4LossTableManager::Instance();
 
  if (command == RndmStepCmd) {
    G4VEnergyLoss::SetRndmStep(RndmStepCmd->GetNewBoolValue(newValue));
    lossTables->SetRandomStep(RndmStepCmd->GetNewBoolValue(newValue));
  }

  if (command == EnlossFlucCmd) {
    G4VEnergyLoss::SetEnlossFluc(EnlossFlucCmd->GetNewBoolValue(newValue));
    lossTables->SetLossFluctuations(EnlossFlucCmd->GetNewBoolValue(newValue));
  }

  if (command == SubSecCmd) {
    G4VEnergyLoss::SetSubSec(SubSecCmd->GetNewBoolValue(newValue));
    lossTables->SetSubCutoff(SubSecCmd->GetNewBoolValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }

  if (command == MinSubSecCmd) {
    lossTables->SetMinSubRange(MinSubSecCmd->GetNewDoubleValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }

  if (command == StepFuncCmd) {
    G4double v1,v2;
    G4String unt;
    std::istringstream is(newValue);
    is >> v1 >> v2 >> unt;
    v2 *= G4UIcommand::ValueOf(unt);
    G4VEnergyLoss::SetStepFunction(v1,v2);
    lossTables->SetStepFunction(v1,v2);
  }

  if (command == mscCmd) {
    G4double f;
    G4bool a = true;
    G4String s;
    std::istringstream is(newValue);
    is >> f >> s;
    if(s == "false" || s == "0" || s == "no") a = false; 
    lossTables->SetMscStepLimitation(a,f);
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }

  if (command == MinEnCmd) {
    lossTables->SetMinEnergy(MinEnCmd->GetNewDoubleValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }

  if (command == MaxEnCmd) { 
    lossTables->SetMaxEnergy(MaxEnCmd->GetNewDoubleValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }  

  if (command == IntegCmd) 
    lossTables->SetIntegral(IntegCmd->GetNewBoolValue(newValue));
  
  if (command == rangeCmd) 
    lossTables->SetBuildCSDARange(rangeCmd->GetNewBoolValue(newValue));
  
  if (command == lpmCmd) {
    lossTables->SetLPMFlag(lpmCmd->GetNewBoolValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }

  if (command == latCmd) {
    lossTables->SetMscLateralDisplacement(latCmd->GetNewBoolValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }

  if (command == verCmd) 
    lossTables->SetVerbose(verCmd->GetNewIntValue(newValue));

  if (command == lllCmd) 
    lossTables->SetLinearLossLimit(lllCmd->GetNewDoubleValue(newValue));
  
  if (command == skinCmd) {
    lossTables->SetSkin(skinCmd->GetNewDoubleValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }  

  G4EmProcessOptions opt;
  if (command == dedxCmd) { 
    opt.SetDEDXBinning(dedxCmd->GetNewIntValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }

  if (command == lbCmd) {
    opt.SetDEDXBinning(lbCmd->GetNewIntValue(newValue));
    G4UImanager::GetUIpointer()->ApplyCommand("/run/physicsModified");
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

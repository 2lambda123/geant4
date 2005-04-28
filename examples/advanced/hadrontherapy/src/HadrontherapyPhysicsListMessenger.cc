//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: HadrontherapyPhysicsListMessenger.cc,v 1.2 2005-04-28 20:39:33 mpiergen Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------

#include "HadrontherapyPhysicsListMessenger.hh"
#include "HadrontherapyPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

HadrontherapyPhysicsListMessenger::HadrontherapyPhysicsListMessenger(HadrontherapyPhysicsList * physList)
:physicsList(physList)
{
  // MGP ---- ToDo: who is responsible for deleting lowEnDir?
  lowEnDir = new G4UIdirectory("/lowEnergy/");
  
  listDir = new G4UIdirectory("/physics/");
  // Building modular PhysicsList

  physicsListCmd = new G4UIcmdWithAString("/physics/addPhysics",this);  
  physicsListCmd->SetGuidance("Add chunks of PhysicsList.");
  physicsListCmd->SetParameterName("physList",false);
  physicsListCmd->AvailableForStates(G4State_PreInit);


  cutGLowLimCmd = new G4UIcmdWithADoubleAndUnit("/lowEnergy/lowlimG",this);
  cutGLowLimCmd->SetGuidance("Set ENERGY low limit for Gamma.");
  cutGLowLimCmd->SetParameterName("energy",true);
  cutGLowLimCmd->SetDefaultValue(1e-3);
  cutGLowLimCmd->SetDefaultUnit("MeV");
  cutGLowLimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  cutELowLimCmd = new G4UIcmdWithADoubleAndUnit("/lowEnergy/lowlimE",this);
  cutELowLimCmd->SetGuidance("Set ENERGY low limit for e-.");
  cutELowLimCmd->SetParameterName("energy",true);
  cutELowLimCmd->SetDefaultValue(1e-3);
  cutELowLimCmd->SetDefaultUnit("MeV");
  cutELowLimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  cutGELowLimCmd = new G4UIcmdWithADoubleAndUnit("/lowEnergy/lowlimGE",this);
  cutGELowLimCmd->SetGuidance("Set ENERGY low limit for e- and Gamma.");
  cutGELowLimCmd->SetParameterName("energy",true);
  cutGELowLimCmd->SetDefaultValue(1e-3);
  cutGELowLimCmd->SetDefaultUnit("MeV");
  cutGELowLimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  cutSecPhotCmd = new G4UIcmdWithADoubleAndUnit("/lowEnergy/secphotcut",this);
  cutSecPhotCmd->SetGuidance("Set production threshold for secondary Gamma");
  cutSecPhotCmd->SetParameterName("energy",true);
  cutSecPhotCmd->SetDefaultValue(5e-5);
  cutSecPhotCmd->SetDefaultUnit("MeV");
  cutSecPhotCmd->AvailableForStates(G4State_Idle);

  cutSecElecCmd = new G4UIcmdWithADoubleAndUnit("/lowEnergy/seceleccut",this);
  cutSecElecCmd->SetGuidance("Set production threshold for secondary e-");
  cutSecElecCmd->SetParameterName("energy",true);
  cutSecElecCmd->SetDefaultValue(5e-5);
  cutSecElecCmd->SetDefaultUnit("MeV");
  cutSecElecCmd->AvailableForStates(G4State_Idle);

  cutGCmd = new G4UIcmdWithADoubleAndUnit("/lowEnergy/cutG",this);
  cutGCmd->SetGuidance("Set cut values by RANGE for Gamma.");
  cutGCmd->SetParameterName("range",true);
  cutGCmd->SetDefaultValue(1.);
  cutGCmd->SetDefaultUnit("mm");
  cutGCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  cutECmd = new G4UIcmdWithADoubleAndUnit("/lowEnergy/cutE",this);
  cutECmd->SetGuidance("Set cut values by RANGE for e-.");
  cutECmd->SetParameterName("range",true);
  cutECmd->SetDefaultValue(1.);
  cutECmd->SetDefaultUnit("mm");
  cutECmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Auger activation
  augerCmd = new G4UIcmdWithABool("/lowEnergy/auger",this);
  augerCmd->SetGuidance("Set flag Auger electrons production.");
  augerCmd->SetParameterName("Auger",true);
  augerCmd->SetDefaultValue(false);
  augerCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Select angular distribution
  
  angularDistributionCmd = new G4UIcmdWithAString("/lowEnergy/angular",this);  
  angularDistributionCmd->SetGuidance("Select angular distribution: tsai or 2bn or 2bs");
  angularDistributionCmd->SetParameterName("angularDistribution",false);
  angularDistributionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

}

HadrontherapyPhysicsListMessenger::~HadrontherapyPhysicsListMessenger()
{

  delete cutGLowLimCmd;
  delete cutELowLimCmd;
  delete cutGELowLimCmd;
  delete cutSecElecCmd;
  delete cutSecPhotCmd;
  delete cutGCmd;
  delete cutECmd;
  delete augerCmd;
  delete physicsListCmd;
  delete angularDistributionCmd;
  delete lowEnDir;
  delete listDir;
}

void HadrontherapyPhysicsListMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == cutGLowLimCmd)
    { physicsList->SetGammaLowLimit(cutGLowLimCmd->GetNewDoubleValue(newValue)); }
  
  if (command == cutELowLimCmd)
    { physicsList->SetElectronLowLimit(cutELowLimCmd->GetNewDoubleValue(newValue)); }
  
  if (command == cutGELowLimCmd)
    { physicsList->SetGELowLimit(cutGELowLimCmd->GetNewDoubleValue(newValue)); }
  
  if (command == cutSecPhotCmd)
    { physicsList->SetLowEnSecPhotCut(cutSecPhotCmd->GetNewDoubleValue(newValue)); }

  if (command == cutSecElecCmd)
    { physicsList->SetLowEnSecElecCut(cutSecElecCmd->GetNewDoubleValue(newValue)); }

  if (command == cutGCmd)
    { physicsList->SetGammaCut(cutGCmd->GetNewDoubleValue(newValue)); }
  
  if (command == cutECmd)
    { physicsList->SetElectronCut(cutECmd->GetNewDoubleValue(newValue)); }
  
  if (command == augerCmd)
    { physicsList->ActivateAuger(augerCmd->GetNewBoolValue(newValue)); }
  
  if (command == physicsListCmd)
    { physicsList->AddPhysicsList(newValue); }
  
  if (command == angularDistributionCmd)
    { physicsList->SetAngularDistribution(newValue); }
}







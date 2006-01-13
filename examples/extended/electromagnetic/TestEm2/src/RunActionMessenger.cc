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
// $Id: RunActionMessenger.cc,v 1.3 2006-01-13 14:20:27 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger(RunAction* run)
:Run(run)
{
  runDir = new G4UIdirectory("/testem/run/");
  runDir->SetGuidance("run control");
      
  accCmd = new G4UIcmdWith3Vector("/testem/run/acceptance",this);
  accCmd->SetGuidance("set Edep and RMS");
  accCmd->SetGuidance("acceptance values for first layer");
  accCmd->SetParameterName("edep","rms","limit",true);
  accCmd->SetRange("edep>0 && edep<1 && rms>0");
  accCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  verbCmd = new G4UIcmdWithAnInteger("/testem/run/verbose",this);
  verbCmd->SetGuidance("set verbose level for runAction");
  verbCmd->SetParameterName("verbose",false);
    
  histoDir = new G4UIdirectory("/testem/histo/");
  histoDir->SetGuidance("histograms control");
  
  factoryCmd = new G4UIcmdWithAString("/testem/histo/setFileName",this);
  factoryCmd->SetGuidance("set name for the histograms file");

  typeCmd = new G4UIcmdWithAString("/testem/histo/setFileType",this);
  typeCmd->SetGuidance("set type (hbook, root, XML) for the histograms file");
  typeCmd->SetCandidates("hbook root XML");         
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
  delete verbCmd;
  delete accCmd;
  delete runDir;
  delete factoryCmd;
  delete typeCmd;
  delete histoDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{      
  if (command == accCmd )
   { Run->SetEdepAndRMS(accCmd->GetNew3VectorValue(newValue));}
   
  if (command == verbCmd )
   { Run->SetVerbose(verbCmd->GetNewIntValue(newValue));}
      
  if (command == factoryCmd)
   { Run->SetHistoName(newValue);}

  if (command == typeCmd)
   { Run->SetHistoType(newValue);}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
// $Id: XrayFluoPrimarygeneratorMessenger.cc
// GEANT4 tag $Name: xray_fluo-V03-02-00
//
// Author: Elena Guardincerri (Elena.Guardincerri@ge.infn.it)
//
// History:
// -----------
// 28 Nov 2001 Elena Guardincerri     Created
//
// -------------------------------------------------------------------

#include "XrayFluoPrimaryGeneratorMessenger.hh"
#include "XrayFluoPrimaryGeneratorAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XrayFluoPrimaryGeneratorMessenger::XrayFluoPrimaryGeneratorMessenger(XrayFluoPrimaryGeneratorAction* XrayFluoGun)
  :XrayFluoAction(XrayFluoGun)
{ 
  RndmCmd = new G4UIcmdWithAString("/gun/random",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RndmVert = new G4UIcmdWithAString("/gun/randomVert",this);
  RndmVert->SetGuidance("Shoot randomly the incident particle.");
  RndmVert->SetGuidance("  Choice : on(default), off");
  RndmVert->SetParameterName("choice",true);
  RndmVert->SetDefaultValue("on");
  RndmVert->SetCandidates("on off");
  RndmVert->AvailableForStates(G4State_PreInit,G4State_Idle);

  spectrum = new G4UIcmdWithAString("/gun/spectrum",this);
  spectrum->SetGuidance("Shoot the incident particle with a certain energy spectrum.");
  spectrum->SetGuidance("  Choice : on(default), off");
  spectrum->SetParameterName("choice",true);
  spectrum->SetDefaultValue("on");
  spectrum->SetCandidates("on off");
  spectrum->AvailableForStates(G4State_PreInit,G4State_Idle);

  isoVert = new G4UIcmdWithAString("/gun/isoVert",this);
  isoVert->SetGuidance("Shoot the incident particle from an isotrofic direction.");
  isoVert->SetGuidance("  Choice : on(default), off");
  isoVert->SetParameterName("choice",true);
  isoVert->SetDefaultValue("on");
  isoVert->SetCandidates("on off");
  isoVert->AvailableForStates(G4State_PreInit,G4State_Idle);

  loadPahseSpace=new G4UIcmdWithAString("/gun/loadGunData",this);
  loadPahseSpace->SetGuidance("Load emission from samples form previous runs");
  loadPahseSpace->SetGuidance("Please enter the filename");
  loadPahseSpace->SetParameterName("choice",true);
  loadPahseSpace->AvailableForStates(G4State_Idle);

  loadRayleighData=new G4UIcmdWithABool("/gun/loadRayleighFlag",this);
  loadRayleighData->SetGuidance("Select if data form rayleigh scattering must be loaded");
  loadRayleighData->SetGuidance("To be used before and togheter with /gun/loadGunData");
  loadRayleighData->SetParameterName("Rayleigh Flag",true);
  loadRayleighData->SetDefaultValue(true);
  loadRayleighData->AvailableForStates(G4State_Idle);

  G4cout << "XrayFluoPrimaryGeneratorMessenger created" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XrayFluoPrimaryGeneratorMessenger::~XrayFluoPrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete  RndmVert;
  delete spectrum;
  delete isoVert;
  delete loadPahseSpace;
  delete loadRayleighData;
  G4cout << "XrayFluoPrimaryGeneratorMessenger deleted" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XrayFluoPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{ 
  if( command == RndmCmd )
   { XrayFluoAction->SetRndmFlag(newValue);}
 if( command == RndmVert )
   { XrayFluoAction->SetRndmVert(newValue);}
 if( command == spectrum )
   { XrayFluoAction->SetSpectrum(newValue);} 
 if( command == isoVert )
   { XrayFluoAction->SetIsoVert(newValue);}
 if( command == loadPahseSpace )
   { XrayFluoAction->ActivatePhaseSpace(newValue);}
 if( command == loadRayleighData )
   { 
     G4cout << "newValue: " << newValue << G4endl;

     G4bool newRayFlag = loadRayleighData->GetNewBoolValue(newValue);
     XrayFluoAction->SetRayleighFlag(newRayFlag);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


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
// $Id: PrimaryGeneratorMessenger.cc,v 1.3 2004-09-15 13:17:20 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"
#include "G4UIdirectory.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                             PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  gunDir = new G4UIdirectory("/testem/gun/");
  gunDir->SetGuidance("gun control");
 
  DefaultCmd = new G4UIcmdWithAnInteger("/testem/gun/setDefault",this);
  DefaultCmd->SetGuidance("set/reset kinematic defined in PrimaryGenerator");
  DefaultCmd->SetGuidance("0=boxCenter, else=frontFace");
  DefaultCmd->SetParameterName("position",true);
  DefaultCmd->SetDefaultValue(1);
  DefaultCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  RndmCmd = new G4UIcmdWithADouble("/testem/gun/rndm",this);
  RndmCmd->SetGuidance("random lateral extension on the beam");
  RndmCmd->SetGuidance("in fraction of 0.5*sizeYZ");
  RndmCmd->SetParameterName("rBeam",false);
  RndmCmd->SetRange("rBeam>=0.&&rBeam<=1.");
  RndmCmd->AvailableForStates(G4State_Idle);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete DefaultCmd;
  delete RndmCmd;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{ 
  if (command == DefaultCmd)
   {Action->SetDefaultKinematic(DefaultCmd->GetNewIntValue(newValue));}
   
  if (command == RndmCmd)
   {Action->SetRndmBeam(RndmCmd->GetNewDoubleValue(newValue));}   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


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
// $Id: RE01PrimaryGeneratorMessenger.cc,v 1.1 2004-11-26 07:37:42 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//


#include "RE01PrimaryGeneratorMessenger.hh"
#include "RE01PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ios.hh"

RE01PrimaryGeneratorMessenger::RE01PrimaryGeneratorMessenger(RE01PrimaryGeneratorAction * mpga)
:myAction(mpga)
{
  mydetDirectory = new G4UIdirectory("/mydet/");
  mydetDirectory->SetGuidance("RE01 detector control commands.");

  genCmd = new G4UIcmdWithAString("/mydet/generator",this);
  genCmd->SetGuidance("Select primary generator.");
  genCmd->SetGuidance(" Available generators : PYTHIA, particleGun");
  genCmd->SetParameterName("generator",true);
  genCmd->SetDefaultValue("PYTHIA");
  genCmd->SetCandidates("PYTHIA particleGun");
}

RE01PrimaryGeneratorMessenger::~RE01PrimaryGeneratorMessenger()
{
  delete genCmd;
  delete mydetDirectory;
}

void RE01PrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if( command==genCmd )
  { myAction->SetHEPEvtGenerator(newValue=="PYTHIA"); }
}

G4String RE01PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String cv;
  
  if( command==genCmd )
  {
    if(myAction->GetHEPEvtGenerator())
    { cv = "PYTHIA"; }
    else
    { cv = "particleGun"; }
  }
  
  return cv;
}


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
//
// --------------------------------------------------------------
//      GEANT 4 - exampleExN06
//
// --------------------------------------------------------------
//
// File:        exampleExN06.cc
// Description: Test of Continuous Process G4Cerenkov
//              and RestDiscrete Process G4Scintillation
//              -- Generation Cerenkov Photons --
//              -- Generation Scintillation Photons --
//              -- Transport of optical Photons --
// Version:     5.0
// Created:     1996-04-30
// Author:      Juliet Armstrong
// Updated:     1998-06-03 by Peter Gumplinger for example ExN06
// mail:        gum@triumf.ca
//
// --------------------------------------------------------------

#include "ExN06RunAction.hh"
#include "ExN06DetectorConstruction.hh"
#include "ExN06PrimaryGeneratorAction.hh"
#include "ExN06PhysicsList.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "G4ios.hh"
#include <stdlib.h>

int main(int argc,char** argv) {

  // Seed the random number generator manually
  // -----------------------------------------

  G4long myseed = 345354;

  HepRandom::setTheSeed(myseed);

  // Run manager

  G4RunManager * runManager = new G4RunManager;

  // UserInitialization classes - mandatory

  runManager-> SetUserInitialization(new ExN06DetectorConstruction);
  runManager-> SetUserInitialization(new ExN06PhysicsList);

  // UserAction classes - optional

  runManager->SetUserAction(new ExN06RunAction);
  runManager->SetUserAction(new ExN06PrimaryGeneratorAction);

  // User interactions
  // Define (G)UI for interactive mode
  if(argc==1)
  {
    // G4UIterminal is a (dumb) terminal.
    G4UIsession * session = new G4UIterminal;
    session->SessionStart();
    delete session;
  }
  else
  // Batch mode
  {
    G4UImanager * UI = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }

  delete runManager;

  return EXIT_SUCCESS;
}


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
// $Id: testDrawVox.cc,v 1.4 2006-05-02 13:25:39 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT4 - test01 
//
// --------------------------------------------------------------
// Comments
//
// 
// --------------------------------------------------------------

#include "TstDrawVox01DetectorConstruction.hh"
#include "TstDrawVox01RunAction.hh"
#include "TstDrawVox01PrimaryGeneratorAction.hh"
#include "TstDrawVox01SteppingAction.hh"
#include "TstDrawVox01PhysicsList.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "G4RunManager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4ios.hh"

int main(int argc,char** argv) {

  // Set the default random engine to RanecuEngine
  CLHEP::RanecuEngine defaultEngine;
  CLHEP::HepRandom::setTheEngine(&defaultEngine);

  // Run manager
  G4RunManager * runManager = new G4RunManager;

  // UserInitialization classes
  runManager->SetUserInitialization(new TstDrawVox01DetectorConstruction);
  runManager->SetUserInitialization(new TstDrawVox01PhysicsList);

  #ifdef G4VIS_USE
    // Visualization, if you choose to have it!
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
  #endif

  // UserAction classes
  runManager->SetUserAction(new TstDrawVox01RunAction);
  runManager->SetUserAction(new TstDrawVox01PrimaryGeneratorAction);
  runManager->SetUserAction(new TstDrawVox01SteppingAction);

  // User interactions
  // Define (G)UI for interactive mode
 
  // User interactions
    G4UImanager * UI = G4UImanager::GetUIpointer();
  
  if(argc==1)
  { 
    // G4UIterminal is a (dumb) terminal.
    G4UIsession * session = new G4UIterminal;
    UI->ApplyCommand("/control/execute prerun.g4mac"); 
    session->SessionStart();
    delete session;
  }
  else
  // Batch mode
  { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }
  
  #ifdef G4VIS_USE
    delete visManager;
  #endif
  delete runManager;
  return 0;
}


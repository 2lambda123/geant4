// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: test14.cc,v 1.2 1999-06-14 14:34:28 aforti Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "Tst14DetectorConstruction.hh"
#include "Tst14RunAction.hh"
#include "Tst14PrimaryGeneratorAction.hh"
#include "Tst14PhysicsList.hh"
#include "Tst14SteppingAction.hh"
#include "Tst14TrackingAction.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4RunManager.hh"

#include "G4ios.hh"

int main(int argc,char** argv) {

  // Set the default random engine to RanecuEngine
  RanecuEngine defaultEngine;
  HepRandom::setTheEngine(&defaultEngine);

  // Run manager
  G4RunManager * runManager = new G4RunManager;

  // UserInitialization classes
  runManager->SetUserInitialization(new Tst14DetectorConstruction);
  runManager->SetUserInitialization(new Tst14PhysicsList);

  // UserAction classes
  runManager->SetUserAction(new Tst14RunAction);
  runManager->SetUserAction(new Tst14PrimaryGeneratorAction);
  //runManager->SetUserAction(new Tst14SteppingAction);
  runManager->SetUserAction(new Tst14TrackingAction);

  if(argc==1)
  {
    // G4UIterminal is a (dumb) terminal.
    G4UIsession* session = new G4UIterminal;
    session->SessionStart();
    delete session;
  }
  else
  {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }

  delete runManager;
  return 0;
}











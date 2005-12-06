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
// $Id: test14.cc,v 1.10 2005-12-06 16:01:18 gcosmo Exp $
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
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Run manager
  G4RunManager * runManager = new G4RunManager;

  // UserInitialization classes
  Tst14DetectorConstruction* detector;
  detector = new Tst14DetectorConstruction;

  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new Tst14PhysicsList);

  // UserAction classes
  runManager->SetUserAction(new Tst14PrimaryGeneratorAction(detector));
  Tst14RunAction* runaction = new Tst14RunAction;
  runManager->SetUserAction(runaction);

  runManager->SetUserAction(new Tst14SteppingAction);
  //runManager->SetUserAction(new Tst14TrackingAction);

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(argc==1)
  {
    // G4UIterminal is a (dumb) terminal.
    //   G4UIsession* session = new G4UIterminal;
    //   UImanager->ApplyCommand("/control/execute prerunTst14.mac");
    G4UIsession * session = 0;
    session = new G4UIterminal();
    session->SessionStart();
    delete session;
  }
  else
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }

  delete runManager;
  return 0;
}

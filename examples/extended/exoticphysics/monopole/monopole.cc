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
// $Id: monopole.cc,v 1.2 2009-07-15 10:19:47 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"
#include "globals.hh"

#include "DetectorConstruction.hh"
#include "G4MonopolePhysics.hh"
#include "QGSP.hh"
#include "PrimaryGeneratorAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {

  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  //create physicsList
  QGSP* phys = new QGSP();
  G4MonopolePhysics * theMonopole = new G4MonopolePhysics();
  phys->RegisterPhysics(theMonopole);

  //get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // Setup monopole
  G4String s = ""; 
  if(argc > 2) s = argv[2];
  UI->ApplyCommand("/control/verbose 1");
  UI->ApplyCommand("/monopole/setup "+s);

  // mandator user classes
  DetectorConstruction* det = new DetectorConstruction();

  runManager->SetUserInitialization(det);  
  runManager->SetUserInitialization(phys);

  PrimaryGeneratorAction* kin = new PrimaryGeneratorAction(det);
  runManager->SetUserAction(kin);

#ifdef G4VIS_USE
  //visualization manager
  G4VisManager* visManager = 0;
#endif

  //user action classes
  RunAction* run;

  runManager->SetUserAction(run = new RunAction(det, kin));
  runManager->SetUserAction(new EventAction);
  runManager->SetUserAction(new TrackingAction(run));
  runManager->SetUserAction(new SteppingAction(det, run));

  if (argc == 1)   // Define UI terminal for interactive mode
    {
      visManager = new G4VisExecutive();
      visManager->Initialize();
      G4UIsession* session = 0;
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);
#else
      session = new G4UIterminal();
#endif
      session->SessionStart();
      delete session;
    }
  else           // Batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }

  //job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
 
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

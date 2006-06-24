// -------------------------------------------------------------------
// $Id: Microbeam.cc,v 1.6 2006-06-24 19:29:12 sincerti Exp $
// -------------------------------------------------------------------
//  GEANT4 - Microbeam example
//  Developed by S. Incerti et al.
//  Centre d'Etudes Nucleaires de Bordeaux-Gradignan
//  CNRS / IN2P3 / Bordeaux 1 University
//  33175 Gradignan, France
//  incerti@cenbg.in2p3.fr
// -------------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "MicrobeamDetectorConstruction.hh"
#include "MicrobeamPhysicsList.hh"
#include "MicrobeamPrimaryGeneratorAction.hh"
#include "MicrobeamRunAction.hh"
#include "MicrobeamEventAction.hh"
#include "MicrobeamTrackingAction.hh"
#include "MicrobeamSteppingAction.hh"
#include "MicrobeamSteppingVerbose.hh"

int main(int argc,char** argv) {

  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // verbose output class
  G4VSteppingVerbose::SetInstance(new MicrobeamSteppingVerbose);
  
  // construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes
  MicrobeamDetectorConstruction* detector = new MicrobeamDetectorConstruction;

  runManager->SetUserInitialization(detector);

  runManager->SetUserInitialization(new MicrobeamPhysicsList);
    
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // set user action classes
  MicrobeamPrimaryGeneratorAction* KinAct = new MicrobeamPrimaryGeneratorAction(detector);
  runManager->SetUserAction(KinAct);
 
  MicrobeamRunAction* RunAct = new MicrobeamRunAction(detector);
  
  runManager->SetUserAction(RunAct);
  runManager->SetUserAction(new MicrobeamEventAction(RunAct));
  runManager->SetUserAction(new MicrobeamTrackingAction(RunAct)); 
  runManager->SetUserAction(new MicrobeamSteppingAction(RunAct,detector));
  
  // initialize G4 kernel
  runManager->Initialize();
    
  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer(); 
  
  // local user files created by the simulation
  system ("rm -rf dose.txt");
  system ("rm -rf 3DDose.txt");
  system ("rm -rf stoppingPower.txt");
  system ("rm -rf range.txt");
  system ("rm -rf beamPosition.txt");
       
  if (argc==1)   // define UI session for interactive mode.
    {
      // G4UIterminal is a dumb or a TCSH terminal.
      // G4UIsession * session = new G4UIterminal(); // dumb terminal (for Windows)
      G4UIsession * session = new G4UIterminal(new G4UItcsh); // TCSH terminal
      UI->ApplyCommand("/control/execute microbeam.mac");    
      session->SessionStart();
      delete session;
    }
  else           // batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }

  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
  
}


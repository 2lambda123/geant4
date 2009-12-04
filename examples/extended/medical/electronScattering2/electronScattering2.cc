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

#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4ScoringManager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#ifdef G4UI_USE_WIN32
#include "G4UIWin32.hh"
#endif

#include "ElectronBenchmarkDetector.hh"
#include "PhysicsList.hh"
#include "ElectronPrimaryGeneratorAction.hh"
#include "ElectronRunAction.hh"
#include "ElectronEventAction.hh"

int main(int argc,char** argv) {

  // Parse the arguments
  G4String outputFile = "output.csv";
  G4String startingSeed = "1";
  G4String macroFile = "None";
  if (argc > 1) macroFile = argv[1];
  if (argc > 2) startingSeed = argv[2];
  if (argc > 3) outputFile = argv[3];
  G4cout << "Starting run with" << G4endl;
  G4cout << "Macro File    : " << macroFile << G4endl;
  G4cout << "Starting Seed : " << startingSeed << G4endl;
  G4cout << "Output File   : " << outputFile << G4endl;

  // Instantiate the run manager
  G4RunManager * runManager = new G4RunManager;

 // Instantiate the random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  
  // Convert the starting seed to integer and feed it to the random engine
  unsigned startingSeedInt;
  std::istringstream is(startingSeed);
  is >> startingSeedInt;
  CLHEP::HepRandom::setTheSeed(startingSeedInt);

  // Instantiate the scoring manager
  G4ScoringManager::GetScoringManager();
     
  // Instantiate the geometry
  runManager->SetUserInitialization(new ElectronBenchmarkDetector);
 
  // Instantiate the physics list (in turn calls one of four choices of physics list)
  runManager->SetUserInitialization(new PhysicsList);
 
  // Instantiate the primary generator action (in turn uses the G4GeneralParticleSource)
  runManager->SetUserAction(new ElectronPrimaryGeneratorAction);
  
  // Instantiate the run action (in turn instantiates ElectronRun, which accumulates and dumps data)
  runManager->SetUserAction(new ElectronRunAction(outputFile));

  // Instantiate the event action (just adds code to periodically report how many events are done)
  runManager->SetUserAction(new ElectronEventAction);
  
  // Instantiate the visualization System
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  if (argc == 1)
  {
    // Since no macro was specified, instantiate an interactive session
    // (exact session type depends on user preference expressed in environment variables).
    G4UIsession* session;
#if defined(G4UI_USE_XM)
    session = new G4UIXm(argc,argv);
#elif defined(G4UI_USE_WIN32)
    session = new G4UIWin32();
#elif defined(G4UI_USE_TCSH)
    session = new G4UIterminal(new G4UItcsh);      
#else
    session = new G4UIterminal();
#endif
    session->SessionStart();
    delete session;
  }
  else
  { 
    // A macro was specified.  Execute it.
    G4UImanager* UI = G4UImanager::GetUIpointer();  
    G4String command = "/control/execute ";
    UI->ApplyCommand(command+macroFile);
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//

#include <stdexcept>

#include "globals.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIsession.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4TransportationManager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4HumanPhantomConstruction.hh"
#include "G4HumanPhantomPhysicsList.hh"
#include "G4HumanPhantomPrimaryGeneratorAction.hh"
#include "G4HumanPhantomSteppingAction.hh"
#include "G4HumanPhantomEventAction.hh"
#include "G4HumanPhantomRunAction.hh"
#ifdef G4ANALYSIS_USE 
#include  "G4HumanPhantomAnalysisManager.hh"
#endif

int main(int argc,char** argv)
{
  G4RunManager* runManager = new G4RunManager;

  // G4VSteppingVerbose* verbosity = new ExN02SteppingVerbose; 
  //  G4VSteppingVerbose::SetInstance(new ExN02SteppingVerbose);
 
 // Set mandatory initialization classes
  G4HumanPhantomConstruction* userPhantom = new G4HumanPhantomConstruction();
  runManager->SetUserInitialization(userPhantom);

  runManager->SetUserInitialization(new G4HumanPhantomPhysicsList);

  runManager->SetUserAction(new G4HumanPhantomPrimaryGeneratorAction);

#ifdef G4ANALYSIS_USE
  G4HumanPhantomAnalysisManager* analysis = G4HumanPhantomAnalysisManager::getInstance();
  analysis->book();
#endif

  G4UIsession* session=0;

  if (argc==1)   
    // Define UI session for interactive mode. 
    { 
      // G4UIterminal is a (dumb) terminal
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);
#else
      session = new G4UIterminal();
#endif
    }

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  
  runManager->SetUserAction(new G4HumanPhantomRunAction);

  

  G4HumanPhantomEventAction* eventAction = new G4HumanPhantomEventAction();
  runManager->SetUserAction(eventAction);

  runManager->SetUserAction(new G4HumanPhantomSteppingAction()); 

  G4UImanager* UI = G4UImanager::GetUIpointer();

  if (session)   // Define UI session for interactive mode.
    { 
      G4cout << " UI session starts ..." << G4endl;
      UI -> ApplyCommand("/control/execute default.mac");    
      session -> SessionStart();
      delete session;
    }
  else           // Batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI -> ApplyCommand(command+fileName);
    }     

#ifdef G4ANALYSIS_USE
  if (analysis)analysis->finish();
#endif

  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;
  return 0;
}

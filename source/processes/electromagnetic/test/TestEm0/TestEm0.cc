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
// $Id: TestEm0.cc,v 1.5 2001-07-11 10:03:35 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - example Em0 
//
// --------------------------------------------------------------
// Comments
//
// 
// --------------------------------------------------------------

///#define Em0NoOptimize 1

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4UIterminal.hh"

#include "Em0DetectorConstruction.hh"
#include "Em0PhysicsList.hh"
#include "Em0PrimaryGeneratorAction.hh"

#ifdef Em0NoOptimize
#include "Em0RunAction.hh"
#include "Em0EventAction.hh"
#include "Em0TrackingAction.hh"
#include "Em0SteppingAction.hh"
#endif

int main(int argc,char** argv) {

  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes
  Em0DetectorConstruction* DetConst;
  runManager->SetUserInitialization(DetConst = new Em0DetectorConstruction);
  runManager->SetUserInitialization(new Em0PhysicsList(DetConst));
  runManager->SetUserAction(new Em0PrimaryGeneratorAction);
  
#ifdef Em0NoOptimize
  // set user action classes
  Em0RunAction*   RunAct;
  Em0EventAction* EvtAct;
  
  runManager->SetUserAction(RunAct = new Em0RunAction); 
  runManager->SetUserAction(EvtAct = new Em0EventAction);
  runManager->SetUserAction(new Em0TrackingAction(RunAct));
  runManager->SetUserAction(new Em0SteppingAction(EvtAct));
#endif
  
  //Initialize G4 kernel
  //runManager->Initialize();
    
  // get the pointer to the User Interface manager 
    G4UImanager* UI = G4UImanager::GetUIpointer();  

  if (argc==1)   // Define UI terminal for interactive mode  
    { 
     G4UIsession * session = new G4UIterminal;
     UI->ApplyCommand("/control/execute init.mac");    
     session->SessionStart();
     delete session;
    }
  else           // Batch mode
    { 
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);
    }

  // job termination
  delete runManager;

  return 0;
}


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
// $Id: Hadrontherapy.cc Main of the Hadrontherapy example; Version 4.0 May 2005
// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the INFN, Catania, Italy
// (b) INFN Section of Genova, Genova, Italy
// 
// * cirrone@lns.infn.it
// ----------------------------------------------------------------------------
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#include "HadrontherapyEventAction.hh"
#include "HadrontherapyDetectorConstruction.hh"
#include "HadrontherapyPhysicsList.hh"
#include "HadrontherapyPhantomSD.hh"
#include "HadrontherapyPrimaryGeneratorAction.hh"
#include "HadrontherapyRunAction.hh"
#include "HadrontherapyMatrix.hh"
#include "Randomize.hh"  
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UImessenger.hh"
#include "globals.hh"
#include "HadrontherapySteppingAction.hh"
#ifdef  G4ANALYSIS_USE
#include "HadrontherapyAnalysisManager.hh"
#endif

int main(int argc ,char ** argv)
{
  HepRandom::setTheEngine(new RanecuEngine);
  G4int seed=time(NULL);
  HepRandom ::setTheSeed(seed);

  G4RunManager* runManager = new G4RunManager;

  // Initialize the geometry
  runManager -> SetUserInitialization(new HadrontherapyDetectorConstruction());
  
  // Initialize the physics 
  runManager -> SetUserInitialization(new HadrontherapyPhysicsList());
  
  // Initialize the primary particles  
  runManager -> SetUserAction(new HadrontherapyPrimaryGeneratorAction());

  // Initialize matrix 
  HadrontherapyMatrix* matrix = new HadrontherapyMatrix();
  matrix -> Initialize();

  // Optional UserActions: run, event, stepping
  HadrontherapyRunAction* pRunAction = new HadrontherapyRunAction();
  runManager -> SetUserAction(pRunAction);

  HadrontherapyEventAction* pEventAction = new HadrontherapyEventAction(matrix);
  runManager -> SetUserAction(pEventAction);


  HadrontherapySteppingAction* steppingAction = new HadrontherapySteppingAction(pRunAction); 
  runManager -> SetUserAction(steppingAction);    


#ifdef G4ANALYSIS_USE
  HadrontherapyAnalysisManager* analysis = 
    HadrontherapyAnalysisManager::getInstance();
  analysis -> book();
#endif
  
#ifdef G4VIS_USE
  // Visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager -> Initialize();
#endif
  
  
  G4UIsession* session = 0;
  if (argc == 1)   // Define UI session for interactive mode.
    {
      session = new G4UIterminal();
    } 

  // Get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  
  if (session)   // Define UI session for interactive mode.
    { 
      G4cout<<" UI session starts ..."<< G4endl;
      UI -> ApplyCommand("/control/execute defaultMacro.mac");    
      session -> SessionStart();
      delete session;
    }
  else           // Batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI -> ApplyCommand(command + fileName);
    }  

  matrix -> TotalEnergyDeposit();

#ifdef G4ANALYSIS_USE
  analysis -> finish();
#endif
  
  // Job termination
#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;

  return 0;
}

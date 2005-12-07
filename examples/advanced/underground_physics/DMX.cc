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
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//      For information related to this code contact: Alex Howard
//      e-mail: a.s.howard@ic.ac.uk
// --------------------------------------------------------------
// Comments
//
//            Underground Advanced example main program
//               by A. Howard and H. Araujo 
//                    (27th November 2001)
//
// main program
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "DMXDetectorConstruction.hh"
#include "DMXPhysicsList.hh"
#include "DMXPrimaryGeneratorAction.hh"
#include "DMXRunAction.hh"
#include "DMXEventAction.hh"
#include "DMXSteppingAction.hh"
#include "DMXStackingAction.hh"

#include <vector>

int main(int argc,char** argv) {

  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes
  DMXDetectorConstruction* detector = new DMXDetectorConstruction;
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new DMXPhysicsList);
  
 G4UIsession* session=0;
  
  if (argc==1)   // Define UI session for interactive mode.
    {
      // G4UIterminal is a (dumb) terminal.
#ifdef G4UI_USE_XM
      session = new G4UIXm(argc,argv);
#else           
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif
#endif
    }
  
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // output environment variables:
#ifdef G4ANALYSIS_USE
  G4cout << G4endl << G4endl << G4endl 
	 << " User Environment " << G4endl
	 << " Using AIDA 3.2.1 analysis " << G4endl;
#else
  G4cout << G4endl << G4endl << G4endl 
	 << " User Environment " << G4endl
	 << " G4ANALYSIS_USE environment variable not set, NO ANALYSIS " 
	 << G4endl;
#endif

#ifdef DMXENV_GPS_USE
  G4cout << " Using GPS and not DMX gun " << G4endl;
#else
  G4cout << " Using the DMX gun " << G4endl;
#endif
    
  // set user action classes
  DMXPrimaryGeneratorAction* DMXGenerator = new DMXPrimaryGeneratorAction;
  runManager->SetUserAction(DMXGenerator);
  //  runManager->SetUserAction(new DMXPrimaryGeneratorAction);
  // RunAction is inherited by EventAction for output filenames - will all
  // change when implement proper analysis manager?
  DMXRunAction* DMXRun = new DMXRunAction;
  runManager->SetUserAction(DMXRun);
  DMXEventAction* eventAction = new DMXEventAction(DMXRun,DMXGenerator);
  runManager->SetUserAction(eventAction);
  // eventAction is inherited by SteppingAction in order to switch colour
  // flag:
  runManager->SetUserAction(new DMXSteppingAction(eventAction));
  runManager->SetUserAction(new DMXStackingAction);

  //Initialize G4 kernel
  runManager->Initialize();
    
  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  

  // Define UI session for interactive mode.
  if(session) {

      // G4UIterminal is a (dumb) terminal.
      UI->ApplyCommand("/control/execute initInter.mac");    
      /*
	#ifdef G4UI_USE_XM
	// Customize the G4UIXm menubar with a macro file :
	UI->ApplyCommand("/control/execute gui.mac");
	#endif
      */
      session->SessionStart();
      delete session;
    }
  // Batch mode
  else
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


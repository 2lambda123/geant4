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
// **********************************************************************
// *                                                                    *
// *                    GEANT 4 xray_telescope advanced example         *
// *                                                                    *
// * MODULE:            XrayTel.cc                            main file *     
// * -------                                                            *
// *                                                                    *
// * Version:           0.4                                             *
// * Date:              06/11/00                                        *
// * Author:            R Nartallo                                      *
// * Organisation:      ESA/ESTEC, Noordwijk, THe Netherlands           *
// *                                                                    *
// **********************************************************************
//
// HISTORY
// -------
//
// The development of this advanced example is based on earlier work 
// carried out by a team of Geant4 collaborators  to simulate the Chandra
// and XMM X-ray observatories. The authors involved in those models are
// J Apostolakis, P Arce, S Giani, F Lei, R Nartallo, S Magni,
// P Truscott, L Urban
//
// **********************************************************************
// 
// CHANGE HISTORY
// --------------
//
// 30.11.2000 R. Nartallo, A. Pfeiffer
// - Implementation of analysis manager code for histograming
//
// 15.11.2000 R. Nartallo
// - Minor changes proposed by F. Lei to implement the GPS module now 
//   replacing the standard particle gun 
// - Remove commented lines related to old histograming code
//
// 06.11.2000 R.Nartallo
// - First implementation of X-ray Telescope advanced example.
// - Based on Chandra and XMM models
// - Lines for using GAG and the histogram manager are commented out.
//
//
// **********************************************************************

#include "G4RunManager.hh"
#include "G4UImanager.hh"
//#include "G4UIGAG.hh"
#include "G4UIterminal.hh"
#include "G4UIXm.hh" 
#include "XrayTelDetectorConstruction.hh"
#include "XrayTelPhysicsList.hh"
#include "XrayTelVisManager.hh"
#include "XrayTelEventAction.hh"
#include "XrayTelRunAction.hh"
#include "XrayTelSteppingAction.hh"
#include "XrayTelPrimaryGeneratorAction.hh"
#include <iostream.h>
#include "g4std/vector"

#include "XrayTelAnalysisManager.hh"

int main( int argc, char** argv )
{
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes
  runManager->SetUserInitialization(new XrayTelDetectorConstruction ) ;
  runManager->SetUserInitialization(new XrayTelPhysicsList);

  // setup some of the common variables
  G4bool drawEvent;
  G4std::vector<G4double*> EnteringEnergy;
  G4std::vector<G4ThreeVector*> EnteringDirection;

  // create manager for analysis. 
  char* s = getenv("G4ANALYSIS_SYSTEM");
  XrayTelAnalysisManager* analysisManager = new XrayTelAnalysisManager(s?s:"");

  // set mandatory user action class
  runManager->SetUserAction(new XrayTelPrimaryGeneratorAction);
  runManager->SetUserAction(new XrayTelRunAction(&EnteringEnergy,
				&EnteringDirection, &drawEvent, analysisManager));
  runManager->SetUserAction(new XrayTelEventAction(&drawEvent));
  runManager->SetUserAction(new XrayTelSteppingAction(
				&EnteringEnergy, &EnteringDirection, &drawEvent, analysisManager));

  // visualization manager
  G4VisManager* visManager = new XrayTelVisManager;
  visManager->Initialize();    

  //Initialize G4 kernel
  runManager->Initialize();
    
  // get the pointer to the User Interface manager 
  G4UImanager *UI = G4UImanager::GetUIpointer();  
  if ( argc==1 ){
    // G4UIsession * session = new G4UIGAG;
    G4UIsession * session = new G4UIterminal;
    session->SessionStart();
    delete session;
  }
  else {
    // Create a pointer to the User Interface manager 
    G4String command = "/control/execute ";
    for (int i=2; i<=argc; i++) {
      G4String macroFileName = argv[i-1];
      UI->ApplyCommand(command+macroFileName);
    }
  }                                  

  // job termination
  delete visManager;
  delete analysisManager;
  delete runManager;
  return 0;
}

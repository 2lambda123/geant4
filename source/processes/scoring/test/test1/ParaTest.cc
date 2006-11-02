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
// $Id: ParaTest.cc,v 1.2 2006-11-02 17:30:29 allison Exp $
// --------------------------------------------------------------
//
// --------------------------------------------------------------
//      GEANT 4 - A01app
// --------------------------------------------------------------
// Comments
//   Tutorial for Geant4 lectures
// --------------------------------------------------------------


#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "A01DetectorConstruction.hh"
#include "A01ParallelWorld.hh"
#include "A01PhysicsList.hh"
#include "A01PrimaryGeneratorAction.hh"

#include "A01EventAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif


int main(int argc,char** argv)
{
  if(argc<3)
  {
    G4cout << "Usage : " << G4endl;
    G4cout << "  % ParaTest para|nopara disp|nodisp [macro_file]" << G4endl;
    return 1;
  }
  G4bool ifPara;
  G4String param = argv[1];
  if(param=="para") 
  { ifPara = true; }
  else if(param=="nopara")
  { ifPara = false; }
  else
  {
    G4cout << "Usage : " << G4endl;
    G4cout << "  % ParaTest para|nopara disp|nodisp [macro_file]" << G4endl;
    return 1;
  }
  G4bool ifDisp;
  param = argv[2];
  if(param=="disp")
  { ifDisp = true; }
  else if(param=="nodisp")
  { ifDisp = false; }
  else
  {
    G4cout << "Usage : " << G4endl;
    G4cout << "  % ParaTest para|nopara disp|nodisp [macro_file]" << G4endl;
    return 1;
  }

  // RunManager construction
  G4RunManager* runManager = new G4RunManager;

#ifdef G4VIS_USE
  // Visualization manager construction
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // mandatory user initialization classes
  A01DetectorConstruction* massWorld = new A01DetectorConstruction;
  if(ifPara) massWorld->RegisterParallelWorld(new A01ParallelWorld("ParallelScoringWorld"));
  runManager->SetUserInitialization(massWorld);
  runManager->SetUserInitialization(new A01PhysicsList(ifPara,ifDisp));

  // initialize Geant4 kernel
  runManager->Initialize();

  // mandatory user action class
  runManager->SetUserAction(new A01PrimaryGeneratorAction);

  // optional user action classes
  runManager->SetUserAction(new A01EventAction(ifPara));

  G4UImanager* UI = G4UImanager::GetUIpointer();  
  if(argc>3)
  // execute an argument macro file if exist
  {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    G4String fileName = argv[3];
    UImanager->ApplyCommand(command+fileName);
  }
  else
  // start interactive session
  {
    UI->ApplyCommand("/control/execute vis.mac");    
    G4UIsession* session = new G4UIterminal(new G4UItcsh);
    session->SessionStart();
    delete session;
  }

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;

  return 0;
}


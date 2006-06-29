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
//
// $Id: G4TrackingManager.cc,v 1.19 2006-06-29 21:16:09 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------
//
// G4TrackingManager.cc
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
//---------------------------------------------------------------

#include "G4TrackingManager.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

//////////////////////////////////////
G4TrackingManager::G4TrackingManager()
//////////////////////////////////////
  : fpUserTrackingAction(NULL), fpTrajectory(NULL),
    StoreTrajectory(false), verboseLevel(0), EventIsAborted(false)
{
  fpSteppingManager = new G4SteppingManager();
  messenger = new G4TrackingMessenger(this);
}

///////////////////////////////////////
G4TrackingManager::~G4TrackingManager()
///////////////////////////////////////
{
  delete messenger;
  delete fpSteppingManager;
  if (fpUserTrackingAction) delete fpUserTrackingAction;
}

////////////////////////////////////////////////////////////////
void G4TrackingManager::ProcessOneTrack(G4Track* apValueG4Track)
////////////////////////////////////////////////////////////////
{

  // Receiving a G4Track from the EventManager, this funciton has the
  // responsibility to trace the track till it stops.
  fpTrack = apValueG4Track;
  EventIsAborted = false;

  // Clear 2ndary particle vector
  //  GimmeSecondaries()->clearAndDestroy();    
  //  std::vector<G4Track*>::iterator itr;
  size_t itr;
  //  for(itr=GimmeSecondaries()->begin();itr=GimmeSecondaries()->end();itr++){ 
  for(itr=0;itr<GimmeSecondaries()->size();itr++){ 
     delete (*GimmeSecondaries())[itr];
  }
  GimmeSecondaries()->clear();  
  
#ifdef G4VERBOSE
                         // !!!!! Verbose
                         if(verboseLevel>0) Verbose("ProcessOneTrack");
#endif

  // Give SteppingManger the pointer to the track which will be tracked 
  fpSteppingManager->SetInitialStep(fpTrack);

  // Pre tracking user intervention process.
  fpTrajectory = 0;
  if( fpUserTrackingAction != NULL ) {
     fpUserTrackingAction->PreUserTrackingAction(fpTrack);
  }
#ifdef G4_STORE_TRAJECTORY
  // Construct a trajectory if it is requested
  if(StoreTrajectory&&(!fpTrajectory)) { 
     fpTrajectory = new G4Trajectory(fpTrack); // default trajectory concrete class object
  }
#endif

  // Give SteppingManger the maxmimum number of processes 
  fpSteppingManager->GetProcessNumber();

  // Give track the pointer to the Step
  fpTrack->SetStep(fpSteppingManager->GetStep());

  // Inform beginning of tracking to physics processes 
  fpTrack->GetDefinition()->GetProcessManager()->StartTracking(fpTrack);

  // Track the particle Step-by-Step while it is alive
  G4StepStatus stepStatus;

  while( (fpTrack->GetTrackStatus() == fAlive) ||
         (fpTrack->GetTrackStatus() == fStopButAlive) ){

    fpTrack->IncrementCurrentStepNumber();
    stepStatus = fpSteppingManager->Stepping();
#ifdef G4_STORE_TRAJECTORY
    if(StoreTrajectory) fpTrajectory->
                        AppendStep(fpSteppingManager->GetStep()); 
#endif
    if(EventIsAborted) {
      fpTrack->SetTrackStatus( fKillTrackAndSecondaries );
    }
  }
  // Inform end of tracking to physics processes 
  fpTrack->GetDefinition()->GetProcessManager()->EndTracking();

  // Post tracking user intervention process.
  if( fpUserTrackingAction != NULL ) {
     fpUserTrackingAction->PostUserTrackingAction(fpTrack);
  }

  // Destruct the trajectory if it was created
#ifdef G4VERBOSE
  if(StoreTrajectory&&verboseLevel>10) fpTrajectory->ShowTrajectory();
#endif
  if( (!StoreTrajectory)&&fpTrajectory ) {
      delete fpTrajectory;
      fpTrajectory = 0;
  }
}

void G4TrackingManager::SetTrajectory(G4VTrajectory* aTrajectory)
{
#ifndef G4_STORE_TRAJECTORY
  G4Exception("G4TrackingManager::SetTrajectory is invoked without G4_STORE_TRAJECTORY compilor option");
#endif
  fpTrajectory = aTrajectory;
}

//////////////////////////////////////
void G4TrackingManager::EventAborted()
//////////////////////////////////////
{
  fpTrack->SetTrackStatus( fKillTrackAndSecondaries );
  EventIsAborted = true;
}

//************************************************************************
//
//  Private Member Functions
//
//************************************************************************


////////////////////////////////////////////////
void G4TrackingManager::Verbose(G4String select)
////////////////////////////////////////////////
{

  // !!!!! Verbose
  if( select == "ProcessOneTrack" ){
#ifdef G4VERBOSE
     if(verboseLevel >= 1) { 
       G4cout << G4endl;
       G4cout << "*******************************************************"
            << "**************************************************"
            << G4endl;
       G4cout << "* G4Track Information: " 
            << "  Particle = " << fpTrack->GetDefinition()->GetParticleName() 
            << "," 
	    << "   Track ID = " << fpTrack->GetTrackID() 
            << "," 
	    << "   Parent ID = " << fpTrack->GetParentID() 
            << G4endl;
       G4cout << "*******************************************************"
            << "**************************************************"
            << G4endl;
       G4cout << G4endl;
      
     }
#endif
  }
}










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
//  Taken from
// $Id: MomoEventAction.cc,v 1.1 2003-11-04 09:02:27 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

 
#include "MomoEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

 
MomoEventAction::MomoEventAction()
{}
 
MomoEventAction::~MomoEventAction()
{}

 
void MomoEventAction::BeginOfEventAction(const G4Event*)
{}

 
void MomoEventAction::EndOfEventAction(const G4Event* evt)
{
  G4int event_id = evt->GetEventID();
  
  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  // periodic printing
  //
  if (event_id < 100 || event_id%100 == 0) {
    G4cout << ">>> Event " << evt->GetEventID() << G4endl;
    G4cout << "    " << n_trajectories 
	   << " trajectories stored in this event." << G4endl;
  }
    
  // extract the trajectories and draw them
  //
  if (G4VVisManager::GetConcreteInstance())
    {
     for (G4int i=0; i<n_trajectories; i++) 
        { G4Trajectory* trj = (G4Trajectory*)
	                            ((*(evt->GetTrajectoryContainer()))[i]);
          trj->DrawTrajectory(50);
        }
    }
}


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
// $Id: Tst52EventAction.cc,v 1.1 2007-04-12 12:00:17 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Susanna Guatelli (guatelli@ge.infn.it)
//
// History:
// -----------
// 17 May  2003   S. Guatelli   1st implementation
//
// -------------------------------------------------------------------
 
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#ifdef G4ANALYSIS_USE
#include "Tst52AnalysisManager.hh"
#endif
#include "Tst52TrackerHit.hh"
#include "Tst52EventAction.hh"

Tst52EventAction::Tst52EventAction()
{ }

Tst52EventAction::~Tst52EventAction()
{ }

void Tst52EventAction::BeginOfEventAction(const G4Event* evt)
{ 
  G4int evtNo = evt -> GetEventID(); 
  G4int printModul = 500;
  if (evtNo%printModul == 0) 
   G4cout << "\n---> Begin Of Event: " << evtNo << G4endl;
  //if (collisionID==-1)
  //  {
  //   G4SDManager * SDman = G4SDManager::GetSDMpointer();
  //    collisionID = SDman->GetCollectionID("Tst52Collection");
  //  }  

  //totalEnergy = 0;
}
 
void Tst52EventAction::EndOfEventAction(const G4Event* evt)
{
 
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  if (G4VVisManager::GetConcreteInstance())
    {
     for (G4int i=0; i<n_trajectories; i++) 
        { 
         G4Trajectory* trj = (G4Trajectory*)
	 ((*(evt->GetTrajectoryContainer()))[i]);
	 trj -> DrawTrajectory(50);
        }
    }
}

G4int Tst52EventAction::GetEventNo()
{
  G4int evno = fpEventManager -> GetConstCurrentEvent() -> GetEventID() ;
  return evno ;
}


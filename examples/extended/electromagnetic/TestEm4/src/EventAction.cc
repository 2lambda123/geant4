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
// $Id: EventAction.cc,v 1.7 2006-06-29 16:53:57 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "RunAction.hh"
#include "EventActionMessenger.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"

#ifdef G4ANALYSIS_USE
  #include "AIDA/IHistogram1D.h"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run)
:Run(run),drawFlag("none"),printModulo(10000),eventMessenger(NULL)
{
  eventMessenger = new EventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  delete eventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction( const G4Event* evt)
{
 G4int evtNb = evt->GetEventID();

 //printing survey
 if (evtNb%printModulo == 0) 
    G4cout << "\n---> Begin of Event: " << evtNb << G4endl;  
 
 //additional initializations   
 TotalEnergyDeposit = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction( const G4Event* evt)
{	 		 
  if (drawFlag != "none") 
    G4cout << " Energy deposit: " 
           << G4BestUnit(TotalEnergyDeposit,"Energy") << G4endl;
	   
#ifdef G4ANALYSIS_USE
  Run->GetHisto(0)->fill(TotalEnergyDeposit/MeV);
#endif

  // draw trajectories
  if(G4VVisManager::GetConcreteInstance())
  {
   G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
   G4int n_trajectories = 0;
   if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();  
   for(G4int i=0; i<n_trajectories; i++) 
      { G4Trajectory* trj = (G4Trajectory *)
                                      ((*(evt->GetTrajectoryContainer()))[i]);
        if (drawFlag == "all") trj->DrawTrajectory(1000);
        else if ((drawFlag == "charged")&&(trj->GetCharge() != 0.))
                               trj->DrawTrajectory(1000); 
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



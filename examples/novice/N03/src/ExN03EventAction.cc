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
// $Id: ExN03EventAction.cc,v 1.20 2003-06-16 16:49:49 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN03EventAction.hh"

#include "ExN03CalorHit.hh"
#include "ExN03EventActionMessenger.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03EventAction::ExN03EventAction()
:calorimeterCollID(-1),drawFlag("all"),printModulo(1),
 eventMessenger(0)
{
  eventMessenger = new ExN03EventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN03EventAction::~ExN03EventAction()
{
  delete eventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03EventAction::BeginOfEventAction(const G4Event* evt)
{
  
 G4int evtNb = evt->GetEventID();
 if (evtNb%printModulo == 0)
   { 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
    HepRandom::showEngineStatus();
   }
    
 if (calorimeterCollID==-1)
   {
    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    calorimeterCollID = SDman->GetCollectionID("CalCollection");
   } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN03EventAction::EndOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  
  // extracted from hits, compute the total energy deposit (and total charged
  // track length) in all absorbers and in all gaps

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  ExN03CalorHitsCollection* CHC = 0;
  G4int n_hit = 0;
  G4double totEAbs=0, totLAbs=0, totEGap=0, totLGap=0;
    
  if (HCE) CHC = (ExN03CalorHitsCollection*)(HCE->GetHC(calorimeterCollID));

  if (CHC)
    {
     n_hit = CHC->entries();
     for (G4int i=0;i<n_hit;i++)
	{
	  totEAbs += (*CHC)[i]->GetEdepAbs(); 
	  totLAbs += (*CHC)[i]->GetTrakAbs();
	  totEGap += (*CHC)[i]->GetEdepGap(); 
	  totLGap += (*CHC)[i]->GetTrakGap();
	}
     }
   
   // print this information event by event (modulo n)  	
	  
  if (evtNb%printModulo == 0) {
    G4cout << "---> End of event: " << evtNb << G4endl;	

    G4cout
       << "   Absorber: total energy: " << std::setw(7)
                                        << G4BestUnit(totEAbs,"Energy")
       << "       total track length: " << std::setw(7)
                                        << G4BestUnit(totLAbs,"Length")
       << G4endl
       << "        Gap: total energy: " << std::setw(7)
                                        << G4BestUnit(totEGap,"Energy")
       << "       total track length: " << std::setw(7)
                                        << G4BestUnit(totLGap,"Length")
       << G4endl;
	  
    G4cout << "\n     " << n_hit
	   << " hits are stored in ExN03CalorHitsCollection." << G4endl;
  }
  
  // extract the trajectories and draw them
  
  if (G4VVisManager::GetConcreteInstance())
    {
     G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
     G4int n_trajectories = 0;
     if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

     for (G4int i=0; i<n_trajectories; i++) 
        { G4Trajectory* trj = (G4Trajectory*)
	                                ((*(evt->GetTrajectoryContainer()))[i]);
          if (drawFlag == "all") trj->DrawTrajectory(50);
          else if ((drawFlag == "charged")&&(trj->GetCharge() != 0.))
                                  trj->DrawTrajectory(50);
          else if ((drawFlag == "neutral")&&(trj->GetCharge() == 0.))
                                  trj->DrawTrajectory(50);
        }
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

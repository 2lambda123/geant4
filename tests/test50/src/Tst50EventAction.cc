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
// $Id: Tst50EventAction.cc,v 1.15 2003-04-28 14:58:56 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "G4UnitsTable.hh" 
#include "Tst50EventAction.hh"
#ifdef G4ANALYSIS_USE
#include "Tst50AnalysisManager.hh"
#endif
#include "Tst50PrimaryGeneratorAction.hh"
#include "Tst50RunAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "Tst50TrackerHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
 
Tst50EventAction::Tst50EventAction(Tst50PrimaryGeneratorAction* Primary):
  hit_CollID(-1),p_Primary(Primary)
{ }

Tst50EventAction::~Tst50EventAction()
{ }

void Tst50EventAction::BeginOfEventAction(const G4Event*)
{ 
  N_Steps=0;
  energyDep=0.;
  energy=0.; 
  energyDepPrimary=0.;
  energyDepSecondary=0.; 
  length=0.;
}
 
void Tst50EventAction::EndOfEventAction(const G4Event* evt)
{

  /*
  G4double initialEnergy=p_Primary->GetInitialEnergy();
	
#ifdef G4ANALYSIS_USE
  if (Foil)
    {
      Tst50AnalysisManager* analysis = Tst50AnalysisManager::getInstance();
	    
      analysis->energy_deposit(energyDep);
            
      analysis->energy_depositPrimary(energyDepPrimary);
           
      analysis->energy_depositSecondary(energyDepSecondary);

      analysis->Steps_Num(N_Steps);
      analysis->track_length(length);		
    }
#endif

  
    G4cout<<"energia iniziale in MeV:"<<initialEnergy/MeV<<G4endl;
    G4cout<<"energia in MeV:"<<energy/MeV<<G4endl;
    G4double radiation=(energy/initialEnergy);
    G4cout<<"Radiation yield:"<< radiation<<G4endl;
 
 
  if (RadiationY)
    {
      G4std::ofstream pmtfile(filename, G4std::ios::app);
      G4double radiation=(energy/MeV)/(initialEnergy/MeV);
      G4cout<<"Radiation yield:"<< radiation<<G4endl;
      if(pmtfile.is_open()){
	pmtfile<<'\t'<<radiation<<'\t'<<'\t'<<initialEnergy/MeV<<G4endl;
      }
    }
  */
  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  // periodic printing
  //
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
G4int Tst50EventAction::GetEventno()
{
  G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID() ;
  return evno ;
}


G4double Tst50EventAction::RadiationYield(G4double energyLost)
{
  energy += energyLost;

  // ---- MGP ---- Added the following line, since the function, 
  // ---- MGP ---- which expected to return a double, did not return anything;
  // ---- MGP ---- is it correct?
  return energy;
}

void Tst50EventAction::CalculateEnergyDeposit(G4double deposit)
{
  energyDep += deposit;
}

void Tst50EventAction::CalculateEnergyDepositPrimary(G4double deposit_primary)
{
  energyDepPrimary += deposit_primary;
}

void Tst50EventAction::CalculateEnergyDepositSecondary(G4double deposit_secondary)
{
  energyDepSecondary += deposit_secondary;
}

void Tst50EventAction::Number_Steps()
{
  N_Steps += 1;
}

void Tst50EventAction::Track_length(G4double step_dl)
{
  length += step_dl;
}

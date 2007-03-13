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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
// 
#include "G4HumanPhantomEventAction.hh"
#include "G4HumanPhantomHit.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4HumanPhantomRunAction.hh"
#include "G4RunManager.hh"

G4HumanPhantomEventAction::G4HumanPhantomEventAction():
hitCollectionID(-1)
{ 
 
}
 
G4HumanPhantomEventAction::~G4HumanPhantomEventAction()
{
}

void G4HumanPhantomEventAction::BeginOfEventAction(const G4Event*)
{
 energyTotal["logicalHead"]=0.;
 energyTotal["logicalTrunk"]=0.;
 energyTotal["logicalLeftLeg"]=0.;
 energyTotal["logicalRightLeg"]=0.;
 energyTotal["logicalBrain"]=0.;
 energyTotal["logicalLeftArmBone"]=0.;
 energyTotal["logicalRightArmBone"]=0.;
 energyTotal["logicalSkull"]=0.;
 energyTotal["logicalUpperSpine"]=0.;
 energyTotal["logicalMiddleLowerSpine"]=0.;
 energyTotal["logicalPelvis"]=0.;
 energyTotal["logicalStomach"]=0.;
 energyTotal["logicalUpperLargeIntestine"]=0.;
 energyTotal["logicalLowerLargeIntestine"]=0.;
 energyTotal["logicalRibCage"]=0.;
 energyTotal["logicalSpleen"]=0.;
 energyTotal["logicalPancreas"]=0.;
 energyTotal["logicalLeftKidney"]=0.;
 energyTotal["logicalRightKidney"]=0.;
 energyTotal["logicalUrinaryBladder"]=0.;
 energyTotal["logicalUterus"]=0.;
 energyTotal["logicalLeftLung"]=0.;
 energyTotal["logicalRightLung"]=0.;
 energyTotal["logicalLeftOvary"]=0.;
 energyTotal["logicalRightOvary"]=0.;
 energyTotal["logicalLeftLegBone"]=0.;
 energyTotal["logicalRightLegBone"]=0.;
 energyTotal["logicalLeftBreast"]=0.;
 energyTotal["logicalRightBreast"]=0.; 
  
 G4SDManager * SDman = G4SDManager::GetSDMpointer();  

  if (hitCollectionID==-1) {
    hitCollectionID = SDman->GetCollectionID("HumanPhantomCollection");
  }
}
 
void G4HumanPhantomEventAction::EndOfEventAction(const G4Event* evt)
{  

 G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
 
 G4HumanPhantomHitsCollection* HC = 0;

 if (HCE)
     HC = (G4HumanPhantomHitsCollection*)(HCE->GetHC(hitCollectionID));

 if (HC)
	{
	  G4int hitNumber = HC->entries();
          G4double edep =0;
          G4String bodyPart;
          for (G4int i=0;i<hitNumber;i++) 
	    {
	      edep = (*HC)[i]->GetEdep();
	      bodyPart = (*HC)[i]->GetBodyPartID();
              Fill(bodyPart, edep);
	      //	      if(edep !=0.) G4cout << bodyPart <<": "<< edep/MeV << G4endl; 
	    }
	}

 totalEventEnergyDeposit();


  // Visualization of particle tracks
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  if (G4VVisManager::GetConcreteInstance())
    {
     for (G4int i=0; i<n_trajectories; i++) 
        { G4Trajectory* trj = (G4Trajectory*)
	                            ((*(evt->GetTrajectoryContainer()))[i]);
          trj->DrawTrajectory(500);
        }
    } 
}

void G4HumanPhantomEventAction:: Fill(G4String bodypartName, 
				      G4double energyDeposit)

{
 energyTotal[bodypartName] += energyDeposit;
}

void G4HumanPhantomEventAction::totalEventEnergyDeposit() 
{

 G4RunManager* runManager = G4RunManager::GetRunManager();
 G4HumanPhantomRunAction* pointerRun = (G4HumanPhantomRunAction*)(runManager->GetUserRunAction());

 std::map<std::string,G4double>::iterator i = energyTotal.begin();
  std::map<std::string,G4double>::iterator end = energyTotal.end();

  while(i!=end)
    {

      G4String bodypart = i->first;
      G4double energyDep = i->second;
      
      if(energyDep != 0.)
	{
	  pointerRun->Fill(bodypart, energyDep);
	}
      i++;
    }
  
}

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
// $Id: Tst52TrackerSD.cc,v 1.1 2007-04-12 12:00:17 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Author: Susanna Guatelli (guatelli@ge.infn.it)
//
// History:
// -----------
// 17 May     2003 SG      first implemntation
// -------------------------------------------------------------------

#include "Tst52TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "Tst52DetectorConstruction.hh"
#include "Tst52AnalysisManager.hh"
#include "Tst52TrackerHit.hh"

Tst52TrackerSD::Tst52TrackerSD(G4String name,
			       Tst52DetectorConstruction* det)
  :G4VSensitiveDetector(name),Detector(det)
{   
  G4String HCname;
  collectionName.insert(HCname="trackerCollection"); 
  totalEnergyDeposit =0;   
}

Tst52TrackerSD::~Tst52TrackerSD()
{
}

void Tst52TrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  //G4cout <<  SensitiveDetectorName << G4endl;
  //G4cout <<  collectionName[0] << G4endl;
 trackerCollection = new Tst52TrackerHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
  static G4int HCID = -1;
  if(HCID<0)
  {HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE -> AddHitsCollection( HCID, trackerCollection ); 
  totalEnergyDeposit = 0;

}

G4bool Tst52TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist)
{
if(!ROhist)
    return false;

  // Retrieve the energy deposit in the target
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;

if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() != "Target")
 return false;

 
#ifdef G4ANALYSIS_USE
//if(aStep -> GetTrack() -> GetTrackID()== 1)
//   { 
  i = ROhist -> GetReplicaNumber();

  //G4double xx = aStep -> GetPreStepPoint() -> GetPosition().x();
  //G4double yy = aStep -> GetPreStepPoint() -> GetPosition().y();
    G4double zz = aStep -> GetPreStepPoint() -> GetPosition().z();
 
  G4double voxelWidthZ = targetZ/numberOfVoxelZ;
  G4double z = (- numberOfVoxelZ+1+2*i)* voxelWidthZ/2;

      G4cout << "Energy deposit (MeV): "<< edep/MeV
  << " in (" << zz/mm << ")" << " mm"<<G4endl;
   
  //    G4cout<< "ReadOut:" << edep/MeV <<" in ( " << z/mm << " )"
  //  << " mm"<<" i: "<< i <<G4endl;          
 
  Tst52TrackerHit* newHit = new Tst52TrackerHit();
  newHit->SetEdep(edep/MeV);//Energy deposit in the voxel i
  newHit->SetPos(i); // Voxel
  trackerCollection->insert( newHit );
  
 Tst52AnalysisManager* analysis = Tst52AnalysisManager::getInstance();
 analysis -> FillEnergyDeposit(z/mm, edep/MeV);
 //   }
#endif 

 //newHit->Print();
  
  return true;
}

void Tst52TrackerSD::EndOfEvent(G4HCofThisEvent*)
{ 
  //static G4int HCID = -1;
  //if(HCID<0)
  //  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  // HCE->AddHitsCollection(HCID,tst51Collection);

   G4int NbHits = trackerCollection->entries();
   //G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
   //        <<G4endl;

   for (G4int i=0;i<NbHits;i++)
     {
       //(*trackerCollection)[i]->Print();
       totalEnergyDeposit =  totalEnergyDeposit + (*trackerCollection)[i]-> GetEdep();  
     };
#ifdef G4ANALYSIS_USE    
 Tst52AnalysisManager* analysis = Tst52AnalysisManager::getInstance();
 analysis -> landau(totalEnergyDeposit/keV);
#endif
 // G4cout<< "Total energy deposit of the event: "<< totalEnergyDeposit/keV << G4endl;
}

void Tst52TrackerSD::ChangeVoxelNumber(G4int newNumber)
{
  numberOfVoxelZ = newNumber;
}

void Tst52TrackerSD::SetSDParameters( G4double ZSize,
			               G4int voxelZ)
{
 numberOfVoxelZ = voxelZ; 
 targetZ = ZSize;
}

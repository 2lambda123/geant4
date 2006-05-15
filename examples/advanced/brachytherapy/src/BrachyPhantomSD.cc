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
// Code developed by:
// S. Agostinelli, F. Foppiano, S. Garelli , M. Tropeano, S.Guatelli
//
//    ********************************
//    *                              *  
//    *    BrachyPhantomSD.cc       *
//    *                              *
//    ********************************
//
// $Id: BrachyPhantomSD.cc,v 1.10 2006-05-15 08:26:54 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "BrachyPhantomSD.hh"
#include "BrachyAnalysisManager.hh"
#include "BrachyDetectorConstruction.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"

BrachyPhantomSD::BrachyPhantomSD(G4String name):G4VSensitiveDetector(name)
{
}

BrachyPhantomSD::~BrachyPhantomSD()
{
  
}

void BrachyPhantomSD::Initialize(G4HCofThisEvent* HCE)
{
}

G4bool BrachyPhantomSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
  //
  // The energy deposit of the hits is stored in histograms and ntuples
  //

  if(!ROhist)
    return false;
  
  // Check the volume
  if(aStep -> GetPreStepPoint() -> GetPhysicalVolume() -> GetName() != "PhantomPhys")
    return false;

  // 
  G4double energyDeposit = aStep -> GetTotalEnergyDeposit();

  // Check that the energy deposit is not null
  if(energyDeposit == 0.)
    return false;
          
  if(energyDeposit != 0)                       
	    {            
	      // Read Voxel indexes: i is the x index, k is the z index
	      G4int j = ROhist -> GetReplicaNumber();
	      G4int k = ROhist -> GetReplicaNumber(1);
	      G4int i = ROhist -> GetReplicaNumber(2);
  
	      G4int numberOfVoxel = 300;
	      G4double voxelWidth = 1. *mm;
	
              // Retrieve the coordinates of the center of the voxel where
              // the energy deposit is located
	      G4double x = ( - numberOfVoxel + 1+ 2*i )* voxelWidth/2; 
	      G4double y = ( - numberOfVoxel + 1+ 2*j )* voxelWidth/2;
	      G4double z = ( - numberOfVoxel + 1+ 2*k )* voxelWidth/2;

#ifdef G4ANALYSIS_USE	
	      BrachyAnalysisManager* analysis = 
		BrachyAnalysisManager::getInstance();   
             
              // Fill the ntuple with position and energy deposit in the phantom
	      analysis -> FillNtupleWithEnergy(x,y,z,energyDeposit/MeV);

	      if (y < 0.8 * mm && y > -0.8 * mm)
		{ 
                  // Fill a 2D histogram with the energy deposit in the plane
                  // containing the source
		  analysis -> FillHistogramWithEnergy(x,z,energyDeposit/MeV);
   
                  //  Fill 1D histogram with the energy deposit 
                  // along the axis perpendicular to the main axis of the source
		  if (z < 0.8 * mm && z > -0.8 * mm)                
		    analysis -> DoseDistribution(x,energyDeposit/MeV);
		}
#endif  
	    }
  return true;
}

void BrachyPhantomSD::EndOfEvent(G4HCofThisEvent*)
{
}

void BrachyPhantomSD::clear()
{
} 

void BrachyPhantomSD::DrawAll()
{
}

void BrachyPhantomSD::PrintAll()
{
}




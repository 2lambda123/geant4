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
// $Id: G4VoxelNavigation.hh,v 1.9 2001-07-11 10:00:31 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// class G4VoxelNavigation
//
// Class description:
//
// Utility for navigation in volumes containing only G4PVPlacement
// daughter volumes for which voxels have been constructed.

// History:
// - Created. Paul Kent, Aug 96

#ifndef G4VOXELNAVIGATION_HH
#define G4VOXELNAVIGATION_HH

#include "geomdefs.hh"
#include "G4NavigationHistory.hh"
#include "G4AffineTransform.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"

#include "G4BlockingList.hh"

// Required for voxel handling & voxel stack
#include "G4SmartVoxelProxy.hh"
#include "G4SmartVoxelNode.hh"
#include "G4SmartVoxelHeader.hh"
#include "g4std/vector"

class G4VoxelNavigation
{
  public:  // with description

	G4VoxelNavigation();
        ~G4VoxelNavigation();
	G4SmartVoxelNode* VoxelLocate(G4SmartVoxelHeader *pHead,
                           const G4ThreeVector &localPoint);
	G4bool LevelLocate(G4NavigationHistory& history,
			   const G4VPhysicalVolume *blockedVol,
                           const G4int blockedNum,
                           const G4ThreeVector &globalPoint,
                           const G4ThreeVector* globalDirection,
			   const G4bool pLocatedOnEdge, 
			   G4ThreeVector &localPoint);

	G4double ComputeStep(const G4ThreeVector &globalPoint,
			     const G4ThreeVector &globalDirection,
			     const G4double currentProposedStepLength,
			     G4double &newSafety,
			     G4NavigationHistory &history,
			     G4bool &validExitNormal,
			     G4ThreeVector &exitNormal,
			     G4bool &exiting,
			     G4bool &entering,
                             G4VPhysicalVolume *(*pBlockedPhysical),
                             G4int &blockedReplicaNo);

        G4double ComputeSafety(const G4ThreeVector &globalpoint,
			       const G4NavigationHistory &history,
    		               const G4double pMaxLength=DBL_MAX );
  private:

	G4double ComputeVoxelSafety(const G4ThreeVector &localPoint) const;
	G4bool LocateNextVoxel(const G4ThreeVector &localPoint,
			const G4ThreeVector& localDirection,
		        const G4double currentStep);
	G4bool VoxelSubLevelSetup(const G4ThreeVector& pLoc,
			G4SmartVoxelHeader *pHeader,
			G4int pDepth);

    G4BlockingList fBList;	// Blocked volumes

    //
    //  BEGIN Voxel Stack information
    //

    G4int fVoxelDepth;
      // Note: fVoxelDepth==0+ => fVoxelAxisStack(0+) contains axes of voxel
      //       fVoxelDepth==-1 -> not in voxel

    G4std::vector<EAxis> fVoxelAxisStack;
      // Voxel axes

    G4std::vector<G4int> fVoxelNoSlicesStack;
      // No slices per voxel at each level

    G4std::vector<G4double> fVoxelSliceWidthStack; 
      // Width of voxels at each level 

    G4std::vector<G4int> fVoxelNodeNoStack;	  
      // Node no point is inside at each level 
				
    G4std::vector<G4SmartVoxelHeader*> fVoxelHeaderStack;
      // Voxel headers at each level

    G4SmartVoxelNode* fVoxelNode;
      // Node containing last located point

    //
    //  END Voxel Stack information
    //

};

#include "G4AuxiliaryNavServices.hh"   // Needed for inline implementation

#include "G4VoxelNavigation.icc"

#endif

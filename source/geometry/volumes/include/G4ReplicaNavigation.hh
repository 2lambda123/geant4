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
// $Id: G4ReplicaNavigation.hh,v 1.4 2001-07-11 10:00:30 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// class G4ReplicaNavigation
//
// Class description:
//
// Utility for navigation in volumes containing a single G4PVParameterised
// volume for which voxels for the replicated volumes have been constructed.
// [Voxels MUST be along one axis only: NOT refined]

// History:
// - Created. Paul Kent, Aug 96

#ifndef G4REPLICANAVIGATION_HH
#define G4REPLICANAVIGATION_HH

#include "globals.hh"
#include "G4NavigationHistory.hh"
#include "G4AffineTransform.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"

#include "G4BlockingList.hh"

// Required for voxel handling
#include "G4SmartVoxelProxy.hh"
#include "G4SmartVoxelNode.hh"
#include "G4SmartVoxelHeader.hh"

class G4ReplicaNavigation
{
  friend class G4ReplicaNavigationTester;

 public:  // with description

  G4ReplicaNavigation();
  G4bool LevelLocate(G4NavigationHistory& history,
		     const G4VPhysicalVolume *blockedVol,
		     const G4int blockedNum,
		     const G4ThreeVector &globalPoint,
		     const G4ThreeVector* globalDirection,
		     const G4bool pLocatedOnEdge, 
		     G4ThreeVector &localPoint);

  G4double ComputeStep(const G4ThreeVector &globalPoint,
		       const G4ThreeVector &globalDirection,
		       const G4ThreeVector &localPoint,
		       const G4ThreeVector &localDirection,
		       const G4double currentProposedStepLength,
		       G4double &newSafety,
		       G4NavigationHistory &history,
		       G4bool &validExitNormal,
		       G4ThreeVector &exitNormal,
		       G4bool &exiting,
		       G4bool &entering,
		       G4VPhysicalVolume *(*pBlockedPhysical),
		       G4int &blockedReplicaNo);

  G4double ComputeSafety(const G4ThreeVector &globalPoint,
			 const G4ThreeVector &localPoint,
			       G4NavigationHistory &history, // -> NON-CONST
		      // const G4NavigationHistory &history, // -> NON-CONST
			 const G4double pProposedMaxLength=DBL_MAX );

  EInside BackLocate(G4NavigationHistory &history,
		     const G4ThreeVector &globalPoint,
		     G4ThreeVector &localPoint,
		     const G4bool &exiting,
		     G4bool &notKnownInside) const;

  void ComputeTransformation(const G4int replicaNo,
			G4VPhysicalVolume *pVol,
			G4ThreeVector &point) const; 
  void ComputeTransformation(const G4int replicaNo,
			G4VPhysicalVolume *pVol) const; 

  EInside Inside(const G4VPhysicalVolume *pVol,
		 const G4int replicaNo,
		 const G4ThreeVector &localPoint) const;
  G4double DistanceToOut(const G4VPhysicalVolume *pVol,
			 const G4int replicaNo,
			 const G4ThreeVector &localPoint) const;
  G4double DistanceToOut(const G4VPhysicalVolume *pVol,
			 const G4int replicaNo,
			 const G4ThreeVector &localPoint,
			 const G4ThreeVector &localDirection) const;

 private:

  G4int VoxelLocate(const G4SmartVoxelHeader *pHead,
		    const G4ThreeVector &localPoint,
		    const G4int blocked=-1) const;

  G4double DistanceToOutPhi(const G4ThreeVector &localPoint,
			    const G4ThreeVector &localDirection,
			    const G4double width) const;

  G4double DistanceToOutRad(const G4ThreeVector &localPoint,
			    const G4ThreeVector &localDirection,
			    const G4double width,
			    const G4double offset,
			    const G4int replicaNo) const;
  void SetPhiTransformation(const G4double ang,
			    G4VPhysicalVolume *pVol=0) const;
};

#include "G4ReplicaNavigation.icc"

#endif

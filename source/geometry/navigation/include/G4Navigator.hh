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
// $Id: G4Navigator.hh,v 1.1 2003-10-01 15:09:31 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4Navigator
//
// Class description:
//
// A class for use by the tracking management, able to obtain/calculate
// dynamic tracking time information such as the distance to the next volume,
// or to find the physical volume containing a given point in the world
// reference system. The navigator maintains a transformation history and
// other information to optimise the tracking time performance.
//
// NOTES:
//
// The following methods provide detailed information when a Step has
// arrived at a geometrical boundary.  They distinguish between the different
// causes that can result in the track leaving its current volume.
//
// Four cases are possible:
//
// 1) The particle has reached a boundary of a daughter of the current volume:
//     (this could cause the relocation to enter the daughter itself
//     or a potential granddaughter or further descendant)
//     
// 2) The particle has reached a boundary of the current
//     volume, exiting into a mother (regardless the level
//     at which it is located in the tree):
//
// 3) The particle has reached a boundary of the current
//     volume, exiting into a volume which is not in its
//     parental hierarchy:
//
// 4) The particle is not on a boundary between volumes:
//     the function returns an exception, and the caller is
//     reccomended to compare the G4touchables associated
//     to the preStepPoint and postStepPoint to handle this case.
//
//   G4bool        EnteredDaughterVolume()
//   G4bool        IsExitNormalValid()
//   G4ThreeVector GetLocalExitNormal()
//
// The expected usefulness of these methods is to allow the caller to
// determine how to compute the surface normal at the volume boundary. The two
// possibilities are to obtain the normal from:
//
//   i) the solid associated with the volume of the initial point of the Step.
//      This is valid for cases 2 and 3.  
//      (Note that the initial point is generally the PreStepPoint of a Step).
//   or
// 
//  ii) the solid of the final point, ie of the volume after the relocation.
//      This is valid for case 1.
//      (Note that the final point is generally the PreStepPoint of a Step).
//
// This way the caller can always get a valid normal, pointing outside
// the solid for which it is computed, that can be used at his own
// discretion.

// History:
// - Created. Paul Kent, July 95/96
// ********************************************************************

#ifndef G4NAVIGATOR_HH
#define G4NAVIGATOR_HH

#include "geomdefs.hh"

#include "G4ThreeVector.hh"
#include "G4AffineTransform.hh"
#include "G4RotationMatrix.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"

#include "G4GRSVolume.hh"
#include "G4GRSSolid.hh"
#include "G4TouchableHandle.hh"
#include "G4TouchableHistoryHandle.hh"

#include "G4NavigationHistory.hh"
#include "G4NormalNavigation.hh"
#include "G4VoxelNavigation.hh"
#include "G4ParameterisedNavigation.hh"
#include "G4ReplicaNavigation.hh"

#include <iostream>

class G4Navigator
{
  public:  // with description

  friend std::ostream& operator << (std::ostream &os, const G4Navigator &n);

  G4Navigator();
    // Constructor - initialisers and setup.

  ~G4Navigator();
    // Destructor. No actions.

  G4double ComputeStep(const G4ThreeVector &pGlobalPoint,
                       const G4ThreeVector &pDirection,
                       const G4double pCurrentProposedStepLength,
                             G4double  &pNewSafety);
    // Calculate the distance to the next boundary intersected
    // along the specified NORMALISED vector direction and
    // from the specified point in the global coordinate
    // system. LocateGlobalPointAndSetup or LocateGlobalPointWithinVolume 
    // must have been called with the same global point prior to this call.
    // The isotropic distance to the nearest boundary is also
    // calculated (usually an underestimate). The current
    // proposed Step length is used to avoid intersection
    // calculations: if it can be determined that the nearest
    // boundary is >pCurrentProposedStepLength away, kInfinity
    // is returned together with the computed isotropic safety
    // distance. Geometry must be closed.

  inline
  G4VPhysicalVolume* LocateGlobalPointAndSetup(const G4ThreeVector &point,
                                               const G4ThreeVector &direction,
                                               const G4TouchableHistory &h);

  G4VPhysicalVolume* LocateGlobalPointAndSetup(const G4ThreeVector& point,
                                             const G4ThreeVector* direction=0,
                                             const G4bool pRelativeSearch=true,
                                             const G4bool ignoreDirection=true);
    // Search the geometrical hierarchy for the volumes deepest in the hierarchy
    // containing the point in the global coordinate space. Two main cases are:
    //  i) If pRelativeSearch=false it makes use of no previous/state
    //     information. Returns the physical volume containing the point, 
    //     with all previous mothers correctly set up.
    // ii) If pRelativeSearch is set to true, the search begin is the
    //     geometrical hierarchy at the location of the last located point,
    //     or the endpoint of the previous Step if SetGeometricallyLimitedStep()
    //     has been called immediately before.
    // The direction is used (to check if a volume is entered) if either
    //   - the argument ignoreDirection is false, or
    //   - the Navigator has determined that it is on an edge shared by two or
    //     more volumes.  (This is state information.)
    // 
    // Important Note: In order to call this the geometry MUST be closed.

  void LocateGlobalPointWithinVolume(const G4ThreeVector& position);
    // Notify the Navigator that a track has moved to the new Global point
    // 'position', that is known to be within the current safety.
    // No check is performed to ensure that it is within  the volume. 
    // This method can be called instead of LocateGlobalPointAndSetup ONLY if
    // the caller is certain that the new global point (position) is inside the
    // same volume as the previous position.  Usually this can be guaranteed
    // only if the point is within safety.

  inline void LocateGlobalPointAndUpdateTouchableHandle(
                const G4ThreeVector&       position,
                const G4ThreeVector&       direction,
                      G4TouchableHandle&   oldTouchableToUpdate,
                const G4bool               RelativeSearch = true);
    // First, search the geometrical hierarchy like the above method
    // LocateGlobalPointAndSetup(). Then use the volume found and its
    // navigation history to update the touchable.

  inline void LocateGlobalPointAndUpdateTouchable(
                const G4ThreeVector&       position,
                const G4ThreeVector&       direction,
                      G4VTouchable*        touchableToUpdate,
                const G4bool               RelativeSearch = true);
    // First, search the geometrical hierarchy like the above method
    // LocateGlobalPointAndSetup(). Then use the volume found and its
    // navigation history to update the touchable.

  inline void LocateGlobalPointAndUpdateTouchable(
                const G4ThreeVector&       position,
                      G4VTouchable*        touchableToUpdate,
                const G4bool               RelativeSearch = true);
    // Old version (missing direction).
    // Not recommended replace with newer version above.

  inline void SetGeometricallyLimitedStep();
    // Inform the navigator that the previous Step calculated
    // by the geometry was taken in its entirety.

  G4double ComputeSafety(const G4ThreeVector &globalpoint,
                         const G4double pProposedMaxLength = DBL_MAX);
    // Calculate the isotropic distance to the nearest boundary from the
    // specified point in the global coordinate system. 
    // The globalpoint utilised must be within the current volume.
    // The value returned is usually an underestimate.  
    // The proposed maximum length is used to avoid volume safety
    // calculations.  The geometry must be closed.

  inline G4ThreeVector GetCurrentLocalCoordinate() const;
    // Return the local coordinate of the point in the reference system
    // of its containing volume that was found by LocalGlobalPointAndSetup.
    // The local coordinate of the last located track.

  inline G4ThreeVector ComputeLocalPoint(const G4ThreeVector& rGlobPoint) const;
    // Return position vector in local coordinate system, given a position
    // vector in world coordinate system.

  inline G4ThreeVector ComputeLocalAxis(const G4ThreeVector& pVec) const;
    // Return the local direction of the specified vector in the reference
    // system of the volume that was found by LocalGlobalPointAndSetup.
    // The Local Coordinates of point in world coordinate system.

  inline G4ThreeVector NetTranslation() const;
  inline G4RotationMatrix NetRotation() const;
    // Compute+return the local->global translation/rotation of current volume.

  inline G4VPhysicalVolume* GetWorldVolume() const;
    // Return the current  world (`topmost') volume.

  inline void SetWorldVolume(G4VPhysicalVolume* pWorld);
    // Set the world (`topmost') volume. This must be positioned at
    // origin (0,0,0) and unrotated.

  inline G4GRSVolume* CreateGRSVolume() const;
  inline G4GRSSolid* CreateGRSSolid() const; 
  inline G4TouchableHistory* CreateTouchableHistory() const;
    // `Touchable' creation methods: caller has deletion responsibility.

  G4TouchableHistoryHandle CreateTouchableHistoryHandle() const;
    // `Touchable' creation methods: caller has deletion responsibility.

  inline G4bool IsExitNormalValid();
    // Return true if the Navigator
    //   i) found that it is exiting the previous volume and is not
    //      entering a daughter volume.
    //  ii) has obtained the normal for the previous volume (in local
    //      coordinates).

  inline G4ThreeVector GetLocalExitNormal();
    // Can be called (i.e. can return a valid result) only if the Navigator
    // replied true to IsExitNormalValid().
    // It returns the ExitNormal of the previous volume.
    // (The normal is in the coordinate system of the final volume.)
    // It is in the coordinate system of the final volume.

  G4ThreeVector GetLocalExitNormal(G4bool* valid);
    // More powerful calculation of Exit Surface Normal, returns validity too.
    // But it can only be called if the Navigator's last Step has crossed a
    // volume geometrical boundary.
    // It returns the Normal to the surface pointing out of the volume that
    // was left behind and/or into the volume that was entered.
    // (The normal is in the coordinate system of the final volume.)
    // This function takes full care about how to calculate this normal,
    // but if the surfaces are not convex it will return valid=false.

  inline G4bool EnteredDaughterVolume();
    // The purpose of this function is to inform the caller if the track is
    // entering a daughter volume while exiting from the current volume.
    // This method returns 
    // - True only in case 1) above, that is when the Step has caused
    //   the track to arrive at a boundary of a daughter.
    // - False in cases 2), 3) and 4), i.e. in all other cases.
    // This function is not guaranteed to work if SetGeometricallyLimitedStep()
    // was not called when it should have been called.

  inline G4int GetVerboseLevel();
  inline void  SetVerboseLevel(G4int level);
    // Get/Set Verbose(ness) level.
    // [if level>0 && G4VERBOSE, printout can occur]

  void PrintState();      
    // Print the internal state of the Navigator (for debugging).
    // The level of detail is according to the verbosity.

  inline const G4AffineTransform& GetGlobalToLocalTransform() const;
  inline const G4AffineTransform  GetLocalToGlobalTransform() const;
    // Obtain the transformations Global/Local (and inverse).
    // Clients of these methods must copy the data if they need to keep it.

 protected:  // with description

  inline void ResetState();
  inline void ResetStackAndState();
    // Reset stack and minimum or navigator state machine necessary for reset
    // as needed by LocalGlobalPointAndSetup.
    // [Does not perform clears, resizes, or reset fLastLocatedPointLocal]

  inline EVolume VolumeType(const G4VPhysicalVolume *pVol) const;
    // Characterise `type' of volume - normal/replicated/parameterised.

  inline EVolume CharacteriseDaughters(const G4LogicalVolume *pLog) const;
    // Characterise daughter of logical volume.

  void SetupHierarchy();
    // Renavigate & reset hierarchy described by current history
    // o Reset volumes
    // o Recompute transforms and/or solids of replicated/parameterised
    //   volumes.

 private:

  //
  // BEGIN State information
  //

  G4ThreeVector fLastLocatedPointLocal;
    // Position of the last located point relative to its containing volume.

  G4bool fWasLimitedByGeometry;
    // Set true if last Step was limited by geometry.
  G4bool fEntering,fExiting;
    // Entering/Exiting volumes blocking/setup
    // o If exiting
    //      volume ptr & replica number (set & used by Locate..())
    //      used for blocking on redescent of geometry
    // o If entering
    //      volume ptr & replica number (set by ComputeStep(),used by
    //      Locate..()) of volume for `automatic' entry

  G4VPhysicalVolume *fBlockedPhysicalVolume;
  G4int fBlockedReplicaNo;

  G4VPhysicalVolume *fCandidatePhysicalVolume;
  G4int fCandidateReplicaNo;

  G4bool fEnteredDaughter;    // A memory of whether in this Step a daughter
                              //  volume is entered (set in Compute & Locate)
                              //  After Compute: it expects to enter a daughter
                              //  After Locate:  it has entered a daughter
  G4bool fExitedMother;       // A similar memory whether the Step exited 
                              //  current "mother" volume completely, 
                              //  not entering daughter.
  
  G4bool fValidExitNormal;    // Set true if have leaving volume normal
  G4ThreeVector fExitNormal;  // Leaving volume normal, in the
                              // volume containing the exited
                              // volume's coordinate system
  G4ThreeVector fGrandMotherExitNormal;  // Leaving volume normal, in its 
                                         // own coordinate system
  G4NavigationHistory fHistory;
                                // Transformation & `path' history
                                // of current path through geomtrical
                                // hierarchy

  G4bool  fLastStepWasZero;
                                // Whether the last ComputeStep moved Zero
                                //  Used to check for edges.
  G4bool  fLocatedOnEdge;       
                                // Whether the Navigator has detected an edge

  G4ThreeVector  fPreviousSftOrigin;
  G4double       fPreviousSafety; 
                                // Memory of last safety origin & value.
                                //  Used in ComputeStep to ensure that
                                //  origin of current Step is in the same
                                //  volume as the point of the last relocation.
  //
  // END State information
  //

  //
  // BEGIN Tracking Invariants
  //

  G4VPhysicalVolume  *fTopPhysical;
    // A link to the topmost physical volume in the detector.
    // Must be positioned at the origin and unrotated.

  //
  // END Tracking Invariants
  //

  //
  // BEGIN Utility information
  //

  G4int  fVerbose;
    // Verbose(ness) level  [if > 0, printout can occur].

  //
  // END Utility Invariants
  //

  // 
  // Helpers/Utility classes
  //
  G4NormalNavigation  fnormalNav;
  G4VoxelNavigation fvoxelNav;
  G4ParameterisedNavigation fparamNav;
  G4ReplicaNavigation freplicaNav;
};

#include "G4Navigator.icc"

#endif

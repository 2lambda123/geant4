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
// $Id: G4PathFinder.hh,v 1.23 2007-05-16 15:59:06 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// class G4PathFinder 
//
// Class description:
// 
// This class directs the lock-stepped propagation of a track in the 
// 'mass' and other parallel geometries.  It ensures that tracking 
// in a magnetic field sees these parallel geometries at each trial step, 
// and that the earliest boundary limits the step.
// 
// For the movement in field, it relies on the class G4PropagatorInField
//
// History:
// -------
//  7.10.05 John Apostolakis,  Draft design 
// 26.04.06 John Apostolakis,  Revised design and first implementation 
// ---------------------------------------------------------------------------
#ifndef G4PATHFINDER_HH 
#define G4PATHFINDER_HH  1

#include <vector>
#include "G4Types.hh"

#include "G4FieldTrack.hh"

class G4TransportationManager; 
class G4Navigator;

#include "G4TouchableHandle.hh"
#include "G4FieldTrack.hh"
#include "G4MultiNavigator.hh"

class G4PropagatorInField;

class G4PathFinder
{

 public:  // with description

   static G4PathFinder* GetInstance();
     //
     // Retrieve singleton instance

   G4double ComputeStep( const G4FieldTrack &pFieldTrack,// Or update non-c
                         G4double            pCurrentProposedStepLength,
                         G4int               navigatorId, 
                         G4int               stepNo,     // See next step/check 
                         G4double           &pNewSafety, // for this geom 
                         ELimited           &limitedStep, 
                         G4FieldTrack       &EndState, 
                         G4VPhysicalVolume*  currentVolume );
     //
     // Compute the next geometric Step  -- Curved or linear

   void Locate( const G4ThreeVector& position, 
                const G4ThreeVector& direction,
                      G4bool  relativeSearch=true); 
     //
     // Make primary relocation of global point in all navigators,
     // and update them.

   void ReLocate( const G4ThreeVector& position ); 
     //
     // Make secondary relocation of global point (within safety only) 
     // in all navigators, and update them.

   void PrepareNewTrack( G4ThreeVector position, G4ThreeVector direction); 
     //
     // Check and cache set of active navigators.

   G4TouchableHandle CreateTouchableHandle( G4int navId ) const;
   inline G4VPhysicalVolume* GetLocatedVolume( G4int navId ) const; 

   // -----------------------------------------------------------------

   inline void SetChargeMomentumMass( G4double charge,     // in e+ units
                                      G4double momentum,   // in Geant4 units
                                      G4double pMass );  

   inline G4bool   IsParticleLooping() const;

   inline G4double GetCurrentSafety() const;
     //
     // Minimum value of safety after last ComputeStep

   G4double       ComputeSafety( const G4ThreeVector& pGlobalPoint ); 
     //
     // Recompute safety for the relevant point
     // the endpoint of the last step!!

   void EnableParallelNavigation( G4bool enableChoice=true ); 
     //
     // Must call it to ensure that PathFinder is prepared,  
     // especially for curved tracks. If true it switches PropagatorInField
     // to use MultiNavigator. Must call it with false to undo (=PiF use
     // Navigator for tracking!)

   inline G4int  SetVerboseLevel(G4int lev=-1);

 public:  // with description

   inline G4int   GetMaxLoopCount() const;
   inline void    SetMaxLoopCount( G4int new_max );
     //
     // A maximum for the number of steps that a (looping) particle can take.

 public:  // without description

   inline void MovePoint();
     //
     // Signal that location will be moved -- internal use primarily

   G4String& LimitedString( ELimited lim );
     // Convert ELimited to string

 protected:  // without description

  G4double  DoNextLinearStep(  const G4FieldTrack  &FieldTrack,
                               G4double            proposedStepLength); 

  G4double  DoNextCurvedStep(  const G4FieldTrack  &FieldTrack,
                               G4double            proposedStepLength,
                               G4VPhysicalVolume*  pCurrentPhysVolume); 

  void WhichLimited();
  void PrintLimited();
  //
  // Print key details out - for debugging

  void ClearState();
  //
  // Clear all the State of this class and its current associates

  inline G4bool UseSafetyForOptimization( G4bool );
  //
  // Whether use safety to discard unneccesary calls to navigator

 protected:

   G4PathFinder();  //  Singleton 
  ~G4PathFinder();

  inline G4Navigator* GetNavigator(G4int n) const;

 private:

  // ----------------------------------------------------------------------
  //  DATA Members
  // ----------------------------------------------------------------------

  G4MultiNavigator *fpMultiNavigator; 
    //  Object that enables G4PropagatorInField to see many geometries

   G4int   fNoActiveNavigators; 
   G4bool  fNewTrack;               // Flag a new track (ensure first step)

   static const G4int fMaxNav = 8;  // rename to kMaxNoNav ??

   // Global state (retained during stepping for one track)

   G4Navigator*  fpNavigator[fMaxNav];

   // State after a step computation 

   ELimited      fLimitedStep[fMaxNav];
   G4bool        fLimitTruth[fMaxNav];
   G4double      fCurrentStepSize[fMaxNav]; 
   G4double      fNewSafety[ fMaxNav ];      // Safety for starting point
   G4double      fMinSafety;

   G4double      fMinStep;      // As reported by Navigators -- can be kInfinity
   G4double      fTrueMinStep;  // Corrected in case >= proposed

   // State after calling 'locate'

   G4VPhysicalVolume* fLocatedVolume[fMaxNav];
   G4ThreeVector      fLastLocatedPosition; 

   G4FieldTrack    fEndState;
     // End point storage
   G4bool fRelocatedPoint;   //  Signals that point was or is being moved 
                             //  from the position of the last location
                             //   or the endpoint resulting from ComputeStep 
                             //   -- invalidates fEndState

   G4ThreeVector fSafetyLocation;       //  point where ComputeSafety is called
   G4double      fMinSafety_atSafLocation; // /\ corresponding value of safety
   G4ThreeVector fPreStepLocation;      //  point where last ComputeStep called
   G4double      fMinSafety_PreStepPt;  //   /\ corresponding value of safety

   G4int           fLastStepNo, fCurrentStepNo; 
   G4bool      fParticleIsLooping;
   G4int  fVerboseLevel;
     // For debuging purposes
   G4int  fMax_loop_count;
     // Limit for the number of sub-steps taken in one call to ComputeStep

   G4TransportationManager* fpTransportManager; // Cache for frequent use
   G4PropagatorInField* fpFieldPropagator;

   G4double kCarTolerance;
};

// ********************************************************************
// Inline methods.
// ********************************************************************

inline G4VPhysicalVolume* G4PathFinder::GetLocatedVolume( G4int navId ) const
{
  G4VPhysicalVolume* vol=0;  
  if( (navId < fMaxNav) && (navId >=0) ) { vol= fLocatedVolume[navId]; }
  return vol; 
}

inline G4int  G4PathFinder::SetVerboseLevel(G4int newLevel)
{
  G4int old= fVerboseLevel;  fVerboseLevel= newLevel; return old;
}

inline G4double G4PathFinder::GetCurrentSafety() const
{
  return fMinSafety;
}

inline void G4PathFinder::MovePoint()
{
  fRelocatedPoint= true;
}

inline G4Navigator* G4PathFinder::GetNavigator(G4int n) const
{ 
  if( (n>fNoActiveNavigators)||(n<0)) { n=0; }
  return fpNavigator[n];
}

#endif 

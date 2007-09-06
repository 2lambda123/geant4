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
// $Id: A01Trajectory.hh,v 1.1 2007-09-06 22:26:29 tinslay Exp $
// --------------------------------------------------------------
//

class A01Trajectory;

#ifndef A01Trajectory_h
#define A01Trajectory_h 1

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>                 // Include from 'system'
#include "G4ios.hh"                 // Include from 'system'
#include <vector>             //
#include <iostream>             //
#include "globals.hh"               // Include from 'global'
#include "G4ParticleDefinition.hh"  // Include from 'particle+matter'
#include "G4TrajectoryPoint.hh"     // Include from 'tracking'
#include "G4Track.hh"
#include "G4Step.hh"

typedef std::vector<G4VTrajectoryPoint*> A01TrajectoryPointContainer;

class G4Polyline;                   // Forward declaration.

///////////////////
class A01Trajectory : public G4VTrajectory
///////////////////
{

//--------
   public:
//--------

// Constructor/Destrcutor

   A01Trajectory();

   A01Trajectory(const G4Track* aTrack);
   A01Trajectory(A01Trajectory &);
   virtual ~A01Trajectory();

// Operators
   inline void* operator new(size_t);
   inline void  operator delete(void*);
   inline int operator == (const A01Trajectory& right) const
   {return (this==&right);}

// Get/Set functions
   inline G4int GetTrackID() const
   { return fTrackID; }
   inline G4int GetParentID() const
   { return fParentID; }
   inline G4String GetParticleName() const
   { return ParticleName; }
   inline G4double GetCharge() const
   { return PDGCharge; }
   inline G4int GetPDGEncoding() const
   { return PDGEncoding; }
   inline G4ThreeVector GetInitialMomentum() const
   { return InitialMomentum; }

// Other member functions
   virtual void ShowTrajectory() const;
   virtual void ShowTrajectory(std::ostream& o) const;
   virtual void DrawTrajectory(G4int i_mode=0) const;
   virtual void AppendStep(const G4Step* aStep);
   virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

   G4ParticleDefinition* GetParticleDefinition();

//---------
   private:
//---------

  A01TrajectoryPointContainer* positionRecord;
  G4int fTrackID;
  G4int fParentID;
  G4ParticleDefinition* fpParticleDefinition;
  G4String ParticleName;
  G4double PDGCharge;
  G4int    PDGEncoding;
// FIXME not initialized !!!
  G4ThreeVector InitialMomentum;

//---------
   public:
//---------
   virtual int GetPointEntries() const
   { return positionRecord->size(); }
   virtual G4VTrajectoryPoint* GetPoint(G4int i) const
   { return (*positionRecord)[i]; }
};

extern G4Allocator<A01Trajectory> myTrajectoryAllocator;

inline void* A01Trajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)myTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void A01Trajectory::operator delete(void* aTrajectory)
{
  myTrajectoryAllocator.FreeSingle((A01Trajectory*)aTrajectory);
}

#endif

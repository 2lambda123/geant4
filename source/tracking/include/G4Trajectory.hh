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
// $Id: G4Trajectory.hh,v 1.13 2001-07-11 10:08:41 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//---------------------------------------------------------------
//
// G4Trajectory.hh
//
// class description:
//   This class represents the trajectory of a particle tracked.
//   It includes information of 
//     1) List of trajectory points which compose the trajectory,
//     2) static information of particle which generated the 
//        trajectory, 
//     3) trackID and parent particle ID of the trajectory,
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Makoto  Asai   (e-mail: asai@kekvax.kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
// ---------------------------------------------------------------

class G4Trajectory;

#ifndef G4Trajectory_h
#define G4Trajectory_h 1

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>                 // Include from 'system'
#include "G4ios.hh"               // Include from 'system'
#include "g4std/vector"            // G4RWTValOrderedVector
#include "globals.hh"               // Include from 'global'
#include "G4ParticleDefinition.hh"  // Include from 'particle+matter'
#include "G4TrajectoryPoint.hh"     // Include from 'tracking'
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

typedef G4std::vector<G4VTrajectoryPoint*>  TrajectoryPointContainer;
///////////////////
class G4Trajectory : public G4VTrajectory
///////////////////
{

//--------
public: // with description
//--------

// Constructor/Destrcutor

   G4Trajectory();

   G4Trajectory(const G4Track* aTrack);
   G4Trajectory(G4Trajectory &);
   virtual ~G4Trajectory();

// Operators
   inline void* operator new(size_t);
   inline void  operator delete(void*);
   inline int operator == (const G4Trajectory& right) const
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

// Other member functions
   virtual void ShowTrajectory() const;
   virtual void DrawTrajectory(G4int i_mode=0) const;
   virtual void AppendStep(const G4Step* aStep);
   virtual int GetPointEntries() const { return positionRecord->size(); }
   virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
   { return (*positionRecord)[i]; }
   virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

   G4ParticleDefinition* GetParticleDefinition();

//---------
   private:
//---------

  TrajectoryPointContainer* positionRecord;
  G4int fTrackID;
  G4int fParentID;
  G4String ParticleName;
  G4double PDGCharge;
  G4int    PDGEncoding;

};

extern G4Allocator<G4Trajectory> aTrajectoryAllocator;

inline void* G4Trajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)aTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void G4Trajectory::operator delete(void* aTrajectory)
{
  aTrajectoryAllocator.FreeSingle((G4Trajectory*)aTrajectory);
}

#endif











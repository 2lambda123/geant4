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
// $Id: G4RayTrajectory.hh,v 1.8 2001-07-11 10:09:03 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

// class description:
//
//  This is a concrete class of G4VTrajectory which represents a trajectory of a ray.
//  This class is used by G4RayTracer. Objects of this class are created by G4RTTrackingAction.
//  This class does not have concrete implementations of draw and print methods but
// information stored in this class is used by determining a colour of a pixel of the picture.
//

///////////////////
//G4RayTrajectory.hh
///////////////////

#ifndef G4RayTrajectory_h
#define G4RayTrajectory_h 1

class G4Step;

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "g4std/vector"
#include "globals.hh"
#include "G4Track.hh"
#include "G4RayTrajectoryPoint.hh"


class G4RayTrajectory : public G4VTrajectory
{
   public:

   G4RayTrajectory(); 
   G4RayTrajectory(G4RayTrajectory & right);
   virtual ~G4RayTrajectory();

   inline void* operator new(size_t);
   inline void  operator delete(void*);
   inline int operator == (const G4RayTrajectory& right){return (this==&right);}

   virtual void AppendStep(const G4Step*);
   virtual void ShowTrajectory() const;
   virtual void DrawTrajectory(G4int i_mode=0) const {;}
   virtual int GetPointEntries() const {return positionRecord->size();}
   virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
   { return (*positionRecord)[i]; }
   G4RayTrajectoryPoint* GetPointC(G4int i) const 
   { return (*positionRecord)[i]; }
   virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);  

   private:

   G4std::vector<G4RayTrajectoryPoint*>* positionRecord;
};


extern G4Allocator<G4RayTrajectory> G4RayTrajectoryAllocator;

inline void* G4RayTrajectory::operator new(size_t)
{
   void* aTrajectory;
   aTrajectory = (void*)G4RayTrajectoryAllocator.MallocSingle();
   return aTrajectory;
}

inline void G4RayTrajectory::operator delete(void* aTrajectory)
{
   G4RayTrajectoryAllocator.FreeSingle((G4RayTrajectory*)aTrajectory);
}


#endif


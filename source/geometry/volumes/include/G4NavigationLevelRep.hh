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
// $Id: G4NavigationLevelRep.hh,v 1.8 2003-11-02 16:06:05 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4NavigationLevelRep
//
// Class description:
//
// A data representation class, used to hold the data for a single level
// of the Navigation history tree.
//
// This is the body of a handle/body pair of classes, that implement
// reference counting for NavigationLevels.
// The corresponding handle class is G4NavigationLevel

// History:
//
// - 1 October 1997, J.Apostolakis: initial version. 
// ----------------------------------------------------------------------
#ifndef G4NAVIGATIONLEVELREP_HH
#define G4NAVIGATIONLEVELREP_HH

#include "G4Types.hh"

#include "G4AffineTransform.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Allocator.hh"

class G4NavigationLevelRep
{

 public:  // with description

   G4NavigationLevelRep( G4VPhysicalVolume*  newPtrPhysVol,
                   const G4AffineTransform&  newT,
                         EVolume             newVolTp,
                         G4int               newRepNo= -1 );

   G4NavigationLevelRep( G4VPhysicalVolume*  newPtrPhysVol,
                   const G4AffineTransform&  levelAbove,
                   const G4AffineTransform&  relativeCurrent,
                         EVolume             newVolTp,
                         G4int               newRepNo= -1 );
     // As the previous constructor, but instead of giving Transform, give 
     // the AffineTransform to the level above and the current level's 
     // Transform relative to that.

   G4NavigationLevelRep();
   G4NavigationLevelRep( G4NavigationLevelRep& );

   ~G4NavigationLevelRep();

   G4NavigationLevelRep& operator=(const G4NavigationLevelRep &right);

   inline G4VPhysicalVolume* GetPhysicalVolume();

   inline const G4AffineTransform* GetTransformPtr() const ;  // New
   inline const G4AffineTransform& GetTransform() const ;     // Old

   inline EVolume            GetVolumeType() const ;
   inline G4int              GetReplicaNo() const ;

   inline void   AddAReference(); 
   inline G4bool RemoveAReference(); 
     // Take care of the reference counts.

   inline void *operator new(size_t);
     // Override "new"    to use "G4Allocator".
   inline void operator delete(void *aTrack);
     // Override "delete" to use "G4Allocator".

 private:

   G4AffineTransform  sTransform;
     // Compounded global->local transformation (takes a point in the 
     // global reference system to the system of the volume at this level)

   G4VPhysicalVolume* sPhysicalVolumePtr;
     // Physical volume ptrs, for this level's volume

   G4int              sReplicaNo;
   EVolume            sVolumeType;
     // Volume `type' 

   G4int              fCountRef; 

};

#include "G4NavigationLevelRep.icc"

#endif

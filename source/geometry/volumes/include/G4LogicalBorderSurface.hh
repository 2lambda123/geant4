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
// $Id: G4LogicalBorderSurface.hh,v 1.13 2004-05-19 08:14:38 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4LogicalBorderSurface
//
// Class description:
//
// A Logical Surface class for surfaces defined by the boundary
// of two physical volumes.

// History:
// -------
// Created:     1997-06-17
// Author:      John Apostolakis (John.Apostolakis@cern.ch)
//
// --------------------------------------------------------------------
#ifndef G4LogicalBorderSurface_h
#define G4LogicalBorderSurface_h 1

#include  "G4LogicalSurface.hh"
#include "G4VPhysicalVolume.hh"

#include <vector>

class G4Event;
class G4VPhysicalVolume;

class G4LogicalBorderSurface : public G4LogicalSurface
{

  public:  // with description

    G4LogicalBorderSurface( const G4String& name,
                                  G4VPhysicalVolume* vol1, 
                                  G4VPhysicalVolume* vol2,
                                  G4SurfaceProperty* surfaceProperty );
    ~G4LogicalBorderSurface();
      // Constructor and destructor

    static G4LogicalBorderSurface* GetSurface( const G4VPhysicalVolume* vol1,
                                               const G4VPhysicalVolume* vol2 );
    inline void SetPhysicalVolumes( G4VPhysicalVolume* vol1,
                                    G4VPhysicalVolume* vol2 );
    inline const G4VPhysicalVolume* GetVolume1() const;
    inline const G4VPhysicalVolume* GetVolume2() const;
      // Generic accessors.

    inline void SetVolume1( G4VPhysicalVolume* vol1 );
    inline void SetVolume2( G4VPhysicalVolume* vol2 );
      // To use with care!

    static void CleanSurfaceTable();
    static const std::vector<G4LogicalBorderSurface*> * GetSurfaceTable();
    static size_t GetNumberOfBorderSurfaces();
    static void DumpInfo(); 
      // To handle the table of surfaces.

    G4int operator==( const G4LogicalBorderSurface &right ) const;
    G4int operator!=( const G4LogicalBorderSurface &right ) const;
      // Operators.

  private:

    G4LogicalBorderSurface(const G4LogicalBorderSurface &right);
    const G4LogicalBorderSurface& operator=(const G4LogicalBorderSurface &right);

  private:

    G4VPhysicalVolume* Volume1;  // Physical Volume pointer on side 1
    G4VPhysicalVolume* Volume2;  // Physical Volume pointer on side 2

    static std::vector<G4LogicalBorderSurface*> theBorderSurfaceTable;
      // The static Table of Surfaces.
};

typedef std::vector<G4LogicalBorderSurface*> G4LogicalBorderSurfaceTable;

// ********************************************************************
// Inline methods
// ********************************************************************

#include "G4LogicalBorderSurface.icc"

#endif /* G4LogicalBorderSurface_h */


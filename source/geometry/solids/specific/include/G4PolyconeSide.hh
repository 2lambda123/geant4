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
// $Id: G4PolyconeSide.hh,v 1.6 2005-11-09 15:04:28 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class header file
//
//
// G4PolyconeSide
//
// Class description:
//
//   Class implmenting a face that represents one conical side
//   of a polycone:
//
//   G4PolyconeSide( const G4PolyconeSideRZ *prevRZ,
//                   const G4PolyconeSideRZ *tail,
//                   const G4PolyconeSideRZ *head,
//                   const G4PolyconeSideRZ *nextRZ,
//                         G4double phiStart, G4double deltaPhi, 
//                         G4bool phiIsOpen, G4bool isAllBehind=false )
//
//   Values for r1,z1 and r2,z2 should be specified in clockwise
//   order in (r,z).

// Author: 
//   David C. Williams (davidw@scipp.ucsc.edu)
// --------------------------------------------------------------------

#ifndef G4PolyconeSide_hh
#define G4PolyconeSide_hh

#include "G4VCSGface.hh"

class G4IntersectingCone;

typedef struct
{
  G4double r, z;  // start of vector
} G4PolyconeSideRZ;
 
class G4PolyconeSide : public G4VCSGface
{
  public:

    G4PolyconeSide( const G4PolyconeSideRZ *prevRZ,
                    const G4PolyconeSideRZ *tail,
                    const G4PolyconeSideRZ *head,
                    const G4PolyconeSideRZ *nextRZ,
                          G4double phiStart, G4double deltaPhi, 
                          G4bool phiIsOpen, G4bool isAllBehind=false );
    virtual ~G4PolyconeSide();
  
    G4PolyconeSide( const G4PolyconeSide &source );
    G4PolyconeSide& operator=( const G4PolyconeSide &source );
  
    G4bool Intersect( const G4ThreeVector &p, const G4ThreeVector &v,  
                            G4bool outgoing, G4double surfTolerance,
                            G4double &distance, G4double &distFromSurface,
                            G4ThreeVector &normal, G4bool &isAllBehind );

    G4double Distance( const G4ThreeVector &p, G4bool outgoing );
  
    EInside Inside( const G4ThreeVector &p, G4double tolerance, 
                          G4double *bestDistance );
  
    G4ThreeVector Normal( const G4ThreeVector &p,  G4double *bestDistance );

    G4double Extent( const G4ThreeVector axis );

    void CalculateExtent( const EAxis axis, 
                          const G4VoxelLimits &voxelLimit,
                          const G4AffineTransform &tranform,
                                G4SolidExtentList &extentList       );

    G4VCSGface *Clone() { return new G4PolyconeSide( *this ); }
  
  public:  // without description

    G4PolyconeSide(__void__&);
      // Fake default constructor for usage restricted to direct object
      // persistency for clients requiring preallocation of memory for
      // persistifiable objects.

  protected:

    G4double r[2], z[2]; // r, z parameters, in specified order
    G4double startPhi,   // Start phi (0 to 2pi), if phiIsOpen
             deltaPhi;   // Delta phi (0 to 2pi), if phiIsOpen
    G4bool   phiIsOpen;  // True if there is a phi slice
    G4bool   allBehind;  // True if the entire solid is "behind" this face
  
    G4IntersectingCone *cone;  // Our intersecting utility class
  
    G4double rNorm, zNorm;  // Normal to surface in r,z space
    G4double rS, zS;        // Unit vector along surface in r,z space
    G4double length;        // Length of face in r,z space
    G4double prevRS,
             prevZS;        // Unit vector along previous polyconeSide
    G4double nextRS,
             nextZS;        // Unit vector along next polyconeSide
  
    G4double rNormEdge[2],
             zNormEdge[2];  // Normal to edges

    G4ThreeVector *corners; // The coordinates of the corners (if phiIsOpen)
        
    G4double DistanceAway( const G4ThreeVector &p, G4bool opposite,
                                 G4double &distOutside2, G4double *rzNorm=0 );
      
    G4bool PointOnCone( const G4ThreeVector &hit, G4double normSign,
                        const G4ThreeVector &p,
                        const G4ThreeVector &v, G4ThreeVector &normal );

    void CopyStuff( const G4PolyconeSide &source );
  
    static void FindLineIntersect( G4double x1, G4double y1,
                                   G4double tx1, G4double ty1,
                                   G4double x2, G4double y2,
                                 G4double tx2, G4double ty2,
                                 G4double &x, G4double &y );
};

#endif

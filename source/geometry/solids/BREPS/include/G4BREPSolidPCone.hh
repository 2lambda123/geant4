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
// $Id: G4BREPSolidPCone.hh,v 1.9 2002-01-22 22:43:01 radoone Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4BREPSolidPCone
//
// Class description:
//
//  The polyconical solid G4BREPSolidPCone is a shape defined by a set of 
//  inner and outer conical or cylindrical surface sections and two planes 
//  perpendicular to the Z axis. Each conical surface is defined by its 
//  radius at two different planes perpendicular to the Z-axis. Inner and 
//  outer conical surfaces are defined using common Z planes:
//
//  G4BREPSolidPCone( const G4String& name,
//                          G4double start_angle,
//                          G4double opening_angle,		   
//                          G4int    num_z_planes,
//                          G4double z_start,		   
//                          G4double z_values[],
//                          G4double RMIN[],
//                          G4double RMAX[] )

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, S.Giani, G.Cosmo.
// ----------------------------------------------------------------------
#ifndef __G4BREPSolidPCone
#define __G4BREPSolidPCone

#include "G4BREPSolid.hh"

class G4BREPSolidPCone : public G4BREPSolid
{

 public: // with description

  G4BREPSolidPCone( const G4String& name,
		    G4double start_angle,
		    G4double opening_angle,		   
		    G4int    num_z_planes, // sections
		    G4double z_start,		   
		    G4double z_values[],
		    G4double RMIN[],
		    G4double RMAX[] );
    // Constructor defining the polycone through its
    // conical/cylindrical surfaces.

  ~G4BREPSolidPCone();
    // Destructor.

  void Initialize();
    // Calculates the bounding box.

  EInside Inside(register const G4ThreeVector& Pt) const;
    // Determines if the point Pt is inside, outside or on the surface
    // of the solid.

  G4ThreeVector SurfaceNormal(const G4ThreeVector&) const;
    // Calculates the normal of the surface at a point on the surface
    // The sense of the normal depends on the sense of the surface.

  G4double DistanceToIn(const G4ThreeVector&) const;
    // Calculates the shortest distance ("safety") from a point
    // outside the solid to any boundary of this solid.
    // Return 0 if the point is already inside.

  G4double DistanceToIn(register const G4ThreeVector& Pt, 
			register const G4ThreeVector& V) const;
    // Calculates the distance from a point Pt outside the solid
    // to the solid's boundary along a specified direction vector V.
    // Note: Intersections with boundaries less than the tolerance must
    //       be ignored if the direction is away from the boundary.

  G4double DistanceToOut(register const G4ThreeVector& Pt, 
			 register const G4ThreeVector& V, 
			 const G4bool calcNorm=false, 
			 G4bool *validNorm=0, G4ThreeVector *n=0) const;
    // Calculates the distance from a point inside the solid to the solid`s
    // boundary along a specified direction vector.
    // Return 0 if the point is already outside.
    // Note: If the shortest distance to a boundary is less than the
    //       tolerance, it is ignored. This allows for a point within a
    //       tolerant boundary to leave immediately.

  G4double DistanceToOut(const G4ThreeVector&) const;
    // Calculates the shortest distance ("safety") from a point inside the
    // solid to any boundary of this solid.
    // Return 0 if the point is already outside.	

public:

  void Reset() const;
    // Resets all distance attributes.

  G4Polyhedron* CreatePolyhedron () const;
    // Creates a G4Polyhedron

private:
  
  G4Surface* ComputePlanarSurface( G4double r1, G4double r2,
                                   G4ThreeVector& origin,
                                   G4ThreeVector& planeAxis,
                                   G4ThreeVector& planeDirection,
                                   G4int surfSense );
    // For a given radius values compute a planar surface

  G4BREPSolidPCone(const G4BREPSolidPCone&);
  G4BREPSolidPCone& operator=(const G4BREPSolidPCone&);
    // Private copy constructor and assignment operator.

  // The following is only utilised in storing the shape parameters for
  // use in visualising this shape.  J.A. Feb  24, 1997
  //
  struct PConeParameters {
     G4double Start_angle;
     G4double Opening_angle;		   
     G4int    Num_z_planes; 
     // G4double z_start;		   
     G4double *Z_values;
     G4double *Rmin;
     G4double *Rmax;
  }  original_parameters;
};

#endif

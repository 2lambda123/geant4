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
// $Id: G4BREPSolidSphere.hh,v 1.9 2003-06-16 16:52:29 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4BREPSolidSphere
//
// Class description:
// 
//  Definition of a generic BREP sphere:
//
//  G4BREPSolidSphere(const G4String& name,
//                    const G4Vector3D& origin,
//                    const G4Vector3D& xhat, 
//                    const G4Vector3D& zhat,
//                          G4double radius)

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
// ----------------------------------------------------------------------
#ifndef __G4BREPSolidSphere
#define __G4BREPSolidSphere

#include "G4BREPSolid.hh"

class G4BREPSolidSphere : public G4BREPSolid
{

 public:  // with description

  G4BREPSolidSphere(const G4String& name,
		    const G4Vector3D& origin,
		    const G4Vector3D& xhat, 
		    const G4Vector3D& zhat,
		    G4double radius);
    // Constructor defining the sphere through its spherical surfaces.

  ~G4BREPSolidSphere();
    // Empty destructor.

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

  virtual std::ostream& StreamInfo(std::ostream& os) const;
    // Streams solid contents to output stream.

 public:  // without description

  inline void SphReset() { active=1; }

 private:

  G4BREPSolidSphere(const G4BREPSolidSphere&);
  G4BREPSolidSphere& operator=(const G4BREPSolidSphere&);
    // Private copy constructor and assignment operator.

  struct G4BREPSphereParams
  {
    G4Vector3D origin;
	  G4Vector3D xhat;
	  G4Vector3D zhat;
	  G4double   radius;
  } constructorParams;
};

#endif

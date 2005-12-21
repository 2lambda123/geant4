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
// $Id: G4Parabola.hh,v 1.9 2005-12-21 17:37:34 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4Parabola
//
// Class description:
// 
// Definition of a generic parabola.

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
// ----------------------------------------------------------------------
#ifndef __PARABOLICCURVE_H
#define __PARABOLICCURVE_H 

#include "G4Conic.hh"

class G4Parabola : public G4Conic
{

public:  // with description

  G4Parabola();
  virtual ~G4Parabola();
    // Constructor & destructor.

  G4Parabola(const G4Parabola& right);
  G4Parabola& operator=(const G4Parabola& right);
    // Copy constructor and assignment operator.

  G4Curve* Project(const G4Transform3D& tr =
                   HepGeom::Transform3D::Identity);
    // Transforms and projects the curve.

  G4bool Tangent(G4CurvePoint& cp, G4Vector3D& v);
    // Returns tangent to curve at a given point, if existing.
    // The tangent is computed from the 3D point representation.

  inline G4double  GetPMax() const;
  inline G4Point3D GetPoint(G4double param) const;
  inline G4double  GetPPoint(const G4Point3D& p) const;
    // Accessors methods.

  inline G4double GetFocalDist() const;
  inline void Init(const G4Axis2Placement3D& position0, G4double focalDist0);
    // Get/Set for the geometric data.

public:  // without description

  inline G4int IntersectRay2D(const G4Ray& ray);

  // void IntersectRay2D(const G4Ray& ray, G4CurveRayIntersection& is);
  // G4Parabola(G4Point3d, G4Point3d, G4double );
  // G4Point3d EvaluateByParameterValue(G4double u);
  // G4Point3d GetBoundMax();
  // G4Point3d GetBoundMin();   

protected:

  void InitBounded();

private:

  // geometric data

  G4double focalDist;

  // for the intersection
  G4Point3D F;
  G4Point3D L0;

};

#include "G4Parabola.icc"

#endif

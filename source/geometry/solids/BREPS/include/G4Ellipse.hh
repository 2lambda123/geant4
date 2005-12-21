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
// $Id: G4Ellipse.hh,v 1.9 2005-12-21 17:37:34 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4Ellipse
//
// Class description:
// 
// Definition of a generic ellipse curve.

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
// ----------------------------------------------------------------------
#ifndef __ELLIPTICCURVE_H
#define __ELLIPTICCURVE_H 

#include "G4CircularCurve.hh"

class G4Ellipse : public G4Conic
{

public:  // with description

  G4Ellipse();
  virtual ~G4Ellipse();
    // Constructor & destructor.

  G4Ellipse(const G4Ellipse& right);
  G4Ellipse& operator=(const G4Ellipse& right);
    // Copy constructor and assignment operator.

  G4Curve* Project(const G4Transform3D& tr=
                   HepGeom::Transform3D::Identity);
    // Transforms and projects the curve.

  G4bool Tangent(G4CurvePoint& cp, G4Vector3D& v);
    // Returns tangent to curve at a given point, if existing.
    // The tangent is computed from the 3D point representation.

  inline G4double  GetPMax() const;
  inline G4Point3D GetPoint(G4double param) const;
  inline G4double  GetPPoint(const G4Point3D& p) const;
    // Accessors methods.

  inline G4double GetSemiAxis1() const;
  inline G4double GetSemiAxis2() const;
  inline void Init(const G4Axis2Placement3D& position0,
	           G4double semiAxis10, G4double semiAxis20);
    // Get/Set methods for the geometric data.


public:

  inline G4int IntersectRay2D(const G4Ray& ray);
  // void IntersectRay2D(const G4Ray& ray, G4CurveRayIntersection& is);

protected:

  void InitBounded();

private:

  // geometric data

  G4double semiAxis1;
  G4double semiAxis2;
  G4double ratioAxis2Axis1;
  
  G4Transform3D toUnitCircle;

  G4double forTangent; // -R_1^2/R_2^2
};

#include "G4Ellipse.icc"

#endif

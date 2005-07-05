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
// $Id: G4Polyhedron.cc,v 1.17 2005-07-05 14:04:02 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4Polyhedron.hh"

G4Polyhedron::G4Polyhedron ():
  fNumberOfRotationStepsAtTimeOfCreation (fNumberOfRotationSteps)
{}

G4Polyhedron::~G4Polyhedron () {}

G4Polyhedron::G4Polyhedron (const HepPolyhedron& from)
  : HepPolyhedron(from)
{
  fNumberOfRotationStepsAtTimeOfCreation =
    from.fNumberOfRotationSteps;
}

G4PolyhedronBox::G4PolyhedronBox (G4double dx, G4double dy, G4double dz):
  G4Polyhedron (HepPolyhedronBox (dx, dy, dz)) {}

G4PolyhedronBox::~G4PolyhedronBox () {}

G4PolyhedronCone::G4PolyhedronCone (G4double Rmn1, G4double Rmx1, 
				    G4double Rmn2, G4double Rmx2, G4double Dz):
  G4Polyhedron (HepPolyhedronCone (Rmn1, Rmx1, Rmn2, Rmx2, Dz)) {}

G4PolyhedronCone::~G4PolyhedronCone () {}

G4PolyhedronCons::G4PolyhedronCons (G4double Rmn1, G4double Rmx1, 
				    G4double Rmn2, G4double Rmx2, G4double Dz,
				    G4double Phi1, G4double Dphi):
  G4Polyhedron (HepPolyhedronCons (Rmn1, Rmx1, Rmn2, Rmx2, Dz, Phi1, Dphi)) {}

G4PolyhedronCons::~G4PolyhedronCons () {}

G4PolyhedronPara::G4PolyhedronPara (G4double Dx, G4double Dy, G4double Dz,
				    G4double Alpha, G4double Theta,
				    G4double Phi):
  G4Polyhedron (HepPolyhedronPara (Dx, Dy, Dz, Alpha, Theta, Phi)) {}

G4PolyhedronPara::~G4PolyhedronPara () {}

G4PolyhedronPcon::G4PolyhedronPcon (G4double phi, G4double dphi, G4int nz,
				    const G4double *z,
				    const G4double *rmin,
				    const G4double *rmax):
  G4Polyhedron (HepPolyhedronPcon (phi, dphi, nz, z, rmin, rmax)) {}

G4PolyhedronPcon::~G4PolyhedronPcon () {}

G4PolyhedronPgon::G4PolyhedronPgon (G4double phi, G4double dphi, G4int npdv,
				    G4int nz,
				    const G4double *z,
				    const G4double *rmin,
				    const G4double *rmax):
  G4Polyhedron (HepPolyhedronPgon (phi, dphi, npdv, nz, z, rmin, rmax)) {}

G4PolyhedronPgon::~G4PolyhedronPgon () {}

G4PolyhedronSphere::G4PolyhedronSphere (G4double rmin, G4double rmax,
					G4double phi, G4double dphi,
					G4double the, G4double dthe):
  G4Polyhedron (HepPolyhedronSphere (rmin, rmax, phi, dphi, the, dthe)) {}

G4PolyhedronSphere::~G4PolyhedronSphere () {}

G4PolyhedronTorus::G4PolyhedronTorus (G4double rmin, G4double rmax,
				      G4double rtor,
				      G4double phi, G4double dphi):
  G4Polyhedron (HepPolyhedronTorus (rmin, rmax, rtor, phi, dphi)) {}

G4PolyhedronTorus::~G4PolyhedronTorus () {}

G4PolyhedronTrap::G4PolyhedronTrap (G4double Dz, G4double Theta, G4double Phi,
				    G4double Dy1,
				    G4double Dx1, G4double Dx2, G4double Alp1,
				    G4double Dy2,
				    G4double Dx3, G4double Dx4, G4double Alp2):
  G4Polyhedron (HepPolyhedronTrap (Dz, Theta, Phi, Dy1, Dx1, Dx2, Alp1,
				   Dy2, Dx3, Dx4, Alp2)) {}

G4PolyhedronTrap::~G4PolyhedronTrap () {}

G4PolyhedronTrd1::G4PolyhedronTrd1 (G4double Dx1, G4double Dx2,
				    G4double Dy, G4double Dz):
  G4Polyhedron (HepPolyhedronTrd1 (Dx1, Dx2, Dy, Dz)) {}

G4PolyhedronTrd1::~G4PolyhedronTrd1 () {}

G4PolyhedronTrd2::G4PolyhedronTrd2 (G4double Dx1, G4double Dx2,
				    G4double Dy1, G4double Dy2, G4double Dz):
  G4Polyhedron (HepPolyhedronTrd2 (Dx1, Dx2, Dy1, Dy2, Dz)) {}

G4PolyhedronTrd2::~G4PolyhedronTrd2 () {}

G4PolyhedronTube::G4PolyhedronTube (G4double Rmin, G4double Rmax, G4double Dz):
  G4Polyhedron (HepPolyhedronTube (Rmin, Rmax, Dz)) {}

G4PolyhedronTube::~G4PolyhedronTube () {}

G4PolyhedronTubs::G4PolyhedronTubs (G4double Rmin, G4double Rmax, G4double Dz, 
				    G4double Phi1, G4double Dphi):
  G4Polyhedron (HepPolyhedronTubs (Rmin, Rmax, Dz, Phi1, Dphi)) {}

G4PolyhedronTubs::~G4PolyhedronTubs () {}

G4PolyhedronEllipsoid::G4PolyhedronEllipsoid (G4double ax, G4double by,
                                              G4double cz, 
					      G4double zCut1, G4double zCut2):
  G4Polyhedron (HepPolyhedronEllipsoid (ax, by, cz, zCut1, zCut2)) {}

G4PolyhedronEllipsoid::~G4PolyhedronEllipsoid () {}

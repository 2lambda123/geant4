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
// $Id: G4BoundingSphereScene.cc,v 1.9 2005-01-27 20:06:49 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  7th June 1997
// An artificial scene to reuse G4VScene code to calculate a bounding sphere.

#include "G4BoundingSphereScene.hh"

#include "G4VSolid.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4Vector3D.hh"

G4BoundingSphereScene::G4BoundingSphereScene (G4VModel* pModel):
  fpModel (pModel),
  fRadius (-1.),
  fpObjectTransformation (0)
{}

G4BoundingSphereScene::~G4BoundingSphereScene () {}

void G4BoundingSphereScene::PreAddSolid
(const G4Transform3D& objectTransformation,
 const G4VisAttributes&) {
  fpObjectTransformation = &objectTransformation;
}

G4VisExtent G4BoundingSphereScene::GetBoundingSphereExtent () {
  return G4VisExtent (fCentre, fRadius);
}

void G4BoundingSphereScene::Accrue (const G4VSolid& solid) {

  const G4VisExtent& newExtent = solid.GetExtent ();
  G4Point3D newCentre = newExtent.GetExtentCentre ();
  if (fpObjectTransformation) {
    newCentre.transform (*fpObjectTransformation);
  }
  const G4double newRadius = newExtent.GetExtentRadius ();
  AccrueBoundingSphere (newCentre, newRadius);

  // Curtail descent - can assume daughters are contained within mother...
  G4PhysicalVolumeModel* pPVM = fpModel->GetG4PhysicalVolumeModel();
  if (pPVM) pPVM->CurtailDescent();
}

void G4BoundingSphereScene::ResetBoundingSphere () {
  fCentre = G4Point3D ();
  fRadius = -1.;
  fpObjectTransformation = 0;
}

void G4BoundingSphereScene::AccrueBoundingSphere
(const G4Point3D& newCentre,
 G4double newRadius) {

  if (fRadius < 0 ) {  // First time.
    fCentre = newCentre;
    fRadius = newRadius;
  }
  else {
    G4Vector3D join = newCentre - fCentre;
    if (join == G4Vector3D (0., 0., 0.)) {             // Centres coincide.
      if (fRadius < newRadius) fRadius = newRadius;
    }
    else if (join.mag () + newRadius <= fRadius) {  // Inside accrued sphere.
      // Do nothing.
    }
    else {
      G4Vector3D unitJoin = join.unit ();
      G4Point3D oldExtremity1 = fCentre - fRadius * unitJoin;
      G4Point3D newExtremity1 = newCentre - newRadius * unitJoin;
      G4Point3D oldExtremity2 = fCentre + fRadius * unitJoin;
      G4Point3D newExtremity2 = newCentre + newRadius * unitJoin;
      G4Point3D extremity1;
      if (oldExtremity1 * unitJoin < newExtremity1 * unitJoin) {
	extremity1 = oldExtremity1;
      }
      else {
	extremity1 = newExtremity1;
      }
      G4Point3D extremity2;
      if (oldExtremity2 * unitJoin > newExtremity2 * unitJoin) {
	extremity2 = oldExtremity2;
      }
      else {
	extremity2 = newExtremity2;
      }
      fCentre = 0.5 * (extremity2 + extremity1);
      fRadius = 0.5 * (extremity2 - extremity1).mag ();
    }
  }
}

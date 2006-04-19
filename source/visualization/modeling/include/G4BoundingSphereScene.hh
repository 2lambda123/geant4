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
// $Id: G4BoundingSphereScene.hh,v 1.16 2006-04-19 14:19:05 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  7th June 1997
// An artificial scene to reuse G4VScene code to calculate a bounding sphere.

#ifndef G4BOUNDINGSPHERESCENE_HH
#define G4BOUNDINGSPHERESCENE_HH

#include "G4VGraphicsScene.hh"
#include "G4VisExtent.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4Para.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"

class G4VModel;

class G4BoundingSphereScene: public G4VGraphicsScene {

public:
  G4BoundingSphereScene (G4VModel* pModel = 0);
  virtual ~G4BoundingSphereScene ();
  void PreAddSolid (const G4Transform3D& objectTransformation,
		    const G4VisAttributes&);
  void PostAddSolid () {}
  void AddSolid (const G4Box& s) {Accrue (s);}
  void AddSolid (const G4Cons& s) {Accrue (s);}
  void AddSolid (const G4Tubs& s) {Accrue (s);}
  void AddSolid (const G4Trd& s) {Accrue (s);}
  void AddSolid (const G4Trap& s) {Accrue (s);}
  void AddSolid (const G4Sphere& s) {Accrue (s);}
  void AddSolid (const G4Para& s) {Accrue (s);}
  void AddSolid (const G4Torus& s) {Accrue (s);}
  void AddSolid (const G4Polycone& s) {Accrue (s);}
  void AddSolid (const G4Polyhedra& s) {Accrue (s);}
  void AddSolid (const G4VSolid& s) {Accrue (s);}
  void AddCompound (const G4VTrajectory&) {}
  void AddCompound (const G4VHit&) {}
  G4VisExtent GetBoundingSphereExtent ();
  const G4Point3D& GetCentre() const {return fCentre;}
  G4double GetRadius() const {return fRadius;}

  void SetCentre(const G4Point3D& centre) {fCentre = centre;}

  ////////////////////////////////////////////////////////////////
  // The following 2 functions can be used by any code which wishes to
  // accrue a bounding sphere.  Just instantiate a
  // G4BoundingSphereScene and use AccrueBoundingSphere.

  void ResetBoundingSphere ();
  void AccrueBoundingSphere (const G4Point3D& centre,
			     G4double radius);

  ////////////////////////////////////////////////////////////////
  // Functions not used by required by the abstract interface.

  virtual void BeginPrimitives (const G4Transform3D&) {}
  virtual void EndPrimitives () {}
  virtual void BeginPrimitives2D () {}
  virtual void EndPrimitives2D () {}
  virtual void AddPrimitive (const G4Polyline&)   {}
  virtual void AddPrimitive (const G4Scale&)      {}
  virtual void AddPrimitive (const G4Text&)       {}
  virtual void AddPrimitive (const G4Circle&)     {}
  virtual void AddPrimitive (const G4Square&)     {}
  virtual void AddPrimitive (const G4Polymarker&) {}
  virtual void AddPrimitive (const G4Polyhedron&) {}
  virtual void AddPrimitive (const G4NURBS&)      {}

private:
  void Accrue (const G4VSolid& solid);
  G4VModel* fpModel;  // Instantiating code may optionally set this.
  G4Point3D fCentre;
  G4double fRadius;
  const G4Transform3D* fpObjectTransformation;
};

#endif

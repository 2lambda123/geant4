// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4PlacedPolyhedron.hh,v 1.5 1999-11-11 15:36:03 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef G4PLACEDPOLYHEDRON_HH
#define G4PLACEDPOLYHEDRON_HH

#include "G4Polyhedron.hh"
#include "G4Transform3D.hh"
#include "g4rw/tvordvec.h"

class G4PlacedPolyhedron {

public:

  G4PlacedPolyhedron ();
  G4PlacedPolyhedron (const G4Polyhedron&, const G4Transform3D&);

  // Uses default copy constructor, destructor and assignment.

  G4bool operator == (const G4PlacedPolyhedron& right) const {
    return this == &right;
  }

  const G4Polyhedron&  GetPolyhedron () const {return fPolyhedron;}
  const G4Transform3D& GetTransform  () const {return fTransform;}

  void SetPolyhedron (const G4Polyhedron& polyhedron) {
    fPolyhedron = polyhedron;
  }
  void SetTransform  (const G4Transform3D& transform) {
    fTransform = transform;
  }

private:

  G4Polyhedron fPolyhedron;
  G4Transform3D fTransform;

};

typedef G4RWTValOrderedVector<G4PlacedPolyhedron> G4PlacedPolyhedronList;

#endif /* G4PLACEDPOLYHEDRON_HH */

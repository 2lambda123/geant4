// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4QPDGCodeVector.hh,v 1.5 2001-09-17 14:19:49 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4QPDGCodeVector_h
#define G4QPDGCodeVector_h 1

// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      For information related to this code contact:
//      CERN, CN Division, ASD group
//      ---------------- G4QCandidateVector ----------------
//             by Mikhail Kossov, Sept 1999.
// Type defenition for Hadron definition in CHIPS model
// ---------------------------------------------------------------

#include "G4QPDGCode.hh"
#include "g4rw/tpordvec.h"

typedef G4RWTPtrOrderedVector<G4QPDGCode> G4QPDGCodeVector;

#endif



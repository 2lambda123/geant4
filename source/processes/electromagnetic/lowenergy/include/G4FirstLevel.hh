// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// -------------------------------------------------------------------
//      GEANT 4 class file 
//
//      For information related to this code contact:
//      CERN, IT Division, ASD group
//      CERN, Geneva, Switzerland
//
//      File name:     G4FirstLevel.hh
//
//      Author:        Alessandra Forti (Alessandra.Forti@cern.ch)
// 
//      Creation date: 1 Giugno 1999
//
//      Modifications: 
//      
// -------------------------------------------------------------------

#ifndef G4FIRSTLEVEL_HH
#define G4FIRSTLEVEL_HH

#include "G4Data.hh"
#include <rw/tpsrtvec.h>

class G4FirstLevel : public RWTPtrSortedVector< G4Data >{


public:

 ~G4FirstLevel();

  G4bool operator == (const G4FirstLevel& ) const;

  G4bool operator < (const G4FirstLevel&) const;

};

#endif







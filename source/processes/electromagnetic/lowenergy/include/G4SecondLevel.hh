// This code implementation is the intellectual property of
// the GEANT4 collaboration.
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
//      File name:     G4SecondLevel.hh
//
//      Author:        Alessandra Forti (Alessandra.Forti@cern.ch)
// 
//      Creation date: 1 Giugno 1999
//
//      Modifications: 24.04.01 V.Ivanchenko remove RogueWave
//      
// -------------------------------------------------------------------

// Class description:
// Utility for Low Energy electromagnetic e/photon processes
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef G4SECONDLEVEL_HH
#define G4SECONDLEVEL_HH

#include "G4FirstLevel.hh"
//#include "g4rw/tpordvec.h"

//class G4SecondLevel : public G4RWTPtrOrderedVector< G4FirstLevel >{
class G4SecondLevel : public G4std::vector< G4FirstLevel* >

{

public:

 ~G4SecondLevel();

  G4bool operator == (const G4SecondLevel& ) const;

  G4bool operator < (const G4SecondLevel&) const;

};

#endif







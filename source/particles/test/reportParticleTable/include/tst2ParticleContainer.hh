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
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: tst2ParticleContainer.hh,v 1.4 2001-07-11 10:02:11 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ---------------------------------------------------------------
#ifndef tst2ParticleContainer_h
#define tst2ParticleContainer_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "g4rw/tpsrtvec.h"

#include "tst2ContainerElement.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

class tst2ParticleContainer
{
 public:
   typedef G4RWTPtrSortedVector<tst2ContainerElement> tst2ParticleVector;

 public:
  //constructors
    tst2ParticleContainer();
    tst2ParticleContainer(const tst2ParticleContainer &);

  //destructor
    ~tst2ParticleContainer();

  //assignment operator
    tst2ParticleContainer & operator=(const tst2ParticleContainer &);

 public:
    // equality operators
    G4int operator==(const tst2ParticleContainer &right) const 
    {   return (this == &right);    }

    G4int operator!=(const tst2ParticleContainer &right) const 
    {   return (this != &right);    }

 public:
    G4int Insert( G4ParticleDefinition* );
    G4int entries() const;

 public:
    G4ParticleDefinition* GetParticle(G4int index) const;
    G4int                 GetEncoding(G4int index) const;
    G4ParticleDefinition* GetAntiParticle(G4int index) const;
 
 private:
    tst2ParticleVector*   pVector;
	G4ParticleTable*      pTable;

};

inline     
 G4int tst2ParticleContainer::entries() const
{
  return pVector->entries();
}

inline     
 G4ParticleDefinition*  tst2ParticleContainer::GetParticle(G4int index) const
{
  G4ParticleDefinition* p;
  p= 0;
  if ( (index>=0) && (index<entries()) ){
    p = ((*pVector)(index))->particle;
  }
  return p;
}

inline     
 G4int  tst2ParticleContainer::GetEncoding(G4int index) const
{
  G4int code = -1;
  if ( (index>=0) && (index<entries()) ){
    code = ((*pVector)(index))->encoding;
  }
  return code;
}
 


#endif

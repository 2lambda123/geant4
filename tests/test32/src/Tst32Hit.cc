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
// $Id: Tst32Hit.cc,v 1.2 2004-03-16 16:20:40 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "Tst32Hit.hh"

#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"

G4Allocator<Tst32Hit> Tst32HitAllocator;

Tst32Hit::Tst32Hit()
{;}

Tst32Hit::Tst32Hit(G4int value)
  :id(value)
{;}

Tst32Hit::~Tst32Hit()
{;}

Tst32Hit::Tst32Hit(const Tst32Hit &right)
  : G4VHit()
{
  *this = right;
}

const Tst32Hit& Tst32Hit::operator=(const Tst32Hit &right)
{
  if (this != &right) {
    edep = right.edep;
    id = right.id;
  }
  return *this;
}

int Tst32Hit::operator==(const Tst32Hit & right) const
{
  return (this==&right) ? 1 : 0;
}

void Tst32Hit::Draw()
{
 
}

void Tst32Hit::Print()
{
}



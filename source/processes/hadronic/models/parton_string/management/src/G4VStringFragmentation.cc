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
// $Id: G4VStringFragmentation.cc,v 1.2 2003-11-03 17:54:53 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4VStringFragmentation
#include "G4VStringFragmentation.hh"

G4VStringFragmentation::G4VStringFragmentation()
{
}

G4VStringFragmentation::G4VStringFragmentation(const G4VStringFragmentation &)
{
}

G4VStringFragmentation::~G4VStringFragmentation()
{
}

const G4VStringFragmentation & G4VStringFragmentation::operator=(const G4VStringFragmentation &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4VStringFragmentation::operator= meant to not be accessable");
  return *this;
}

int G4VStringFragmentation::operator==(const G4VStringFragmentation &) const
{
  return 0;
}

int G4VStringFragmentation::operator!=(const G4VStringFragmentation &) const
{
  return 1;
}


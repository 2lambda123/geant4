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
// $Id: G4He8GEMProbability.cc,v 1.2 2003-11-03 17:53:04 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999)
//


#include "G4He8GEMProbability.hh"

G4He8GEMProbability::G4He8GEMProbability() :
  G4GEMProbability(8,2,0.0) // A,Z,Spin
{
  SetExcitationEnergiesPtr(&ExcitEnergies);
  SetExcitationSpinsPtr(&ExcitSpins);
  SetExcitationLifetimesPtr(&ExcitLifetimes);
}


G4He8GEMProbability::G4He8GEMProbability(const G4He8GEMProbability &) : G4GEMProbability()
{
  throw G4HadronicException(__FILE__, __LINE__, "G4He8GEMProbability::copy_constructor meant to not be accessable");
}




const G4He8GEMProbability & G4He8GEMProbability::
operator=(const G4He8GEMProbability &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4He8GEMProbability::operator= meant to not be accessable");
  return *this;
}


G4bool G4He8GEMProbability::operator==(const G4He8GEMProbability &) const
{
  return false;
}

G4bool G4He8GEMProbability::operator!=(const G4He8GEMProbability &) const
{
  return true;
}




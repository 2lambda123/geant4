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
// $Id: G4C16GEMProbability.cc,v 1.2 2003-11-03 17:53:03 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999)
//


#include "G4C16GEMProbability.hh"

G4C16GEMProbability::G4C16GEMProbability() :
  G4GEMProbability(16,6,0.0) // A,Z,Spin
{
  SetExcitationEnergiesPtr(&ExcitEnergies);
  SetExcitationSpinsPtr(&ExcitSpins);
  SetExcitationLifetimesPtr(&ExcitLifetimes);
}


G4C16GEMProbability::G4C16GEMProbability(const G4C16GEMProbability &) : G4GEMProbability()
{
  throw G4HadronicException(__FILE__, __LINE__, "G4C16GEMProbability::copy_constructor meant to not be accessable");
}




const G4C16GEMProbability & G4C16GEMProbability::
operator=(const G4C16GEMProbability &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4C16GEMProbability::operator= meant to not be accessable");
  return *this;
}


G4bool G4C16GEMProbability::operator==(const G4C16GEMProbability &) const
{
  return false;
}

G4bool G4C16GEMProbability::operator!=(const G4C16GEMProbability &) const
{
  return true;
}




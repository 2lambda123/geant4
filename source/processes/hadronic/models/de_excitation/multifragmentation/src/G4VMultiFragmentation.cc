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
// $Id: G4VMultiFragmentation.cc,v 1.2 2003-11-03 17:53:05 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1998)

#include "G4VMultiFragmentation.hh"


G4VMultiFragmentation::G4VMultiFragmentation()
{
}

G4VMultiFragmentation::G4VMultiFragmentation(const G4VMultiFragmentation &)
{
    throw G4HadronicException(__FILE__, __LINE__, "G4VMultiFragmentation::copy_constructor meant to not be accessable");
}


G4VMultiFragmentation::~G4VMultiFragmentation()
{
}


const G4VMultiFragmentation & G4VMultiFragmentation::operator=(const G4VMultiFragmentation &)
{
    throw G4HadronicException(__FILE__, __LINE__, "G4VMultiFragmentation::operator= meant to not be accessable");
    return *this;
}


G4bool G4VMultiFragmentation::operator==(const G4VMultiFragmentation &) const
{
    throw G4HadronicException(__FILE__, __LINE__, "G4VMultiFragmentation::operator== meant to not be accessable");
    return false;
}

G4bool G4VMultiFragmentation::operator!=(const G4VMultiFragmentation &) const
{
    throw G4HadronicException(__FILE__, __LINE__, "G4VMultiFragmentation::operator=! meant to not be accessable");
    return true;
}




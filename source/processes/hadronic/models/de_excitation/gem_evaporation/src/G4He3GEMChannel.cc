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
// $Id: G4He3GEMChannel.cc,v 1.3 2005-06-04 13:25:25 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov. 1999)
//

#include "G4He3GEMChannel.hh"


const G4He3GEMChannel & G4He3GEMChannel::operator=(const G4He3GEMChannel & )
{
    throw G4HadronicException(__FILE__, __LINE__, "G4He3GEMChannel::operator= meant to not be accessable");
    return *this;
}

G4He3GEMChannel::G4He3GEMChannel(const G4He3GEMChannel & ): G4GEMChannel()
{
    throw G4HadronicException(__FILE__, __LINE__, "G4He3GEMChannel::CopyConstructor meant to not be accessable");
}

G4bool G4He3GEMChannel::operator==(const G4He3GEMChannel & right) const 
{
    return (this == (G4He3GEMChannel *) &right);
    //  return false;
}

G4bool G4He3GEMChannel::operator!=(const G4He3GEMChannel & right) const 
{
    return (this != (G4He3GEMChannel *) &right);
    //  return true;
}


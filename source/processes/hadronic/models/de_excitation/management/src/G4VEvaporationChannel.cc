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
// $Id: G4VEvaporationChannel.cc,v 1.2 2003-11-03 17:53:05 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//

#include "G4VEvaporationChannel.hh"

G4VEvaporationChannel::G4VEvaporationChannel(const G4VEvaporationChannel &)
{
 throw G4HadronicException(__FILE__, __LINE__, "G4VEvaporationChannel::copy_constructor meant to not be accessable");
}




const G4VEvaporationChannel & G4VEvaporationChannel::operator=(const G4VEvaporationChannel &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4VEvaporationChannel::operator= meant to not be accessable");
  return *this;
}


G4bool G4VEvaporationChannel::operator==(const G4VEvaporationChannel &right) const
{
    return (this == (G4VEvaporationChannel *) &right);
    //  return false;
}

G4bool G4VEvaporationChannel::operator!=(const G4VEvaporationChannel &right) const
{
    return (this != (G4VEvaporationChannel *) &right);
    //  return true;
}



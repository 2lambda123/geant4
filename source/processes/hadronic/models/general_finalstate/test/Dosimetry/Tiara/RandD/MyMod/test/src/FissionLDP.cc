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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: FissionLDP.cc,v 1.1 2003-10-08 12:32:19 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//


#include "FissionLDP.hh"


FissionLDP::
FissionLDP(const FissionLDP &right)
{
    G4Exception("G4FissionLevelDensityParameter::copy_constructor meant to not be accessable");
}


const FissionLDP & FissionLDP::
operator=(const FissionLDP &right)
{
    G4Exception("G4FissionLevelDensityParameter::operator= meant to not be accessable");
    return *this;
}


G4bool FissionLDP::
operator==(const FissionLDP &right) const
{
    return false;
}

G4bool FissionLDP::
operator!=(const FissionLDP &right) const
{
    return true;
}


G4double FissionLDP::
LevelDensityParameter(const G4int A,const G4int Z,const G4double U) const 
{
    G4double EvapLDP = theEvaporationLevelDensityParameter.LevelDensityParameter(A,Z,U);

    if (Z >= 89) return 1.04*EvapLDP;
    else if (Z >= 85) return (1.04*(1./MeV) + 0.01*(89-Z))*EvapLDP;
    else return 1.08*EvapLDP;
}

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4FissionLevelDensityParameter.cc,v 1.4 2006-06-29 20:13:37 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//


#include "G4FissionLevelDensityParameter.hh"
#include "G4HadronicException.hh"


G4FissionLevelDensityParameter::
G4FissionLevelDensityParameter(const G4FissionLevelDensityParameter &) : G4VLevelDensityParameter()
{
    throw G4HadronicException(__FILE__, __LINE__, "G4FissionLevelDensityParameter::copy_constructor meant to not be accessable");
}


const G4FissionLevelDensityParameter & G4FissionLevelDensityParameter::
operator=(const G4FissionLevelDensityParameter &)
{
    throw G4HadronicException(__FILE__, __LINE__, "G4FissionLevelDensityParameter::operator= meant to not be accessable");
    return *this;
}


G4bool G4FissionLevelDensityParameter::
operator==(const G4FissionLevelDensityParameter &) const
{
    return false;
}

G4bool G4FissionLevelDensityParameter::
operator!=(const G4FissionLevelDensityParameter &) const
{
    return true;
}


G4double G4FissionLevelDensityParameter::
LevelDensityParameter(const G4int A,const G4int Z,const G4double U) const 
{
    G4double EvapLDP = theEvaporationLevelDensityParameter.LevelDensityParameter(A,Z,U);

    if (Z >= 89) return 1.04*EvapLDP;
    else if (Z >= 85) return (1.04*(1./MeV) + 0.01*(89-Z))*EvapLDP;
    else return 1.08*EvapLDP;
}

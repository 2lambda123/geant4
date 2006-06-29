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
// $Id: G4NucleiPropertiesTableA.cc,v 1.10 2006-06-29 19:25:42 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1997
//      CERN Geneva Switzerland
//
//
//      File name:     G4NucleiPropertiesTable.cc 
//
//      Authors:       Vicente Lara (Vicente.Lara@cern.ch)
//                     Christian V'olcker (Christian.Volcker@cern.ch),
//
//      Creation date: November 1997
//
//      Modifications: 
// -------------------------------------------------------------------
// Migrate into particles category by H.Kurashige (17 Nov. 98)
// Remove "theInstance"  by H.Kurashige (12 Dec. 03)

#include "G4NucleiPropertiesTable.hh"

// Class G4NucleiPropertiesTable 

// Determine the table index for a Nuclide with Z protons and A nucleons

G4int G4NucleiPropertiesTable::GetIndex(G4int Z, G4int A) 
{
  if(A>G4NucleiPropertiesTable::MaxA) {
    G4Exception("G4NucleiPropertiesTable::GetIndex",
		"Illegal arguemnt",
		EventMustBeAborted,"Nucleon number larger than 273!");
  } else if(A<1) {
    G4Exception("G4NucleiPropertiesTable::GetIndex",
		"Illegal arguemnt",
		EventMustBeAborted," Nucleon number is negative!"); 
  } else if(Z>A) {
    G4Exception("G4NucleiPropertiesTable::GetIndex",
		"Illegal arguemnt",
		EventMustBeAborted, "Nucleon number smaller than Z!"); 
  }
   
  G4int i = shortTable[A-1];
  while ( i < shortTable[A] ) {
    if (indexArray[0][i] != Z ) {
      i++;
    } else {
      return i;
    }
  }
  return -1;
}


G4int G4NucleiPropertiesTable::MinZ(G4int A)
{
  G4int i = shortTable[A-1];
  return indexArray[0][i];
}


G4int G4NucleiPropertiesTable::MaxZ(G4int A)
{
  G4int i = shortTable[A]-1;
  return indexArray[0][i];
}




G4double G4NucleiPropertiesTable::GetNuclearMass(G4int Z, G4int A)
{
  G4int i=GetIndex(Z, A);	
  if (i >= 0){
    const G4double NuclearMass = GetAtomicMass(Z,A) - G4double(Z)*electron_mass_c2 +
      1.433e-5*MeV*std::pow(G4double(Z),2.39);
    return NuclearMass;
  } else { 
    return 0.0;
  }
}




G4double G4NucleiPropertiesTable::GetMassExcess(G4int Z, G4int A) 
{
    G4int i=GetIndex(Z, A);
    if (i >= 0) {
      return MassExcess[i]*keV;
    } else {
        return 0.0;
    }
}

G4double G4NucleiPropertiesTable::GetBindingEnergy(G4int Z, G4int A)
{
  G4int i=GetIndex(Z, A);
  if (i >= 0){
      return (G4double(A-Z)*MassExcess[0] + G4double(Z)*MassExcess[1] - MassExcess[i])*keV;
  } else { 
    return 0.0;
  }
}

G4double  G4NucleiPropertiesTable::GetBetaDecayEnergy(G4int Z, G4int A)
{
  G4int i=GetIndex(Z, A);
    if (i >= 0){
      return BetaEnergy[i]*keV;
    } else { 
      return 0.0;
    }
}

G4double  G4NucleiPropertiesTable::GetAtomicMass(G4int Z, G4int A)
{
  G4int i=GetIndex(Z, A);	
  if (i >= 0){
    return MassExcess[i]*keV + G4double(A)*amu_c2;
  } else { 
    return 0.0;
  }	
}


G4bool G4NucleiPropertiesTable::IsInTable(G4int Z, G4int A)
{
  return (Z <= A && A >= 1 && A <= 273 && Z >= 0 && Z <= 110 && GetIndex(Z, A) >= 0);
}


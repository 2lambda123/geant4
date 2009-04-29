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
// $Id: G4NucleiPropertiesTable.hh,v 1.15 2009-04-29 14:05:12 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class file
//
//      File name:     G4NucleiPropertiesTable.cc 
//
//      Authors:       Vicente Lara (Vicente.Lara@cern.ch)
//                     Christian V"olcker (Christian.Volcker@cern.ch),
//
//      Creation date: November 1997
//
//      Modifications: 
//
// Migrate into particles category by H.Kurashige (17 Nov. 98)
// Subtract electron mass by H.Kurashige
// Avoid substraction of electron mass in Atomic masses by V.Lara (12 May 99)
// Remove "theInstance"  by H.Kurashige (12 Dec. 03)
// -------------------------------------------------------------------
#include "globals.hh"
#include <cmath>


#ifndef G4NucleiPropertiesTable_h
#define G4NucleiPropertiesTable_h 1

// Class Description
// Class: G4NucleiPropertiesTable
//	Encapsulates Data from G. Audi and A.H. Wapstra, Nucl.
//	Physics,A595 vol 4 p 409-480,
//	25. Dec. 1995.
//	Name, Z, A,
//	Mass Excess
//	Binding Energy
//	Beta Decay Energy
//	Atomic Mass

class G4NucleiProperties;

class G4NucleiPropertiesTable 
{
private:
  
  // Default constructor - this class should exist only once!
  G4NucleiPropertiesTable(){};

public:

  // Destructor (generated)
  ~G4NucleiPropertiesTable() { };

  enum  {nEntries = 2931,MaxA = 273}; 

  // Other Operations 
  // all methods are private and can be used only by G4NucleiProperties

  friend class G4NucleiProperties;  

private:

  // Operation: GetMassExcess
  static G4double GetMassExcess(G4int Z, G4int A); 

  // Operation: GetNuclearMass
  static G4double GetNuclearMass(G4int Z, G4int A);

  // Operation: GetBindingEnergy
  static G4double GetBindingEnergy(G4int Z, G4int A);

  // Operation: GetBetaDecayEnergy
  static G4double GetBetaDecayEnergy(G4int Z, G4int A);

  // Operation: GetAtomicMass .. in Geant4 Energy units!
  static G4double GetAtomicMass(G4int Z, G4int A);

  // Is the nucleus (A,Z) in table?
  static G4bool IsInTable(G4int Z, G4int A);

  static G4int MaxZ(G4int A);
  static G4int MinZ(G4int A);


private:

  // Operation: GetIndex
  static G4int GetIndex(G4int Z, G4int A);
  

  // Data Members for Class Attributes
  //----------------------------------  

  // The following arrays are static for allow inicialization.
  // The inicialization is Done in G4NucleiPropertiesTable.cc

  // Mass Excess
  static G4double MassExcess[nEntries];
  
  
  // Beta Decay Energy
  static G4double BetaEnergy[nEntries];

    
  // Table of Z (number of protons) and A (number of nucleons)
  //        indexArray[0][ ] --> Z
  //        indexArray[1][ ] --> A
  static G4int indexArray[2][nEntries];

  // Reduced Table of A for shorter index search.
  //         The index in this table coincide with A-1
  //         For each A value shortTable[A-1] has the index of the 1st occurrence in
  //         the indexArray[][]
  static G4int shortTable[MaxA+1];


};


#endif







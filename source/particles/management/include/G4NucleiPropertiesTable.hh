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
// $Id: G4NucleiPropertiesTable.hh,v 1.12 2004-12-02 08:08:58 kurasige Exp $
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
//                     Christian V"olcker (Christian.Volcker@cern.ch),
//
//      Creation date: November 1997
//
//      Testfiles:
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
 public: // With Description

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







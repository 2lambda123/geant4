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
// $Id: G4MaterialStoppingICRU73.hh,v 1.2 2008-10-30 21:51:35 alechner Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef G4MaterialStoppingICRU73_h
#define G4MaterialStoppingICRU73_h 1

//---------------------------------------------------------------------------
//
// ClassName:   G4MaterialStoppingICRU73
//
// Description: Data on stopping powers for light ions in compounds
//
// Author:      A.Ivantchenko 10.07.2008
//
// Modifications:
//
//----------------------------------------------------------------------------
//
// Class Description:
//
// Data on Stopping Powers from the ICRU73 report
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "globals.hh"
#include "G4LPhysicsFreeVector.hh"
#include <vector>

class G4MaterialStoppingICRU73
{
public:

  G4MaterialStoppingICRU73(G4bool splineFlag = true);

  ~G4MaterialStoppingICRU73();

  G4double GetDEDX(G4int ionZ, G4int idxMaterial, G4double kinEnergy);

  inline G4double GetDEDX(G4int ionZ, const G4String& NameMaterial, 
			  G4double kinEnergy);

  inline G4int GetMaterialIndex(const G4String& NameMaterial);

  // Function returns an unique index (>=0) for each ion-material couple (the 
  // return value is -1 if the couple is not found):
  inline G4int GetIonMaterialCoupleIndex(
                        G4int atomicNumber,            // Atomic number of ion 
                        const G4String& materialName); // Material name

  inline G4double GetDensity(G4int idxMaterial);

  inline G4String GetMaterialName(G4int idxMaterial);

  inline G4PhysicsVector* GetPhysicsVector(G4int ionZ, G4int idxMaterial);

  inline G4PhysicsVector* GetPhysicsVector(G4int ionZ, 
					   const G4String& NameMaterial);

  inline G4double GetLowerEnergyBoundary();

  inline G4double GetUpperEnergyBoundary();

private:

  void Initialise();

  void AddData(G4double* energy, G4double* stoppower, G4double factor);

  // hide assignment operator
  G4MaterialStoppingICRU73 & operator=(const G4MaterialStoppingICRU73 &right);
  G4MaterialStoppingICRU73(const G4MaterialStoppingICRU73&);

  G4bool   spline;
  G4int    Z[16];
  G4double A[16];
  G4String MatName[31];
  G4double Density[31];

  // Lower and upper energy boundaries for dE/dx vectors:
  G4double lowerEnergyBoundary;
  G4double upperEnergyBoundary;

  std::vector<G4LPhysicsFreeVector*>  dedx;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double 
G4MaterialStoppingICRU73::GetDEDX(G4int ionZ, const G4String& NameMaterial, 
				  G4double kinEnergy)
{
  return GetDEDX(ionZ, GetMaterialIndex(NameMaterial), kinEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4int
G4MaterialStoppingICRU73::GetMaterialIndex(const G4String& NameMaterial)
{
  for (G4int idx=0; idx<31; idx++){
    if(MatName[idx] == NameMaterial) return idx;
  }
  return -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4int
G4MaterialStoppingICRU73::GetIonMaterialCoupleIndex(
                       G4int atomicNumber,             // Atomic number of ion
                       const G4String& materialName) { // Material name

  if(atomicNumber < 3 || atomicNumber > 18) return -1;

  G4int materialIndex = GetMaterialIndex(materialName);
  if(materialIndex < 0) return -1;

  return materialIndex * 16 + atomicNumber - 3;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double 
G4MaterialStoppingICRU73::GetDensity(G4int idxMaterial)
{
  if( idxMaterial < 0 || idxMaterial > 30) return 0.0;
  return Density[idxMaterial];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4String G4MaterialStoppingICRU73::GetMaterialName(G4int idxMaterial)
{
  G4String s = "";
  if( idxMaterial < 0 || idxMaterial > 30) return s;
  return MatName[idxMaterial];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4PhysicsVector* 
G4MaterialStoppingICRU73::GetPhysicsVector(G4int ionZ, G4int idxMaterial)
{
  if(ionZ < 3 || ionZ > 18 || idxMaterial < 0 || idxMaterial > 30) return 0; 
  G4int idx = idxMaterial*16 + ionZ - 3;
  return dedx[idx];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4PhysicsVector* 
G4MaterialStoppingICRU73::GetPhysicsVector(G4int ionZ, 
					   const G4String& NameMaterial)
{
  return GetPhysicsVector(ionZ, GetMaterialIndex(NameMaterial));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double
G4MaterialStoppingICRU73::GetLowerEnergyBoundary() {

  return lowerEnergyBoundary;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double
G4MaterialStoppingICRU73::GetUpperEnergyBoundary() {

  return upperEnergyBoundary;
}

#endif

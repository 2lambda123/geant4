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
// $Id: G4DensityEffectData.hh,v 1.1 2009-10-29 17:13:45 bagoulia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

//---------------------------------------------------------------------------
//
// GEANT4 Class file
//
// Description: Data on density effect
//
// Author:    A.Ivanchenko 28.10.2009
//
// Modifications:
// 
// 
//
//----------------------------------------------------------------------------
//
//  Data are taken from:  
//  R.M. Sternheimer et al. Density Effect For The Ionization Loss Of Charged 
//  Particles In Various Substances. Atom. Data Nucl. Data Tabl. 30 (1984) 261-271. 

#ifndef DensityEffectData_h
#define DensityEffectData_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4int NDENSDATA  = 278;
const G4int NDENSARRAY = 9;

class G4DensityEffectData 
{
public:

  G4DensityEffectData();

  ~G4DensityEffectData();

  G4int GetIndex(G4int ionZ);

  G4int GetIndex(const G4String& matName);

  inline G4float GetPlasmaEnergy(G4int idx); 
  inline G4float GetAdjustmentFactor(G4int idx); 
  inline G4float GetCdensity(G4int idx); 
  inline G4float GetX0density(G4int idx); 
  inline G4float GetX1density(G4int idx); 
  inline G4float GetAdensity(G4int idx); 
  inline G4float GetMdensity(G4int idx); 
  inline G4float GetDelta0density(G4int idx); 
  inline G4float GetErrorDensity(G4int idx); 

private:

  void Initialize();

  // Function for creating a physics vector
  void AddMaterial(G4float* val, const G4String& matName);

  // Assignment operator and copy constructor
  G4DensityEffectData & operator=(const G4DensityEffectData &right);
  G4DensityEffectData(const G4DensityEffectData&);

  // Vectors containing the atomic numbers and names of the materials
  G4float data[NDENSDATA][NDENSARRAY];
  std::vector<G4String> names;

  G4int index;

};

inline G4float G4DensityEffectData::GetPlasmaEnergy(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[0][idx]; }
  return x;
} 

inline G4float G4DensityEffectData::GetAdjustmentFactor(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[1][idx]; }
  return x;
} 

inline G4float G4DensityEffectData::GetCdensity(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[2][idx]; }
  return x;
} 

inline G4float G4DensityEffectData::GetX0density(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[3][idx]; }
  return x;
} 

inline G4float G4DensityEffectData::GetX1density(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[4][idx]; }
  return x;
} 

inline G4float G4DensityEffectData::GetAdensity(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[5][idx]; }
  return x;
}
 
inline G4float G4DensityEffectData::GetMdensity(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[6][idx]; }
  return x;
} 

inline G4float G4DensityEffectData::GetDelta0density(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[7][idx]; }
  return x;
} 

inline G4float G4DensityEffectData::GetErrorDensity(G4int idx)
{
  G4float x = DBL_MAX;
  if(idx >= 0 && idx < NDENSDATA) { x = data[8][idx]; }
  return x;
} 


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
 

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
// $Id: G4eCoulombScatteringModel.hh,v 1.29 2008-06-10 10:24:15 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4eCoulombScatteringModel
//
// Author:        Vladimir Ivanchenko 
//
// Creation date: 19.02.2006
//
// Modifications:
// 01.08.06 V.Ivanchenko extend upper limit of table to TeV and review the
//          logic of building - only elements from G4ElementTable
// 08.08.06 V.Ivanchenko build internal table in ekin scale, introduce faclim
// 19.08.06 V.Ivanchenko add inline function ScreeningParameter and
//                       make some members protected
// 09.10.07 V.Ivanchenko reorganized methods, add cut dependence in scattering off e- 
//
// Class Description:
//
// Implementation of eCoulombScattering of pointlike charge particle 
// on Atomic Nucleus for interval of scattering anles in Lab system 
// thetaMin - ThetaMax, nucleus recoil is neglected.
//   The model based on analysis of J.M.Fernandez-Varea et al. 
// NIM B73(1993)447 originated from G.Wentzel Z.Phys. 40(1927)590 with 
// screening parameter from H.A.Bethe Phys. Rev. 89 (1953) 1256.
// 

// -------------------------------------------------------------------
//

#ifndef G4eCoulombScatteringModel_h
#define G4eCoulombScatteringModel_h 1

#include "G4VEmModel.hh"
#include "G4PhysicsTable.hh"
#include "G4NistManager.hh"
#include "globals.hh"

class G4ParticleChangeForGamma;
class G4ParticleDefinition;

class G4eCoulombScatteringModel : public G4VEmModel
{

public:

  G4eCoulombScatteringModel(G4double thetaMin = 0.0, G4double thetaMax = pi,
			    G4double tlim = TeV*TeV,
			    const G4String& nam = "eCoulombScattering");
 
  virtual ~G4eCoulombScatteringModel();

  virtual void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  virtual G4double ComputeCrossSectionPerAtom(
                                const G4ParticleDefinition*,
				G4double kinEnergy, 
				G4double Z, 
				G4double A, 
				G4double cut,
				G4double emax);

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin,
				 G4double maxEnergy);

protected:

  void SelectAtomRandomly();

  G4double SelectIsotope();

  G4double SampleCosineTheta();

  inline void DefineMaterial(const G4MaterialCutsCouple*);

  inline void SetupParticle(const G4ParticleDefinition*);

  inline void SetupKinematic(G4double kinEnergy, G4double cut);
  
  inline void SetupTarget(G4double Z, G4double A, G4double kinEnergy); 

private:

  void ComputeMaxElectronScattering(G4double cut);

  inline G4double FormFactorMev2(G4double Z, G4double A);

  // hide assignment operator
  G4eCoulombScatteringModel & operator=(const G4eCoulombScatteringModel &right);
  G4eCoulombScatteringModel(const  G4eCoulombScatteringModel&);

protected:
 
  const G4ParticleDefinition* theProton;
  const G4ParticleDefinition* theElectron;
  const G4ParticleDefinition* thePositron;

  G4ParticleTable*          theParticleTable; 
  G4ParticleChangeForGamma* fParticleChange;
  G4NistManager*            fNistManager;
  const G4DataVector*       currentCuts;

  const G4MaterialCutsCouple* currentCouple;
  const G4Material*           currentMaterial;
  const G4Element*            currentElement;
  G4int                       currentMaterialIndex;

  G4double                  coeff;
  G4double                  constn;
  G4double                  cosThetaMin;
  G4double                  cosThetaMax;
  G4double                  cosTetMinNuc;
  G4double                  cosTetMaxNuc;
  G4double                  cosTetMaxElec;
  G4double                  cosTetMaxHad;
  G4double                  cosTetLimit;
  G4double                  q2Limit;
  G4double                  elecXSection;
  G4double                  nucXSection;
  G4double                  ecut;
  G4double                  xsece[40];
  G4double                  xsecn[40];
  G4double                  xsect[40];

  // projectile
  const G4ParticleDefinition* particle;

  G4double                  chargeSquare;
  G4double                  spin;
  G4double                  mass;
  G4double                  tkin;
  G4double                  mom2;
  G4double                  invbeta2;
  G4double                  etag;

  // target
  G4double                  targetZ;
  G4double                  targetA;
  G4double                  screenZ;
  G4double                  formfactA;
  G4int                     idxelm;

private:

  G4double                  a0;
  G4double                  lowKEnergy;
  G4double                  highKEnergy;
  G4double                  alpha2;
  G4double                  faclim;
  G4double                  FF[100];

  G4bool                    isInitialised;             
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline
void G4eCoulombScatteringModel::DefineMaterial(const G4MaterialCutsCouple* cup) 
{ 
  if(cup != currentCouple) {
    currentCouple = cup;
    currentMaterial = cup->GetMaterial();
    currentMaterialIndex = currentCouple->GetIndex(); 
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline 
void G4eCoulombScatteringModel::SetupParticle(const G4ParticleDefinition* p)
{
  // Initialise mass and charge
  if(p != particle) {
    particle = p;
    mass = particle->GetPDGMass();
    spin = particle->GetPDGSpin();
    G4double q = particle->GetPDGCharge()/eplus;
    chargeSquare = q*q;
    tkin = 0.0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4eCoulombScatteringModel::SetupKinematic(G4double ekin, 
						      G4double cut)
{
  if(ekin != tkin || ecut != cut) {
    cosTetMinNuc = cosThetaMin;
    cosTetMaxNuc = cosThetaMax;
    if(cut > ekin && cosThetaMin < 1.0) {
      cosTetMinNuc = ekin*(1.0 + cosThetaMin)/cut - 1.0;  
    }
    tkin  = ekin;
    mom2  = tkin*(tkin + 2.0*mass);
    invbeta2 = 1.0 +  mass*mass/mom2;
    ComputeMaxElectronScattering(cut);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  
inline void G4eCoulombScatteringModel::SetupTarget(G4double Z, G4double A, 
						   G4double e)
{
  if(Z != targetZ || A != targetA || e != etag) {
    etag    = e; 
    targetZ = Z;
    targetA = A;
    G4double x = fNistManager->GetZ13(Z);
    screenZ = a0*x*x*(1.13 + 3.76*invbeta2*Z*Z*chargeSquare*alpha2)/mom2;
    //screenZ = a0*x*x*(1.13 + 3.76*Z*Z*chargeSquare*alpha2)/mom2;
    cosTetLimit = cosTetMaxNuc;
    if(particle == theProton && A < 1.5 && cosTetMaxNuc < 0.0) 
      cosTetLimit = 0.0;
    formfactA = mom2*FormFactorMev2(Z, A);
  } 
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  
inline G4double G4eCoulombScatteringModel::FormFactorMev2(G4double Z, 
							  G4double A)
{
  // A.V. Butkevich et al., NIM A 488 (2002) 282
  G4int iz = G4int(Z);
  if(iz > 99) iz = 99;
  G4double res = FF[iz];
  if(res == 0.0) { 
    res = constn*std::pow(A,0.54);
    FF[iz] = res;
  }
  cosTetMaxHad = 1.0 - 0.5*q2Limit/(res*mom2);
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

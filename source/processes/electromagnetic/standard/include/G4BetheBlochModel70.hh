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
// $Id: G4BetheBlochModel70.hh,v 1.1 2005-02-26 22:01:07 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4BetheBlochModel70
//
// Author:        Vladimir Ivanchenko on base of Laszlo Urban code
//
// Creation date: 03.01.2002
//
// Modifications:
//
// 23-12-02 V.Ivanchenko change interface in order to moveto cut per region
// 24-01-03 Make models region aware (V.Ivanchenko)
// 13-02-03 Add name (V.Ivanchenko)
// 12-11-03 Fix for GenericIons (V.Ivanchenko)
//
// Class Description:
//
// Implementation of Bethe-Bloch model of energy loss and
// delta-electron production by heavy charged particles

// -------------------------------------------------------------------
//

#ifndef G4BetheBlochModel70_h
#define G4BetheBlochModel70_h 1

#include "G4VEmModel.hh"

class G4EmCorrections;
class G4BraggModel;

class G4BetheBlochModel70 : public G4VEmModel
{

public:

  G4BetheBlochModel70(const G4ParticleDefinition* p = 0, const G4String& nam = "BetheBloch");

  virtual ~G4BetheBlochModel70();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  G4double HighEnergyLimit(const G4ParticleDefinition* p);

  G4double LowEnergyLimit(const G4ParticleDefinition* p);

  void SetHighEnergyLimit(G4double e) {highKinEnergy = e;};

  void SetLowEnergyLimit(G4double e) {lowKinEnergy = e;};

  G4double MinEnergyCut(const G4ParticleDefinition*,
                        const G4MaterialCutsCouple*);

  G4bool IsInCharge(const G4ParticleDefinition*);

  G4double ComputeDEDX(const G4MaterialCutsCouple*,
                       const G4ParticleDefinition*,
                             G4double kineticEnergy,
                             G4double cutEnergy);

  G4double ComputeDEDX(const G4Material*,
                       const G4ParticleDefinition*,
                             G4double kineticEnergy,
                             G4double cutEnergy);

  G4double CrossSection(const G4MaterialCutsCouple*,
                        const G4ParticleDefinition*,
                              G4double kineticEnergy,
                              G4double cutEnergy,
                              G4double maxEnergy);

  G4DynamicParticle* SampleSecondary(
                                const G4MaterialCutsCouple*,
                                const G4DynamicParticle*,
                                      G4double tmin,
                                      G4double maxEnergy);

  std::vector<G4DynamicParticle*>* SampleSecondaries(
                                const G4MaterialCutsCouple*,
                                const G4DynamicParticle*,
                                      G4double tmin,
                                      G4double maxEnergy);

  G4double MaxSecondaryEnergy(const G4DynamicParticle*);

protected:

  G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
                                    G4double kinEnergy);

private:

  void SetParticle(const G4ParticleDefinition* p);

  // hide assignment operator
  G4BetheBlochModel70 & operator=(const  G4BetheBlochModel70 &right);
  G4BetheBlochModel70(const  G4BetheBlochModel70&);

  const G4ParticleDefinition* particle;
  G4EmCorrections*            corr;
  G4double mass;
  G4double spin;
  G4double chargeSquare;
  G4double ratio;
  G4double highKinEnergy;
  G4double lowKinEnergy;
  G4double twoln10;
  G4double bg2lim;
  G4double taulim;
  G4bool   isIon;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4BetheBlochModel70::MaxSecondaryEnergy(
          const G4ParticleDefinition* pd,
                G4double kinEnergy) 
{
  if(pd != particle) SetParticle(pd);
  G4double tau  = kinEnergy/mass;
  G4double tmax = 2.0*electron_mass_c2*tau*(tau + 2.) /
                  (1. + 2.0*(tau + 1.)*ratio + ratio*ratio);

  return tmax;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4BetheBlochModel70::MaxSecondaryEnergy(const G4DynamicParticle* dp)
{
  if(isIon) {
    mass =  dp->GetMass();
    ratio = electron_mass_c2/mass;
  }
  G4double tau  = dp->GetKineticEnergy()/mass;
  G4double tmax = electron_mass_c2 * 
    std::min(2.0*tau*(tau + 2.) /(1. + 2.0*(tau + 1.)*ratio + ratio*ratio),  
             51200.*std::pow(proton_mass_c2/mass,0.66667));

  return tmax;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4BetheBlochModel70::SetParticle(const G4ParticleDefinition* p)
{
  particle = p;
  mass = particle->GetPDGMass();
  spin = particle->GetPDGSpin();
  G4double q = particle->GetPDGCharge()/eplus;
  chargeSquare = q*q;
  ratio = electron_mass_c2/mass;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4BetheBlochModel70::ComputeDEDX(const G4MaterialCutsCouple* couple,
                                                 const G4ParticleDefinition* p,
                                                       G4double kineticEnergy,
                                                       G4double cut)
{
  return ComputeDEDX(couple->GetMaterial(), p, kineticEnergy, cut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

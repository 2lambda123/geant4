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
// $Id: G4MuPairProductionModel.hh,v 1.13 2004-10-25 13:32:52 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4MuPairProductionModel
//
// Author:        Vladimir Ivanchenko on base of Laszlo Urban code
//
// Creation date: 18.05.2002
//
// Modifications:
//
// 23-12-02 Change interface in order to move to cut per region (V.Ivanchenko)
// 27-01-03 Make models region aware (V.Ivanchenko)
// 13-02-03 Add name (V.Ivanchenko)
// 10-02-04 Update parameterisation using R.Kokoulin model (V.Ivanchenko)
// 10-02-04 Add lowestKinEnergy (V.Ivanchenko)
//

//
// Class Description:
//
// Implementation of e+e- pair production by muons
//

// -------------------------------------------------------------------
//

#ifndef G4MuPairProductionModel_h
#define G4MuPairProductionModel_h 1

#include "G4VEmModel.hh"
#include <vector>

class G4Element;

class G4MuPairProductionModel : public G4VEmModel
{

public:

  G4MuPairProductionModel(const G4ParticleDefinition* p = 0, const G4String& nam = "MuPairProd");

  virtual ~G4MuPairProductionModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  G4double HighEnergyLimit(const G4ParticleDefinition* p);

  G4double LowEnergyLimit(const G4ParticleDefinition* p);

  void SetHighEnergyLimit(G4double e) {highKinEnergy = e;};

  void SetLowEnergyLimit(G4double e) {lowKinEnergy = e;};

  void SetLowestKineticEnergy(G4double e) {lowestKinEnergy = e;};

  G4double MinEnergyCut(const G4ParticleDefinition*,
                        const G4MaterialCutsCouple*);

  G4bool IsInCharge(const G4ParticleDefinition*);

  G4double ComputeDEDX(const G4MaterialCutsCouple*,
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

  virtual G4double MaxSecondaryEnergy(
				const G4DynamicParticle* dynParticle);

protected:

  virtual G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
    				            G4double kineticEnergy);

//private:
public:

  G4double ComputMuPairLoss(G4double Z, G4double tkin, G4double cut, G4double tmax);

  G4double ComputeMicroscopicCrossSection(G4double tkin,
                                          G4double Z,
                                          G4double cut);

  G4double ComputeDMicroscopicCrossSection(G4double tkin,
                                           G4double Z,
                                           G4double pairEnergy);
private:

  const G4Element* SelectRandomAtom(G4double kinEnergy, G4double dt, G4int it,
                              const G4MaterialCutsCouple* couple);

  void MakeSamplingTables();

  void SetCurrentElement(G4double Z);

  G4double InterpolatedIntegralCrossSection(G4double dt, G4double dz,
                                            G4int iz, G4int it, G4int iy, G4double z);

  // hide assignment operator
  G4MuPairProductionModel & operator=(const  G4MuPairProductionModel &right);
  G4MuPairProductionModel(const  G4MuPairProductionModel&);

  G4double minPairEnergy;
  G4double highKinEnergy;
  G4double lowKinEnergy;
  G4double lowestKinEnergy;

  G4double factorForCross;
  G4double sqrte;
  G4double particleMass;
  G4double currentZ;
  G4double z13;
  G4double z23;
  G4double lnZ;


  const G4ParticleDefinition* particle;

  // tables for sampling
  G4int nzdat;
  G4int ntdat;
  G4int nbiny;
  size_t nmaxElements;
  static G4double zdat[5],adat[5],tdat[8],xgi[8],wgi[8];
  G4double ya[1001],proba[5][8][1001];

  G4double ymin;
  G4double ymax;
  G4double dy;

  G4bool  samplingTablesAreFilled;
  std::vector<G4double>   partialSum;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline
G4double G4MuPairProductionModel::MaxSecondaryEnergy(
				 const G4DynamicParticle* dynParticle)
{
  G4double e = dynParticle->GetKineticEnergy();
  G4double maxPairEnergy = e + particleMass*(1.0 - 0.75*sqrte*z13);
  return maxPairEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline
G4double G4MuPairProductionModel::MaxSecondaryEnergy(
                                 const G4ParticleDefinition*,
    				       G4double kineticEnergy)
{
  G4double maxPairEnergy = kineticEnergy + particleMass*(1.0 - 0.75*sqrte*z13);
  return maxPairEnergy;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline
void G4MuPairProductionModel::SetCurrentElement(G4double Z)
{
  if(Z != currentZ) {
    currentZ = Z;
    z13 = pow(Z,0.333333333);
    z23 = z13*z13;
    lnZ = log(Z);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline
G4double G4MuPairProductionModel::InterpolatedIntegralCrossSection(G4double dt, G4double dz,
         G4int iz, G4int it, G4int iy, G4double z)
{
  G4double fac =  1./(zdat[iz]  *(zdat[iz]  +1.));
  G4double fac1 = 1./(zdat[iz-1]*(zdat[iz-1]+1.));
  G4double f0 = fac1*proba[iz-1][it-1][iy] + 
                (fac*proba[iz][it-1][iy]-fac1*proba[iz-1][it-1][iy])*dz;
  G4double f1 = fac1*proba[iz-1][it  ][iy] + 
                (fac*proba[iz][it  ][iy]-fac1*proba[iz-1][it  ][iy])*dz;
  return (f0 + (f1-f0)*dt)*z*(z+1.);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

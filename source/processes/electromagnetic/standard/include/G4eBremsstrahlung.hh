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
// $Id: G4eBremsstrahlung.hh,v 1.28 2005-03-28 23:07:54 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4eBremsstrahlung
//
// Author:        Laszlo Urban
//
// Creation date: 24.06.1996
//
// Modifications:
//
// 01-10-96 new type G4OrderedTable;  ComputePartialSumSigma()
// 20-03-97 new energy loss+ionisation+brems scheme, L.Urban
// 01-09-98 new method  PrintInfo()
// 10-02-00 modifications , new e.m. structure, L.Urban
// 07-08-00 new cross section/en.loss parametrisation, LPM flag , L.Urban
// 09-08-01 new methods Store/Retrieve PhysicsTable (mma)
// 19-09-01 come back to previous process name "eBrem"
// 29-10-01 all static functions no more inlined (mma)
// 07-01-02 new design of em processes (V.Ivanchenko)
// 26-12-02 secondary production moved to derived classes (VI)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 05-02-03 Fix compilation warnings (V.Ivanchenko)
// 08-08-03 STD substitute standard  (V.Ivanchenko)
// 17-10-03 PrintInfoDefinition - virtual (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 21-01-04 Migrade to G4ParticleChangeForLoss (V.Ivanchenko)
// 04-11-04 add gamma threshold (V.Ivanchenko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
//
//
// Class Description:
//
// This class manages the bremsstrahlung for e-/e+
// it inherites from G4VContinuousDiscreteProcess via G4VEnergyLoss.
//

// -------------------------------------------------------------------
//

#ifndef G4eBremsstrahlung_h
#define G4eBremsstrahlung_h 1

#include "G4VEnergyLossProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

class G4Material;

class G4eBremsstrahlung : public G4VEnergyLossProcess
{

public:

  G4eBremsstrahlung(const G4String& name = "eBrem", G4double thresh=DBL_MAX);

  virtual ~G4eBremsstrahlung();

  G4bool IsApplicable(const G4ParticleDefinition& p);

  virtual G4double MinPrimaryEnergy(const G4ParticleDefinition*,
                                    const G4Material*, G4double cut);

  virtual std::vector<G4Track*>* SecondariesAlongStep(
                             const G4Step&,
			           G4double&,
			           G4double&,
                                   G4double&);

  virtual void SecondariesPostStep(
                                   G4VEmModel*,
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle*,
                                   G4double&,
                                   G4double&);

  virtual void PrintInfoDefinition();
  // Print out of the class parameters

  void SetGammaThreshold(G4double val);

  G4double GammaThreshold() const;

protected:

  virtual void InitialiseEnergyLossProcess(const G4ParticleDefinition*,
                                           const G4ParticleDefinition*);

  virtual G4double MaxSecondaryEnergy(const G4DynamicParticle* dynParticle);

private:

  // hide assignment operator
  G4eBremsstrahlung & operator=(const G4eBremsstrahlung &right);
  G4eBremsstrahlung(const G4eBremsstrahlung&);

  const G4ParticleDefinition* particle;
  G4double gammaThreshold;
  G4bool   isInitialised;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4VEmModel.hh"

inline G4bool G4eBremsstrahlung::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4eBremsstrahlung::MinPrimaryEnergy(const G4ParticleDefinition*,
                                                    const G4Material*,
						          G4double cut)
{
  return cut;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline std::vector<G4Track*>* G4eBremsstrahlung::SecondariesAlongStep(
                             const G4Step&,
			           G4double&,
			           G4double&,
                                   G4double&)
{
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4eBremsstrahlung::MaxSecondaryEnergy(const G4DynamicParticle* dynParticle)
{
  return dynParticle->GetKineticEnergy();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4eBremsstrahlung::SecondariesPostStep(
                                                      G4VEmModel* model,
                                                const G4MaterialCutsCouple* couple,
                                                const G4DynamicParticle* dp,
                                                      G4double& tcut,
                                                      G4double& kinEnergy)
{
  G4ThreeVector dir = dp->GetMomentumDirection();
  G4double edep;
  std::vector<G4DynamicParticle*>* newp = model->PostStepDoIt(couple, particle, kinEnergy, edep, 
                                          dir, dir, tcut, kinEnergy);
  G4DynamicParticle* gamma = (*newp)[0];
  G4double gammaEnergy = gamma->GetKineticEnergy();

  G4int nSecond = 1;
  if(gammaEnergy > gammaThreshold) nSecond = 2;
  fParticleChange.SetNumberOfSecondaries(nSecond);
  fParticleChange.AddSecondary(gamma);
  if(nSecond == 2) {
    fParticleChange.ProposeTrackStatus(fStopAndKill);
    G4DynamicParticle* el = new G4DynamicParticle(dp->GetDefinition(),dir,kinEnergy);
    fParticleChange.AddSecondary(el);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4eBremsstrahlung::SetGammaThreshold(G4double val)
{
  gammaThreshold = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4eBremsstrahlung::GammaThreshold() const
{
  return gammaThreshold;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif


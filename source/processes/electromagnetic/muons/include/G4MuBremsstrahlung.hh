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
// $Id: G4MuBremsstrahlung.hh,v 1.26 2005-04-11 10:40:47 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4MuBremsstrahlung
//
// Author:        Laszlo Urban
//
// Creation date: 30.09.1997
//
// Modifications:
//
// 10/02/00 modifications , new e.m. structure, L.Urban
// 10-08-01 new methods Store/Retrieve PhysicsTable (mma)
// 29-10-01 all static functions no more inlined (mma)
// 10-05-02 V.Ivanchenko update to new design
// 26-12-02 secondary production moved to derived classes (VI)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 05-02-03 Fix compilation warnings (V.Ivanchenko)
// 08-08-03 STD substitute standard  (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 21-01-04 Migrade to G4ParticleChangeForLoss (V.Ivanchenko)
// 10-02-04 Add lowestKinEnergy (V.Ivanchenko)
// 17-08-04 Rename the process "Mu" -> "mu" (V.Ivanchenko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
//
// Class Description:
//
// This class manages the Bremsstrahlung process for muons.
// it inherites from G4VContinuousDiscreteProcess via G4VEnergyLossProcess.
//

// -------------------------------------------------------------------
//

#ifndef G4MuBremsstrahlung_h
#define G4MuBremsstrahlung_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VEmModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4ParticleDefinition;

class G4MuBremsstrahlung : public G4VEnergyLossProcess

{
public:

  G4MuBremsstrahlung(const G4String& processName = "muBrems");

  virtual ~G4MuBremsstrahlung();

  G4bool IsApplicable(const G4ParticleDefinition& p);

  G4double MinPrimaryEnergy(const G4ParticleDefinition* p,
			    const G4Material*, 
			    G4double cut);

  std::vector<G4DynamicParticle*>* SecondariesPostStep( 
                            G4VEmModel*,
			    const G4MaterialCutsCouple*,
			    const G4DynamicParticle*,
			    G4double& tcut);

  // Print out of the class parameters
  void PrintInfo();

protected:

  void InitialiseEnergyLossProcess(const G4ParticleDefinition*,
				   const G4ParticleDefinition*);

private:

  G4MuBremsstrahlung & operator=(const G4MuBremsstrahlung &right);
  G4MuBremsstrahlung(const G4MuBremsstrahlung&);

  const G4ParticleDefinition* theParticle;
  const G4ParticleDefinition* theBaseParticle;

  G4double  lowestKinEnergy;
  G4bool    isInitialised;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4bool G4MuBremsstrahlung::IsApplicable(const G4ParticleDefinition& p)
{
  return (p.GetPDGCharge() != 0.0 && p.GetPDGMass() > 10.0*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4MuBremsstrahlung::MinPrimaryEnergy(const G4ParticleDefinition*,
                                                     const G4Material*,
                                                           G4double)
{
  return lowestKinEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline std::vector<G4DynamicParticle*>* G4MuBremsstrahlung::SecondariesPostStep(
						    G4VEmModel* model,
						    const G4MaterialCutsCouple* couple,
						    const G4DynamicParticle* dp,
						    G4double& tcut)
{
  return model->SampleSecondaries(couple, dp, tcut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

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
// $Id: G4MuIonisation.hh,v 1.19 2004-02-10 18:07:23 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4MuIonisation
//
// Author:        Laszlo Urban
//
// Creation date: 30.05.1997
//
// Modifications:
//
// corrected by L.Urban on 24/09/97
// corrected by L.Urban on 13/01/98
// bugs fixed by L.Urban on 02/02/99
// 10/02/00 modifications , new e.m. structure, L.Urban
// 10-08-01 new methods Store/Retrieve PhysicsTable (mma)
// 14-08-01 new function ComputeRestrictedMeandEdx() + 'cleanup' (mma)
// 19-09-01 come back to previous process name "hIoni"
// 29-10-01 all static functions no more inlined
// 10-05-02 V.Ivanchenko update to new design
// 09-12-02 V.Ivanchenko remove warning
// 26-12-02 Secondary production moved to derived classes (VI)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 05-02-03 Fix compilation warnings (V.Ivanchenko)
// 13-02-03 SubCutoff regime is assigned to a region (V.Ivanchenko)
// 23-05-03 Add fluctuation model as a member function (V.Ivanchenko)
// 03-06-03 Add SetIntegral method to choose fluctuation model (V.Ivanchenko)
// 03-06-03 Fix initialisation problem for STD ionisation (V.Ivanchenko)
// 08-08-03 STD substitute standard  (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 21-01-04 Migrade to G4ParticleChangeForLoss (V.Ivanchenko)
//
// Class Description:
//
// This class manages the ionisation process for muons.
// it inherites from G4VContinuousDiscreteProcess via G4VEnergyLossProcess.
//

// -------------------------------------------------------------------
//

#ifndef G4MuIonisation_h
#define G4MuIonisation_h 1

#include "G4VEnergyLossProcess.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "globals.hh"

class G4Material;
class G4VEmFluctuationModel;

class G4MuIonisation : public G4VEnergyLossProcess
{

public:

  G4MuIonisation(const G4String& name = "MuIoni");

  ~G4MuIonisation();

  G4bool IsApplicable(const G4ParticleDefinition& p)
    {return (p.GetPDGCharge() != 0.0 && p.GetPDGMass() > 10.0*MeV);};

  virtual G4double MinPrimaryEnergy(const G4ParticleDefinition* p,
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

  void SetSubCutoff(G4bool val);

  void PrintInfoDefinition();
  // Print out of the class parameters

protected:

  const G4ParticleDefinition* DefineBaseParticle(const G4ParticleDefinition* p);

  virtual G4double MaxSecondaryEnergy(const G4DynamicParticle* dynParticle);

private:

  void InitialiseProcess();

  // hide assignment operator
  G4MuIonisation & operator=(const G4MuIonisation &right);
  G4MuIonisation(const G4MuIonisation&);

  G4double    mass;
  G4double    ratio;

  const G4ParticleDefinition* theParticle;
  const G4ParticleDefinition* theBaseParticle;
  G4VEmFluctuationModel*      flucModel;

  G4bool                 subCutoff;
  G4bool                 isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4MuIonisation::MinPrimaryEnergy(const G4ParticleDefinition*,
                                                    const G4Material*,
                                                          G4double cut)
{
  G4double x = 0.5*cut/electron_mass_c2;
  G4double g = x*ratio + sqrt((1. + x)*(1. + x*ratio*ratio));
  return mass*(g - 1.0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4MuIonisation::MaxSecondaryEnergy(const G4DynamicParticle* dynParticle)
{
  G4double tau  = dynParticle->GetKineticEnergy()/mass;
  G4double tmax = 2.0*electron_mass_c2*tau*(tau + 2.0)/
                  (1. + 2.0*(tau + 1.0)*ratio + ratio*ratio);

  return tmax;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4VSubCutoffProcessor.hh"

inline std::vector<G4Track*>*  G4MuIonisation::SecondariesAlongStep(
                           const G4Step&   step,
	             	         G4double& tmax,
			         G4double& eloss,
                                 G4double& kinEnergy)
{
  std::vector<G4Track*>* newp = 0;
  if(subCutoff) {
    G4VSubCutoffProcessor* sp = SubCutoffProcessor(CurrentMaterialCutsCoupleIndex());
    if (sp) {
      G4VEmModel* model = SelectModel(kinEnergy);
      newp = sp->SampleSecondaries(step,tmax,eloss,model);
    }
  }
  return newp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4VEmModel.hh"

inline void G4MuIonisation::SecondariesPostStep(
                                                   G4VEmModel* model,
                                             const G4MaterialCutsCouple* couple,
                                             const G4DynamicParticle* dp,
                                                   G4double& tcut,
                                                   G4double& kinEnergy)
{
  G4DynamicParticle* delta = model->SampleSecondary(couple, dp, tcut, kinEnergy);
  fParticleChange.SetNumberOfSecondaries(1);
  fParticleChange.AddSecondary(delta);
  G4ThreeVector finalP = dp->GetMomentum();
  kinEnergy -= delta->GetKineticEnergy();
  finalP -= delta->GetMomentum();
  finalP = finalP.unit();
  fParticleChange.SetProposedMomentumDirection(finalP);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

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
// $Id: G4ionIonisation.hh,v 1.41 2006-06-29 19:52:34 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4ionIonisation
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 07.05.2002
//
// Modifications:
//
// 26-12-02 Secondary production moved to derived classes (VI)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 05-02-03 Fix compilation warnings (V.Ivanchenko)
// 13-02-03 SubCutoff regime is assigned to a region (V.Ivanchenko)
// 15-02-03 Add control on delta pointer (V.Ivanchenko)
// 23-05-03 Add fluctuation model as a member function (V.Ivanchenko)
// 03-08-03 Add effective charge and saturation of tmax (V.Ivanchenko)
// 12-11-03 Fix problem of negative effective charge (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 21-01-04 Migrade to G4ParticleChangeForLoss (V.Ivanchenko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
// 11-04-05 Move MaxSecondary energy to model (V.Ivanchneko)
// 11-04-04 Move MaxSecondaryEnergy to models (V.Ivanchenko)
// 10-05-06 Add a possibility to download user data (V.Ivantchenko)
//
// Class Description:
//
// This class manages the ionisation process for ions.
// it inherites from G4VContinuousDiscreteProcess via G4VEnergyLoss.
//

// -------------------------------------------------------------------
//

#ifndef G4ionIonisation_h
#define G4ionIonisation_h 1

#include "G4VEnergyLossProcess.hh"
#include "G4ionEffectiveCharge.hh"
#include "G4VEmModel.hh"
#include "G4EmCorrections.hh"

class G4Material;
class G4VEmFluctuationModel;
class G4PhysicsVector;
class G4BraggIonModel;

class G4ionIonisation : public G4VEnergyLossProcess
{
public:

  G4ionIonisation(const G4String& name = "ionIoni");

  virtual ~G4ionIonisation();

  G4bool IsApplicable(const G4ParticleDefinition& p);

  // Print out of the class parameters
  virtual void PrintInfo();

  void AddStoppingData(G4int Z, G4int A, const G4String& materialName,
		       G4PhysicsVector& dVector);

  void ActivateStoppingData(G4bool);

protected:

  void CorrectionsAlongStep(
                           const G4MaterialCutsCouple*,
	             	   const G4DynamicParticle*,
			         G4double& eloss,
			         G4double& length);

  virtual std::vector<G4DynamicParticle*>* SecondariesPostStep(
			    G4VEmModel*,
			    const G4MaterialCutsCouple*,
			    const G4DynamicParticle*,
			    G4double&);

  void InitialiseEnergyLossProcess(const G4ParticleDefinition*,
				   const G4ParticleDefinition*);

  G4double GetMeanFreePath(const G4Track& track,
			   G4double previousStepSize,
			   G4ForceCondition* condition);

  virtual G4double MinPrimaryEnergy(const G4ParticleDefinition* p,
				    const G4Material*, G4double cut);

private:

  void DefineMassCharge(const G4ParticleDefinition* pd,
			const G4Material* mat,
			G4double mass,
			G4double kinEnergy);

  // hide assignment operator
  G4ionIonisation & operator=(const G4ionIonisation &right);
  G4ionIonisation(const G4ionIonisation&);

  G4ionEffectiveCharge*       effCharge;
  G4VEmFluctuationModel*      flucModel;
  G4EmCorrections*            corr;

  // cash
  const G4Material*           curMaterial;
  const G4ParticleDefinition* curParticle;
  const G4ParticleDefinition* theParticle;
  const G4ParticleDefinition* theBaseParticle;

  G4double                    preKinEnergy;

  G4double                    eth;
  G4double                    baseMass;
  G4double                    massRatio;
  G4double                    massFactor;
  G4double                    charge2;

  G4bool                      isInitialised;
  G4bool                      stopDataActive;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4bool G4ionIonisation::IsApplicable(const G4ParticleDefinition& p)
{
  return (p.GetPDGCharge() != 0.0 && !p.IsShortLived() &&
          p.GetParticleType() == "nucleus");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4ionIonisation::MinPrimaryEnergy(
          const G4ParticleDefinition*, const G4Material*, G4double cut)
{
  G4double x = 0.5*cut/electron_mass_c2;
  G4double g = std::sqrt(1. + x);
  return proton_mass_c2*(g - 1.0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4ionIonisation::DefineMassCharge(const G4ParticleDefinition* pd,
					      const G4Material* mat,
					      G4double mass,
					      G4double kinEnergy)
{
  preKinEnergy    = kinEnergy;
  massRatio       = baseMass/mass;
  charge2         = effCharge->EffectiveChargeSquareRatio(pd,mat,kinEnergy);
  SetDynamicMassCharge(massRatio, charge2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4ionIonisation::CorrectionsAlongStep(
                           const G4MaterialCutsCouple* couple,
	             	   const G4DynamicParticle* dp,
			         G4double& eloss,
                                 G4double& s)
{
  if(eloss < preKinEnergy) {
    const G4ParticleDefinition* part = dp->GetDefinition();
    const G4Material* mat = couple->GetMaterial();
    if(preKinEnergy*massRatio > eth)
      eloss += s*corr->HighOrderCorrections(part,mat,preKinEnergy);
    else {
      if(stopDataActive)
	eloss *= corr->EffectiveChargeCorrection(part,mat,preKinEnergy);
      eloss += s*corr->NuclearDEDX(part,mat,preKinEnergy - eloss*0.5);
    }
    fParticleChange.SetProposedCharge(effCharge->EffectiveCharge(part,
                                      mat,preKinEnergy-eloss));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline std::vector<G4DynamicParticle*>* G4ionIonisation::SecondariesPostStep(
                                                 G4VEmModel* model,
                                           const G4MaterialCutsCouple* couple,
                                           const G4DynamicParticle* dp,
                                                 G4double& tcut)
{
  return model->SampleSecondaries(couple, dp, tcut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4ionIonisation::ActivateStoppingData(G4bool val)
{
  stopDataActive = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

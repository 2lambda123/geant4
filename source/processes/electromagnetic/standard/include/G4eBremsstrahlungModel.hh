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
// $Id: G4eBremsstrahlungModel.hh,v 1.12 2005-03-18 12:48:25 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4eBremsstrahlungModel
//
// Author:        Vladimir Ivanchenko on base of Laszlo Urban code
//
// Creation date: 07.01.2002
//
// Modifications:
//
// 23-12-02 Change interface in order to move to cut per region (V.Ivanchenko)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 13-02-03 Add name (V.Ivanchenko)
//
//
// Class Description:
//
// Implementation of energy loss for gamma emission by electrons and
// positrons

// -------------------------------------------------------------------
//

#ifndef G4eBremsstrahlungModel_h
#define G4eBremsstrahlungModel_h 1

#include "G4VEmModel.hh"

class G4Element;

class G4eBremsstrahlungModel : public G4VEmModel
{

public:

  G4eBremsstrahlungModel(const G4ParticleDefinition* p = 0, const G4String& nam = "StanBrem");

  virtual ~G4eBremsstrahlungModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  G4bool IsInCharge(const G4ParticleDefinition*);

  G4double MinEnergyCut(const G4ParticleDefinition*, const G4MaterialCutsCouple*);

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

  void   SetLPMflag(G4bool val) {theLPMflag = val;};
  G4bool LPMflag() const {return theLPMflag;};

protected:

  virtual G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
    				            G4double kineticEnergy);

private:

  void SetParticle(const G4ParticleDefinition* p);

  G4double ComputeBremLoss(G4double Z, G4double tkin, G4double cut);

  G4double PositronCorrFactorLoss(G4double Z, G4double tkin, G4double cut);

  G4double PositronCorrFactorSigma(G4double Z, G4double tkin, G4double cut);

  G4double CrossSectionPerAtom(G4double tkin, G4double Z, G4double cut);

  G4DataVector* ComputePartialSumSigma(const G4Material* material,
                                             G4double tkin, G4double cut);

  const G4Element* SelectRandomAtom(const G4MaterialCutsCouple* couple) const;

  G4double SupressionFunction(const G4Material* material, G4double tkin,
                                    G4double gammaEnergy);

  G4double ScreenFunction1(G4double ScreenVariable);

  G4double ScreenFunction2(G4double ScreenVariable);

  // hide assignment operator
  G4eBremsstrahlungModel & operator=(const  G4eBremsstrahlungModel &right);
  G4eBremsstrahlungModel(const  G4eBremsstrahlungModel&);

  const G4ParticleDefinition* particle;
  G4ParticleDefinition* theGamma;
  G4double highKinEnergy;
  G4double lowKinEnergy;
  G4double minThreshold;
  G4double probsup;
  G4double MigdalConstant;
  G4double LPMconstant;
  G4bool   isElectron;
  G4bool   theLPMflag;
  std::vector<G4DataVector*> partialSumSigma;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4eBremsstrahlungModel::ScreenFunction1(G4double ScreenVariable)

// compute the value of the screening function 3*PHI1 - PHI2

{
   G4double screenVal;

   if (ScreenVariable > 1.)
     screenVal = 42.24 - 8.368*std::log(ScreenVariable+0.952);
   else
     screenVal = 42.392 - ScreenVariable* (7.796 - 1.961*ScreenVariable);

   return screenVal;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline
G4double G4eBremsstrahlungModel::ScreenFunction2(G4double ScreenVariable)

// compute the value of the screening function 1.5*PHI1 - 0.5*PHI2

{
   G4double screenVal;

   if (ScreenVariable > 1.)
     screenVal = 42.24 - 8.368*std::log(ScreenVariable+0.952);
   else
     screenVal = 41.734 - ScreenVariable* (6.484 - 1.250*ScreenVariable);

   return screenVal;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline 
G4double G4eBremsstrahlungModel::MaxSecondaryEnergy(
                                 const G4ParticleDefinition*,
    				       G4double kineticEnergy)
{
  return kineticEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

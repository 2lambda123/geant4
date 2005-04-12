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
// $Id: G4MollerBhabhaModel.hh,v 1.12 2005-04-12 18:12:41 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4MollerBhabhaModel
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
// Class Description:
//
// Implementation of energy loss process and delta-electrons production
// of electrons and positrons

// -------------------------------------------------------------------
//

#ifndef G4MollerBhabhaModel_h
#define G4MollerBhabhaModel_h 1

#include "G4VEmModel.hh"

class G4ParticleChangeForLoss;

class G4MollerBhabhaModel : public G4VEmModel
{

public:

  G4MollerBhabhaModel(const G4ParticleDefinition* p = 0, 
		      const G4String& nam = "MollerBhabha");

  virtual ~G4MollerBhabhaModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  G4double MinEnergyCut(const G4ParticleDefinition*,
                        const G4MaterialCutsCouple*);

  G4double ComputeDEDXPerVolume(const G4Material*,
				const G4ParticleDefinition*,
				G4double kineticEnergy,
				G4double cutEnergy);

  G4double CrossSectionPerVolume(const G4Material*,
				 const G4ParticleDefinition*,
				 G4double kineticEnergy,
				 G4double cutEnergy,
				 G4double maxEnergy);

  std::vector<G4DynamicParticle*>* SampleSecondaries(
                                const G4MaterialCutsCouple*,
                                const G4DynamicParticle*,
                                      G4double tmin,
                                      G4double maxEnergy);

protected:

  G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
			      G4double kinEnergy);

private:

  void SetParticle(const G4ParticleDefinition* p);

  // hide assignment operator 
  G4MollerBhabhaModel & operator=(const  G4MollerBhabhaModel &right);
  G4MollerBhabhaModel(const  G4MollerBhabhaModel&);

  const G4ParticleDefinition* particle;
  G4ParticleDefinition*       theElectron;
  G4ParticleChangeForLoss*    fParticleChange;
  G4double twoln10;
  G4double lowLimit;
  G4bool   isElectron;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4double G4MollerBhabhaModel::MaxSecondaryEnergy(
          const G4ParticleDefinition*,
                G4double kinEnergy) 
{
  G4double tmax = kinEnergy;
  if(isElectron) tmax *= 0.5;
  return tmax;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

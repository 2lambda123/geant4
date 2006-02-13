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
// $Id: G4MuBetheBlochModel.hh,v 1.15 2006-02-13 16:46:15 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4MuBetheBlochModel
//
// Author:        Vladimir Ivanchenko on base of Laszlo Urban code
//
// Creation date: 09.08.2002
//
// Modifications:
//
// 23-12-02 Change interface in order to move to cut per region (V.Ivanchenko)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 13-02-03 Add Nama (V.Ivanchenko)
// 10-02-04 Calculation of radiative corrections using R.Kokoulin model (V.I)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
// 12-04-05 Add usage of G4EmCorrections (V.Ivanchenko)
// 13-02-06 ComputeCrossSectionPerElectron, ComputeCrossSectionPerAtom (mma)
//

//
// Class Description:
//
// Implementation of Bethe-Bloch model of energy loss and
// delta-electron production by heavy charged particles

// -------------------------------------------------------------------
//

#ifndef G4MuBetheBlochModel_h
#define G4MuBetheBlochModel_h 1

#include "G4VEmModel.hh"

class G4ParticleChangeForLoss;
class G4EmCorrections;

class G4MuBetheBlochModel : public G4VEmModel
{

public:

  G4MuBetheBlochModel(const G4ParticleDefinition* p = 0,
                      const G4String& nam = "MuBetheBloch");

  virtual ~G4MuBetheBlochModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  G4double MinEnergyCut(const G4ParticleDefinition*,
                        const G4MaterialCutsCouple*);
			
  virtual G4double ComputeCrossSectionPerElectron(
				 const G4ParticleDefinition*,
				 G4double kineticEnergy,
				 G4double cutEnergy,
				 G4double maxEnergy);
				 
  virtual G4double ComputeCrossSectionPerAtom(
				 const G4ParticleDefinition*,
				 G4double kineticEnergy,
				 G4double Z, G4double A,
				 G4double cutEnergy,
				 G4double maxEnergy);
				 				 
  virtual G4double CrossSectionPerVolume(const G4Material*,
				 const G4ParticleDefinition*,
				 G4double kineticEnergy,
				 G4double cutEnergy,
				 G4double maxEnergy);

  virtual G4double ComputeDEDXPerVolume(const G4Material*,
                                        const G4ParticleDefinition*,
                                        G4double kineticEnergy,
                                        G4double cutEnergy);

  virtual std::vector<G4DynamicParticle*>* SampleSecondaries(
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
  G4MuBetheBlochModel & operator=(const  G4MuBetheBlochModel &right);
  G4MuBetheBlochModel(const  G4MuBetheBlochModel&);

  const G4ParticleDefinition* particle;
  G4ParticleDefinition*       theElectron;
  G4ParticleChangeForLoss*    fParticleChange;
  G4EmCorrections*            corr;

  G4double limitKinEnergy;
  G4double logLimitKinEnergy;
  G4double mass;
  G4double massSquare;
  G4double ratio;
  G4double twoln10;
  G4double bg2lim;
  G4double taulim;
  G4double alphaprime;
  static G4double xgi[8],wgi[8];
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

inline G4double G4MuBetheBlochModel::MaxSecondaryEnergy(
          const G4ParticleDefinition*,
                G4double kinEnergy) 
{
  G4double tau  = kinEnergy/mass;
  G4double tmax = 2.0*electron_mass_c2*tau*(tau + 2.) /
                  (1. + 2.0*(tau + 1.)*ratio + ratio*ratio);
  return tmax;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#endif

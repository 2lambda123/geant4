// ********************************************************************
// *                                                                  *
// * cosmicray_charging advanced example for Geant4                   *
// * (adapted simulation of test-mass charging in the LISA mission)   *
// *                                                                  *
// * Henrique Araujo (h.araujo@imperial.ac.uk) & Peter Wass           *
// * Imperial College London                                          *
// *                                                                  *
// * LISAPrimaryGeneratorAction class                                 *
// *                                                                  *
// ********************************************************************
//
// HISTORY
// 22/02/2004: migrated from LISA-V04
//
// ********************************************************************


#include "LISAPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

#include "G4Event.hh"
#include "Randomize.hh"


LISAPrimaryGeneratorAction::LISAPrimaryGeneratorAction() {
  
  particleGun = new G4GeneralParticleSource();

  energy_pri=0;
  seeds[0]=-1;
  seeds[1]=-1;

}


LISAPrimaryGeneratorAction::~LISAPrimaryGeneratorAction() {

  delete particleGun;
}


void LISAPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  energy_pri = 0.;

  // seeds
  seeds[0] = *HepRandom::getTheSeeds();
  seeds[1] = *(HepRandom::getTheSeeds()+1);
  //  G4cout << " 1st seed: " << *seeds << G4endl;;
  //  G4cout << " 2nd seed: " << *(seeds+1) << G4endl;
  // HepRandom::showEngineStatus();
      
  particleGun->GeneratePrimaryVertex(anEvent);

  energy_pri = particleGun->GetParticleEnergy();

}


// ********************************************************************
// *                                                                  *
// * cosmicray_charging advanced example for Geant4                   *
// * (adapted simulation of test-mass charging in the LISA mission)   *
// *                                                                  *
// * Henrique Araujo (h.araujo@imperial.ac.uk) & Peter Wass           *
// * Imperial College London                                          *
// *                                                                  *
// * LISAStackingAction class                                        *
// *                                                                  *
// ********************************************************************
//
// HISTORY
// 22/02/2004: migrated from LISA-V04
//
// ********************************************************************


#include "LISAStackingAction.hh"

#include "LISAStackingActionMessenger.hh"

#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include <fstream>

LISAStackingAction::LISAStackingAction() {

  // messenger defaults  
  PrimarySurveyFlag  = false;
  ParticleSurveyFlag = false;

  // create messenger
  stackingMessenger = new LISAStackingActionMessenger(this);

}


LISAStackingAction::~LISAStackingAction() {

  delete stackingMessenger;

}


G4ClassificationOfNewTrack LISAStackingAction::ClassifyNewTrack 
(const G4Track* aTrack) {


  // write primary spectrum to file
  if(PrimarySurveyFlag) {
    if(!aTrack->GetParentID()) {
      G4double energy = aTrack->GetKineticEnergy();
      std::ofstream primaries("primaries.out",std::ios::app);
      primaries << energy/MeV << G4endl;
      return (G4ClassificationOfNewTrack) fKill;
    }
  }

  // Particle Survey
  if(ParticleSurveyFlag) {
    G4String particle = aTrack->GetDefinition()->GetParticleName();
    G4double energy = aTrack->GetKineticEnergy();
    //
    // gammas above 1 MeV
    if(particle=="gamma" && energy>1*MeV) {
      std::ofstream gammas("gammas.out",std::ios::app);
      gammas << energy/MeV << G4endl;
    }
    // electrons and positrons above 1 MeV
    else if((particle=="e-" || particle=="e+") && energy>1.*MeV) {
      std::ofstream electrons("electrons.out",std::ios::app);
      electrons << energy/MeV << G4endl;
    }
    // delta electrons
    //  else if(particle=="e-" && aTrack->GetParentID()>0) {
    //    std::ofstream deltas("deltas.out",std::ios::app);
    //    deltas << energy/eV << G4endl;
    //    return (G4ClassificationOfNewTrack) fKill;
    //  }
    //
    // neutrons
    else if(particle=="neutron") {
      std::ofstream neutrons("neutrons.out",std::ios::app);
      neutrons << energy/MeV << G4endl;
    }
    // pions
    else if(particle=="pi+" || particle=="pi-" || particle=="pi0") {
      std::ofstream pions("pions.out",std::ios::app);
      pions << energy/MeV << G4endl;
    }
    // fragments (energy per nucleon)
    else if(particle=="deuteron" || particle=="triton" || 
	    particle=="He3" || particle=="alpha") {
      G4int A = aTrack->GetDefinition()->GetBaryonNumber();
      std::ofstream fragments("fragments.out",std::ios::app);
      fragments << energy/A/MeV << G4endl;
    }
    // muons
    else if(particle=="mu+" || particle=="mu-") {
      std::ofstream muons("muons.out",std::ios::app);
      muons << energy/MeV << G4endl;
    }
    // kaons
    else if(particle=="kaon+"  || particle=="kaon-" ||
	    particle=="kaon0S" || particle=="kaon0L" ) {
      std::ofstream kaons("kaons.out",std::ios::app);
      kaons << energy/MeV << G4endl;
    }
    // nuclei (energy per nucleon)
    else if(aTrack->GetDefinition()->GetParticleType()=="nucleus") {
      G4int A = aTrack->GetDefinition()->GetBaryonNumber();
      std::ofstream nuclei("nuclei.out",std::ios::app);
      nuclei << energy/A/MeV << "\t" << particle << G4endl;
    }
    // others
    else if(particle!="proton" && energy>1.*MeV) {
      std::ofstream others("others.out",std::ios::app);
      others << energy/MeV << "\t" << particle << G4endl;
    }

  }


  return (G4ClassificationOfNewTrack) fWaiting;
  
}


void LISAStackingAction::NewStage() {;}

    
void LISAStackingAction::PrepareNewEvent() {;}








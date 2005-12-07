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
//
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//      For information related to this code contact: Alex Howard
//      e-mail: a.s.howard@ic.ac.uk
// --------------------------------------------------------------
// Comments
//
//                  Underground Advanced
//               by A. Howard and H. Araujo 
//                    (27th November 2001)
//
//
// PrimaryGeneratorAction program
// --------------------------------------------------------------

#include "DMXPrimaryGeneratorAction.hh"

#ifdef DMXENV_GPS_USE
#include "G4GeneralParticleSource.hh"
#else
#include "DMXParticleSource.hh"
#endif

#ifdef G4ANALYSIS_USE
#include "DMXAnalysisManager.hh"
#endif

#include "G4Event.hh"

#include "Randomize.hh"

#include "globals.hh"


DMXPrimaryGeneratorAction::DMXPrimaryGeneratorAction() {
  
#ifdef DMXENV_GPS_USE
  particleGun = new G4GeneralParticleSource();
#else
  particleGun = new DMXParticleSource();
#endif

  energy_pri=0;
  //  seeds=NULL;
  seeds[0] =-1;
  seeds[1] =-1;

}


DMXPrimaryGeneratorAction::~DMXPrimaryGeneratorAction() {

  delete particleGun;
}


void DMXPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  energy_pri = 0.;

  // seeds
  //  seeds = HepRandom::getTheSeeds();
  seeds[0] = *CLHEP::HepRandom::getTheSeeds();
  seeds[1] = *(CLHEP::HepRandom::getTheSeeds()+1);

  particleGun->GeneratePrimaryVertex(anEvent);

  energy_pri = particleGun->GetParticleEnergy();
#ifdef G4ANALYSIS_USE 
  DMXAnalysisManager* analysis =  DMXAnalysisManager::getInstance();
  analysis->analysePrimaryGenerator(energy_pri);
#endif

}



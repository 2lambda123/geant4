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
// $Id: TargetPrimaryGeneratorAction.cc,v 1.1 2003-10-08 12:32:11 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
#include "TargetPrimaryGeneratorAction.hh"

#include "TargetConstruction.hh"
#include "TargetPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"


TargetPrimaryGeneratorAction::TargetPrimaryGeneratorAction(
                                                    TargetConstruction* TC)
 :Target(TC),rndmFlag("off")
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  gunMessenger = new TargetPrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="proton");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(50.*MeV);
  G4double position = -0.5*(Target->GetWorldLength());
  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,position));

}


TargetPrimaryGeneratorAction::~TargetPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}


void TargetPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the beginning of the event
  // 
  G4double x0 = 0.;
  G4double y0 = 0.;
  G4double z0 = -0.5*(Target->GetWorldLength());
  //  if (rndmFlag == "on") {
  //    y0 = (G4UniformRand()-0.5)*mm;
  //    z0 = (G4UniformRand()-0.5)*mm;
  //  } 
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  particleGun->GeneratePrimaryVertex(anEvent);
}


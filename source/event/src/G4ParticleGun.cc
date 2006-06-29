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
//
// $Id: G4ParticleGun.cc,v 1.12 2006-06-29 18:09:53 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

// G4ParticleGun
#include "G4ParticleGun.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleGunMessenger.hh"
#include "G4Event.hh"
#include "G4ios.hh"

G4ParticleGun::G4ParticleGun()
{
  SetInitialValues();
}

G4ParticleGun::G4ParticleGun(G4int numberofparticles)
{
  SetInitialValues();
  NumberOfParticlesToBeGenerated = numberofparticles;
}

G4ParticleGun::G4ParticleGun
    (G4ParticleDefinition * particleDef, G4int numberofparticles)
{
  SetInitialValues();
  NumberOfParticlesToBeGenerated = numberofparticles;
  SetParticleDefinition( particleDef );
}

void G4ParticleGun::SetInitialValues()
{
  NumberOfParticlesToBeGenerated = 1;
  particle_definition = 0;
  G4ThreeVector zero;
  particle_momentum_direction = (G4ParticleMomentum)zero;
  particle_energy = 0.0;
  particle_position = zero;
  particle_time = 0.0;
  particle_polarization = zero;
  particle_charge = 0.0;
  theMessenger = new G4ParticleGunMessenger(this);
}

G4ParticleGun::~G4ParticleGun()
{
  delete theMessenger;
}

void G4ParticleGun::SetParticleDefinition
                 (G4ParticleDefinition * aParticleDefinition)
{ 
  if(!aParticleDefinition)
  {
    G4Exception("G4ParticleGun::SetParticleDefinition()","Event00003",FatalException,
     "Null pointer is given.");
  }
  if(aParticleDefinition->IsShortLived())
  {
    if(!(aParticleDefinition->GetDecayTable()))
    {
      G4cerr << "G4ParticleGun does not support shooting a short-lived particle without a valid decay table." << G4endl;
      G4cerr << "G4ParticleGun::SetParticleDefinition for "
             << aParticleDefinition->GetParticleName() << " is ignored." << G4endl;
      return;
    }
  }
  particle_definition = aParticleDefinition; 
  particle_charge = particle_definition->GetPDGCharge();
}
 
void G4ParticleGun::SetParticleMomentum(G4ParticleMomentum aMomentum)
{
  if(particle_definition==0)
  {
    G4cout <<"Particle Definition not defined yet for G4ParticleGun"<< G4endl;
    G4cout <<"Zero Mass is assumed"<<G4endl;
    particle_momentum_direction =  aMomentum.unit();
    particle_energy = aMomentum.mag();
  } 
  else 
  {
    G4double mass =  particle_definition->GetPDGMass();
    G4double p = aMomentum.mag();
    particle_momentum_direction =  aMomentum.unit();
    if ((particle_energy>0.0)&&(std::abs(particle_energy+mass-std::sqrt(p*p+mass*mass))>keV))
    {
      G4cout << "G4ParticleGun::" << particle_definition->GetParticleName() << G4endl;
      G4cout << "  KineticEnergy and Momentum could be inconsistent" << G4endl;
      G4cout << " (Momentum:" << p/GeV << " GeV/c";
      G4cout << "  Mass:" << mass/GeV << " GeV/c/c)" << G4endl;
      G4cout << "  KineticEnergy is overwritten!! ";
      G4cout << particle_energy/GeV << "->";
      G4cout << (std::sqrt(p*p+mass*mass)-mass)/GeV << "GeV" << G4endl;
    }
    particle_energy = std::sqrt(p*p+mass*mass)-mass;
  }
}

void G4ParticleGun::GeneratePrimaryVertex(G4Event* evt)
{
  if(particle_definition==0) return;

  // create a new vertex
  G4PrimaryVertex* vertex = 
    new G4PrimaryVertex(particle_position,particle_time);

  // create new primaries and set them to the vertex
  G4double mass =  particle_definition->GetPDGMass();
  G4double energy = particle_energy + mass;
  G4double pmom = std::sqrt(energy*energy-mass*mass);
  G4double px = pmom*particle_momentum_direction.x();
  G4double py = pmom*particle_momentum_direction.y();
  G4double pz = pmom*particle_momentum_direction.z();
  for( G4int i=0; i<NumberOfParticlesToBeGenerated; i++ )
  {
    G4PrimaryParticle* particle =
      new G4PrimaryParticle(particle_definition,px,py,pz);
    particle->SetMass( mass );
    particle->SetCharge( particle_charge );
    particle->SetPolarization(particle_polarization.x(),
                               particle_polarization.y(),
                               particle_polarization.z());
    vertex->SetPrimary( particle );
  }

  evt->AddPrimaryVertex( vertex );
}



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
// $Id: RE02PrimaryGeneratorAction.cc,v 1.1 2005-11-24 01:44:18 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "RE02PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "globals.hh"

//
RE02PrimaryGeneratorAction::RE02PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

// default particle
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("proton");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.));
  particleGun->SetParticleEnergy(150.0*MeV);
//
// default beam position
  G4double position = -200./2.*cm;
//
// Initial beam spot size in sigma.; This is not a part of ParticleGun.
  fsigmaPosition = 10.* mm;

//
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm, 0.*cm, position));
}

//
RE02PrimaryGeneratorAction::~RE02PrimaryGeneratorAction()
{
  delete particleGun;
}

//
void RE02PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 

  G4ThreeVector position = particleGun->GetParticlePosition();
  G4double dx = (G4UniformRand()-0.5)*fsigmaPosition;
  G4double dy = (G4UniformRand()-0.5)*fsigmaPosition;
  position.setX(dx);
  position.setY(dy);
  particleGun->SetParticlePosition(position);
  particleGun->GeneratePrimaryVertex(anEvent);
}

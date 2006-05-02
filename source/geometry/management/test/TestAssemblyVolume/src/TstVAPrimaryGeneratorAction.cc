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
// $Id: TstVAPrimaryGeneratorAction.cc,v 1.6 2006-05-02 11:47:55 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// --------------------------------------------------------------

#include "globals.hh"
#include "Randomize.hh"

#include "TstVAPrimaryGeneratorAction.hh"

#include "TstVAPrimaryGeneratorMessenger.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4TransportationManager.hh"

TstVAPrimaryGeneratorAction::TstVAPrimaryGeneratorAction():
  generatorAction (standardGun),
  particleGun (0),
  messenger (0),
  worldVolume (0)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  // default particle

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle 
    = particleTable->FindParticle(particleName="e-");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  particleGun->SetParticleEnergy(1.*GeV);
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,0.*cm));

  // messenger

  messenger = new TstVAPrimaryGeneratorMessenger (this);

  // world extent

  worldVolume = G4TransportationManager::GetTransportationManager ()
    -> GetNavigatorForTracking () -> GetWorldVolume ();
  if (worldVolume) worldExtent = worldVolume -> GetLogicalVolume ()
		     -> GetSolid () -> GetExtent ();

}

TstVAPrimaryGeneratorAction::~TstVAPrimaryGeneratorAction()
{
  delete particleGun;
  delete messenger;
}

void TstVAPrimaryGeneratorAction::SelectPrimaryGeneratorAction
(Action action)
{
  generatorAction = action;
}

void TstVAPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4VPhysicalVolume* currentWorldVolume;

  double costheta;
  double sintheta;
  double phi;
  double cosphi;
  double sinphi;

  switch (generatorAction) {

  case standardGun:

    particleGun->GeneratePrimaryVertex(anEvent);
    break;

  case randomDirectionGun:

    costheta = CLHEP::RandFlat::shoot (-1., 1.);
    sintheta = std::sqrt (1. - costheta * costheta);
    phi      = CLHEP::RandFlat::shoot (twopi);
    cosphi   = std::cos (phi);
    sinphi   = std::sin (phi);
    particleGun->SetParticleMomentumDirection
      (G4ThreeVector (sintheta * cosphi, sintheta * sinphi, costheta));

    particleGun->GeneratePrimaryVertex(anEvent);
    break;

  case randomPositionGun:

    // Check if world is in place or has changed.
    currentWorldVolume = 
      G4TransportationManager::GetTransportationManager ()
      -> GetNavigatorForTracking () -> GetWorldVolume ();
    if (!worldVolume ||	worldVolume != currentWorldVolume) {
      worldVolume = currentWorldVolume;
      if (worldVolume) worldExtent = worldVolume -> GetLogicalVolume ()
			 -> GetSolid () -> GetExtent ();
    }

    particleGun->SetParticlePosition
      (G4ThreeVector
       (CLHEP::RandFlat::shoot (worldExtent.GetXmin (), worldExtent.GetXmax ()),
	CLHEP::RandFlat::shoot (worldExtent.GetYmin (), worldExtent.GetYmax ()),
	CLHEP::RandFlat::shoot (worldExtent.GetZmin (), worldExtent.GetZmax ())));

    particleGun->GeneratePrimaryVertex(anEvent);
    break;

  case randomPositionAndDirectionGun:

    // Check if world is in place or has changed.
    currentWorldVolume =
      G4TransportationManager::GetTransportationManager ()
      -> GetNavigatorForTracking () -> GetWorldVolume ();
    if (!worldVolume ||	worldVolume != currentWorldVolume) {
      worldVolume = currentWorldVolume;
      if (worldVolume) worldExtent = worldVolume -> GetLogicalVolume ()
			 -> GetSolid () -> GetExtent ();
    }

    particleGun->SetParticlePosition
      (G4ThreeVector
       (CLHEP::RandFlat::shoot (worldExtent.GetXmin (), worldExtent.GetXmax ()),
	CLHEP::RandFlat::shoot (worldExtent.GetYmin (), worldExtent.GetYmax ()),
	CLHEP::RandFlat::shoot (worldExtent.GetZmin (), worldExtent.GetZmax ())));

    costheta = CLHEP::RandFlat::shoot (-1., 1.);
    sintheta = std::sqrt (1. - costheta * costheta);
    phi      = CLHEP::RandFlat::shoot (twopi);
    cosphi   = std::cos (phi);
    sinphi   = std::sin (phi);
    particleGun->SetParticleMomentumDirection
      (G4ThreeVector (sintheta * cosphi, sintheta * sinphi, costheta));

    particleGun->GeneratePrimaryVertex(anEvent);
    break;

  }
}

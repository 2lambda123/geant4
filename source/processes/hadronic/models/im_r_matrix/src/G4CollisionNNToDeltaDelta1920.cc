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
// $Id: G4CollisionNNToDeltaDelta1920.cc,v 1.1 2003-10-07 12:37:34 hpw Exp $ //

#include "globals.hh"
#include "G4CollisionNNToDeltaDelta1920.hh"
#include "G4ConcreteNNToDeltaDeltastar.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4ParticleTable.hh"
#include "G4ShortLivedConstructor.hh"

// complete hpw

G4CollisionNNToDeltaDelta1920::G4CollisionNNToDeltaDelta1920()
{ 
  // Subtype of interacting particles
  G4ShortLivedConstructor ShortLived;
  ShortLived.ConstructParticle();

  G4ParticleDefinition * aNeutron = G4Neutron::NeutronDefinition();
  G4ParticleDefinition * aProton = G4Proton::ProtonDefinition();
  G4ParticleDefinition * aDeltapp = G4ParticleTable::GetParticleTable()->FindParticle(2224); // D++
  G4ParticleDefinition * aDeltap = G4ParticleTable::GetParticleTable()->FindParticle(2214); // D+
  G4ParticleDefinition * aDelta0 = G4ParticleTable::GetParticleTable()->FindParticle(2114); // D0
  G4ParticleDefinition * aDeltam = G4ParticleTable::GetParticleTable()->FindParticle(1114); // D-
  
  G4ParticleDefinition * aDm_1920 = G4ParticleTable::GetParticleTable()->FindParticle(21114); 
  G4ParticleDefinition * aD0_1920 = G4ParticleTable::GetParticleTable()->FindParticle(22114); 
  G4ParticleDefinition * aDp_1920 = G4ParticleTable::GetParticleTable()->FindParticle(22214); 
  G4ParticleDefinition * aDpp_1920 = G4ParticleTable::GetParticleTable()->FindParticle(22224); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aNeutron, aNeutron, aDeltam, aDp_1920));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aNeutron, aNeutron, aDelta0, aD0_1920));  
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aNeutron, aNeutron, aDeltap, aDm_1920));
  
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aNeutron, aProton, aDeltap, aD0_1920));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aNeutron, aProton, aDelta0, aDp_1920));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aNeutron, aProton, aDeltam, aDpp_1920));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aNeutron, aProton, aDeltapp, aD0_1920));
  
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aProton, aProton, aDelta0, aDpp_1920));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aProton, aProton, aDeltap, aDp_1920));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaDeltastar(aProton, aProton, aDeltapp, aD0_1920));
}




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
// $Id: G4CollisionNNToDeltaNstar.cc,v 1.1 2003-10-07 12:37:35 hpw Exp $ //

#include "globals.hh"
#include "G4CollisionNNToDeltaNstar.hh"
#include "G4KineticTrack.hh"
#include "G4VCrossSectionSource.hh"
#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"
#include "G4KineticTrackVector.hh"
#include "G4ParticleTable.hh"
#include "G4CollisionVector.hh"
#include "G4CollisionNNToDeltaNstar.hh"
#include "G4ConcreteNNToDeltaNstar.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"

G4CollisionNNToDeltaNstar::G4CollisionNNToDeltaNstar()
{ 
  G4ParticleDefinition * aNeutron = G4Neutron::NeutronDefinition();
  G4ParticleDefinition * aProton = G4Proton::ProtonDefinition();
  
  G4ParticleDefinition * aDeltapp = G4ParticleTable::GetParticleTable()->FindParticle(2224); // D++
  G4ParticleDefinition * aDeltap = G4ParticleTable::GetParticleTable()->FindParticle(2214); // D+
  G4ParticleDefinition * aDelta0 = G4ParticleTable::GetParticleTable()->FindParticle(2114); // D0
  G4ParticleDefinition * aDeltam = G4ParticleTable::GetParticleTable()->FindParticle(1114); // D-
  
  G4ParticleDefinition * aN_1400p = G4ParticleTable::GetParticleTable()->FindParticle(12212); 
  G4ParticleDefinition * aN_1400n = G4ParticleTable::GetParticleTable()->FindParticle(12112); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1400n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1400p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1400p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1400n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1400p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1400n));

  G4ParticleDefinition * aN_1520p = G4ParticleTable::GetParticleTable()->FindParticle(2124); 
  G4ParticleDefinition * aN_1520n = G4ParticleTable::GetParticleTable()->FindParticle(1214); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1520n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1520p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1520p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1520n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1520p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1520n));

  G4ParticleDefinition * aN_1535p = G4ParticleTable::GetParticleTable()->FindParticle(22212); 
  G4ParticleDefinition * aN_1535n = G4ParticleTable::GetParticleTable()->FindParticle(22112); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1535n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1535p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1535p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1535n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1535p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1535n));

  G4ParticleDefinition * aN_1650p = G4ParticleTable::GetParticleTable()->FindParticle(32212); 
  G4ParticleDefinition * aN_1650n = G4ParticleTable::GetParticleTable()->FindParticle(32112); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1650n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1650p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1650p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1650n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1650p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1650n));

  G4ParticleDefinition * aN_1675p = G4ParticleTable::GetParticleTable()->FindParticle(2216); 
  G4ParticleDefinition * aN_1675n = G4ParticleTable::GetParticleTable()->FindParticle(2116); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1675n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1675p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1675p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1675n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1675p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1675n));

  G4ParticleDefinition * aN_1680p = G4ParticleTable::GetParticleTable()->FindParticle(12216); 
  G4ParticleDefinition * aN_1680n = G4ParticleTable::GetParticleTable()->FindParticle(12116); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1680n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1680p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1680p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1680n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1680p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1680n));

  G4ParticleDefinition * aN_1700p = G4ParticleTable::GetParticleTable()->FindParticle(22124); 
  G4ParticleDefinition * aN_1700n = G4ParticleTable::GetParticleTable()->FindParticle(21214); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1700n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1700p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1700p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1700n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1700p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1700n));

  G4ParticleDefinition * aN_1710p = G4ParticleTable::GetParticleTable()->FindParticle(42212); 
  G4ParticleDefinition * aN_1710n = G4ParticleTable::GetParticleTable()->FindParticle(42112); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1710n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1710p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1710p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1710n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1710p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1710n));

  G4ParticleDefinition * aN_1720p = G4ParticleTable::GetParticleTable()->FindParticle(32124); 
  G4ParticleDefinition * aN_1720n = G4ParticleTable::GetParticleTable()->FindParticle(31214); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1720n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1720p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1720p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1720n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1720p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1720n));

  G4ParticleDefinition * aN_1900p = G4ParticleTable::GetParticleTable()->FindParticle(42124); 
  G4ParticleDefinition * aN_1900n = G4ParticleTable::GetParticleTable()->FindParticle(41214); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1900n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1900p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1900p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1900n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1900p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1900n));

  G4ParticleDefinition * aN_1990p = G4ParticleTable::GetParticleTable()->FindParticle(12218); 
  G4ParticleDefinition * aN_1990n = G4ParticleTable::GetParticleTable()->FindParticle(12118); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_1990n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_1990p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_1990p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_1990n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_1990p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_1990n));

  G4ParticleDefinition * aN_2090p = G4ParticleTable::GetParticleTable()->FindParticle(52214); 
  G4ParticleDefinition * aN_2090n = G4ParticleTable::GetParticleTable()->FindParticle(52114); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_2090n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_2090p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_2090p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_2090n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_2090p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_2090n));

  G4ParticleDefinition * aN_2190p = G4ParticleTable::GetParticleTable()->FindParticle(2128); 
  G4ParticleDefinition * aN_2190n = G4ParticleTable::GetParticleTable()->FindParticle(1218); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_2190n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_2190p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_2190p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_2190n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_2190p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_2190n));

  G4ParticleDefinition * aN_2220p = G4ParticleTable::GetParticleTable()->FindParticle(100002210); 
  G4ParticleDefinition * aN_2220n = G4ParticleTable::GetParticleTable()->FindParticle(100002110); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_2220n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_2220p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_2220p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_2220n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_2220p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_2220n));

  G4ParticleDefinition * aN_2250p = G4ParticleTable::GetParticleTable()->FindParticle(100012210); 
  G4ParticleDefinition * aN_2250n = G4ParticleTable::GetParticleTable()->FindParticle(100012110); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDelta0,  aN_2250n)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aNeutron, aDeltam,  aN_2250p)); 
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltap,  aN_2250p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aProton,  aProton,  aDeltapp, aN_2250n));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDelta0,  aN_2250p));
  G4CollisionComposite::AddComponent(new G4ConcreteNNToDeltaNstar(aNeutron, aProton,  aDeltap,  aN_2250n));
}


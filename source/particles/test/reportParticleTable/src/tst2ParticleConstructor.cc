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
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id$
//
// 
// ---------------------------------------------------------------
#include "tst2ParticleConstructor.hh"


#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

void tst2ParticleConstructor::ConstructAllBosons()
{
  // Construct all bosons
  G4cout << "Bosons" << G4endl;
  G4BosonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void tst2ParticleConstructor::ConstructAllLeptons()
{
  // Construct all leptons
  G4cout << "Leptons" << G4endl;
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void tst2ParticleConstructor::ConstructAllMesons()
{
  //  Construct all mesons
  G4cout << "Leptons" << G4endl;
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void tst2ParticleConstructor::ConstructAllBaryons()
{
  //  Construct all barions
  G4cout << "Baryons" << G4endl;
  G4BaryonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void tst2ParticleConstructor::ConstructAllIons()
{
  //  Construct light ions
  G4cout << "Ions" << G4endl;
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();  
}

void tst2ParticleConstructor::ConstructAllShortLiveds()
{
  //  Construct  resonaces and quarks
  G4cout << "Shortlived particles" << G4endl;
  G4ShortLivedConstructor pConstructor;
  pConstructor.ConstructParticle();  
}


void tst2ParticleConstructor::ConstructParticle()
{

  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  // create all particles
  ConstructAllBosons();
  ConstructAllLeptons();
  ConstructAllMesons();
  ConstructAllBaryons();
  ConstructAllIons();
  ConstructAllShortLiveds();
}


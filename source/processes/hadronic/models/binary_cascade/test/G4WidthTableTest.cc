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
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     G4WidthTableTest.cc
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it), 
// 
//      Creation date: 15 April 1999
//
//      Modifications: 
//
// -------------------------------------------------------------------

#include "globals.hh"

#include "G4ios.hh"
#include "g4std/fstream"
#include "g4std/iomanip"
#include "g4std/iostream"
#include <assert.h>
#include "g4std/vector"

#include "CLHEP/Hist/TupleManager.h"
#include "CLHEP/Hist/HBookFile.h"
#include "CLHEP/Hist/Histogram.h"
#include "CLHEP/Hist/Tuple.h"

#include "Randomize.hh"

#include "G4AntiProton.hh"
#include "G4AntiNeutron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Gamma.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonPlus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4Lambda.hh"

#include "G4ParticleDefinition.hh"

#include "G4VDecayChannel.hh"
#include "G4DecayTable.hh"

#include "G4KineticTrack.hh"
#include "G4KineticTrackVector.hh"

#include "G4VShortLivedParticle.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4ParticleTable.hh"
#include "G4ShortLivedTable.hh"

#include "G4ResonanceNames.hh"


int main()
{
  // MGP ---- HBOOK initialization
  HepTupleManager* hbookManager;
  G4String fileName = "wtables.hbook";
  hbookManager = new HBookFile(fileName, 58);
  assert (hbookManager != 0);

  // ==== Initialization phase ====

  G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();

  G4ParticleDefinition* proton = G4Proton::ProtonDefinition();
  G4ParticleDefinition* antiProton = G4AntiProton::AntiProtonDefinition();
  G4ParticleDefinition* neutron = G4Neutron::NeutronDefinition();   
  G4ParticleDefinition* antiNeutron = G4AntiNeutron::AntiNeutronDefinition();   

  G4ParticleDefinition* pionPlus = G4PionPlus::PionPlusDefinition();
  G4ParticleDefinition* pionMinus = G4PionMinus::PionMinusDefinition();
  G4ParticleDefinition* pionZero = G4PionZero::PionZeroDefinition();

  G4ParticleDefinition* kaonPlus = G4KaonPlus::KaonPlusDefinition();
  G4ParticleDefinition* kaonMinus = G4KaonMinus::KaonMinusDefinition();
   
  G4ParticleDefinition* lambda = G4Lambda::LambdaDefinition();

  G4ParticleDefinition* theMuonPlus = G4MuonPlus::MuonPlusDefinition();
  G4ParticleDefinition* theMuonMinus = G4MuonMinus::MuonMinusDefinition();

  G4ParticleDefinition* theNeutrinoMu = G4NeutrinoMu::NeutrinoMuDefinition();
  G4ParticleDefinition* theAntiNeutrinoMu = G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

  // Construct resonances
  G4ShortLivedConstructor shortLived;
  shortLived.ConstructParticle();

  // Get the particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //  particleTable->DumpTable("ALL");

  // ==== End of the initialization phase ====


  // The list of resonances handled by the Kinetic Model
  G4ResonanceNames* resonanceList = new G4ResonanceNames;


  // ==== Nucleons ====

  G4cout << proton->GetParticleName() 
	 << G4endl; 

  G4cout << neutron->GetParticleName() 
	 << G4endl; 

  // ===== Delta =====

  const G4std::vector<G4String> listDelta = resonanceList->DeltaNames();
  G4int nDelta = listDelta.size();
  G4cout << G4endl << "===== Delta ===== " << G4endl;

  G4int i;
  for (i=0; i<nDelta; i++)
    {
      // Particle information
      G4String name = listDelta[i];
      G4ParticleDefinition* def = particleTable->FindParticle(name);
      if (def == 0) G4cout << name << "does not have a ParticleDefinition " << G4endl;
      G4cout << def->GetParticleName() << G4endl; 
      G4DecayTable* decayTable = def->GetDecayTable();
      decayTable->DumpInfo();
    }

 
  // ===== Excited Nucleons =====

  G4cout << G4endl << "===== Excited Nucleons ===== " << G4endl;

  const G4std::vector<G4String> listNstar = resonanceList->NstarNames();
  G4int nNstar = listNstar.size();

  for (i=0; i<nNstar; i++)
    {
      G4String name = listNstar[i];
      G4ParticleDefinition* def = particleTable->FindParticle(name);
      if (def == 0) G4cout << name << "does not have a ParticleDefinition " << G4endl;
      G4cout << def->GetParticleName() << G4endl; 
      G4DecayTable* decayTable = def->GetDecayTable();
      decayTable->DumpInfo();
    }

 
  // ===== Excited Deltas ===== 

  G4cout << G4endl << "===== Excited Deltas ===== " << G4endl;

  const G4std::vector<G4String> listDeltastar = resonanceList->DeltastarNames();
  G4int nDeltastar = listDeltastar.size();

  for (i=0; i<nDeltastar; i++)
    {
      G4String name = listDeltastar[i];
      G4ParticleDefinition* def = particleTable->FindParticle(name);
      if (def == 0) G4cout << name << "does not have a ParticleDefinition " << G4endl;
      G4cout << def->GetParticleName() << G4endl; 
      G4DecayTable* decayTable = def->GetDecayTable();
      decayTable->DumpInfo();
    }

  // ===== Lambdas ===== 

  G4cout << G4endl << "===== Lambdas ===== " << G4endl;

  const G4std::vector<G4String> listLambda = resonanceList->LambdaNames();
  G4int nLambda = listLambda.size();

  for (i=0; i<nLambda; i++)
    {
      G4String name = listLambda[i];
      G4ParticleDefinition* def = particleTable->FindParticle(name);
      if (def == 0) G4cout << name << "does not have a ParticleDefinition " << G4endl;
      G4cout << def->GetParticleName() << G4endl; 
      G4DecayTable* decayTable = def->GetDecayTable();
      decayTable->DumpInfo();
    }


  // ===== Sigmas ===== 

  G4cout << G4endl << "===== Sigmas ===== " << G4endl;
  
  const G4std::vector<G4String> listSigma = resonanceList->SigmaNames();
  G4int nSigma = listSigma.size();

  for (i=0; i<nSigma; i++)
    {
      G4String name = listSigma[i];
      G4ParticleDefinition* def = particleTable->FindParticle(name);
      if (def == 0) G4cout << name << "does not have a ParticleDefinition " << G4endl;
      G4cout << def->GetParticleName() << G4endl; 
      G4DecayTable* decayTable = def->GetDecayTable();
      decayTable->DumpInfo();
    }

  
  // ===== Xis ===== 

  G4cout << G4endl << "===== Xis ===== " << G4endl;
  
  const G4std::vector<G4String> listXi = resonanceList->XiNames();
  G4int nXi = listXi.size();

  for (i=0; i<nXi; i++)
    {
      G4String name = listXi[i];
      G4ParticleDefinition* def = particleTable->FindParticle(name);
      if (def == 0) G4cout << name << "does not have a ParticleDefinition " << G4endl;
      G4cout << def->GetParticleName() << G4endl; 
      G4DecayTable* decayTable = def->GetDecayTable();
      decayTable->DumpInfo();
    }


  hbookManager->write();

  G4cout << "ntuples are in file " << fileName<< G4endl;

  delete resonanceList;

  return EXIT_SUCCESS;
}

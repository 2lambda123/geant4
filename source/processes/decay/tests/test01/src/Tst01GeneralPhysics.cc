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
// $Id: Tst01GeneralPhysics.cc,v 1.3 2003-06-16 16:59:31 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#include "Tst01GeneralPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   

Tst01GeneralPhysics::Tst01GeneralPhysics(const G4String& name)
                     :  G4VPhysicsConstructor(name)
{
}

Tst01GeneralPhysics::~Tst01GeneralPhysics()
{
}

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4Gamma.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

#include "G4MesonConstructor.hh"

void Tst01GeneralPhysics::ConstructParticle()
{
   // gamma
  G4Gamma::GammaDefinition();
 
  // electron
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();

  // muon
  G4MuonMinus::MuonMinusDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
 
  // nucleon
  G4Proton::ProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4AntiNeutron::AntiNeutronDefinition();

  //  Construct all mesons
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();
}

void Tst01GeneralPhysics::ConstructProcess()
{
  // Add Decay Process
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (fDecayProcess.IsApplicable(*particle)) { 
      pmanager ->AddProcess(&fDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(&fDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(&fDecayProcess, idxAtRest);
    }
  }
}



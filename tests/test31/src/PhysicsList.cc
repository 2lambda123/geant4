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
// $Id: PhysicsList.cc,v 1.21 2006-11-16 11:33:41 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   PhysicsList
//
// Author:      V.Ivanchenko 03.05.2004
//
// Modified:
// 16.11.06 Use components from physics_lists subdirectory (V.Ivanchenko)
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "ParticlesBuilder.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics71.hh"
#include "G4EmStandardPhysics72.hh"
#include "PhysListEmLivermore.hh"
#include "PhysListEmPenelope.hh"
#include "G4StepLimiterBuilder.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4QStoppingPhysics.hh"
//#include "PhysListEmModelPai.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() 
: G4VModularPhysicsList()
{
  emBuilderIsRegisted = false;
  decayIsRegisted = false;
  stepLimiterIsRegisted = false;
  helIsRegisted = false;
  bicIsRegisted = false;
  ionIsRegisted = false;
  gnucIsRegisted = false;
  stopIsRegisted = false;
  verbose = 1;
  G4LossTableManager::Instance()->SetVerbose(1);
  defaultCutValue = 1.*mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  pMessenger = new PhysicsListMessenger(this);

  // Add Physics builders
  RegisterPhysics(new ParticlesBuilder());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  if(verbose > 0) 
    G4cout << "### PhysicsList Construte Particles" << G4endl;
  G4VModularPhysicsList::ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  if(verbose > 0) 
    G4cout << "### PhysicsList Construte Processes" << G4endl;
  if(!emBuilderIsRegisted) AddPhysicsList("standard");
  G4VModularPhysicsList::ConstructProcess();

  // Define energy interval for loss processes
  G4EmProcessOptions emOptions;
  emOptions.SetMinEnergy(0.1*keV);
  emOptions.SetMaxEnergy(100.*GeV);
  emOptions.SetDEDXBinning(90);
  emOptions.SetLambdaBinning(90);
  //  emOptions.SetBuildCSDARange(false);
  emOptions.SetApplyCuts(true);
  //emOptions.SetVerbose(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddPhysicsList(const G4String& name)
{
  if(verbose > 0) {
    G4cout << "### PhysicsList Add Physics <" << name 
           << "> emBuilderIsRegisted= " << emBuilderIsRegisted
           << G4endl;
  }
  if ((name == "standard") && !emBuilderIsRegisted) {
    RegisterPhysics(new G4EmStandardPhysics());
    emBuilderIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;    

  } else if (name == "standard_emv" && !emBuilderIsRegisted) {
    RegisterPhysics(new G4EmStandardPhysics71());
    emBuilderIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "standard_emx" && !emBuilderIsRegisted) {
    RegisterPhysics(new G4EmStandardPhysics72());
    emBuilderIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "lowenergy" && !emBuilderIsRegisted) {
    RegisterPhysics(new PhysListEmLivermore());
    emBuilderIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "penelope" && !emBuilderIsRegisted) {
    RegisterPhysics(new PhysListEmPenelope());
    emBuilderIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    /*
  } else if (name == "pai" && !emBuilderIsRegisted) {
    RegisterPhysics(new PhysListEmModelPai());
    emBuilderIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    */
  } else if (name == "step_limit" && !stepLimiterIsRegisted && emBuilderIsRegisted) {
    RegisterPhysics(new G4StepLimiterBuilder());
    stepLimiterIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "decay" && !decayIsRegisted && emBuilderIsRegisted) {
    RegisterPhysics(new G4DecayPhysics());
    decayIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "elastic" && !helIsRegisted && emBuilderIsRegisted) {
    RegisterPhysics(new G4HadronElasticPhysics());
    helIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    
  } else if (name == "binary" && !bicIsRegisted && emBuilderIsRegisted) {
    RegisterPhysics(new G4HadronInelasticQBBC());
    bicIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    
  } else if (name == "binary_ion" && !ionIsRegisted && emBuilderIsRegisted) {
    RegisterPhysics(new G4IonBinaryCascadePhysics());
    ionIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "gamma_nuc" && !gnucIsRegisted && emBuilderIsRegisted) {
    RegisterPhysics(new G4EmExtraPhysics());
    gnucIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "stopping" && !stopIsRegisted && emBuilderIsRegisted) {
    RegisterPhysics(new G4QStoppingPhysics());
    gnucIsRegisted = true;
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    
  } else if(!emBuilderIsRegisted) {
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" 
           << " fail - EM physics should be registered first " << G4endl;
  } else {
    G4cout << "PhysicsList::AddPhysicsList <" << name << ">" 
           << " fail - module is already regitered or is unknown " << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{

  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  if (verbose>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetVerbose(G4int val)
{
  verbose = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

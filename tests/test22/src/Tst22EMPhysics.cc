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
// $Id$
//
// 

#include <iomanip>   

#include "Tst22EMPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

Tst22EMPhysics::Tst22EMPhysics(const G4String& name)
               :  G4VPhysicsConstructor(name)
{
  theGammaReaction = new G4GammaNuclearReaction;
  theElectroReaction = new G4ElectroNuclearReaction;
}

Tst22EMPhysics::~Tst22EMPhysics()
{
}

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

void Tst22EMPhysics::ConstructParticle()
{
  // gamma
  G4Gamma::GammaDefinition();
 
  // electron
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}


#include "G4ProcessManager.hh"


void Tst22EMPhysics::ConstructProcess()
{

  G4ProcessManager * pManager = 0;
  G4cout << " Tst22EMPhysicsList::ConstructProcess() " << " called." << G4endl;
  
  // Gamma Physics
  pManager = G4Gamma::Gamma()->GetProcessManager();
  G4bool allPhysics = true;
  if( allPhysics ) {
     pManager->AddDiscreteProcess(&thePhotoEffect);
     pManager->AddDiscreteProcess(&theComptonEffect);
     pManager->AddDiscreteProcess(&thePairProduction);
  }

  theGammaReaction = new G4GammaNuclearReaction;
  theModel = new G4TheoFSGenerator;
  theCascade = new G4StringChipsParticleLevelInterface;
  theModel->SetTransport(theCascade);
  theModel->SetHighEnergyGenerator(&theStringModel);
  theStringDecay = new G4ExcitedStringDecay(&theFragmentation);
  theStringModel.SetFragmentationModel(theStringDecay);
  theGammaReaction->SetMaxEnergy(3.5*GeV);
  thePhotoNuclearProcess.RegisterMe(theGammaReaction);
  theModel->SetMinEnergy(3.*GeV);
  theModel->SetMaxEnergy(100*TeV);
  thePhotoNuclearProcess.RegisterMe(theModel);

  G4double  biasFactorPhotoNuclear= 100;
  thePhotoNuclearProcess.BiasCrossSectionByFactor(biasFactorPhotoNuclear);
  G4cout << " Bias factor for Phot Nuclear is set to " << biasFactorPhotoNuclear << G4endl;
  pManager->AddDiscreteProcess(&thePhotoNuclearProcess);

  // Electron Physics
  pManager = G4Electron::Electron()->GetProcessManager();
   // add processes
  pManager->AddDiscreteProcess(&theElectronBremsStrahlung);  
  pManager->AddProcess(&theElectronIonisation, ordInActive,2, 2);
  pManager->AddProcess(&theElectronMultipleScattering);
  theElectronNuclearProcess.RegisterMe(theElectroReaction);

  G4double biasFactorElectroNuclear= 1000000;
  theElectronNuclearProcess.BiasCrossSectionByFactor(biasFactorElectroNuclear);

  G4cout << " Bias factor for ElectronNuclear is set to " << biasFactorElectroNuclear << G4endl;
  pManager->AddDiscreteProcess(&theElectronNuclearProcess);
  
  pManager->SetProcessOrdering(&theElectronMultipleScattering, idxAlongStep,  1);
  pManager->SetProcessOrdering(&theElectronMultipleScattering, idxPostStep,  1);

  //Positron Physics
  pManager = G4Positron::Positron()->GetProcessManager();
  // add processes
  pManager->AddDiscreteProcess(&thePositronBremsStrahlung);
  pManager->AddDiscreteProcess(&theAnnihilation);
  pManager->AddRestProcess(&theAnnihilation);
  pManager->AddProcess(&thePositronIonisation, ordInActive,2, 2);
  G4cout << " Bias factor for PositronNuclear is set to " << biasFactorElectroNuclear << " - same process instance is used as for electron " << G4endl;
  thePositronNuclearProcess.RegisterMe(theElectroReaction);
  pManager->AddDiscreteProcess(&thePositronNuclearProcess);
  pManager->AddProcess(&thePositronMultipleScattering);
  pManager->SetProcessOrdering(&thePositronMultipleScattering, idxAlongStep,  1);
  pManager->SetProcessOrdering(&thePositronMultipleScattering, idxPostStep,  1);

}




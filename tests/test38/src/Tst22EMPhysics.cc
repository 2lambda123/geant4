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
// $Id: Tst22EMPhysics.cc,v 1.1 2006-05-03 07:09:59 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#include "Tst22EMPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   


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
  
  // Gamma Physics
  pManager = G4Gamma::Gamma()->GetProcessManager();
  pManager->AddDiscreteProcess(&thePhotoEffect);
  pManager->AddDiscreteProcess(&theComptonEffect);
  pManager->AddDiscreteProcess(&thePairProduction);
  
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
  pManager->AddDiscreteProcess(&thePhotoNuclearProcess);

  // Electron Physics
  pManager = G4Electron::Electron()->GetProcessManager();
   // add processes
  pManager->AddDiscreteProcess(&theElectronBremsStrahlung);  
  pManager->AddProcess(&theElectronIonisation, ordInActive,2, 2);
  pManager->AddProcess(&theElectronMultipleScattering);
  theElectronNuclearProcess.RegisterMe(theElectroReaction);
  theElectronNuclearProcess.BiasCrossSectionByFactor(1000);
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
  thePositronNuclearProcess.RegisterMe(theElectroReaction);
  pManager->AddDiscreteProcess(&thePositronNuclearProcess);
  pManager->AddProcess(&thePositronMultipleScattering);
  pManager->SetProcessOrdering(&thePositronMultipleScattering, idxAlongStep,  1);
  pManager->SetProcessOrdering(&thePositronMultipleScattering, idxPostStep,  1);

}




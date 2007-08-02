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
// $Id: PhysListEmPaiPhoton.cc,v 1.4 2007-08-02 12:01:53 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "PhysListEmPaiPhoton.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4MultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4PAIPhotonModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4Proton.hh"
#include "G4Electron.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmPaiPhoton::PhysListEmPaiPhoton(const G4String& name)
   :  G4VPhysicsConstructor(name)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmPaiPhoton::~PhysListEmPaiPhoton()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmPaiPhoton::ConstructProcess()
{
  // Add standard EM Processes

  const G4RegionStore* theRegionStore = G4RegionStore::GetInstance();
  G4Region* gas = theRegionStore->GetRegion("VertexDetector");

  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager     = particle->GetProcessManager();
    G4String particleName          = particle->GetParticleName();
     
    if (particleName == "gamma") 
    {     
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      
    } 
    else if (particleName == "e-") 
    { 
      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);

      G4eIonisation* eion = new G4eIonisation();   
      G4PAIPhotonModel*     pai = new G4PAIPhotonModel(particle,"PAIModel");
      eion->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(eion,-1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,-1,-1,3);
	    
    } 
    else if (particleName == "e+") 
    {
      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);

      G4eIonisation* eion = new G4eIonisation();   
      G4PAIPhotonModel*     pai = new G4PAIPhotonModel(particle,"PAIModel");
      eion->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(eion,        -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1,-1,3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);
      
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) 
    {
      pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);

      G4MuIonisation* muion = new G4MuIonisation();

      G4PAIPhotonModel*     pai = new G4PAIPhotonModel(particle,"PAIModel");
      
      muion->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(muion,      -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1,-1,3);
      pmanager->AddProcess(new G4MuPairProduction,  -1,-1,4);       

    } 
    else if (particleName == "GenericIon") 
    {
      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);
      pmanager->AddProcess(new G4ionIonisation,      -1, 2,2);     
    }
    else if ( ( !particle->IsShortLived() )       &&
	      ( particle->GetPDGCharge() != 0.0 ) && 
	      ( particle->GetParticleName() != "chargedgeantino") ) 
    {
      pmanager->AddProcess(new G4MultipleScattering,-1,1,1);

      G4hIonisation* hion =     new G4hIonisation();
      G4PAIPhotonModel*     pai = new G4PAIPhotonModel(particle,"PAIModel");
      hion->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(hion,       -1,2,2);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


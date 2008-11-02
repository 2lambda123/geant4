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
// $Id: PhysicsList.cc,v 1.5 2008-11-02 10:50:19 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4ProcessManager.hh"
#include "G4LossTableManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList(DetectorConstruction* det)
: G4VUserPhysicsList(), detector(det)
{
  defaultCutValue = 10.*km;
  singleScattering = false;
  registerBrem = false;
  pMessenger = new PhysicsListMessenger(this);
  SetVerboseLevel(1);
  
  G4LossTableManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ delete pMessenger; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4Geantino::GeantinoDefinition();
  G4Gamma::GammaDefinition();

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();

  G4Proton::ProtonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  AddStepMax();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4CoulombScattering.hh"

#include "G4eIonisation.hh"
#include "MyMollerBhabhaModel.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4hIonisation.hh"

#include "G4EmProcessOptions.hh"
#include "G4MscStepLimitType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void PhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4int iAlong = 0, iPost = 0; 
     
    if (particleName == "gamma") {
                 
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);         
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      
    } else if (particleName == "e-") {

      if (singleScattering)
        pmanager->AddProcess(new G4CoulombScattering,  -1, -1,       ++iPost);
        else    
        pmanager->AddProcess(new G4eMultipleScattering,-1, ++iAlong, ++iPost);
      
      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetEmModel(new MyMollerBhabhaModel);             
      pmanager->AddProcess(eIoni,                      -1, ++iAlong, ++iPost);
      
      if (registerBrem)
        pmanager->AddProcess(new G4eBremsstrahlung,    -1, ++iAlong, ++iPost);      
       
    } else if (particleName == "e+") {
    
      if (singleScattering)
        pmanager->AddProcess(new G4CoulombScattering,  -1, -1,       ++iPost);
        else   
        pmanager->AddProcess(new G4eMultipleScattering,-1, ++iAlong, ++iPost);
      
      G4eIonisation* pIoni = new G4eIonisation();
      pIoni->SetEmModel(new MyMollerBhabhaModel);                   
      pmanager->AddProcess(pIoni,                      -1, ++iAlong, ++iPost);
      
      if (registerBrem) {
        pmanager->AddProcess(new G4eBremsstrahlung,    -1, ++iAlong, ++iPost);
        pmanager->AddProcess(new G4eplusAnnihilation,   0, -1,       ++iPost);
      }
      
    } else if (particleName == "proton") {
    
      if (singleScattering)
        pmanager->AddProcess(new G4CoulombScattering,  -1, -1,       ++iPost);
        else   
        pmanager->AddProcess(new G4hMultipleScattering,-1, ++iAlong, ++iPost);
	
      pmanager->AddProcess(new G4hIonisation,          -1, ++iAlong, ++iPost);
    }
  }
  
  // Em options
  //
  // Main options and setting parameters are shown here.
  // Several of them have default values.
  //
  G4EmProcessOptions emOptions;
  
  //physics tables
  //
  emOptions.SetMinEnergy(100*eV);	//default    
  emOptions.SetMaxEnergy(10*GeV);
  emOptions.SetDEDXBinning(80);		//default=56  
  emOptions.SetLambdaBinning(80);	//default=56  
  emOptions.SetSplineFlag(true);	//default
      
  //multiple coulomb scattering
  //
  emOptions.SetMscStepLimitation(fUseDistanceToBoundary);   //default=fUseSafety
  emOptions.SetMscRangeFactor(0.02);	//default=0.04
  emOptions.SetMscGeomFactor (2.5);	//default       
  emOptions.SetSkin(3.);		//default
      
  //energy loss
  //
  emOptions.SetStepFunction(0.2, 10*um);	//default=(0.2, 1*mm)   
  emOptions.SetLinearLossLimit(1.e-3);		//default=1.e-2
   
          
  //build CSDA range
  //
  emOptions.SetBuildCSDARange(true);		//default=false
  emOptions.SetMaxEnergyForCSDARange(10*GeV);  
  emOptions.SetDEDXBinningForCSDARange(80);      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "StepMax.hh"

void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  StepMax* stepMaxProcess = new StepMax();

  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (stepMaxProcess->IsApplicable(*particle))
        {
	  pmanager ->AddDiscreteProcess(stepMaxProcess);
        }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UnitsTable.hh"

void PhysicsList::SetCuts()
{
  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }
     
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "e-");
  SetCutValue(defaultCutValue, "e+");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SingleCoulombScattering(G4bool flag)
{
  singleScattering = flag;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::RegisterBrem(G4bool flag)
{
  registerBrem = flag;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......





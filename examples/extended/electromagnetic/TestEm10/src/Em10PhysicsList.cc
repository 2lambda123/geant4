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
// $Id: Em10PhysicsList.cc,v 1.10 2005-01-14 11:42:14 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "G4Timer.hh"

#include "Em10PhysicsList.hh"
#include "Em10DetectorConstruction.hh"
#include "Em10PhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4EnergyLossTables.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include <iomanip>

#include "G4FastSimulationManagerProcess.hh"


/////////////////////////////////////////////////////////////
//
//

Em10PhysicsList::Em10PhysicsList(Em10DetectorConstruction* p)
  :  G4VUserPhysicsList(),
     MaxChargedStep(DBL_MAX),
     thePhotoElectricEffect(0),      theComptonScattering(0),
     theGammaConversion(0),
     theeminusMultipleScattering(0), theeminusIonisation(0),
     theeminusBremsstrahlung(0),
     theeplusMultipleScattering(0),  theeplusIonisation(0),
     theeplusBremsstrahlung(0),
     theeplusAnnihilation(0),
     theeminusStepCut(0),            theeplusStepCut(0)
{
  pDet = p;

  defaultCutValue = 1.000*mm ;

  cutForGamma = defaultCutValue ;
  cutForElectron = defaultCutValue ;

  SetVerboseLevel(1);
  physicsListMessenger = new Em10PhysicsListMessenger(this);
}

/////////////////////////////////////////////////////////////////////////
//
//

Em10PhysicsList::~Em10PhysicsList()
{
  delete physicsListMessenger; 
}

///////////////////////////////////////////////////////////////////////////
//
//

void Em10PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBarions();
}

////////////////////////////////////////////////////////////////////////////
//
//

void Em10PhysicsList::ConstructBosons()
{
  // gamma
  G4Gamma::GammaDefinition();
}

void Em10PhysicsList::ConstructLeptons()
{
  // leptons

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void Em10PhysicsList::ConstructMesons()
{
 //  mesons

  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
}


void Em10PhysicsList::ConstructBarions()
{
//  barions

  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
}


///////////////////////////////////////////////////////////////////////
//
//

void Em10PhysicsList::ConstructProcess()
{
  AddTransportation();
  // AddParameterisation();

  ConstructEM();
  ConstructGeneral();
}

/////////////////////////////////////////////////////////////////////////////
//
//

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4MultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4PAIModel.hh"
#include "G4PAIPhotonModel.hh"
#include "G4PAIwithPhotons.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

#include "G4ForwardXrayTR.hh"
#include "G4RegularXTRadiator.hh"

#include "Em10StepCut.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"



void Em10PhysicsList::ConstructEM()
{
  const G4RegionStore* theRegionStore = G4RegionStore::GetInstance();
  G4Region* gas = theRegionStore->GetRegion("XTRdEdxDetector");

  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma")
    {
      // Construct processes for gamma

      thePhotoElectricEffect = new G4PhotoElectricEffect();
      theComptonScattering   = new G4ComptonScattering();
      theGammaConversion     = new G4GammaConversion();

      pmanager->AddDiscreteProcess(thePhotoElectricEffect);
      pmanager->AddDiscreteProcess(theComptonScattering);
      pmanager->AddDiscreteProcess(theGammaConversion);

    }
    else if (particleName == "e-")
    {
      // Construct processes for electron

      theeminusMultipleScattering = new G4MultipleScattering();
      theeminusBremsstrahlung = new G4eBremsstrahlung();

      theeminusStepCut = new Em10StepCut();
      
      theeminusIonisation = new G4eIonisation();
      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      theeminusIonisation->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(theeminusIonisation,-1,2,2);
      
      pmanager->AddProcess(theeminusMultipleScattering,-1,1,1);
      pmanager->AddProcess(theeminusBremsstrahlung,-1,-1,3);

      pmanager->AddContinuousProcess(
                 new G4RegularXTRadiator(pDet->GetLogicalRadiator(),
						    pDet->GetFoilMaterial(),
						    pDet->GetGasMaterial(),
						    pDet->GetFoilThick(),
						    pDet->GetGasThick(),
						    pDet->GetFoilNumber(),
					 "RegularXTRadiator"));
       // ,-1,1,-1);


      pmanager->AddProcess(theeminusStepCut,-1,-1,4);
      theeminusStepCut->SetMaxStep(MaxChargedStep) ;

    }
    else if (particleName == "e+")
    {
      // Construct processes for positron

      theeplusMultipleScattering = new G4MultipleScattering();
      theeplusIonisation = new G4eIonisation();
      theeplusBremsstrahlung = new G4eBremsstrahlung();
      theeplusAnnihilation = new G4eplusAnnihilation();


      theeplusStepCut = new Em10StepCut();

      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      theeplusIonisation->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(theeplusMultipleScattering,-1,1,1);
      pmanager->AddProcess(theeplusIonisation,-1,2,2);
      pmanager->AddProcess(theeplusBremsstrahlung,-1,-1,3);
      pmanager->AddProcess(theeplusAnnihilation,0,-1,4);


      pmanager->AddProcess(theeplusStepCut,-1,-1,5);
      theeplusStepCut->SetMaxStep(MaxChargedStep) ;

    }
    else if( particleName == "mu+" ||
               particleName == "mu-"    )
    {
     // Construct processes for muon+

      Em10StepCut* muonStepCut = new Em10StepCut();

      G4MuIonisation* themuIonisation = new G4MuIonisation() ;

      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      themuIonisation->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
      pmanager->AddProcess(themuIonisation,-1,2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);
      pmanager->AddProcess(new G4MuPairProduction(),-1,-1,4);

      pmanager->AddProcess( muonStepCut,-1,-1,5);
      muonStepCut->SetMaxStep(MaxChargedStep) ;

    }
    else if (
                particleName == "proton"
               || particleName == "antiproton"
               || particleName == "pi+"
               || particleName == "pi-"
               || particleName == "kaon+"
               || particleName == "kaon-"
              )
    {
      Em10StepCut* thehadronStepCut = new Em10StepCut();

      G4hIonisation* thehIonisation = new G4hIonisation();
      G4MultipleScattering* thehMultipleScattering =
                        new G4MultipleScattering() ;


      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      thehIonisation->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(thehMultipleScattering,-1,1,1);
      pmanager->AddProcess(thehIonisation,-1,2,2);

      pmanager->AddProcess( thehadronStepCut,-1,-1,3);
      thehadronStepCut->SetMaxStep(MaxChargedStep) ;

    }
  }
}


#include "G4Decay.hh"

void Em10PhysicsList::ConstructGeneral()
{
  // Add Decay Process

   G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (theDecayProcess->IsApplicable(*particle)) 
    { 
      pmanager ->AddProcess(theDecayProcess);

      // set ordering for PostStepDoIt and AtRestDoIt

      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
/*
void Em10PhysicsList::AddParameterisation()
{
  G4FastSimulationManagerProcess* theFastSimulationManagerProcess = 
                                  new G4FastSimulationManagerProcess() ;
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value() ;
    G4ProcessManager* pmanager = particle->GetProcessManager() ;

    // both postStep and alongStep action are required: because
    // of the use of ghost volumes. If no ghost, the postStep is sufficient.

    pmanager->AddProcess(theFastSimulationManagerProcess, -1, 1, 1);
  }
}
*/


/////////////////////////////////////////////////////////////////////////////

void Em10PhysicsList::SetCuts()
{
  if (verboseLevel >0)
  {
    G4cout << "Em10PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }  
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
 
  SetCutValue(cutForGamma,"gamma");

  SetCutValue(cutForElectron,"e-");
  SetCutValue(cutForElectron,"e+");

  if (verboseLevel>1)     DumpCutValuesTable();
}

///////////////////////////////////////////////////////////////////////////

void Em10PhysicsList::SetGammaCut(G4double val)
{
  cutForGamma = val;
}

///////////////////////////////////////////////////////////////////////////

void Em10PhysicsList::SetElectronCut(G4double val)
{
  cutForElectron = val;
}

////////////////////////////////////////////////////////////////////////////

void Em10PhysicsList::SetMaxStep(G4double step)
{
  MaxChargedStep = step ;
  G4cout << " MaxChargedStep=" << MaxChargedStep << G4endl;
  G4cout << G4endl;
}


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
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
   
//
// Always place it in front!
//
#include "G4Timer.hh"

#include "Test17PhysicsList.hh"
#include "Test17DetectorConstruction.hh"
#include "Test17PhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4IonC12.hh"
#include "G4IonAr40.hh"
#include "G4IonFe56.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4EnergyLossTables.hh"
#include "G4ios.hh"
#include "g4std/iomanip"                

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Test17PhysicsList::Test17PhysicsList(Test17DetectorConstruction* p)
:G4VUserPhysicsList()
{
  pDet = p;

  defaultCutValue = 0.1*mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForProton    = defaultCutValue;
  
  //  MaxChargedStep = DBL_MAX; 
  MaxChargedStep = 0.1*mm; 
  
  SetVerboseLevel(2);
  physicsListMessenger = new Test17PhysicsListMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Test17PhysicsList::~Test17PhysicsList()
{
  delete physicsListMessenger; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructBarions();
  ConstructMesons();
  ConstructIons();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructBosons()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
  // gamma
  G4Gamma::GammaDefinition();
}
 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructLeptons()
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructMesons()
{
 //  mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructBarions()
{
//  barions
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructIons()
{
//  Ions
  G4GenericIon::GenericIonDefinition();
  G4Deuteron::DeuteronDefinition();
  G4Alpha::AlphaDefinition();
  G4IonC12::IonC12Definition();
  //  G4IonAr40::IonAr40Definition();
  //  G4IonFe56::IonFe56Definition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4MultipleScattering.hh"

#include "G4LowEnergyBremsstrahlung.hh"
#include "G4LowEnergyIonisation.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCaptureAtRest.hh"

#include "G4hIonisation.hh"
#include "G4hLowEnergyIonisation.hh"

#include "Test17StepCut.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  theStepCut = new Test17StepCut();
  theStepCut->SetMaxStep(MaxChargedStep);
          		       
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
    // gamma
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);    
      
    } else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1,-1,3);       

      pmanager->AddProcess(theStepCut,         -1,-1,4);

    } else if (particleName == "e+") {
      //positron      
      pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1,-1,3);
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);
                  
      pmanager->AddProcess(theStepCut,          -1,-1,5);
  
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
     //muon  
     pmanager->AddProcess(new G4MultipleScattering,-1, 1,1);
     pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
     pmanager->AddProcess(new G4MuBremsstrahlung,  -1,-1,3);
     pmanager->AddProcess(new G4MuPairProduction,  -1,-1,4);       	       
     pmanager->AddProcess(new G4MuonMinusCaptureAtRest,0,-1,-1);       	       

    } else if (
                particleName == "proton"  
               || particleName == "anti_proton"  
               || particleName == "pi+"  
               || particleName == "pi-"  
               || particleName == "kaon+"  
               || particleName == "kaon-"  
              )
    {
      pmanager->AddProcess(new G4MultipleScattering,-1,1,1);

      G4cout << "Hadronic processes for " << particleName << G4endl; 

      // Standard ionisation
      //       G4hIonisation* hIon = new G4hIonisation() ;

      // Standard ionisation with low energy extention
      G4hLowEnergyIonisation* hIon = new G4hLowEnergyIonisation() ;
      //      hIon->SetNuclearStoppingOff() ;
      //         hIon->SetBarkasOff() 
      // hIon->SetNuclearStoppingOff() ;
      // hIon->SetAntiProtonStoppingOff() ;
      //      hIon->SetBarkasOff() ;
      //      hIon->SetNuclearStoppingOn() ;

      //hIon->SetStoppingPowerTableName("Ziegler1977He") ;
     //   hIon->SetElectronicStoppingPowerModel(particle,"Ziegler1977p") ;
      //       hIon->SetNuclearStoppingPowerModel("ICRU_R49Mollere") ;
      //   hIon->SetElectronicStoppingPowerModel(particle,"ICRU_R49p") ;

      pmanager->AddProcess(hIon,-1,2,2);
      hionVector.push_back(hIon);
      
      pmanager->AddProcess( theStepCut,       -1,-1,3);
   
    } else if (   particleName == "alpha"  
               || particleName == "deuteron"  
               || particleName == "IonC12"  
               || particleName == "IonAr40"  
               || particleName == "IonFe56"  
               || particleName == "GenericIon"  
              )
    {
      pmanager->AddProcess(new G4MultipleScattering,-1,1,1);

      G4cout << "Ionic processes for " << particleName << G4endl; 

      // Standard ionisation
      //      G4hIonisation* iIon = new G4hIonisation() ;

      // Standard ionisation with low energy extantion
       G4hLowEnergyIonisation* iIon = new G4hLowEnergyIonisation() ;
       	   iIon->SetVerboseLevel(1);
	   //   iIon->SetNuclearStoppingOff() ;
	//  iIon->SetNuclearStoppingOn() ;

      //iIon->SetStoppingPowerTableName("Ziegler1977He") ;
      //iIon->SetStoppingPowerTableName("Ziegler1977H") ;
	// iIon->SetStoppingPowerTableName("ICRU_R49p") ;
      //iIon->SetStoppingPowerTableName("ICRU_R49He") ;
      //iIon->SetStoppingPowerTableName("ICRU_R49PowersHe") ;

      pmanager->AddProcess(iIon,-1,2,2);
      //      hionVector.push_back(iIon);
      
      pmanager->AddProcess( theStepCut,       -1,-1,3);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4Decay.hh"

void Test17PhysicsList::ConstructGeneral()
{
  // Add Decay Process
   G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetCuts()
{
  // G4Timer theTimer ;
  // theTimer.Start() ;

  //special for low energy physics
  //
  G4double lowlimit=1*eV;  
  G4Gamma   ::SetEnergyRange(lowlimit,100*GeV);
  G4Electron::SetEnergyRange(lowlimit,100*GeV);
  G4Positron::SetEnergyRange(lowlimit,100*GeV);
   
  if (verboseLevel >0){
    G4cout << "Test17PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(MaxChargedStep,"Length") << G4endl;
  }  
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 

      G4cout << "Set cuts for all particles! " << G4endl; 

   SetCutValue(cutForGamma,"gamma");

   SetCutValue(cutForElectron,"e-");
   SetCutValue(cutForElectron,"e+");

   SetCutValue(cutForProton,"mu-");
   SetCutValue(cutForProton,"mu+");

  // set cut values for proton and anti_proton before all other hadrons
  // because some processes for hadrons need cut values for proton/anti_proton 

     SetCutValue(cutForProton, "proton");

     SetCutValue(cutForProton, "anti_proton");

     SetCutValueForOthers(cutForProton);

  //  G4cout << "Dump the table!" << G4endl;
              
  if (verboseLevel>0) DumpCutValuesTable();

  //  theTimer.Stop();
  //  G4cout.precision(6);
  //  G4cout << G4endl ;
  //  G4cout << "total time(SetCuts)=" << theTimer.GetUserElapsed() << " s " <<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetGammaCut(G4double val)
{
  ResetCuts();
  cutForGamma = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetElectronCut(G4double val)
{
  ResetCuts();
  cutForElectron = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetProtonCut(G4double val)
{
  //  ResetCuts();
  cutForProton = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetCutsByEnergy(G4double val)
{
  G4ParticleTable* theTest17ParticleTable =  G4ParticleTable::GetParticleTable();
  G4Material* currMat = pDet->GetAbsorberMaterial();

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  G4ParticleDefinition* part;
  G4double cut;

  part = theTest17ParticleTable->FindParticle("e-");
  cut = G4EnergyLossTables::GetRange(part,val,currMat);
  SetCutValue(cut, "e-");

  part = theTest17ParticleTable->FindParticle("e+");
  cut = G4EnergyLossTables::GetRange(part,val,currMat);
  SetCutValue(cut, "e+");

  // set cut values for proton and anti_proton before all other hadrons
  // because some processes for hadrons need cut values for proton/anti_proton
  part = theTest17ParticleTable->FindParticle("proton");
  cut = G4EnergyLossTables::GetRange(part,val,currMat);
  SetCutValue(cut, "proton");

  part = theTest17ParticleTable->FindParticle("anti_proton");
  cut = G4EnergyLossTables::GetRange(part,val,currMat);
  SetCutValue(cut, "anti_proton");

  SetCutValueForOthers(cut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::GetRange(G4double val)
{
  G4ParticleTable* theTest17ParticleTable =  G4ParticleTable::GetParticleTable();
  G4Material* currMat = pDet->GetAbsorberMaterial();

  G4ParticleDefinition* part;
  G4double cut;
  part = theTest17ParticleTable->FindParticle("e-");
  cut = G4EnergyLossTables::GetRange(part,val,currMat);
  G4cout << "material : " << currMat->GetName() << G4endl;
  G4cout << "particle : " << part->GetParticleName() << G4endl;
  G4cout << "energy   : " << val / keV << " (keV)" << G4endl;
  G4cout << "range    : " << cut / mm << " (mm)" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetMaxStep(G4double step)
{
  MaxChargedStep = step ;
  theStepCut->SetMaxStep(MaxChargedStep);
  G4cout << "Test17PhysicsList:MaxChargedStep=" << MaxChargedStep << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetCutForSecondaryPhotons(G4double cut)
{
  size_t n = hionVector.size();
  G4cout << " Cut for secondary gamma = " << cut/keV << " keV" << G4endl;

  for (size_t i=0; i<n; i++) {    
    hionVector[i]->SetCutForSecondaryPhotons(cut);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Test17PhysicsList::SetCutForAugerElectrons(G4double cut)
{
  size_t n = hionVector.size();
  G4cout << " Cut for Auger electrons = " << cut/keV << " keV" << G4endl;

  for (size_t i=0; i<n; i++) {    
    hionVector[i]->SetCutForAugerElectrons(cut);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....






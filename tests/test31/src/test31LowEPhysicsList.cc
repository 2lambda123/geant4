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
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
//
// ClassName:   test31LowEPhysicsList
//  
// Description: LowEnergy EM processes list
//
// Authors:    08.04.01 V.Ivanchenko 
//
// Modified:
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "test31LowEPhysicsList.hh"
#include "G4ProcessManager.hh"

#include "G4LowEnergyRayleigh.hh"
#include "G4LowEnergyCompton.hh"   
#include "G4LowEnergyGammaConversion.hh"
#include "G4LowEnergyPhotoElectric.hh"

#include "G4MultipleScattering.hh"
#include "G4MultipleScatteringSTD.hh"

#include "G4eIonisationSTD.hh"
#include "G4eBremsstrahlungSTD.hh"
#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisationSTD.hh"
#include "G4MuBremsstrahlungSTD.hh"
#include "G4MuPairProductionSTD.hh"
#include "G4MuonMinusCaptureAtRest.hh"

#include "G4hLowEnergyIonisation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

test31LowEPhysicsList::test31LowEPhysicsList(const G4String& name,
  G4int ver, G4bool nuc, G4bool bar, const G4String& tab) :
  G4VPhysicsConstructor(name),
  verbose(ver),
  nuclStop(nuc),
  barkas(bar),
  table(tab)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

test31LowEPhysicsList::~test31LowEPhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void test31LowEPhysicsList::ConstructParticle()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void test31LowEPhysicsList::ConstructProcess()
{
  G4cout << "LowEnergy Electromagnetic PhysicsList is initilized" << G4endl;
  G4ParticleDefinition* proton = G4Proton::ProtonDefinition();
  G4ParticleDefinition* antiproton = G4AntiProton::AntiProtonDefinition();
  // nuclStop = false;
  G4double eCut = 0.1*keV;
  G4double gCut = 0.3*keV;

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4String particleType = particle->GetParticleType();
    G4double charge = particle->GetPDGCharge();

    if(0 < verbose) G4cout << "LowE EM processes for " << particleName << G4endl;

    if (particleName == "gamma") {
      if(0 < verbose) G4cout << "LowE gamma" << G4endl;
      pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh());
      G4LowEnergyPhotoElectric* pe = new G4LowEnergyPhotoElectric();
      pe->SetCutForLowEnSecPhotons(gCut);
      pe->SetCutForLowEnSecElectrons(eCut);
      //pe->ActivateAuger(true);
      pmanager->AddDiscreteProcess(pe);
      pmanager->AddDiscreteProcess(new G4LowEnergyCompton());
      pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion());

    } else if (particleName == "e-") {
      if(0 < verbose) G4cout << "LowE e-" << G4endl;
      pmanager->AddProcess(new G4MultipleScatteringSTD(), -1, 1,1);
      G4LowEnergyIonisation* lei = new G4LowEnergyIonisation();
      lei->SetCutForLowEnSecPhotons(gCut);
      lei->SetCutForLowEnSecElectrons(eCut);
      //lei->SetEnlossFluc(false);
      //lei->ActivateAuger(true);
//      lei->ActivateFluorescence(true);
      lei->SetVerboseLevel(verbose);
      G4LowEnergyBremsstrahlung* bre = new G4LowEnergyBremsstrahlung();
      bre->SetVerboseLevel(verbose);
      pmanager->AddProcess(lei,  -1, 2,2);
      pmanager->AddProcess(bre,  -1,-1,3);

    } else if (particleName == "e+") {
      if(0 < verbose) G4cout << "LowE e+" << G4endl;
      pmanager->AddProcess(new G4MultipleScatteringSTD(), -1, 1,1);
      pmanager->AddProcess(new G4eIonisationSTD,        -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlungSTD,    -1,-1,3);
      pmanager->AddProcess(new G4eplusAnnihilation(),   0,-1,4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
      if(0 < verbose) G4cout << "LowE " << particleName << G4endl;
      pmanager->AddProcess(new G4MultipleScatteringSTD(),-1, 1,1);
      pmanager->AddProcess(new G4MuIonisationSTD(),      -1, 2,2);
      pmanager->AddProcess(new G4MuBremsstrahlungSTD(),  -1,-1,3);
      pmanager->AddProcess(new G4MuPairProductionSTD(),  -1,-1,4);
      if(particleName == "mu-")
        pmanager->AddProcess(new G4MuonMinusCaptureAtRest(),0,-1,-1);

    } else if (
                particleName == "proton"
               || particleName == "anti_proton"
               || particleName == "pi+"
               || particleName == "pi-"
               || particleName == "kaon+"
               || particleName == "kaon-"
               || particleName == "sigma+"
               || particleName == "sigma-"
              )
    {
      if(0 < verbose) G4cout << "LowE " << particleName << G4endl;
      pmanager->AddProcess(new G4MultipleScatteringSTD(),-1,1,1);
      G4hLowEnergyIonisation* hIon = new G4hLowEnergyIonisation() ;

      if(nuclStop) hIon->SetNuclearStoppingOn();
      else         hIon->SetNuclearStoppingOff();

      if(barkas)   hIon->SetBarkasOn();
      else         hIon->SetBarkasOff();

      hIon->SetVerboseLevel(verbose);
      //   hIon->SetEnlossFluc(false);

      if(table == G4String("Ziegler1977He") ||
         table == G4String("Ziegler1977H") ||
         table == G4String("Ziegler1985p") ||
         table == G4String("ICRU_R49p") ||
         table == G4String("ICRU_R49He") ||
         table == G4String("ICRU_R49PowersHe") ) {

	if(particle->GetPDGCharge() > 0.0)
          hIon->SetElectronicStoppingPowerModel(proton,table);
        else
          hIon->SetElectronicStoppingPowerModel(antiproton,table);
      }

      pmanager->AddProcess(hIon,-1,2,2);

    } else if (   particleName == "alpha"
               || particleName == "deuteron"
               || particleName == "triton"
               || particleName == "IonC12"
               || particleName == "IonAr40"
               || particleName == "IonFe56"
               || particleName == "He3"
               || particleName == "GenericIon"
               || (particleType == "nucleus" && charge != 0)
              )
    {
      if(0 < verbose) G4cout << "LowE " << particleName << G4endl;
      pmanager->AddProcess(new G4MultipleScatteringSTD(),-1,1,1);
      G4hLowEnergyIonisation* iIon = new G4hLowEnergyIonisation() ;

      if(nuclStop) iIon->SetNuclearStoppingOn();
      else         iIon->SetNuclearStoppingOff();

      if(barkas)   iIon->SetBarkasOn();
      else         iIon->SetBarkasOff();

      iIon->SetVerboseLevel(verbose);

      if(table == G4String("Ziegler1977He") ||
         table == G4String("Ziegler1977H") ||
         table == G4String("ICRU_R49p") ||
         table == G4String("ICRU_R49He") ||
         table == G4String("ICRU_R49PowersHe") )
         iIon->SetElectronicStoppingPowerModel(proton,table);

      pmanager->AddProcess(iIon,-1,2,2);
    }
    if(2 < verbose) pmanager->DumpInfo();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



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
// $Id: G4LossTableManager.cc,v 1.23 2003-08-29 07:34:04 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4LossTableManager
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 03.01.2002
//
// Modifications:
//
// 20-01-03 Migrade to cut per region (V.Ivanchenko)
// 15-02-03 Lambda table can be scaled (V.Ivanchenko)
// 17-02-03 Fix problem of store/restore tables (V.Ivanchenko)
// 10-03-03 Add Ion registration (V.Ivanchenko)
// 25-03-03 Add deregistration (V.Ivanchenko)
// 02-04-03 Change messenger (V.Ivanchenko)
// 26-04-03 Fix retrieve tables (V.Ivanchenko)
// 13-05-03 Add calculation of precise range (V.Ivanchenko)
// 23-07-03 Add exchange with G4EnergyLossTables (V.Ivanchenko)
//
// Class Description:
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4LossTableManager.hh"
#include "G4EnergyLossMessenger.hh"
#include "G4PhysicsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4ProcessManager.hh"
#include "G4Electron.hh"
#include "G4VMultipleScattering.hh"

G4LossTableManager* G4LossTableManager::theInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4LossTableManager* G4LossTableManager::Instance()
{
  if(0 == theInstance) {
    theInstance = new G4LossTableManager();
  }
  return theInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4LossTableManager::~G4LossTableManager()
{
  for (G4int i=0; i<n_loss; i++) {
    if( loss_vector[i] ) delete loss_vector[i];
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j] ) delete msc_vector[j];
  }  Clear();
  delete theMessenger;
  delete tableBuilder;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4LossTableManager::G4LossTableManager()
{
  n_loss = 0;
  first_entry = true;
  all_tables_are_built = false;
  all_tables_are_stored = false;
  electron_table_are_built = false;
  currentLoss = 0;
  currentParticle = 0;
  lossFluctuationFlag = true;
  subCutoffFlag = false;
  rndmStepFlag = false;
  minSubRange = 0.0;
  maxRangeVariation = 1.0;
  maxFinalStep = 0.0;
  eIonisation = 0;
  minKinEnergy = 0.1*eV;
  maxKinEnergy = 100.0*GeV;
  theMessenger = new G4EnergyLossMessenger();
  theElectron  = G4Electron::Electron();
  tableBuilder = new G4LossTableBuilder();
  integral = true;
  integralActive = false;
  buildPreciseRange = false;
  minEnergyActive = false;
  maxEnergyActive = false;
  verbose = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::Clear()
{
  all_tables_are_built = false;
  electron_table_are_built = false;
  eIonisation = 0;
  currentLoss = 0;
  currentParticle = 0;
  if(n_loss)
    {
      dedx_vector.clear();
      range_vector.clear();
      inv_range_vector.clear();
      loss_map.clear();
      loss_vector.clear();
      part_vector.clear();
      base_part_vector.clear();
      tables_are_built.clear();
      n_loss = 0;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::Register(G4VEnergyLossSTD* p)
{
  n_loss++;
  loss_vector.push_back(p);
  part_vector.push_back(0);
  base_part_vector.push_back(0);
  dedx_vector.push_back(0);
  range_vector.push_back(0);
  inv_range_vector.push_back(0);
  tables_are_built.push_back(false);
  all_tables_are_built = false;
  if(integralActive) p->SetIntegral(integral);
  if(minEnergyActive) p->SetMinKinEnergy(minKinEnergy);
  if(maxEnergyActive) p->SetMaxKinEnergy(maxKinEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::DeRegister(G4VEnergyLossSTD* p)
{
  for (G4int i=0; i<n_loss; i++) {
    if(loss_vector[i] == p) loss_vector[i] = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::Register(G4VMultipleScattering* p)
{
  msc_vector.push_back(p);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::DeRegister(G4VMultipleScattering* p)
{
  size_t msc = msc_vector.size();
  for (size_t i=0; i<msc; i++) {
    if(msc_vector[i] == p) msc_vector[i] = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::RegisterIon(const G4ParticleDefinition* ion, G4VEnergyLossSTD* p)
{
  loss_map[ion] = p;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4EnergyLossMessenger* G4LossTableManager::GetMessenger()
{
  return theMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::ParticleHaveNoLoss(const G4ParticleDefinition* aParticle)
{
  G4String s = "G4LossTableManager:: dE/dx table not found for "
             + aParticle->GetParticleName() + "!";
  G4Exception(s);
  exit(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::Initialise(const G4ParticleDefinition* aParticle)
{
  if (first_entry) {
    first_entry = false;
    firstParticle = aParticle;
  }

  for(G4int j=0; j<n_loss; j++) {

    if (loss_vector[j]) {
      const G4ParticleDefinition* pd = loss_vector[j]->Particle();
      if (!pd) {
        pd = loss_vector[j]->GetProcessManager()->GetParticleType();
        loss_vector[j]->SetParticle(pd);
      }
      if(!tables_are_built[j]) all_tables_are_built = false;
      if(pd != part_vector[j]) {
        part_vector[j] = pd;
        base_part_vector[j] = loss_vector[j]->BaseParticle();
      }
      if(1 < verbose) {
        G4String nm = "unknown";
        if(pd) nm = pd->GetParticleName();
        G4cout << "For " << loss_vector[j]->GetProcessName()
               << " for " << nm
               << " tables_are_built= " << tables_are_built[j]
               << " procFlag= " << loss_vector[j]->TablesAreBuilt()
               << " all_tables_are_built= "     << all_tables_are_built
               << G4endl;
      }
    } else {
      tables_are_built[j] = true;
      part_vector[j] = 0;
    }
  }

  // All tables have to be rebuilt
  if (all_tables_are_built && firstParticle == aParticle) {
    for (G4int i=0; i<n_loss; i++) {
      if (loss_vector[i]) tables_are_built[i] = false;
    }
    all_tables_are_built = false;
    electron_table_are_built = false;
    loss_map.clear();
  }

  if(1 < verbose) {
    G4cout << "electron_table_are_built= "  << electron_table_are_built
           << " all_tables_are_built= "     << all_tables_are_built
           << G4endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::BuildPhysicsTable(const G4ParticleDefinition* aParticle)
{

  if(0 < verbose) {
    G4cout << "G4LossTableManager::BuildDEDXTable() for "
           << aParticle->GetParticleName()
           << G4endl;
  }

  Initialise(aParticle);

  if(aParticle == theElectron) {
    if(!electron_table_are_built) {
      eIonisation = BuildTables(theElectron);
      electron_table_are_built = true;
      for(G4int i=0; i<n_loss; i++) {
        G4VEnergyLossSTD* em = loss_vector[i];
        if (tables_are_built[i] && em->SecondaryParticle() == theElectron) {
                em->SetSecondaryRangeTable(eIonisation->RangeTable());
        }
      }
    }

  } else {
    for(G4int i=0; i<n_loss; i++) {

      if(aParticle == part_vector[i]) {
        if(tables_are_built[i]) break;
        if(!base_part_vector[i]) {
          G4VEnergyLossSTD* hIonisation = BuildTables(aParticle);
          for(G4int j=0; j<i; j++) {
            if (aParticle == base_part_vector[j]) {
              tables_are_built[j] = true;
              G4VEnergyLossSTD* em = loss_vector[j];
              em->Initialise();
              em->SetDEDXTable(hIonisation->DEDXTable());
              em->SetRangeTable(hIonisation->RangeTable());
              em->SetInverseRangeTable(hIonisation->InverseRangeTable());
	      em->SetLambdaTable(hIonisation->LambdaTable());
	      em->SetSubLambdaTable(hIonisation->SubLambdaTable());
	      loss_map[part_vector[j]] = em;
              if (em->SecondaryParticle() == theElectron && eIonisation) {
                em->SetSecondaryRangeTable(eIonisation->RangeTable());
              }
              if (0 < verbose) {
                 G4cout << "For " << loss_vector[j]->GetProcessName()
                        << " for " << part_vector[j]->GetParticleName()
                        << " base_part= " << base_part_vector[j]->GetParticleName()
                        << " tables_are_built= " << G4endl;
              }
            }
	  }
        } else {
          G4VEnergyLossSTD* em = loss_vector[i];
          for(G4int j=0; j<n_loss; j++) {
            if (tables_are_built[j] && part_vector[j] == base_part_vector[i]) {
              G4VEnergyLossSTD* hIonisation = loss_vector[j];
              tables_are_built[i] = true;
              em->Initialise();
              em->SetDEDXTable(hIonisation->DEDXTable());
              em->SetRangeTable(hIonisation->RangeTable());
              em->SetInverseRangeTable(hIonisation->InverseRangeTable());
              em->SetLambdaTable(hIonisation->LambdaTable());
              em->SetSubLambdaTable(hIonisation->SubLambdaTable());
              loss_map[part_vector[i]] = em;
              if (em->SecondaryParticle() == theElectron && eIonisation)
                  em->SetSecondaryRangeTable(eIonisation->RangeTable());
              if (0 < verbose) {
                 G4cout << "For " << em->GetProcessName()
                        << " for " << part_vector[i]->GetParticleName()
                        << " base_part= " << base_part_vector[i]->GetParticleName()
                        << " tables_are_built= " << G4endl;
              }
              break;
            }
          }
        }
        break;
      }
    }
  }

  all_tables_are_built = true;
  for (G4int ii=0; ii<n_loss; ii++) {
    if ( !tables_are_built[ii] ) {
      all_tables_are_built = false;
      break;
    }
  }

  if(1 < verbose) {
    G4cout << "G4LossTableManager::BuildDEDXTable: end; "
           << "all_tables_are_built= " << all_tables_are_built
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::RetrievePhysicsTables(const G4ParticleDefinition* aParticle,
                                                     G4VEnergyLossSTD* theLoss)
{
  if(0 < verbose) {
    G4cout << "G4LossTableManager::RetrievePhysicsTable() for "
           << aParticle->GetParticleName()
           << G4endl;
  }
  if (first_entry) Initialise(aParticle);
  if (all_tables_are_built) return;

  G4bool hasRange = false;
  for (G4int i=0; i<n_loss; i++) {
    if ( theLoss == loss_vector[i] ) {
      tables_are_built[i] = true;
      if (theLoss->RangeTable()) {
        if (part_vector[i] == theElectron) {
	  eIonisation = theLoss;
	//  currentLoss = theLoss;
	//  currentParticle = theElectron;
          electron_table_are_built = true;
        }
        loss_map[part_vector[i]] = theLoss;
	hasRange = true;
        if (0 < verbose) {
          G4cout << "dEdx and Range tables are defined for "
                 << aParticle->GetParticleName()
                 << G4endl;
        }
      }
    }
    if (electron_table_are_built && theLoss->SecondaryParticle() == theElectron)
      theLoss->SetSecondaryRangeTable(eIonisation->RangeTable());
  }

  if ( hasRange ) {
    for (G4int j=0; j<n_loss; j++) {
      if ( base_part_vector[j] == aParticle && !tables_are_built[j] ) {
        G4VEnergyLossSTD* em = loss_vector[j];
        tables_are_built[j] = true;
        em->Initialise();
        em->SetDEDXTable(theLoss->DEDXTable());
        em->SetRangeTable(theLoss->RangeTable());
        em->SetInverseRangeTable(theLoss->InverseRangeTable());
        em->SetLambdaTable(theLoss->LambdaTable());
        em->SetSubLambdaTable(theLoss->SubLambdaTable());
        if (electron_table_are_built && em->SecondaryParticle() == theElectron)
           em->SetSecondaryRangeTable(eIonisation->RangeTable());
        loss_map[part_vector[j]] = em;
        if (1 < verbose) {
          G4String nm = "0";
          G4String nm1= "0";
          G4String nm2= "0";
          const G4ParticleDefinition* pd = part_vector[j];
          const G4ParticleDefinition* bpd = base_part_vector[j];
          if (pd) nm = pd->GetParticleName();
          if (bpd) nm2 = bpd->GetParticleName();
          if (part_vector[j]) nm1 = part_vector[j]->GetParticleName();
          G4cout << "For " << loss_vector[j]->GetProcessName()
                 << " for " << nm
                 << " (" << nm1 << ") "
                 << " base_part= " << nm2
                 << " tables_are_built= " << tables_are_built[j]
                 << G4endl;
        }
      }
    }
  }

  all_tables_are_built = true;
  for (G4int ii=0; ii<n_loss; ii++) {
    if ( !tables_are_built[ii] ) {
      all_tables_are_built = false;
      break;
    }
  }

  if(0 < verbose) {
    G4cout << "G4LossTableManager::RetrievePhysicsTable: end; "
           << "all_tables_are_built= " << all_tables_are_built
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VEnergyLossSTD* G4LossTableManager::BuildTables(const G4ParticleDefinition* aParticle)
{
  if(0 < verbose) {
    G4cout << "G4LossTableManager::BuildTables() for "
           << aParticle->GetParticleName() << G4endl;
  }

  // Check is it new particle or all tables have to be rebuilt
  std::vector<G4PhysicsTable*> list;
  list.clear();
  std::vector<G4VEnergyLossSTD*> loss_list;
  loss_list.clear();
  G4VEnergyLossSTD* em = 0;
  G4int iem = 0;
  G4bool no_el = true;

  for (G4int i=0; i<n_loss; i++) {
    if (aParticle == part_vector[i]) {
      loss_vector[i]->Initialise();
      if(no_el && loss_vector[i]->SecondaryParticle() == theElectron) {
        em = loss_vector[i];
        iem= i;
        no_el = false;
      }
      if(!em) {
        em = loss_vector[i];
        iem= i;
      }
      tables_are_built[i] = true;
      list.push_back(loss_vector[i]->BuildDEDXTable());
      loss_list.push_back(loss_vector[i]);
    }
  }

  G4int n_dedx = list.size();
  if (!n_dedx) return 0;

  if (aParticle == theElectron) eIonisation = em;

  if (1 < verbose) {
    G4cout << "G4LossTableManager::BuildTables() start to build range tables"
           << " and the sum of " << n_dedx << " processes"
           << G4endl;
  }

  G4PhysicsTable* dedx = list[0];
  if (1 < n_dedx) {
    dedx = tableBuilder->BuildDEDXTable(list);
    for(G4int i=0; i<n_dedx; i++) {
      list[i]->clearAndDestroy();
    }
  }
  em->SetDEDXTable(dedx);
  dedx_vector[iem] = dedx;
  G4PhysicsTable* range = tableBuilder->BuildRangeTable(dedx);
  G4PhysicsTable* invrange = tableBuilder->BuildInverseRangeTable(dedx, range);
  em->SetInverseRangeTable(invrange);
  inv_range_vector[iem] = invrange;

  if(buildPreciseRange) {
    range->clearAndDestroy();
    std::vector<G4PhysicsTable*> newlist;
    for (G4int i=0; i<n_dedx; i++) {
      newlist.push_back(loss_list[i]->BuildDEDXTableForPreciseRange());
    }
    G4PhysicsTable* dedxForRange = newlist[0];
    if (1 < n_dedx) dedxForRange = tableBuilder->BuildDEDXTable(newlist);
    range = tableBuilder->BuildRangeTable(dedx);
    for(G4int j=0; j<n_dedx; j++) {
      newlist[j]->clearAndDestroy();
    }
  }

  em->SetRangeTable(range);
  range_vector[iem] = range;

  loss_map[aParticle] = em;
  for (G4int j=0; j<n_dedx; j++) {
    if(loss_list[j]->SecondaryParticle() == theElectron && eIonisation)
       loss_list[j]->SetSecondaryRangeTable(eIonisation->RangeTable());

    G4PhysicsTable* lambdaTable = loss_list[j]->BuildLambdaTable();
    loss_list[j]->SetLambdaTable(lambdaTable);
    if (0 < loss_list[j]->NumberOfSubCutoffRegions()) {
      lambdaTable = loss_list[j]->BuildLambdaSubTable();
      loss_list[j]->SetSubLambdaTable(lambdaTable);
    }
  }
  if (0 < verbose) {
    G4cout << "G4LossTableManager::BuildTables: Tables are built for "
           << aParticle->GetParticleName()
	   << "; ionisation process: " << em->GetProcessName()
           << G4endl;
  }
  return em;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetLossFluctuations(G4bool val)
{
  lossFluctuationFlag = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetLossFluctuations(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetSubCutoff(G4bool val)
{
  subCutoffFlag = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetSubCutoff(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetIntegral(G4bool val)
{
  integral = val;
  integralActive = true;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetIntegral(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetMinSubRange(G4double val)
{
  minSubRange = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetMinSubRange(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetRandomStep(G4bool val)
{
  rndmStepFlag = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetRandomStep(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetMinEnergy(G4double val)
{
  minEnergyActive = true;
  minKinEnergy = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetMinKinEnergy(val);
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j]) msc_vector[j]->SetMinKinEnergy(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetMaxEnergy(G4double val)
{
  maxEnergyActive = true;
  maxKinEnergy = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetMaxKinEnergy(val);
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j]) msc_vector[j]->SetMaxKinEnergy(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetStepLimits(G4double v1, G4double v2)
{
  maxRangeVariation = v1;
  maxFinalStep = v2;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetStepFunction(v1, v2);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LossTableManager::SetBuildPreciseRange(G4bool val)
{
  buildPreciseRange = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



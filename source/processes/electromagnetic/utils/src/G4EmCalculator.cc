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
// $Id: G4EmCalculator.cc,v 1.9 2004-11-16 12:05:51 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4EmCalculator
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 28.06.2004
//
// Modifications:
// 12.09.2004 Add verbosity (V.Ivanchenko) 
//
// Class Description:
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4EmCalculator.hh"
#include "G4LossTableManager.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VMultipleScattering.hh"
#include "G4Material.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicsTable.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ProcessManager.hh"
#include "G4ionEffectiveCharge.hh"
#include "G4RegionStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4EmCalculator::G4EmCalculator()
{
  manager = G4LossTableManager::Instance();
  nLocalMaterials    = 0;
  verbose            = 0;
  currentCoupleIndex = 0;
  currentCouple      = 0;
  currentMaterial    = 0;
  currentParticle    = 0;
  currentLambda      = 0;
  chargeSquare       = 1.0;
  massRatio          = 1.0;
  currentParticleName= "";
  currentMaterialName= "";
  ionEffCharge       = new G4ionEffectiveCharge();
  isIon              = false;
  isApplicable       = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4EmCalculator::~G4EmCalculator()
{
  delete ionEffCharge;
  for (G4int i=0; i<nLocalMaterials; i++) {
    delete localCouples[i];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetDEDX(const G4ParticleDefinition* p, const G4Material* mat,
                                       G4double kinEnergy, const G4Region* region)
{
  G4double res = 0.0;
  const G4MaterialCutsCouple* couple = FindCouple(mat, region);
  if(couple && UpdateParticle(p, kinEnergy) ) {
    res = manager->GetDEDX(p, kinEnergy, couple);
    if(verbose>0) {
      G4cout << "E(MeV)= " << kinEnergy/MeV
	     << " DEDX(MeV/mm)= " << res*mm/MeV
	     << " DEDX(MeV*cm^2/g)= " << res*gram/(MeV*cm2*mat->GetDensity())
	     << "  " <<  p->GetParticleName()
	     << " in " <<  mat->GetName()
	     << G4endl;
    }
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetDEDX(const G4String& particle, const G4String& material,
                                       G4double kinEnergy, const G4String& reg)
{
  return GetDEDX(FindParticle(particle),FindMaterial(material),kinEnergy,FindRegion(reg));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetRange(const G4ParticleDefinition* p, const G4Material* mat,
                                        G4double kinEnergy, const G4Region* region)
{
  G4double res = 0.0;
  const G4MaterialCutsCouple* couple = FindCouple(mat,region);
  if(couple && UpdateParticle(p, kinEnergy)) {
    res = manager->GetRange(p, kinEnergy, couple);
    if(verbose>0) {
      G4cout << "E(MeV)= " << kinEnergy/MeV
	     << " range(mm)= " << res/mm
	     << "  " <<  p->GetParticleName()
	     << " in " <<  mat->GetName()
	     << G4endl;
    }
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetRange(const G4String& particle, const G4String& material,
                                        G4double kinEnergy, const G4String& reg)
{
  return GetRange(FindParticle(particle),FindMaterial(material),kinEnergy,FindRegion(reg));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetKinEnergy(const G4ParticleDefinition* p,
                                      const G4Material* mat,
                                            G4double range, const G4Region* region)
{
  G4double res = 0.0;
  const G4MaterialCutsCouple* couple = FindCouple(mat,region);
  if(couple && UpdateParticle(p, 1.0*GeV)) {
    res = manager->GetEnergy(p, range, couple);
    if(verbose>0) {
      G4cout << "Range(mm)= " << range/mm
	     << " KinE(MeV)= " << res/MeV
	     << "  " <<  p->GetParticleName()
	     << " in " <<  mat->GetName()
	     << G4endl;
    }
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetKinEnergy(const G4String& particle, const G4String& material,
                                            G4double range, const G4String& reg)
{
  return GetKinEnergy(FindParticle(particle),FindMaterial(material),range,FindRegion(reg));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetCrossSectionPerMass(
                                         const G4ParticleDefinition* p,
                                         const G4Material* mat,
                                         const G4String& processName, G4double kinEnergy,
					 const G4Region* region)
{
  G4double res = 0.0;
  const G4MaterialCutsCouple* couple = FindCouple(mat,region);

  if(couple) {
    G4int idx = couple->GetIndex();
    FindLambdaTable(p, processName);
    if(currentLambda && UpdateParticle(p, kinEnergy)) {
      G4bool b;
      G4double e = kinEnergy*massRatio;
      res = (((*currentLambda)[idx])->GetValue(e,b))*chargeSquare;
      res /= mat->GetTotNbOfAtomsPerVolume();
      if(verbose>0) {
	G4cout << "E(MeV)= " << kinEnergy/MeV
	       << " cross(barn)= " << res/barn
	       << "  " <<  p->GetParticleName()
	       << " in " <<  mat->GetName()
	       << G4endl;
      }
    }
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetCrossSectionPerMass(
                                         const G4String& particle, const G4String& material,
                                         const G4String& processName, G4double kinEnergy,
					 const G4String& reg)
{
  return GetCrossSectionPerMass(FindParticle(particle),FindMaterial(material),
                                processName,kinEnergy,FindRegion(reg));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetMeanFreePath(const G4ParticleDefinition* p,
                                         const G4Material* mat,
                                         const G4String& processName,
                                               G4double kinEnergy,
                                         const G4Region* region)
{
  G4double res = DBL_MAX;
  G4double x = GetCrossSectionPerMass(p, mat, processName, kinEnergy,region);
  if(x > 0.0) res = 1.0/(x*(mat->GetTotNbOfAtomsPerVolume()));
  if(verbose>1) {
    G4cout << "E(MeV)= " << kinEnergy/MeV
	   << " MFP(mm)= " << res/mm
	   << "  " <<  p->GetParticleName()
	   << " in " <<  mat->GetName()
	   << G4endl;
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::GetMeanFreePath(const G4String& particle, const G4String& material,
                                         const G4String& processName, G4double kinEnergy,
					 const G4String& reg)
{
  return GetMeanFreePath(FindParticle(particle),FindMaterial(material),
                         processName,kinEnergy,FindRegion(reg));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4EmCalculator::PrintDEDXTable(const G4ParticleDefinition* p)
{
  const G4VEnergyLossProcess* elp = FindEnergyLossProcess(p);
  G4cout << "##### DEDX Table for " << p->GetParticleName() << G4endl;
  if(elp) G4cout << *(elp->DEDXTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4EmCalculator::PrintRangeTable(const G4ParticleDefinition* p)
{
  const G4VEnergyLossProcess* elp = FindEnergyLossProcess(p);
  G4cout << "##### Range Table for " << p->GetParticleName() << G4endl;
  if(elp) G4cout << *(elp->RangeTableForLoss()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4EmCalculator::PrintInverseRangeTable(const G4ParticleDefinition* p)
{
  const G4VEnergyLossProcess* elp = FindEnergyLossProcess(p);
  G4cout << "### G4EmCalculator: Inverse Range Table for " << p->GetParticleName() << G4endl;
  if(elp) G4cout << *(elp->InverseRangeTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::ComputeDEDX(const G4ParticleDefinition* p,
                                     const G4Material* mat,
                                     const G4String& processName,
                                           G4double kinEnergy,
                                           G4double cut)
{
  G4double res = 0.0;
  if(verbose > 1) {
    G4cout << "ComputeDEDX: " << p->GetParticleName()
           << " in " << mat->GetName()
           << " e(MeV)= " << kinEnergy/MeV << "  cut(MeV)= " << cut/MeV << G4endl;
  }
  if(FindEmModel(p, processName, kinEnergy)) {
    //    G4cout << "currentModel= " << currentModel << G4endl;
    if(UpdateCouple(mat, cut) && UpdateParticle(p, kinEnergy)) {
      if(baseParticle) {
        G4double e = kinEnergy*massRatio;
        res = currentModel->ComputeDEDX(currentCouple, baseParticle, e, cut);
        if(verbose > 1) {
          G4cout <<  baseParticle->GetParticleName() << " E(MeV)= " << e
                 << " DEDX(MeV/mm)= " << res*mm/MeV
                 << " DEDX(MeV*cm^2/g)= " << res*gram/(MeV*cm2*mat->GetDensity())
                 << G4endl;
	}
        res *= chargeSquare;
      } else {
        res = currentModel->ComputeDEDX(currentCouple, p, kinEnergy, cut);
      }
      if(verbose > 0) {
        G4cout << "E(MeV)= " << kinEnergy/MeV
               << " DEDX(MeV/mm)= " << res*mm/MeV
               << " DEDX(MeV*cm^2/g)= " << res*gram/(MeV*cm2*mat->GetDensity())
               << " cut(MeV)= " << cut/MeV
               << "  " <<  p->GetParticleName()
               << " in " <<  mat->GetName()
               << " Zi^2= " << chargeSquare
               << G4endl;
      }
    }
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::ComputeDEDX(const G4String& particle, const G4String& material,
                                     const G4String& processName, G4double kinEnergy,
                                           G4double cut)
{
  return ComputeDEDX(FindParticle(particle),FindMaterial(material),
                     processName,kinEnergy,cut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::ComputeCrossSectionPerMass(
                                             const G4ParticleDefinition* p,
                                             const G4Material* mat,
                                             const G4String& processName,
                                                   G4double kinEnergy,
                                                   G4double cut)
{
  G4double res = 0.0;
  if(FindEmModel(p, processName, kinEnergy)) {
    if(UpdateCouple(mat, cut) && UpdateParticle(p, kinEnergy)) {
      G4double e = kinEnergy;
      if(baseParticle) {
        e *= kinEnergy*massRatio;
        res = currentModel->CrossSection(currentCouple, baseParticle, e, cut, e)*chargeSquare;
      } else {
        res = currentModel->CrossSection(currentCouple, p, e, cut, e);
      }
    }
    if(verbose>0) {
      G4cout << "E(MeV)= " << kinEnergy/MeV
	     << " cross(barn)= " << res/barn
	     << "  " <<  p->GetParticleName()
	     << " in " <<  mat->GetName()
	     << G4endl;
    }
  }
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::ComputeCrossSectionPerMass(
                                             const G4String& particle, const G4String& material,
                                             const G4String& processName, G4double kinEnergy,
                                                   G4double cut)
{
  return ComputeCrossSectionPerMass(FindParticle(particle),FindMaterial(material),
                                    processName,kinEnergy,cut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::ComputeMeanFreePath(const G4ParticleDefinition* p,
                                             const G4Material* mat,
                                             const G4String& processName,
                                                   G4double kinEnergy,
                                                   G4double cut)
{
  G4double mfp = DBL_MAX;
  G4double x = ComputeCrossSectionPerMass(p, mat, processName, kinEnergy, cut);
  if(x > 0.0) mfp = 1.0/(x*(currentMaterial->GetTotNbOfAtomsPerVolume()));
  if(verbose>1) {
    G4cout << "E(MeV)= " << kinEnergy/MeV
	   << " MFP(mm)= " << mfp/mm
	   << "  " <<  p->GetParticleName()
	   << " in " <<  mat->GetName()
	   << G4endl;
  }
  return mfp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4EmCalculator::ComputeMeanFreePath(const G4String& particle,
                                             const G4String& material,
                                             const G4String& processName, G4double kinEnergy,
                                                   G4double cut)
{
  return ComputeMeanFreePath(FindParticle(particle),FindMaterial(material),
                             processName,kinEnergy,cut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4EmCalculator::UpdateParticle(const G4ParticleDefinition* p, G4double kinEnergy)
{
  if(p != currentParticle) {
    currentParticle = p;
    currentParticleName = p->GetParticleName();
  }
  isApplicable    = false;
  if(p->GetPDGCharge() != 0.0) {
    const G4VEnergyLossProcess* proc = FindEnergyLossProcess(p);
    if(proc) {
      isApplicable    = true;
      baseParticle = proc->BaseParticle();
      if(p->GetParticleType() == "nucleus") {
	isIon = true;
        chargeSquare =
          ionEffCharge->EffectiveChargeSquareRatio(p, currentMaterial, kinEnergy);
      } else {
	isIon = false;
	G4double q = p->GetPDGCharge()/eplus;
	chargeSquare = q*q;
      }
      massRatio = 1.0;
      if(baseParticle) {
        massRatio = baseParticle->GetPDGMass()/p->GetPDGMass();
        G4double q = baseParticle->GetPDGCharge()/eplus;
        chargeSquare /= (q*q);
      }
    }
  }
  return isApplicable;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4ParticleDefinition* G4EmCalculator::FindParticle(const G4String& name)
{
  if(name != currentParticleName) {
    currentParticle = G4ParticleTable::GetParticleTable()->FindParticle(name);
    if(!currentParticle) {
      G4cout << "### WARNING: G4EmCalculator::FindParticle fails to find " << name << G4endl;
    }
    currentParticleName = name;
  }
  return currentParticle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4Material* G4EmCalculator::FindMaterial(const G4String& name)
{
  if(name != currentMaterialName) {
    currentMaterial = G4Material::GetMaterial(name);
    currentMaterialName = name;
    if(!currentMaterial)
      G4cout << "### WARNING: G4EmCalculator::FindMaterial fails to find " << name << G4endl;
  }
  return currentMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4Region* G4EmCalculator::FindRegion(const G4String& reg)
{
  return G4RegionStore::GetInstance()->GetRegion(reg);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4MaterialCutsCouple* G4EmCalculator::FindCouple(const G4Material* material,
                                                       const G4Region* region)
{
  // Access to materials
  const G4ProductionCutsTable* theCoupleTable=
        G4ProductionCutsTable::GetProductionCutsTable();
  const G4Region* r = region;
  if(!r) r = G4RegionStore::GetInstance()->GetRegion("DefaultRegionForTheWorld");

  return theCoupleTable->GetMaterialCutsCouple(material,r->GetProductionCuts());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4EmCalculator::UpdateCouple(const G4Material* material, G4double cut)
{
  if(!material) return false;
  currentMaterial = material;
  currentMaterialName = material->GetName();
  for (G4int i=0; i<nLocalMaterials; i++) {
    if(material == localMaterials[i] && cut == localCuts[i]) {
      currentCouple = localCouples[i];
      currentCoupleIndex = currentCouple->GetIndex();
      currentCut = cut;
      return true;
    }
  }
  const G4MaterialCutsCouple* cc = new G4MaterialCutsCouple(material);
  localMaterials.push_back(material);
  localCouples.push_back(cc);
  localCuts.push_back(cut);
  nLocalMaterials++;
  currentCouple = cc;
  currentCoupleIndex = currentCouple->GetIndex();
  currentCut = cut;
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4EmCalculator::FindLambdaTable(const G4ParticleDefinition* p,
                                     const G4String& processName)
{
  // Search for the process
  if (p != currentParticle || processName != currentName) {
    currentName     = processName;
    currentLambda   = 0;
    G4LossTableManager* manager = G4LossTableManager::Instance();
    const std::vector<G4VEnergyLossProcess*> vel = manager->GetEnergyLossProcessVector();
    G4int n = vel.size();
    for(G4int i=0; i<n; i++) {
      if((vel[i])->GetProcessName() == currentName && (vel[i])->Particle() == p) {
        currentLambda = (vel[i])->LambdaTable();
	isApplicable    = true;
	break;
      }
    }
    if(!currentLambda) {
      const std::vector<G4VEmProcess*> vem = manager->GetEmProcessVector();
      G4int n = vem.size();
      for(G4int i=0; i<n; i++) {
        if((vem[i])->GetProcessName() == currentName && (vem[i])->Particle() == p) {
          currentLambda = (vem[i])->LambdaTable();
          isApplicable    = true;
	  break;
        }
      }
    }
    if(!currentLambda) {
      const std::vector<G4VMultipleScattering*> vmsc = manager->GetMultipleScatteringVector();
      G4int n = vmsc.size();
      for(G4int i=0; i<n; i++) {
        if((vmsc[i])->GetProcessName() == currentName && (vmsc[i])->Particle() == p) {
          currentLambda = (vmsc[i])->LambdaTable();
	  isApplicable    = true;
	  break;
        }
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4EmCalculator::FindEmModel(const G4ParticleDefinition* p,
                                   const G4String& processName,
	  	 		         G4double kinEnergy)
{
  G4bool res = false;
  if(verbose > 1) {
    G4cout << "G4EmCalculator::FindEmModel for " << p->GetParticleName()
           << " and " << processName << " at e(MeV)= " << kinEnergy
           << G4endl;
  }
  // Search for the process
  currentName = processName;
  currentModel = 0;
  size_t idx   = 0;
  G4LossTableManager* manager = G4LossTableManager::Instance();
  const std::vector<G4VEnergyLossProcess*> vel = manager->GetEnergyLossProcessVector();
  G4int n = vel.size();
  for(G4int i=0; i<n; i++) {
    if((vel[i])->GetProcessName() == currentName && (vel[i])->Particle() == p) {
      const G4ParticleDefinition* bp = (vel[i])->BaseParticle();
      //      G4cout << "i= " << i << " bp= " << bp << G4endl;
      if(!bp) {
        currentModel = (vel[i])->SelectModelForMaterial(kinEnergy, idx);
      } else {
        G4double e = kinEnergy*bp->GetPDGMass()/p->GetPDGMass();
        for(G4int j=0; j<n; j++) {
          if((vel[j])->Particle() == bp) {
            currentModel = (vel[j])->SelectModelForMaterial(e, idx);
            break;
	  }
	}
      }
      break;
    }
  }
  if(!currentModel) {
    const std::vector<G4VEmProcess*> vem = manager->GetEmProcessVector();
    G4int n = vem.size();
    for(G4int i=0; i<n; i++) {
      if((vem[i])->GetProcessName() == currentName && (vem[i])->Particle() == p) {
        currentModel = (vem[i])->SelectModelForMaterial(kinEnergy, idx);
	isApplicable    = true;
        break;
      }
    }
  }
  if(!currentModel) {
    const std::vector<G4VMultipleScattering*> vmsc = manager->GetMultipleScatteringVector();
    G4int n = vmsc.size();
    for(G4int i=0; i<n; i++) {
      if((vmsc[i])->GetProcessName() == currentName && (vmsc[i])->Particle() == p) {
        currentModel = (vmsc[i])->SelectModelForMaterial(kinEnergy, idx);
	isApplicable    = true;
        break;
      }
    }
  }
  if(currentModel) res = true;
  return res;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4VEnergyLossProcess* G4EmCalculator::FindEnergyLossProcess(
                      const G4ParticleDefinition* p)
{
  const G4VEnergyLossProcess* elp = 0;
  
  G4LossTableManager* manager = G4LossTableManager::Instance();
  const std::vector<G4VEnergyLossProcess*> vel = manager->GetEnergyLossProcessVector();
  G4int n = vel.size();
  for(G4int i=0; i<n; i++) {
    if((vel[i])->Particle() == p) {
      elp = vel[i];
      break;
    }
  }
  return elp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4EmCalculator::SetVerbose(G4int verb)
{
  verbose = verb;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


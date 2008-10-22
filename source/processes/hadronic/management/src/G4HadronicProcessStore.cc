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
// $Id: G4HadronicProcessStore.cc,v 1.7 2008-10-22 07:58:20 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4HadronicProcessStore
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 09.05.2008
//
// Modifications:
//
//
// Class Description:
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4HadronicProcessStore.hh"
#include "G4Element.hh"
#include "G4ProcessManager.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"

G4HadronicProcessStore* G4HadronicProcessStore::theInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4HadronicProcessStore* G4HadronicProcessStore::Instance()
{
  if(0 == theInstance) {
    static G4HadronicProcessStore manager;
    theInstance = &manager;
  }
  return theInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4HadronicProcessStore::~G4HadronicProcessStore()
{
  /*
  for (G4int i=0; i<n_proc; i++) {
    if( process[i] ) delete process[i];
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4HadronicProcessStore::G4HadronicProcessStore()
{
  n_proc = 0;
  n_part = 0;
  n_model= 0;
  n_extra= 0;
  currentProcess  = 0;
  currentParticle = 0;
  verbose = 1;
  buildTableStart = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetElasticCrossSectionPerVolume(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Material *material)
{
  G4double cross = 0.0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  const G4double* theAtomNumDensityVector = material->GetVecNbOfAtomsPerVolume();
  size_t nelm = material->GetNumberOfElements();
  for (size_t i=0; i<nelm; i++) {
    const G4Element* elm = (*theElementVector)[i];
    cross += theAtomNumDensityVector[i]*
      GetElasticCrossSectionPerAtom(aParticle,kineticEnergy,elm);
  }
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetElasticCrossSectionPerAtom(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Element *anElement)
{
  G4HadronicProcess* hp = FindProcess(aParticle, fHadronElastic);
  localDP.SetKineticEnergy(kineticEnergy);
  G4double cross = 0.0;
  if(hp) cross = hp->GetMicroscopicCrossSection(&localDP,
						anElement,
						STP_Temperature);
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetElasticCrossSectionPerIsotope(
    const G4ParticleDefinition*,
    G4double,
    G4int, G4int)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetInelasticCrossSectionPerVolume(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Material *material)
{
  G4double cross = 0.0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  const G4double* theAtomNumDensityVector = material->GetVecNbOfAtomsPerVolume();
  size_t nelm = material->GetNumberOfElements();
  for (size_t i=0; i<nelm; i++) {
    const G4Element* elm = (*theElementVector)[i];
    cross += theAtomNumDensityVector[i]*
      GetInelasticCrossSectionPerAtom(aParticle,kineticEnergy,elm);
  }
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetInelasticCrossSectionPerAtom(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Element *anElement)
{
  G4HadronicProcess* hp = FindProcess(aParticle, fHadronInelastic);
  localDP.SetDefinition(const_cast<G4ParticleDefinition*>(aParticle));
  localDP.SetKineticEnergy(kineticEnergy);
  G4double cross = 0.0;
  if(hp) cross = hp->GetMicroscopicCrossSection(&localDP,
						anElement,
						STP_Temperature);
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetInelasticCrossSectionPerIsotope(
    const G4ParticleDefinition *,
    G4double,
    G4int, G4int)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetCaptureCrossSectionPerVolume(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Material *material)
{
  G4double cross = 0.0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  const G4double* theAtomNumDensityVector = material->GetVecNbOfAtomsPerVolume();
  size_t nelm = material->GetNumberOfElements();
  for (size_t i=0; i<nelm; i++) {
    const G4Element* elm = (*theElementVector)[i];
    cross += theAtomNumDensityVector[i]*
      GetCaptureCrossSectionPerAtom(aParticle,kineticEnergy,elm);
  }
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetCaptureCrossSectionPerAtom(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Element *anElement)
{
  G4HadronicProcess* hp = FindProcess(aParticle, fCapture);
  localDP.SetDefinition(const_cast<G4ParticleDefinition*>(aParticle));
  localDP.SetKineticEnergy(kineticEnergy);
  G4double cross = 0.0;
  if(hp) cross = hp->GetMicroscopicCrossSection(&localDP,
						anElement,
						STP_Temperature);
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetCaptureCrossSectionPerIsotope(
    const G4ParticleDefinition *,
    G4double,
    G4int, G4int)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetFissionCrossSectionPerVolume(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Material *material)
{
  G4double cross = 0.0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  const G4double* theAtomNumDensityVector = material->GetVecNbOfAtomsPerVolume();
  size_t nelm = material->GetNumberOfElements();
  for (size_t i=0; i<nelm; i++) {
    const G4Element* elm = (*theElementVector)[i];
    cross += theAtomNumDensityVector[i]*
      GetFissionCrossSectionPerAtom(aParticle,kineticEnergy,elm);
  }
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetFissionCrossSectionPerAtom(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Element *anElement)
{
  G4HadronicProcess* hp = FindProcess(aParticle, fFission);
  localDP.SetDefinition(const_cast<G4ParticleDefinition*>(aParticle));
  localDP.SetKineticEnergy(kineticEnergy);
  G4double cross = 0.0;
  if(hp) cross = hp->GetMicroscopicCrossSection(&localDP,
						anElement,
						STP_Temperature);
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetFissionCrossSectionPerIsotope(
    const G4ParticleDefinition *,
    G4double,
    G4int, G4int)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetChargeExchangeCrossSectionPerVolume(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Material *material)
{
  G4double cross = 0.0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  const G4double* theAtomNumDensityVector = material->GetVecNbOfAtomsPerVolume();
  size_t nelm = material->GetNumberOfElements();
  for (size_t i=0; i<nelm; i++) {
    const G4Element* elm = (*theElementVector)[i];
    cross += theAtomNumDensityVector[i]*
      GetChargeExchangeCrossSectionPerAtom(aParticle,kineticEnergy,elm);
  }
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetChargeExchangeCrossSectionPerAtom(
    const G4ParticleDefinition *aParticle,
    G4double kineticEnergy,
    const G4Element *anElement)
{
  G4HadronicProcess* hp = FindProcess(aParticle, fChargeExchange);
  localDP.SetDefinition(const_cast<G4ParticleDefinition*>(aParticle));
  localDP.SetKineticEnergy(kineticEnergy);
  G4double cross = 0.0;
  if(hp) cross = hp->GetMicroscopicCrossSection(&localDP,
						anElement,
						STP_Temperature);
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4double G4HadronicProcessStore::GetChargeExchangeCrossSectionPerIsotope(
    const G4ParticleDefinition *,
    G4double,
    G4int, G4int)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::Register(G4HadronicProcess* proc) 
{ 
  for(G4int i=0; i<n_proc; i++) {if(process[i] == proc) return;}
    
  n_proc++;
  process.push_back(proc);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::RegisterParticle(G4HadronicProcess* proc, 
					      const G4ParticleDefinition* part) 
{ 
  G4int i=0;
  for(; i<n_proc; i++) {if(process[i] == proc) break;}
  G4int j=0;
  for(; j<n_part; j++) {if(particle[j] == part) break;}

  if(j == n_part) {
    n_part++;
    particle.push_back(part);
    wasPrinted.push_back(0);
  }
  
  // the pair should be added?
  if(i < n_proc) {
    std::multimap<PD,HP,std::less<PD> >::iterator it;
    for(it=p_map.lower_bound(part); it!=p_map.upper_bound(part); ++it) {
      if(it->first == part) {
	HP process = (it->second);
	if(proc == process) return;
      }
    }
  }
  
  p_map.insert(std::multimap<PD,HP>::value_type(part,proc));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::RegisterInteraction(G4HadronicProcess* proc,
						 G4HadronicInteraction* mod)
{
  G4int i=0;
  for(; i<n_proc; i++) {if(process[i] == proc) break;}
  G4int k=0;
  for(; k<n_model; k++) {if(model[k] == mod) break;}
   
  m_map.insert(std::multimap<HP,HI>::value_type(proc,mod));
    
  if(k == n_model) {
    n_model++;
    model.push_back(mod);
    modelName.push_back(mod->GetModelName());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::DeRegister(G4HadronicProcess* proc)
{
  for(G4int i=0; i<n_proc; i++) {
    if(process[i] == proc) {
      process[i] = 0;
      break;
    }
  }
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::RegisterExtraProcess(G4VProcess* proc)
{
  for(G4int i=0; i<n_extra; i++) {if(extraProcess[i] == proc) return;}
    
  n_extra++;
  extraProcess.push_back(proc);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::RegisterParticleForExtraProcess(
                             G4VProcess* proc,
			     const G4ParticleDefinition* part)
{
  G4int i=0;
  for(; i<n_extra; i++) {if(extraProcess[i] == proc) break;}
  G4int j=0;
  for(; j<n_part; j++) {if(particle[j] == part) break;}

  if(j == n_part) {
    n_part++;
    particle.push_back(part);
    wasPrinted.push_back(0);
  }
  
  // the pair should be added?
  if(i < n_extra) {
    std::multimap<PD,G4VProcess*,std::less<PD> >::iterator it;
    for(it=ep_map.lower_bound(part); it!=ep_map.upper_bound(part); ++it) {
      if(it->first == part) {
	G4VProcess* process = (it->second);
	if(proc == process) return;
      }
    }
  }
  
  ep_map.insert(std::multimap<PD,G4VProcess*>::value_type(part,proc));
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::DeRegisterExtraProcess(G4VProcess* proc)
{
  for(G4int i=0; i<n_extra; i++) {
    if(extraProcess[i] == proc) {
      extraProcess[i] = 0;
      break;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::PrintInfo(const G4ParticleDefinition* part) 
{
  if(buildTableStart && part == particle[n_part - 1]) {
    buildTableStart = false;
    Dump(verbose);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::Dump(G4int level)
{
  if(level > 0) {
    G4cout << "=============================================================="
	   << "=============================="
	   << G4endl;
      G4cout << "             HADRONIC PROCESSES SUMMARY (verbose level " << level
	     << ")" << G4endl;
  }
  for(G4int i=0; i<n_part; i++) {
    PD part = particle[i];
    G4String pname = part->GetParticleName();
    G4bool yes = false;
    if(level >= 2) yes = true;
    else if(level == 1 && (pname == "proton" || 
			   pname == "neutron" ||
			   pname == "pi+" ||
			   pname == "pi-" ||
                           pname == "gamma" ||
                           pname == "e-" ||
                           pname == "mu-" ||
			   pname == "kaon+" ||
			   pname == "kaon-" ||
			   pname == "lambda" ||
			   pname == "anti_neutron" ||
			   pname == "anti_proton")) yes = true;
    if(yes) {
      // main processes
      std::multimap<PD,HP,std::less<PD> >::iterator it;
      for(it=p_map.lower_bound(part); it!=p_map.upper_bound(part); ++it) {
	if(it->first == part) {
	  HP proc = (it->second);
	  G4int j=0;
	  for(; j<n_proc; j++) {
	    if(process[j] == proc) {
	      Print(j, i);
	    }
	  }
	}
      }
      // extra processes
      std::multimap<PD,G4VProcess*,std::less<PD> >::iterator itp;
      for(itp=ep_map.lower_bound(part); itp!=ep_map.upper_bound(part); ++itp) {
	if(itp->first == part) {
	  G4VProcess* proc = (itp->second);
	  if(wasPrinted[i] == 0) {
	    wasPrinted[i] = 1;
	    G4cout<<G4endl;
	    G4cout << "                     Hadronic Processes for <" 
		   <<part->GetParticleName() << ">" << G4endl; 
	  }
	  G4cout << "          " << proc->GetProcessName() << G4endl;
	}
      }
    }
  }
  if(level > 0) {
    G4cout << "=============================================================="
	   << "=============================="
	   << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::Print(G4int idxProc, G4int idxPart)
{
  G4HadronicProcess* proc = process[idxProc];
  const G4ParticleDefinition* part = particle[idxPart];
  if(wasPrinted[idxPart] == 0) {
    wasPrinted[idxPart] = 1;
    G4cout<<G4endl;
    G4cout << "                     Hadronic Processes for <" 
	   <<part->GetParticleName() << ">" << G4endl; 
  }
  HI hi = 0;
  G4bool first;
  std::multimap<HP,HI,std::less<HP> >::iterator ih;
  G4cout << std::setw(20) << proc->GetProcessName()  
	 << "  Models: ";
  first = true;
  for(ih=m_map.lower_bound(proc); ih!=m_map.upper_bound(proc); ++ih) {
    if(ih->first == proc) {
      hi = ih->second;
      G4int i=0;
      for(; i<n_model; i++) {
	if(model[i] == hi) break;
      }
      if(!first) G4cout << "                              ";
      first = false;
      G4cout << std::setw(25) << modelName[i] 
	     << ": Emin(GeV)= "  
	     << std::setw(5) << hi->GetMinEnergy()/GeV
	     << "  Emax(GeV)= " 
	     << hi->GetMaxEnergy()/GeV
	     << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4HadronicProcessStore::SetVerbose(G4int val)
{
  verbose = val;
  G4int i;
  for(i=0; i<n_proc; i++) {
    if(process[i]) process[i]->SetVerboseLevel(val);
  }
  for(i=0; i<n_model; i++) {
    if(model[i]) model[i]->SetVerboseLevel(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4int G4HadronicProcessStore::GetVerbose()
{
  return verbose;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4HadronicProcess* G4HadronicProcessStore::FindProcess(
   const G4ParticleDefinition* part, G4HadronicProcessType subType)
{
  bool isNew = false;
  G4HadronicProcess* hp = 0;

  if(part != currentParticle) {
    isNew = true;
    currentParticle = part;
    localDP.SetDefinition(const_cast<G4ParticleDefinition*>(part));
  } else if(!currentProcess) {
    isNew = true;
  } else if(subType == currentProcess->GetProcessSubType()) {
    hp = currentProcess;
  } else {
    isNew = true;
  }

  if(isNew) {
    std::multimap<PD,HP,std::less<PD> >::iterator it;
    for(it=p_map.lower_bound(part); it!=p_map.upper_bound(part); ++it) {
      if(it->first == part && subType == (it->second)->GetProcessSubType()) {
	hp = it->second;
	break;
      }
    }  
    currentProcess = hp;
  }

  return hp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

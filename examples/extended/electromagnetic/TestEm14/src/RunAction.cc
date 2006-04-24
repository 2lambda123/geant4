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
// $Id: RunAction.cc,v 1.2 2006-04-24 15:42:53 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "G4Gamma.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim,
                     HistoManager* histo)
  : detector(det), primary(prim), ProcCounter(0), histoManager(histo)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  // save Rndm status
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  CLHEP::HepRandom::showEngineStatus();

  ProcCounter = new ProcessesCount;
  totalCount = 0;
  sumTrack = sumTrack2 = 0.;
  
  histoManager->book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::CountProcesses(G4String procName)
{
   //does the process  already encounted ?
   size_t nbProc = ProcCounter->size();
   size_t i = 0;
   while ((i<nbProc)&&((*ProcCounter)[i]->GetName()!=procName)) i++;
   if (i == nbProc) ProcCounter->push_back( new OneProcessCount(procName));

   (*ProcCounter)[i]->Count();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  G4int  prec = G4cout.precision(5);
    
  G4Material* material = detector->GetMaterial();
  G4double density = material->GetDensity();
  G4int survive = 0;
   
  G4ParticleDefinition* particle = 
                            primary->GetParticleGun()->GetParticleDefinition();
  G4String Particle = particle->GetParticleName();    
  G4double energy = primary->GetParticleGun()->GetParticleEnergy();
  G4cout << "\n The run consists of " << NbOfEvents << " "<< Particle << " of "
         << G4BestUnit(energy,"Energy") << " through " 
	 << G4BestUnit(detector->GetSize(),"Length") << " of "
	 << material->GetName() << " (density: " 
	 << G4BestUnit(density,"Volumic Mass") << ")" << G4endl;
  
  //frequency of processes
  G4cout << "\n Process calls frequency --->";
  for (size_t i=0; i< ProcCounter->size();i++) {
     G4String procName = (*ProcCounter)[i]->GetName();
     G4int    count    = (*ProcCounter)[i]->GetCounter(); 
     G4cout << "\t" << procName << " = " << count;
     if (procName == "Transportation") survive = count;
  }
  
  if (survive > 0) {
    G4cout << "\n\n Nb of incident particles surviving after "
           << G4BestUnit(detector->GetSize(),"Length") << " of "
	   << material->GetName() << " : " << survive << G4endl;
  }
  
  if (totalCount == 0) totalCount = 1;   //force printing anyway
  
  //compute mean free path and related quantities
  //
  G4double MeanFreePath = sumTrack /totalCount;     
  G4double MeanTrack2   = sumTrack2/totalCount;     
  G4double rms = std::sqrt(std::fabs(MeanTrack2 - MeanFreePath*MeanFreePath));
  G4double CrossSection = 1./MeanFreePath;     
  G4double massicMFP = MeanFreePath*density;
  G4double massicCS  = 1./massicMFP;
   
  G4cout << "\n\n MeanFreePath:\t"   << G4BestUnit(MeanFreePath,"Length")
         << " +- "                   << G4BestUnit( rms,"Length")
         << "\tmassic: "             << G4BestUnit(massicMFP, "Mass/Surface")
         << "\n CrossSection:\t"     << CrossSection*cm << " cm^-1 "
	 << "\t\t\tmassic: "         << G4BestUnit(massicCS, "Surface/Mass")
         << G4endl;

  //check cross section from G4EmCalculator
  //
  G4cout << "\n Verification : "
         << "crossSections from G4EmCalculator. \n";
  
  G4EmCalculator emCalculator;
  G4double sumc = 0.0;  
  for (size_t i=0; i< ProcCounter->size();i++) {
    G4String procName = (*ProcCounter)[i]->GetName();
    G4double massSigma = 
    emCalculator.GetCrossSectionPerVolume(energy,particle,
                                              procName,material)/density;
    if (particle == G4Gamma::Gamma())
       massSigma = 
       emCalculator.ComputeCrossSectionPerVolume(energy,particle,
                                              procName,material)/density;					      
    sumc += massSigma;
    G4cout << "\t" << procName << "= " 
           << G4BestUnit(massSigma, "Surface/Mass");
  }  	   
  G4cout << "\ttotal= " 
         << G4BestUnit(sumc, "Surface/Mass") << G4endl;
	 	 
  //restore default format	 
  G4cout.precision(prec);         

  // delete and remove all contents in ProcCounter 
  while (ProcCounter->size()>0){
    OneProcessCount* aProcCount=ProcCounter->back();
    ProcCounter->pop_back();
    delete aProcCount;
  }
  delete ProcCounter;
  
  histoManager->save();

  // show Rndm status
  CLHEP::HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

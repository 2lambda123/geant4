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
// $Id: RunAction.cc,v 1.5 2004-11-03 12:43:02 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "MuCrossSections.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"

#ifdef G4ANALYSIS_USE
 #include "AIDA/AIDA.h"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim,
                     HistoManager* HistM)
  : detector(det), primary(prim), ProcCounter(0), histoManager(HistM)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  // save Rndm status
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  HepRandom::showEngineStatus();

  ProcCounter = new ProcessesCount;
  
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
  
  std::ios::fmtflags mode = G4cout.flags();
  G4int  prec = G4cout.precision(2);
    
  G4Material* material = detector->GetMaterial();
  G4double length  = detector->GetSize();
  G4double density = material->GetDensity();
   
  G4String particle = primary->GetParticleGun()->GetParticleDefinition()
                      ->GetParticleName();    
  G4double energy = primary->GetParticleGun()->GetParticleEnergy();
  
  G4cout << "\n The run consists of " << NbOfEvents << " "<< particle << " of "
         << G4BestUnit(energy,"Energy") << " through " 
	 << G4BestUnit(length,"Length") << " of "
	 << material->GetName() << " (density: " 
	 << G4BestUnit(density,"Volumic Mass") << ")" << G4endl;
  
  //total number of process calls
  G4double countTot = 0.;
  G4cout << "\n Number of process calls --->";
  for (size_t i=0; i< ProcCounter->size();i++) {
     G4String procName = (*ProcCounter)[i]->GetName();
     if (procName != "Transportation") {
       G4int count    = (*ProcCounter)[i]->GetCounter(); 
       G4cout << "\t" << procName << " : " << count;
       countTot += count;
     }
  }
  G4cout << G4endl;
  
  //compute totalCrossSection, meanFreePath and massicCrossSection
  //
  G4double totalCrossSection = countTot/(NbOfEvents*length);
  G4double MeanFreePath      = 1./totalCrossSection;        
  G4double massCrossSection  =totalCrossSection/density;     
   
  G4cout.precision(5);
  G4cout << "\n Simulation: "
         <<    "total CrossSection = " << totalCrossSection*cm << " /cm"
         << "\t MeanFreePath = "       << G4BestUnit(MeanFreePath,"Length")
         << "\t massicCrossSection = " << massCrossSection*g/cm2 << " cm2/g"
         << G4endl;
  
  //compute theoritical predictions
  //
  totalCrossSection = 0.;
  for (size_t i=0; i< ProcCounter->size();i++) {
     G4String procName = (*ProcCounter)[i]->GetName();
     if (procName != "Transportation")
       totalCrossSection += ComputeTheory(procName, NbOfEvents);
  }
  
  MeanFreePath     = 1./totalCrossSection;
  massCrossSection = totalCrossSection/density;
  
  G4cout << " Theory:     "
         <<    "total CrossSection = " << totalCrossSection*cm << " /cm"
         << "\t MeanFreePath = "       << G4BestUnit(MeanFreePath,"Length")
         << "\t massicCrossSection = " << massCrossSection*g/cm2 << " cm2/g"
         << G4endl;
	 	 
	 	 	               	                           
  G4cout.setf(mode,std::ios::floatfield);
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
  HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double RunAction::ComputeTheory(G4String process, G4int NbOfMu)
{   
  G4Material* material = detector->GetMaterial();
  G4double length = detector->GetSize();
  G4double ekin = primary->GetParticleGun()->GetParticleEnergy();
  MuCrossSections crossSections;

  G4int id = 0; G4double cut = 0.;
  if (process == "muIoni")     {id = 1; cut =    GetEnergyCut(material,1); }
  if (process == "muPairProd") {id = 2; cut = 2*(GetEnergyCut(material,1) 
                                                      + electron_mass_c2); }
  if (process == "muBrems")    {id = 3; cut =    GetEnergyCut(material,0); }
  if (process == "muNucl")      id = 4;
  if (id == 0) return 0.;
  
  G4int nbOfBins = 100;
  G4double binMin = -10., binMax = 0., binWidth = (binMax-binMin)/nbOfBins;
    
#ifdef G4ANALYSIS_USE
  //create histo for theoritical crossSections, with same bining as simulation
  //
  const G4String label[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19"};
		      
  AIDA::IHistogram1D* histoMC = 0; AIDA::IHistogram1D* histoTh = 0;  
  if (histoManager->HistoExist(id)) {
    histoMC  = histoManager->GetHisto(id);  
    nbOfBins = histoManager->GetNbins(id);
    binMin   = histoManager->GetVmin (id);
    binMax   = histoManager->GetVmax (id);
    binWidth = histoManager->GetBinWidth(id);
    
    G4String labelTh = label[MaxHisto + id];
    G4String titleTh = histoManager->GetTitle(id) + " (Th)";
    histoTh = histoManager->GetHistogramFactory()
          ->createHistogram1D(labelTh,titleTh,nbOfBins,binMin,binMax);    
  }
#endif
  
  //compute and plot differential crossSection, as function of energy transfert.
  //compute and return integrated crossSection for a given process.
  //(note: to compare with simulation, the integrated crossSection is function
  //       of the energy cut.) 
  // 
  G4double lgeps, etransf, sigmaE, dsigma, NbProcess;
  G4double sigmaTot = 0.;
  const G4double ln10 = log(10.);
    
  for (G4int ibin=0; ibin<nbOfBins; ibin++) {
    lgeps = binMin + (ibin+0.5)*binWidth;
    etransf = ekin*pow(10,lgeps);
    sigmaE = crossSections.CR_Macroscopic(process,material,ekin,etransf);
    dsigma = sigmaE*etransf*binWidth*ln10;
    if (etransf > cut) sigmaTot += dsigma;    
    NbProcess = NbOfMu*length*dsigma;
#ifdef G4ANALYSIS_USE
    if (histoTh) histoTh->fill(lgeps,NbProcess);
#endif     
  }
  
#ifdef G4ANALYSIS_USE 
  //compare simulation and theory
  //
  if (histoMC && histoTh) histoManager->GetHistogramFactory()
                     ->divide(label[2*MaxHisto+id], *histoMC, *histoTh);
#endif
   
  //return integrated crossSection
  //
  return sigmaTot;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ProductionCutsTable.hh"

G4double RunAction::GetEnergyCut(G4Material* material, G4int idParticle)
{ 
 G4ProductionCutsTable* table = G4ProductionCutsTable::GetProductionCutsTable();
 
 size_t index = 0;
 while ( (table->GetMaterialCutsCouple(index)->GetMaterial() != material) &&
        (index < table->GetTableSize())) index++;

 return (*(table->GetEnergyCutsVector(idParticle)))[index];
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    	       

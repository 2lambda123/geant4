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
// -------------------------------------------------------------------
// $Id: RunAction.cc,v 1.1 2009-05-04 10:05:35 sincerti Exp $
// -------------------------------------------------------------------

#include "RunAction.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
:Detector(det), Primary(prim), ProcCounter(0)
{  
  ProcCounter = new ProcessesCount;
  totalCount = 0;
  eTransfer = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::BeginOfRunAction(const G4Run*)
{  
  eTransfer = 0.;
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  G4int  prec = G4cout.precision(5);
    
  G4Material* material = Detector->GetMaterial();
  G4double density = material->GetDensity();
  G4int survive = 0;
   
  G4ParticleDefinition* particle = 
                            Primary->GetParticleGun()->GetParticleDefinition();
  G4String Particle = particle->GetParticleName();    
  
  G4double energy = Primary->GetParticleGun()->GetParticleEnergy();
  G4cout << "\n The run consists of " << NbOfEvents << " "<< Particle << " of "
         << G4BestUnit(energy,"Energy") << " through " 
	 << G4BestUnit(Detector->GetSize(),"Length") << " of "
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
           << G4BestUnit(Detector->GetSize(),"Length") << " of "
	   << material->GetName() << " : " << survive << G4endl;
  }
  
  if (totalCount == 0) totalCount = 1;   //force printing anyway
 
  G4cout << G4endl;
  G4cout << " Total transfered energy (keV)=" <<eTransfer/keV<< G4endl;
  G4cout << G4endl;

/*
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
	 
  //compute energy transfer coefficient
  //
  G4double MeanTransfer   = eTransfer/totalCount;
  G4double massTransfCoef = massicCS*MeanTransfer/energy;
   
  G4cout << "\n mean energy of charged secondaries: " << G4BestUnit(MeanTransfer, "Energy")
	 << "\tmass_energy_transfer coef: "           << G4BestUnit(massTransfCoef, "Surface/Mass")
         << G4endl;       
 
  //check cross section from G4EmCalculator
  //
  G4cout << "\n Verification : "
         << "crossSections from G4EmCalculator \n";
  
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

*/	 	 
  //restore default format	 
  G4cout.precision(prec);         

  // delete and remove all contents in ProcCounter 
  while (ProcCounter->size()>0){
    OneProcessCount* aProcCount=ProcCounter->back();
    ProcCounter->pop_back();
    delete aProcCount;
  }
//  delete ProcCounter;
  
}

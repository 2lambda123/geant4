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
// $Id: HistoManager.cc,v 1.5 2005-06-01 13:58:22 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "HistoMessenger.hh"
#include "G4UnitsTable.hh"

#ifdef G4ANALYSIS_USE
#include "AIDA/AIDA.h"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
:af(0),tree(0),hf(0),factoryOn(false)
{
#ifdef G4ANALYSIS_USE
  // Creating the analysis factory
  af = AIDA_createAnalysisFactory();
  if(!af) {
    G4cout << " HistoManager::HistoManager() :" 
           << " problem creating the AIDA analysis factory."
           << G4endl;
  }    
#endif 
 
  fileName[0] = "muonprocesses";
  fileType    = "hbook";
  fileOption  = "--noErrors uncompress";    
  // histograms
  for (G4int k=0; k<MaxHisto; k++) {
    histo[k] = 0;
    exist[k] = false;
    Unit[k]  = 1.0;
    Width[k] = 1.0;
  }

  histoMessenger = new HistoMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete histoMessenger;
  
#ifdef G4ANALYSIS_USE  
  delete af;
#endif    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{
#ifdef G4ANALYSIS_USE
  if(!af) return;
    
  // Creating a tree mapped to an hbook file.
  fileName[1] = fileName[0] + "." + fileType;    
  G4bool readOnly  = false;
  G4bool createNew = true;
  AIDA::ITreeFactory* tf  = af->createTreeFactory();
  tree = tf->create(fileName[1], fileType, readOnly, createNew, fileOption);
  delete tf;
  if(!tree) {
    G4cout << "HistoManager::book() :" 
           << " problem creating the AIDA tree with "
           << " storeName = " << fileName[1]
           << " storeType = " << fileType
           << " readOnly = "  << readOnly
           << " createNew = " << createNew
           << " options = "   << fileOption
           << G4endl;
    return;
  }

  // Creating a histogram factory, whose histograms will be handled by the tree
  hf = af->createHistogramFactory(*tree);

  // create selected histograms
  for (G4int k=0; k<MaxHisto; k++) {
    if (exist[k]) {
      histo[k] = hf->createHistogram1D( Label[k], Title[k],
                                                  Nbins[k], Vmin[k], Vmax[k]);
      factoryOn = true;
    }
  }
  if(factoryOn) 
      G4cout << "\n----> Histogram Tree is opened in " << fileName[1]  << G4endl;

#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{
#ifdef G4ANALYSIS_USE
  if (factoryOn) {
    tree->commit();       // Writing the histograms to the file
    tree->close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved in " << fileName[1] << G4endl;

    delete hf;
    delete tree;
    tree = 0;
    factoryOn = false;
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double e, G4double weight)
{
  if (ih > MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << "does not exist; e= " << e << " w= " << weight << G4endl;
    return;
  }
#ifdef G4ANALYSIS_USE
  if(exist[ih]) histo[ih]->fill(e/Unit[ih], weight);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::SetHisto(G4int ih,
                 G4int nbins, G4double valmin, G4double valmax, const G4String& unit)
{
  if (ih > MaxHisto) {
    G4cout << "---> warning from HistoManager::SetHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }
  
  const G4String id[] = { "0", "1", "2", "3", "4"};
  const G4String title[] = 
                { "dummy",						//0
                  "log10(Etransfert/Emu) Ionization",			//1
                  "log10(Etransfert/Emu) Pair",				//2
                  "log10(Etransfert/Emu) Brems",			//3
                  "log10(Etransfert/Emu) Nuclear"			//4
                 };

  G4String titl = title[ih];
  G4double vmin = valmin, vmax = valmax;
  Unit[ih] = 1.;

  if (unit != "none") {
    titl = title[ih] + " (" + unit + ")";
    Unit[ih] = G4UnitDefinition::GetValueOf(unit);
    vmin = valmin/Unit[ih]; vmax = valmax/Unit[ih];
  }

  exist[ih] = true;
  Label[ih] = id[ih];
  Title[ih] = titl;
  Nbins[ih] = nbins;
  Vmin[ih]  = vmin;
  Vmax[ih]  = vmax;
  Width[ih] = (valmax-valmin)/nbins;

  G4cout << "----> SetHisto " << ih << ": " << titl << ";  "
         << nbins << " bins from "
         << vmin << " " << unit << " to " << vmax << " " << unit << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::RemoveHisto(G4int ih)
{
 if (ih > MaxHisto) {
    G4cout << "---> warning from HistoManager::RemoveHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }

  histo[ih] = 0;  exist[ih] = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


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
// $Id: HistoManager.cc,v 1.8 2004-06-18 09:47:49 vnivanch Exp $
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
:tree(0),hf(0),factoryOn(false)
{
  fileName = "testem5.paw";
  fileType = "hbook";
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{
#ifdef G4ANALYSIS_USE
  // Creating the analysis factory
  AIDA::IAnalysisFactory* af = AIDA_createAnalysisFactory();

  // Creating the tree factory
  AIDA::ITreeFactory* tf = af->createTreeFactory();

  // Creating a tree mapped to an hbook file.
  G4bool readOnly  = false;
  G4bool createNew = true;
  tree = tf->create(fileName, fileType, readOnly, createNew);

  // Creating a histogram factory, whose histograms will be handled by the tree
  hf = af->createHistogramFactory(*tree);

  // create selected histograms
  for (G4int k=0; k<MaxHisto; k++) {
    if (exist[k]) {
      histo[k] = hf->createHistogram1D( Label[k],Title[k],Nbins[k], Vmin[k], Vmax[k]);
      factoryOn = true;
    }
  }
  if(factoryOn) 
      G4cout << "----> Histogram Tree is opened " << G4endl;

  delete tf;
  delete af;
#endif
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{
#ifdef G4ANALYSIS_USE
  if (factoryOn) {
    tree->commit();       // Writing the histograms to the file
    tree->close();        // and closing the tree (and the file)
    G4cout << "----> Histogram Tree is saved in " << fileName << G4endl;

    delete hf;
    delete tree;
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
  if(exist[ih]) histo[ih]->fill(e/Unit[ih],weight);
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

  const G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                         "10","11","12","13","14","15","16"};
  const G4String title[] =
                { "dummy",						//0
                  "energy deposit in absorber",				//1
                  "energy of charged secondaries at creation",		//2
                  "energy of gammas at creation (log10(ekin/MeV))",	//3
		  "(transmit, charged) : kinetic energy at exit",	//4
		  "(transmit, charged) : space angle at exit",		//5
		  "(transmit, charged) : projected angle at exit",	//6
		  "(transmit, charged) : projected position at exit",	//7
		  "(transmit, neutral) : kinetic energy at exit",	//8
		  "(transmit, neutral) : space angle at exit",		//9
		  "(transmit, neutral) : projected angle at exit",	//10
		  "(reflect , charged) : kinetic energy at exit",	//11
		  "(reflect , charged) : space angle at exit",		//12
		  "(reflect , charged) : projected angle at exit",	//13
		  "(reflect , neutral) : kinetic energy at exit",	//14
		  "(reflect , neutral) : space angle at exit",		//15
		  "(reflect , neutral) : projected angle at exit",	//16
                 };

  G4String titl = title[ih];
  G4double vmin = valmin, vmax = valmax;
  Unit[ih] = 1.;

  if (ih == 3) { vmin = log10(valmin/MeV); vmax = log10(valmax/MeV);}
  else if (unit != "none") {
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

  histo[ih] = 0;
  exist[ih] = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


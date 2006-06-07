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
// $Id: Histo.hh,v 1.2 2006-06-07 15:17:26 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef Histo_h
#define Histo_h 1

//---------------------------------------------------------------------------
//
// ClassName:   Histo
//
// Description: Utility class to hold and manipulate histograms/nTuples
//
// Author:      V.Ivanchenko 30/10/03
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include <vector>
#include "G4DynamicParticle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4DataVector.hh"
#include "G4Track.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class HistoMessenger;

namespace AIDA {
 class ITree;
 class ITuple;
 class IHistogram1D;
}

class Histo
{

public:
  Histo(G4int ver);

  ~Histo();

  void book();
  // Book predefined histogramms 

  void save();
  // Save histogramms to file

  void reset();
  // Reset histogramms 

  void add1D(const G4String&, const G4String&, G4int nb=100, G4double x1=0., 
                                               G4double x2=1., G4double u=1.);
  // In this method histogramms are predefined

  void setHisto1D(G4int, G4int, G4double, G4double, G4double);
  // It change bins and boundaries

  void fill(G4int, G4double, G4double);
  // Histogramms are filled

  void scale(G4int, G4double);

  void addTuple(const G4String&, const G4String&, const G4String&);
  // In this method nTuple is booked

  void fillTuple(const G4String&, G4double);
  // Fill nTuple parameter

  void addRow();
  // Save tuple event 

  void setFileName(const G4String& nam) {histName = nam;};

  void setFileOption(const G4String& nam) {option = nam;};

  void setFileType(const G4String& nam);

  void print(G4int i);

  void setVerbose(G4int val) {verbose = val;};

private:

  G4String histName;
  G4String histType;
  G4String option;
  G4String tupleName;
  G4String tupleId;
  G4String tupleList;
  G4int    nHisto;
  G4int    verbose;
  G4int    defaultAct;

  std::vector<AIDA::IHistogram1D*> histo;
  AIDA::ITuple*   ntup;
  AIDA::ITree*    tree;
  HistoMessenger* messenger;
  std::vector<G4int>     active;
  std::vector<G4int>     bins;
  std::vector<G4double>  xmin;
  std::vector<G4double>  xmax;
  std::vector<G4double>  unit;
  std::vector<G4String>  ids;
  std::vector<G4String>  tittles;
};

#endif

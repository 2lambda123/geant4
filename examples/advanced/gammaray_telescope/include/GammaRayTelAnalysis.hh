#ifdef G4ANALYSIS_USE
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
// $Id: GammaRayTelAnalysis.hh,v 1.13 2003-05-29 14:34:26 flongo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// ------------------------------------------------------------
//      GEANT 4 class header file
//      CERN Geneva Switzerland
//     
//
//      ------------ GammaRayTelAnalysis  ------
//           by R.Giannitrapani, F. Longo & G.Santin (30 nov 2000)
//
// 07.12.2001 A.Pfeiffer
// - integrated Guy's addition of the ntuple
//
// 06.12.2001 A.Pfeiffer
// - updating to new design (singleton)
//
// 22.11.2001 G.Barrand
// - Adaptation to AIDA
//
// ************************************************************

#ifndef GammaRayTelAnalysis_h 
#define GammaRayTelAnalysis_h 1

#include "globals.hh"
#include "g4std/vector"
#include "G4ThreeVector.hh"

#include <AIDA/AIDA.h>

using namespace AIDA;

class GammaRayTelAnalysisMessenger;
class GammaRayTelDetectorConstruction;

class AIDA::IAnalysisFactory;
class AIDA::ITree;
class AIDA::IHistogramFactory;
class AIDA::ITupleFactory;
class AIDA::IPlotter;
class AIDA::IHistogram1D;
class AIDA::IHistogram2D;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class GammaRayTelAnalysis {
public:
  virtual ~GammaRayTelAnalysis();
  
public:
  void BeginOfRun(G4int n);
  void EndOfRun(G4int n);
  void EndOfEvent(G4int flag);

  void Init();
  void Finish();

  void SetHisto1DDraw(G4String str) {histo1DDraw = str;};
  void SetHisto1DSave(G4String str) {histo1DSave = str;};
  void SetHisto2DDraw(G4String str) {histo2DDraw = str;};
  void SetHisto2DSave(G4String str) {histo2DSave = str;};
  void SetHisto2DMode(G4String str) {histo2DMode = str;};

  G4String GetHisto2DMode() {return histo2DMode;};

  void InsertPositionXZ(double x, double z);
  void InsertPositionYZ(double y, double z);
  void InsertEnergy(double en);
  void InsertHits(int nplane);

  void setNtuple(float E, float p, float x, float y, float z);

  static GammaRayTelAnalysis* getInstance(int = 0, char** = 0);

private:
  GammaRayTelAnalysis(int = 0, char** = 0);

  void plot1D(IHistogram1D* histo);
  void plot2D(IHistogram2D* histo);
  void Plot();

private:
  static GammaRayTelAnalysis* instance;

  GammaRayTelDetectorConstruction*    GammaRayTelDetector;

  IAnalysisFactory* analysisFactory;
  ITree* tree;
  IPlotter* plotter;
  ITuple* tuple;

  IHistogram1D* energy;
  IHistogram1D* hits;
  IHistogram2D* posXZ;
  IHistogram2D* posYZ;

  G4String histo1DDraw;
  G4String histo1DSave;
  G4String histo2DDraw;
  G4String histo2DSave;
  G4String histo2DMode;

  GammaRayTelAnalysisMessenger* analysisMessenger;
};


#endif
#endif

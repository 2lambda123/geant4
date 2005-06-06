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
#ifndef test31Histo_h
#define test31Histo_h 1

//---------------------------------------------------------------------------
//
// ClassName:   test31Histo
//
// Description: Singleton class to hold Emc geometry parameters.
//              User cannot access to the constructor. 
//              The pointer of the only existing object can be got via 
//              test31Histo::GetPointer() static method. 
//              The first invokation of this static method makes 
//              the singleton object.
//
// Author:      V.Ivanchenko 27/09/00
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include "G4DynamicParticle.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Histo;
class EmAnalysis;

class test31Histo
{

public:
  // With description

  static test31Histo* GetPointer();

  test31Histo();

  ~test31Histo();
 
  void BeginOfHisto(G4int);
  // In this method histogramms are booked

  void EndOfHisto();
  // In this method bookHisto method is called in which histogramms are filled

public: // Without description

  void SetHistoName(const G4String& name) {histName = name;};
  void bookHisto();
  void SaveToTuple(const G4String&, G4double);
  void SaveToTuple(const G4String&, G4double, G4double);
  void SaveEvent();
  G4double GetTrackLength() const {return trackLength;};
  void ResetTrackLength() {trackLength = 0.0, trackAbs = true;};
  void SetTrackOutAbsorber() {trackAbs = false;};
  G4bool GetTrackInAbsorber() const {return trackAbs;};
  void AddTrackLength(G4double x)   {trackLength += x;};
  void AddEndPoint(G4double);
  void AddEnergy(G4double, G4double);
  void AddDeltaElectron(const G4DynamicParticle* dp);
  void AddPhoton(const G4DynamicParticle* dp);
  void AddParticleLeak(const G4DynamicParticle* dp);
  void AddParticleBack(const G4DynamicParticle* dp);
  void AddPositron(const G4DynamicParticle*) {n_posit++;};
  void SetVerbose(G4int val) {verbose = val;};
  G4int GetVerbose() const {return verbose;};
  void SetHistoNumber(G4int val) {nHisto = val;};
  void SetNtuple(G4bool val) {nTuple = val;};

  void SetNumberOfAbsorbers(G4int val) {NumberOfAbsorbers = val;};     
  G4int GetNumberOfAbsorbers() const {return NumberOfAbsorbers;};
  void SetAbsorberThickness(G4double val) {AbsorberThickness = val;};     
  G4double  GetAbsorberThickness() const {return AbsorberThickness;};
  void SetGap(G4double val) {gap = val;};     
  G4double  GetGap() const {return gap;};
  void SetNumAbsorbersSaved(G4int val) {nAbsSaved = val;};
  G4int GetNumAbsorbersSaved() const {return nAbsSaved;};
  void SetMaxEnergy(G4double val) {maxEnergy = val;};     
  G4double  GetMaxEnergy() const {return maxEnergy;};
  void AddEvent() {n_evt++;};     
  void AddStep() {n_step++;};     

  void CountProcess(const G4String&);

private:

  void TableControl();
  void MuonTest();

  test31Histo(const test31Histo&);
  const test31Histo& operator=(const test31Histo& right);

  // MEMBERS
  static test31Histo* fManager;

  EmAnalysis* ema;

  G4String histName;
  G4String theName;
  Histo* histo;
  G4int nHisto;
  std::vector<G4int> histoID;
  G4int verbose; 
  G4double zend;
  G4double zend2;
  G4double etot;
  G4double zEvt;
  G4double AbsorberThickness;
  G4double gap;
  G4int NumberOfAbsorbers;
  G4int nAbsSaved;
  G4double maxEnergy;
  G4double trackLength;
  G4bool trackAbs;        // Track is in absorber
  G4int n_evt;
  G4int n_elec;
  G4int n_posit;
  G4int n_gam;
  G4int n_step;
  G4int n_charged_leak;
  G4int n_gam_leak;
  G4int n_charged_back;
  G4int n_gam_back;
  G4bool nTuple;

  G4int n_mumu;
  G4int n_pipi;
};

#endif

// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: FCALTestbeamSetupSD.hh,v 1.2 2002-10-02 19:40:09 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef FCALTestbeamSetupSD_h
#define FCALTestbeamSetupSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class FCALTestbeamSetup;
class G4HCofThisEvent;
class G4Step;
#include "FCALCalorHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class FCALTestbeamSetupSD : public G4VSensitiveDetector
{
  public:
  
  //      FCALTestbeamSetupSD(G4String, FCALTestbeamSetup* );
      FCALTestbeamSetupSD(G4String); 
     ~FCALTestbeamSetupSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
  
  //  FCALCalorHitsCollection*  CalCollection;      
  //      FCALTestbeamSetup* Detector;
  //   G4int*                   HitID;
  
  G4int InitBeam;

public:
  
  G4double EBeamS1, EBeamS2, EBeamS3;
  G4double EHoleScint, EBeamHole;
  G4double EBeamDead;
  G4int TailCatcherID;
  G4double ETailVis[8], ETailDep[7];
  

};

#endif


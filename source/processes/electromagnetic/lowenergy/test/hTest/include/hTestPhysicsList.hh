#ifndef hTestPhysicsList_h
#define hTestPhysicsList_h 1

//---------------------------------------------------------------------------
//
// ClassName:   hTestPhysicsList
//  
// Description: hTest PhysicsList 
//
// Authors:    07.04.01 V.Ivanchenko
//
// Modified:
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4VUserPhysicsList.hh"
#include "hTestDetectorConstruction.hh"
#include "globals.hh"

class hTestPhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class hTestPhysicsList: public G4VUserPhysicsList
{
public: // Without description
    hTestPhysicsList(hTestDetectorConstruction*);
   ~hTestPhysicsList();

public: // Without description
    void SetGammaCut(G4double);
    void SetElectronCut(G4double);
    void SetProtonCut(G4double);
    void SetCutsByEnergy(G4double);
    void GetRange(G4double);
    void SetMaxStep(G4double);
    void SetVerbose(G4int val) {verbose = val;};    
    void SetEMPhysicsList(const G4String&);  
    void SetHadronPhysicsList(const G4String&);  

protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();

private:
    void SetCuts();

    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBarions();
    void ConstructIons();

  // these methods Construct physics processes and register them
    void ConstructGeneral();
    
  private:

    hTestDetectorConstruction* pDet;
    hTestPhysicsListMessenger* theMessenger;
    hTestVEMPhysicsList* theHadList;
    hTestVHadronPhysicsList* theHadList;

    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForProton;
    G4double maxChargedStep;    

    G4String emPhysics;
    G4String hadronPhysics;

    G4int verbose;
};

#endif




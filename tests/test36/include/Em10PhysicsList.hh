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
// $Id: Em10PhysicsList.hh,v 1.3 2007-05-31 18:23:42 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef Em10PhysicsList_h
#define Em10PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4PhotoElectricEffect;
class G4ComptonScattering;
class G4GammaConversion;

class G4MultipleScattering;

class G4PAIonisation ;
class G4ForwardXrayTR ;
class G4eIonisation;
class G4eBremsstrahlung;
class G4eplusAnnihilation;

class G4MuIonisation;
class G4MuBremsstrahlung;
class G4MuPairProduction;

class G4hIonisation;

class Em10StepCut;

class Em10DetectorConstruction;
// class ALICEDetectorConstruction;
class Em10PhysicsListMessenger;
class G4ProductionCuts;


class Em10PhysicsList: public G4VModularPhysicsList  // G4VUserPhysicsList
{
  public:
    Em10PhysicsList( Em10DetectorConstruction*);
  // Em10PhysicsList( ALICEDetectorConstruction*);
   ~Em10PhysicsList();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();
 
    void SetCuts();

  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBarions();

  protected:

    // these methods Construct physics processes and register them

    void AddParameterisation();
    void ConstructGeneral();
    void ConstructEM();

  public:

    void SetGammaCut(G4double);
    void SetElectronCut(G4double);

    void SetRegGammaCut(G4double    cut){fGammaCut    = cut;};
    void SetRegElectronCut(G4double cut){fElectronCut = cut;};
    void SetRegPositronCut(G4double cut){fPositronCut = cut;};

    void SetRadiatorCuts();
    void SetDetectorCuts();

    void SetMaxStep(G4double);
    void SetMinElectronEnergy(G4double E){fMinElectronEnergy=E;};     
    void SetMinGammaEnergy(G4double E)   {fMinGammaEnergy=E;};       
  void SetXTRModel(G4String m)   {fXTRModel = m; G4cout<<fXTRModel<<G4endl;};       

  public:   

    G4double MaxChargedStep;

  private:

    G4PhotoElectricEffect* thePhotoElectricEffect;
    G4ComptonScattering*   theComptonScattering;
    G4GammaConversion*     theGammaConversion;

    G4MultipleScattering*  theeminusMultipleScattering;
    G4eIonisation*         theeminusIonisation;
    G4eBremsstrahlung*     theeminusBremsstrahlung;

    G4ForwardXrayTR*       fForwardXrayTR ;

    G4MultipleScattering*  theeplusMultipleScattering;
    G4eIonisation*         theeplusIonisation;
    G4eBremsstrahlung*     theeplusBremsstrahlung;
    G4eplusAnnihilation*   theeplusAnnihilation;

    Em10StepCut* theeminusStepCut ;
    Em10StepCut* theeplusStepCut ;

    G4double cutForGamma;
    G4double cutForElectron, cutForPositron;

    Em10DetectorConstruction* pDet;
  //  ALICEDetectorConstruction* apDet;
    Em10PhysicsListMessenger* physicsListMessenger;

    G4double fMinElectronEnergy;      // minimalEnergy of produced electrons
    G4double fMinGammaEnergy; 
        // minimalEnergy of scattered photons
    G4ProductionCuts* fRadiatorCuts;
    G4ProductionCuts* fDetectorCuts;
    G4double fElectronCut, fGammaCut, fPositronCut;
    G4String fXTRModel;
};

#endif




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
// $Id: F01PhysicsList.hh,v 1.3 2003-08-27 17:38:54 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef F01PhysicsList_h
#define F01PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
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
class G4hIonisationPlus;

class F01StepCut;

class F01DetectorConstruction;
class F01PhysicsListMessenger;


class F01PhysicsList: public G4VUserPhysicsList
{
  public:
    F01PhysicsList( F01DetectorConstruction*);
   ~F01PhysicsList();

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

    void SetMaxStep(G4double);

  public:   

    G4double MaxChargedStep;

  private:

    G4PhotoElectricEffect* thePhotoElectricEffect;
    G4ComptonScattering*   theComptonScattering;
    G4GammaConversion*     theGammaConversion;
    
    G4MultipleScattering*  theeminusMultipleScattering;
    G4eIonisation*         theeminusIonisation;
    G4eBremsstrahlung*     theeminusBremsstrahlung;

    G4PAIonisation*        fPAIonisation ;
    G4ForwardXrayTR*       fForwardXrayTR ;
    
    G4MultipleScattering*  theeplusMultipleScattering;
    G4eIonisation*         theeplusIonisation;
    G4eBremsstrahlung*     theeplusBremsstrahlung;
    G4eplusAnnihilation*   theeplusAnnihilation;

    F01StepCut* theeminusStepCut ;
    F01StepCut* theeplusStepCut ;

    G4double cutForGamma;
    G4double cutForElectron;

    F01DetectorConstruction* pDet;
    F01PhysicsListMessenger* physicsListMessenger;
};

#endif




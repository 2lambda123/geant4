//========================
// taken from example N04 of G4 distribution
//========================
#ifndef IonPhysics_h
#define IonPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

#include "G4HadronElasticProcess.hh"
#include "G4LElastic.hh"

#include "G4DeuteronInelasticProcess.hh"
#include "G4LEDeuteronInelastic.hh"

#include "G4TritonInelasticProcess.hh"
#include "G4LETritonInelastic.hh"

#include "G4AlphaInelasticProcess.hh"
#include "G4LEAlphaInelastic.hh"

#include "G4hIonisation.hh"
#include "G4MultipleScattering.hh"

class IonPhysics : public G4VPhysicsConstructor
{
  public: 
    IonPhysics(const G4String& name="ion");
    virtual ~IonPhysics();

  public: 
    // This method will be invoked in the Construct() method. 
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type 
    virtual void ConstructProcess();

  protected:
   // Elastic Process
   G4LElastic*            theElasticModel;

   // Generic Ion physics
   G4HadronElasticProcess theIonElasticProcess;
   G4MultipleScattering   fIonMultipleScattering;
   G4hIonisation          fIonIonisation;

   // Deuteron physics
   G4HadronElasticProcess      theDElasticProcess;
   G4MultipleScattering        fDeuteronMultipleScattering;
   G4hIonisation               fDeuteronIonisation;
   G4DeuteronInelasticProcess  fDeuteronProcess;
   G4LEDeuteronInelastic*      fDeuteronModel;

   // Triton physics
   G4HadronElasticProcess      theTElasticProcess;
   G4MultipleScattering        fTritonMultipleScattering;
   G4hIonisation               fTritonIonisation;
   G4TritonInelasticProcess    fTritonProcess;
   G4LETritonInelastic*        fTritonModel;
  
   // Alpha physics
   G4HadronElasticProcess      theAElasticProcess;
   G4MultipleScattering        fAlphaMultipleScattering;
   G4hIonisation               fAlphaIonisation;
   G4AlphaInelasticProcess     fAlphaProcess;
   G4LEAlphaInelastic*         fAlphaModel;

   // He3 physics
   G4HadronElasticProcess      theHe3ElasticProcess;
   G4MultipleScattering        fHe3MultipleScattering;
   G4hIonisation               fHe3Ionisation;

   G4bool wasActivated;
};

// 2002 by J.P. Wellisch

#endif


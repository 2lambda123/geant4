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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: exrdm02StepCut.hh,v 1.1 2003-10-08 16:31:50 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
// ------------------------------------------------------------
//                  05 March 1999  L.Urban
// ------------------------------------------------------------

#ifndef exrdm02StepCut_h
#define exrdm02StepCut_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4Step.hh"

class exrdm02StepCut : public G4VDiscreteProcess
{
  public:     

     exrdm02StepCut(const G4String& processName ="UserStepCut" );
     exrdm02StepCut(exrdm02StepCut &);

     ~exrdm02StepCut();

     G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition
			    );

     G4VParticleChange* PostStepDoIt(
			     const G4Track& ,
			     const G4Step& 
			    );

    void SetMaxStep(G4double);

  protected:

     // it is not needed here !
     G4double GetMeanFreePath(const G4Track& aTrack,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );

			    
  private:
  
  // hide assignment operator as private 
      exrdm02StepCut & operator=(const exrdm02StepCut &right);

  private:

     G4double MaxChargedStep ;
};

// inlined function members implementation

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"
#include "G4EnergyLossTables.hh"

inline G4double exrdm02StepCut::PostStepGetPhysicalInteractionLength(
                             const G4Track& aTrack,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            )
{
  // condition is set to "Not Forced"
  *condition = NotForced;

   G4double ProposedStep = DBL_MAX;

   if((MaxChargedStep > 0.) &&
      (aTrack.GetVolume() != NULL) &&
      (aTrack.GetVolume()->GetName() == "Absorber") &&
      (aTrack.GetDynamicParticle()->GetDefinition()->GetPDGCharge() != 0.))
        ProposedStep = MaxChargedStep ;

   return ProposedStep;
}

inline G4VParticleChange* exrdm02StepCut::PostStepDoIt(
                             const G4Track& aTrack,
                             const G4Step&
                            )
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}

inline G4double exrdm02StepCut::GetMeanFreePath(const G4Track& aTrack,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            )
{
  return 0.;
}

#endif


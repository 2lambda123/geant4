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
// $Id: G4VRestDiscreteProcess.hh,v 1.6 2002-04-15 04:29:57 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
//
// Class Description
//  Abstract class which defines the public behavior of
//  rest + discrete physics interactions.
//
// ------------------------------------------------------------
//   New Physics scheme           8  Mar. 1997  H.Kurahige
// ------------------------------------------------------------
//   modified for new ParticleChange 12 Mar. 1998  H.Kurashige
//   Fixed a bug in PostStepGetPhysicalInteractionLength  
//                                15 Apr. 2002 H.Kurashige 
//

#ifndef G4VRestDiscreteProcess_h
#define G4VRestDiscreteProcess_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VProcess.hh"


class G4VRestDiscreteProcess : public G4VProcess 
{
  //  Abstract class which defines the public behavior of
  //  rest + discrete physics interactions.
  public:     

     G4VRestDiscreteProcess(const G4String& ,
			    G4ProcessType   aType = fNotDefined );
     G4VRestDiscreteProcess(G4VRestDiscreteProcess &);

     virtual ~G4VRestDiscreteProcess();

  public :// with description
     virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition
			    );

     virtual G4VParticleChange* PostStepDoIt(
			     const G4Track& ,
			     const G4Step& 
			    );

     virtual G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& ,
			     G4ForceCondition* 
			    );
      
     virtual G4VParticleChange* AtRestDoIt(
			     const G4Track& ,
			     const G4Step&
			    );

     //  no operation in  AlongStepDoIt
     virtual G4double AlongStepGetPhysicalInteractionLength(
                             const G4Track&,
			     G4double  ,
			     G4double  ,
			     G4double& ,
                             G4GPILSelection*
			    ){ return -1.0; };

     //  no operation in  AlongStepDoIt
     virtual G4VParticleChange* AlongStepDoIt(
			     const G4Track& ,
			     const G4Step& 
			    ) {return 0;};
 
  protected:// with description
     virtual G4double GetMeanFreePath(const G4Track& aTrack,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                           )=0;
      //  Calculates from the macroscopic cross section a mean
      //  free path, the value is returned in units of distance.

     virtual G4double GetMeanLifeTime(const G4Track& aTrack,G4ForceCondition* condition)=0;
      //  Calculates the mean life-time (i.e. for decays) of the
      //  particle at rest due to the occurence of the given process,
      //  or converts the probability of interaction (i.e. for
      //  annihilation) into the life-time of the particle for the
      //  occurence of the given process.

  private:
  // hide default constructor and assignment operator as private 
      G4VRestDiscreteProcess();
      G4VRestDiscreteProcess & operator=(const G4VRestDiscreteProcess &right);

};

// -----------------------------------------
//  inlined function members implementation
// -----------------------------------------
#include "Randomize.hh"              
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4MaterialTable.hh"
#include "G4VParticleChange.hh"

inline 
 G4double G4VRestDiscreteProcess::PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition
			    )
{
  if ( (previousStepSize <=0.0) || (theNumberOfInteractionLengthLeft<=0.0)) {
    // beggining of tracking (or just after DoIt of this process)
    ResetNumberOfInteractionLengthLeft();
  } else {
    // subtract NumberOfInteractionLengthLeft 
    SubtractNumberOfInteractionLengthLeft(previousStepSize);
    if(theNumberOfInteractionLengthLeft<perMillion)
       theNumberOfInteractionLengthLeft=0.;          
  }

  // condition is set to "Not Forced"
  *condition = NotForced;

  // get mean free path
  currentInteractionLength = GetMeanFreePath(track, previousStepSize, condition);

#ifdef G4VERBOSE
   if ((currentInteractionLength <=0.0) || (verboseLevel>2)){
    G4cout << "G4VRestDiscreteProcess::PostStepGetPhysicalInteractionLength ";
    G4cout << "[ " << GetProcessName() << "]" <<G4endl;
    track.GetDynamicParticle()->DumpInfo();
    G4cout << " in Material  " << track.GetMaterial()->GetName() <<G4endl;
    G4cout << "MeanFreePath = " << currentInteractionLength/cm << "[cm]" <<G4endl;
  }
#endif

  G4double value;
  if (currentInteractionLength <DBL_MAX) {
    value = theNumberOfInteractionLengthLeft * currentInteractionLength;
  } else {
    value = DBL_MAX;
  }
#ifdef G4VERBOSE
   if (verboseLevel>1){
    G4cout << "G4VRestDiscreteProcess::PostStepGetPhysicalInteractionLength ";
    G4cout << "[ " << GetProcessName() << "]" <<G4endl;
    track.GetDynamicParticle()->DumpInfo();
    G4cout << " in Material  " <<  track.GetMaterial()->GetName() <<G4endl;
    G4cout << "InteractionLength= " << value/cm <<"[cm] " <<G4endl;
  }
#endif
  return value;
}

inline G4VParticleChange* G4VRestDiscreteProcess::PostStepDoIt(
			     const G4Track& ,
			     const G4Step& 
			    )
{ 
//  reset NumberOfInteractionLengthLeft
    ClearNumberOfInteractionLengthLeft();

    return pParticleChange;
}

inline G4double G4VRestDiscreteProcess::AtRestGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4ForceCondition* condition
			    )
{
  // beggining of tracking 
  ResetNumberOfInteractionLengthLeft();

  // condition is set to "Not Forced"
  *condition = NotForced;

  // get mean life time
  currentInteractionLength = GetMeanLifeTime(track, condition);

#ifdef G4VERBOSE
   if ((currentInteractionLength <0.0) || (verboseLevel>2)){
    G4cout << "G4VRestDiscreteProcess::AtRestGetPhysicalInteractionLength ";
    G4cout << "[ " << GetProcessName() << "]" <<G4endl;
    track.GetDynamicParticle()->DumpInfo();
    G4cout << " in Material  " << track.GetMaterial()->GetName() <<G4endl;
    G4cout << "MeanLifeTime = " << currentInteractionLength/ns << "[ns]" <<G4endl;
  }
#endif

  return theNumberOfInteractionLengthLeft * currentInteractionLength;
}


inline G4VParticleChange* G4VRestDiscreteProcess::AtRestDoIt( 
			     const G4Track&,
			     const G4Step& 
			    )
{
//  clear NumberOfInteractionLengthLeft
    ClearNumberOfInteractionLengthLeft();

    return pParticleChange;
}



#endif


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
// $Id: G4Decay.hh,v 1.9 2004-03-12 04:46:31 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History: first implementation, based on object model of
//      7 July 1996 H.Kurashige
// ------------------------------------------------------------
//  New Physics scheme           18 Jan. 1997  H.Kurahige
// ------------------------------------------------------------
//   modified                     4  Feb. 1997  H.Kurahige
//   modified                     8  Sep. 1997  H.Kurahige
//   remove BuildPhysicsTable()   27 Nov. 1997   H.Kurashige
//   modified for new ParticleChange 12 Mar. 1998  H.Kurashige
//   added aPhysicsTable          2  Aug. 1998 H.Kurashige
//   PreAssignedDecayTime         18 Jan. 2001 H.Kurashige
//   Add External Decayer         23 Feb. 2001  H.Kurashige
//   Remove PhysicsTable          12 Feb. 2002 H.Kurashige
//   Fixed bug in PostStepGPIL 
//    in case of stopping during AlongStepDoIt 12 Mar. 2004 H.Kurashige
//
#ifndef G4Decay_h
#define G4Decay_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VRestDiscreteProcess.hh"
#include "G4ParticleChangeForDecay.hh"
class G4VExtDecayer;

class G4Decay : public G4VRestDiscreteProcess 
{
 // Class Description
  //  This class is a decay process

  public:
    //  Constructors 
    G4Decay(const G4String& processName ="Decay");

    //  Destructor
    virtual ~G4Decay();

  private:
    //  copy constructor
      G4Decay(const G4Decay &right);

    //  Assignment Operation (generated)
      G4Decay & operator=(const G4Decay &right);

  public: //With Description
     // G4Decay Process has both 
     // PostStepDoIt (for decay in flight) 
     //   and 
     // AtRestDoIt (for decay at rest)
  
     virtual G4VParticleChange *PostStepDoIt(
			     const G4Track& aTrack,
                             const G4Step& aStep
                            );

     virtual G4VParticleChange* AtRestDoIt(
			     const G4Track& aTrack,
			     const G4Step&  aStep
			    );

     virtual void BuildPhysicsTable(const G4ParticleDefinition&); 
     // In G4Decay, thePhysicsTable stores values of
    //    beta * sqrt( 1 - beta*beta) 
    //  as a function of normalized kinetic enregy (=Ekin/mass),
    //  becasuse this table is universal for all particle types,


    virtual G4bool IsApplicable(const G4ParticleDefinition&);
    // returns "true" if the decay process can be applied to
    // the particle type. 
 
  protected: // With Description
    virtual G4VParticleChange* DecayIt(
			     const G4Track& aTrack,
			     const G4Step&  aStep
			    );
    // The DecayIt() method returns by pointer a particle-change object,
    // which has information of daughter particles.

  public:
    virtual G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4ForceCondition* condition
                            );

    virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );


  protected: // With Description
    // GetMeanFreePath returns ctau*beta*gamma for decay in flight 
    // GetMeanLifeTime returns ctau for decay at rest
    virtual G4double GetMeanFreePath(const G4Track& aTrack,
                              G4double   previousStepSize,
                              G4ForceCondition* condition
                             );

    virtual G4double GetMeanLifeTime(const G4Track& aTrack,
                              G4ForceCondition* condition
                            );

  public: //With Description
     void SetExtDecayer(G4VExtDecayer*);
     const G4VExtDecayer* GetExtDecayer() const;
     // Set/Get External Decayer
    
  public:
     void  SetVerboseLevel(G4int value);
     G4int GetVerboseLevel() const;

  private:
     G4int verboseLevel;
     // controle flag for output message
     //  0: Silent
     //  1: Warning message
     //  2: More

  private:
    // HighestValue.
    const G4double HighestValue;
 
    // Remainder of life time at rest
    G4double                 fRemainderLifeTime;
 
    // ParticleChange for decay process
    G4ParticleChangeForDecay fParticleChangeForDecay;
    
    // External Decayer
    G4VExtDecayer*    pExtDecayer;
};

inline 
 G4double G4Decay::PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            )
{
  // reset fRemainderLifeTime
  fRemainderLifeTime = 0.0;

  // pre-assigned Decay time
  G4double pTime = track.GetDynamicParticle()->GetPreAssignedDecayProperTime();

  if (pTime < 0.) {
    // normal case 
    fRemainderLifeTime = 0.0;
    return G4VRestDiscreteProcess::PostStepGetPhysicalInteractionLength(track, previousStepSize, condition);
  }

  // condition is set to "Not Forced"
  *condition = NotForced;
  
  // reminder proper time
  G4double remainder = pTime - track.GetProperTime();
  if (remainder <= 0.0) remainder = DBL_MIN;
  
  // use pre-assigned Decay time to determine PIL
  G4double tau = track.GetDefinition()->GetPDGLifeTime();
  return (remainder/tau)*GetMeanFreePath(track, previousStepSize, condition);

}
inline
  G4double G4Decay::AtRestGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4ForceCondition* condition
                            )
{
  G4double pTime = track.GetDynamicParticle()->GetPreAssignedDecayProperTime();
  if (pTime >= 0.) {
    fRemainderLifeTime = pTime - track.GetProperTime();
    if (fRemainderLifeTime <= 0.0) fRemainderLifeTime = DBL_MIN;
  } else {
    fRemainderLifeTime = 
      G4VRestDiscreteProcess::AtRestGetPhysicalInteractionLength(track, condition );
  }
  return fRemainderLifeTime;
}

inline
 void  G4Decay::SetVerboseLevel(G4int value){ verboseLevel = value; }

inline
 G4int G4Decay::GetVerboseLevel() const { return verboseLevel; }

inline  
  G4VParticleChange* G4Decay::AtRestDoIt(
			     const G4Track& aTrack,
			     const G4Step&  aStep
			    )
{
  return DecayIt(aTrack, aStep);
}

inline  
  G4VParticleChange* G4Decay::PostStepDoIt(
			     const G4Track& aTrack,
			     const G4Step&  aStep
			    )
{
  return DecayIt(aTrack, aStep);
}

inline
 void G4Decay::SetExtDecayer(G4VExtDecayer* val)
{
  pExtDecayer = val;
}

inline
 const G4VExtDecayer* G4Decay::GetExtDecayer() const
{
  return pExtDecayer;
}

#endif











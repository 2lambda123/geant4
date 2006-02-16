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
//
// -------------------------------------------------------------------
//      GEANT4 Class file
//
//
//      File name:    G4BinaryCascade.hh 
//
//      Author: Alessandro Brunengo (Alessandro.Brunengo@ge.infn.it)       
// 
//      Creation date: 8 June 2000
// -----------------------------------------------------------------------------

#ifndef G4BinaryCascade_hh
#define G4BinaryCascade_hh

#include "G4VIntraNuclearTransportModel.hh"
#include "G4ReactionProductVector.hh"
#include "G4KineticTrackVector.hh"
#include "G4ListOfCollisions.hh"
#include "G4V3DNucleus.hh"
#include "G4Fancy3DNucleus.hh"
#include "G4Fragment.hh"
#include "G4VFieldPropagation.hh"
#include "G4VScatterer.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"

#include "G4BCDecay.hh"
#include "G4BCLateParticle.hh"
#include "G4BCAction.hh"

class G4CollisionManager;

class G4Track;
class G4KineticTrack;
class G43DNucleus;

class G4BinaryCascade : public G4VIntraNuclearTransportModel
{
public:

  G4BinaryCascade();
  G4BinaryCascade(const G4BinaryCascade & right);

  virtual ~G4BinaryCascade();

  const G4BinaryCascade& operator=(G4BinaryCascade & right);
  G4int operator==(G4BinaryCascade& right) {return (this == &right);}
  G4int operator!=(G4BinaryCascade& right) {return (this != &right);}

  G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack, 
                                              G4Nucleus& theNucleus);
  virtual G4ReactionProductVector * Propagate(G4KineticTrackVector * secondaries,
				      G4V3DNucleus * nucleus);

private:

  G4int GetTotalCharge(std::vector<G4KineticTrack *> & aV)
  {
    G4int result = 0;
    std::vector<G4KineticTrack *>::iterator i;
    for(i = aV.begin(); i != aV.end(); ++i)
    {
       result += G4lrint((*i)->GetDefinition()->GetPDGCharge());
    }
    return result;
  }
  void PrintWelcomeMessage();
  void BuildTargetList();
  void FindCollisions(G4KineticTrackVector * secondaries);
  void FindDecayCollision(G4KineticTrack * secondary);
  void FindLateParticleCollision(G4KineticTrack * secondary);
  G4bool ApplyCollision(G4CollisionInitialState * collision);
  G4bool Capture(G4bool verbose=false);
  G4bool Absorb();
  G4bool CheckPauliPrinciple(G4KineticTrackVector * products);
  G4double GetExcitationEnergy();
  G4bool CheckDecay(G4KineticTrackVector * products);
  void CorrectFinalPandE();
  void UpdateTracksAndCollisions(G4KineticTrackVector * oldSecondaries,
				 G4KineticTrackVector * oldTarget,
				 G4KineticTrackVector * newSecondaries);
  G4bool DoTimeStep(G4double timeStep);
  G4KineticTrackVector* CorrectBarionsOnBoundary(G4KineticTrackVector *in, 
                                               G4KineticTrackVector *out);
  G4Fragment * FindFragments();
  void StepParticlesOut();
  G4LorentzVector GetFinal4Momentum();
  G4LorentzVector GetFinalNucleusMomentum();
  G4ReactionProductVector * Propagate1H1(G4KineticTrackVector * secondaries,
				      G4V3DNucleus * nucleus);
  G4double GetIonMass(G4int Z, G4int A);
  
// utility methods
  G4ThreeVector GetSpherePoint(G4double r, const G4LorentzVector & momentumdirection);
  void ClearAndDestroy(G4KineticTrackVector * ktv);
  void ClearAndDestroy(G4ReactionProductVector * rpv);

// for debugging purpose
  void PrintKTVector(G4KineticTrackVector * ktv, std::string comment=std::string(""));
  void PrintKTVector(G4KineticTrack* kt, std::string comment=std::string(""));
  void DebugApplyCollision(G4CollisionInitialState * collision, 
                           G4KineticTrackVector *products);
private:
  G4KineticTrackVector theProjectileList;
  G4KineticTrackVector theTargetList;
  G4KineticTrackVector theSecondaryList;
  G4KineticTrackVector theCapturedList;
  G4KineticTrackVector theFinalState;

  G4ExcitationHandler * theExcitationHandler;
  G4CollisionManager * theCollisionMgr;

  std::vector<G4BCAction *> theImR;
  G4BCDecay * theDecay;
  G4BCLateParticle * theLateParticle;
  G4VFieldPropagation * thePropagator;
  G4double theCurrentTime;
  G4double theCutOnP;
  G4double theCutOnPAbsorb;
  G4LorentzVector theInitial4Mom;
  G4int currentA, currentZ;
  G4double massInNucleus;
  G4double currentInitialEnergy;   // for debugging
  G4LorentzRotation precompoundLorentzboost;
  G4double theOuterRadius;
  G4bool thePrimaryEscape;
  G4ParticleDefinition * thePrimaryType;
  G4ThreeVector theMomentumTransfer;
  
  

};

#endif





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
// $Id: G4QCaptureAtRest.hh,v 1.1 2004-03-05 13:26:57 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QCaptureAtRest class ----------------
//                 by Mikhail Kossov, December 2003.
//  G4QCaptureAtRest class (pi-,K-,antiN) of the CHIPS Simulation Branch in GEANT4
// -------------------------------------------------------------------------------
// This is a unique CHIPS class for the Nuclear Capture At Rest Prosesses.
// -------------------------------------------------------------------------------
// At present (Dec.03) only pi-, K- and antiNucleon capture is implemented, which
// are the most crucial for the in matter simulation. The hyperon capture (Sigma-,
// Xi-, Omega-, antiSigma+) is implemented, but not tested and it is not clear how
// frequently this kind of interaction takes place in the simulation of the hadronic
// showers. The antiNeutron Capture At Rest is implemented by this G4QCaptureAtRest
// class, but it is not clear how the anti-neutrons are stopped in Geant4 tracking.
// It can be stopped only by interactions with electrons, as the annihilation
// cross section is huge and any interaction with nucleus results in annihilation.
// --> The mu-&tau- Capture At Rest (mu-,nu)&(mu-,nu) are weak processes, which must
// be simulated together with the reversed Betha decay (e-,nu). While mu- capture is
// similar to the pi- capture from the nuclear fragmentation point of view (the energy
// scale is shrinked mecause m_mu<m_pi and a part of energy is lost because of the
// neutrino radiation), the time scale of the mu- capture process is not clear,
// but it is clear, that it is well delayed. By this reason the mu- capture is not
// included in the G4QCaptureAtRest and must be implemented in the "LongLivingDecay"
// branch of simulation, which includes excited states of nuclei and short living
// isotopes. On the "Fast Simulation" Level all radioactiv isotopes, long living
// nuclear excitations, mu-atoms etc, which can be important for the background
// signals, must be collected in the continuous database and simulated separately.
// --> CHIPS is SU(3) event generator, so it does not include reactions with the
// heavy (c,b,t) quarks involved such as tau- or antiDs-, which can be simulated
// only by SU(6) QUIPS (QUark Invariant Phase Space) model.-December 2003.M.Kossov.-
// -------------------------------------------------------------------------------
// All algorithms are similar: the captured particle is absorbed by a nuclear cluster
// with the subsequent Quark Exchange nuclear fragmentation. The Anti-Proton (antiSigma+)
// Capture algorithm is more complicated: the anti-baryon annihilates with the quasyfree
// nucleons on the nuclear periphery. The peripheral interaction results in a number
// of mesons. A part of them misses the nucleus and comes directly to the output,
// while others create Multy Quasmon Excitation in the nucleus with the subsequent
// Quark Excange Fragmentation of the nucleus. At present the two step mechanism of
// the antiProton-Nucleus interaction is hardwired in the G4QEnvironment class, but
// with time the first step of the interaction can be moved to this G4QCaptureAtRest
// class, to make the G4QEnvirement class simpler and better defined. This is
// necessary because the G4QEnvironment class is going to loos the previlage of
// the CHIPS Head Class (as previously the G4Quasmon class lost it) and G4QCollision
// class is going to be the CHIPS Head Class, where a few Nuclear Environments can
// exist (e.g. the Nuclear Environment of the Projectile Nucleus and the Nuclear
// Environment of the Target Nucleus). By the way the antiProton-H1 interaction At
// Rest (CHIPSI) can be still simulated with only the G4Quasmon class, as this
// reaction does not have any nuclear environment.- December 2003.Mikhail Kossov.-
// --------------------------------------------------------------------------------

#ifndef G4QCaptureAtRest_hh
#define G4QCaptureAtRest_hh

// GEANT4 Headers
#include "globals.hh"
#include "G4ios.hh"
#include "G4VRestProcess.hh"
#include "G4ParticleTypes.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4NucleiPropertiesTable.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"

// CHIPS Headers
#include "G4QEnvironment.hh"
#include "G4QIsotope.hh"
//<vector> is included in G4QIsotope.hh
//#include <vector>

class G4QCaptureAtRest : public G4VRestProcess
{  
private:

  // Hide assignment operator as private 
  G4QCaptureAtRest& operator=(const G4QCaptureAtRest &right);

  // Copy constructor
  G4QCaptureAtRest(const G4QCaptureAtRest& );

public:

  // Constructor
  G4QCaptureAtRest(const G4String& processName ="CHIPSNuclearAbsorptionAtRest");

  // Destructor
  ~G4QCaptureAtRest();

  G4bool IsApplicable(const G4ParticleDefinition& particle);

  G4VParticleChange* AtRestDoIt(const G4Track& aTrack, const G4Step& aStep); 

protected:                         

  // zero mean lifetime
  G4double GetMeanLifeTime(const G4Track& aTrack, G4ForceCondition* );
private:

  G4bool RandomizeMuDecayOrCapture(G4int Z, G4int N); // true=MuCapture, false=MuDecay
  void CalculateEnergyDepositionOfMuCapture(G4int Z); // (2p->1s, MeV) @@ Now N-independent
  G4bool RandomizeTauDecayOrCapture(G4int Z, G4int N);// true=TauCapture, false=TauDecay
  void CalculateEnergyDepositionOfTauCapture(G4int Z);// (2p->1s, MeV) @@N-independ,Improve

private:
  G4double Time;                                      // Time shift of the capture reaction
  G4double EnergyDeposition;                          // Energy deposited in the reaction

};
#endif


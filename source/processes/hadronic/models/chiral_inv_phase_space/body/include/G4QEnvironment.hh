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
// $Id: G4QEnvironment.hh,v 1.26 2005-03-24 16:06:06 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QEnvironment ----------------
//             by Mikhail Kossov, August 2000.
//      header for Multy Quasmon Environment in the CHIPS Model
// ------------------------------------------------------------

#ifndef G4QEnvironment_h
#define G4QEnvironment_h 1

#include "G4QuasmonVector.hh"

class G4QEnvironment 
{
public:
  G4QEnvironment(const G4QHadronVector& projHadrons, const G4int targPDG);
  G4QEnvironment(const G4QEnvironment& right);         // copy QEnvironment by value
  G4QEnvironment(G4QEnvironment* right);               // copy QEnvironment by pointer
  ~G4QEnvironment();                                   // Public Destructor

  // Overloaded operators
  const G4QEnvironment& operator=(const G4QEnvironment& right);
  G4bool operator==(const G4QEnvironment &right) const;
  G4bool operator!=(const G4QEnvironment &right) const;

  //Selectors
  G4QNucleus       GetEnvironment() const;
  G4QuasmonVector* GetQuasmons();           // User is responsible for Destroy/Clear/Delete
  G4QHadronVector* GetQHadrons();           // User is responsible for Destroy/Clear/Delete
  G4QHadronVector* GetProjectiles();        // User is responsible for Destroy/Clear/Delete

  // Modifiers
  G4QHadronVector* Fragment();              // User must clear and destroy the G4QHadronVec

  // Static functions
  static void SetParameters(G4double solAn=0.4,G4bool efFlag=false,G4double piThresh=141.4,
                            G4double mpisq=20000., G4double dinum=1880.);
  static void OpenElectromagneticDecays();
  static void CloseElectromagneticDecays();

protected:
  void CleanUpQHadrons();  // Only another G4QEnvironment issue can use it (can make mess)
  void FillQHadrons(G4QHadronVector* input); //Only another G4QEnvironment issue can use it

private:
  G4QHadronVector* FSInteraction();         // Final State Interaction after Hadronization
  G4QHadronVector  HadronizeQEnvironment(); // Main HadronizationFunction used in Fragment
  void             CopyAndDeleteHadronVector(G4QHadronVector* HV);//Copy HadrVect to Output
  void             CreateQuasmon(const G4QContent& projQC, const G4LorentzVector& proj4M);
  void             InitClustersVector(G4int maxC, G4int maxA);//Init.NucClust's for 1st int
  void             CleanUp();               // Makes theEnvironment=vacuum & kill Quasmons
  void             PrepareInteractionProbabilities(const G4QContent& projQC, G4double AP);
  void             EvaporateResidual(G4QHadron* evap, G4bool corFlag=false);// Final Evap.
  void             DecayBaryon(G4QHadron* dB);     // Decay baryon (gamma+N or Delta->N+Pi)
  void             DecayDibaryon(G4QHadron* dB);   // Decay di-baryon (deuteron is kept)
  void             DecayIsonucleus(G4QHadron* dB); // Decay nP+(Pi+) or nN+(Pi-) system
  void             DecayMultyBaryon(G4QHadron* dB);// Decay of Ap, An or AL states
  void             DecayAntiStrange(G4QHadron* dB);// Decay nuclei containing K+/K0
  void             DecayAlphaBar(G4QHadron* dB);   // Decay of alpha+p or alpha+n states
  void             DecayAlphaDiN(G4QHadron* dB);   // Decay of alpha+p+p states
  void             DecayAlphaAlpha(G4QHadron* dB); // Decay of alpha+alpha state
  G4bool           CheckGroundState(G4Quasmon* quasm,G4bool corFlag=false);//as G4Q for QHV
  G4bool           DecayInEnvQ(G4Quasmon* quasm);  // Use befor evaporation in PANIC case

// Body
private:
  // Static Parameters
  static G4bool      WeakDecays;     // Flag for opening WeakDecays (notUsed: allAreClosed)
  static G4bool      ElMaDecays;     // Flag for opening ElectroMagDecays (true by default)
  static G4bool      EnergyFlux;     // Flag for Energy Flux use instead of Multy Quasmon
  static G4double    SolidAngle;     // Part of Solid Angle to capture secondaries(@@A-dep)
  static G4double    PiPrThresh;     // Pion Production Threshold for gammas
  static G4double    M2ShiftVir;     // Shift for M2=-Q2=m_pi^2 of the virtual gamma
  static G4double    DiNuclMass;     // Double Nucleon Mass for virtual normalization
  // Output hadrons
  G4QHadronVector    theQHadrons;    // Vector of generated secondary hadrons
  // Internal working objects
  G4QHadronVector    intQHadrons;    // Vector of postponed secondary hadrons
  G4QCHIPSWorld*     theWorld;       // the CHIPS World
  G4int              nBarClust;      // Maximum BarionNumber of clusters (To optimize calc)
  G4double           f2all;          // Ratio of freeNucleons to free+denseNucleons
  G4QuasmonVector    theQuasmons;    // Intermediate vectorOfQuasmons before fragmentation
  G4QCandidateVector theQCandidates; // Vector of possible candidates to clusters
  G4QNucleus         theEnvironment; // InitialNucleus (later ResidualNuclearEnvironment)
  G4LorentzVector    tot4Mom;        // Total 4-momentum in the reaction
  G4int              totCharge;      // Total charge in the reaction (for current control)
  G4int              totBaryoN;      // Total baryon number in the reaction (for cur.cont)
  G4QHadronVector    theProjectiles; // Vector of projectiles in the interaction
  G4int              theTargetPDG;   // PDG of the target nucleus in the interaction
};

//General function makes Random Unit 3D-Vector
G4ThreeVector RndmDir();

// Inline functions
inline G4bool G4QEnvironment::operator==(const G4QEnvironment &rhs) const
                                                                     {return this == &rhs;}
inline G4bool G4QEnvironment::operator!=(const G4QEnvironment &rhs) const
                                                                     {return this != &rhs;}
inline G4QNucleus G4QEnvironment::GetEnvironment() const {return theEnvironment;}
#endif

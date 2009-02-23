//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4QSplitter.hh,v 1.3 2009-02-23 09:49:24 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QSplitter ----------------
//             by Mikhail Kossov, Avgust 2005.
// header for Hadron-Hadron Splitter in parton pairs of the CHIPS Model
// --------------------------------------------------------------------
// Short description: the hadron befor the interaction must be splitted
// in partons: quark-antiquark (mesons) or quark-diquark (baryon) parts
// (some time in more than two parts, e.g. baryon in three quarks or a
// few quark-antiquark pairs can be added for the splitting). Then each
// projectile parton can create a parton pair (G4QPartonPair) with the
// target parton. This pair with the big rapidity difference on the ends
// creates a planar Quark-Gluon String (a pole). A pair of the projectile
// partons with a pair of the target partons can create a cylindrical
// string (doubled string in the algorithm - a cut).  
// -------------------------------------------------===------------------

#ifndef G4QSplitter_h
#define G4QSplitter_h 1

#include "G4QuasmonVector.hh"
//#include "G4RandomDirection.hh"
// Call G4RandomDirection() (global) instead of old RndmDir() (local)

class G4QSplitter 
{
public:
  // projectile Hadron (probably as a part of proj Nucleus) colliding with a nuclear target
  //G4QSplitter(G4QHadron projHadron, const G4bool projEnvFlag, const G4int targPDG);
  // projectile Hadron (probably as a part of proj Nucleus) colliding with nuclear Cluster
  G4QSplitter(G4QHadron projHadron, const G4bool projEnvFlag, const G4int targPDG,
              const G4bool targEnvFlag);   // Cluster can have target nuclear environment
  // projectile Cluster (probably as a part of proj Nucleus) colliding with nuclear target
  //G4QSplitter(G4QNucleus projCluster, const G4bool projEnvFlag, const G4int targPDG);
  // projectile Cluster (probably as a part of proj Nucleus) colliding with nuclear Cluster
  //G4QSplitter(G4QNucleus projCluster, const G4bool projEnvFlag, const G4int targPDG,
  //            const G4bool targEnvFlag);   // Cluster can have target nuclear environment
  G4QSplitter(const G4QSplitter& right);   // Copy the splitted system by value
  G4QSplitter(G4QSplitter* right);         // Copy the splitted system by pointer
  ~G4QSplitter();                          // Public Destructor

  // Overloaded operators
  const G4QSplitter& operator=(const G4QSplitter& right); // Copy the splitted system
  G4bool operator==(const G4QSplitter &right) const; // Compare two splitted systems
  G4bool operator!=(const G4QSplitter &right) const; // Compare two splitted systems

  //Selectors
  G4bool           GetProjEnvFlag(); // Flag of nuclear environment around the projectile
  G4bool           GetTargEnvFlag(); // Flag of nuclear environment around the target
  G4QuasmonVector* GetQuasmons();    // Get not decayed Quasmons (User must ClearAndDestr)
  G4QHadronVector* GetHadrons();     // Get current outputHadrons (User must ClearAndDestr)
  G4double         GetWeight();      // Get weight of the event (?)
  G4int            GetNOfHadrons();  // Get the number of created G4QHadrons
  G4int            GetNOfQuasmons(); // Get the number of created G4Quasmons
  // Modifiers (Output: a Vector of Quasmons for fragmentation, some Quasms can be Hadrons)
  G4QHadronVector* Fragment();       // User must clear and destroy the G4QHadronVector

  // Static functions
  //static void SetParameters(G4double StParName=0., G4bool stFlag=false);

private:  
  G4QHadronVector HadronizeQString();     // Main HadronizationFunction used in Fragment()
  G4double RandomizeMomFractionFree(G4int nPart); // RandomMomFrac for nPart free partons
  G4double RandomizeMomFractionString(G4int nPart); // RandomMomFrac for nPart free partons

// Body
private:
  // Static Parameters
  //static G4double    StParName;    // Example of static parameter (see SetParameters)

  // Body
  G4bool             theProjEnvFlag; // Projectile Environment Flag
  G4bool             theTargEnvFlag; // Target Environment Flag
  G4QContent         theProjQC;      // Projectile Quark Content
  G4QContent         theTargQC;      // Target Quark Content
  G4LorentzVector    theProj4Mom;    // Projectile 4-momentum
  G4LorentzVector    theTarg4Mom;    // Target 4-momentum
  // Internal Quasmons
  G4QuasmonVector    theQuasmons;    // Vector of generated secondary Quasmons
  // Output Hadrons
  G4QHadronVector    theQHadrons;    // Vector of generated output Hadrons

  // Internal working parameters
  G4QCHIPSWorld*     theWorld;       // the CHIPS World
  G4LorentzVector    tot4Mom;        // Total 4-momentum in the reaction
  G4int              totCharge;      // Total charge in the reaction (for current control)
  G4int              totBaryNum;     // Total baryon number in the reaction (for cur.cont.)
  G4double           theWeight;      // Weight of the event
};

//General function makes Random Unit 3D-Vector
G4ThreeVector RndmDir();             // @@ ??

// Inline functions
inline G4bool G4QSplitter::operator==(const G4QSplitter &rhs) const {return this == &rhs;}
inline G4bool G4QSplitter::operator!=(const G4QSplitter &rhs) const {return this != &rhs;}

#endif

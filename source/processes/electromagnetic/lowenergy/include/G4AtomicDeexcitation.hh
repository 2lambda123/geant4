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
// $Id: G4AtomicTransitionManager.hh,v 1.2 ????
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Authors: Elena Guardincerri (Elena.Guardincerri@ge.infn.it)
//          Alfonso Mantero (Alfonso.Mantero@ge.infn.it)
//
// History:
// -----------
//  
//  16 Sept 2001  First committed to cvs
//
// -------------------------------------------------------------------

// Class description:
// Low Energy Electromagnetic Physics, management of atomic deexcitation
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef G4AtomicDeexcitation_h
#define G4AtomicDeexcitation_h 1

#include "globals.hh"
#include "g4std/vector"
#include "G4DynamicParticle.hh"

class G4AtomicDeexcitation {

public:

  G4AtomicDeexcitation();
  ~G4AtomicDeexcitation();
  
  // Returns a vector contains the photons generated by radiative transitions
  // (non zero particles) or by non radiative transitions (zero particles)  
  G4std::vector<G4DynamicParticle*>* GenerateParticles(G4int Z, G4int shellId);
  
  void SetCutForSecondaryPhotons(G4double cut);
  // Set threshold energy for fluorescence 

  void SetCutForAugerElectrons(G4double cut);
  // Set threshold energy for Auger electron production

  void ActivateAugerElectronProduction(G4bool val);
  // Activate Auger electron production


private:
  
  // Decides wether a radiative transition is possible and, if it is,
  // returns the identity of the starting shell for the transition
  const G4int SelectTypeOfTransition(G4int Z, G4int shellId);
  
  // Generates a particle from a radiative transition and returns it
  G4DynamicParticle* GenerateFluorescence(G4int Z, G4int shellId,G4int provShellId);
 
  // Dummy function: when invowed returns a null pointer
  G4DynamicParticle* GenerateAuger(G4int Z, G4int shellId);
 
  // Data member which stores the shells to be filled by 
  // the radiative transition
  G4int newShellId;

  G4double minGammaEnergy;
  G4double minElectronEnergy;
  G4bool   fAuger;

  // Data member wich stores the id of the shell where is the vacancy 
  // left from the Auger electron
  G4int augerVacancyId;
 
};

#endif





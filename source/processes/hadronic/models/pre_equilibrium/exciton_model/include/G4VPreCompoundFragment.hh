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
// $Id: G4VPreCompoundFragment.hh,v 1.3 2006-06-29 20:58:42 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// by V. Lara

#ifndef G4VPreCompoundFragment_h
#define G4VPreCompoundFragment_h 1

#include "G4ios.hh"
#include <iomanip>
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4Fragment.hh"
#include "G4VCoulombBarrier.hh"

class G4ReactionProduct;

class G4VPreCompoundFragment
{
  // ============================
  // Constructors and destructor
  // ============================
  
protected:
  // default constructor
  G4VPreCompoundFragment() {};
    
public:  
  // copy constructor
  G4VPreCompoundFragment(const G4VPreCompoundFragment &right);
    
  // constructor  
  G4VPreCompoundFragment(const G4double anA, const G4double aZ,
			 G4VCoulombBarrier * aCoulombBarrier,
			 const G4String &  aName);
  
  virtual ~G4VPreCompoundFragment();
  
  // ==========
  // operators 
  // ========== 
  
  const G4VPreCompoundFragment& 
  operator= (const G4VPreCompoundFragment &right);
  
  G4int operator==(const G4VPreCompoundFragment &right) const;
  
  G4int operator!=(const G4VPreCompoundFragment &right) const;
  
  friend std::ostream& 
  operator<<(std::ostream&, const G4VPreCompoundFragment*);
  friend std::ostream& 
  operator<<(std::ostream&, const G4VPreCompoundFragment&);
  
  // =====================
  // Pure Virtual methods
  // =====================
  virtual G4ReactionProduct * GetReactionProduct() const = 0; 	
  
  // Initialization method
  void Initialize(const G4Fragment & aFragment);
    
  // Methods for calculating the emission probability
  // ------------------------------------------------
  
  // Calculates the total (integrated over kinetic energy) emission
  // probability of a fragment
  virtual G4double CalcEmissionProbability(const G4Fragment & aFragment) = 0;
  
  virtual G4double GetKineticEnergy(const G4Fragment & aFragment) = 0;

public:
  inline const G4double GetA() const;
  
  inline const G4double GetZ() const;
  
  inline const G4double GetRestA() const;
  
  inline const G4double GetRestZ() const;
  
  inline const G4double GetCoulombBarrier() const;
  
  inline const G4double GetBindingEnergy() const;
  
  inline const G4double GetMaximalKineticEnergy() const;
  
  inline const G4double GetEnergyThreshold() const;

  inline const G4double GetEmissionProbability() const;
  
  inline const G4double GetNuclearMass() const;
  
  inline const G4double GetRestNuclearMass() const;
  
  inline const G4double GetReducedMass() const;
  
  inline const G4LorentzVector GetMomentum() const;
  
  inline void  SetMomentum(const G4LorentzVector & value);
  
  inline void  SetFragmentName(const G4String& aName);
  
  inline const G4String GetName() const;
 
  inline void ResetStage();

  inline G4int GetStage() const;

  inline void IncrementStage();


  // =============
  // Data members
  // =============

private:
  
  G4double theA;
  
  G4double theZ;
  
  G4double theRestNucleusA;
  
  G4double theRestNucleusZ;
protected:  
  G4double theCoulombBarrier;
private:
  G4VCoulombBarrier * theCoulombBarrierPtr;
  
  G4double theBindingEnergy;
  
  G4double theMaximalKineticEnergy;
  
protected:
  G4double theEmissionProbability;
private:  
  G4LorentzVector theMomentum;
  
  G4String theFragmentName;

  G4int theStage;    
};

#include "G4VPreCompoundFragment.icc"

#endif

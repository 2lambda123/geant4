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
// $Id: EvapChannel.hh,v 1.1 2003-10-08 12:32:17 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//


#ifndef EvaporationChannel_h
#define EvaporationChannel_h 1

#include "G4VEvaporationChannel.hh"
#include "G4VEmissionProbability.hh"
#include "MyEvapProb.hh"
#include "G4VLevelDensityParameter.hh"
#include "G4VCoulombBarrier.hh"
#include "MyLDP.hh"
#include "G4NucleiProperties.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"


class EvaporationChannel : public G4VEvaporationChannel
{
public:
  // Available constructors
  EvaporationChannel(const G4int theA, const G4int theZ,
		       G4VEmissionProbability * aEmissionStrategy,
		       G4VCoulombBarrier * aCoulombBarrier);

  EvaporationChannel(const G4int theA, const G4int theZ, const G4String & aName,
		       G4VEmissionProbability * aEmissionStrategy,
		       G4VCoulombBarrier * aCoulombBarrier);

  EvaporationChannel(const G4int theA, const G4int theZ, const G4String * aName,
		       G4VEmissionProbability * aEmissionStrategy,
		       G4VCoulombBarrier * aCoulombBarrier);

public:
  // destructor
  ~EvaporationChannel();
  
  void SetEmissionStrategy(G4VEmissionProbability * aEmissionStrategy)
  {theEvaporationProbabilityPtr = aEmissionStrategy;}
  
  void SetCoulombBarrierStrategy(G4VCoulombBarrier * aCoulombBarrier)
  {theCoulombBarrierPtr = aCoulombBarrier;} 
  
protected:
  // default constructor
  EvaporationChannel() {};
  
private:
  // copy constructor
  EvaporationChannel(const EvaporationChannel & right);
  
private:
  const EvaporationChannel & operator=(const EvaporationChannel & right);
  
public:
  G4bool operator==(const EvaporationChannel & right) const;
  G4bool operator!=(const EvaporationChannel & right) const;

public:
  void Initialize(const G4Fragment & fragment);

  G4FragmentVector * BreakUp(const G4Fragment & theNucleus);
  
//   inline void SetEmissionStrategy(G4VEmissionProbability * aStrategy)
//     {
//       if (MyOwnEvaporationProbability) delete theEvaporationProbabilityPtr;
//       theEvaporationProbabilityPtr = aStrategy;
//       MyOwnEvaporationProbability = false;
//     }


  inline void SetLevelDensityParameter(G4VLevelDensityParameter * aLevelDensity)
  {
    if (MyOwnLevelDensity) delete theLevelDensityPtr;
    theLevelDensityPtr = aLevelDensity;
    MyOwnLevelDensity = false;
  }
  
public:


  inline G4double GetEmissionProbability(void) const 
  {return EmissionProbability;}
  
  
  inline G4double GetMaximalKineticEnergy(void) const 
  { return MaximalKineticEnergy; }
  
  // ----------------------
  
private: 
  
  // Calculate Binding Energy for separate fragment from nucleus
  G4double CalcBindingEnergy(const G4int anA, const G4int aZ);

  // Calculate maximal kinetic energy that can be carried by fragment (in MeV)
  G4double CalcMaximalKineticEnergy(const G4double U);

  // Samples fragment kinetic energy.
  G4double CalcKineticEnergy(G4Fragment& frag);

  // This has to be removed and put in Random Generator
  G4ThreeVector IsotropicVector(const G4double Magnitude  = 1.0);

	// Data Members
	// ************
private:

  // This data member define the channel. 
  // They are intializated at object creation (constructor) time.

  // Atomic Number
  G4int A;

  // Charge
  G4int Z;


  // For evaporation probability calcualtion
  G4VEmissionProbability * theEvaporationProbabilityPtr;

  // For Level Density calculation
  G4bool MyOwnLevelDensity;
  G4VLevelDensityParameter * theLevelDensityPtr;
  
  // For Coulomb Barrier calculation
  G4VCoulombBarrier * theCoulombBarrierPtr;
  G4double CoulombBarrier;

  //---------------------------------------------------

  // These values depend on the nucleus that is being evaporated.
  // They are calculated through the Initialize method which takes as parameters 
  // the atomic number, charge and excitation energy of nucleus.

  // Residual Atomic Number
  G4int AResidual;

  // Residual Charge
  G4int ZResidual;

//   // Binding Energy
//   G4double BindingEnergy;

// 	// Level Density Parameter
// 	G4double LevelDensityParameter;
	
  // Emission Probability
  G4double EmissionProbability;


  // Maximal Kinetic Energy that can be carried by fragment
  G4double MaximalKineticEnergy;
};


#endif

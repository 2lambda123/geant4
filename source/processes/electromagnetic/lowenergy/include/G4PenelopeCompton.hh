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

// $Id: G4PenelopeCompton.hh,v 1.7 2003-06-16 16:59:46 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Luciano Pandola
//
// History:
// -----------
// 05 Dec 2002   L. Pandola   1st implementation
// 12 Feb 2003   MG Pia       const argument in SelectRandomAtomForCompton
// 14 Feb 2003   MG Pia       Modified some variables to lowercase initial 
// 26 Mar 2003   L.Pandola    Added fluorescence
//
// -------------------------------------------------------------------

// Class description:
// Low Energy Electromagnetic Physics, Compton Scattering
// Penelope Model
// -------------------------------------------------------------------

#ifndef G4PENELOPECOMPTON_HH
#define G4PENELOPECOMPTON_HH 1

#include "globals.hh"
#include "G4VDiscreteProcess.hh"

class G4Track;
class G4Step;
class G4ParticleDefinition;
class G4VParticleChange;
class G4VEMDataSet;
class G4VRangeTest;
class G4Material;

class G4PenelopeCompton : public G4VDiscreteProcess {

public:
  
  G4PenelopeCompton(const G4String& processName ="PenCompton");
  
  ~G4PenelopeCompton();

  G4bool IsApplicable(const G4ParticleDefinition& definition);
  
  void BuildPhysicsTable(const G4ParticleDefinition& photon);
 
  G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);
 
  // For testing purpose only
  G4double DumpMeanFreePath(const G4Track& aTrack, 
			    G4double previousStepSize, 
			    G4ForceCondition* condition) 
  { return GetMeanFreePath(aTrack, previousStepSize, condition); }

protected:

  G4double GetMeanFreePath(const G4Track& aTrack, 
			   G4double previousStepSize, 
			   G4ForceCondition* condition);

private: 

  // Hide copy constructor and assignment operator as private 
  G4PenelopeCompton& operator=(const G4PenelopeCompton& right);
  G4PenelopeCompton(const G4PenelopeCompton& );

  void ReadData();

  G4double CrossSection(G4double energy,G4int Z);
  G4double DifferentialCrossSection (G4double cdt);

  G4double lowEnergyLimit;  // low energy limit  applied to the process
  G4double highEnergyLimit; // high energy limit applied to the process

  G4VEMDataSet* meanFreePathTable;
  G4VRangeTest* rangeTest;

  const G4double intrinsicLowEnergyLimit; // intrinsic validity range
  const G4double intrinsicHighEnergyLimit;

  G4double energyForIntegration; //for numerical integration of
  G4int ZForIntegration;// analytical cross section

  std::vector<G4DataVector*> *ionizationEnergy;
  std::vector<G4DataVector*> *hartreeFunction;
  std::vector<G4DataVector*> *occupationNumber;
  
  G4int SelectRandomAtomForCompton(const G4Material* material,G4double e) const;

  const G4int nBins; //for building cross section table

  std::vector<G4VEMDataSet*>* matCrossSections; //for random choice of atom
  G4double cutForLowEnergySecondaryPhotons;
};

#endif


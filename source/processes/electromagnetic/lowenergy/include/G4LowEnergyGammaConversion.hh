// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4LowEnergyGammaConversion.hh,v 1.8 2001-05-07 23:32:07 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//      CERN Geneva Switzerland
//
//      For information related to this code contact:
//      CERN, CN Division, ASD group
//      ------------ G4LowEnergyGammaConversion physics process ------
//                   by A.Forti 1999/03/02
//
// Class description:
// Low Energy process, Photon conversion
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// ************************************************************

#ifndef G4LowEnergyGammaConversion_h
#define G4LowEnergyGammaConversion_h 1

// Base Class Headers
#include "G4VDiscreteProcess.hh"

// Contained Variables Headers
#include "G4LowEnergyUtilities.hh"
#include "G4Gamma.hh"

class G4LowEnergyGammaConversion : public G4VDiscreteProcess
 
{
private:
  // hide assignment operator as private 
  G4LowEnergyGammaConversion& operator=(const G4LowEnergyGammaConversion &right);
  G4LowEnergyGammaConversion(const G4LowEnergyGammaConversion& );
  
public:
 
  G4LowEnergyGammaConversion(const G4String& processName ="LowEnConversion");
 
  ~G4LowEnergyGammaConversion();

  G4bool IsApplicable(const G4ParticleDefinition&);

  void BuildPhysicsTable(const G4ParticleDefinition& GammaType);

  G4double GetMeanFreePath(const G4Track& aTrack, 
			   G4double previousStepSize, 
			   G4ForceCondition* condition);

  G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);
 
protected:
  
  void BuildCrossSectionTable();
  void BuildMeanFreePathTable();
  void BuildZVec();

private:
  
  G4Element* SelectRandomAtom(const G4DynamicParticle*, G4Material*);

  static G4double ScreenFunction1(G4double ScreenVariable);
  static G4double ScreenFunction2(G4double ScreenVariable);
  
private:

  G4SecondLevel* theCrossSectionTable;    
  G4PhysicsTable* theMeanFreePathTable;

  G4DataVector* ZNumVec;

  G4double LowestEnergyLimit; 
  G4double HighestEnergyLimit;
  G4int NumbBinTable; 

  G4double MeanFreePath; // actual Mean Free Path (current medium)
  G4LowEnergyUtilities util;
};

#include "G4LowEnergyGammaConversion.icc"
#endif
 









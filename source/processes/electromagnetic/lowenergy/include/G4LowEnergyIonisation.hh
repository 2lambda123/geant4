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
// $Id: G4LowEnergyIonisation.hh,v 1.23 2001-09-10 18:05:16 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//      --- G4LowEnergyIonisation physics process for electrons
//                by Alessandra Forti July 1999
// ************************************************************
//
//   07.04.2000 Veronique Lefebure + Laszlo Urban
// - First implemention of continuous energy loss
// 14/07/99: corrections , L.Urban
// 20/09/00 update fluctuations V.Ivanchenko
//
// Class description:
// Low Energy Electromagnetic process, electron Ionisation
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// ------------------------------------------------------------
 
#ifndef G4LowEnergyIonisation_h
#define G4LowEnergyIonisation_h 1


// Base Class Headers
#include "G4eLowEnergyLoss.hh"

// Contained Variables Headers
#include "G4LowEnergyUtilities.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

typedef G4FirstLevel oneShellTable;
typedef G4SecondLevel oneAtomTable;
typedef G4ThirdLevel allAtomTable;

class G4LowEnergyIonisation : public G4eLowEnergyLoss{

public:
  
  G4LowEnergyIonisation(const G4String& processName = "LowEnergyIoni"); 
  
  ~G4LowEnergyIonisation();
  
  G4bool IsApplicable(const G4ParticleDefinition&); 
  
  void SetCutForLowEnSecPhotons(G4double);

  void SetCutForLowEnSecElectrons(G4double);

  void BuildPhysicsTable(const G4ParticleDefinition& aParticleType);
  
  G4double GetMeanFreePath(const G4Track& track,
			   G4double previousStepSize,
			   G4ForceCondition* condition ) ;

  inline G4double GetTransitionShell(G4int k){return(thePrimShVec[k]);};

  G4VParticleChange *PostStepDoIt(const G4Track& track,         
				  const G4Step& Step ) ;                 
  
  void PrintInfoDefinition();

  G4double GetShellCrossSection(const G4double AtomicNumber,
                                const G4int subshellindex,
                                const G4double KineticEnergy) ;
  G4double GetShellCrossSectionwithCut(const G4double AtomicNumber,
                                       const G4int subshellindex,
                                       const G4double KineticEnergy,
                                       const G4double Tcut) ;
  G4double GetShellEnergyLosswithCut(const G4double AtomicNumber,
                                     const G4int subshellindex,
                                     const G4double KineticEnergy,
                                     const G4double Tcut) ;
  void SetLowEnergyLimit(G4double val) {if(val > 0.0) lEnergyLimit = val;};
  
  private:
  
  virtual G4double ComputeCrossSection(const G4double AtomicNumber,
				       const G4double IncEnergy);
  G4double ComputeCrossSectionWithCut(const G4double AtomIndex,
				      const G4double IncEnergy,
		   	 	      const G4double CutEnergy);
  G4double ComputeMicroscopicCrossSection(const G4double AtomIndex,
				      const G4double IncEnergy,
		   	 	      const G4double CutEnergy);
  
  void BuildLossTable(const G4ParticleDefinition& aParticleType);
  void BuildShellCrossSectionTable();
  void BuildBindingEnergyTable();
  void BuildFluorTransitionTable();
  void BuildSamplingCoeffTable();
  void BuildZVec();
  void BuildLambdaTable(const G4ParticleDefinition& aParticleType);

private:
  
  // hide assignment operator 
  G4LowEnergyIonisation & operator=(const G4LowEnergyIonisation &right);
  G4LowEnergyIonisation(const G4LowEnergyIonisation&);
  
private:
  
  G4int SelectRandomShell(const G4int AtomIndex
                        , const G4double IncEnergy
			, const G4double CutEnergy);
  
  G4Element* SelectRandomAtom(const G4DynamicParticle* aDynamicPhoton, 
			      G4Material* aMaterial);
  
  G4bool SelectRandomTransition(G4int, G4double*, 
				const oneAtomTable*);



  G4double EnergySampling(const G4int AtomicNumber
                        , const G4int ShellIndex
			, const G4double KinEn
			, const G4double deltaRayMinE = 0.1*eV);

  allAtomTable* allAtomShellCrossSec;
  allAtomTable* theFluorTransitionTable;
  allAtomTable* theSamplingCoeffTable;
  G4SecondLevel* theBindingEnergyTable;  
  G4DataVector* ZNumVec;
  G4DataVector* ZNumVecFluor;

  G4PhysicsTable* theMeanFreePathTable;

  G4double CutForLowEnergySecondaryPhotons;
  G4double CutForLowEnergySecondaryElectrons;
  G4double MeanFreePath;

  G4double lowestKineticEnergy;
  G4double highestKineticEnergy;
  G4int TotBin;

  G4LowEnergyUtilities util;

  G4DataVector thePrimShVec;
  G4double lEnergyLimit;
};
 
#include "G4LowEnergyIonisation.icc"
 
#endif
 















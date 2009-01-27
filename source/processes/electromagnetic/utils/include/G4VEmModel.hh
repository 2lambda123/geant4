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
// $Id: G4VEmModel.hh,v 1.62 2009-01-27 10:19:32 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4VEmModel
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 03.01.2002
//
// Modifications:
//
// 23-12-02 V.Ivanchenko change interface before move to cut per region
// 24-01-03 Cut per region (V.Ivanchenko)
// 13-02-03 Add name (V.Ivanchenko)
// 25-02-03 Add sample theta and displacement (V.Ivanchenko)
// 23-07-03 Replace G4Material by G4MaterialCutCouple in dE/dx and CrossSection
//          calculation (V.Ivanchenko)
// 01-03-04 L.Urban signature changed in SampleCosineTheta 
// 23-04-04 L.urban signature of SampleCosineTheta changed back 
// 17-11-04 Add method CrossSectionPerAtom (V.Ivanchenko)
// 14-03-05 Reduce number of pure virtual methods and make inline part 
//          separate (V.Ivanchenko)
// 24-03-05 Remove IsInCharge and add G4VParticleChange in the constructor (VI)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
// 15-04-05 optimize internal interface for msc (V.Ivanchenko)
// 08-05-05 A -> N (V.Ivanchenko)
// 25-07-05 Move constructor and destructor to the body (V.Ivanchenko)
// 02-02-06 ComputeCrossSectionPerAtom: default value A=0. (mma)
// 06-02-06 add method ComputeMeanFreePath() (mma)
// 07-03-06 Optimize msc methods (V.Ivanchenko)
// 29-06-06 Add member currentElement and Get/Set methods (V.Ivanchenko)
// 29-10-07 Added SampleScattering (V.Ivanchenko)
// 15-07-08 Reorder class members and improve comments (VI)
// 21-07-08 Added vector of G4ElementSelector and methods to use it (VI)
// 12-09-08 Added methods GetParticleCharge, GetChargeSquareRatio, 
//          CorrectionsAlongStep, ActivateNuclearStopping (VI)
//
// Class Description:
//
// Abstract interface to energy loss models

// -------------------------------------------------------------------
//

#ifndef G4VEmModel_h
#define G4VEmModel_h 1

#include "globals.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4ElementVector.hh"
#include "G4DataVector.hh"
#include "G4VEmFluctuationModel.hh"
#include "G4EmElementSelector.hh"
#include "Randomize.hh"
#include <vector>

class G4PhysicsTable;
class G4Region;
class G4VParticleChange;
class G4Track;

class G4VEmModel
{

public:

  G4VEmModel(const G4String& nam);

  virtual ~G4VEmModel();

  //------------------------------------------------------------------------
  // Virtual methods to be implemented for any concrete model
  //------------------------------------------------------------------------

  virtual void Initialise(const G4ParticleDefinition*, 
			  const G4DataVector&) = 0;

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin = 0.0,
				 G4double tmax = DBL_MAX) = 0;

  //------------------------------------------------------------------------
  // Methods with standard implementation; may be overwritten if needed 
  //------------------------------------------------------------------------

  // dEdx per unit length
  virtual G4double ComputeDEDX(const G4MaterialCutsCouple*,
			       const G4ParticleDefinition*,
			       G4double kineticEnergy,
			       G4double cutEnergy = DBL_MAX);

  // main method to compute dEdx
  virtual G4double ComputeDEDXPerVolume(const G4Material*,
					const G4ParticleDefinition*,
					G4double kineticEnergy,
					G4double cutEnergy = DBL_MAX);

  // cross section per volume
  virtual G4double CrossSection(const G4MaterialCutsCouple*,
				const G4ParticleDefinition*,
				G4double kineticEnergy,
				G4double cutEnergy = 0.0,
				G4double maxEnergy = DBL_MAX);

  // main method to compute cross section per Volume
  virtual G4double CrossSectionPerVolume(const G4Material*,
					 const G4ParticleDefinition*,
					 G4double kineticEnergy,
					 G4double cutEnergy = 0.0,
					 G4double maxEnergy = DBL_MAX);

  // main method to compute cross section depending on atom
  virtual G4double ComputeCrossSectionPerAtom(const G4ParticleDefinition*,
					      G4double kinEnergy, 
					      G4double Z, 
					      G4double A = 0., /* amu */
					      G4double cutEnergy = 0.0,
					      G4double maxEnergy = DBL_MAX);
				      				     
  // min cut in kinetic energy allowed by the model
  virtual G4double MinEnergyCut(const G4ParticleDefinition*,
				const G4MaterialCutsCouple*);

  // Compute effective ion charge square
  virtual G4double GetChargeSquareRatio(const G4ParticleDefinition*,
					const G4Material*,
					G4double kineticEnergy);

  // Compute ion charge 
  virtual G4double GetParticleCharge(const G4ParticleDefinition*,
				     const G4Material*,
				     G4double kineticEnergy);

  // add correction to energy loss and ompute non-ionizing energy loss
  virtual void CorrectionsAlongStep(const G4MaterialCutsCouple*,
				    const G4DynamicParticle*,
				    G4double& eloss,
				    G4double& niel,
				    G4double length);

  // sample PIXE deexcitation
  virtual void SampleDeexcitationAlongStep(const G4Material*,
					   const G4Track&,
                                           G4double& eloss);

protected:

  // kinematically allowed max kinetic energy of a secondary
  virtual G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
				      G4double kineticEnergy);  
				          
  //------------------------------------------------------------------------
  // Methods for msc simulation which needs to be overwritten 
  //------------------------------------------------------------------------

public:

  virtual void SampleScattering(const G4DynamicParticle*,
				G4double safety);

  virtual G4double ComputeTruePathLengthLimit(const G4Track& track, 
					      G4PhysicsTable* theLambdaTable, 
					      G4double currentMinimalStep);

  virtual G4double ComputeGeomPathLength(G4double truePathLength);

  virtual G4double ComputeTrueStepLength(G4double geomPathLength);

  virtual void DefineForRegion(const G4Region*);

  virtual void SetupForMaterial(const G4ParticleDefinition*,
				const G4Material*,
                                G4double kineticEnergy);

  //------------------------------------------------------------------------
  // Generic methods common to all models
  //------------------------------------------------------------------------

  // should be called at initialisation to build element selectors
  void InitialiseElementSelectors(const G4ParticleDefinition*, 
				  const G4DataVector&);

  // compute mean free path via cross section per volume
  G4double ComputeMeanFreePath(const G4ParticleDefinition*,
			       G4double kineticEnergy,
			       const G4Material*,    
			       G4double cutEnergy = 0.0,
			       G4double maxEnergy = DBL_MAX);

  // generic cross section per element
  inline G4double ComputeCrossSectionPerAtom(const G4ParticleDefinition*,
                                             const G4Element*,
					     G4double kinEnergy, 
					     G4double cutEnergy = 0.0,
					     G4double maxEnergy = DBL_MAX);

  // atom can be selected effitiantly if element selectors are initialised 
  inline const G4Element* SelectRandomAtom(const G4MaterialCutsCouple*,
					   const G4ParticleDefinition*,
					   G4double kineticEnergy,
					   G4double cutEnergy = 0.0,
					   G4double maxEnergy = DBL_MAX);

  // this method can be used only in the case if generic method to compute
  // cross section per volume is used and not overwritten in derived class 
  inline const G4Element* SelectRandomAtom(const G4Material*,
					   const G4ParticleDefinition*,
					   G4double kineticEnergy,
					   G4double cutEnergy = 0.0,
					   G4double maxEnergy = DBL_MAX);

  // select isotope in order to have precise mass of the nucleus
  inline G4int SelectIsotopeNumber(const G4Element*);

  //------------------------------------------------------------------------
  // Get/Set methods
  //------------------------------------------------------------------------

  inline G4VEmFluctuationModel* GetModelOfFluctuations();

  inline G4double HighEnergyLimit() const;

  inline G4double LowEnergyLimit() const;

  inline G4double PolarAngleLimit() const;

  inline G4double SecondaryThreshold() const;

  inline G4bool LPMFlag() const;

  inline void SetHighEnergyLimit(G4double);

  inline void SetLowEnergyLimit(G4double);

  inline void SetPolarAngleLimit(G4double);

  inline void SetSecondaryThreshold(G4double);

  inline void SetLPMFlag(G4bool val);

  inline void ActivateNuclearStopping(G4bool);

  inline G4double MaxSecondaryKinEnergy(const G4DynamicParticle* dynParticle);

  inline const G4String& GetName() const;

  inline void SetParticleChange(G4VParticleChange*, G4VEmFluctuationModel*);

protected:

  inline const G4Element* GetCurrentElement() const;

  inline void SetCurrentElement(const G4Element*);

private:

  //  hide assignment operator
  G4VEmModel & operator=(const  G4VEmModel &right);
  G4VEmModel(const  G4VEmModel&);

  // ======== Parameters of the class fixed at construction =========

  G4VEmFluctuationModel* fluc;
  const G4String   name;

  // ======== Parameters of the class fixed at initialisation =======

  G4double        lowLimit;
  G4double        highLimit;
  G4double        polarAngleLimit;
  G4double        secondaryThreshold;
  G4bool          theLPMflag;

  G4int           nSelectors;
  std::vector<G4EmElementSelector*> elmSelectors;

protected:

  G4VParticleChange*  pParticleChange;
  G4bool              nuclearStopping;

  // ======== Cashed values - may be state dependent ================

private:

  const G4Element* currentElement;
  G4int                  nsec;
  std::vector<G4double>  xsec;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::HighEnergyLimit() const
{
  return highLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::LowEnergyLimit() const
{
  return lowLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::PolarAngleLimit() const
{
  return polarAngleLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::SecondaryThreshold() const
{
  return secondaryThreshold;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4bool G4VEmModel::LPMFlag() const 
{
  return theLPMflag;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SetHighEnergyLimit(G4double val)
{
  highLimit = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SetLowEnergyLimit(G4double val)
{
  lowLimit = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SetPolarAngleLimit(G4double val)
{
  polarAngleLimit = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SetSecondaryThreshold(G4double val) 
{
  secondaryThreshold = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4VEmModel::SetLPMFlag(G4bool val) 
{
  theLPMflag = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4VEmModel::ActivateNuclearStopping(G4bool val)
{
  nuclearStopping = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::ComputeCrossSectionPerAtom(
		const G4ParticleDefinition* part,
		const G4Element* elm,
		G4double kinEnergy, 
		G4double cutEnergy,
		G4double maxEnergy)
{
  currentElement = elm;
  return ComputeCrossSectionPerAtom(part,kinEnergy,elm->GetZ(),elm->GetN(),
				    cutEnergy,maxEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SetParticleChange(G4VParticleChange* p,  
                                          G4VEmFluctuationModel* f = 0)
{
  if(p && pParticleChange != p) pParticleChange = p;
  fluc = f;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


inline G4VEmFluctuationModel* G4VEmModel::GetModelOfFluctuations()
{
  return fluc;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::MinEnergyCut(const G4ParticleDefinition*,
                                         const G4MaterialCutsCouple*)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::GetChargeSquareRatio(const G4ParticleDefinition* p,
						 const G4Material*, G4double)
{
  G4double q = p->GetPDGCharge()/CLHEP::eplus;
  return q*q;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::GetParticleCharge(const G4ParticleDefinition* p,
					      const G4Material*, G4double)
{
  return p->GetPDGCharge();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::CorrectionsAlongStep(const G4MaterialCutsCouple*,
					     const G4DynamicParticle*,
					     G4double&,G4double&,G4double)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SampleDeexcitationAlongStep(const G4Material*,
						    const G4Track&,
						    G4double&)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::ComputeDEDXPerVolume(const G4Material*,
						 const G4ParticleDefinition*,
						 G4double,G4double)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::ComputeDEDX(const G4MaterialCutsCouple* c,
                                        const G4ParticleDefinition* p,
					G4double kinEnergy,
					G4double cutEnergy)
{
  return ComputeDEDXPerVolume(c->GetMaterial(),p,kinEnergy,cutEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::CrossSection(const G4MaterialCutsCouple* c,
                                         const G4ParticleDefinition* p,
					 G4double kinEnergy,
					 G4double cutEnergy,
					 G4double maxEnergy)
{
  return CrossSectionPerVolume(c->GetMaterial(),p,
			       kinEnergy,cutEnergy,maxEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::ComputeCrossSectionPerAtom(
                                         const G4ParticleDefinition*,
					 G4double, G4double, G4double,
					 G4double, G4double)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline 
const G4Element* G4VEmModel::SelectRandomAtom(const G4MaterialCutsCouple* couple,
					      const G4ParticleDefinition* p,
					      G4double kinEnergy,
					      G4double cutEnergy,
					      G4double maxEnergy)
{
  if(nSelectors > 0) {
    currentElement = 
      elmSelectors[couple->GetIndex()]->SelectRandomAtom(kinEnergy);
  } else {
    currentElement = SelectRandomAtom(couple->GetMaterial(),p,kinEnergy,
				      cutEnergy,maxEnergy);
  }
  return currentElement;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline 
const G4Element* G4VEmModel::SelectRandomAtom(const G4Material* material,
					      const G4ParticleDefinition* pd,
					      G4double kinEnergy,
					      G4double tcut,
					      G4double tmax)
{
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int n = material->GetNumberOfElements() - 1;
  currentElement = (*theElementVector)[n];
  if (n > 0) {
    G4double x = G4UniformRand()*
                 G4VEmModel::CrossSectionPerVolume(material,pd,kinEnergy,tcut,tmax);
    for(G4int i=0; i<n; i++) {
      if (x <= xsec[i]) {
	currentElement = (*theElementVector)[i];
	break;
      }
    }
  }
  return currentElement;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4int G4VEmModel::SelectIsotopeNumber(const G4Element* elm)
{
  G4int N = G4int(elm->GetN() + 0.5);
  G4int ni = elm->GetNumberOfIsotopes();
  if(ni > 0) {
    G4int idx = 0;
    if(ni > 1) {
      G4double* ab = currentElement->GetRelativeAbundanceVector();
      G4double x = G4UniformRand();
      for(; idx<ni; idx++) {
	x -= ab[idx];
	if (x <= 0.0) break;
      }
      if(idx >= ni) idx = ni - 1;
    }
    N = elm->GetIsotope(idx)->GetN();
  }
  return N;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline const G4Element* G4VEmModel::GetCurrentElement() const
{
  return currentElement;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SetCurrentElement(const G4Element* elm)
{
  currentElement = elm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline 
G4double G4VEmModel::MaxSecondaryKinEnergy(const G4DynamicParticle* dynPart)
{
  return MaxSecondaryEnergy(dynPart->GetDefinition(),
                            dynPart->GetKineticEnergy());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::MaxSecondaryEnergy(const G4ParticleDefinition*,
					       G4double kineticEnergy)
{
  return kineticEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline const G4String& G4VEmModel::GetName() const 
{
  return name;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Methods for msc simulation
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SampleScattering(const G4DynamicParticle*, G4double)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::ComputeTruePathLengthLimit(
                                const G4Track&, 
				G4PhysicsTable*, 
				G4double)
{
  return DBL_MAX;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::ComputeGeomPathLength(G4double truePathLength)
{
  return truePathLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double G4VEmModel::ComputeTrueStepLength(G4double geomPathLength)
{
  return geomPathLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::DefineForRegion(const G4Region*) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4VEmModel::SetupForMaterial(const G4ParticleDefinition*,
					 const G4Material*, G4double)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


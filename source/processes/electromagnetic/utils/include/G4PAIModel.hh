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
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4PAIModel
//
// Author:        V. Grichine based on Vladimir Ivanchenko  code
//
// Creation date: 05.10.2003
//
// Modifications:
//
//
// Class Description:
//
// Implementation of PAI model of energy loss and
// delta-electron production by heavy charged particles

// -------------------------------------------------------------------
//

#ifndef G4PAIModel_h
#define G4PAIModel_h 1

#include <vector>
#include "G4VEmModel.hh"
#include "G4Region.hh"
#include "globals.hh"
#include "G4VEmFluctuationModel.hh"

class G4PAIModel : public G4VEmModel, public G4VEmFluctuationModel
{

public:

  G4PAIModel(const G4ParticleDefinition* p = 0, const G4String& nam = "PAI");

  ~G4PAIModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  G4double HighEnergyLimit(const G4ParticleDefinition* p);

  G4double LowEnergyLimit(const G4ParticleDefinition* p);

  void SetHighEnergyLimit(G4double e) {fHighKinEnergy = e;};

  void SetLowEnergyLimit(G4double e) {fLowKinEnergy = e;};

  G4double MinEnergyCut(const G4ParticleDefinition*,
                        const G4MaterialCutsCouple*);

  G4bool IsInCharge(const G4ParticleDefinition*);

  G4double ComputeDEDX(const G4Material*,
                       const G4ParticleDefinition*,
                             G4double kineticEnergy,
                             G4double cutEnergy);

  G4double CrossSection(const G4Material*,
                        const G4ParticleDefinition*,
                              G4double kineticEnergy,
                              G4double cutEnergy,
                              G4double maxEnergy);

  G4DynamicParticle* SampleSecondary(
                                const G4MaterialCutsCouple*,
                                const G4DynamicParticle*,
                                      G4double tmin,
                                      G4double maxEnergy);

  std::vector<G4DynamicParticle*>* SampleSecondaries(
                                const G4MaterialCutsCouple*,
                                const G4DynamicParticle*,
                                      G4double tmin,
                                      G4double maxEnergy);

  G4double MaxSecondaryEnergy(const G4DynamicParticle*);

  G4double SampleFluctuations(const G4Material*,
                          const G4DynamicParticle*,
 				G4double&,
                                G4double&,
                                G4double&);

  G4double Dispersion(    const G4Material*,
                          const G4DynamicParticle*,
 				G4double&,
                                G4double&);

  void DefineForRegion(const G4Region* r) ;

protected:

  G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
                                    G4double kinEnergy);

private:

  void SetParticle(const G4ParticleDefinition* p);

  // hide assignment operator 
  G4PAIModel & operator=(const  G4PAIModel &right);
  G4PAIModel(const  G4PAIModel&);


  std::vector<const G4Region*> fPAIRegionVector;
  const G4ParticleDefinition* fParticle;
  G4double fMass;
  G4double fSpin;
  G4double fChargeSquare;
  G4double fRatio;
  G4double fHighKinEnergy;
  G4double fLowKinEnergy;
  G4double fTwoln10;
  G4double fBg2lim; 
  G4double fTaulim;
  G4double fQc;
};

/////////////////////////////////////////////////////////////////////

inline G4double G4PAIModel::MaxSecondaryEnergy( const G4ParticleDefinition*,
                                                      G4double kinEnergy) 
{

  G4double gamma= kinEnergy/fMass + 1.0;
  G4double tmax = 2.0*electron_mass_c2*(gamma*gamma - 1.) /
                  (1. + 2.0*gamma*fRatio + fRatio*fRatio);
  
  return tmax;
}

/////////////////////////////////////////////////////////////////////////

inline G4double G4PAIModel::MaxSecondaryEnergy(const G4DynamicParticle* dp)
{

  G4double kineticEnergy = dp->GetKineticEnergy();
  G4double gamma= kineticEnergy/fMass + 1.0;
  G4double tmax = 2.0*electron_mass_c2*(gamma*gamma - 1.) /
                  (1. + 2.0*gamma*fRatio + fRatio*fRatio);
  
  return tmax;
}

///////////////////////////////////////////////////////////////

inline  void G4PAIModel::DefineForRegion(const G4Region* r) 
{
  //  G4Region* rPAI = r;
  //  fPAIRegionVector.push_back(rPAI);
  fPAIRegionVector.push_back(r);
}



#endif








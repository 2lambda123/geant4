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
// File name:     G4PAIPhotonModel
//
// Author:        V. Grichine based on Vladimir Ivanchenko  code
//
// Creation date: 05.10.2003
//
// Modifications:
// 11.04.05 Major optimisation of internal interfaces (V.Ivantchenko)
//
//
// Class Description:
//
// Implementation of PAI model of energy loss and
// delta-electron production by heavy charged particles

// -------------------------------------------------------------------
//

#ifndef G4PAIPhotonModel_h
#define G4PAIPhotonModel_h 1

#include <vector>
#include "G4VEmModel.hh"
#include "globals.hh"
#include "G4VEmFluctuationModel.hh"

class G4PhysicsLogVector;
class G4PhysicsTable;
class G4Region;
class G4MaterialCutsCouple;
class G4ParticleChangeForLoss;

class G4PAIPhotonModel : public G4VEmModel, public G4VEmFluctuationModel
{

public:

  G4PAIPhotonModel(const G4ParticleDefinition* p = 0, const G4String& nam = "PAI");

  virtual ~G4PAIPhotonModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  
  void InitialiseMe(const G4ParticleDefinition*) {};

  G4double MinEnergyCut(const G4ParticleDefinition*,
                        const G4MaterialCutsCouple*);

  G4double ComputeDEDX(const G4MaterialCutsCouple*,
                       const G4ParticleDefinition*,
                             G4double kineticEnergy,
                             G4double cutEnergy);

  G4double CrossSection(const G4MaterialCutsCouple*,
                        const G4ParticleDefinition*,
                              G4double kineticEnergy,
                              G4double cutEnergy,
                              G4double maxEnergy);

  std::vector<G4DynamicParticle*>* SampleSecondaries(
                                const G4MaterialCutsCouple*,
                                const G4DynamicParticle*,
                                      G4double tmin,
                                      G4double maxEnergy);

  G4double SampleFluctuations(const G4Material*,
                          const G4DynamicParticle*,
 				G4double&,
                                G4double&,
                                G4double&);

  G4double Dispersion(    const G4Material*,
                          const G4DynamicParticle*,
 				G4double&,
                                G4double&);

  void     DefineForRegion(const G4Region* r) ;
  void     ComputeSandiaPhotoAbsCof();
  void     BuildPAIonisationTable();
  void     BuildLambdaVector(const G4MaterialCutsCouple* matCutsCouple);

  G4double GetdNdxCut( G4int iPlace, G4double transferCut);
  G4double GetdNdxPhotonCut( G4int iPlace, G4double transferCut);
  G4double GetdNdxPlasmonCut( G4int iPlace, G4double transferCut);

  G4double GetdEdxCut( G4int iPlace, G4double transferCut);

  G4double GetPostStepTransfer(G4PhysicsTable*, G4PhysicsLogVector*,
                               G4int iPlace, G4double scaledTkin );
  G4double GetAlongStepTransfer(G4PhysicsTable*, G4PhysicsLogVector*,
                               G4int iPlace, G4double scaledTkin,G4double step, G4double cof );
  G4double GetEnergyTransfer(G4PhysicsTable*, G4int iPlace,
                             G4double position, G4int iTransfer );



protected:

  G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
                                    G4double kinEnergy);

private:

  void SetParticle(const G4ParticleDefinition* p);

  // hide assignment operator 
  G4PAIPhotonModel & operator=(const  G4PAIPhotonModel &right);
  G4PAIPhotonModel(const  G4PAIPhotonModel&);

  // The vector over proton kinetic energies: the range of gammas

  G4double             fLowestKineticEnergy;
  G4double             fHighestKineticEnergy;
  G4int                fTotBin;
  G4int                fMeanNumber;
  G4PhysicsLogVector*  fProtonEnergyVector ;



  // vectors

  G4PhysicsTable*                    fPAItransferTable;
  std::vector<G4PhysicsTable*>       fPAIxscBank;

  G4PhysicsTable*                    fPAIphotonTable;
  std::vector<G4PhysicsTable*>       fPAIphotonBank;

  G4PhysicsTable*                    fPAIplasmonTable;
  std::vector<G4PhysicsTable*>       fPAIplasmonBank;

  G4PhysicsTable*                    fPAIdEdxTable;
  std::vector<G4PhysicsTable*>       fPAIdEdxBank;

  std::vector<const G4MaterialCutsCouple*> fMaterialCutsCoupleVector;
  std::vector<const G4Region*>       fPAIRegionVector;

  size_t                             fMatIndex ;  
  G4double**                         fSandiaPhotoAbsCof ;
  G4int                              fSandiaIntervalNumber ;

  G4PhysicsLogVector*              fdEdxVector ;
  std::vector<G4PhysicsLogVector*> fdEdxTable ;

  G4PhysicsLogVector*              fLambdaVector ;
  std::vector<G4PhysicsLogVector*> fLambdaTable ;

  G4PhysicsLogVector*              fdNdxCutVector ;
  std::vector<G4PhysicsLogVector*> fdNdxCutTable ;

  G4PhysicsLogVector*              fdNdxCutPhotonVector ;
  std::vector<G4PhysicsLogVector*> fdNdxCutPhotonTable ;

  G4PhysicsLogVector*              fdNdxCutPlasmonVector ;
  std::vector<G4PhysicsLogVector*> fdNdxCutPlasmonTable ;


  const G4ParticleDefinition* fParticle;
  G4ParticleChangeForLoss*    fParticleChange;

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

inline G4double G4PAIPhotonModel::MaxSecondaryEnergy( const G4ParticleDefinition*,
                                                      G4double kinEnergy) 
{

  G4double gamma= kinEnergy/fMass + 1.0;
  G4double tmax = 2.0*electron_mass_c2*(gamma*gamma - 1.) /
                  (1. + 2.0*gamma*fRatio + fRatio*fRatio);
  
  return tmax;
}

///////////////////////////////////////////////////////////////

inline  void G4PAIPhotonModel::DefineForRegion(const G4Region* r) 
{
  //  G4Region* rPAI = r;
  //  fPAIRegionVector.push_back(rPAI);
  fPAIRegionVector.push_back(r);
}



#endif








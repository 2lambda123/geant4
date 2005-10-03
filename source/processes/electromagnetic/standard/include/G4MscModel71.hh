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
// $Id: G4MscModel71.hh,v 1.1 2005-10-03 01:09:57 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
//
// GEANT4 Class header file
//
//
// File name:     G4MscMOdel71
//
// Author:        Laszlo Urban
//
// Creation date: 01.03.2001
//
// Modifications:
//
// 27-03-03 Move model part from G4MultipleScattering (V.Ivanchenko)
// 27-03-03 Rename (V.Ivanchenko)
//
// 05-08-03 angle distribution has been modified (L.Urban)
// 26-11-03 new data member currentRange (L.Urban)
// 01-03-04 changes in data members + signature changed in SampleCosineTheta
// 11-03-04 changes in data members (L.Urban)
// 23-04-04 changes in data members and in signature of SampleCosineTheta
//          (L.Urban)
// 17-08-04 name of data member facxsi changed to factail (L.Urban)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
// 15-04-05 optimize internal interface - add SampleSecondaries method (V.Ivanchenko)
// 03-10-05 Model is freezed with the name McsModel71 (V.Ivanchenko)

//
// Class Description:
//
// Implementation of the model of multiple scattering based on
// H.W.Lewis Phys Rev 78 (1950) 526 and L.Urban model

// -------------------------------------------------------------------
//

#ifndef G4MscModel71_h
#define G4MscModel71_h 1

#include "G4VEmModel.hh"

class G4ParticleChangeForMSC;
class G4Navigator;

class G4MscModel71 : public G4VEmModel
{

public:

  G4MscModel71(G4double&, G4double&, G4double&, G4double&, G4bool&,
	     const G4String& nam = "MscUni");

  virtual ~G4MscModel71();

  virtual void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  virtual G4double CrossSectionPerVolume(const G4Material*,
					 const G4ParticleDefinition*,
					 G4double kineticEnergy,
					 G4double cutEnergy = 0.0,
					 G4double maxEnergy = DBL_MAX);

  virtual std::vector<G4DynamicParticle*>* SampleSecondaries(
                                const G4MaterialCutsCouple*,
                                const G4DynamicParticle*,
                                      G4double length,
                                      G4double safety);

  G4double GeomPathLength(G4PhysicsTable* theLambdaTable,
                    const G4MaterialCutsCouple* couple,
                    const G4ParticleDefinition* particle,
                          G4double& kineticEnergy,
                          G4double lambda,
                          G4double range,
                          G4double truePathLength);

  G4double TrueStepLength(G4double geomStepLength);

  G4double SampleCosineTheta(G4double trueStepLength,G4double KineticEnergy);

  G4double SampleDisplacement();

  void SetLateralDisplasmentFlag(G4bool val);

private:

  G4double ComputeTransportCrossSection(
                             const G4ParticleDefinition* particle,
                                   G4double KineticEnergy,
                                   G4double AtomicNumber,
                                   G4double AtomicWeight);

  // hide assignment operator
  G4MscModel71 & operator=(const  G4MscModel71 &right);
  G4MscModel71(const  G4MscModel71&);

  const G4ParticleDefinition* particle;
  G4ParticleChangeForMSC*     fParticleChange;
  G4Navigator*                navigator;

  G4double mass;
  G4double charge;
  G4double massRate;

  G4double taubig;
  G4double tausmall;
  G4double taulim;
  G4double currentTau;
  G4double dtrl;
  G4double NuclCorrPar;
  G4double FactPar;
  G4double factail ;

  G4double sigmafactor;
  G4double b;
  G4double xsi;

  G4double lambda0;
  G4double tPathLength;
  G4double par1,par2,par3 ;

  G4double stepmin ;

  G4double currentKinEnergy;
  G4double currentRange ; 
  G4double currentRadLength;

  G4bool   samplez;
  G4bool   latDisplasment;
  G4bool   isInitialized;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4MscModel71::SetLateralDisplasmentFlag(G4bool val)
{
  latDisplasment = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif


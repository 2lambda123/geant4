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
// $Id: G4CoulombScattering.hh,v 1.1 2006-03-21 15:40:54 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4CoulombScattering
//
// Author:        Vladimir Ivanchenko 
//
// Creation date: 12.03.2006
//
// Modifications:
//
// Class Description:
//
// This class manages the process of Coulomb elastic scattering
//

// -------------------------------------------------------------------
//

#ifndef G4CoulombScattering_h
#define G4CoulombScattering_h 1

#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"

class G4CoulombScattering : public G4VEmProcess
{

public:

  G4CoulombScattering(const G4String& name = "CoulombScat");

  virtual ~G4CoulombScattering();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  void SetThetaMin(G4double);

  void SetThetaMax(G4double);

  void SetQ2Max(G4double);

  // Print out of the class parameters
  virtual void PrintInfo();

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*);

  virtual std::vector<G4DynamicParticle*>* SecondariesPostStep(
                                   G4VEmModel*,
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle*);

private:

 // hide assignment operator
  G4CoulombScattering & operator=(const G4CoulombScattering &right);
  G4CoulombScattering(const G4CoulombScattering&);
  
  G4double thetaMin;
  G4double thetaMax;
  G4double q2Max;
  G4bool isInitialised;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4bool G4CoulombScattering::IsApplicable(const G4ParticleDefinition& p)
{
  return (p.GetPDGCharge() != 0.0 && !p.IsShortLived());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4CoulombScattering::SetThetaMin(G4double val)
{
  thetaMin = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4CoulombScattering::SetThetaMax(G4double val)
{
  thetaMax = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void G4CoulombScattering::SetQ2Max(G4double val)
{
  q2Max = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline std::vector<G4DynamicParticle*>* G4CoulombScattering::SecondariesPostStep(
                                                  G4VEmModel* model,
                                            const G4MaterialCutsCouple* couple,
                                            const G4DynamicParticle* dp)
{
  return model->SampleSecondaries(couple, dp);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

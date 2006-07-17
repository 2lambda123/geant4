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
// Calculation of the total, elastic and inelastic cross-sections
// based on parametrisations of (proton, pion, kaon, photon) nucleon
// cross-sections and the hadron-nucleous cross-section model in 
// the framework of Glauber-Gribov approach
//
//
//
//
//
// 17.07.06 V. Grichine - first implementation
//
//

#ifndef G4GlauberGribovCrossSection_h
#define G4GlauberGribovCrossSection_h

#include "globals.hh"
#include "G4Proton.hh"
#include "G4Nucleus.hh"

#include "G4VCrossSectionDataSet.hh"

class G4ParticleDefinition;

class G4GlauberGribovCrossSection : public G4VCrossSectionDataSet
{
public:

  G4GlauberGribovCrossSection ();
   
  virtual
  G4bool IsApplicable(const G4DynamicParticle* aDP, const G4Element*)
  {
    G4int baryonNumber = aDP->GetDefinition()->GetBaryonNumber();
    G4double kineticEnergy = aDP->GetKineticEnergy();
 
    if ( kineticEnergy / baryonNumber <= fUpperLimit ) return true;
                                                       return false;
  }

  virtual
  G4double GetCrossSection(const G4DynamicParticle*, 
                            const G4Element*, G4double aTemperature);

  virtual
  void BuildPhysicsTable(const G4ParticleDefinition&)
  {}

  virtual
  void DumpPhysicsTable(const G4ParticleDefinition&) 
  {G4cout << "G4GlauberGribovCrossSection: uses Glauber-Gribov formula"<<G4endl;}

  G4double GetHadronNucleaonXsc(const G4DynamicParticle*, const G4Element*);

  G4double CalculateEcmValue ( const G4double , const G4double , const G4double ); 

  G4double CalculateCeValue ( const G4double ); 

private:

  const G4double fUpperLimit;
  const G4double fLowerLimit; 
  const G4double fRadiusConst;

  G4double fTotalXsc, fElasticXsc, fInelasticXsc;
  G4double fHadronNucleonXsc;
 
  G4ParticleDefinition* theGamma;
  G4ParticleDefinition* theProton;
  G4ParticleDefinition* theNeutron;
  G4ParticleDefinition* theAProton;
  G4ParticleDefinition* theANeutron;
  G4ParticleDefinition* thePiPlus;
  G4ParticleDefinition* thePiMinus;
  G4ParticleDefinition* thePiZero;
  G4ParticleDefinition* theKPlus;
  G4ParticleDefinition* theKMinus;
  G4ParticleDefinition* theK0S;
  G4ParticleDefinition* theK0L;
  G4ParticleDefinition* theL;
  G4ParticleDefinition* theAntiL;
  G4ParticleDefinition* theSPlus;
  G4ParticleDefinition* theASPlus;
  G4ParticleDefinition* theSMinus;
  G4ParticleDefinition* theASMinus;
  G4ParticleDefinition* theS0;
  G4ParticleDefinition* theAS0;
  G4ParticleDefinition* theXiMinus;
  G4ParticleDefinition* theXi0;
  G4ParticleDefinition* theAXiMinus;
  G4ParticleDefinition* theAXi0;
  G4ParticleDefinition* theOmega;
  G4ParticleDefinition* theAOmega;
  G4ParticleDefinition* theD;
  G4ParticleDefinition* theT;
  G4ParticleDefinition* theA;
  G4ParticleDefinition* theHe3;

};

#endif

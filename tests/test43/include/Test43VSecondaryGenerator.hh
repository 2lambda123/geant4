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
// -------------------------------------------------------------
//      GEANT 4 class
//
//      ---------- Test43Material -------
//           created from test30 files originally by Vladimir Ivanchenko
//
//    Modified:
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Test43VSecondaryGenerator_Test43VSecondaryGenerator_h
#define Test43VSecondaryGenerator_Test43VSecondaryGenerator_h 1

#include <string>
#include <vector>
#include "globals.hh"
#include "G4LorentzVector.hh"
#include "G4Nucleus.hh"
#include "G4HadProjectile.hh"
#include "G4HadFinalState.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Track;
class G4ParticleDefinition;
class G4HadronicInteraction;
class G4Material;
class G4VParticleChange;
class G4ParticleChange;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Test43VSecondaryGenerator
{
public:

  Test43VSecondaryGenerator(G4HadronicInteraction* hadi, G4Material* mat);

  virtual ~Test43VSecondaryGenerator();

  virtual G4HadFinalState* Secondaries(const G4Track& track);

  const G4String GeneratorName() const {return generatorName;};

  G4double GetMass() {return mass;};

protected:

  G4String generatorName;

private:

  // hide assignment operator as private
  Test43VSecondaryGenerator(const Test43VSecondaryGenerator&);
  Test43VSecondaryGenerator& operator = (const Test43VSecondaryGenerator &right);

  G4HadronicInteraction* hInteraction;
  G4Material* material;
  G4Nucleus targetNucleus;
  G4double mass;
  G4HadFinalState* result;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif  // Test43VSecondaryGenerator_Test43VSecondaryGenerator_h


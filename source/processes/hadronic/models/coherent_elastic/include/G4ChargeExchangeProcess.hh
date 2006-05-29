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
// $Id: G4ChargeExchangeProcess.hh,v 1.1 2006-05-29 12:43:07 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Geant4 Hadron Elastic Charge Exchange Process -- header file
//
// Created 21 April 2006 V.Ivanchenko
//
// Modified:
//

// Class Description
// Process for hadron nuclear elastic scattering using optimal
// combination of Geant4 models
// Class Description - End

#ifndef G4ChargeExchangeProcess_h
#define G4ChargeExchangeProcess_h 1

#include "globals.hh"
#include "G4HadronicProcess.hh"
#include "G4Nucleus.hh"

class G4VQCrossSection;
class G4ParticleDefinition;
class G4CrossSectionDataStore;
class G4PhysicsLinearVector;

class G4ChargeExchangeProcess : public G4HadronicProcess
{
public:

  G4ChargeExchangeProcess(const G4String& procName = "chargeExc");

  virtual ~G4ChargeExchangeProcess();

  virtual G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
					  const G4Step& aStep);

  virtual G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

  virtual void BuildPhysicsTable(const G4ParticleDefinition& aParticleType);

  virtual void DumpPhysicsTable(const G4ParticleDefinition& aParticleType);

  virtual G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*);

  virtual G4double GetMicroscopicCrossSection(const G4DynamicParticle* aParticle,
					      const G4Element* anElement,
					      G4double aTemp);

  void SetQElasticCrossSection(G4VQCrossSection*);

private:

  G4VQCrossSection*           qCManager;

  const G4ParticleDefinition* theParticle;

  const G4ParticleDefinition* theProton;
  const G4ParticleDefinition* theNeutron;
  const G4ParticleDefinition* theAProton;
  const G4ParticleDefinition* theANeutron;
  const G4ParticleDefinition* thePiPlus;
  const G4ParticleDefinition* thePiMinus;
  const G4ParticleDefinition* thePiZero;
  const G4ParticleDefinition* theKPlus;
  const G4ParticleDefinition* theKMinus;
  const G4ParticleDefinition* theK0S;
  const G4ParticleDefinition* theK0L;
  const G4ParticleDefinition* theL;
  const G4ParticleDefinition* theAntiL;
  const G4ParticleDefinition* theSPlus;
  const G4ParticleDefinition* theASPlus;
  const G4ParticleDefinition* theSMinus;
  const G4ParticleDefinition* theASMinus;
  const G4ParticleDefinition* theS0;
  const G4ParticleDefinition* theAS0;
  const G4ParticleDefinition* theXiMinus;
  const G4ParticleDefinition* theXi0;
  const G4ParticleDefinition* theAXiMinus;
  const G4ParticleDefinition* theAXi0;
  const G4ParticleDefinition* theOmega;
  const G4ParticleDefinition* theAOmega;
  const G4ParticleDefinition* theD;
  const G4ParticleDefinition* theT;
  const G4ParticleDefinition* theA;
  const G4ParticleDefinition* theHe3;

  G4CrossSectionDataStore* store;
  G4Nucleus                targetNucleus;
  G4PhysicsLinearVector*   factors;

  G4double        xsec[40];
  G4double        cross;
  G4double        thEnergy;

  G4int    pPDG;
  G4bool   first;
};

#endif

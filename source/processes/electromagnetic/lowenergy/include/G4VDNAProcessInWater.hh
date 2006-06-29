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
//
// $Id: G4VDNAProcessInWater.hh,v 1.6 2006-06-29 19:37:03 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef   G4VDNAPROCESSINWATER_HH
#define  G4VDNAPROCESSINWATER_HH 1
 
#include "G4VLowEnergyTestableDiscreteProcess.hh"

// TotalCrossSectionPolicy must provide:
//  - [protected] const G4ParticleDefinition * IncomingParticleDefinition(void)
//  - [protected] G4double TotalCrossSection(G4double k, G4int z)
//  - [protected] void BuildTotalCrossSection(void)
 
// FinalStatesPolicy must provide:
//  - [protected] G4bool KillIncomingParticle(G4double k)
//  - [protected] void BuildFinalStatesData(void)
 
template<typename TotalCrossSectionPolicy, typename FinalStatesPolicy> 
class G4VDNAProcessInWater : public G4VLowEnergyTestableDiscreteProcess, public TotalCrossSectionPolicy, public FinalStatesPolicy
{
public:

  G4VDNAProcessInWater(const G4String& name) : G4VLowEnergyTestableDiscreteProcess(name) {}

  virtual ~G4VDNAProcessInWater() {}
 
  virtual G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);

  virtual void BuildPhysicsTable(const G4ParticleDefinition& aParticleDefinition);

  virtual G4bool IsApplicable(const G4ParticleDefinition& aParticleDefinition);

protected:

  void ValidateInWater(const G4Track& aTrack) const;

  virtual G4double GetMeanFreePath(const G4Track& aTrack, G4double previousStepSize, G4ForceCondition* condition);

private:

  // Hides default constructor and assignment operator as private 
  G4VDNAProcessInWater();
  G4VDNAProcessInWater& operator=(const G4VDNAProcessInWater & right);
};

#include "G4VDNAProcessInWater.icc"
#endif /* G4VDNAPROCESSINWATER_HH */


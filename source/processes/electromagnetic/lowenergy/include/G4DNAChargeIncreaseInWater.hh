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
// $Id: G4DNAChargeIncreaseInWater.hh,v 1.3 2006-06-29 19:33:44 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef   G4DNACHARGEINCREASEINWATER_HH
 #define  G4DNACHARGEINCREASEINWATER_HH 1

 #include "G4VDNAProcessInWater.hh"

 // TotalCrossSectionPolicy must provide:
 //  - [protected] const G4ParticleDefinition * IncomingParticleDefinition(void)
 //  - [protected] G4double TotalCrossSection(G4double k, G4int z)
 //  - [protected] void BuildTotalCrossSection(void)
 //  - [protected] G4int RandomizePartialCrossSection(G4double k, G4int z)

 // FinalStatesPolicy must provide:
 //  - [protected] G4bool KillIncomingParticle(G4double k)
 //  - [protected] void BuildFinalStatesData(void)
 //  - [protected] G4int NumberOfFinalStates(G4int finalStateIndex)
 //  - [protected] G4double OverallBindingEnergyConstant(G4int finalStateIndex)

 template<typename TotalCrossSectionPolicy, typename FinalStatesPolicy>
 class G4DNAChargeIncreaseInWater : public G4VDNAProcessInWater<TotalCrossSectionPolicy, FinalStatesPolicy>
 {
  public:
                                         G4DNAChargeIncreaseInWater(const G4String & name) : G4VDNAProcessInWater<TotalCrossSectionPolicy, FinalStatesPolicy>(name) {}
   virtual                              ~G4DNAChargeIncreaseInWater() {}

   virtual G4VParticleChange *           PostStepDoIt(const G4Track & aTrack, const G4Step & aStep);

  private:
   // Hides default constructor and assignment operator as private
                                         G4DNAChargeIncreaseInWater(const G4DNAChargeIncreaseInWater & copy);
   G4DNAChargeIncreaseInWater &              operator=(const G4DNAChargeIncreaseInWater & right);
 };

 #include "G4DNAChargeIncreaseInWater.icc"
#endif /* G4DNACHARGEINCREASEINWATER_HH */


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
// $Id: G4DNAHydrogenChargeIncrease.hh,v 1.2 2006-06-29 19:34:26 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef   G4DNAHYDROGENCHARGEINCREASE_HH
 #define  G4DNAHYDROGENCHARGEINCREASE_HH 1

 #include "G4DNAChargeIncreaseInWater.hh"
 #include "G4DNAHydrogenChargeIncreaseTotalCrossSectionPolicy.hh"
 #include "G4DNAHydrogenChargeIncreaseFinalStatesPolicy.hh"
 #include "G4DNAStopAndKillBelowEnergyLimitPolicy.hh"

 class G4DNAHydrogenChargeIncreaseEnergyLimitsPolicy
 {
  protected:
                      G4DNAHydrogenChargeIncreaseEnergyLimitsPolicy();

   const G4double     lowEnergyLimit;
   const G4bool       zeroBelowLowEnergyLimit;
   const G4double     highEnergyLimit;
   const G4bool       zeroAboveHighEnergyLimit;
 };

 class G4DNAHydrogenChargeIncreaseIncomingParticlePolicy
 {
  protected:
                                        G4DNAHydrogenChargeIncreaseIncomingParticlePolicy();
   const G4ParticleDefinition *         IncomingParticleDefinition(void) const;
 };

 class G4DNAHydrogenChargeIncrease : public G4DNAChargeIncreaseInWater<G4DNAHydrogenChargeIncreaseTotalCrossSectionPolicy<G4DNAHydrogenChargeIncreaseIncomingParticlePolicy, G4DNAHydrogenChargeIncreaseEnergyLimitsPolicy>, G4DNAHydrogenChargeIncreaseFinalStatesPolicy<G4DNAHydrogenChargeIncreaseEnergyLimitsPolicy> >
 {
  public:
                                         G4DNAHydrogenChargeIncrease(const G4String & name = "G4DNAHydrogenChargeIncrease") : G4DNAChargeIncreaseInWater<G4DNAHydrogenChargeIncreaseTotalCrossSectionPolicy<G4DNAHydrogenChargeIncreaseIncomingParticlePolicy, G4DNAHydrogenChargeIncreaseEnergyLimitsPolicy>, G4DNAHydrogenChargeIncreaseFinalStatesPolicy<G4DNAHydrogenChargeIncreaseEnergyLimitsPolicy> > (name) {}
   virtual                              ~G4DNAHydrogenChargeIncrease() {}
 };
#endif /* G4DNAHYDROGENCHARGEINCREASE_HH */

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
// $Id: G4DNAElectronExcitation.hh,v 1.3 2006-06-29 19:34:04 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef   G4DNAELECTRONEXCITATION_HH
 #define  G4DNAELECTRONEXCITATION_HH 1

 #include "G4DNAExcitationInWater.hh"
 #include "G4DNAEmfietzoglouExcitationTotalCrossSectionPolicy.hh"
 #include "G4DNAStopAndKillBelowEnergyLimitPolicy.hh"

 class G4DNAElectronExcitationEnergyLimitsPolicy
 {
  protected:
                      G4DNAElectronExcitationEnergyLimitsPolicy();

   const G4double     lowEnergyLimit;
   const G4bool       zeroBelowLowEnergyLimit;
   const G4double     highEnergyLimit;
   const G4bool       zeroAboveHighEnergyLimit;
 };

 class G4DNAElectronExcitationIncomingParticlePolicy
 {
  protected:
                                        G4DNAElectronExcitationIncomingParticlePolicy();
   const G4ParticleDefinition *         IncomingParticleDefinition(void) const;
 };

 class G4DNAElectronExcitation : public G4DNAExcitationInWater<G4DNAEmfietzoglouExcitationTotalCrossSectionPolicy<G4DNAElectronExcitationIncomingParticlePolicy, G4DNAElectronExcitationEnergyLimitsPolicy>, G4DNAStopAndKillBelowEnergyLimitPolicy<G4DNAElectronExcitationEnergyLimitsPolicy> >
 {
  public:
                                         G4DNAElectronExcitation(const G4String & name = "G4DNAElectronExcitation") : G4DNAExcitationInWater<G4DNAEmfietzoglouExcitationTotalCrossSectionPolicy<G4DNAElectronExcitationIncomingParticlePolicy, G4DNAElectronExcitationEnergyLimitsPolicy>, G4DNAStopAndKillBelowEnergyLimitPolicy<G4DNAElectronExcitationEnergyLimitsPolicy> > (name) {}
   virtual                              ~G4DNAElectronExcitation() {}
 };
#endif /* G4DNAELECTRONEXCITATION_HH */

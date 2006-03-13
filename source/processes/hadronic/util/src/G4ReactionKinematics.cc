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
//
//      CERN Geneva Switzerland
//
//      ------------ G4ReactionDynamics::TwoBody ------
//                     new method TwoBodyScattering
//                   by Christian V"olcker (CERN-Munich), September 1997
//                     E-mail: Christian.Volcker@cern.ch
// ************************************************************
//-----------------------------------------------------------------------------

#include "G4ReactionKinematics.hh"

// ************************************************************
void G4ReactionKinematics::TwoBodyScattering(
           const G4DynamicParticle* pIn1, const G4DynamicParticle* pIn2,
           G4DynamicParticle* pOut1, G4DynamicParticle* pOut2)
// ************************************************************
{           
// initial particles:

// - total invariant mass
   G4LorentzVector sumIn(pIn1->Get4Momentum()+pIn2->Get4Momentum());
   G4double invariantMass=sumIn.mag();

// - beta of center-of-mass system
   G4ThreeVector betaCMS=sumIn.boostVector();

// final particles:

// - get final particle masses
   G4double massOut1=pOut1->GetMass();
   G4double massOut2=pOut2->GetMass();

// - calculate breakup momentum:
   G4double breakupMomentum=BreakupMomentum(invariantMass, massOut1, massOut2);

// - isotropic decay angle
   G4double costheta = 2.0*G4UniformRand() - 1.0;
   G4double sintheta = std::sqrt(1.0 - costheta*costheta);
   G4double phi = 2.0*pi*G4UniformRand();

// - setup LorentzVectors
   G4double pz=costheta*breakupMomentum;
   G4double px=sintheta*std::cos(phi)*breakupMomentum;
   G4double py=sintheta*std::sin(phi)*breakupMomentum;
   
   G4double breakupMomentumSquared=breakupMomentum*breakupMomentum;
   G4double energy1=std::sqrt(breakupMomentumSquared+massOut1*massOut1);
   G4double energy2=std::sqrt(breakupMomentumSquared+massOut2*massOut2);

   G4LorentzVector lorentz1(px, py, pz, energy1);
   G4LorentzVector lorentz2(px, py, pz, energy2);

// - back into lab system

   lorentz1.boost(betaCMS);
   lorentz2.boost(betaCMS);

// fill in new particles:

   pOut1->Set4Momentum(lorentz1);
   pOut2->Set4Momentum(lorentz2);

   return;
}

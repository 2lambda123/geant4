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
 // Hadronic Process: Light Media Charge and/or Strangeness Exchange
 // J.L. Chuma, TRIUMF, 21-Feb-1997
 // Last modified: 21-Feb-1997

#ifndef G4LightMedia_h
#define G4LightMedia_h 1
 
#include "globals.hh"
#include "G4DynamicParticle.hh"
#include "G4Nucleus.hh"
#include "G4ParticleTypes.hh" 
 
 class G4LightMedia
 {
 public:
    
    G4LightMedia() { }

    G4LightMedia( const G4LightMedia &right )
    { *this = right; }
    
    ~G4LightMedia() { }
    
    G4LightMedia & operator=( const G4LightMedia &)
    { return *this; }
    
    G4bool operator==( const G4LightMedia &right ) const
    { return ( this == (G4LightMedia *) &right ); }
    
    G4bool operator!=( const G4LightMedia &right ) const
    { return ( this != (G4LightMedia *) &right ); }
    
    G4DynamicParticle * PionPlusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * PionMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * KaonPlusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * KaonZeroShortExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * KaonZeroLongExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * KaonMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * ProtonExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiProtonExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * NeutronExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiNeutronExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * LambdaExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiLambdaExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * SigmaPlusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * SigmaMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiSigmaPlusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiSigmaMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * XiZeroExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * XiMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiXiZeroExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiXiMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * OmegaMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
    G4DynamicParticle * AntiOmegaMinusExchange(
     const G4HadProjectile *incidentParticle,
     const  G4Nucleus &aNucleus );
    
 private:
    
 };
 
#endif
 

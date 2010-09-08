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
// $Id: G4LowEIonFragmentation.cc,v 1.7 2010-09-08 16:38:09 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4LowEIonFragmentation
//
// Author:  H.P. Wellisch
//
// Modified:
// 02 Jun 2010 M. A. Cortes Giraldo fix: particlesFromTarget must be 
//                     accounted for as particles of initial compound nucleus

#include "G4LowEIonFragmentation.hh"
#include <algorithm>

G4int G4LowEIonFragmentation::hits = 0;
G4int G4LowEIonFragmentation::totalTries = 0;
G4double G4LowEIonFragmentation::area = 0;

G4LowEIonFragmentation::G4LowEIonFragmentation(G4ExcitationHandler * const value) 
{
  theHandler = value;
  theModel = new G4PreCompoundModel(theHandler);
}

G4LowEIonFragmentation::G4LowEIonFragmentation() 
{
  theHandler = new G4ExcitationHandler;
  theModel = new G4PreCompoundModel(theHandler);
}

G4LowEIonFragmentation::~G4LowEIonFragmentation() 
{
  delete theModel;
}

G4HadFinalState * G4LowEIonFragmentation::
ApplyYourself(const G4HadProjectile & thePrimary, G4Nucleus & theNucleus)
{
  area = 0;
  // initialize the particle change
  theResult.Clear();
  theResult.SetStatusChange( stopAndKill );
  theResult.SetEnergyChange( 0.0 );

  // Get Target A, Z
  G4int aTargetA = theNucleus.GetA_asInt();
  G4int aTargetZ = theNucleus.GetZ_asInt();

  // Get Projectile A, Z
  G4int aProjectileA = thePrimary.GetDefinition()->GetBaryonNumber();
  G4int aProjectileZ = G4lrint(thePrimary.GetDefinition()->GetPDGCharge());

  // Get Maximum radius of both
  
  G4Fancy3DNucleus aPrim;
  aPrim.Init(aProjectileA, aProjectileZ);
  G4double projectileOuterRadius = aPrim.GetOuterRadius();
  
  G4Fancy3DNucleus aTarg;
  aTarg.Init(aTargetA, aTargetZ);
  G4double targetOuterRadius = aTarg.GetOuterRadius();

  // Get the Impact parameter
  G4int particlesFromProjectile = 0;
  G4int chargedFromProjectile = 0;
  G4double impactParameter = 0;
  G4double x,y;
  G4Nucleon * pNucleon;
  // need at lease one particle from the projectile model beyond the 
  // projectileHorizon.
  while(0==particlesFromProjectile)
  {
    do
    {
      x = 2*G4UniformRand() - 1;
      y = 2*G4UniformRand() - 1;
    }
    while(x*x + y*y > 1);
    impactParameter = std::sqrt(x*x+y*y)*(targetOuterRadius+projectileOuterRadius);
    totalTries++;
    area = pi*(targetOuterRadius+projectileOuterRadius)*
              (targetOuterRadius+projectileOuterRadius);
    G4double projectileHorizon = impactParameter-targetOuterRadius; 
    
    // Empirical boundary transparency.
    G4double empirical = G4UniformRand();
    if(projectileHorizon/projectileOuterRadius>empirical) continue;
    
    // Calculate the number of nucleons involved in collision
    // From projectile
    aPrim.StartLoop();
    while((pNucleon = aPrim.GetNextNucleon()))
    {
      if(pNucleon->GetPosition().y()>projectileHorizon)
      {
        // We have one
        particlesFromProjectile++;
        if(pNucleon->GetParticleType()==G4Proton::ProtonDefinition()) 
        {
          chargedFromProjectile++;
        } 
      }
    }
  }
  hits++;

  // From target:
  G4double targetHorizon = impactParameter-projectileOuterRadius;
  G4int chargedFromTarget = 0;
  G4int particlesFromTarget = 0;
  aTarg.StartLoop();  
  while((pNucleon = aTarg.GetNextNucleon()))
  {
    if(pNucleon->GetPosition().y()>targetHorizon)
    {
      // We have one
      particlesFromTarget++;
      if(pNucleon->GetParticleType()==G4Proton::ProtonDefinition()) 
      {
        chargedFromTarget++;
      }
    }
  }
  
  // Energy sharing between projectile and target. Note that this is a quite simplistic kinetically.
  G4ThreeVector exciton3Momentum = thePrimary.Get4Momentum().vect();
  exciton3Momentum *= particlesFromProjectile/aProjectileA;
  
  G4double compoundEnergy = thePrimary.GetTotalEnergy()*particlesFromProjectile/aProjectileA;  
  G4double targetMass = G4ParticleTable::GetParticleTable()
                        ->GetIonTable()->GetIonMass(aTargetZ, aTargetA);
  compoundEnergy += targetMass;
  G4LorentzVector fragment4Momentum(exciton3Momentum, compoundEnergy);
 
  // take the nucleons and fill the Fragments
  G4Fragment anInitialState;
  anInitialState.SetZandA_asInt(aTargetZ+chargedFromProjectile,
  				aTargetA+particlesFromProjectile);
  // M.A. Cortes fix
  //anInitialState.SetNumberOfParticles(particlesFromProjectile);
  anInitialState.SetNumberOfParticles(particlesFromProjectile+particlesFromTarget);
  anInitialState.SetNumberOfHoles(particlesFromTarget);
  anInitialState.SetNumberOfCharged(chargedFromProjectile + chargedFromTarget);
  anInitialState.SetMomentum(fragment4Momentum);

  // Fragment the Fragment using Pre-compound
  G4ReactionProductVector* thePreCompoundResult;
  thePreCompoundResult = theModel->DeExcite(anInitialState);
  
  // De-excite the projectile using ExcitationHandler
  
  G4ReactionProductVector * theExcitationResult = 0; 
  if(particlesFromProjectile != aProjectileA)
  {
    G4ThreeVector residual3Momentum = thePrimary.Get4Momentum().vect();
    residual3Momentum -= exciton3Momentum;
    G4double residualEnergy = thePrimary.GetTotalEnergy()*(1.-particlesFromProjectile/aProjectileA);
    G4LorentzVector residual4Momentum(residual3Momentum, residualEnergy);  
 
    G4Fragment initialState2;
    initialState2.SetZandA_asInt(aProjectileZ-chargedFromProjectile,
    				 aProjectileA-particlesFromProjectile);
    initialState2.SetNumberOfHoles(static_cast<G4int>((aProjectileA-particlesFromProjectile)/2.0));
    initialState2.SetNumberOfParticles(static_cast<G4int>((aProjectileZ-chargedFromProjectile)/2.0));
    initialState2.SetNumberOfCharged(static_cast<G4int>((aProjectileZ-chargedFromProjectile)/2.0));


    initialState2.SetMomentum(residual4Momentum);
    theExcitationResult = theHandler->BreakItUp(initialState2);
  }

  // Fill the particle change
  G4int nSecondaries = 0;
  if(theExcitationResult) nSecondaries+=theExcitationResult->size();
  if(thePreCompoundResult) nSecondaries+=thePreCompoundResult->size();
  
  unsigned int k;
  if(theExcitationResult!=0)
  {
    for(k=0; k<theExcitationResult->size(); k++)
    {
      G4DynamicParticle* p0 = new G4DynamicParticle;
      p0->SetDefinition( theExcitationResult->operator[](k)->GetDefinition() );
      p0->SetMomentum( theExcitationResult->operator[](k)->GetMomentum() );
      theResult.AddSecondary(p0);
    }
  }
  
  for(k=0; k<thePreCompoundResult->size(); k++)
  {
    G4DynamicParticle* p0 = new G4DynamicParticle;
    p0->SetDefinition(thePreCompoundResult->operator[](k)->GetDefinition());
    p0->SetMomentum(thePreCompoundResult->operator[](k)->GetMomentum());
    theResult.AddSecondary(p0);
  }
  
  // clean up
  std::for_each(thePreCompoundResult->begin(), thePreCompoundResult->end(), DeleteReactionProduct());
  if(theExcitationResult) 
  {
    std::for_each(theExcitationResult->begin(), theExcitationResult->end(), DeleteReactionProduct());
  }
  delete thePreCompoundResult;
  if(theExcitationResult) delete theExcitationResult;

  // return the particle change
  return &theResult;
  
}

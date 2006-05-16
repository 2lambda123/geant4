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
 // Hadronic Process: Triton Inelastic Process
 // J.L. Chuma, TRIUMF, 25-Feb-1997
 // Last modified: 27-Mar-1997
 // J.L. Chuma, 08-May-2001: Update original incident passed back in vec[0]
 //                          from NuclearReaction
 //
#include "G4LETritonInelastic.hh"
#include "Randomize.hh"
#include "G4Electron.hh"
 
 G4HadFinalState *
  G4LETritonInelastic::ApplyYourself( const G4HadProjectile &aTrack,
                                      G4Nucleus &targetNucleus )
  {
    bool triton_debug = false;
    if(getenv("TritonLEDebug")) triton_debug = true;
    theParticleChange.Clear();
    const G4HadProjectile *originalIncident = &aTrack;
    if(triton_debug)  std::cout << "entering LETritonInelastic "<<originalIncident->GetKineticEnergy()<<std::endl;
    if (originalIncident->GetKineticEnergy()<= 0.1*MeV) 
    {
      theParticleChange.SetStatusChange(isAlive);
      theParticleChange.SetEnergyChange(aTrack.GetKineticEnergy());
      theParticleChange.SetMomentumChange(aTrack.Get4Momentum().vect().unit()); 
      return &theParticleChange;      
    }
    
    if( verboseLevel > 1 )
    {
      const G4Material *targetMaterial = aTrack.GetMaterial();
      G4cout << "G4LETritonInelastic::ApplyYourself called" << G4endl;
      G4cout << "kinetic energy = " << originalIncident->GetKineticEnergy()/MeV << "MeV, ";
      G4cout << "target material = " << targetMaterial->GetName() << ", ";
    }
    
    if(triton_debug)std::cout << "running LETritonInelastic 1"<<std::endl;
    // Work-around for lack of model above 100 MeV
    if (originalIncident->GetKineticEnergy()/MeV > 100. ||
        originalIncident->GetKineticEnergy() <= 0.) 
      {
        theParticleChange.SetStatusChange(isAlive);
        theParticleChange.SetEnergyChange(aTrack.GetKineticEnergy());
        theParticleChange.SetMomentumChange(aTrack.Get4Momentum().vect().unit()); 
        return &theParticleChange;
      }

    if(triton_debug)std::cout << "running LETritonInelastic 2"<<std::endl;
    G4double N = targetNucleus.GetN();
    G4double Z = targetNucleus.GetZ();
    G4double theAtomicMass = targetNucleus.AtomicMass( N, Z );
    G4double massVec[9];
    massVec[0] = targetNucleus.AtomicMass( N+3.0, Z+1.0 );
    massVec[1] = targetNucleus.AtomicMass( N+2.0, Z+1.0 );
    massVec[2] = targetNucleus.AtomicMass( N+2.0, Z     );
    massVec[3] = targetNucleus.AtomicMass( N+1.0, Z     );
    massVec[4] = theAtomicMass;
    massVec[5] = targetNucleus.AtomicMass( N-1.0, Z-1.0 );
    massVec[6] = targetNucleus.AtomicMass( N+1.0, Z+1.0 );
    massVec[7] = massVec[3];
    massVec[8] = targetNucleus.AtomicMass( N+1.0, Z-1.0 );
    
    G4FastVector<G4ReactionProduct,4> vec;  // vec will contain the secondary particles
    G4int vecLen = 0;
    vec.Initialize( 0 );
    
    if(triton_debug)std::cout << "running LETritonInelastic 3"<<std::endl;
    theReactionDynamics.NuclearReaction( vec, vecLen, originalIncident,
                                         targetNucleus, theAtomicMass, massVec );
    if(triton_debug)std::cout << "running LETritonInelastic 4"<<std::endl;
    //
    G4double p = vec[0]->GetMomentum().mag();
    theParticleChange.SetMomentumChange( vec[0]->GetMomentum()*(1./p) );
    theParticleChange.SetEnergyChange( vec[0]->GetKineticEnergy() );
    delete vec[0];
    //
    G4DynamicParticle *pd;
    for( G4int i=1; i<vecLen; ++i )
    {
      pd = new G4DynamicParticle();
      pd->SetDefinition( vec[i]->GetDefinition() );
      pd->SetMomentum( vec[i]->GetMomentum() );
      theParticleChange.AddSecondary( pd );
      delete vec[i];
    }
    
    if(triton_debug)std::cout << "leaving LETritonInelastic"<<std::endl;
    return &theParticleChange;
  }
 
 /* end of file */
 

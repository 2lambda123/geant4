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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4HEAntiSigmaZeroInelastic.cc,v 1.6 2001-08-01 17:09:53 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

#include "globals.hh"
#include "G4ios.hh"

//
// G4 Process: Gheisha High Energy Collision model.
// This includes the high energy cascading model, the two-body-resonance model
// and the low energy two-body model. Not included are the low energy stuff like
// nuclear reactions, nuclear fission without any cascading and all processes for
// particles at rest.  
// First work done by J.L.Chuma and F.W.Jones, TRIUMF, June 96.  
// H. Fesefeldt, RWTH-Aachen, 23-October-1996
// Last modified: 29-July-1998 
 
#include "G4HEAntiSigmaZeroInelastic.hh"
#include "G4Gamma.hh"

G4VParticleChange * G4HEAntiSigmaZeroInelastic::
ApplyYourself( const G4Track &aTrack, G4Nucleus &targetNucleus )
  {
    G4HEVector * pv = new G4HEVector[MAXPART];
    theParticleChange.Initialize( aTrack );
    const G4DynamicParticle *aParticle = aTrack.GetDynamicParticle();
    G4HEVector incidentParticle(aParticle);
    G4HEAntiLambdaInelastic theAntiLambdaInelastic;
    theAntiLambdaInelastic.SetMaxNumberOfSecondaries(MAXPART);
    theAntiLambdaInelastic.SetVerboseLevel(verboseLevel);
    
    G4double incidentTotalMomentum = incidentParticle.getTotalMomentum();
    G4double pgam = G4UniformRand()*incidentTotalMomentum*0.75;
    G4HEVector incidentAntiLambda; 
    incidentAntiLambda.SmulAndUpdate( incidentParticle, 
                                     (incidentTotalMomentum - pgam)/incidentTotalMomentum);                   
    G4DynamicParticle * aLambda = new G4DynamicParticle();
    aLambda->SetDefinition(G4AntiLambda::AntiLambda());
    G4Track aLambdaTrack(aLambda, 0, aTrack.GetPosition());
    aLambda->SetMomentum(incidentAntiLambda.getMomentum());
    G4VParticleChange * result = theAntiLambdaInelastic.ApplyYourself(aLambdaTrack, targetNucleus);         	
    vecLength = theAntiLambdaInelastic.GetNumberOfSecondaries();
    pv[vecLength] = Gamma;
    pv[vecLength].setMomentum(incidentParticle.getMomentum());
    pv[vecLength].SmulAndUpdate( pv[vecLength],pgam/incidentTotalMomentum); 
    G4DynamicParticle * aPhoton = new G4DynamicParticle();
    aPhoton->SetDefinition(G4Gamma::Gamma());
    aPhoton->SetMomentum(pv[vecLength].getMomentum());
    G4Track * aGammaTrack = new G4Track(aPhoton, aTrack.GetGlobalTime(), aTrack.GetPosition());
    result->AddSecondary(aGammaTrack);
      delete [] pv;
    return result;
  } 
        









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
// $Id: ExN05PiModel.cc,v 1.4 2002-01-09 17:24:20 ranjard Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "ExN05PiModel.hh"

#include "G4PionMinus.hh"
#include "G4PionPlus.hh"
#include "G4Gamma.hh"

ExN05PiModel::ExN05PiModel(G4Envelope *anEnvelope) :
  G4VFastSimulationModel("ExN05PiModel",anEnvelope)
{;}

ExN05PiModel::~ExN05PiModel()
{;}

G4bool ExN05PiModel::IsApplicable(const G4ParticleDefinition& particleType)
{
  return
    &particleType == G4PionMinus::PionMinusDefinition() ||
    &particleType == G4PionPlus::PionPlusDefinition();
}

G4bool ExN05PiModel::ModelTrigger(const G4FastTrack& fastTrack) {
  //-------------------------------------------------------------
  // UserTrigger() method: method which has to decide if
  // the parameterisation has to be applied.
  // Here ModelTrigger() asks the user (ie you) a 0/1 answer.
  //
  // Note that quantities like the local/global position/direction etc..
  // are available at this level via the fastTrack parameter (allowing 
  // to check distance from boundaries, see below  to allow the decision)
  //--------------------------------------------------------------
  G4cout << "\nExN05PiModel::ModelTrigger() called:" << G4endl;
  G4cout <<   "--------------------------------" << G4endl;
  G4cout << "(particle is a " << fastTrack.GetPrimaryTrack()->
    GetDefinition()->GetParticleName() << " )\n" << G4endl;

  // -- Examples of available informations:

  // -- position:
  G4cout << "         Track position: " <<
    fastTrack.GetPrimaryTrack()->GetPosition()  << "(global coord.)"   <<
    fastTrack.GetPrimaryTrackLocalPosition()  << "(in envelope coord.)" 
       << G4endl;

  // -- direction:
  G4cout << "         Track direction:"       <<
    fastTrack.GetPrimaryTrack()->GetMomentum().unit() << 
    "(global coord.)"      <<
    fastTrack.GetPrimaryTrackLocalDirection() << "(in envelope coord.)" << 
    G4endl;

  return true;
}

void ExN05PiModel::DoIt(const G4FastTrack& fastTrack, 
		     G4FastStep& fastStep)
  //--------------------------------------------------
  //
  // User method to code the parameterisation properly
  // said.
  //
  //--------------------------------------------------
{

  //------------------------------------------------
  // The primary track continues along its direction.
  // One secondary (a photon) is added:
  //------------------------------------------------
  G4cout << "      Pion `model' applied\n" << G4endl;

  //------------------------------
  // Primary:
  //    idem as in "DefaultModel":
  //
  //------------------------------
  G4ThreeVector position;
  G4double distance;
  distance = fastTrack.GetEnvelopeSolid()->
    DistanceToOut(fastTrack.GetPrimaryTrackLocalPosition(),
		  fastTrack.GetPrimaryTrackLocalDirection());
  position = fastTrack.GetPrimaryTrackLocalPosition() + 
    distance*fastTrack.GetPrimaryTrackLocalDirection();

  // -- set final position:
  fastStep.SetPrimaryTrackFinalPosition(position);

  //---------------------------
  // Secondary:
  //   Adds one "secondary":
  //
  //---------------------------
  // -- First, user has to say how many secondaries will be created:
  fastStep.SetNumberOfSecondaryTracks(1);

  //------------------------
  // -- Build the secondary:
  //------------------------
  // -- direction:
  G4ParticleMomentum direction(fastTrack.GetPrimaryTrackLocalDirection());
  direction.setZ(direction.z()*0.5);
  direction.setY(direction.y()+direction.z()*0.1);
  direction = direction.unit(); // necessary ?

  // -- dynamics (Note that many constructors exists for G4DynamicParticle
  // -- see prototype/particle+matter/particles/management/include/G4DynamicParticle.hh)
  G4DynamicParticle dynamique(G4Gamma::GammaDefinition(),
			      direction,
			      fastTrack.GetPrimaryTrack()->
			      GetKineticEnergy()/2.);
  // -- position:
  G4double Dist;
  Dist = fastTrack.GetEnvelopeSolid()->
    DistanceToOut(fastTrack.GetPrimaryTrackLocalPosition(),
		  direction);
  G4ThreeVector posi;
  posi = fastTrack.GetPrimaryTrackLocalPosition() + Dist*direction;
  
  //------------------------------------
  //-- Creation of the secondary Track:
  //------------------------------------
  fastStep.CreateSecondaryTrack(dynamique, posi, 
		       fastTrack.GetPrimaryTrack()->GetGlobalTime());

}

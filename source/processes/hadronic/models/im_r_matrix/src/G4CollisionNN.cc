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
// $Id: G4CollisionNN.cc,v 1.3 2003-12-12 11:33:11 hpw Exp $ //


#include "globals.hh"
#include "G4CollisionNN.hh"
#include "G4CollisionComposite.hh"
#include "G4VCollision.hh"
#include "G4CollisionVector.hh"
#include "G4KineticTrack.hh"
#include "G4VCrossSectionSource.hh"
#include "G4XNNTotal.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4CollisionNNElastic.hh"
#include "G4CollisionnpElastic.hh"
#include "G4CollisionNNToNDelta.hh"
#include "G4CollisionNNToDeltaDelta.hh"
#include "G4CollisionNNToNDeltastar.hh"
#include "G4CollisionNNToDeltaDeltastar.hh"
#include "G4CollisionNNToNNstar.hh"
#include "G4CollisionNNToDeltaNstar.hh"
#include "G4Pair.hh"

typedef GROUP8(G4CollisionnpElastic, G4CollisionNNElastic, 
              G4CollisionNNToNDelta, G4CollisionNNToDeltaDelta, 
	      G4CollisionNNToNDeltastar, G4CollisionNNToDeltaDeltastar,
	      G4CollisionNNToNNstar, G4CollisionNNToDeltaNstar) theChannels;

G4CollisionNN::G4CollisionNN()
{ 
  crossSectionSource = new G4XNNTotal();
  G4ForEach<theChannels, G4CollisionComposite::Register>::Apply(this);
}


G4CollisionNN::~G4CollisionNN()
{ 
  delete crossSectionSource;
  crossSectionSource = 0;
}


const std::vector<G4String>& G4CollisionNN::GetListOfColliders(G4int ) const
{
	  throw G4HadronicException(__FILE__, __LINE__, "G4CollisionNN::GetListOfColliders - Argument outside valid range"); 
	  return colliders1;
}


G4double G4CollisionNN::CrossSection(const G4KineticTrack& aTrk1, 
				    const G4KineticTrack& aTrk2) const
{
  G4double sigma = 0.;

  // nucleon-nucleon cross-sections made for on-shell particles.
  // here we take the kinetic energy as the quantity relevant
  // for calculating the scattering cross-sections for off-shell hadrons
  
  const G4VCrossSectionSource* xSource = GetCrossSectionSource();
  G4LorentzVector p1 = aTrk1.Get4Momentum();
  G4LorentzVector p2 = aTrk2.Get4Momentum();
  G4double t1 = p1.e()-aTrk1.GetActualMass();
  G4double t2 = p2.e()-aTrk2.GetActualMass();
  p1.setE(t1+aTrk1.GetDefinition()->GetPDGMass());
  p2.setE(t2+aTrk2.GetDefinition()->GetPDGMass());
  G4KineticTrack trk1(aTrk1);
  trk1.Set4Momentum(p1);
  G4KineticTrack trk2(aTrk2);
  trk2.Set4Momentum(p2);
  if( (p1+p2).mag()<aTrk1.GetDefinition()->GetPDGMass()+aTrk2.GetDefinition()->GetPDGMass())
  {
    return 0.;
  }

  if (xSource != 0)
    {
      // There is a cross section for this Collision
      sigma = xSource->CrossSection(trk1,trk2);
    }
  return sigma;
}


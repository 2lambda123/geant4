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
#ifndef G4GeneralNNCollision_h
#define G4GeneralNNCollision_h

#include "G4CollisionComposite.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4HadParticleCodes.hh"
#include "G4Pair.hh"

class G4GeneralNNCollision : public G4CollisionComposite
{
  public:

  G4bool 
  IsInCharge(const G4KineticTrack& trk1, const G4KineticTrack& trk2) const
  {
    G4bool result = false;
    G4ParticleDefinition * aD = trk1.GetDefinition();
    G4ParticleDefinition * bD = trk2.GetDefinition();
    if(  (aD==G4Proton::Proton() || aD == G4Neutron::Neutron())
       &&(bD==G4Proton::Proton() || bD == G4Neutron::Neutron()) ) result = true;
    return result;
  }
  
  protected:

  template <int dm, int d0, int dp, int dpp, class channelType> 
  void MakeNNToNDelta(G4CollisionComposite * aC)
  {
    typedef INT4(channelType, NeutronPC, NeutronPC, NeutronPC, d0)  theC1;
    typedef INT4(channelType, NeutronPC, NeutronPC, ProtonPC,  dm)  theC2;
    typedef INT4(channelType, NeutronPC, ProtonPC,  ProtonPC,  d0)  theC3;
    typedef INT4(channelType, NeutronPC, ProtonPC,  NeutronPC, dp)  theC4;
    typedef INT4(channelType, ProtonPC,  ProtonPC,  NeutronPC, dpp) theC5;
    typedef INT4(channelType, ProtonPC,  ProtonPC,  ProtonPC,  dp)  theC6;
    typedef GROUP6(theC1, theC2, theC3, theC4, theC5, theC6) theChannels;
    G4ForEach<theChannels, G4CollisionComposite::Resolve>::Apply(aC); 
  }

  template <int Np, int Nn, class channelType> 
  void MakeNNToNNStar(G4CollisionComposite * aC)
  {
    typedef INT4(channelType, NeutronPC, NeutronPC, NeutronPC, Nn)  theC1;
    typedef INT4(channelType, ProtonPC,  ProtonPC,  ProtonPC,  Np)  theC2;
    typedef INT4(channelType, NeutronPC, ProtonPC,  NeutronPC, Np)  theC3;
    typedef INT4(channelType, NeutronPC, ProtonPC,  ProtonPC,  Nn)  theC4;
    typedef GROUP4(theC1, theC2, theC3, theC4) theChannels;
    G4ForEach<theChannels, G4CollisionComposite::Resolve>::Apply(aC); 
  }
  
  template <class channelType, int Np, int Nn> 
  void MakeNNStarToNN(G4CollisionComposite * aC)
  {
    typedef INT4(channelType, Nn, NeutronPC, NeutronPC, NeutronPC)  theC1;
    typedef INT4(channelType, Np, ProtonPC,  ProtonPC,  ProtonPC)   theC2;
    typedef INT4(channelType, Np, NeutronPC, NeutronPC, ProtonPC)   theC3;
    typedef INT4(channelType, Nn, ProtonPC, NeutronPC, ProtonPC)    theC4;
    typedef GROUP4(theC1, theC2, theC3, theC4) theChannels;
    G4ForEach<theChannels, G4CollisionComposite::Resolve>::Apply(aC); 
  }
  
  template <int Np, class channelType, int Nn> 
  void MakeNNToDeltaNstar(G4CollisionComposite * aC)
  {
    typedef INT4(channelType, NeutronPC, NeutronPC, D1232::D0,  Nn)  theC1;
    typedef INT4(channelType, NeutronPC, NeutronPC, D1232::Dm,  Np)  theC2;
    typedef INT4(channelType, ProtonPC,  ProtonPC,  D1232::Dp,  Np)  theC3;
    typedef INT4(channelType, ProtonPC,  ProtonPC,  D1232::Dpp, Nn)  theC4;
    typedef INT4(channelType, NeutronPC, ProtonPC,  D1232::D0,  Np)  theC5;
    typedef INT4(channelType, NeutronPC, ProtonPC,  D1232::Dp,  Nn)  theC6;
    typedef GROUP6(theC1, theC2, theC3, theC4, theC5, theC6) theChannels;
    G4ForEach<theChannels, G4CollisionComposite::Resolve>::Apply(aC); 
  }
  
  template <int dm, int d0, int dp, int dpp, class channelType> 
  void MakeNNToDeltaDelta(G4CollisionComposite * aC)
  {
    typedef INT4(channelType, NeutronPC, NeutronPC, DeltamPC, dp)  theC1;
    typedef INT4(channelType, NeutronPC, NeutronPC, Delta0PC, d0)  theC2;
    typedef INT4(channelType, NeutronPC, NeutronPC, DeltapPC, dm)  theC3;
    typedef INT4(channelType, NeutronPC, ProtonPC, DeltapPC, d0)  theC4;
    typedef INT4(channelType, NeutronPC, ProtonPC, Delta0PC, dp)  theC5;
    typedef INT4(channelType, NeutronPC, ProtonPC, DeltamPC, dpp)  theC6;
    typedef INT4(channelType, NeutronPC, ProtonPC, DeltappPC, dm)  theC7;
    typedef INT4(channelType, ProtonPC, ProtonPC, Delta0PC, dpp)  theC8;
    typedef INT4(channelType, ProtonPC, ProtonPC, DeltapPC, dp)  theC9;
    typedef INT4(channelType, ProtonPC, ProtonPC, DeltappPC, d0)  theC10;
    typedef GROUP10(theC1, theC2, theC3, theC4, theC5, theC6, theC7, theC8, theC9, theC10) theChannels;
    G4ForEach<theChannels, G4CollisionComposite::Resolve>::Apply(aC); 
  }
};

#endif

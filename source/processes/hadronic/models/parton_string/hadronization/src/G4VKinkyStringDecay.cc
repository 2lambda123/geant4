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
// $Id: G4VKinkyStringDecay.cc,v 1.1 2003-10-07 11:25:41 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//  Maxim Komogorov
//
// -----------------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation, Maxim Komogorov, 10-Oct-1998
// -----------------------------------------------------------------------------

#include "G4VKinkyStringDecay.hh"
#include "G4KineticTrackVector.hh"
#include "G4KineticTrack.hh"
#include "Randomize.hh"

//*****************************************************************************************************

G4VKinkyStringDecay::G4VKinkyStringDecay(G4VLongitudinalStringDecay* theModal)
   {
   this->SetLongitudinalStringDecay(theModal);
   }

//*****************************************************************************************************

G4double G4VKinkyStringDecay::GetLightConeGluonZ(G4double zmin, G4double zmax)
    {
    G4double z, yf;
    do {
       z = zmin + G4UniformRand()*(zmax-zmin);
       yf = z*z +sqr(1 - z);	
       } 
    while (G4UniformRand() > yf); 
    return z;
    }

//*****************************************************************************************************

G4KineticTrackVector* G4VKinkyStringDecay::FragmentString(const G4ExcitedString& String) 
    {
    G4LorentzVector Mom = String.GetGluon()->Get4Momentum();
    G4ThreeVector Pos = String.GetGluon()->GetPosition();
    G4int QuarkEncoding = theLongitudinalStringDecay->SampleQuarkFlavor();
    G4ThreeVector Pquark=theLongitudinalStringDecay->SampleQuarkPt();
    G4double Pt2 = Pquark.mag2();
    G4double z = GetLightConeGluonZ(0, 1);
    G4double w = Mom.e() + Mom.pz();
    //... now compute quark longitudinal momentum and energy

    Pquark.setZ(  (z*w - Pt2/(z*w))*0.5);
    G4double E  = (z*w + Pt2/(z*w))*0.5;
    
    G4Parton* AntiColor = new G4Parton(-QuarkEncoding);
    AntiColor->SetPosition(Pos);
    G4LorentzVector AntiColorMom(-Pquark, E);
    AntiColor->Set4Momentum(AntiColorMom);
    G4Parton* Color = new G4Parton(*String.GetColorParton());
    G4ExcitedString Str1(Color, AntiColor, String.GetDirection());
    G4KineticTrackVector* KTV1 = theLongitudinalStringDecay->FragmentString(Str1);

    Color = new G4Parton(QuarkEncoding);
    Color->SetPosition(Pos);
    G4LorentzVector ColorMom(Pquark, E);
    Color->Set4Momentum(ColorMom);
    AntiColor = new G4Parton(*String.GetAntiColorParton());
    G4ExcitedString Str2(Color, AntiColor, String.GetDirection());
    G4KineticTrackVector* KTV2 = theLongitudinalStringDecay->FragmentString(Str2);

    if (KTV1 && KTV2)
         while(!KTV2->empty())
	 {
             KTV1->push_back(KTV2->back());
	     KTV1->erase(KTV1->end()-1);
	 }
    return KTV1;            
    } 
 
//*****************************************************************************************************
 
 
 
 
 
 
 
 
 
 
 
 
 
 

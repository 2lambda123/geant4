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
// $Id: G4VPartonStringModel.cc,v 1.1 2003-10-07 11:26:00 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//// ------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      ---------------- G4VPartonStringModel ----------------
//             by Gunter Folger, May 1998.
//      abstract class for all Parton String Models
// ------------------------------------------------------------


#include "G4VPartonStringModel.hh"
#include "G4ios.hh"
#include "G4ShortLivedConstructor.hh"


G4VPartonStringModel::G4VPartonStringModel()
{
//  Make shure Shotrylived partyicles are constructed.
	G4ShortLivedConstructor ShortLived;
	ShortLived.ConstructParticle();
}

G4VPartonStringModel::G4VPartonStringModel(const G4VPartonStringModel &) : G4VHighEnergyGenerator()
{
}


G4VPartonStringModel::~G4VPartonStringModel()
{
}


const G4VPartonStringModel & G4VPartonStringModel::operator=(const G4VPartonStringModel &)
{
  G4Exception("G4VPartonStringModel::operator= meant to not be accessable");
  return *this;
}


int G4VPartonStringModel::operator==(const G4VPartonStringModel &) const
{
 return 0;
}

int G4VPartonStringModel::operator!=(const G4VPartonStringModel &) const
{
  return 1;
}

G4KineticTrackVector * G4VPartonStringModel::Scatter(const G4Nucleus &theNucleus, 
                                                const G4DynamicParticle &aPrimary)
{  
  G4ExcitedStringVector * strings = NULL;

  G4DynamicParticle thePrimary=aPrimary;
  
  G4LorentzRotation toZ;
  G4LorentzVector Ptmp=thePrimary.Get4Momentum();
  toZ.rotateZ(-1*Ptmp.phi());
  toZ.rotateY(-1*Ptmp.theta());
  thePrimary.Set4Momentum(toZ*Ptmp);
  G4LorentzRotation toLab(toZ.inverse());

  G4int attempts = 0, maxAttempts=20;
  while ( strings  == NULL )
  {
  	if (attempts++ > maxAttempts ) 
  	{
		G4Exception("G4VPartonStringModel::Scatter(): fails to generate strings");
  	}

	theThis->Init(theNucleus,thePrimary);
  	strings = GetStrings();
  }
  
  G4KineticTrackVector * theResult = 0;
  G4double stringEnergy(0);
  for ( unsigned int astring=0; astring < strings->size(); astring++)
  {
//    rotate string to lab frame, models have it aligned to z
    stringEnergy += (*strings)[astring]->GetLeftParton()->Get4Momentum().t();
    stringEnergy += (*strings)[astring]->GetRightParton()->Get4Momentum().t();
    (*strings)[astring]->LorentzRotate(toLab);
  }
  // G4cout << "Total string energy = "<<stringEnergy<<G4endl;
  
  theResult = stringFragmentationModel->FragmentStrings(strings);
  std::for_each(strings->begin(), strings->end(), DeleteString() );
  delete strings;

  return theResult;
}


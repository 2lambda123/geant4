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
// $Id: G4StableFermiFragment.cc,v 1.1 2003-08-26 18:34:47 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1998)

#include "G4StableFermiFragment.hh"


G4StableFermiFragment::G4StableFermiFragment()
{
}

G4StableFermiFragment::G4StableFermiFragment(const G4StableFermiFragment &) : G4VFermiFragment()
{
    G4Exception("G4StableFermiFragment::copy_constructor meant to not be accessable");
}


G4StableFermiFragment::~G4StableFermiFragment()
{
}


const G4StableFermiFragment & G4StableFermiFragment::operator=(const G4StableFermiFragment &)
{
    G4Exception("G4StableFermiFragment::operator= meant to not be accessable");
    return *this;
}


G4bool G4StableFermiFragment::operator==(const G4StableFermiFragment &) const
{
    return false;
}

G4bool G4StableFermiFragment::operator!=(const G4StableFermiFragment &) const
{
    return true;
}



G4FragmentVector * G4StableFermiFragment::GetFragment(const G4LorentzVector & aMomentum) const
{
    G4FragmentVector * theResult = new G4FragmentVector;

    theResult->push_back(new G4Fragment(A,Z,aMomentum));
  
    return theResult;
}

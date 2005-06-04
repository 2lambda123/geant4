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
// $Id: G4Be8FermiFragment.hh,v 1.2 2005-06-04 13:22:14 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1998)

#ifndef G4Be8FermiFragment_h
#define G4Be8FermiFragment_h 1

#include "G4UnstableFermiFragment.hh"


class G4Be8FermiFragment : public G4UnstableFermiFragment
{
public:
  G4Be8FermiFragment(const G4int anA, const G4int aZ, const G4int Pol, const G4double ExE);

  virtual ~G4Be8FermiFragment();
  
private:
  G4Be8FermiFragment();
  
  G4Be8FermiFragment(const G4Be8FermiFragment &right);
  
  const G4Be8FermiFragment & operator=(const G4Be8FermiFragment &right);
  G4bool operator==(const G4Be8FermiFragment &right) const;
  G4bool operator!=(const G4Be8FermiFragment &right) const;
  
};


#endif



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
// $Id: G4FermiBreakUp.hh,v 1.2 2005-06-04 13:22:14 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1998)

#ifndef G4FermiBreakUp_h
#define G4FermiBreakUp_h 1

#include "G4VFermiBreakUp.hh"
#include "G4FermiConfiguration.hh"
#include "G4FermiConfigurationList.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"

class G4FermiBreakUp : public G4VFermiBreakUp 
{
public:
  G4FermiBreakUp();
  ~G4FermiBreakUp();
  
private:
  G4FermiBreakUp(const G4FermiBreakUp &right);
  
  const G4FermiBreakUp & operator=(const G4FermiBreakUp &right);
  G4bool operator==(const G4FermiBreakUp &right) const;
  G4bool operator!=(const G4FermiBreakUp &right) const;
  
public:
  G4FragmentVector * BreakItUp(const G4Fragment &theNucleus);
};


#endif



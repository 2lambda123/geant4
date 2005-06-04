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
// $Id: G4Mg23GEMCoulombBarrier.hh,v 1.2 2005-06-04 13:25:25 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Dec 1999)

#ifndef G4Mg23GEMCoulombBarrier_h
#define G4Mg23GEMCoulombBarrier_h 1

#include "G4GEMCoulombBarrierHE.hh"
#include "globals.hh"

class G4Mg23GEMCoulombBarrier : public G4GEMCoulombBarrierHE
{
public:
  G4Mg23GEMCoulombBarrier() : G4GEMCoulombBarrierHE(23,12) {};
  ~G4Mg23GEMCoulombBarrier() {};

private:
  G4Mg23GEMCoulombBarrier(const G4Mg23GEMCoulombBarrier & right);

  const G4Mg23GEMCoulombBarrier & operator=(const G4Mg23GEMCoulombBarrier & right);
  G4bool operator==(const G4Mg23GEMCoulombBarrier & right) const;
  G4bool operator!=(const G4Mg23GEMCoulombBarrier & right) const;
  

};

#endif

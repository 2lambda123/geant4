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
// $Id: G4F20GEMCoulombBarrier.hh,v 1.1 2003-08-26 18:42:09 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Dec 1999)

#ifndef G4F20GEMCoulombBarrier_h
#define G4F20GEMCoulombBarrier_h 1

#include "G4GEMCoulombBarrierHE.hh"
#include "globals.hh"

class G4F20GEMCoulombBarrier : public G4GEMCoulombBarrierHE
{
public:
  G4F20GEMCoulombBarrier() : G4GEMCoulombBarrierHE(20,9) {};
  ~G4F20GEMCoulombBarrier() {};

private:
  G4F20GEMCoulombBarrier(const G4F20GEMCoulombBarrier & right);

  const G4F20GEMCoulombBarrier & operator=(const G4F20GEMCoulombBarrier & right);
  G4bool operator==(const G4F20GEMCoulombBarrier & right) const;
  G4bool operator!=(const G4F20GEMCoulombBarrier & right) const;
  

};

#endif

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
// $Id: G4Li6GEMChannel.hh,v 1.2 2005-06-04 13:25:25 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Sept. 2001)
//


#ifndef G4Li6GEMChannel_h
#define G4Li6GEMChannel_h 1

#include "G4GEMChannel.hh"
#include "G4Li6GEMCoulombBarrier.hh"
#include "G4Li6GEMProbability.hh"

class G4Li6GEMChannel : public G4GEMChannel
{
public:
  // only available constructor
  G4Li6GEMChannel() : G4GEMChannel(6,3,"Li6",
				   &theEvaporationProbability,
				   &theCoulombBarrier)
  {
    theEvaporationProbability.SetCoulomBarrier(&theCoulombBarrier);
  }
  
  // destructor
  ~G4Li6GEMChannel() {};
  
private:
  const G4Li6GEMChannel & operator=(const G4Li6GEMChannel & right);  
    
  G4Li6GEMChannel(const G4Li6GEMChannel & right);
  
public:
  G4bool operator==(const G4Li6GEMChannel & right) const;
  G4bool operator!=(const G4Li6GEMChannel & right) const;
    
private:
  
  G4Li6GEMCoulombBarrier theCoulombBarrier;
	
  G4Li6GEMProbability theEvaporationProbability;
  
};
#endif

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
// $Id: G4C12GEMChannel.hh,v 1.1 2003-08-26 18:41:54 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Sept. 2001)
//


#ifndef G4C12GEMChannel_h
#define G4C12GEMChannel_h 1

#include "G4GEMChannel.hh"
#include "G4C12GEMCoulombBarrier.hh"
#include "G4C12GEMProbability.hh"

class G4C12GEMChannel : public G4GEMChannel
{
public:
  // only available constructor
  G4C12GEMChannel() : G4GEMChannel(12,6,"C12",
				   &theEvaporationProbability,
				   &theCoulombBarrier)
  {
    theEvaporationProbability.SetCoulomBarrier(&theCoulombBarrier);
  }
  
  // destructor
  ~G4C12GEMChannel() {};
  
private:
  const G4C12GEMChannel & operator=(const G4C12GEMChannel & right);  
    
  G4C12GEMChannel(const G4C12GEMChannel & right);
  
public:
  G4bool operator==(const G4C12GEMChannel & right) const;
  G4bool operator!=(const G4C12GEMChannel & right) const;
    
private:
  
  G4C12GEMCoulombBarrier theCoulombBarrier;
	
  G4C12GEMProbability theEvaporationProbability;
  
};
#endif

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
// $Id: G4B12GEMChannel.hh,v 1.1 2003-08-26 18:41:39 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Sept. 2001)
//


#ifndef G4B12GEMChannel_h
#define G4B12GEMChannel_h 1

#include "G4GEMChannel.hh"
#include "G4B12GEMCoulombBarrier.hh"
#include "G4B12GEMProbability.hh"

class G4B12GEMChannel : public G4GEMChannel
{
public:
  // only available constructor
  G4B12GEMChannel() : G4GEMChannel(12,5,"B12",
				   &theEvaporationProbability,
				   &theCoulombBarrier)
  {
    theEvaporationProbability.SetCoulomBarrier(&theCoulombBarrier);
  }
  
  // destructor
  ~G4B12GEMChannel() {};
  
private:
  const G4B12GEMChannel & operator=(const G4B12GEMChannel & right);  
    
  G4B12GEMChannel(const G4B12GEMChannel & right);
  
public:
  G4bool operator==(const G4B12GEMChannel & right) const;
  G4bool operator!=(const G4B12GEMChannel & right) const;
    
private:
  
  G4B12GEMCoulombBarrier theCoulombBarrier;
	
  G4B12GEMProbability theEvaporationProbability;
  
};
#endif

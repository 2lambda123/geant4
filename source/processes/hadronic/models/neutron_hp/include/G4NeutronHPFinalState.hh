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
// $Id: G4NeutronHPFinalState.hh,v 1.8 2003-05-30 11:32:33 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4NeutronHPFinalState_h
#define G4NeutronHPFinalState_h

#include "G4Material.hh"
#include "G4FastVector.hh"
#include "G4ParticleChange.hh"
#include "G4NeutronHPNames.hh"
#include "G4NeutronHPVector.hh"

class G4NeutronHPFinalState
{
public:

  G4NeutronHPFinalState()
  { 
    hasFSData = true; 
    hasXsec = true;
    hasAnyData = true;
    theBaseZ = 0;
    theBaseA = 0;
  };
  
  virtual ~G4NeutronHPFinalState(){};

  virtual void Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType) = 0;
  virtual G4ParticleChange * ApplyYourself(const G4Track & ) 
  {
    G4Exception("G4ParticleChange * ApplyYourself(const G4Track & theTrack) needs implementation");
    return NULL;
  }
  
  // of course this would better be Done templating G4NeutronHPChannel..., 
  // but due to peculiarities of the DEC compiler, this way it
  // is easier to maintain.
  virtual G4NeutronHPFinalState * New() = 0;
  
  G4bool HasXsec() {return hasXsec;}
  G4bool HasFSData() {return hasFSData;}
  G4bool HasAnyData() {return hasAnyData;}
  
  virtual G4double GetXsec(G4double ) { return 0; }
  virtual G4NeutronHPVector * GetXsec() { return NULL; }
  
  void     SetA_Z(G4double anA, G4double aZ) {theBaseA = anA; theBaseZ = aZ; }
  G4double GetZ() { return theBaseZ; }
  G4double GetN() { return theBaseA; }
  
  protected:
  
  G4bool hasXsec;
  G4bool hasFSData;
  G4bool hasAnyData;
  G4NeutronHPNames theNames;
  
  G4ParticleChange theResult;
  
  G4double theBaseA;
  G4double theBaseZ;

  private:
  
};
#endif

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
// $Id: G4NeutronHPT2AInelasticFS.hh,v 1.7 2003-07-01 15:58:37 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4NeutronHPT2AInelasticFS_h
#define G4NeutronHPT2AInelasticFS_h 1

#include "globals.hh"
#include "G4HadProjectile.hh"
#include "G4HadFinalState.hh"
#include "G4NeutronHPInelasticBaseFS.hh"

class G4NeutronHPT2AInelasticFS : public G4NeutronHPInelasticBaseFS
{
  public:
  
  G4NeutronHPT2AInelasticFS(){}
  ~G4NeutronHPT2AInelasticFS(){}
  void Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType);
  G4HadFinalState * ApplyYourself(const G4HadProjectile & theTrack);
  G4NeutronHPFinalState * New() 
  {
   G4NeutronHPT2AInelasticFS * theNew = new G4NeutronHPT2AInelasticFS;
   return theNew;
  }
  
  private:
};
#endif

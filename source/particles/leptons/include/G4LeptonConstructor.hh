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
// $Id: G4LeptonConstructor.hh,v 1.4 2004-09-02 01:52:39 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//	GEANT 4 class implementatLepton file 
//
#ifndef G4LeptonConstructor_h
#define G4LeptonConstructor_h 1

#include "globals.hh"
#include "G4ios.hh"

class G4LeptonConstructor
{
  //This class is a utility class for constructLepton 
  //short lived particles

  public:
    G4LeptonConstructor();
    ~G4LeptonConstructor();
  
  public:
    void ConstructParticle();

  protected:
    void ConstructELeptons();
    void ConstructMuLeptons();
    void ConstructTauLeptons();
};

#endif

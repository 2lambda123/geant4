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
// $Id: G4MesonConstructor.hh,v 1.4 2005-09-30 22:59:05 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//	GEANT 4 class implementation file 
//
#ifndef G4MesonConstructor_h
#define G4MesonConstructor_h 1

#include "globals.hh"
#include "G4ios.hh"

class G4MesonConstructor
{
  //This class is a utility class for construction 

  public:
    G4MesonConstructor();
    ~G4MesonConstructor();
  
  public:
    static void ConstructParticle();

  protected:
    static void ConstructLightMesons();
    static void ConstructCharmMesons();
    static void ConstructBottomMesons();
};

#endif

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
// $Id: G4AntiDMesonZero.hh,v 1.10 2005-01-14 03:49:13 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//
//      Created,             Hisaya Kurashige, 15 June 1997
// **********************************************************************
//  New implementation as a utility class  M.Asai, 26 July 2004
// ----------------------------------------------------------------

#ifndef G4AntiDMesonZero_h
#define G4AntiDMesonZero_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

// ######################################################################
// ###                         AntiDMesonZero                         ###
// ######################################################################

class G4AntiDMesonZero : public G4ParticleDefinition
{
 private:
   static G4AntiDMesonZero* theInstance;
   G4AntiDMesonZero(){}
   ~G4AntiDMesonZero(){}

 public:
   static G4AntiDMesonZero* Definition();
   static G4AntiDMesonZero* AntiDMesonZeroDefinition();
   static G4AntiDMesonZero* AntiDMesonZero();
};

#endif

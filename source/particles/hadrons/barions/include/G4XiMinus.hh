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
// $Id: G4XiMinus.hh,v 1.9 2005-01-14 03:49:08 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History: first implementation, based on object model of
//      4-th April 1996, G.Cosmo
// ****************************************************************
//  New implementation as a utility class  M.Asai, 26 July 2004
// ----------------------------------------------------------------

#ifndef G4XiMinus_h
#define G4XiMinus_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

// ######################################################################
// ###                          XiMinus                               ###
// ######################################################################

class G4XiMinus : public G4ParticleDefinition
{
 private:
   static G4XiMinus* theInstance;
   G4XiMinus(){}
   ~G4XiMinus(){}

 public:
   static G4XiMinus* Definition();
   static G4XiMinus* XiMinusDefinition();
   static G4XiMinus* XiMinus();
};

#endif

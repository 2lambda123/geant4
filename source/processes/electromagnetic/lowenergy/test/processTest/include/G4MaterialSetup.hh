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
// $Id: G4MaterialSetup.hh,v 1.2 2006-05-12 17:47:58 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// File name:     G4MaterialSetup
//
// Author:        Maria Grazia Pia
// 
// Creation date: 1 October 2001
//
// Modifications: 
//
// -------------------------------------------------------------------

#ifndef G4MATERIALSETUP_HH
#define G4MATERIALSETUP_HH

#include "globals.hh"

class G4ProcessTest;
class G4Material;

class G4MaterialSetup
{
  public:

  G4MaterialSetup() { }
  virtual ~G4MaterialSetup() { }
  
  void makeMaterials();

 private:

};

#endif 

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
// $Id: G4MCCIndexConversionTable.cc,v 1.2 2004-12-15 15:42:36 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//
// History:
// -------
// - First implementation   20th August 2004  by H.Kurashige
//-------------------------------------

#include  "G4MCCIndexConversionTable.hh"


G4MCCIndexConversionTable::G4MCCIndexConversionTable()
{

} 

G4MCCIndexConversionTable::~G4MCCIndexConversionTable()
{
  vecNewIndex.clear();
}

void G4MCCIndexConversionTable::Reset(size_t size)
{
  // reset conversion table 
  vecNewIndex.clear();
  vecNewIndex.resize(size, -1);
}
 


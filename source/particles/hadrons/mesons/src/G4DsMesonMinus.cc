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
// $Id: G4DsMesonMinus.cc,v 1.9 2004-09-02 01:52:37 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//
//      Created                 Hisaya Kurashige, 16 June 1997
// **********************************************************************
//  New impelemenataion as an utility class  M.Asai, 26 July 2004
// ----------------------------------------------------------------

#include "G4DsMesonMinus.hh"
#include "G4ParticleTable.hh"

// ######################################################################
// ###                        DsMesonMinus                            ###
// ######################################################################

G4ParticleDefinition* G4DsMesonMinus::theInstance = 0;

G4ParticleDefinition* G4DsMesonMinus::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "Ds-";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  theInstance = pTable->FindParticle(name);
  if (theInstance !=0) return theInstance;

  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
  theInstance = new G4ParticleDefinition(
                 name,   1.9685*GeV,  1.342e-9*MeV,    -1.*eplus,
                    0,              -1,             0,
                    0,               0,             0,
              "meson",               0,             0,         -431,
                false,     0.467e-3*ns,          NULL,
             false,           "Ds"
              );

  return theInstance;
}

G4ParticleDefinition*  G4DsMesonMinus::DsMesonMinusDefinition()
{
  return Definition();
}

G4ParticleDefinition*  G4DsMesonMinus::DsMesonMinus()
{
  return Definition();
}


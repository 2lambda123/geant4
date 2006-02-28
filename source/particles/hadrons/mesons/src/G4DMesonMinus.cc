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
// $Id: G4DMesonMinus.cc,v 1.12 2006-02-28 10:26:24 kurasige Exp $
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
// ----------------------------------------------------------------------

#include "G4DMesonMinus.hh"
#include "G4ParticleTable.hh"

// ######################################################################
// ###                         DMesonMinus                            ###
// ######################################################################

G4DMesonMinus* G4DMesonMinus::theInstance = 0;

G4DMesonMinus* G4DMesonMinus::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "D-";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
  {
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding

   anInstance = new G4ParticleDefinition(
                 name,    1.8693*GeV,   6.33e-10*MeV,    -1.*eplus,
                    0,              -1,             0,
                    1,              -1,             0,
              "meson",               0,             0,        -411,
                false,     1.040e-3*ns,          NULL,
                false,       "D");
  }
  theInstance = reinterpret_cast<G4DMesonMinus*>(anInstance);
  return theInstance;
}

G4DMesonMinus*  G4DMesonMinus::DMesonMinusDefinition()
{
  return Definition();
}

G4DMesonMinus*  G4DMesonMinus::DMesonMinus()
{
  return Definition();
}


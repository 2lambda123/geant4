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
// $Id: G4MCTruthRootIO.cc,v 1.5 2005-04-22 16:26:47 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// File: G4MCTruthRootIO.cc
//
// History:
//   '02.5.7  Youhei Morita  Initial creation

#include "G4MCTruthRootIO.hh"

G4MCTruthRootIO* G4MCTruthRootIO::thePointer=G4MCTruthRootIO::GetMCTruthRootIO();

// Implementation of Store
bool G4MCTruthRootIO::Store(G4MCTEvent*)
{
  return true;
}

// Implementation of Retrieve
bool G4MCTruthRootIO::Retrieve(G4MCTEvent* &)
{
  return true;
}

// Implementation of GetMCTruthRootIO
G4MCTruthRootIO* G4MCTruthRootIO::GetMCTruthRootIO()
{
  return 0;
}

// End of G4MCTruthRootIO.cc


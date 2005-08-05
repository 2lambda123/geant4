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
// $Id: G4BetheBlochNoDeltaModel.cc,v 1.1 2005-08-05 13:43:14 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4BetheBlochNoDeltaModel
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 05.08.2005
//
// Modifications:
//
//
// Class Description:
//
// Ionisation of heavy charged particles including the monopole

// -------------------------------------------------------------------
//

#include "G4BetheBlochNoDeltaModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4BetheBlochNoDeltaModel::G4BetheBlochNoDeltaModel(G4double ch, 
  const G4ParticleDefinition*p, const G4String& nam) :
  G4BetheBlochModel(p, nam), 
  mcharge2(ch*ch)
{}

G4BetheBlochNoDeltaModel::~G4BetheBlochNoDeltaModel()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



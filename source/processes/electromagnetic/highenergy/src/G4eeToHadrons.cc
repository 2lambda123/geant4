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
// $Id: G4eeToHadrons.cc,v 1.5 2005-11-23 19:17:26 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4eeToHadrons
//
// Author:        Vladimir Ivanchenko on base of Michel Maire code
//
// Creation date: 02.08.2004
//
// Modifications:
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
// 23-11-05 Istert AddEmModel which was lost (V.Ivantchenko)
//

//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4eeToHadrons.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4eeToHadrons::G4eeToHadrons(const G4String& name)
  : G4VEmProcess(name),
    csFactor(1.0),
    isInitialised(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4eeToHadrons::~G4eeToHadrons()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToHadrons::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;

    SetBuildTableFlag(false);
    SetIntegral(true);
    SetMaxKinEnergy(10.0*TeV);

    SetSecondaryParticle(G4Gamma::Gamma());
    SetParticle(G4Positron::Positron());

    multimodel = new G4eeToHadronsMultiModel(verboseLevel);
    if(csFactor > 1.0) multimodel->SetCrossSecFactor(csFactor);
    AddEmModel(0, multimodel);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToHadrons::PrintInfo()
{
  multimodel->PrintInfo();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToHadrons::SetCrossSecFactor(G4double fac)
{
  if(multimodel) multimodel->SetCrossSecFactor(fac);
  csFactor = fac;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

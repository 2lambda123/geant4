//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4hPairProduction.cc,v 1.1 2008-02-29 17:50:05 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4hPairProduction
//
// Author:        Vladimir Ivanchenko on base of model for muons
//
// Creation date: 01.03.2008
//
// Modifications:
//
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4hPairProduction.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4hPairProductionModel.hh"
#include "G4UniversalFluctuation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4hPairProduction::G4hPairProduction(const G4String& name)
  : G4VEnergyLossProcess(name),
    theParticle(0),
    theBaseParticle(0),
    lowestKinEnergy(1.*GeV),
    isInitialised(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hPairProduction::~G4hPairProduction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hPairProduction::InitialiseEnergyLossProcess(
                         const G4ParticleDefinition* part,
			 const G4ParticleDefinition*)
{
  if (!isInitialised) {
    isInitialised = true;

    theParticle = part;
    SetSecondaryParticle(G4Positron::Positron());
    SetIonisation(false);

    G4hPairProductionModel* em = new G4hPairProductionModel();
    em->SetLowestKineticEnergy(lowestKinEnergy);
    G4VEmFluctuationModel* fm = new G4UniversalFluctuation();
    em->SetLowEnergyLimit(0.1*keV);
    em->SetHighEnergyLimit(100.0*TeV);
    AddEmModel(1, em, fm);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hPairProduction::PrintInfo()
{
  G4cout << "      Theoretical model "
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....





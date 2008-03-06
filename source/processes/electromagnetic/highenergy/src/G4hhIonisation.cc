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
// $Id: G4hhIonisation.cc,v 1.7 2008-03-06 18:44:58 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4hhIonisation
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 30.09.2005
//
// Modifications:
// 10-01-06 SetStepLimits -> SetStepFunction (V.Ivantchenko)
// 27-10-06 Add maxKinEnergy (V.Ivantchenko)
//
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4hhIonisation.hh"
#include "G4BraggNoDeltaModel.hh"
#include "G4BetheBlochNoDeltaModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4BohrFluctuations.hh"
#include "G4UnitsTable.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hhIonisation::G4hhIonisation(const G4String& name)
  : G4VEnergyLossProcess(name),
    theParticle(0),
    theBaseParticle(0),
    isInitialised(false)
{
  minKinEnergy = 0.1*keV;
  maxKinEnergy = 100.*TeV;
  SetDEDXBinning(120);
  SetMinKinEnergy(minKinEnergy);
  SetMaxKinEnergy(maxKinEnergy);
  SetStepFunction(0.1, 0.1*mm);
  SetVerboseLevel(1);
  SetProcessSubType(2);
  mass = 0.0;
  ratio = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hhIonisation::~G4hhIonisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hhIonisation::InitialiseEnergyLossProcess(const G4ParticleDefinition* part,
                                                 const G4ParticleDefinition* bpart)
{
  if(isInitialised) return;

  theParticle = part;
  if(bpart) G4cout << "G4hhIonisation::InitialiseEnergyLossProcess WARNING: no "
                   << "base particle should be defined for the process "
		   << GetProcessName() << G4endl;

  SetBaseParticle(0);
  SetSecondaryParticle(G4Electron::Electron());
  mass  = theParticle->GetPDGMass();
  ratio = electron_mass_c2/mass;
  eth = 2.0*MeV*mass/proton_mass_c2;
  flucModel = new G4BohrFluctuations();

  G4int nm = 1;

  if(eth > minKinEnergy) {
    G4VEmModel* em = new G4BraggNoDeltaModel();
    em->SetLowEnergyLimit(minKinEnergy);
    em->SetHighEnergyLimit(eth);
    AddEmModel(nm, em, flucModel);
    nm++;
  }

  if(eth < maxKinEnergy) {
    G4VEmModel* em1 = new G4BetheBlochNoDeltaModel();
    em1->SetLowEnergyLimit(std::max(eth,minKinEnergy));
    em1->SetHighEnergyLimit(maxKinEnergy);
    AddEmModel(nm, em1, flucModel);
  }

  if(verboseLevel>0)
    G4cout << "G4hhIonisation is initialised: Nmodels= " << nm << G4endl;

  isInitialised = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hhIonisation::PrintInfo()
{
  G4cout << "      Delta-ray will not be produced; "
         << "Bether-Bloch model for E > " << std::max(eth,minKinEnergy)
	 << G4endl;
  if(eth > minKinEnergy) G4cout
	 << "      ICRU49 parametrisation scaled from protons below.";
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

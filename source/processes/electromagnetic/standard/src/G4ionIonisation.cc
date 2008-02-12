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
// $Id: G4ionIonisation.cc,v 1.47 2008-02-12 09:42:13 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4ionIonisation
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 07.05.2002
//
// Modifications:
//
// 23-12-02 Change interface in order to move to cut per region (V.Ivanchenko)
// 26-12-02 Secondary production moved to derived classes (V.Ivanchenko)
// 13-02-03 SubCutoff regime is assigned to a region (V.Ivanchenko)
// 18-04-03 Use IonFluctuations (V.Ivanchenko)
// 03-08-03 Add effective charge (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 27-05-04 Set integral to be a default regime (V.Ivanchenko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
// 10-01-06 SetStepLimits -> SetStepFunction (V.Ivantchenko)
// 10-05-06 Add a possibility to download user data (V.Ivantchenko)
// 13-05-06 Add data for light ion stopping in water (V.Ivantchenko)
// 14-01-07 use SetEmModel() and SetFluctModel() from G4VEnergyLossProcess (mma)
// 16-05-07 Add data for light ion stopping only for GenericIon (V.Ivantchenko)
// 07-11-07 Fill non-ionizing energy loss (V.Ivantchenko)
//
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4ionIonisation.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4GenericIon.hh"
#include "G4BraggModel.hh"
#include "G4BraggIonModel.hh"
#include "G4BetheBlochModel.hh"
#include "G4IonFluctuations.hh"
#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"
#include "G4WaterStopping.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4ionIonisation::G4ionIonisation(const G4String& name)
  : G4VEnergyLossProcess(name),
    theParticle(0),
    theBaseParticle(0),
    isInitialised(false),
    stopDataActive(true),
    nuclearStopping(true)
{
  //  SetLinearLossLimit(0.15);
  SetStepFunction(0.1, 0.1*mm);
  SetIntegral(true);
  SetVerboseLevel(1);
  corr = G4LossTableManager::Instance()->EmCorrections();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4ionIonisation::~G4ionIonisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4ionIonisation::InitialiseEnergyLossProcess(
		      const G4ParticleDefinition* part,
		      const G4ParticleDefinition* bpart)
{
  if(isInitialised) return;

  theParticle = part;

  if(part == bpart || part == G4GenericIon::GenericIon()) theBaseParticle = 0;
  else if(bpart == 0) theBaseParticle = G4GenericIon::GenericIon();
  else                theBaseParticle = bpart;

  SetBaseParticle(theBaseParticle);
  SetSecondaryParticle(G4Electron::Electron());

  if(theBaseParticle) baseMass = theBaseParticle->GetPDGMass();
  else                baseMass = theParticle->GetPDGMass();
  
  if (!EmModel(1)) SetEmModel(new G4BraggIonModel(),1);
  EmModel(1)->SetLowEnergyLimit(100*eV);
  eth = 2.0*MeV;  
  EmModel(1)->SetHighEnergyLimit(eth);
  if (!FluctModel()) SetFluctModel(new G4IonFluctuations());
  AddEmModel(1, EmModel(1), FluctModel());

  if (!EmModel(2)) SetEmModel(new G4BetheBlochModel(),2);  
  EmModel(2)->SetLowEnergyLimit(eth);
  EmModel(2)->SetHighEnergyLimit(100*TeV);
  AddEmModel(2, EmModel(2), FluctModel());    

  // Add ion stoping tables for Generic Ion
  if(part == G4GenericIon::GenericIon()) {
    G4WaterStopping  ws(corr);
    effCharge = corr->GetIonEffectiveCharge(EmModel(1));
  } else {
    effCharge = corr->GetIonEffectiveCharge(0);
  }

  isInitialised = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4ionIonisation::PrintInfo()
{
  if(EmModel(1) && EmModel(2))
    G4cout << "      Scaling relation is used from proton dE/dx and range."
	   << "\n      Delta cross sections and sampling from " 
	   << EmModel(2)->GetName() << " model for scaled energy > "
	   << eth/MeV << " MeV"
	   << "\n      Parametrisation from "
	   << EmModel(1)->GetName() << " for protons below."
	   << " NuclearStopping= " << nuclearStopping
	   << G4endl;	 
  if (stopDataActive)
    G4cout << "\n      Stopping Power data for " 
           << corr->GetNumberOfStoppingVectors()
	   << " ion/material pairs are used."
           << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4ionIonisation::AddStoppingData(G4int Z, G4int A,
				      const G4String& mname,
				      G4PhysicsVector& dVector)
{
  corr->AddStoppingData(Z, A, mname, dVector);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4ionIonisation::CorrectionsAlongStep(const G4MaterialCutsCouple* couple,
					   const G4DynamicParticle* dp,
					   G4double& eloss,
					   G4double& niel,
                                           G4double s)
{
  const G4ParticleDefinition* part = dp->GetDefinition();
  const G4Material* mat = couple->GetMaterial();
  if(eloss < preKinEnergy) {
    //    G4cout << "e= " << preKinEnergy << " ratio= " << massRatio 
    //	   << " eth= " << eth<< " eloss0= " << eloss <<  G4endl;
    if(preKinEnergy*massRatio > eth) {
      eloss += s*corr->IonHighOrderCorrections(part,mat,preKinEnergy);
      //G4cout << "Above th: eloss= " << eloss << G4endl;
    } else {

      if(stopDataActive)
	eloss *= corr->EffectiveChargeCorrection(part,mat,preKinEnergy);
      //G4cout << "Below th: eloss= " << eloss << G4endl;

    }
    fParticleChange.SetProposedCharge(effCharge->EffectiveCharge(part,
                                      mat,preKinEnergy-eloss));
  }
  if(nuclearStopping && preKinEnergy*massRatio < 50.*eth*charge2) {
    G4double e = preKinEnergy - eloss*0.5;
    if(e <= 0.0) e = 0.5*preKinEnergy;
    G4double nloss = s*corr->NuclearDEDX(part,mat,e);
    if(eloss + nloss > preKinEnergy) {
      nloss *= (preKinEnergy/(eloss + nloss));
      eloss = preKinEnergy - nloss;
    }
    niel += nloss;
    //G4cout << "G4ionIonisation::CorrectionsAlongStep: e= " << preKinEnergy
    //	   << " de= " << eloss << " NIEL= " << nloss << G4endl;
    fParticleChange.ProposeNonIonizingEnergyDeposit(nloss);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

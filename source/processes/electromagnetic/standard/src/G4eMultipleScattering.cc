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
// $Id: G4eMultipleScattering.cc,v 1.4 2008-07-31 13:11:34 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -----------------------------------------------------------------------------
//
// GEANT4 Class file
//
// File name:     G4eMultipleScattering 
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 10 March 2008
// 
// Modifications:
//
// -----------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4eMultipleScattering.hh"
#include "G4UrbanMscModel2.hh"
#include "G4MscStepLimitType.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

G4eMultipleScattering::G4eMultipleScattering(const G4String& processName)
  : G4VMultipleScattering(processName)
{
  isInitialized = false;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4eMultipleScattering::~G4eMultipleScattering()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4eMultipleScattering::IsApplicable (const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eMultipleScattering::InitialiseProcess(const G4ParticleDefinition* p)
{
  // Modification of parameters between runs
  if(isInitialized) {
    if (p->GetParticleType() != "nucleus") {
      mscUrban->SetStepLimitType(StepLimitType());
      mscUrban->SetLateralDisplasmentFlag(LateralDisplasmentFlag());
      mscUrban->SetSkin(Skin());
      mscUrban->SetRangeFactor(RangeFactor());
      mscUrban->SetGeomFactor(GeomFactor());
    }
    return;
  }

  // defaults for ions, which cannot be overwritten
  if (p->GetParticleType() == "nucleus") {
    SetStepLimitType(fMinimal);
    SetLateralDisplasmentFlag(false);
    SetBuildLambdaTable(false);
  }

  // initialisation of parameters - defaults for particles other
  // than ions can be overwritten by users
  mscUrban = new G4UrbanMscModel2();
  mscUrban->SetStepLimitType(StepLimitType());
  mscUrban->SetLateralDisplasmentFlag(LateralDisplasmentFlag());
  mscUrban->SetSkin(Skin());
  mscUrban->SetRangeFactor(RangeFactor());
  mscUrban->SetGeomFactor(GeomFactor());

  AddEmModel(1,mscUrban);
  isInitialized = true;
  /*
  G4cout << "G4eMultipleScattering::InitialiseProcess for " 
	 << p->GetParticleName()
	 << " skin= " << Skin()
	 << " SA= " << steppingAlgorithm
	 << G4endl;
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eMultipleScattering::PrintInfo()
{
  G4cout << "      Model <" << mscUrban->GetName() << ">, RangeFactor= "
	 << RangeFactor()
	 << ", Step limit type " << StepLimitType()
	 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


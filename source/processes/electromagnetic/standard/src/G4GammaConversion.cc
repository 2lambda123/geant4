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
// $Id: G4GammaConversion.cc,v 1.26 2006-09-11 12:34:09 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//------------------ G4GammaConversion physics process -------------------------
//                   by Michel Maire, 24 May 1996
//
// 11-06-96 Added SelectRandomAtom() method, M.Maire
// 21-06-96 SetCuts implementation, M.Maire
// 24-06-96 simplification in ComputeCrossSectionPerAtom, M.Maire
// 24-06-96 in DoIt : change the particleType stuff, M.Maire
// 25-06-96 modification in the generation of the teta angle, M.Maire
// 16-09-96 minors optimisations in DoIt. Thanks to P.Urban
//          dynamical array PartialSumSigma
// 13-12-96 fast sampling of epsil below 2 MeV, L.Urban
// 14-01-97 crossection table + meanfreepath table.
//          PartialSumSigma removed, M.Maire
// 14-01-97 in DoIt the positron is always created, even with Ekine=0,
//          for further annihilation, M.Maire
// 14-03-97 new Physics scheme for geant4alpha, M.Maire
// 28-03-97 protection in BuildPhysicsTable, M.Maire
// 19-06-97 correction in ComputeCrossSectionPerAtom, L.Urban
// 04-06-98 in DoIt, secondary production condition:
//            range>std::min(threshold,safety)
// 13-08-98 new methods SetBining() PrintInfo()
// 28-05-01 V.Ivanchenko minor changes to provide ANSI -wall compilation
// 11-07-01 PostStepDoIt - sampling epsil: power(rndm,0.333333)
// 13-07-01 DoIt: suppression of production cut for the (e-,e+) (mma)
// 06-08-01 new methods Store/Retrieve PhysicsTable (mma)
// 06-08-01 BuildThePhysicsTable() called from constructor (mma)
// 17-09-01 migration of Materials to pure STL (mma)
// 20-09-01 DoIt: fminimalEnergy = 1*eV (mma)
// 01-10-01 come back to BuildPhysicsTable(const G4ParticleDefinition&)
// 11-01-02 ComputeCrossSection: correction of extrapolation below EnergyLimit
// 21-03-02 DoIt: correction of the e+e- angular distribution (bug 363) mma
// 08-11-04 Remove of Store/Retrieve tables (V.Ivantchenko)
// 19-04-05 Migrate to model interface and inherit 
//          from G4VEmProcess (V.Ivanchenko) 
// 04-05-05, Make class to be default (V.Ivanchenko)
// 09-08-06, add SetModel(G4VEmModel*) (mma)
// -----------------------------------------------------------------------------

#include "G4GammaConversion.hh"
#include "G4BetheHeitlerModel.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

G4GammaConversion::G4GammaConversion(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false),
    selectedModel(0)    
{
  SetLambdaBinning(100);
  SetMinKinEnergy(2.0*electron_mass_c2);
  SetMaxKinEnergy(100.0*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4GammaConversion::~G4GammaConversion()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4GammaConversion::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(true);
    SetSecondaryParticle(G4Electron::Electron());
    G4double emin = max(MinKinEnergy(), 2.0*electron_mass_c2);
    SetMinKinEnergy(emin);
    G4double emax = MaxKinEnergy();
    if(!selectedModel) selectedModel = new G4BetheHeitlerModel();
    selectedModel->SetLowEnergyLimit(emin);
    selectedModel->SetHighEnergyLimit(emax);
    AddEmModel(1, selectedModel);
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4GammaConversion::PrintInfo()
{
  G4cout
    << " Total cross sections has a good parametrisation" 
    << " from 1.5 MeV to 100 GeV for all Z;"
    << "\n      sampling secondary e+e- according "
    << selectedModel->GetName() << " model"
    << G4endl;
}         

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4GammaConversion::SetModel(G4VEmModel* model)
{
  selectedModel = model;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

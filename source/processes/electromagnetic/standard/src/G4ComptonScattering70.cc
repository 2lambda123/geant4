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
// $Id: G4ComptonScattering70.cc,v 1.7 2005-05-02 12:44:18 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//------------ G4ComptonScattering70 physics process -----------------------------
//                   by Michel Maire, April 1996
//
// 28-05-96, DoIt() small change in ElecDirection, by M.Maire
// 10-06-96, simplification in ComputeMicroscopicCrossSection(), by M.Maire
// 21-06-96, SetCuts implementation, M.Maire
// 13-09-96, small changes in DoIt for better efficiency. Thanks to P.Urban
// 06-01-97, crossection table + meanfreepath table, M.Maire
// 05-03-97, new Physics scheme, M.Maire
// 28-03-97, protection in BuildPhysicsTable, M.Maire
// 07-04-98, remove 'tracking cut' of the scattered gamma, MMa
// 04-06-98, in DoIt, secondary production condition:
//                                     range>std::min(threshold,safety)
// 13-08-98, new methods SetBining()  PrintInfo()
// 15-12-98, cross section=0 below 10 keV
// 28-05-01, V.Ivanchenko minor changes to provide ANSI -wall compilation
// 13-07-01, DoIt: suppression of production cut for the electron (mma)
// 03-08-01, new methods Store/Retrieve PhysicsTable (mma)
// 06-08-01, BuildThePhysicsTable() called from constructor (mma)
// 17-09-01, migration of Materials to pure STL (mma)
// 20-09-01, DoIt: fminimalEnergy = 1*eV (mma)
// 01-10-01, come back to BuildPhysicsTable(const G4ParticleDefinition&)
// 17-04-02, LowestEnergyLimit = 1*keV     
// 26-05-04, cross section parametrization improved for low energy :
//           Egamma <~ 15 keV (Laszlo) 
// 08-11-04, Remove Store/Retrieve tables (V.Ivanchenko)
// 09-03-05  Migrate to model interface and inherit from G4VEmProcess (V.Ivanchenko) 
// -----------------------------------------------------------------------------

#include "G4ComptonScattering70.hh"
#include "G4KleinNishinaCompton.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

G4ComptonScattering70::G4ComptonScattering70(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false),
    selectedModel(0),
    mType(0)
{
  SetLambdaBinning(90);
  SetMinKinEnergy(0.1*keV);
  SetMaxKinEnergy(100.0*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4ComptonScattering70::~G4ComptonScattering70()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4ComptonScattering70::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(true);
    SetSecondaryParticle(G4Electron::Electron());
    G4double emin = MinKinEnergy();
    G4double emax = MaxKinEnergy();
    if(0 == mType) selectedModel = new G4KleinNishinaCompton();
    selectedModel->SetLowEnergyLimit(emin);
    selectedModel->SetHighEnergyLimit(emax);
    AddEmModel(1, selectedModel);
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ComptonScattering70::PrintInfo()
{
  G4cout << " Total cross sections has a good parametrisation"
         << " from 10 KeV to (100/Z) GeV" 
         << "\n      Sampling according " << selectedModel->GetName() << " model" 
	 << G4endl;
}         

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4ComptonScattering70::SetModel(const G4String& s)
{
  if(s == "Klein-Nishina") mType = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

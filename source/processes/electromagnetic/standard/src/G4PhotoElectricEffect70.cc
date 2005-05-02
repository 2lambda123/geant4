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
// $Id: G4PhotoElectricEffect70.cc,v 1.2 2005-05-02 12:44:18 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//------------------ G4PhotoElectricEffect physics process -------------------------
//                   by Michel Maire, 24 May 1996
//
// 12-06-96, Added SelectRandomAtom() method, by M.Maire
// 21-06-96, SetCuts implementation, M.Maire
// 17-09-96, PartialSumSigma(i)
//           split of ComputeBindingEnergy, M.Maire
// 08-01-97, crossection table + meanfreepath table, M.Maire
// 13-03-97, adapted for the new physics scheme, M.Maire
// 28-03-97, protection in BuildPhysicsTable, M.Maire
// 04-06-98, in DoIt, secondary production condition:
//                        range > std::min(threshold,safety)
// 13-08-98, new methods SetBining() PrintInfo()
// 17-11-98, use table of Atomic shells in PostStepDoIt
// 06-01-99, use Sandia crossSection below 50 keV, V.Grichine mma
// 20-05-99, protection against very low energy photons ,L.Urban
// 08-06-99, removed this above protection from the DoIt. mma
// 21-06-00, in DoIt, killing photon: aParticleChange.SetEnergyChange(0.); mma
// 22-06-00, in DoIt, absorbe very low energy photon (back to 20-05-99); mma
// 22-02-01, back to 08-06-99 after correc in SandiaTable (materials-V03-00-05)
// 28-05-01, V.Ivanchenko minor changes to provide ANSI -wall compilation
// 13-07-01, DoIt: suppression of production cut of the electron (mma)
// 06-08-01, new methods Store/Retrieve PhysicsTable (mma)
// 06-08-01, BuildThePhysicsTable() called from constructor (mma)
// 17-09-01, migration of Materials to pure STL (mma)
// 20-09-01, DoIt: fminimalEnergy of generated electron = 1*eV (mma)
// 01-10-01, come back to BuildPhysicsTable(const G4ParticleDefinition&)
// 10-01-02, moved few function from icc to cc
// 17-04-02, Keep only Sandia crossSections. Remove BuildPhysicsTables.
//           Simplify public interface (mma)
// 29-04-02, Generate theta angle of the photoelectron from Sauter-Gavrila
//           distribution (mma)
// 15-01-03, photoelectron theta ditribution : return costeta=1 if gamma>5
//           (helmut burkhardt)
// 21-04-05  Migrate to model interface and inherit from G4VEmProcess (V.Ivanchenko)
// -----------------------------------------------------------------------------

#include "G4PhotoElectricEffect70.hh"
#include "G4PEEffectModel.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

G4PhotoElectricEffect70::G4PhotoElectricEffect70(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4PhotoElectricEffect70::~G4PhotoElectricEffect70()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4PhotoElectricEffect70::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    //    SetVerboseLevel(1);
    SetBuildTableFlag(false);
    SetSecondaryParticle(G4Electron::Electron());
    G4VEmModel* model = new G4PEEffectModel();
    model->SetLowEnergyLimit(MinKinEnergy());
    model->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, model);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4PhotoElectricEffect70::PrintInfo()
{
  G4cout << " Total cross sections from Sandia parametrisation. "
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

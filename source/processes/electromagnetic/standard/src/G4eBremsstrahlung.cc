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
// $Id: G4eBremsstrahlung.cc,v 1.41 2005-03-28 23:07:54 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4eBremsstrahlung
//
// Author:        Michel Maire
//
// Creation date: 26.06.1996
//
// Modifications:
//
// 26-09-96 extension of the total crosssection above 100 GeV, M.Maire
//  1-10-96 new type G4OrderedTable; ComputePartialSumSigma(), M.Maire
// 16-10-96 DoIt() call to the non static GetEnergyCuts(), L.Urban
// 13-12-96 Sign corrected in grejmax and greject
//          error definition of screenvar, L.Urban
// 20-03-97 new energy loss+ionisation+brems scheme, L.Urban
// 07-04-98 remove 'tracking cut' of the diffracted particle, MMa
// 13-08-98 new methods SetBining() PrintInfo()
// 03-03-99 Bug fixed in LPM effect, L.Urban
// 10-02-00 modifications , new e.m. structure, L.Urban
// 07-08-00 new cross section/en.loss parametrisation, LPM flag , L.Urban
// 21-09-00 corrections in the LPM implementation, L.Urban
// 28-05-01 V.Ivanchenko minor changes to provide ANSI -wall compilation
// 09-08-01 new methods Store/Retrieve PhysicsTable (mma)
// 17-09-01 migration of Materials to pure STL (mma)
// 21-09-01 completion of RetrievePhysicsTable() (mma)
// 29-10-01 all static functions no more inlined (mma)
// 08-11-01 particleMass becomes a local variable
// 30-04-02 V.Ivanchenko update to new design
// 23-12-02 Change interface in order to move to cut per region (VI)
// 26-12-02 Secondary production moved to derived classes (VI)
// 23-05-03 Define default integral + BohrFluctuations (V.Ivanchenko)
// 08-08-03 STD substitute standard  (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 04-11-04 add gamma threshold (V.Ivanchenko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4eBremsstrahlung.hh"
#include "G4Gamma.hh"
#include "G4eBremsstrahlungModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4UnitsTable.hh"

#include "G4ProductionCutsTable.hh"
#include "G4MaterialCutsCouple.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;
 
G4eBremsstrahlung::G4eBremsstrahlung(const G4String& name, G4double thresh):
  G4VEnergyLossProcess(name), 
  gammaThreshold(thresh),
  isInitialised(false)
{
  SetDEDXBinning(120);
  SetLambdaBinning(120);
  SetMinKinEnergy(0.1*keV);
  SetMaxKinEnergy(100.0*TeV);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4eBremsstrahlung::~G4eBremsstrahlung()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eBremsstrahlung::InitialiseEnergyLossProcess(const G4ParticleDefinition* p,
                                                    const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    particle = p;
    SetSecondaryParticle(G4Gamma::Gamma());
    SetIonisation(false);

    //G4VEmFluctuationModel* fm =  0;
    G4VEmFluctuationModel* fm = new G4UniversalFluctuation();

    G4VEmModel* em = new G4eBremsstrahlungModel();
    em->SetLowEnergyLimit(0.1*keV);
    em->SetHighEnergyLimit(100.0*TeV);
    AddEmModel(1, em, fm);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eBremsstrahlung::PrintInfoDefinition()
{
  G4VEnergyLossProcess::PrintInfoDefinition();

  G4cout << "      Total cross sections from a parametrisation"
         << " based on the EEDL data library. " 
         << G4endl
         << "      Good description from 1 KeV to 100 GeV, "
	 << "log scale extrapolation above 100 GeV."
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... 

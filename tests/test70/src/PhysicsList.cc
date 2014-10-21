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
// -------------------------------------------------------------------
// $Id$
// -------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4EmDNAChemistry.hh"
#include "G4EmDNAPhysics.hh"
#include "G4ITStepManager.hh"
#include "G4DNAChemistryManager.hh"
#include "G4DNAMolecularStepByStepModel.hh"
#include "G4DNASmoluchowskiReactionModel.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() :
    G4VModularPhysicsList(), fpChemList(0)
{
  defaultCutValue = 1 * nanometer;

  SetVerboseLevel(1);
  RegisterPhysics(new G4EmDNAPhysics());

  if (G4DNAChemistryManager::Instance()->IsChemistryActivated())
  {
    fpChemList = new G4EmDNAChemistry();
    G4DNAChemistryManager::Instance()->SetChemistryList(fpChemList);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PhysicsList::~PhysicsList()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::ConstructParticle()
{
  if (G4DNAChemistryManager::Instance()->IsChemistryActivated())
  {
    fpChemList->ConstructMolecule();
  }
  G4VModularPhysicsList::ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
  if (G4DNAChemistryManager::Instance()->IsChemistryActivated())
  {
    fpChemList->ConstructProcess();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

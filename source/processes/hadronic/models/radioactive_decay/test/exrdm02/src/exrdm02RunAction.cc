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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: exrdm02RunAction.cc,v 1.1 2003-10-08 16:31:51 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "exrdm02RunActionMessenger.hh"
#include "exrdm02RunAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
//#include "Histo.hh"

#include <fstream>
#include <iomanip>
#include <vector>

//using namespace std;

//extern G4String filename;

extern std::vector<G4String> Particles;
extern std::vector<G4double> Energies;
extern std::vector<G4double> Weights;
extern std::vector<G4double> Times;
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

exrdm02RunAction::exrdm02RunAction()
  : fileName("rdmex2.log")
{
  runMessenger = new exrdm02RunActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

exrdm02RunAction::~exrdm02RunAction()
{
  delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void exrdm02RunAction::BeginOfRunAction(const G4Run* aRun)
{
 
  G4int RunN = aRun->GetRunID();
  if ( RunN % 1000 == 0 ) 
    G4cout << "### Run : " << RunN << G4endl;

  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager* UI = G4UImanager::GetUIpointer(); 
      UI->ApplyCommand("/vis/clear/view");
      UI->ApplyCommand("/vis/draw/current");
    } 
  
  Particles.clear();
  Energies.clear();
  Weights.clear();
  Times.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void exrdm02RunAction::EndOfRunAction(const G4Run* )
{
  
  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/show/view");
  std::ofstream outscat(fileName, std::ios::app);

  for (G4int i=0; i<Particles.size();i++) {

    outscat 
      << std::setiosflags(std::ios::fixed)
      << std::setprecision(5)
      << std::setiosflags(std::ios::right)
      << std::setw(12)
      << Energies[i]
      << std::setw(12)<<std::setprecision(5) 
      << std::setiosflags(std::ios::scientific)
      << std::setiosflags(std::ios::right)
      << Weights[i]
      << std::setw(12)<<std::setprecision(5)
      << std::setiosflags(std::ios::scientific)
      << std::setiosflags(std::ios::right)
      << Times[i] << "     "
      << Particles[i]
      << G4endl ;    
  }
  outscat << G4endl;
  outscat.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....





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
//
// $Id: G4FluoDataTest.cc,v ????
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     G4DataSetManagerTest
//
//      Author:        Elena Guardincerri
// 
//      Creation date: 6 August 2001
//
//      Modifications: 26 April 2002 -- AM
//
// -------------------------------------------------------------------

#include "globals.hh"
#include "G4DataVector.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>

#include "G4FluoData.hh"

int main()
{
  G4cout.setf( std::ios::scientific, std::ios::floatfield );

  G4cout << "Enter Z" << G4endl;
  G4int Z;
  G4cin >> Z;
  
  G4FluoData* dataSet = new G4FluoData();

  dataSet->LoadData(Z);
 
  G4int vac= dataSet->NumberOfVacancies();
  G4cout << "The atom of atomic number "<<Z<<" has "
	 << vac<<" vacancies "<<G4endl;
  G4cout << "Enter the index of the vacancy" << G4endl;
  G4int vacancyIndex;
  G4cin >> vacancyIndex;
  
 
   G4int n = dataSet->NumberOfTransitions(vacancyIndex);
   G4int id = dataSet->VacancyId(vacancyIndex);
  G4cout << " The shell whose index is " <<vacancyIndex  
<< " has identity " << id << G4endl;
  G4cout <<" Electrons can reach it from "<< n <<" shells."<<G4endl;
 G4cout << "Enter the index of the starting shell" << G4endl;
  G4int initIndex;
  G4cin >>initIndex;
  G4int startingShellId = dataSet->StartShellId(initIndex,vacancyIndex);
G4double startingShellEnergy = dataSet-> StartShellEnergy(initIndex,vacancyIndex);
 G4double startingShellProb = dataSet-> StartShellProb(initIndex,vacancyIndex);
 G4cout <<" The identity of the starting shell is "<<startingShellId<<G4endl;
 G4cout<<" The energy of the transition to the final shell is "
<< startingShellEnergy<< " MeV "<<G4endl;
 G4cout<<" The probability of the transition to the final shell is "
       <<startingShellProb <<G4endl;

  G4cout << "PRINT DATA"<<G4endl;

  dataSet->PrintData();
 
delete dataSet;

G4cout << "END OF THE MAIN PROGRAM" << G4endl;
}














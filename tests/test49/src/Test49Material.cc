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
// -------------------------------------------------------------
//      GEANT 4 class 
//
//      History: based on object model of
//      ---------- Test49Material -------
// 
//      Converted from Test49 to Test49 by Mikhail Kossov, 29 Jan 2004
//
//=====================================================================

#include "Test49Material.hh"

#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"

Test49Material::Test49Material()
{
	Initialise();
}

Test49Material::~Test49Material()
{}

void Test49Material::Initialise()
{
  G4Material* ma;

  ma  = new G4Material("H",     1.,  1.0*g/mole, 1.*g/cm3);
  ma  = new G4Material("D",     1.,  2.0*g/mole, 1.*g/cm3);
  ma  = new G4Material("Li",    3.,  6.941*g/mole, 1.*g/cm3);
  ma  = new G4Material("Be",    4.,  9.01*g/mole, 1.848*g/cm3);
  ma  = new G4Material("C",     6.,  12.00*g/mole, 2.0*g/cm3);
	 ma  = new G4Material("Graphite",6., 12.00*g/mole, 2.265*g/cm3 );
  ma->SetChemicalFormula("Graphite");
  ma  = new G4Material("Al",    13.,  26.98*g/mole,  2.7 *g/cm3);
	 ma  = new G4Material("Si",    14.,  29.055*g/mole, 2.33*g/cm3);
	 ma  = new G4Material("LAr",   18.,  39.95*g/mole,  1.393*g/cm3);
	 ma  = new G4Material("Zr",    40.,  91.224*g/mole, 4.0*g/cm3);
	 ma  = new G4Material("LXe",   54., 131.29*g/mole,  3.02*g/cm3);
	 ma  = new G4Material("Fe",    26.,  55.85*g/mole,  7.87*g/cm3);
	 ma  = new G4Material("Ni",    29.,  58.6934*g/mole,  8.00*g/cm3);
	 ma  = new G4Material("Cu",    29.,  63.55*g/mole,  8.96*g/cm3);
	 ma  = new G4Material("Au",    79., 196.97*g/mole, 19.32*g/cm3);
	 ma  = new G4Material("Ta",    73., 180.9479*g/mole, 16.67*g/cm3);
	 ma  = new G4Material("W",     74., 183.85*g/mole, 19.30*g/cm3);
	 ma  = new G4Material("Pb",    82., 207.19*g/mole, 11.35*g/cm3);
	 ma  = new G4Material("Bi",    83., 208.98*g/mole, 12.*g/cm3);
	 ma  = new G4Material("U",     92., 238.03*g/mole, 18.95*g/cm3);

  G4Element*   H  = new G4Element ("Hydrogen", "H",   1. ,  1.01*g/mole);
  G4Element*   N  = new G4Element ("Nitrigen", "N",   7. , 14.00*g/mole);
  G4Element*   O  = new G4Element ("Oxygen"  , "O",   8. , 16.00*g/mole);
  G4Element*   C  = new G4Element ("Carbon"  , "C",   6. , 12.00*g/mole);
  G4Element*  Cs  = new G4Element ("Cesium"  , "Cs", 55. , 132.905*g/mole);
  G4Element*   I  = new G4Element ("Iodide"  , "I",  53. , 126.9044*g/mole);

  ma = new G4Material("O2", 8., 16.00*g/mole, 1.1*g/cm3);
  ma->SetChemicalFormula("O_2");
  ma = new G4Material ("Water" , 1.*g/cm3, 2);
  ma->AddElement(H,2);
  ma->AddElement(O,1);
  ma->SetChemicalFormula("H_2O");
  ma = new G4Material ("Ethane" , 0.4241*g/cm3, 2);
  ma->AddElement(H,6);
  ma->AddElement(C,2);
  ma->SetChemicalFormula("C_2H_6");
  ma = new G4Material ("CsI" , 4.53*g/cm3, 2);
  ma->AddElement(Cs,1);
  ma->AddElement(I,1);
  ma->SetChemicalFormula("CsI");
  ma = new G4Material("Air"  , 1.290*mg/cm3, 2);
	// use fraction in mass
  ma->AddElement(N, 0.7);
  ma->AddElement(O, 0.3);

}


G4Material* Test49Material::GetMaterial(const G4String& name)
{ 

  //  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

	G4Material* ma = G4Material::GetMaterial(name);
	
  G4cout << "Material is selected: " << ma->GetName() << G4endl;
  return ma;
}	

  







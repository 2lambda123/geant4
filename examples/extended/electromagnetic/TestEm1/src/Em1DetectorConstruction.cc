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

//
// $Id: Em1DetectorConstruction.cc,v 1.11 2003-03-11 18:00:35 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Em1DetectorConstruction.hh"
#include "Em1DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4RunManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Em1DetectorConstruction::Em1DetectorConstruction()
:pBox     (0), lBox (0),
 aMaterial(0),
 magField (0)
{
  // create commands for interactive definition of the detector
  DefineMaterials();
  BoxSize = 20*m;
  detectorMessenger = new Em1DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Em1DetectorConstruction::~Em1DetectorConstruction()
{ delete detectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* Em1DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Em1DetectorConstruction::DefineMaterials()
{
 //This function illustrates the possible ways to define materials

  G4String name, symbol;             //a=mass of a mole;
  G4double a, z, density;            //z=mean number of protons;

  G4int ncomponents, natoms;
  G4double fractionmass;

  G4Material* ma;
//
// define Elements
//

  a = 1.01*g/mole;
  G4Element* H = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);

  a = 14.01*g/mole;
  G4Element* N = new G4Element(name="Nitrogen",symbol="N" , z= 7., a);

  a = 16.00*g/mole;
  G4Element* O = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);

//
// define materials
//

  density = 1.290*mg/cm3;
  G4Material* Air = new G4Material(name="Air"  , density, ncomponents=2);
  Air->AddElement(N, fractionmass=70.*perCent);
  Air->AddElement(O, fractionmass=30.*perCent);

  density = 70.8*mg/cm3;
  G4Material* H2l = new G4Material(name="H2liquid", density, ncomponents=1);
  H2l->AddElement(H, fractionmass=1.);

  density = 1.000*g/cm3;
  G4Material* H2O = new G4Material(name="Water", density, ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
  H2O->SetChemicalFormula("H_2O");
  G4double exc = H2O->GetIonisation()->FindMeanExcitationEnergy("H_2O");
  H2O->GetIonisation()->SetMeanExcitationEnergy(exc);

  density = 1.390*g/cm3;
  a = 39.95*g/mole;
  ma = new G4Material(name="liquidArgon", z=18., a, density);

  density = 2.700*g/cm3;
  a = 26.98*g/mole;
  G4Material* Al  = new G4Material(name="Aluminium"  , z=13., a, density);

  density = 2.330*g/cm3;
  a = 28.09*g/mole;
  ma  = new G4Material(name="Silicon"    , z=14., a, density);

  density = 5.323*g/cm3;
  a = 72.61*g/mole;
  ma  = new G4Material(name="Germanium"  , z=32., a, density);

  density = 7.870*g/cm3;
  a = 55.85*g/mole;
  ma  = new G4Material(name="Iron"       , z=26., a, density);

  density = 19.30*g/cm3;
  a = 183.85*g/mole;
  ma   = new G4Material(name="Tungsten"   , z=74., a, density);

  density = 11.35*g/cm3;
  a = 207.19*g/mole;
  ma = new G4Material(name="Lead"        , z=82., a, density);

  density = 18.95*g/cm3;
  a = 238.03*g/mole;
  ma = new G4Material(name="Uranium"     , z=92., a, density);


  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  //default materials of the calorimeter
  aMaterial = Al;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* Em1DetectorConstruction::ConstructVolumes()
{
  if(lBox) delete lBox;
  if(pBox) delete pBox;
  G4Box*
  sBox = new G4Box("Container",				//its name
                   BoxSize/2,BoxSize/2,BoxSize/2);	//its dimensions

  lBox = new G4LogicalVolume(sBox,			//its shape
                             aMaterial,			//its material
                             aMaterial->GetName());	//its name

  pBox = new G4PVPlacement(0,				//no rotation
  			   G4ThreeVector(),		//at (0,0,0)
                           aMaterial->GetName(),	//its name
                           lBox,			//its logical volume
                           0,				//its mother  volume
                           false,			//no boolean operation
                           0);				//copy number

  //
  // Visualization attributes
  //

  G4VisAttributes* visAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAtt->SetVisibility(true);
  lBox->SetVisAttributes(visAtt);

  //
  //always return the root volume
  //

  PrintParameters();
  return pBox;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Em1DetectorConstruction::PrintParameters()
{
  G4cout << "\n The Box is " << G4BestUnit(BoxSize,"Length")
         << " of " << aMaterial->GetName() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Em1DetectorConstruction::SetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial)
     {aMaterial = pttoMaterial;
      lBox->SetMaterial(aMaterial);
     }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Em1DetectorConstruction::SetSize(G4double value)
{
  BoxSize = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Em1DetectorConstruction::SetMagField(G4double fieldValue)
{
  //apply a global uniform magnetic field along Z axis
  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if (magField) delete magField;	//delete the existing magn field

  if (fieldValue!=0.)			// create a new one if non nul
    {
      magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
      fieldMgr->SetDetectorField(magField);
      fieldMgr->CreateChordFinder(magField);
    }
   else
    {
      magField = 0;
      fieldMgr->SetDetectorField(magField);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Em1DetectorConstruction::UpdateGeometry()
{
  G4bool first = true;
  if (pBox) first = false;
  G4VPhysicalVolume* v = ConstructVolumes();
  G4RunManager* rm = G4RunManager::GetRunManager();
  rm->GeometryHasBeenModified();
  rm->DefineWorldVolume(v);
  if (!first) rm->ResetNavigator();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

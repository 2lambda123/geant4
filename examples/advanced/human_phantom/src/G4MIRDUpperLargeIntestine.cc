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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//
#include "G4MIRDUpperLargeIntestine.hh"

#include "globals.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4EllipticalTube.hh"
#include "G4UnionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4HumanPhantomMaterial.hh"

G4MIRDUpperLargeIntestine::G4MIRDUpperLargeIntestine()
{
}

G4MIRDUpperLargeIntestine::~G4MIRDUpperLargeIntestine()
{
}

G4VPhysicalVolume* G4MIRDUpperLargeIntestine::ConstructUpperLargeIntestine(G4VPhysicalVolume* mother, G4String sex, G4bool sensitivity)
{
G4cout << "ConstructUpperLargeIntestine for " << sex << G4endl;
 
 G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
 G4Material* soft = material -> GetMaterial("soft_tissue");
 delete material;

  G4double dx = 2.16 * cm;
  G4double dy = 2.45 * cm;
  G4double dz = 4.3 * cm;

  G4VSolid* AscendingColonUpperLargeIntestine = new G4EllipticalTube("AscendingColon",dx, dy, dz);
 
  dx = 2.45 * cm;
  dy = 1.35 *cm;
  dz = 9.06* cm;

  G4VSolid* TraverseColonUpperLargeIntestine = new G4EllipticalTube("TraverseColon",dx, dy, dz);

  G4RotationMatrix* relative_rm =  new G4RotationMatrix();
  relative_rm -> rotateX(90. * degree);
  relative_rm -> rotateZ(0. * degree);
  relative_rm -> rotateY(90. * degree);
  G4UnionSolid* upperLargeIntestine = new G4UnionSolid("UpperLargeIntestine",
						      AscendingColonUpperLargeIntestine,
						      TraverseColonUpperLargeIntestine,
						      relative_rm, 
						      G4ThreeVector(7.2 *cm, 0.0, 5.65 * cm));


  G4LogicalVolume* logicUpperLargeIntestine = new G4LogicalVolume(upperLargeIntestine, soft,
								  "UpperLargeIntestineVolume", 
								  0, 0, 0);
 
  G4VPhysicalVolume* physUpperLargeIntestine = new G4PVPlacement(0,
								 G4ThreeVector(-7.33 * cm, -2.31 *cm,-14.22 *cm),
      			       "physicalUpperLargeIntestine",
  			       logicUpperLargeIntestine,
			       mother,
			       false,
			       0);

  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicUpperLargeIntestine->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  G4VisAttributes* UpperLargeIntestineVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  UpperLargeIntestineVisAtt->SetForceSolid(true);
  logicUpperLargeIntestine->SetVisAttributes(UpperLargeIntestineVisAtt);

  G4cout << "UpperLargeIntestine created !!!!!!" << G4endl;

  // Testing UpperLargeIntestine Volume
  G4double UpperLargeIntestineVol = logicUpperLargeIntestine->GetSolid()->GetCubicVolume();
  G4cout << "Volume of UpperLargeIntestine = " << UpperLargeIntestineVol/cm3 << " cm^3" << G4endl;
  
  // Testing UpperLargeIntestine Material
  G4String UpperLargeIntestineMat = logicUpperLargeIntestine->GetMaterial()->GetName();
  G4cout << "Material of UpperLargeIntestine = " << UpperLargeIntestineMat << G4endl;
  
  // Testing Density
  G4double UpperLargeIntestineDensity = logicUpperLargeIntestine->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << UpperLargeIntestineDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double UpperLargeIntestineMass = (UpperLargeIntestineVol)*UpperLargeIntestineDensity;
  G4cout << "Mass of UpperLargeIntestine = " << UpperLargeIntestineMass/gram << " g" << G4endl;

  
  return physUpperLargeIntestine;
}

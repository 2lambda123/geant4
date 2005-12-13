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
// $Id: Tst33ParallelGeometry.cc,v 1.11 2005-12-13 07:14:49 ahoward Exp $
// GEANT4 tag 
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// Tst33ParallelGeometry.cc
//
// ----------------------------------------------------------------------

#include "Tst33ParallelGeometry.hh"

#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryCell.hh"

Tst33ParallelGeometry::Tst33ParallelGeometry()
  :
  fWorldVolume(0),
  fGalactic(0)
{
  Construct();
}
Tst33ParallelGeometry::~Tst33ParallelGeometry(){}

void Tst33ParallelGeometry::Construct(){
  fGalactic = fMaterialFactory.CreateGalactic();

  //////////////////////////////////
  // parallel world cylinder volume
  //////////////////////////////////

  // parallel world solid larger than in the mass geometry

  G4double innerRadiusCylinder = 0*cm;
  G4double outerRadiusCylinder = 220*cm;
  G4double hightCylinder       = 220*cm;
  G4double startAngleCylinder  = 0*deg;
  G4double spanningAngleCylinder    = 360*deg;

  G4Tubs *worldCylinder = new G4Tubs("worldCylinder",
                                     innerRadiusCylinder,
                                     outerRadiusCylinder,
                                     hightCylinder,
                                     startAngleCylinder,
                                     spanningAngleCylinder);

  // logical world

  G4LogicalVolume *worldCylinder_log = 
    new G4LogicalVolume(worldCylinder, fGalactic, "worldCylinder_log");

  G4String name("parallelWorld");
  fWorldVolume = new 
    G4PVPlacement(0, G4ThreeVector(0,0,0), worldCylinder_log,
		  name, 0, false, 0);
  if (!fWorldVolume) {
    G4Exception("Tst33ParallelGeometry::Construct(): new failed to create G4PVPlacement!");
  }
  fPVolumeStore.AddPVolume(G4GeometryCell(*fWorldVolume, -1));




  // creating 18 slobs of 10 cm thicknes

  G4double innerRadiusShield = 0*cm;
  G4double outerRadiusShield = 200*cm;
  G4double hightShield       = 10*cm;
  G4double startAngleShield  = 0*deg;
  G4double spanningAngleShield    = 360*deg;

  G4Tubs *aShield = new G4Tubs("aShield",
                               innerRadiusShield,
                               outerRadiusShield,
                               hightShield,
                               startAngleShield,
                               spanningAngleShield);

  G4Tubs *aShieldI1 = new G4Tubs("aShieldI1",
				 innerRadiusShield,
				 50*cm,
				 1*cm,
				 startAngleShield,
				 spanningAngleShield);

  
  // logical parallel cells


  G4LogicalVolume *aShield_logI1 = 
    new G4LogicalVolume(aShieldI1, fGalactic, "aShieldI1_log");

  // physical parallel cells

  G4int i = 1;
  G4double startz = -170*cm; 
  for (i=1; i<=18; ++i) {
   
    name = fPVolumeStore.GetCellName(i);
    G4LogicalVolume *aShield_log = 
      new G4LogicalVolume(aShield, fGalactic, "aShield_log");
    
    G4VPhysicalVolume *pvolIMinus = 
      new G4PVPlacement(0, 
			G4ThreeVector(0, 0, -0.5*hightShield),
			aShield_logI1, 
			name + "I1-", 
			aShield_log, 
			false, 
			0);
    
    G4VPhysicalVolume *pvolIPlus = 
      new G4PVPlacement(0, 
			G4ThreeVector(0, 0, +0.5*hightShield),
			aShield_logI1, 
			name + "I1+", 
			aShield_log, 
			false, 
			0);
    
    G4double pos_x = 0*cm;
    G4double pos_y = 0*cm;
    G4double pos_z = startz + (i-1) * (2*hightShield);
    G4VPhysicalVolume *pvol = 
      new G4PVPlacement(0, 
			G4ThreeVector(pos_x, pos_y, pos_z),
			aShield_log, 
			name, 
			worldCylinder_log, 
			false, 
			0);
    G4GeometryCell cell(*pvol, 0);
    fPVolumeStore.AddPVolume(cell);
    G4GeometryCell cellM(*pvolIMinus, 0);
    fPVolumeStore.AddPVolume(cellM);
    G4GeometryCell cellP(*pvolIPlus, 0);
    fPVolumeStore.AddPVolume(cellP);    
  }

  // filling the rest of the world volumr behind the concrete with
  // another slob which should get the same importance value as the 
  // last slob
  innerRadiusShield = 0*cm;
  outerRadiusShield = 220*cm;
  hightShield       = 20*cm;
  startAngleShield  = 0*deg;
  spanningAngleShield    = 360*deg;

  G4Tubs *aRest = new G4Tubs("Rest",
			     innerRadiusShield,
			     outerRadiusShield,
			     hightShield,
			     startAngleShield,
			     spanningAngleShield);
  
  G4LogicalVolume *aRest_log = 
    new G4LogicalVolume(aRest, fGalactic, "aRest_log");
  name = fPVolumeStore.GetCellName(19);
    
  G4double pos_x = 0*cm;
  G4double pos_y = 0*cm;
  G4double pos_z = 200*cm;
  G4VPhysicalVolume *pvol = 
    new G4PVPlacement(0, 
		      G4ThreeVector(pos_x, pos_y, pos_z),
		      aRest_log, 
		      name, 
		      worldCylinder_log, 
		      false, 
		      0);
  G4GeometryCell cell(*pvol, 0);
  fPVolumeStore.AddPVolume(cell);
  

}

G4VPhysicalVolume &Tst33ParallelGeometry::GetWorldVolume() const{
  return *fWorldVolume;
}


G4GeometryCell Tst33ParallelGeometry::
GetGeometryCell(G4int i, const G4String &nameExt) const {
  return fPVolumeStore.GetGeometryCell(i, nameExt);
}

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
// $Id: MedLinacPhantomROGeometry.cc,v 1.2 2004-04-02 17:48:03 mpiergen Exp $
//
//
// Code developed by: M. Piergentili
//
#include "MedLinacPhantomROGeometry.hh"
#include "MedLinacDummySD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"

MedLinacPhantomROGeometry::MedLinacPhantomROGeometry(G4String aString,
                                                 G4double phantomDim_x,
					         G4double phantomDim_y,
                                                 G4double phantomDim_z,
                                                 G4int numberOfVoxelsX,
					         G4int numberOfVoxelsY,
                                                 G4int numberOfVoxelsZ):
  G4VReadOutGeometry(aString),
  PhantomDimensionX(phantomDim_x),
  PhantomDimensionY(phantomDim_y),
  PhantomDimensionZ(phantomDim_z),
  NumberOfVoxelsAlongX(numberOfVoxelsX),
  NumberOfVoxelsAlongY(numberOfVoxelsY),
  NumberOfVoxelsAlongZ(numberOfVoxelsZ)
{}
MedLinacPhantomROGeometry::~MedLinacPhantomROGeometry()
{
}

G4VPhysicalVolume* MedLinacPhantomROGeometry::Build()
{
 
  G4cout << " MedLinacPhantomROGeometry::Build()-----------------------" << G4endl;
  
  // A dummy material is used to fill the volumes of the readout geometry.
  // (It will be allowed to set a NULL pointer in volumes of such virtual
  // division in future, since this material is irrelevant for tracking.)

  G4Material* dummyMat = new G4Material(name="dummyMat", 1., 1.*g/mole, 1.*g/cm3);

  G4double expHall_x = 3.0*m;
  G4double expHall_y = 3.0*m;
  G4double expHall_z = 3.0*m;

  // variables for x division ...
  G4double halfXVoxelDimensionX = PhantomDimensionX/NumberOfVoxelsAlongX;
  G4double halfXVoxelDimensionZ = PhantomDimensionZ;
  G4double voxelXThickness = 2*halfXVoxelDimensionX;

 // variables for y division ...
  G4double halfZVoxelDimensionX = PhantomDimensionX; 
  G4double halfZVoxelDimensionZ = PhantomDimensionZ/NumberOfVoxelsAlongZ;
  G4double voxelZThickness = 2*halfZVoxelDimensionZ;



  // world volume of ROGeometry ...
  G4Box *ROWorld = new G4Box("ROWorld",
			     expHall_x,
			     expHall_y,
			     expHall_z);

  G4LogicalVolume *ROWorldLog = new G4LogicalVolume(ROWorld,
						    dummyMat,
						    "ROWorldLog",
						    0,0,0);

  G4VPhysicalVolume *ROWorldPhys = new G4PVPlacement(0,
						     G4ThreeVector(),
						     "ROWorldPhys",
						     ROWorldLog,
						     0,false,0);
  
  // phantom ROGeometry ... 
  G4Box *ROPhantom = new G4Box("ROPhantom", 
			       PhantomDimensionX, 
			       PhantomDimensionY, 
			       PhantomDimensionZ);

  G4LogicalVolume *ROPhantomLog = new G4LogicalVolume(ROPhantom,
						      dummyMat,
						      "ROPhantomLog",
						      0,0,0);

  G4VPhysicalVolume *ROPhantomPhys = new G4PVPlacement(0,
						       G4ThreeVector(),
						       "PhantomPhys",
                                                       ROPhantomLog,
                                                       ROWorldPhys,
                                                       false,0);
 
  // ROGeomtry: Voxel division
 
  // X division first... 
  G4Box *ROPhantomXDivision = new G4Box("ROPhantomXDivision",
					halfXVoxelDimensionX,
					halfXVoxelDimensionZ,
					halfXVoxelDimensionZ);

  G4LogicalVolume *ROPhantomXDivisionLog = new G4LogicalVolume(ROPhantomXDivision,
							       dummyMat,
							       "ROPhantomXDivisionLog",
							       0,0,0);

  G4VPhysicalVolume *ROPhantomXDivisionPhys = new G4PVReplica("ROPhantomXDivisionPhys",
                                                              ROPhantomXDivisionLog,
                                                              ROPhantomPhys,
                                                              kXAxis,
                                                              NumberOfVoxelsAlongX,
                                                              voxelXThickness);
  // ...then Z division
  
  G4Box *ROPhantomZDivision = new G4Box("ROPhantomZDivision",
					halfZVoxelDimensionX,
					halfZVoxelDimensionX, 
					halfZVoxelDimensionZ);

  G4LogicalVolume *ROPhantomZDivisionLog = new G4LogicalVolume(ROPhantomZDivision,
							       dummyMat,
							       "ROPhantomZDivisionLog",
							       0,0,0);

  G4VPhysicalVolume *ROPhantomZDivisionPhys = new G4PVReplica("ROPhantomZDivisionPhys",
							      ROPhantomZDivisionLog,
							      ROPhantomXDivisionPhys,
							      kZAxis,
							      NumberOfVoxelsAlongZ,
							      voxelZThickness);
  // ...then Y  division

  G4Box *ROPhantomYDivision = new G4Box("ROPhantomYDivision",
					halfZVoxelDimensionX, 
					halfZVoxelDimensionX,
					halfZVoxelDimensionX);

  G4LogicalVolume *ROPhantomYDivisionLog = new G4LogicalVolume(ROPhantomYDivision,
							       dummyMat,
							       "ROPhantomYDivisionLog",
							       0,0,0);
 
  ROPhantomYDivisionPhys = new G4PVReplica("ROPhantomYDivisionPhys",
							      ROPhantomYDivisionLog,
							      ROPhantomZDivisionPhys,
							      kYAxis,
							      NumberOfVoxelsAlongZ,
							      voxelZThickness);
  MedLinacDummySD *dummySD = new  MedLinacDummySD;
  ROPhantomYDivisionLog->SetSensitiveDetector(dummySD);

  return ROWorldPhys;
}




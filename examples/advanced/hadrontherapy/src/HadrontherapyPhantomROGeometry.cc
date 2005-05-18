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

#include "HadrontherapyPhantomROGeometry.hh"
#include "HadrontherapyDummySD.hh"

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
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

HadrontherapyPhantomROGeometry::HadrontherapyPhantomROGeometry(G4String aString,
							       G4double phantomDimX,
							       G4double phantomDimY,
							       G4double phantomDimZ,
							       G4int numberOfVoxelsX,
							       G4int numberOfVoxelsY,
							       G4int numberOfVoxelsZ):
G4VReadOutGeometry(aString),
phantomDimensionX(phantomDimX),
phantomDimensionY(phantomDimY),
phantomDimensionZ(phantomDimZ),
numberOfVoxelsAlongX(numberOfVoxelsX),
numberOfVoxelsAlongY(numberOfVoxelsY),
numberOfVoxelsAlongZ(numberOfVoxelsZ)
{
}
// -----------------------------------------------------
HadrontherapyPhantomROGeometry::~HadrontherapyPhantomROGeometry()
{
}
// -----------------------------------------------------
G4VPhysicalVolume* HadrontherapyPhantomROGeometry::Build()
{
// A dummy material is used to fill the volumes of the readout geometry.
// (It will be allowed to set a NULL pointer in volumes of such virtual
// division in future, since this material is irrelevant for tracking.)

G4Material* dummyMat = new G4Material(name="dummyMat", 1., 1.*g/mole, 1.*g/cm3);

G4double worldDimensionX = 200.0 *cm;
G4double worldDimensionY = 200.0 *cm;
G4double worldDimensionZ = 200.0 *cm;


G4double halfPhantomDimensionX = phantomDimensionX;
G4double halfPhantomDimensionY = phantomDimensionY;
G4double halfPhantomDimensionZ = phantomDimensionZ;



// world volume of ROGeometry ...
  G4Box *ROWorld = new G4Box("ROWorld",
			     worldDimensionX,
			     worldDimensionY,
			     worldDimensionZ);

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
			       halfPhantomDimensionX, 
			       halfPhantomDimensionY, 
			       halfPhantomDimensionZ);

  G4LogicalVolume *ROPhantomLog = new G4LogicalVolume(ROPhantom,
						      dummyMat,
						      "ROPhantomLog",
						      0,0,0);

  G4VPhysicalVolume *ROPhantomPhys = new G4PVPlacement(0,
						       G4ThreeVector(-180.0 *mm, 0.0 *mm, 0.0 *mm),
						       "PhantomPhys",
                                                       ROPhantomLog,
                                                       ROWorldPhys,
                                                       false,0);



// ROGeomtry: Voxel division

// variables for x division ...

G4double halfXVoxelDimensionX = halfPhantomDimensionX/numberOfVoxelsAlongX;
G4double halfXVoxelDimensionY = halfPhantomDimensionY;
G4double halfXVoxelDimensionZ = halfPhantomDimensionZ;
G4double voxelXThickness = 2*halfXVoxelDimensionX;


// X division first... 
  G4Box *ROPhantomXDivision = new G4Box("ROPhantomXDivision",
					halfXVoxelDimensionX,
					halfXVoxelDimensionY,
					halfXVoxelDimensionZ);

  G4LogicalVolume *ROPhantomXDivisionLog = new G4LogicalVolume(ROPhantomXDivision,
							       dummyMat,
							       "ROPhantomXDivisionLog",
							       0,0,0);

  G4VPhysicalVolume *ROPhantomXDivisionPhys = new G4PVReplica("ROPhantomXDivisionPhys",
                                                              ROPhantomXDivisionLog,
                                                              ROPhantomPhys,
                                                              kXAxis,
                                                              numberOfVoxelsAlongX,
                                                              voxelXThickness);

// variables for y division ...

G4double halfYVoxelDimensionX =  halfXVoxelDimensionX;
G4double halfYVoxelDimensionY = halfPhantomDimensionY/numberOfVoxelsAlongY;
G4double halfYVoxelDimensionZ = halfPhantomDimensionZ;
G4double voxelYThickness = 2*halfYVoxelDimensionY;



// ...then Y  division

  G4Box *ROPhantomYDivision = new G4Box("ROPhantomYDivision",
					halfYVoxelDimensionX, 
					halfYVoxelDimensionY,
					halfYVoxelDimensionZ);

  G4LogicalVolume *ROPhantomYDivisionLog = new G4LogicalVolume(ROPhantomYDivision,
							       dummyMat,
							       "ROPhantomYDivisionLog",
							       0,0,0);
 
   G4VPhysicalVolume *ROPhantomYDivisionPhys = new G4PVReplica("ROPhantomYDivisionPhys",
							      ROPhantomYDivisionLog,
							      ROPhantomXDivisionPhys,
							      kYAxis,
							      numberOfVoxelsAlongY,
							      voxelYThickness);




// variables for z division ...

G4double halfZVoxelDimensionX = halfXVoxelDimensionX;
G4double halfZVoxelDimensionY = halfYVoxelDimensionY;
G4double halfZVoxelDimensionZ = halfPhantomDimensionZ/numberOfVoxelsAlongZ;
G4double voxelZThickness = 2*halfZVoxelDimensionZ;


//....and ...then Z division
 
 G4Box *ROPhantomZDivision = new G4Box("ROPhantomZDivision",
				       halfZVoxelDimensionX,
				       halfZVoxelDimensionY, 
				       halfZVoxelDimensionZ);
 
 G4LogicalVolume *ROPhantomZDivisionLog = new G4LogicalVolume(ROPhantomZDivision,
							      dummyMat,
							      "ROPhantomZDivisionLog",
							      0,0,0);
 
 ROPhantomZDivisionPhys = new G4PVReplica("ROPhantomZDivisionPhys",
					  ROPhantomZDivisionLog,
					  ROPhantomYDivisionPhys,
					  kZAxis,
					  numberOfVoxelsAlongZ,
					  voxelZThickness);
 



// ------------------------                                        
// Visualization attributes

G4VisAttributes * green = new G4VisAttributes( G4Colour(0. ,1. ,0.));
green -> SetVisibility(true);
green -> SetForceWireframe(true);

G4VisAttributes * brawn = new G4VisAttributes( G4Colour(0.8, 0.5, 0.35));
brawn -> SetVisibility(true);
brawn -> SetForceSolid(true);


 ROPhantomYDivisionLog->SetVisAttributes(green);
 ROPhantomXDivisionLog ->SetVisAttributes(green);
 ROPhantomZDivisionLog ->SetVisAttributes(green);

// -----------------------------------------------------------------------

HadrontherapyDummySD *dummySD = new HadrontherapyDummySD;
ROPhantomXDivisionLog->SetSensitiveDetector(dummySD);
ROPhantomYDivisionLog->SetSensitiveDetector(dummySD);
ROPhantomZDivisionLog->SetSensitiveDetector(dummySD);

  return ROWorldPhys;
}




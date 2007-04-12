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
// Code developed by:
// S.Guatelli (ge.infn.it)
//
//    ************************************
//    *                                  *
//    *   Tst52PhantomROGeometry.cc    *
//    *                                  *
//    ************************************
//
// $Id: Tst52PhantomROGeometry.cc,v 1.1 2007-04-12 12:00:17 guatelli Exp $

// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "Tst52PhantomROGeometry.hh"
#include "Tst52DummySD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"

#include "G4ThreeVector.hh"
#include "G4Material.hh"

Tst52PhantomROGeometry::Tst52PhantomROGeometry(G4String aString):
  G4VReadOutGeometry(aString)
 
{
}

Tst52PhantomROGeometry::~Tst52PhantomROGeometry()
{
}

G4VPhysicalVolume* Tst52PhantomROGeometry::Build()

{
  G4cout << "Number of voxels in the ROGeometry"<< numberOfVoxelsAlongZ << G4endl;
  // A dummy material is used to fill the volumes of the readout geometry.
  // (It will be allowed to set a NULL pointer in volumes of such virtual
  // division in future, since this material is irrelevant for tracking.)


  G4Material* dummyMat = new G4Material(name="dummyMat", 1., 1.*g/mole, 
					1.*g/cm3);

  G4double worldHalfDimensionX = phantomDimensionX/2.;
  G4double worldHalfDimensionY = phantomDimensionX/2.;
  G4double worldHalfDimensionZ = phantomDimensionX/2.;
 
  G4double halfPhantomDimensionX = phantomDimensionX/2.;
  G4double halfPhantomDimensionY = phantomDimensionY/2.;
  G4double halfPhantomDimensionZ = phantomDimensionZ/2.;

 G4cout <<  " ---- > RO Geometry  "<< G4endl;
 G4cout <<"z: "<<  halfPhantomDimensionZ * 2.<< G4endl;
 G4cout <<"y: "<<  halfPhantomDimensionY * 2. << G4endl;
 G4cout <<"x: "<<  halfPhantomDimensionX * 2.<< G4endl;
  // world volume of ROGeometry ...
  G4Box *ROWorld = new G4Box("ROWorld",
			     worldHalfDimensionX,
			     worldHalfDimensionY,
			     worldHalfDimensionZ);

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
						       G4ThreeVector(),
						       "PhantomPhys",
                                                       ROPhantomLog,
                                                       ROWorldPhys,
                                                       false,0);
 
  // ROGeometry: Voxel division
 
  // Z division first... 
 
 G4double voxelSizeZ = phantomDimensionZ/numberOfVoxelsAlongZ;

 G4Box *ROPhantomZDivision = new G4Box("ROPhantomZDivision",
					halfPhantomDimensionX,
					halfPhantomDimensionY,
					voxelSizeZ/2.);

  G4LogicalVolume *ROPhantomZDivisionLog = new G4LogicalVolume(ROPhantomZDivision,
							       dummyMat,
							       "ROPhantomZDivisionLog",
							       0,0,0);

  ROPhantomZDivisionPhys = new G4PVReplica("ROPhantomXDivisionPhys",
                                            ROPhantomZDivisionLog,
                                            ROPhantomPhys,
                                            kZAxis,
                                            numberOfVoxelsAlongZ,
                                            voxelSizeZ);

  Tst52DummySD *dummySD = new Tst52DummySD;
  ROPhantomZDivisionLog -> SetSensitiveDetector(dummySD);

  return ROWorldPhys;
}

void Tst52PhantomROGeometry::SetROParameter(G4double phantomDimX,
					    G4double phantomDimY,
					    G4double phantomDimZ,
					    G4int numberOfVoxelsZ)
{
  phantomDimensionX = phantomDimX;
  phantomDimensionY = phantomDimY;
  phantomDimensionZ = phantomDimZ;
  numberOfVoxelsAlongZ = numberOfVoxelsZ;
  G4cout << "RO set parameters " << phantomDimensionZ << G4endl;

}


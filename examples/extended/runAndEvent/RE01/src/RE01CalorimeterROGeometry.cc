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
// $Id: RE01CalorimeterROGeometry.cc,v 1.1 2004-11-26 07:37:41 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "RE01CalorimeterROGeometry.hh"
#include "RE01DummySD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
 

RE01CalorimeterROGeometry::RE01CalorimeterROGeometry()
  : G4VReadOutGeometry()
{
#include "RE01DetectorParameterDef.icc"
}


RE01CalorimeterROGeometry::RE01CalorimeterROGeometry(G4String aString)
  : G4VReadOutGeometry(aString)
{
#include "RE01DetectorParameterDef.icc"
}

RE01CalorimeterROGeometry::~RE01CalorimeterROGeometry()
{
}

G4VPhysicalVolume* RE01CalorimeterROGeometry::Build()
{
  // A dummy material is used to fill the volumes of the readout geometry.
  // ( It will be allowed to set a NULL pointer in volumes of such virtual
  // division in future, since this material is irrelevant for tracking.)
  G4Material* dummyMat  = new G4Material(name="dummyMat", 1., 1.*g/mole, 1.*g/cm3);

  //Builds the ReadOut World:
  G4Box *ROWorldBox = new G4Box("ROWorldBox", expHall_x, expHall_y, expHall_z);
  G4LogicalVolume *ROWorldLog = new G4LogicalVolume(ROWorldBox, dummyMat,
						    "ROWorldLogical", 0, 0, 0);
  G4PVPlacement *ROWorldPhys = new G4PVPlacement(0,G4ThreeVector(),
						 "ROWorldPhysical",
						 ROWorldLog,
						 0,false,0);
  // Calorimeter volume:
  G4VSolid * caloROtub
    = new G4Tubs("caloROtub",caloTubs_rmin,caloTubs_rmax,
		 caloTubs_dz,caloTubs_sphi,caloTubs_dphi);
  G4LogicalVolume * caloROlog
    = new G4LogicalVolume(caloROtub,dummyMat,"caloROlogical",0,0,0);
  G4VPhysicalVolume * caloROphys
    = new G4PVPlacement(0,G4ThreeVector(),"calROphysical",caloROlog,
			ROWorldPhys,false,0);

  // -------------------------------
  // Calorimeter readout division:
  // -------------------------------
  // Phi division first: 48 sectors
  G4VSolid * caloROphiDivisionTub
    = new G4Tubs("caloROphiDivision", caloCell_rmin, caloCell_rmax,
		 caloCell_dz, caloCell_sphi, caloCell_dphi);
  G4LogicalVolume * caloROphiDivisionLog
    = new G4LogicalVolume(caloROphiDivisionTub, dummyMat, "caloROphiDivisionLogical",0,0,0);
  G4VPhysicalVolume * caloROphiDivisionPhys
    = new G4PVReplica("caloROphiDivisionPhysical", caloROphiDivisionLog, caloROphys,
		      kPhi, segmentsinPhi, caloCell_dphi);
  // then z division: 20 slices:
  G4VSolid * caloROcellTub
    = new G4Tubs("caloROcellTub", caloRing_rmin, caloRing_rmax,
		 caloRing_dz, caloRing_sphi, caloRing_dphi);
  G4LogicalVolume * caloROcellLog
    = new G4LogicalVolume(caloROcellTub, dummyMat, "caloROcellLogical",0,0,0);
//  G4VPhysicalVolume * caloROcellPhys =
      new G4PVReplica("caloROcellPhysical", caloROcellLog, caloROphiDivisionPhys,
		      kZAxis, segmentsinZ,2.*caloRing_dz);
  

  //Flags the cells as sensitive .The pointer here serves
  // as a flag only to check for sensitivity.
  // (Could we make it by a simple cast of a non-NULL value ?)
  RE01DummySD * dummySensi = new RE01DummySD;
  caloROcellLog->SetSensitiveDetector(dummySensi);

  return ROWorldPhys;
}

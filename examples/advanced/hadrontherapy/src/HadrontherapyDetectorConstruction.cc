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
// $Id: HadrontherapyDetectorConstruction.cc;
//
// See more at: http://workgroup.lngs.infn.it/geant4lns/
//
// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the INFN, Catania, Italy
// (b) INFN Section of Genova, Genova, Italy
// 
// * cirrone@lns.infn.it
// ----------------------------------------------------------------------------

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "HadrontherapyDetectorROGeometry.hh"
#include "HadrontherapyDetectorMessenger.hh"
#include "HadrontherapyDetectorSD.hh"
#include "HadrontherapyDetectorConstruction.hh"
#include "HadrontherapyMaterial.hh"
#include "PassiveProtonBeamLine.hh"
#include "HadrontherapyModulator.hh"

/////////////////////////////////////////////////////////////////////////////
HadrontherapyDetectorConstruction::HadrontherapyDetectorConstruction()
  : detectorSD(0), detectorROGeometry(0), 
    passiveProtonBeamLine(0), modulator(0),
    physicalTreatmentRoom(0),
    patientPhysicalVolume(0), 
    detectorLogicalVolume(0), 
    detectorPhysicalVolume(0)
{
  // Messenger to change parameters of the geometry
  detectorMessenger = new HadrontherapyDetectorMessenger(this);

  material = new HadrontherapyMaterial();

  // Detector sizes
  detectorSizeX = 20.*mm;
  detectorSizeY = 20.*mm;
  detectorSizeZ = 20.*mm;

  // Number of the detector voxels  
  numberOfVoxelsAlongX = 400;
  numberOfVoxelsAlongY = 1;
  numberOfVoxelsAlongZ = 1;
}

/////////////////////////////////////////////////////////////////////////////
HadrontherapyDetectorConstruction::~HadrontherapyDetectorConstruction()
{ 
  delete material;
  if (detectorROGeometry) delete detectorROGeometry;  
  delete detectorMessenger;
}

G4VPhysicalVolume* HadrontherapyDetectorConstruction::Construct()
{  
  // Define the materials of the experimental set-up
  material -> DefineMaterials();
  
  // Define the geometry components
  ConstructBeamLine();
  ConstructDetector();
  
  // Set the sensitive detector where the energy deposit is collected
  ConstructSensitiveDetector();
 
  return physicalTreatmentRoom;
}

/////////////////////////////////////////////////////////////////////////////
void HadrontherapyDetectorConstruction::ConstructBeamLine()
{ 
  G4Material* air = material -> GetMat("Air") ;
  G4Material* water = material -> GetMat("G4_WATER");

  // -----------------------------
  // Treatment room - World volume
  //------------------------------

  // Treatment room sizes
  const G4double worldX = 400.0 *cm;
  const G4double worldY = 400.0 *cm;
  const G4double worldZ = 400.0 *cm;

  G4Box* treatmentRoom = new G4Box("TreatmentRoom",worldX,worldY,worldZ);

  G4LogicalVolume* logicTreatmentRoom = new G4LogicalVolume(treatmentRoom, 
                                                            air, 
                                                            "logicTreatmentRoom", 
							    0,0,0);
  physicalTreatmentRoom = new G4PVPlacement(0,
					    G4ThreeVector(),
					    "physicalTreatmentRoom", 
					    logicTreatmentRoom, 
					    0,false,0);

  G4double maxStepTreatmentRoom = 0.1 *mm;
  logicTreatmentRoom -> SetUserLimits(new G4UserLimits(maxStepTreatmentRoom));

  // The treatment room is invisible in the Visualisation
  logicTreatmentRoom -> SetVisAttributes (G4VisAttributes::Invisible);
 
  // Components of the Passive Proton Beam Line
  passiveProtonBeamLine = new PassiveProtonBeamLine(physicalTreatmentRoom);
  passiveProtonBeamLine -> HadrontherapyBeamLineSupport();
  passiveProtonBeamLine -> HadrontherapyBeamScatteringFoils();
  passiveProtonBeamLine -> HadrontherapyRangeShifter();
  passiveProtonBeamLine -> HadrontherapyBeamCollimators();
  passiveProtonBeamLine -> HadrontherapyBeamMonitoring();
  passiveProtonBeamLine -> HadrontherapyMOPIDetector();
  passiveProtonBeamLine -> HadrontherapyBeamNozzle();
  passiveProtonBeamLine -> HadrontherapyBeamFinalCollimator();

  modulator = new HadrontherapyModulator();
  modulator -> BuildModulator(physicalTreatmentRoom);

  //----------------------------------------
  // Patient: 
  // a water box used to approximate tissues
  //----------------------------------------
 
  G4Box* patient = new G4Box("patient",20 *cm, 20 *cm, 20 *cm);
  
  G4LogicalVolume* patientLogicalVolume = new G4LogicalVolume(patient,
							      water, 
							      "patientLog", 0, 0, 0);
  
  patientPhysicalVolume = new G4PVPlacement(0,G4ThreeVector(200.*mm, 0.*mm, 0.*mm),
					    "patientPhys",
					    patientLogicalVolume,
					    physicalTreatmentRoom,
					    false,0);
 
  // Visualisation attributes of the patient 
  G4VisAttributes * redWire = new G4VisAttributes(G4Colour(1. ,0. ,0.));
  redWire -> SetVisibility(true);
  redWire -> SetForceWireframe(true);
  patientLogicalVolume -> SetVisAttributes(redWire); 
}

/////////////////////////////////////////////////////////////////////////////
void HadrontherapyDetectorConstruction::ConstructDetector()
{
  G4Colour  lightBlue   (0.0, 0.0, .75);
  G4Material* water = material -> GetMat("G4_WATER");
  
  //-----------
  // Detector   
  //-----------
  
  G4Box* detector = new G4Box("Detector",detectorSizeX,detectorSizeY,detectorSizeZ);
  
  detectorLogicalVolume = new G4LogicalVolume(detector,
					      water,
					      "DetectorLog",
					      0,0,0);
  
  // Fixing the max step allowed in the detector
  G4double maxStep = 0.01 *mm;
  detectorLogicalVolume -> SetUserLimits(new G4UserLimits(maxStep));
  
  G4double detectorXtranslation = -180.*mm;
  detectorPhysicalVolume = new G4PVPlacement(0,
					     G4ThreeVector(detectorXtranslation, 0.0 *mm, 0.0 *mm),
					     "DetectorPhys",
					     detectorLogicalVolume,
					     patientPhysicalVolume,
					     false,0);
  
  // Visualisation attributes of the phantom
  G4VisAttributes* simpleBoxVisAttributes = new G4VisAttributes(lightBlue);
  simpleBoxVisAttributes -> SetVisibility(true);
  simpleBoxVisAttributes -> SetForceSolid(true);
  detectorLogicalVolume -> SetVisAttributes(simpleBoxVisAttributes);
  
  // **************
  // Cut per Region
  // **************
  
  // A smaller cut is fixed in the phantom to calculate the energy deposit with the
  // required accuracy 
  G4Region* aRegion = new G4Region("DetectorLog");
  detectorLogicalVolume -> SetRegion(aRegion);
  aRegion -> AddRootLogicalVolume(detectorLogicalVolume);
}

void  HadrontherapyDetectorConstruction::ConstructSensitiveDetector()
{  
  // Sensitive Detector and ReadOut geometry definition
  G4SDManager* sensitiveDetectorManager = G4SDManager::GetSDMpointer();

  G4String sensitiveDetectorName = "Detector"; 

  if(!detectorSD)
    {
      // The sensitive detector is instantiated
      detectorSD = new HadrontherapyDetectorSD(sensitiveDetectorName);
      
      // The Read Out Geometry is instantiated
      G4String ROGeometryName = "DetectorROGeometry";
      detectorROGeometry = new HadrontherapyDetectorROGeometry(ROGeometryName,
							     detectorSizeX,
							     detectorSizeY,
							     detectorSizeZ,
							     numberOfVoxelsAlongX,
							     numberOfVoxelsAlongY,
							     numberOfVoxelsAlongZ);
      detectorROGeometry -> BuildROGeometry();
      detectorSD -> SetROgeometry(detectorROGeometry);
      sensitiveDetectorManager -> AddNewDetector(detectorSD);
      detectorLogicalVolume -> SetSensitiveDetector(detectorSD);
    }
}

void HadrontherapyDetectorConstruction::SetModulatorAngle(G4double value)
{  
  modulator -> SetModulatorAngle(value);
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}

void HadrontherapyDetectorConstruction::SetRangeShifterXPosition(G4double value)
{
  passiveProtonBeamLine -> SetRangeShifterXPosition(value);
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}

void HadrontherapyDetectorConstruction::SetRSMaterial(G4String materialChoice)
{
  passiveProtonBeamLine -> SetRSMaterial(materialChoice);
}

void HadrontherapyDetectorConstruction::SetRangeShifterXSize(G4double value)
{
  passiveProtonBeamLine -> SetRangeShifterXSize(value);
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}

void HadrontherapyDetectorConstruction::SetFirstScatteringFoilSize(G4double value)
{
  passiveProtonBeamLine -> SetFirstScatteringFoilXSize(value);  
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}

void HadrontherapyDetectorConstruction::SetSecondScatteringFoilSize(G4double value)
{
  passiveProtonBeamLine -> SetSecondScatteringFoilXSize(value);
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}

void HadrontherapyDetectorConstruction::SetOuterRadiusStopper(G4double value)
{
  passiveProtonBeamLine -> SetOuterRadiusStopper(value); 
  G4RunManager::GetRunManager() -> GeometryHasBeenModified(); 
}

void HadrontherapyDetectorConstruction::SetInnerRadiusFinalCollimator(G4double value)
{
 passiveProtonBeamLine -> SetInnerRadiusFinalCollimator(value);
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}






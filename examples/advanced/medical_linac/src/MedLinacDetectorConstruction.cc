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
// $Id: MedLinacDetectorConstruction.cc,v 1.4 2004-05-14 18:25:40 mpiergen Exp $
//
// Code developed by: M. Piergentili
//
//
//------------------------------ beam line along z axis---------------------

#include "MedLinacDetectorMessenger.hh"
#include "MedLinacDetectorConstruction.hh"
#include "MedLinacPhantomROGeometry.hh"
#include "MedLinacPhantomSD.hh"
#include "MedLinacVGeometryComponent.hh"
#include "MedLinacHead.hh"
#include "MedLinacDecorator.hh"
#include "MedLinacTargetAndFilterDecorator.hh"
#include "G4CSGSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"
#include "G4GeometryManager.hh"
#include "G4BooleanSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSolid.hh"
#include "G4Region.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RegionStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"

MedLinacDetectorConstruction::MedLinacDetectorConstruction(G4String SDName)
  : phantomSD(0),phantomROGeometry(0),
    experimentalHall_log(0),vacuumBlock_log(0),
    JawY1_log(0),JawY2_log(0),
    JawX1_log(0),JawX2_log(0),
    Phantom_log(0),
    experimentalHall_phys(0),vacuumBlock_phys(0),
    JawY1_phys(0), JawY2_phys(0),
    JawX1_phys(0), JawX2_phys(0),
    Phantom_phys(0)
{
  numberOfVoxelsAlongX=300;
  numberOfVoxelsAlongY=300;
  numberOfVoxelsAlongZ=300;

  ComputeDimVoxel();

  sensitiveDetectorName = SDName;

 // default parameter values

  fieldX1 = -10.*cm;
  fieldX2 =  10.*cm;
  fieldY1 = -10.*cm;
  fieldY2 =  10.*cm;


  detectorMessenger = new MedLinacDetectorMessenger(this);
  pHead = new MedLinacHead();
  decorator = new MedLinacTargetAndFilterDecorator(pHead);
}

MedLinacDetectorConstruction* MedLinacDetectorConstruction::instance = 0;

MedLinacDetectorConstruction* MedLinacDetectorConstruction::GetInstance(G4String SDName)
{
  if (instance == 0)
    {
      instance = new MedLinacDetectorConstruction(SDName);
     
    }
  return instance;
}

MedLinacDetectorConstruction::~MedLinacDetectorConstruction()
{
  delete detectorMessenger; 
  delete pHead;
  delete decorator;
  //delete decorator1;
  //delete decorator2;
}

G4VPhysicalVolume* MedLinacDetectorConstruction::Construct()
{
  return ConstructGeom();
}

G4VPhysicalVolume* MedLinacDetectorConstruction::ConstructGeom ()
{

  // Cleanup old geometry

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //    materials

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4int ncomponents;
  G4int natoms;
  G4String name;
  G4String symbol;
  G4double fractionmass;                                           
  G4double massOfMole;   
  G4double pressure; 
  G4double temperature; 

  a = 14.00674*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen" ,symbol="N", z=7., a);

  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxygen",symbol="O", z=8., a);

  a = 1.00794*g/mole;
  G4Element* elH = new G4Element(name="Hydrogen",symbol="H", z=1., a);
 
  density = 1.290*mg/cm3;
  G4Material* Air = new G4Material(name="Air",density, ncomponents=2);
  Air->AddElement(elN, fractionmass=70*perCent);
  Air->AddElement(elO, fractionmass=30*perCent);

  density = 1.000*g/cm3;
  G4Material* H2O = new G4Material(name="Water",density, ncomponents=2);
  H2O->AddElement(elH, natoms=2);
  H2O->AddElement(elO, natoms=1);

  a = 207.19*g/mole;
  density = 11.35*g/cm3;
  G4Material* Pb = new G4Material(name="Lead", z=82., a, density);

  massOfMole = 1.008*g/mole;
  density = 1.e-25*g/cm3;
  temperature = 2.73*kelvin;
  pressure = 3.e-18*pascal; 
  G4Material* Vacuum = new G4Material("interGalactic", z=1.,massOfMole, 
				      density, kStateGas,temperature, pressure);  

  //------------------------------ beam line along z axis------------------

  //---------jaws position--------

  //----1Y------------------------

  G4double thetaY1 = atan(-fieldY1/(100.*cm));// in rad
  G4double JawY1Pos_y = -((3.9*cm+28.*cm)*sin(thetaY1)+5.*cm*cos(thetaY1));
  G4double JawY1Pos_z =115.*cm-((3.9*cm+28.*cm)*cos(thetaY1))+5.*cm*sin(thetaY1);
  //----2Y-------------------------
  G4double thetaY2 = atan(fieldY2/(100.*cm)); // in rad
  G4double JawY2Pos_y = (3.9*cm+28.*cm)*sin(thetaY2)+5.*cm*cos(thetaY2);
  G4double JawY2Pos_z = 115.*cm-(3.9*cm+28.*cm)*cos(thetaY2)+5.*cm*sin(thetaY2);
  //----1X-------------------------
  G4double thetaX1 = atan(-fieldX1/(100.*cm));
  G4double JawX1Pos_x = -((36.7*cm+3.9*cm)*sin(thetaX1)+5.*cm*cos(thetaX1));
  G4double JawX1Pos_z =115.*cm-(36.7*cm+3.9*cm)*cos(thetaX1)+5.*cm*sin(thetaX1);
   //----2X-------------------------
  G4double thetaX2 = atan(fieldX2/(100.*cm));
  G4double JawX2Pos_x = (36.7*cm+3.9*cm)*sin(thetaX2)+5.*cm*cos(thetaX2);
  G4double JawX2Pos_z = 115.*cm-(36.7*cm+3.9*cm)*cos(thetaX2)+5.*cm*sin(thetaX2);
  //---------rotation jaw1Y--------

  G4RotationMatrix* rotatejaw1Y=new G4RotationMatrix();
  rotatejaw1Y->rotateX(thetaY1);

//---------rotation jaw2Y--------

  G4RotationMatrix* rotatejaw2Y=new G4RotationMatrix();
  rotatejaw2Y->rotateX(-thetaY2);

  //---------rotation jaw1X--------

  G4RotationMatrix*  rotatejaw1X=new G4RotationMatrix();
  rotatejaw1X->rotateY(-thetaX1);

  //---------rotation jaw2X--------

  G4RotationMatrix*  rotatejaw2X=new G4RotationMatrix();
  rotatejaw2X->rotateY(thetaX2);

  //---------------colors----------

   G4Colour  white   (1.0, 1.0, 1.0);
   G4Colour  magenta (1.0, 0.0, 1.0); 
   G4Colour  lblue   (0.20, .50, .85);

  //------------------------------------------------------ volumes

  //------------------------------ beam line along z axis-------SSD=100cm--------------  

   //------------------------------ experimental hall (world volume)
  G4double expHall_x = 3.0*m;
  G4double expHall_y = 3.0*m;
  G4double expHall_z = 3.0*m;
  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                             Air,"expHall_log",0,0,0);
  experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                      "expHall",experimentalHall_log,0,false,0);


 
  //------------------------vacuum box------------------------

  G4double vacudim_x = 9.*cm;
  G4double vacudim_y = 9.*cm;
  G4double vacudim_z = 8.755*cm;
  G4Box* vacuumBlock_box = new G4Box("vacuumBlock_box",vacudim_x,
				     vacudim_y,vacudim_z);
  vacuumBlock_log = new G4LogicalVolume(vacuumBlock_box,
					Vacuum,"vacuumBlock_log",0,0,0);
  G4double vacublockPos_x = 0.0*m;
  G4double vacublockPos_y = 0.0*m;
  G4double vacublockPos_z = 114.755*cm;
  vacuumBlock_phys = new G4PVPlacement(0,
             G4ThreeVector(vacublockPos_x,vacublockPos_y,vacublockPos_z),
             "vacuBlock",vacuumBlock_log,experimentalHall_phys,false,0);


  //----------------Jaw Y1 collimator---------------

  G4double JawY1Dim_x = 10.0*cm;
  G4double JawY1Dim_y = 5.0*cm;
  G4double JawY1Dim_z = 2.5*cm;
  G4Box* JawY1 = new G4Box("JawY1",JawY1Dim_x,
                                  JawY1Dim_y,JawY1Dim_z);
  JawY1_log = new G4LogicalVolume(JawY1,
                                             Pb,"JawY1_log",0,0,0);



  G4double JawY1Pos_x = 0.0*cm;
  JawY1_phys = new G4PVPlacement(rotatejaw1Y,
             G4ThreeVector(JawY1Pos_x,JawY1Pos_y,JawY1Pos_z),
             "JawY1",JawY1_log,experimentalHall_phys,false,0);

  //----------------Jaw Y2 collimator---------------

  G4double JawY2Dim_x = 10.0*cm;
  G4double JawY2Dim_y = 5.0*cm;
  G4double JawY2Dim_z = 2.5*cm;
  G4Box* JawY2 = new G4Box("JawY2",JawY2Dim_x,
                                  JawY2Dim_y,JawY2Dim_z);
  JawY2_log = new G4LogicalVolume(JawY2,
                                             Pb,"JawY1_log",0,0,0);
  G4double JawY2Pos_x = 0.0*cm;
  JawY2_phys = new G4PVPlacement(rotatejaw2Y,
             G4ThreeVector(JawY2Pos_x,JawY2Pos_y,JawY2Pos_z),
             "JawY2",JawY2_log,experimentalHall_phys,false,0);


 //----------------Jaw X1 collimator---------------

  G4double JawX1Dim_x = 5.0*cm;
  G4double JawX1Dim_y = 10.0*cm;
  G4double JawX1Dim_z = 2.5*cm;
  G4Box* JawX1 = new G4Box("JawX1",JawX1Dim_x,
                                 JawX1Dim_y,JawX1Dim_z);
  JawX1_log = new G4LogicalVolume(JawX1,
                                             Pb,"JawX1_log",0,0,0);
  
    G4double JawX1Pos_y = 0.0*cm;
    JawX1_phys = new G4PVPlacement(rotatejaw1X,
             G4ThreeVector(JawX1Pos_x,JawX1Pos_y,JawX1Pos_z),
             "JawX1",JawX1_log,experimentalHall_phys,false,0);

  //----------------Jaw X2 collimator---------------

  G4double JawX2Dim_x = 5.0*cm;
  G4double JawX2Dim_y = 10.0*cm;
  G4double JawX2Dim_z = 2.5*cm;
  G4Box* JawX2 = new G4Box("JawX2",JawX2Dim_x,
                                  JawX2Dim_y,JawX2Dim_z);
  JawX2_log = new G4LogicalVolume(JawX2,
                                             Pb,"JawX1_log",0,0,0);
  G4double JawX2Pos_y = 0.0*cm;
  JawX2_phys = new G4PVPlacement(rotatejaw2X,
             G4ThreeVector(JawX2Pos_x,JawX2Pos_y,JawX2Pos_z),
             "JawX2",JawX2_log,experimentalHall_phys,false,0);

  //----------------Phantom---------
  phantomDim_x = 15.*cm;
  phantomDim_y = 15.*cm;
  phantomDim_z = 15.*cm;
  G4Box* Phantom = new G4Box("Phantom",phantomDim_x,
                                  phantomDim_y,phantomDim_z);
  Phantom_log = new G4LogicalVolume(Phantom,
                                             H2O,"Phantom_log",0,0,0);
  G4double PhantomPos_x = 0.0*m;
  G4double PhantomPos_y = 0.0*m;
  G4double PhantomPos_z = 0.0*cm;
  Phantom_phys = new G4PVPlacement(0,
             G4ThreeVector(PhantomPos_x,PhantomPos_y,PhantomPos_z),
             "Phantom_phys",Phantom_log,experimentalHall_phys,false,0);

  PrintParameters();   

//--------- Visualization attributes -------------------------------


 
   G4VisAttributes* simpleH2OVisAtt= new G4VisAttributes(lblue);
   simpleH2OVisAtt->SetVisibility(true);
   simpleH2OVisAtt->SetForceSolid(true);
   Phantom_log->SetVisAttributes(simpleH2OVisAtt);

   G4VisAttributes* simpleVacuumWVisAtt= new G4VisAttributes(white);
   simpleVacuumWVisAtt->SetVisibility(true);
   simpleVacuumWVisAtt->SetForceWireframe(true);
   vacuumBlock_log->SetVisAttributes(simpleVacuumWVisAtt);

   G4VisAttributes* simpleLeadSVisAtt= new G4VisAttributes(magenta);
   simpleLeadSVisAtt->SetVisibility(true);
   simpleLeadSVisAtt->SetForceSolid(true);
   JawY1_log->SetVisAttributes(simpleLeadSVisAtt);
   JawY2_log->SetVisAttributes(simpleLeadSVisAtt);
   JawX1_log->SetVisAttributes(simpleLeadSVisAtt);
   JawX2_log->SetVisAttributes(simpleLeadSVisAtt);

 
   G4VisAttributes* simpleWorldVisAtt= new G4VisAttributes(white);
   simpleWorldVisAtt->SetVisibility(false);
   experimentalHall_log ->SetVisAttributes(simpleWorldVisAtt);

   //if you want not to see phantom, jaws e vacuum ...:

   //Phantom_log->SetVisAttributes(simpleWorldVisAtt);
   //JawY1_log->SetVisAttributes(simpleWorldVisAtt);
   //JawY2_log->SetVisAttributes(simpleWorldVisAtt);
   //JawX1_log->SetVisAttributes(simpleWorldVisAtt);
   //JawX2_log->SetVisAttributes(simpleWorldVisAtt);
   vacuumBlock_log->SetVisAttributes(simpleWorldVisAtt);

  //   Sets a max Step length in the detector
    G4double maxStep = 0.1*mm;
   Phantom_log->SetUserLimits(new G4UserLimits(maxStep));

   ConstructVolume();

   ConstructSensitiveDetector();
   return experimentalHall_phys;
}

void MedLinacDetectorConstruction::PrintParameters()
{
  G4cout <<"jaws1 x position "<< fieldX1/cm << " cm "<<G4endl ;
  G4cout <<"jaws2 x position "<< fieldX2/cm << " cm "<<G4endl ; 
  G4cout <<"jaws1 y position "<< fieldY1/cm << " cm "<<G4endl ;
  G4cout <<"jaws2 y position "<< fieldY2/cm << " cm "<<G4endl ; 
    
}



void MedLinacDetectorConstruction::SetJawX1Pos_x (G4double val)
{ 
  fieldX1 = val;
}


void MedLinacDetectorConstruction::SetJawX2Pos_x (G4double val)
{
  fieldX2 = val;
}

void MedLinacDetectorConstruction::SetJawY1Pos_y (G4double val)
{
  fieldY1 = val;
}

void MedLinacDetectorConstruction::SetJawY2Pos_y (G4double val)
{
  fieldY2 = val;
}


void MedLinacDetectorConstruction::UpdateGeometry()
{ 
  G4cout <<"UpdateGeometry "<< G4endl;

  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructGeom());
}

void MedLinacDetectorConstruction::ConstructVolume()
{
  pHead ->ConstructComponent(experimentalHall_phys,vacuumBlock_phys);
  decorator ->ConstructComponent( experimentalHall_phys,vacuumBlock_phys);
}

void  MedLinacDetectorConstruction::ConstructSensitiveDetector()
// Sensitive Detector and ReadOut geometry definition
{ 
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer();

  if(!phantomSD)
  {
    phantomSD = new MedLinacPhantomSD (sensitiveDetectorName);

    pSDManager->AddNewDetector(phantomSD);
  }

 G4String ROGeometryName = "PhantomROGeometry";
    phantomROGeometry = new MedLinacPhantomROGeometry (ROGeometryName, phantomDim_x,phantomDim_y,phantomDim_z,
						  numberOfVoxelsAlongX,numberOfVoxelsAlongY,numberOfVoxelsAlongZ);

    phantomROGeometry->BuildROGeometry();
    phantomSD->SetROgeometry(phantomROGeometry);

    Phantom_log->SetSensitiveDetector(phantomSD);
}

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
// $Id: MedLinacTargetAndFilterDecorator.cc,v 1.1 2004-05-14 18:25:40 mpiergen Exp $
//
// Code developed by: M. Piergentili
//
//
#include "MedLinacVGeometryComponent.hh"
#include "G4Material.hh"
#include "MedLinacTargetAndFilterDecorator.hh"
#include "MedLinacDecorator.hh"

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4GeometryManager.hh"
#include "G4BooleanSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSolid.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
MedLinacTargetAndFilterDecorator::MedLinacTargetAndFilterDecorator(MedLinacVGeometryComponent* comp)
  : MedLinacDecorator(comp)
{
  
}
MedLinacTargetAndFilterDecorator::~MedLinacTargetAndFilterDecorator()
{
  ;
}
void MedLinacTargetAndFilterDecorator::ConstructComponent(G4VPhysicalVolume* world, G4VPhysicalVolume* vacuumBlock)
{
   MedLinacDecorator::ConstructComponent(world,vacuumBlock);
   ConstructTargetAndFilter(world,vacuumBlock);
}

void MedLinacTargetAndFilterDecorator::DestroyComponent()
{
  ;
}
void MedLinacTargetAndFilterDecorator::ConstructTargetAndFilter(G4VPhysicalVolume* world, G4VPhysicalVolume* vacuumBlock)
{

  //    materials

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4String symbol;
  G4String name;
  G4double fractionmass;
  G4int ncomponents;

  a = 14.00674*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen" ,symbol="N", z=7., a);

  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxygen",symbol="O", z=8., a);
 
  a = 49.29*g/mole;
  density = 2.6989*g/cm3;
  G4Material* Al = new G4Material(name="Aluminium", z=13., a, density);

  a = 207.19*g/mole;
  density = 11.35*g/cm3;
  G4Material* Pb = new G4Material(name="Lead", z=82., a, density);
;
  density = 1.290*mg/cm3;
  G4Material* Air = new G4Material(name="Air",density, ncomponents=2);
  Air->AddElement(elN, fractionmass=70*perCent);
  Air->AddElement(elO, fractionmass=30*perCent);

  //    colors
  G4Colour  cyan    (0.0, 1.0, 1.0);
  G4Colour  magenta (1.0, 0.0, 1.0); 
 
 //---------rotation matrix filter--------

  G4RotationMatrix*  rotateMatrix=new G4RotationMatrix();
  rotateMatrix->rotateX(180.0*deg);


  //    volumes
  //    beam line along z axis
//------------------------target 6MV------------------------

  G4double targetADim_x = 0.6*cm;
  G4double targetADim_y = 0.6*cm;
  G4double targetADim_z = 0.04445*cm;
  G4Box* targetA_box = new G4Box("targetA_box",targetADim_x,targetADim_y,targetADim_z);
  targetA_log = new G4LogicalVolume(targetA_box,Pb,"targetA_log",0,0,0);
  G4double targetAPos_x = 0.0*m;
  G4double targetAPos_y = 0.0*m;
  G4double targetAPos_z = 0.20055*cm;
  targetA_phys = new G4PVPlacement(0,
            G4ThreeVector(targetAPos_x,targetAPos_y,targetAPos_z),
            "targetA",targetA_log,vacuumBlock,false,0);

    G4double targetBDim_x = 0.6*cm;
  G4double targetBDim_y = 0.6*cm;
  G4double targetBDim_z = 0.0787*cm;
  G4Box* targetB_box = new G4Box("targetB_box",targetBDim_x,targetBDim_y,targetBDim_z);
  targetB_log = new G4LogicalVolume(targetB_box,Al,"targetB_log",0,0,0);
  G4double targetBPos_x = 0.0*m;
  G4double targetBPos_y = 0.0*m;
  G4double targetBPos_z = 0.0773*cm;
  targetB_phys = new G4PVPlacement(0,
            G4ThreeVector(targetBPos_x,targetBPos_y,targetBPos_z),
            "targetB",targetB_log,vacuumBlock,false,0);

  //--------------Flattening Filter---------------
  G4double  layerRmin1 = 0.*cm;
  G4double  layerRmin2 = 0.*cm;
  G4double layerPos_x = 0.0*m;
  G4double layerPos_y = 0.0*m;

    //-----------layer1-----------------------------
  G4double  layer1Rmax1 = 0.00000001*cm;
  G4double  layer1Rmax2 = 0.025*cm;
  G4double  layer1HightOfTheCone = 0.006*cm;
  G4double  startAngleOfTheCone = 0.*deg;
  G4double  spanningAngleOfTheCone = 360.*deg;
  G4Cons* layer1 = new G4Cons("layer1",layerRmin1,layer1Rmax1,layerRmin2,
  			   layer1Rmax2,layer1HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer1_log = new G4LogicalVolume(layer1,Al,"layer1_log",0,0,0);

  G4double layer1Pos_z = 103.404*cm;
  layer1_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer1Pos_z),
             "layer1",layer1_log,world,false,0);

   //-----------layer2-----------------------------
  G4double  layer2Rmax1 = 0.025*cm;
  G4double  layer2Rmax2 = 0.051*cm;
  G4double  layer2HightOfTheCone = 0.0065*cm;
  G4Cons* layer2 = new G4Cons("layer2",layerRmin1,layer2Rmax1,layerRmin2,
  			   layer2Rmax2,layer2HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer2_log = new G4LogicalVolume(layer2,Al,"layer2_log",0,0,0);

  G4double layer2Pos_z = 103.3888*cm;
  layer2_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer2Pos_z),
             "layer2",layer2_log,world,false,0);
    //-----------layer3-----------------------------
  G4double  layer3Rmax1 = 0.051*cm;
  G4double  layer3Rmax2 = 0.077*cm;
  G4double  layer3HightOfTheCone = 0.0075*cm;
  G4Cons* layer3 = new G4Cons("layer3",layerRmin1,layer3Rmax1,layerRmin2,
  			   layer3Rmax2,layer3HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer3_log = new G4LogicalVolume(layer3,Al,"layer3_log",0,0,0);

  G4double layer3Pos_z = 103.3748*cm;
  layer3_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer3Pos_z),
             "layer3",layer3_log,world,false,0);

  //-----------layer4-----------------------------
  G4double  layer4Rmax1 = 0.077*cm;
  G4double  layer4Rmax2 = 0.103*cm;
  G4double  layer4HightOfTheCone = 0.009*cm;
  G4Cons* layer4 = new G4Cons("layer4",layerRmin1,layer4Rmax1,layerRmin2,
  			   layer4Rmax2,layer4HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer4_log = new G4LogicalVolume(layer4,Al,"layer4_log",0,0,0);

  G4double layer4Pos_z = 103.3583*cm;
  layer4_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer4Pos_z),
             "layer4",layer4_log,world,false,0);
  //-----------layer5-----------------------------
  G4double  layer5Rmax1 = 0.103*cm;
  G4double  layer5Rmax2 = 0.154*cm;
  G4double  layer5HightOfTheCone = 0.022*cm;
  G4Cons* layer5 = new G4Cons("layer5",layerRmin1,layer5Rmax1,layerRmin2,
  			   layer5Rmax2,layer5HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer5_log = new G4LogicalVolume(layer5,Al,"layer5_log",0,0,0);

  G4double layer5Pos_z = 103.3273*cm;
  layer5_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer5Pos_z),
             "layer5",layer5_log,world,false,0);

   //-----------layer6-----------------------------
  G4double  layer6Rmax1 = 0.154*cm;
  G4double  layer6Rmax2 = 0.205*cm;
  G4double  layer6HightOfTheCone = 0.024*cm;
  G4Cons* layer6 = new G4Cons("layer6",layerRmin1,layer6Rmax1,layerRmin2,
  			   layer6Rmax2,layer6HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer6_log = new G4LogicalVolume(layer6,Al,"layer6_log",0,0,0);

  G4double layer6Pos_z = 103.2813*cm;
  layer6_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer6Pos_z),
             "layer6",layer6_log,world,false,0);

    //-----------layer7-----------------------------
  G4double  layer7Rmax1 = 0.205*cm;
  G4double  layer7Rmax2 = 0.256*cm;
  G4double  layer7HightOfTheCone = 0.023*cm;
  G4Cons* layer7 = new G4Cons("layer7",layerRmin1,layer7Rmax1,layerRmin2,
  			   layer7Rmax2,layer7HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer7_log = new G4LogicalVolume(layer7,Al,"layer7_log",0,0,0);

  G4double layer7Pos_z = 103.2343*cm;
  layer7_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer7Pos_z),
             "layer7",layer7_log,world,false,0);

 //-----------layer8-----------------------------
  G4double  layer8Rmax1 = 0.256*cm;
  G4double  layer8Rmax2 = 0.307*cm;
  G4double  layer8HightOfTheCone = 0.023*cm;
  G4Cons* layer8 = new G4Cons("layer8",layerRmin1,layer8Rmax1,layerRmin2,
  			   layer8Rmax2,layer8HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer8_log = new G4LogicalVolume(layer8,Al,"layer8_log",0,0,0);

  G4double layer8Pos_z = 103.1883*cm;
  layer8_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer8Pos_z),
             "layer8",layer8_log,world,false,0);

   //-----------layer9-----------------------------
  G4double  layer9Rmax1 = 0.307*cm;
  G4double  layer9Rmax2 = 0.358*cm;
  G4double  layer9HightOfTheCone = 0.0225*cm;
  G4Cons* layer9 = new G4Cons("layer9",layerRmin1,layer9Rmax1,layerRmin2,
  			   layer9Rmax2,layer9HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer9_log = new G4LogicalVolume(layer9,Al,"layer9_log",0,0,0);

  G4double layer9Pos_z = 103.1428*cm;
  layer9_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer9Pos_z),
             "layer9",layer9_log,world,false,0);

  //-----------layer10-----------------------------
  G4double  layer10Rmax1 = 0.358*cm;
  G4double  layer10Rmax2 = 0.409*cm;
  G4double  layer10HightOfTheCone = 0.022*cm;
  G4Cons* layer10 = new G4Cons("layer10",layerRmin1,layer10Rmax1,layerRmin2,
  			   layer10Rmax2,layer10HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer10_log = new G4LogicalVolume(layer10,Al,"layer10_log",0,0,0);

  G4double layer10Pos_z = 103.0983*cm;
  layer10_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer10Pos_z),
             "layer10",layer10_log,world,false,0);
   //-----------layer11-----------------------------
  G4double  layer11Rmax1 = 0.409*cm;
  G4double  layer11Rmax2 = 0.510*cm;
  G4double  layer11HightOfTheCone = 0.0425*cm;
  G4Cons* layer11 = new G4Cons("layer11",layerRmin1,layer11Rmax1,layerRmin2,
  			   layer11Rmax2,layer11HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer11_log = new G4LogicalVolume(layer11,Al,"layer11_log",0,0,0);

  G4double layer11Pos_z = 103.0338*cm;
  layer11_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer11Pos_z),
             "layer11",layer11_log,world,false,0);

  //-----------layer12-----------------------------
  G4double  layer12Rmax1 = 0.510*cm;
  G4double  layer12Rmax2 = 0.620*cm;
  G4double  layer12HightOfTheCone = 0.0395*cm;
  G4Cons* layer12 = new G4Cons("layer12",layerRmin1,layer12Rmax1,layerRmin2,
  			   layer12Rmax2,layer12HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer12_log = new G4LogicalVolume(layer12,Al,"layer12_log",0,0,0);

  G4double layer12Pos_z = 102.9518*cm;
  layer12_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer12Pos_z),
             "layer12",layer12_log,world,false,0);

  //-----------layer13-----------------------------
  G4double  layer13Rmax1 = 0.620*cm;
  G4double  layer13Rmax2 = 0.730*cm;
  G4double  layer13HightOfTheCone = 0.038*cm;
  G4Cons* layer13 = new G4Cons("layer13",layerRmin1,layer13Rmax1,layerRmin2,
  			   layer13Rmax2,layer13HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer13_log = new G4LogicalVolume(layer13,Al,"layer13_log",0,0,0);

  G4double layer13Pos_z = 102.8743*cm;
  layer13_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer13Pos_z),
             "layer13",layer13_log,world,false,0);
 //-----------layer14-----------------------------
  G4double  layer14Rmax1 = 0.730*cm;
  G4double  layer14Rmax2 = 0.840*cm;
  G4double  layer14HightOfTheCone = 0.0335*cm;
  G4Cons* layer14 = new G4Cons("layer14",layerRmin1,layer14Rmax1,layerRmin2,
  			   layer14Rmax2,layer14HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer14_log = new G4LogicalVolume(layer14,Al,"layer14_log",0,0,0);

  G4double layer14Pos_z = 102.8028*cm;
  layer14_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer14Pos_z),
             "layer14",layer14_log,world,false,0);

   //-----------layer15-----------------------------
  G4double  layer15Rmax1 = 0.840*cm;
  G4double  layer15Rmax2 = 0.950*cm;
  G4double  layer15HightOfTheCone = 0.0325*cm;
  G4Cons* layer15 = new G4Cons("layer15",layerRmin1,layer15Rmax1,layerRmin2,
  			   layer15Rmax2,layer15HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer15_log = new G4LogicalVolume(layer15,Al,"layer15_log",0,0,0);

  G4double layer15Pos_z = 102.7368*cm;
  layer15_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer15Pos_z),
             "layer15",layer15_log,world,false,0);
  //-----------layer16-----------------------------
  G4double  layer16Rmax1 = 0.950*cm;
  G4double  layer16Rmax2 = 1.050*cm;
  G4double  layer16HightOfTheCone = 0.028*cm;
  G4Cons* layer16 = new G4Cons("layer16",layerRmin1,layer16Rmax1,layerRmin2,
  			   layer16Rmax2,layer16HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer16_log = new G4LogicalVolume(layer16,Al,"layer16_log",0,0,0);

  G4double layer16Pos_z = 102.6763*cm;
  layer16_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer16Pos_z),
             "layer16",layer16_log,world,false,0);

  //-----------layer17-----------------------------
  G4double  layer17Rmax1 = 1.050*cm;
  G4double  layer17Rmax2 = 1.150*cm;
  G4double  layer17HightOfTheCone = 0.0275*cm;
  G4Cons* layer17 = new G4Cons("layer17",layerRmin1,layer17Rmax1,layerRmin2,
  			   layer17Rmax2,layer17HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer17_log = new G4LogicalVolume(layer17,Al,"layer17_log",0,0,0);

  G4double layer17Pos_z = 102.56208*cm;
  layer17_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer17Pos_z),
             "layer17",layer17_log,world,false,0);

    //-----------layer18-----------------------------
  G4double  layer18Rmax1 = 1.150*cm;
  G4double  layer18Rmax2 = 1.215*cm;
  G4double  layer18HightOfTheCone = 0.019*cm;
  G4Cons* layer18 = new G4Cons("layer18",layerRmin1,layer18Rmax1,layerRmin2,
  			   layer18Rmax2,layer18HightOfTheCone,startAngleOfTheCone,
  			   spanningAngleOfTheCone);

  layer18_log = new G4LogicalVolume(layer18,Al,"layer18_log",0,0,0);

  G4double layer18Pos_z = 102.4743*cm;
  layer18_phys = new G4PVPlacement(rotateMatrix,
             G4ThreeVector(layerPos_x,layerPos_y,layer18Pos_z),
             "layer18",layer18_log,world,false,0);
  //-----------layer19-----------------------------
  G4double innerRadiusOfTheLayer19 = 0.0*cm;
  G4double outerRadiusOfTheLayer19 = 1.335*cm;
  G4double hightOfTheLayer19 = 0.040*cm;
  G4double startAngleOfTheLayer19 = 0.*deg;
  G4double spanningAngleOfTheLayer19 = 360.*deg;
  G4Tubs* layer19 = new G4Tubs("layer19",innerRadiusOfTheLayer19,
                                    outerRadiusOfTheLayer19,hightOfTheLayer19,
				    startAngleOfTheLayer19,spanningAngleOfTheLayer19);
  layer19_log = new G4LogicalVolume(layer19,Al,"layer19_log",0,0,0);

  G4double layer19PosX = 0.*cm;
  G4double layer19PosY = 0.*cm;
  G4double layer19PosZ = 102.4153*cm;

  layer19_phys = new G4PVPlacement(0,
					   G4ThreeVector(layer19PosX,layer19PosY,
							 layer19PosZ),"layer19",
					   layer19_log,world,false,0);
  //-----------layer20-----------------------------
  G4double innerRadiusOfTheLayer20A = 1.300*cm;
  G4double outerRadiusOfTheLayer20A = 1.335*cm;
  G4double hightOfTheLayer20A = 0.0225*cm;
  G4double startAngleOfTheLayer20A = 0.*deg;
  G4double spanningAngleOfTheLayer20A = 360.*deg;
  G4Tubs* layer20A = new G4Tubs("layer20A",innerRadiusOfTheLayer20A,
                                    outerRadiusOfTheLayer20A,hightOfTheLayer20A,
				    startAngleOfTheLayer20A,spanningAngleOfTheLayer20A);
  layer20A_log = new G4LogicalVolume(layer20A,Al,"layer20A_log",0,0,0);
  
  G4double  cone20Rmin1 = 0.*cm;
  G4double  cone20Rmax1 = 1.335*cm;
  G4double  cone20Rmin2 = 0.*cm;
  G4double  cone20Rmax2 = 1.301*cm;
  G4double  hightOfThecone20 =0.0225*cm;
  G4double  startAngleOfThecone20 = 0.*deg;
  G4double  spanningAngleOfThecone20 = 360.*deg;

  G4Cons* cone20 = new G4Cons("cone20",cone20Rmin1,cone20Rmax1,cone20Rmin2,
				   cone20Rmax2,hightOfThecone20,startAngleOfThecone20,
				   spanningAngleOfThecone20);
  cone20_log = new G4LogicalVolume(cone20,Air,"cone20_log",0,0,0);

  G4SubtractionSolid* layer20 = new G4SubtractionSolid("layer20",layer20A,cone20);
  layer20_log = new G4LogicalVolume(layer20,Al,"layer20_log",0,0,0);

  G4double layer20PosX = 0.*cm;
  G4double layer20PosY = 0.*cm;
  G4double layer20PosZ = 102.4778*cm;
  layer20_phys = new G4PVPlacement(0,
					   G4ThreeVector(layer20PosX,layer20PosY,
							 layer20PosZ),"layer20",
				   layer20_log,world,false,0);

  //-----------layer21-----------------------------
  G4double innerRadiusOfTheLayer21 = 1.335*cm;
  G4double outerRadiusOfTheLayer21 = 1.600*cm;
  G4double hightOfTheLayer21 = 0.0625*cm;
  G4double startAngleOfTheLayer21 = 0.*deg;
  G4double spanningAngleOfTheLayer21 = 360.*deg;
  G4Tubs* layer21 = new G4Tubs("layer21",innerRadiusOfTheLayer21,
                                    outerRadiusOfTheLayer21,hightOfTheLayer21,
				    startAngleOfTheLayer21,spanningAngleOfTheLayer21);
  layer21_log = new G4LogicalVolume(layer21,Al,"layer21_log",0,0,0);

  G4double layer21PosX = 0.*cm;
  G4double layer21PosY = 0.*cm;
  G4double layer21PosZ = 102.4378*cm;

  layer21_phys = new G4PVPlacement(0,
					   G4ThreeVector(layer21PosX,layer21PosY,
							 layer21PosZ),"layer19",
					   layer21_log,world,false,0);

  //    Visualization attributes 

   G4VisAttributes* simpleAlSVisAtt= new G4VisAttributes(cyan);
   simpleAlSVisAtt->SetVisibility(true);
   simpleAlSVisAtt->SetForceSolid(true);
   targetB_log->SetVisAttributes(simpleAlSVisAtt);
   layer1_log->SetVisAttributes(simpleAlSVisAtt);
   layer2_log->SetVisAttributes(simpleAlSVisAtt);
   layer3_log->SetVisAttributes(simpleAlSVisAtt);
   layer4_log->SetVisAttributes(simpleAlSVisAtt);
   layer5_log->SetVisAttributes(simpleAlSVisAtt);
   layer6_log->SetVisAttributes(simpleAlSVisAtt);
   layer7_log->SetVisAttributes(simpleAlSVisAtt);
   layer8_log->SetVisAttributes(simpleAlSVisAtt);
   layer9_log->SetVisAttributes(simpleAlSVisAtt);
   layer10_log->SetVisAttributes(simpleAlSVisAtt);
   layer11_log->SetVisAttributes(simpleAlSVisAtt);
   layer12_log->SetVisAttributes(simpleAlSVisAtt);
   layer13_log->SetVisAttributes(simpleAlSVisAtt);
   layer14_log->SetVisAttributes(simpleAlSVisAtt);
   layer15_log->SetVisAttributes(simpleAlSVisAtt);
   layer16_log->SetVisAttributes(simpleAlSVisAtt);
   layer17_log->SetVisAttributes(simpleAlSVisAtt);
   layer18_log->SetVisAttributes(simpleAlSVisAtt);
   layer19_log->SetVisAttributes(simpleAlSVisAtt);
   layer20_log->SetVisAttributes(simpleAlSVisAtt);
   layer21_log->SetVisAttributes(simpleAlSVisAtt);

   G4VisAttributes* simpleLeadSVisAtt= new G4VisAttributes(magenta);
   simpleLeadSVisAtt->SetVisibility(true);
   simpleLeadSVisAtt->SetForceSolid(true);
   targetA_log->SetVisAttributes(simpleLeadSVisAtt);







}

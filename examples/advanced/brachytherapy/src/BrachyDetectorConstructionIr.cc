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
// --------------------------------------------------------------
//                 GEANT 4 - Brachytherapy example
// --------------------------------------------------------------
//
// Code developed by:
// S. Agostinelli, F. Foppiano, S. Garelli , M. Tropeano, S.Guatelli
//
//    ****************************************
//    *                                      *
//    *    BrachyDetectorConstructionIr.cc   *
//    *                                      *
//    ****************************************
//
// $Id: BrachyDetectorConstructionIr.cc,v 1.4 2003-05-09 13:21:20 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "globals.hh"
#include "BrachyDetectorConstructionIr.hh"
#include "G4CSGSolid.hh"
#include "G4Sphere.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4RunManager.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4MaterialTable.hh"
#include "Randomize.hh"  
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4PVParameterised.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4UnionSolid.hh"
#include "BrachyMaterial.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

BrachyDetectorConstructionIr::BrachyDetectorConstructionIr()
 : Capsule(0),CapsuleLog(0),CapsulePhys(0),
   CapsuleTip(0),CapsuleTipLog(0),CapsuleTipPhys(0),
   IridiumCore(0),IridiumCoreLog(0),IridiumCorePhys(0)
{
  pMat= new BrachyMaterial();
}

//....
BrachyDetectorConstructionIr::~BrachyDetectorConstructionIr()
{ 
  delete pMat; 
}

//....
void BrachyDetectorConstructionIr::ConstructIridium(G4VPhysicalVolume* mother)
{
  G4Colour  red     (1.0, 0.0, 0.0) ;
  G4Colour  magenta (1.0, 0.0, 1.0) ; 

  G4Material* CapsuleMat=pMat->GetMat("Stainless steel");
  G4Material*IridiumMat=pMat->GetMat("Iridium");

  // Capsule main body

  Capsule =
     new G4Tubs("Capsule",0,0.55*mm,3.725*mm,0.*deg,360.*deg);
  CapsuleLog =
     new G4LogicalVolume(Capsule,CapsuleMat,"CapsuleLog");
  CapsulePhys =
     new G4PVPlacement(0,G4ThreeVector(0,0,-1.975),"CapsulePhys",CapsuleLog,mother,false,0);

  // Capsule tip

  CapsuleTip =
     new G4Sphere("CapsuleTipIridium",0.*mm,0.55*mm,0.*deg,360.*deg,0.*deg,90.*deg); 
  CapsuleTipLog =
     new G4LogicalVolume(CapsuleTip,CapsuleMat,"CapsuleTipIridumLog");
  CapsuleTipPhys =
     new G4PVPlacement(0,G4ThreeVector(0.,0.,1.75*mm),"CapsuleTipIridiumPhys",CapsuleTipLog,mother,false,0);

  // Iridium core

  IridiumCore =
     new G4Tubs("IrCore",0,0.30*mm,1.75*mm,0.*deg,360.*deg);
  IridiumCoreLog =
     new G4LogicalVolume(IridiumCore,IridiumMat,"IridiumCoreLog");
  IridiumCorePhys =
     new G4PVPlacement(0,G4ThreeVector(),"IridiumCorePhys",IridiumCoreLog,CapsulePhys,false,0);

  simpleCapsuleVisAtt= new G4VisAttributes(red);
  simpleCapsuleVisAtt->SetVisibility(true);  
  simpleCapsuleVisAtt->SetForceWireframe(true);
  CapsuleLog->SetVisAttributes(simpleCapsuleVisAtt);

  simpleCapsuleTipVisAtt= new G4VisAttributes(red);
  simpleCapsuleTipVisAtt->SetVisibility(true);  
  simpleCapsuleTipVisAtt->SetForceSolid(true);
  CapsuleTipLog->SetVisAttributes(simpleCapsuleTipVisAtt);

  simpleIridiumVisAtt= new G4VisAttributes(magenta);
  simpleIridiumVisAtt->SetVisibility(true);
  simpleIridiumVisAtt->SetForceWireframe(true);
  IridiumCoreLog->SetVisAttributes(simpleIridiumVisAtt);
}

void BrachyDetectorConstructionIr::CleanIridium()
{ 
  delete simpleIridiumVisAtt; simpleIridiumVisAtt=0;
  delete IridiumCorePhys; IridiumCorePhys=0;
  delete IridiumCore; IridiumCore=0;
  delete IridiumCoreLog; IridiumCoreLog=0 ;

  delete simpleCapsuleTipVisAtt; simpleCapsuleTipVisAtt=0;
  delete CapsuleTipPhys; CapsuleTipPhys=0;
  delete CapsuleTip; CapsuleTip=0;
  delete CapsuleTipLog; CapsuleTipLog=0;

  delete simpleCapsuleVisAtt; simpleCapsuleVisAtt=0;
  delete CapsulePhys; CapsulePhys=0;
  delete Capsule; Capsule=0;
  delete CapsuleLog; CapsuleLog=0;
}

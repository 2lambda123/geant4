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
// $Id$
//
/// \file parameterisations/gflash/src/ExGflashDetectorConstruction.cc
/// \brief Implementation of the ExGflashDetectorConstruction class
//
// Created by Joanna Weng 26.11.2004
#include <iostream>
// G4 Classes
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

// User Classes
#include "ExGflashDetectorConstruction.hh"
#include "ExGflashSensitiveDetector.hh"
//fast simulation
#include "GFlashHomoShowerParameterisation.hh"
#include "G4FastSimulationManager.hh"
#include "GFlashShowerModel.hh"
#include "GFlashHitMaker.hh"
#include "GFlashParticleBounds.hh"

using namespace std;

ExGflashDetectorConstruction::ExGflashDetectorConstruction()
 :fExperimentalHall_log(0), 
  fCalo_log(0), 
  fExperimentalHall_phys(0), 
  fCalo_phys(0)
{
  G4cout<<"ExGflashDetectorConstruction::Detector constructor"<<G4endl;
  
  // Simplified `CMS-like` PbWO4 crystal calorimeter  
  fCalo_xside=31*cm;
  fCalo_yside=31*cm;
  fCalo_zside=24*cm; 
  
  // GlashStuff
  fTheParticleBounds  = new GFlashParticleBounds();   // Energy Cuts to kill particles
  fTheHMaker          = new GFlashHitMaker();         // Makes the EnergieSpots  
}


ExGflashDetectorConstruction::~ExGflashDetectorConstruction()
{ 
  //@@@ ExGflashDetectorConstruction::Soll ich alles dlete
  delete fTheParameterisation;
  delete fTheParticleBounds;
  delete fTheHMaker;
  delete fTheFastShowerModel;
}


G4VPhysicalVolume* ExGflashDetectorConstruction::Construct()
{
  //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  G4cout<<"Defining the materials"<<G4endl;
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();
  // Build materials
  G4Material* air   = nistManager->FindOrBuildMaterial("G4_AIR");
  G4Material* pbWO4 = nistManager->FindOrBuildMaterial("G4_PbWO4");
  
  
  /*******************************
   * The Experimental Hall       *
   *******************************/
  fExperimentalHall_x=1000.*cm;
  fExperimentalHall_y=1000.*cm;
  fExperimentalHall_z=1000.*cm;
  
  fExperimentalHall_box = new G4Box("expHall_box",              // World Volume
                                    fExperimentalHall_x,        // x size
                                    fExperimentalHall_y,        // y size 
                                    fExperimentalHall_z);       // z size
  
  fExperimentalHall_log = new G4LogicalVolume(fExperimentalHall_box, //its solid
                                              air,                   //its material
                                              "expHall_log",         // its name 
                                              0,                     //opt: fieldManager
                                              0,                     //opt: SensitiveDetector
                                              0);                    //opt: UserLimits
  fExperimentalHall_phys = new G4PVPlacement(0,                      //no rotation
                                             G4ThreeVector(),        //at (0,0,0)
                                             "expHall",              //its name 
                                             fExperimentalHall_log,  //its logical volume
                                             0,                      //its mother  volume
                                             false,                  //no boolean operation
                                             0);                     //copy number
  
  
  //------------------------------ 
  // Calorimeter segments
  //------------------------------
  // Simplified `CMS-like` PbWO4 crystal calorimeter  
  
  fNbOfCrystals = 10;  // this are the crystals PER ROW in this example 
                       // cube of 10 x 10 crystals 
                       // don't change it @the moment, since 
                       // the readout in event action assumes this 
                       // dimensions and is not automatically adapted
                       // in this version of the example :-( 
  fCrystalWidht = 3*cm;
  fCrystalLenght= 24*cm;
  fCalo_xside=(fCrystalWidht*fNbOfCrystals)+1*cm;
  fCalo_yside=(fCrystalWidht*fNbOfCrystals)+1*cm;
  fCalo_zside=fCrystalLenght;
  
  G4Box *calo_box= new G4Box("CMS calorimeter",  // its name
                             fCalo_xside/2.,     // size
                             fCalo_yside/2.,
                             fCalo_zside/2.);
  fCalo_log = new G4LogicalVolume(calo_box,      // its solid
                                  air,           // its material
                                  "calo log",    // its name
                                  0,             // opt: fieldManager
                                  0,             // opt: SensitiveDetector 
                                  0);            // opt: UserLimit
  
  G4double Xpos = 0.0;
  G4double Ypos = 0.0;
  G4double Zpos = 100.0*cm;
  
  fCalo_phys = new G4PVPlacement(0,                              // no rotation
                                 G4ThreeVector(Xpos,Ypos,Zpos),  // at (0,0,0)
                                 fCalo_log,                      // its logical volume
                                 "calorimeter",                  // its name
                                 fExperimentalHall_log,          // its mother volume
                                 false,                          // no boolean operation
                                 1);
  //Visibility
  for (int i=0; i<fNbOfCrystals;i++)
    {
      
      for (int j=0; j<fNbOfCrystals;j++)
        {  
          int n =  i*10+j;
          fCrystal[n]= new G4Box("Crystal",                       // its name
                                 fCrystalWidht/2,
                                 fCrystalWidht/2,
                                 fCrystalLenght/2);               // size      
          fCrystal_log[n] = new G4LogicalVolume(fCrystal[n],      // its solid
                                                pbWO4,            // its material
                                                "Crystal_log");   // its name
          G4ThreeVector crystalPos((i*fCrystalWidht)-135,(j*fCrystalWidht)-135,0 );
          fCrystal_phys[n] = new G4PVPlacement(0,                 // no rotation
                                               crystalPos,        // translation
                                               fCrystal_log[n],   // its logical volume
                                               "crystal",         // its name
                                               fCalo_log,         // its mother volume
                                               false,             // no boolean operation
                                               1);                // Visibility
        }
    }  
  G4cout << "There are " << fNbOfCrystals <<
    " crystals per row in the calorimeter, so in total "<<
    fNbOfCrystals*fNbOfCrystals << " crystals" << G4endl;  
  G4cout << "The have widthof  " << fCrystalWidht /cm << 
    "  cm and a lenght of  " <<  fCrystalLenght /cm
         <<" cm. The Material is "<< pbWO4 << G4endl;
  
  // Sensitive Detector part
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  ExGflashSensitiveDetector* CaloSD=
    new ExGflashSensitiveDetector("Calorimeter",this);
  SDman->AddNewDetector(CaloSD);
  
  fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);             
  G4VisAttributes* CaloVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* CrystalVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  fCalo_log->SetVisAttributes(CaloVisAtt);
  for (int i=0; i<100;i++)
    {
      fCrystal_log[i]->SetVisAttributes(CrystalVisAtt);
      fCrystal_log[i]->SetSensitiveDetector(CaloSD);
    }
  // define the parameterisation region
  fRegion = new G4Region("crystals");
  fCalo_log->SetRegion(fRegion);
  fRegion->AddRootLogicalVolume(fCalo_log);
  
  
  /**********************************************
   * Initializing shower modell
   ***********************************************/  
  G4cout<<"Shower parameterization"<<G4endl;
  fTheFastShowerModel =  new GFlashShowerModel("fastShowerModel",fRegion);
  fTheParameterisation = new GFlashHomoShowerParameterisation(pbWO4);
  fTheFastShowerModel->SetParameterisation(*fTheParameterisation);
  fTheFastShowerModel->SetParticleBounds(*fTheParticleBounds) ;
  fTheFastShowerModel->SetHitMaker(*fTheHMaker);   
  G4cout<<"end shower parameterization"<<G4endl;
  /**********************************************/
  
  return fExperimentalHall_phys;
}















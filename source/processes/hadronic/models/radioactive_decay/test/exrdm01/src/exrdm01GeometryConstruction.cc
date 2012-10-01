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
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:		exrdm01GeometryConstruction.cc
//
// Version:		0.b.3
// Date:		30/06/99
// Author:		P R Truscott
// Organisation:	DERA UK
// Customer:		ESA/ESTEC, NOORDWIJK
// Contract:		12115/JG/NL Work Order No. 3
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 29 February 2000, P R Truscott, DERA UK
// 0.b.3 release.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////
//
#include "exrdm01GeometryConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
///////////////////////////////////////////////////////////////////////////////
//
exrdm01GeometryConstruction::exrdm01GeometryConstruction ()
{
  //
  //
  // Define size of experiment hall and volumes which will fill it.
  //
  universe_x = 5000000.0*km;
  universe_y = 5000000.0*km;
  universe_z = 5000000.0*km;

  aSphere_r1 = 2500000.0*km;
  aSphere_r2 = 125000.0*km;
}
///////////////////////////////////////////////////////////////////////////////
//
exrdm01GeometryConstruction::~exrdm01GeometryConstruction ()
{;}
////////////////////////////////////////////////////////////////////////////////
//
G4VPhysicalVolume* exrdm01GeometryConstruction::Construct ()
{
  //
  //
  // Define materials.
  //
  G4double a, iz, z, density,pressure, temperature;
  G4String name, symbol;

 
  density     = universe_mean_density;    //from PhysicalConstants.h
  pressure    = 3.e-18*pascal;
  temperature = 2.73*kelvin;
  G4Material* Vacuum   = new G4Material("Vacuum",
                                        1., 1.01*g/mole, density,
                                        kStateGas,temperature,pressure);    
  //
  //
  // Define bodies, logical volumes and physical volumes.
  // First define the experimental hall.
  //
  G4Box * universe_box
    = new G4Box("universe_b",universe_x,universe_y,universe_z);
  G4LogicalVolume * universe_log
    = new G4LogicalVolume(universe_box,Vacuum,"universe_L",0,0,0);
  G4VPhysicalVolume * universe_phys
    = new G4PVPlacement(0,G4ThreeVector(),"universe_P",
                        universe_log,0,false,0);
  //
  //
  // Define outer sphere.
  //
  //  G4Sphere * aSphere_sph1
    //    = new G4Sphere("aSphere1",0,aSphere_r1,0.*deg,360.*deg,0.*deg,180.*deg);
  G4Box * aSphere_sph1
    = new G4Box("aSphere1",aSphere_r1, aSphere_r1, aSphere_r1);
  G4LogicalVolume * aSphere_log1
    = new G4LogicalVolume(aSphere_sph1,Vacuum,"aSphere_L1",0,0,0);
  G4VPhysicalVolume * aSphere_phys1
    = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),"aSphere_P1",
    aSphere_log1,universe_phys,false,0);
  //
  //
  // Define middle sphere.
  //
  G4Sphere * aSphere_sph2
    = new G4Sphere("aSphere2",0,aSphere_r2,0.*deg,360.*deg,0.*deg,180.*deg);
  G4LogicalVolume * aSphere_log2
    = new G4LogicalVolume(aSphere_sph2,Vacuum,"aSphere_L2",0,0,0);
  G4VPhysicalVolume * aSphere_phys2
    = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),"aSphere_P2",aSphere_log2,
                      universe_phys,false,0);

  return universe_phys;
}






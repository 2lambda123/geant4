// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//
#include "exGPSGeometryConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"


////////////////////////////////////////////////////////////////////////////////
//
exGPSGeometryConstruction::exGPSGeometryConstruction ()
{;}
////////////////////////////////////////////////////////////////////////////////
//
exGPSGeometryConstruction::~exGPSGeometryConstruction ()
{;}
////////////////////////////////////////////////////////////////////////////////
//
G4VPhysicalVolume* exGPSGeometryConstruction::Construct ()
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
  a = 26.98154*g/mole;
  density = 2.70*g/cm3;
  G4Material* Aluminium = new G4Material(name="aluminium", z=13., a, density);

  a = 28.0855*g/mole;
  G4Element* elSi = new G4Element(name="silicon", symbol="Si", iz=14., a);

  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxygen", symbol="O", iz=8., a);

  density = 2.65*g/cm3;
  G4Material* SiliconDioxide =
    new G4Material(name="silicon oxide", density, 2);
  SiliconDioxide->AddElement(elSi, 1);
  SiliconDioxide->AddElement(elO,  2);
  //
  // Define size of experiment hall and volumes which will fill it.
  //
  G4double world_r = 150.0*mm;

  G4double box_x = 100.0*mm;
  G4double box_y = 100.0*mm;
  G4double box_z = 100.0*mm;

  G4double sphere_r = 50.0*mm;

  // Define bodies, logical volumes and physical volumes.
  // First define the experimental hall.
  //
  G4Sphere * universe_s 
    = new G4Sphere("universe_s",0,world_r,0.*deg,360.*deg,0.*deg,180.*deg);
  G4LogicalVolume * universe_log
    = new G4LogicalVolume(universe_s,Vacuum,"universe_L",0,0,0);
  //
  universe_phys
    = new G4PVPlacement(0,G4ThreeVector(),"universe_P",
                        universe_log,0,false,0);
  //define an aluminium box
  //
  G4Box * Al_box
    = new G4Box("Al_b",box_x,box_y,box_z);  //
  G4LogicalVolume * Al_log
    = new G4LogicalVolume(Al_box,Aluminium,"Box_log",0,0,0);
  //
  Al_phys
    = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),"Box_phys",
			Al_log,universe_phys,false,0);
  //
  // Define an inner sphere.
  //x1
  G4Sphere * aSphere_sph
    = new G4Sphere("aSphere",0,sphere_r,0.*deg,360.*deg,0.*deg,180.*deg);
  G4LogicalVolume * aSphere_log
    = new G4LogicalVolume(aSphere_sph,SiliconDioxide,"Sphere_log",0,0,0);
  //
  aSphere_phys
    = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),"Sphere_phys",aSphere_log,
			Al_phys,false,0);
  
//--------- Visualization attributes -------------------------------
  universe_log->SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes* aVisAtt= new G4VisAttributes(G4Colour(0,1.0,1.0));
  Al_log->SetVisAttributes(aVisAtt);
  G4VisAttributes* bVisAtt= new G4VisAttributes(G4Colour(1.0,2.0,.0));
  aSphere_log->SetVisAttributes(bVisAtt);

  return universe_phys;
}












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

#include "Tst01DetectorConstruction.hh"

#include "Tst01DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"

#include "G4BooleanSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4AffineTransform.hh"
#include "G4Transform3D.hh"
#include "G4VoxelLimits.hh"

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4TransportationManager.hh"
#include "G4GeometryManager.hh"
#include "G4StateManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4TransportationManager.hh"

//////////////////////////////////////////////////////////////////
//
// Constructor/Destructor

Tst01DetectorConstruction::Tst01DetectorConstruction()
:simpleBoxLog(NULL),simpleBoxDetector(NULL),honeycombDetector(NULL),
 detectorChoice(0),selectedMaterial(NULL),Air(NULL),Al(NULL),Pb(NULL),
 fChoiceCSG(0),fChoiceBool(0),fWorldPhysVol(NULL),
 fTestCSG(NULL),fTestLog(NULL),fTestVol(NULL),
 AssemblyDetectorLog(0),AssemblyDetector(0),AssemblyCalo(0),AssemblyCellLog(0)
{
  detectorMessenger = new Tst01DetectorMessenger(this);
  materialChoice = "Pb";
}

Tst01DetectorConstruction::~Tst01DetectorConstruction()
{
  delete detectorMessenger;

  // Clean up Assembly setup
  if( AssemblyCalo != 0 ) delete +AssemblyCalo;
}

//////////////////////////////////////////////////////////////////////
//
// 

G4VPhysicalVolume* Tst01DetectorConstruction::Construct()
{
  if((!simpleBoxDetector)&&(!honeycombDetector)&&(!AssemblyDetector))
  {
    ConstructDetectors();
  }

  switch(detectorChoice)
  { 
    case 1:
      fWorldPhysVol = honeycombDetector; 
      break;
    case 2:
      fWorldPhysVol = AssemblyDetector; 
      break;
    default:
      fWorldPhysVol = simpleBoxDetector;
  }
  return fWorldPhysVol;
}

////////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SwitchDetector()
{
  if((!simpleBoxDetector)&&(!honeycombDetector)&&(!AssemblyDetector))
  {
    ConstructDetectors();
  }
  
  switch(detectorChoice)
  { 
    case 1:
    {
      G4TransportationManager::GetTransportationManager()->
                               GetNavigatorForTracking()->
                               SetWorldVolume(honeycombDetector);
      fWorldPhysVol = honeycombDetector ;
      break;
    }
    case 2:
    {
      G4TransportationManager::GetTransportationManager()->
                               GetNavigatorForTracking()->
                               SetWorldVolume(AssemblyDetector);
      fWorldPhysVol = AssemblyDetector ;
      break;
    }
    default:
    {
      G4TransportationManager::GetTransportationManager()->
                               GetNavigatorForTracking()->
                               SetWorldVolume(simpleBoxDetector);
      fWorldPhysVol = simpleBoxDetector ;
    }
  }
  
  G4ThreeVector center(0,0,0);
  G4TransportationManager::GetTransportationManager()->
                           GetNavigatorForTracking()->
                           LocateGlobalPointAndSetup(center,0,false);
}

/////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SelectDetector(G4String val)
{
  if(val == "Honeycomb")
    detectorChoice = 1 ;
  else if(val == "Assembly") 
    detectorChoice = 2 ;
  else
    detectorChoice = 0 ;

  G4cout << "Now Detector is " << val << G4endl ;
}
/////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SelectCSG(G4String name)
{
  if     ( name == "Tubs"   ) fChoiceCSG = 1 ; 
  else if( name == "Cons"   ) fChoiceCSG = 2 ;
  else if( name == "Sphere" ) fChoiceCSG = 3 ;
  else                        fChoiceCSG = 0 ; // default or error in name
 
  G4cout << "Now CGS is " << name << G4endl;
}

////////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SwitchCSG()
{
  SelectMaterialPointer();

  // if( (!simpleBoxDetector) && (!honeycombDetector) ) ConstructDetectors();

  // G4VPhysicalVolume* worldPhysVol =    G4TransportationManager::
  // GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume();

  if( fTestCSG ) fTestCSG = NULL ;
  
  switch(fChoiceCSG)
  { 
    case 1 :
    {
      fTestCSG = new G4Tubs("testCSG", 20*cm, 50*cm, 50*cm, 0, 2*pi) ;
      break ;
    }
    case 2 :
    {
      fTestCSG = new G4Cons("testCSG",10*cm,30*cm, 20*cm,50*cm, 50*cm, 0, 2*pi) ;
      break ;
    }
    case 3 :
    {
      fTestCSG = new G4Sphere("testCSG",20*cm,50*cm, 0,2*pi, 0,2*pi) ;
      break ;
    }
    //  new G4Torus("testCSG",20*cm,40*cm,50*cm,0,2*pi) ;

    default:
    {
      fTestCSG = new G4Box("testCSG", 20*cm, 50*cm, 50*cm ) ;
    }
  }
  if( fTestLog ) fTestLog = NULL ;
  fTestLog = new G4LogicalVolume(fTestCSG,selectedMaterial,
                                                "testLog",0,0,0) ;
  if( fTestVol ) fTestVol = NULL ;
  fTestVol = new G4PVPlacement(0,G4ThreeVector(),
				   "testVol",fTestLog, 
                                    fWorldPhysVol,
				    // simpleBoxDetector,
                                    false,0);  
}

/////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SelectBoolean(G4String name)
{
  if(      name == "Union"   )     fChoiceBool = 1 ; 
  else if( name == "Subtraction" ) fChoiceBool = 2 ;
  else                             fChoiceBool = 0 ; // default or error in name
 
  G4cout << "Now Boolean is " << name << G4endl;
}

////////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SwitchBoolean()
{
  SelectMaterialPointer();
  if( (!simpleBoxDetector) && (!honeycombDetector) ) ConstructDetectors();

  // G4VPhysicalVolume* worldPhysVol =    G4TransportationManager::
  // GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume();

  G4RotationMatrix identity, xRot ;
    
// NOTE: xRot = rotation such that x axis->y axis & y axis->-x axis

  xRot.rotateZ(-M_PI*0.5) ;

  G4Transform3D transform(xRot,G4ThreeVector()) ;
  


  G4Box* pb1 = new G4Box("b1",50*cm,50*cm,50*cm) ;
  G4Box* pb2 = new G4Box("b2",10*cm,10*cm,60*cm) ;
  G4Box* pb3 = new G4Box("b3",40*cm,40*cm,40*cm) ;
  G4Box* pb4 = new G4Box("b4",50*cm,50*cm, 5*cm) ;

  G4DisplacedSolid* disPb4 = new G4DisplacedSolid("disPb4",pb4,&identity,
                                                   G4ThreeVector(0,0,60*cm)) ;

  G4Tubs* tubs1 = new G4Tubs("tubs1",80*cm,90*cm,50*cm,0,2*pi) ;

  G4Sphere* sphere1 = new G4Sphere("shere1",80*cm,90*cm,0,2*pi,0,pi) ;
  G4Sphere* sphere2 = new G4Sphere("shere2",0,50*cm,0,2*pi,0,pi) ;
  G4Sphere* sphere3 = new G4Sphere("shere3",0,40*cm,0,2*pi,0,pi) ;

  G4VSolid* testBool ;
  
  switch(fChoiceBool)
  { 
    case 1 :
    {
      // testBool = new G4UnionSolid("b1UnionB2", pb1, pb2) ;
      // testBool = new G4UnionSolid("b1UnionT1", pb1, tubs1) ;
      testBool = new G4UnionSolid("b1UnionDisPb4", pb1, disPb4) ;
      testBool = new G4UnionSolid("sp1UnionDisSp2",sphere1,sphere2) ;
      break ;
    }
    case 2 :
    {
      testBool = new G4SubtractionSolid("b1SubtractB2", pb1, pb2) ;
      break ;
    }
    default:
    {
      testBool = new G4IntersectionSolid("b1IntersectB2", pb1, pb2) ;
    }
  }



  G4LogicalVolume*   testBoolLog = new G4LogicalVolume(testBool,selectedMaterial,
                                                "testBoolLog",0,0,0) ;

  G4VPhysicalVolume* testBoolVol = new G4PVPlacement(0,G4ThreeVector(),
                             "testBoolVol",testBoolLog,fWorldPhysVol,false,0);

  // daughters
  
  G4LogicalVolume*   testD1Log = new G4LogicalVolume(sphere3,selectedMaterial,
                                                "testD1Log",0,0,0) ;

  G4VPhysicalVolume* testD1Vol = new G4PVPlacement(0,G4ThreeVector(),
                             "testD1Vol",testD1Log,testBoolVol,false,0);  

  G4LogicalVolume*   testD2Log = new G4LogicalVolume(sphere1,selectedMaterial,
                                                "testD2Log",0,0,0) ;

  G4VPhysicalVolume* testD2Vol = new G4PVPlacement(0,G4ThreeVector(0,0,0),
                             "testD2Vol",testD2Log,testBoolVol,false,0);  

}

//////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SelectMaterial(G4String val)
{
  materialChoice = val;
  SelectMaterialPointer();
  G4cout << "Daughter CSG/Boolean will be made of " << materialChoice << G4endl;
}

///////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::SelectMaterialPointer()
{

//
// Material definition 
//

  G4double a, iz, z, density;
  G4String name, symbol;
  G4int nel;

  if(!Air)
  {
    a = 14.01*g/mole;
    G4Element* elN = new G4Element(name="Nitrogen", symbol="N", iz=7., a);
    a = 16.00*g/mole;
    G4Element* elO = new G4Element(name="Oxigen", symbol="O", iz=8., a);
    density = 1.29e-03*g/cm3;
    Air = new G4Material(name="Air", density, nel=2);
    Air->AddElement(elN, .7);
    Air->AddElement(elO, .3);
  }

  if(!Al)
  {
    a = 26.98*g/mole;
    density = 2.7*g/cm3;
    Al = new G4Material(name="Aluminium", z=13., a, density);
  }

  if(!Pb)
  {
    a = 207.19*g/mole;
    density = 11.35*g/cm3;
    Pb = new G4Material(name="Lead", z=82., a, density);
  }

  if(materialChoice=="Air")
  { selectedMaterial = Air; }
  else if(materialChoice=="Al")
  { selectedMaterial = Al; }
  else
  { selectedMaterial = Pb; }

  if(simpleBoxLog)
  { simpleBoxLog->SetMaterial(selectedMaterial); }
}

/////////////////////////////////////////////////////////////////////////
//
//

void Tst01DetectorConstruction::ConstructDetectors()
{
  //  SelectMaterialPointer();

//   G4VSolid, G4LogicalVolume, G4VPhysicalVolume  
//
//   simpleBoxDetector 

  G4Box * mySimpleBox = new G4Box("SBox",2000*cm, 2000*cm, 2000*cm);
  simpleBoxLog = new G4LogicalVolume( mySimpleBox,Air,
				      // selectedMaterial,
                                      "SLog",0,0,0);
  simpleBoxDetector = new G4PVPlacement(0,G4ThreeVector(),
                                        "SPhys",simpleBoxLog,0,false,0);



//
//  honeycombDetector
//

  G4double offset=22.5*cm, xTlate, yTlate;
  G4int i,j,copyNo;

  G4Box *myWorldBox= new G4Box("WBox",2000*cm, 2000*cm, 2000*cm);
  G4Box *myCalBox = new G4Box("CBox",1500*cm, 1500*cm, 1000*cm);
  G4Tubs *myTargetTube 
     = new G4Tubs("TTube",0*cm, 22.5*cm, 1000*cm, 0.*deg, 360.*deg);

  G4LogicalVolume *myWorldLog=new G4LogicalVolume(myWorldBox,Air,
						    "WLog", 0, 0, 0);
  G4LogicalVolume *myCalLog=new G4LogicalVolume(myCalBox,Al,
						  "CLog", 0, 0, 0);
  G4LogicalVolume *myTargetLog=new G4LogicalVolume(myTargetTube,Pb,
						     "TLog", 0, 0, 0);

  honeycombDetector = new G4PVPlacement(0,G4ThreeVector(),
						 "WPhys",
						 myWorldLog,
						 0,false,0);

  const double worldX              = 2000*mm;
  const double worldY              = 2000*mm;
  const double worldZ              = 2000*mm;

  const double caloX               = 1600*mm;
  const double caloY               = 1600*mm;
  const double caloZ               =  200*mm;

  const double plateX              =  700*mm;
  const double plateY              =  700*mm;
  const double plateZ              =  100*mm;

  const unsigned int layers        =    5;

  const double firstCaloPos        =  500*mm;
  const double caloCaloOffset      =   50*mm;
  const double plateCaloOffset     =    1*mm;
  const double platePlateOffset    =    2*mm;

  // Define world volume for Assembly detector
  G4Box* AssemblyBox                   = new G4Box( "AssemblyBox", worldX/2., worldY/2., worldZ/2. );
  G4LogicalVolume* AssemblyDetectorLog = new G4LogicalVolume( AssemblyBox, selectedMaterial, "AssemblyDetectorLog", 0, 0, 0);
  AssemblyDetector                     = new G4PVPlacement(0, G4ThreeVector(), "AssemblyDetector", AssemblyDetectorLog, 0, false, 0);

  // Define a calorimeter plate
  G4Box* AssemblyCellBox = new G4Box( "AssemblyCellBox", plateX/2., plateY/2., plateZ/2. );
  AssemblyCellLog        = new G4LogicalVolume( AssemblyCellBox, Pb, "AssemblyCellLog", 0, 0, 0 );
  
  // Define one calorimeter layer as one assembly volume
  AssemblyCalo = new G4AssemblyVolume();

  // Rotation and translation of a plate inside the assembly
  G4RotationMatrix        Ra;
  G4ThreeVector           Ta;

  // Rotation of the assembly inside the world
  G4RotationMatrix        Rm;

  // Fill the assembly by the plates  
  Ta.setX( caloX/4. );  Ta.setY( caloY/4. );  Ta.setZ( 0. );
  AssemblyCalo->AddPlacedVolume( AssemblyCellLog, Ta, &Ra );
  
  Ta.setX( -1*caloX/4. );  Ta.setY( caloY/4. );  Ta.setZ( 0. );
  AssemblyCalo->AddPlacedVolume( AssemblyCellLog, Ta, &Ra );
  
  Ta.setX( -1*caloX/4. );  Ta.setY( -1*caloY/4. );  Ta.setZ( 0. );
  AssemblyCalo->AddPlacedVolume( AssemblyCellLog, Ta, &Ra );

  Ta.setX( caloX/4. );  Ta.setY( -1*caloY/4. );  Ta.setZ( 0. );
  AssemblyCalo->AddPlacedVolume( AssemblyCellLog, Ta, &Ra );
  
  // Now instantiate the layers of calorimeter
  for( i = 0; i < layers; i++ )
  {
    // Translation of the assembly inside the world
    G4ThreeVector Tm( 0,0,i*(caloZ + caloCaloOffset) - firstCaloPos );
    AssemblyCalo->MakeImprint( AssemblyDetectorLog, Tm, &Rm );
  }

  /*  *****************************************************

  G4PVPlacement *myCalPhys=new G4PVPlacement(0,G4ThreeVector(),
					       "CalPhys",
					       myCalLog,
                                               honeycombDetector,
					       false,0);

  G4String tName1("TPhys1");	// Allow all target physicals to share
				// same name (delayed copy)
  copyNo=0;
  for (j=1;j<=25;j++)
  {
    yTlate = -1000.0*cm - 40.0*cm + j*80.0*cm;
    for (i=1;i<=50;i++)
    {
      xTlate = -1000.0*cm - 20.0*cm + i*45.0*cm - offset;
      G4PVPlacement *myTargetPhys
        =new G4PVPlacement(0,G4ThreeVector(xTlate,yTlate,0*cm),
                           tName1,myTargetLog,myCalPhys,false,copyNo++);
    }
  }
  for (j=1;j<=26;j++)
  {
    yTlate = -1000.0*cm - 80.0*cm + j*80.0*cm;
    for (i=1;i<=50;i++)
    {
      xTlate = -1000.0*cm - 20.0*cm + i*45.0*cm;
      G4PVPlacement *myTargetPhys
        =new G4PVPlacement(0,G4ThreeVector(xTlate,yTlate,0*cm),
                           tName1,myTargetLog,myCalPhys,false,copyNo++);
    }
  }

  ************************************************ */

//
// Visualization attributes 
// 

  G4VisAttributes * simpleBoxVisAtt
    = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  simpleBoxLog->SetVisAttributes(simpleBoxVisAtt);

  G4VisAttributes * experimentalHallVisAtt
    = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  experimentalHallVisAtt->SetVisibility(false);
  myWorldLog->SetVisAttributes(experimentalHallVisAtt);

  G4VisAttributes * calorimeterBoxVisAtt
    = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  calorimeterBoxVisAtt->SetForceWireframe(true);
  calorimeterBoxVisAtt->SetVisibility(true);
  myCalLog->SetVisAttributes(calorimeterBoxVisAtt);

  G4VisAttributes * calorimeterTubeVisAtt
    = new G4VisAttributes(G4Colour(0.0,0.5,0.5));
  calorimeterTubeVisAtt->SetForceWireframe(true);
  calorimeterTubeVisAtt->SetVisibility(true);
  myTargetLog->SetVisAttributes(calorimeterTubeVisAtt);

}


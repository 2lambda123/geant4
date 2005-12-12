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
// $Id: Em10DetectorConstruction.cc,v 1.15 2005-12-12 10:16:31 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#include "Em10DetectorConstruction.hh"
#include "Em10DetectorMessenger.hh"
#include "Em10CalorimeterSD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4ProductionCuts.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

/////////////////////////////////////////////////////////////////////////////
//
//

Em10DetectorConstruction::Em10DetectorConstruction()
  :worldchanged(false), AbsorberMaterial(0),  fGapMat(0),
   WorldMaterial(0),  solidWorld(0),      logicWorld(0),      physiWorld(0),
   fSolidRadSlice(0), fLogicRadSlice(0),  fPhysicRadSlice(0),
   solidRadiator(0),  logicRadiator(0),   physiRadiator(0),
   fRadiatorMat(0),
   solidAbsorber(0),  logicAbsorber(0),   physiAbsorber(0),
   magField(0),       calorimeterSD(0),   fRegGasDet(0), fRadRegion(0)
{
  // default parameter values of the calorimeter


  //  G4double inch = 2.54*cm ;
  // G4double  mil = inch/1000.0 ;

  WorldSizeZ = 400.*cm; // 200.*cm;
  WorldSizeR = 20.*cm;

  // Radiator and detector parameters


  fRadThickness = 0.020*mm;   // 0.0127*mm ; // 25*micrometer ;     
  fGasGap       = 0.250*mm;  // 0.762*mm ;          //  1500*micrometer  ;   
  foilGasRatio  = fRadThickness/(fRadThickness+fGasGap) ;
  fFoilNumber   = 220;  // 100 ;             //  188 ;

  fAlphaPlate   = 160.0;
  fAlphaGas     = 160.0;
  fModelNumber  = 0;


  AbsorberThickness = 38.3*mm; // 15.0*mm ; // 40.0*mm ;

  AbsorberRadius   = 1.*mm;
  zAbsorber        = 136.*cm; // 36.*cm ;

  fWindowThick    = 51.0*micrometer ;
  fElectrodeThick = 10.0*micrometer ;
  fGapThick       =  1.0*mm ;


  fDetThickness =  40.0*mm ;
  fDetLength    = 200.0*cm  ;
  fDetGap       =   0.01*mm ;


  fStartR       = 40*cm  ;
  fStartZ       = 100.0*mm  ;

  fModuleNumber = 1      ;  

  // create commands for interactive definition of the calorimeter  

  // fGammaCut    = 23*mm; 
  // fElectronCut = 23*mm; 
  // fPositronCut = 23*mm; 


  detectorMessenger = new Em10DetectorMessenger(this);
}

//////////////////////////////////////////////////////////////////////////
//
//

Em10DetectorConstruction::~Em10DetectorConstruction()
{ 
  delete detectorMessenger;
}

//////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume* Em10DetectorConstruction::Construct()
{
  DefineMaterials();
  return ConstructCalorimeter();  
}

//////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::DefineMaterials()
{ 
  //This function illustrates the possible ways to define materials
  
  G4String name, symbol ;                            //a =mass of a mole;
  G4double a, z ;   //z =mean number of protons;  
  G4double density, foilDensity, gasDensity, totDensity ;
  G4double fractionFoil, fractionGas ;
  G4int nel ; 
  
  //G4int ncomponents, natoms;
  G4int  ncomponents;
  //G4double abundance, fractionmass;
  G4double fractionmass;
  //G4double temperature, pressure;
  
  //
  // define Elements
  //
  
  a = 1.01*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);
  
  a = 6.94*g/mole;
  G4Element* elLi  = new G4Element(name="Lithium",symbol="Li" , z= 3., a);
  
  a = 9.01*g/mole;
  G4Element* elBe  = new G4Element(name="Berillium",symbol="Be" , z= 4., a);
  
  a = 12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);
  
  a = 14.01*g/mole;
  G4Element* elN  = new G4Element(name="Nitrogen",symbol="N" , z= 7., a);
  
  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
  
  a = 39.948*g/mole;
  G4Element* elAr = new G4Element(name="Argon", symbol="Ar", z=18., a);
  
  a = 131.29*g/mole;
  G4Element* elXe = new G4Element(name="Xenon", symbol="Xe", z=54., a);
  
  a = 19.00*g/mole;
  G4Element* elF  = new G4Element(name="Fluorine", symbol="F", z=9., a);

  /////////////////////////////////////////////////////////////////
  //
  // Detector windows, electrodes 
  // Al for electrodes
  
  density = 2.700*g/cm3;
  a = 26.98*g/mole;
  G4Material* Al = new G4Material(name="Aluminium", z=13., a, density);
  
  
  ////////////////////////////////////////////////////////////////////////////
  //
  // Materials for popular X-ray TR radiators
  //
  
  // TRT_CH2
  
  density = 0.935*g/cm3;
  G4Material* TRT_CH2 = new G4Material(name="TRT_CH2",density, nel=2);
  TRT_CH2->AddElement(elC,1);
  TRT_CH2->AddElement(elH,2);
  
  // Radiator
  
  density = 0.059*g/cm3;
  G4Material* Radiator = new G4Material(name="Radiator",density, nel=2);
  Radiator->AddElement(elC,1);
  Radiator->AddElement(elH,2);
  
  // Carbon Fiber
  
  density = 0.145*g/cm3;
  G4Material* CarbonFiber = new G4Material(name="CarbonFiber",density, nel=1);
  CarbonFiber->AddElement(elC,1);
  
  // Lithium
  
  density = 0.534*g/cm3;
  G4Material* Li = new G4Material(name="Li",density, nel=1);
  Li->AddElement(elLi,1);
  
  // Beryllium
  
  density = 1.848*g/cm3;
  G4Material* Be = new G4Material(name="Be",density, nel=1);
  Be->AddElement(elBe,1);
  
  // Mylar
  
  density = 1.39*g/cm3;
  G4Material* Mylar = new G4Material(name="Mylar", density, nel=3);
  Mylar->AddElement(elO,2);
  Mylar->AddElement(elC,5);
  Mylar->AddElement(elH,4);

// Kapton Dupont de Nemur (density: 1.396-1.430, get middle )

  density = 1.413*g/cm3;
  G4Material* Kapton = new G4Material(name="Kapton", density, nel=4);
  Kapton->AddElement(elO,5);
  Kapton->AddElement(elC,22);
  Kapton->AddElement(elN,2);
  Kapton->AddElement(elH,10);

  
  // Kapton (polyimide) ??? since = Mylar C5H4O2
  
  // density = 1.39*g/cm3;
  // G4Material* Kapton = new G4Material(name="Kapton", density, nel=3);
  // Kapton->AddElement(elO,2);
  // Kapton->AddElement(elC,5);
  // Kapton->AddElement(elH,4);
  
  // Polypropelene

  G4Material* CH2 = new G4Material ("CH2" , 0.91*g/cm3, 2);
  CH2->AddElement(elH,2);
  CH2->AddElement(elC,1);
  
  //////////////////////////////////////////////////////////////////////////
  //
  // Noble gases , STP conditions
  
  // Helium as detector gas, STP
  
  density = 0.178*mg/cm3 ;
  a = 4.0026*g/mole ;
  G4Material* He  = new G4Material(name="He",z=2., a, density );
  
  // Neon as detector gas, STP

  density = 0.900*mg/cm3 ;
  a = 20.179*g/mole ;
  G4Material* Ne  = new G4Material(name="Ne",z=10., a, density );

  // Argon as detector gas, STP

  density = 1.7836*mg/cm3 ;       // STP
  G4Material* Argon = new G4Material(name="Argon"  , density, ncomponents=1);
  Argon->AddElement(elAr, 1);

  // Krypton as detector gas, STP

  density = 3.700*mg/cm3 ;
  a = 83.80*g/mole ;
  G4Material* Kr  = new G4Material(name="Kr",z=36., a, density );

  // Xenon as detector gas, STP

  density = 5.858*mg/cm3 ;
  a = 131.29*g/mole ;
  G4Material* Xe  = new G4Material(name="Xenon",z=54., a, density );

/////////////////////////////////////////////////////////////////////////////
//
// Hydrocarbones, metane and others

  // Metane, STP
  
  density = 0.7174*mg/cm3 ;
  G4Material* metane = new G4Material(name="CH4",density,nel=2) ;
  metane->AddElement(elC,1) ;
  metane->AddElement(elH,4) ;
  
  // Propane, STP
  
  density = 2.005*mg/cm3 ;
  G4Material* propane = new G4Material(name="C3H8",density,nel=2) ;
  propane->AddElement(elC,3) ;
  propane->AddElement(elH,8) ;
  
  // iso-Butane (methylpropane), STP
  
  density = 2.67*mg/cm3 ;
  G4Material* isobutane = new G4Material(name="isoC4H10",density,nel=2) ;
  isobutane->AddElement(elC,4) ;
  isobutane->AddElement(elH,10) ;


///////////////////////////////////////////////////////////////////////////
//
// Molecular gases

  // Carbon dioxide, STP 

  density = 1.977*mg/cm3;
  G4Material* CO2 = new G4Material(name="CO2", density, nel=2,
				       kStateGas,273.15*kelvin,1.*atmosphere);
  CO2->AddElement(elC,1);
  CO2->AddElement(elO,2);

  // Carbon dioxide, STP

  density = 1.977*273.*mg/cm3/293.;
  G4Material* CarbonDioxide = new G4Material(name="CO2", density, nel=2);
  CarbonDioxide->AddElement(elC,1);
  CarbonDioxide->AddElement(elO,2);


  // Nitrogen, STP

  density = 1.25053*mg/cm3 ;       // STP
  G4Material* Nitrogen = new G4Material(name="N2"  , density, ncomponents=1);
  Nitrogen->AddElement(elN, 2);

 // Oxygen, STP

  density = 1.4289*mg/cm3 ;       // STP
  G4Material* Oxygen = new G4Material(name="O2"  , density, ncomponents=1);
  Oxygen->AddElement(elO, 2);

  /* *****************************
  density = 1.25053*mg/cm3 ;       // STP
  a = 14.01*g/mole ;       // get atomic weight !!!
  //  a = 28.016*g/mole;
  G4Material* N2  = new G4Material(name="Nitrogen", z= 7.,a,density) ;

  density = 1.25053*mg/cm3 ;       // STP
  G4Material* anotherN2 = new G4Material(name="anotherN2", density,ncomponents=2);
  anotherN2->AddElement(elN, 1);
  anotherN2->AddElement(elN, 1);

  // air made from oxigen and nitrogen only

  density = 1.290*mg/cm3;  // old air from elements
  G4Material* air = new G4Material(name="air"  , density, ncomponents=2);
  air->AddElement(elN, fractionmass=0.7);
  air->AddElement(elO, fractionmass=0.3);
  ******************************************** */

  // Dry Air (average composition), STP

  density = 1.2928*mg/cm3 ;       // STP
  G4Material* Air = new G4Material(name="Air"  , density, ncomponents=3);
  Air->AddMaterial( Nitrogen, fractionmass = 0.7557 ) ;
  Air->AddMaterial( Oxygen,   fractionmass = 0.2315 ) ;
  Air->AddMaterial( Argon,    fractionmass = 0.0128 ) ;
  
  ////////////////////////////////////////////////////////////////////////////
  //
  // MWPC mixtures

  // 85% Xe + 15% CO2, STP
  
  density = 4.9*mg/cm3;
  G4Material* Xe15CO2 = new G4Material(name="Xe15CO2"  , density, ncomponents=2);
  Xe15CO2->AddMaterial( Xe,              fractionmass = 0.979);
  Xe15CO2->AddMaterial( CarbonDioxide,   fractionmass = 0.021);
  
  // 80% Xe + 20% CO2, STP
  
  density = 5.0818*mg/cm3 ;      
  G4Material* Xe20CO2 = new G4Material(name="Xe20CO2"  , density, ncomponents=2);
  Xe20CO2->AddMaterial( Xe,              fractionmass = 0.922 ) ;
  Xe20CO2->AddMaterial( CarbonDioxide,   fractionmass = 0.078 ) ;
  
  // 70% Xe + 27% CO2 + 3% O2, 20 1 atm ATLAS straw tube mixture
  
  density = 4.358*mg/cm3;
  G4Material* Xe27CO23O2 = new G4Material(name="Xe27CO23O2"  , density, ncomponents=3);
  Xe27CO23O2->AddMaterial( Xe,            fractionmass = 0.87671);
  Xe27CO23O2->AddMaterial( CarbonDioxide, fractionmass = 0.11412);
  Xe27CO23O2->AddMaterial( Oxygen,        fractionmass = 0.00917);


  // 80% Kr + 20% CO2, STP
  
  density = 3.601*mg/cm3 ;      
  G4Material* Kr20CO2 = new G4Material(name="Kr20CO2", density, 
				       ncomponents=2);
  Kr20CO2->AddMaterial( Kr,              fractionmass = 0.89 ) ;
  Kr20CO2->AddMaterial( CarbonDioxide,   fractionmass = 0.11 ) ;
  
  // Xe + 55% He + 15% CH4 ; NIM A294 (1990) 465-472; STP
  
  density = 1.963*273.*mg/cm3/293.;
  G4Material* Xe55He15CH4 = new G4Material(name="Xe55He15CH4",density,
					   ncomponents=3);
  Xe55He15CH4->AddMaterial(Xe, 0.895);
  Xe55He15CH4->AddMaterial(He, 0.050);
  Xe55He15CH4->AddMaterial(metane,0.055);
  
  // 90% Xe + 10% CH4, STP ; NIM A248 (1986) 379-388
  
  density = 5.344*mg/cm3 ;      
  G4Material* Xe10CH4 = new G4Material(name="Xe10CH4"  , density, 
				       ncomponents=2);
  Xe10CH4->AddMaterial( Xe,       fractionmass = 0.987 ) ;
  Xe10CH4->AddMaterial( metane,   fractionmass = 0.013 ) ;
  
  // 95% Xe + 5% CH4, STP ; NIM A214 (1983) 261-268
  
  density = 5.601*mg/cm3 ;      
  G4Material* Xe5CH4 = new G4Material(name="Xe5CH4"  , density, 
				      ncomponents=2);
  Xe5CH4->AddMaterial( Xe,       fractionmass = 0.994 ) ;
  Xe5CH4->AddMaterial( metane,   fractionmass = 0.006 ) ;
  
  // 80% Xe + 20% CH4, STP ; NIM A253 (1987) 235-244
  
  density = 4.83*mg/cm3 ;      
  G4Material* Xe20CH4 = new G4Material(name="Xe20CH4"  , density, 
				       ncomponents=2);
  Xe20CH4->AddMaterial( Xe,       fractionmass = 0.97 ) ;
  Xe20CH4->AddMaterial( metane,   fractionmass = 0.03 ) ;

  // 93% Ar + 7% CH4, STP ; NIM 107 (1973) 413-422

  density = 1.709*mg/cm3 ;      
  G4Material* Ar7CH4 = new G4Material(name="Ar7CH4"  , density, 
                                                  ncomponents=2);
  Ar7CH4->AddMaterial( Argon,       fractionmass = 0.971 ) ;
  Ar7CH4->AddMaterial( metane,   fractionmass = 0.029 ) ;

  // 93% Kr + 7% CH4, STP ; NIM 107 (1973) 413-422

  density = 3.491*mg/cm3 ;      
  G4Material* Kr7CH4 = new G4Material(name="Kr7CH4"  , density, 
                                                  ncomponents=2);
  Kr7CH4->AddMaterial( Kr,       fractionmass = 0.986 ) ;
  Kr7CH4->AddMaterial( metane,   fractionmass = 0.014 ) ;

  // 0.5*(95% Xe + 5% CH4)+0.5*(93% Ar + 7% CH4), STP ; NIM A214 (1983) 261-268

  density = 3.655*mg/cm3 ;      
  G4Material* XeArCH4 = new G4Material(name="XeArCH4"  , density, 
                                                  ncomponents=2);
  XeArCH4->AddMaterial( Xe5CH4,       fractionmass = 0.766 ) ;
  XeArCH4->AddMaterial( Ar7CH4,   fractionmass = 0.234 ) ;


  // Preparation of mixed radiator material

  foilDensity =  1.39*g/cm3; // Mylar // 0.91*g/cm3;  // CH2 0.534*g/cm3; //Li     
  gasDensity  =  1.2928*mg/cm3; // Air // 1.977*mg/cm3; // CO2 0.178*mg/cm3; // He  
  totDensity  = foilDensity*foilGasRatio + gasDensity*(1.0-foilGasRatio) ;

  fractionFoil =  foilDensity*foilGasRatio/totDensity ; 
  fractionGas  =  gasDensity*(1.0-foilGasRatio)/totDensity ;  
    
  G4Material* radiatorMat = new G4Material(name="radiatorMat"  , totDensity, 
                                                  ncomponents=2);
  radiatorMat->AddMaterial( Mylar, fractionFoil ) ;
  radiatorMat->AddMaterial( Air, fractionGas  ) ;

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
  // default materials of the calorimeter and TR radiator

  fRadiatorMat =  radiatorMat; // CH2; // Mylar ; 
  fFoilMat     = Mylar; // CH2; // Kapton; // Mylar ; // Li ; // CH2 ;  
  fGasMat      = Air; // CO2; // He; //   
  
  fWindowMat    = Mylar ;
  fElectrodeMat = Al ;

  AbsorberMaterial = Xe15CO2; // Xe55He15CH4; // Xe10CH4; // Xe27CO23O2;  // Ar7CH4; // 
 

  fGapMat          = Xe15CO2; // Xe55He15CH4;

  WorldMaterial    = Air; // CO2 ;  

}

/////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume* Em10DetectorConstruction::ConstructCalorimeter()
{

 // Cleanup old geometry

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();





  //  G4int i, j ; 
  // G4int j ;
  //  G4double zModule, zRadiator, rModule, rRadiator ; 
  G4double zModule; // zRadiator ;
  
  // complete the Calor parameters definition and Print 

  ComputeCalorParameters();
  PrintCalorParameters();
      
  // World
  
  // if(solidWorld) delete solidWorld ;
  // if(logicWorld) delete logicWorld ;
  // if(physiWorld) delete physiWorld ;

  solidWorld = new G4Box("World",				//its name
                   WorldSizeR,WorldSizeR,WorldSizeZ/2.)       ;//its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   WorldMaterial,	//its material
                                   "World");		//its name
                                   
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 "World",		//its name
                                 logicWorld,		//its logical volume
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number

  // TR radiator envelope

  G4double radThick = fFoilNumber*(fRadThickness + fGasGap) - fGasGap + fDetGap   ;

  G4double zRad = fStartZ + 0.5*radThick ;
  G4cout<<"zRad = "<<zRad/mm<<" mm"<<G4endl ;

  //  radThick *= 1.02 ;

  G4cout<<"radThick = "<<radThick/mm<<" mm"<<G4endl ;
  G4cout<<"fFoilNumber = "<<fFoilNumber<<G4endl ;
  G4cout<<"fRadiatorMat = "<<fRadiatorMat->GetName()<<G4endl ;
  G4cout<<"WorldMaterial = "<<WorldMaterial->GetName()<<G4endl ;
 
  //  if(solidRadiator) delete solidRadiator;
  //  if(logicRadiator) delete logicRadiator;
  //  if(physiRadiator) delete physiRadiator;

  solidRadiator = new G4Box("Radiator",1.1*AbsorberRadius , 
                                              1.1*AbsorberRadius, 
                                              0.5*radThick             ) ; 
                         
  logicRadiator = new G4LogicalVolume(solidRadiator,	
                                                       fRadiatorMat,      
                                                       "Radiator");	       
                                   
  physiRadiator = new G4PVPlacement(0,
                                     G4ThreeVector(0,0,zRad),	        
                                     "Radiator", logicRadiator,		
                                     physiWorld, false,	0       );  	

  //  if(fSolidRadSlice) delete fSolidRadSlice;
  //  if(fLogicRadSlice) delete fLogicRadSlice; 
  //  if(fPhysicRadSlice) delete fPhysicRadSlice; 

  fSolidRadSlice = new G4Box("RadSlice",AbsorberRadius,
                                AbsorberRadius,0.5*fRadThickness ) ;

  fLogicRadSlice = new G4LogicalVolume(fSolidRadSlice,fRadiatorMat,
                                          "RadSlice",0,0,0);

  zModule = fStartZ ;   //  ??? + fRadThickness ; 
  G4cout<<"zModule = "<<zModule/mm<<" mm"<<G4endl ;
  /*
    for(j=0;j<fFoilNumber;j++)
    {  

      zRadiator = zModule + j*(fRadThickness + fGasGap) ;
      G4cout<<zRadiator/mm<<" mm"<<"\t" ;
      //   G4cout<<"j = "<<j<<"\t" ;         
      
      fPhysicRadSlice = new G4PVPlacement(0,G4ThreeVector(0.,0.,zRadiator-zRad),
                                         "RadSlice",fLogicRadSlice,
                                          physiRadiator,false,j);
     }                                 
  G4cout<<G4endl ;
  */
   
  G4double zWindow = fStartZ + radThick + fWindowThick/2. + 15.0*mm ;    
      			                  
  G4Box* solidWindowR = new G4Box("WindowR",AbsorberRadius+0.001,
                                          AbsorberRadius+0.001,
                                          fWindowThick/2.+0.001  ); 
                          
  G4LogicalVolume* logicWindowR = new G4LogicalVolume(solidWindowR,
                                     WorldMaterial, "WindowR"); 
  G4VPhysicalVolume*    physiWindowR = new G4PVPlacement(0,		   
                        G4ThreeVector(0.,0.,zWindow),        
                              "Window",logicWindowR,physiWorld,false,0); 
      			                  
  G4Box* solidWindow = new G4Box("Window",AbsorberRadius,
                                          AbsorberRadius,
                                          fWindowThick/2.  ); 
                          
  G4LogicalVolume* logicWindow = new G4LogicalVolume(solidWindow,
                                     fWindowMat, "Window"); 
  G4VPhysicalVolume*    physiWindow = new G4PVPlacement(0,		   
                        G4ThreeVector(0.,0.,0.),        
                              "Window",logicWindow,physiWindowR,false,0); 

  // create region for window inside windowR for G4ForwardXrayTR class

  if( fRadRegion != 0 )  // remove obsolete root logical volume
  {
    // fRadRegion->RemoveRootLogicalVolume(logicWindowR);
    delete fRadRegion;
  }
  // G4ProductionCuts* cutsR = 0;

  if( fRadRegion == 0 ) // First time - instantiate a region and a cut objects
  {    
    fRadRegion = new G4Region("XTRradiator");
    // cutsR = new G4ProductionCuts();
    // fRadRegion->SetProductionCuts(cutsR);
  }
  else  // Second time - get a cut object from region
  {   
    // cutsR = fRadRegion->GetProductionCuts();
  }
  fRadRegion->AddRootLogicalVolume(logicWindowR);                               

  // cutsR->SetProductionCut(fGammaCut,"gamma");
  // cutsR->SetProductionCut(fElectronCut,"e-");
  // cutsR->SetProductionCut(fPositronCut,"e+");




  G4Box* solidGap = new G4Box("Gap",AbsorberRadius,
                                    AbsorberRadius,
                                    fGapThick/2.     ) ; 
                          
  G4LogicalVolume* logicGap = new G4LogicalVolume(solidGap,fGapMat, "Gap"); 

  G4double zGap = zWindow + fWindowThick/2. + fGapThick/2. + 0.01*mm ;    
      			                  
  // G4VPhysicalVolume*    physiGap = new G4PVPlacement(0,		   
  // 		                       G4ThreeVector(0.,0.,zGap),        
  //                                    "Gap",logicGap,physiWorld,false,0); 

  G4Box* solidElectrode = new G4Box("Electrode",AbsorberRadius,
                                                AbsorberRadius,
                                                fElectrodeThick/2. ); 
                          
  G4LogicalVolume* logicElectrode = new G4LogicalVolume(solidElectrode,
                                        fElectrodeMat, "Electrode"); 

  G4double zElectrode = zGap + fGapThick/2. + fElectrodeThick/2. + 0.01*mm;    
      			                  
  //  G4VPhysicalVolume*    physiElectrode = new G4PVPlacement(0,		   
  //  		                       G4ThreeVector(0.,0.,zElectrode),        
  //                                    "Electrode",logicElectrode,
  //                                     physiWorld,false,0);    
                               
  // Absorber

  zAbsorber = zElectrode + fElectrodeThick/2. + AbsorberThickness/2. + 0.01*mm; 
  G4cout<<"zAbsorber = "<<zAbsorber/mm<<" mm"<<G4endl;
  if (AbsorberThickness > 0.) 
  { 
    //  if(solidAbsorber) delete solidAbsorber ;
    //  if(logicAbsorber) delete logicAbsorber ;
    //  if(physiAbsorber) delete physiAbsorber ;

      solidAbsorber = new G4Box("Absorber",AbsorberRadius,		
                                           AbsorberRadius,
                                           AbsorberThickness/2.   ); 
                          
      logicAbsorber = new G4LogicalVolume(solidAbsorber,    
      			                  AbsorberMaterial, 
      			                  "Absorber");     
      			                  
      physiAbsorber = new G4PVPlacement(0,		   
      		    G4ThreeVector(0.,0.,zAbsorber),        
                                        "Absorber",        
                                        logicAbsorber,     
                                        physiWorld,       
                                        false,             
                                        0);                
                                        
  }
  

  if( fRegGasDet != 0 )  // remove obsolete root logical volume
  {
    // fRegGasDet->RemoveRootLogicalVolume(logicAbsorber);
    delete fRegGasDet;
  }
  // G4ProductionCuts* cuts = 0;

  if( fRegGasDet == 0 ) // First time - instantiate a region and a cut objects
  {    
    fRegGasDet = new G4Region("XTRdEdxDetector");
    // cuts = new G4ProductionCuts();
    //  fRegGasDet->SetProductionCuts(cuts);
  }
  else  // Second time - get a cut object from region
  {   
    //  cuts = fRegGasDet->GetProductionCuts();
  }
  fRegGasDet->AddRootLogicalVolume(logicAbsorber);                               

  // cuts->SetProductionCut(fGammaCut,"gamma");
  // cuts->SetProductionCut(fElectronCut,"e-");
  // cuts->SetProductionCut(fPositronCut,"e+");

                                 
  // Sensitive Detectors: Absorber 
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if(!calorimeterSD)
  {
    calorimeterSD = new Em10CalorimeterSD("CalorSD",this);
    SDman->AddNewDetector( calorimeterSD );
  }
  if (logicAbsorber)  logicAbsorber->SetSensitiveDetector(calorimeterSD);

  // always return physics world

  return physiWorld;
}

////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::PrintCalorParameters()
{
  G4cout << "\n The  WORLD   is made of " 
       << WorldSizeZ/mm << "mm of " << WorldMaterial->GetName() ;
  G4cout << ", the transverse size (R) of the world is " << WorldSizeR/mm << " mm. " << G4endl;
  G4cout << " The ABSORBER is made of " 
       << AbsorberThickness/mm << "mm of " << AbsorberMaterial->GetName() ;
  G4cout << ", the transverse size (R) is " << AbsorberRadius/mm << " mm. " << G4endl;
  G4cout << " Z position of the (middle of the) absorber " << zAbsorber/mm << "  mm." << G4endl;
  G4cout << G4endl;
}

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name   
  G4Material* pttoMaterial;

  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J]; 
    
    if(pttoMaterial->GetName() == materialChoice)
    {
      AbsorberMaterial = pttoMaterial;
      logicAbsorber->SetMaterial(pttoMaterial); 
        // PrintCalorParameters();
    }             
  }
}
///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetRadiatorMaterial(G4String materialChoice)
{
  // get the pointer to the material table

  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name
   
  G4Material* pttoMaterial;
  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J];
     
    if(pttoMaterial->GetName() == materialChoice)
    {
      fRadiatorMat = pttoMaterial;
      fLogicRadSlice->SetMaterial(pttoMaterial); 
      // PrintCalorParameters();
    }             
  }
}

////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name   
  G4Material* pttoMaterial;

  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J];
     
    if(pttoMaterial->GetName() == materialChoice)
    {
      WorldMaterial = pttoMaterial;
      logicWorld->SetMaterial(pttoMaterial); 
       //  PrintCalorParameters();
    }             
  }
}

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberThickness(G4double val)
{
  // change Absorber thickness and recompute the calorimeter parameters
  AbsorberThickness = val;
  ComputeCalorParameters();
}  

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetRadiatorThickness(G4double val)
{
  // change XTR radiator thickness and recompute the calorimeter parameters
  fRadThickness = val;
  // ComputeCalorParameters();
}
  
///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetGasGapThickness(G4double val)
{
  // change XTR gas gap thickness and recompute the calorimeter parameters
  fGasGap = val;
  // ComputeCalorParameters();
}  

/////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberRadius(G4double val)
{
  // change the transverse size and recompute the calorimeter parameters
  AbsorberRadius = val;
  ComputeCalorParameters();
}  

////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetWorldSizeZ(G4double val)
{
  worldchanged=true;
  WorldSizeZ = val;
  ComputeCalorParameters();
}  

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetWorldSizeR(G4double val)
{
  worldchanged=true;
  WorldSizeR = val;
  ComputeCalorParameters();
}  

//////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberZpos(G4double val)
{
  zAbsorber  = val;
  ComputeCalorParameters();
}  

//////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetMagField(G4double)
{
  //apply a global uniform magnetic field along X axis

  /* *********************************************************

  G4FieldManager* fieldMgr 
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    
  if(magField) delete magField;		//delete the existing magn field
  
  if(fieldValue!=0.)			// create a new one if non nul
  { 
    magField = new G4UniformMagField(G4ThreeVector(fieldValue,0.,0.));        
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  } 
  else 
  {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }

  *************************************************************** */

}

///////////////////////////////////////////////////////////////////////////////
//
//
  
void Em10DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
}

//
//
////////////////////////////////////////////////////////////////////////////


















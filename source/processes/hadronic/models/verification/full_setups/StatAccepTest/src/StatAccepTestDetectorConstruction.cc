#include "StatAccepTestDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"

#include "G4PVReplica.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
      
#include "StatAccepTestDetectorMessenger.hh"

#include "StatAccepTestSensitiveCalorimeter.hh"
#include "G4SDManager.hh"

#include "StatAccepTestAnalysis.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
           

StatAccepTestDetectorConstruction::StatAccepTestDetectorConstruction() :  
  Vacuum(0), Iron(0), Copper(0), Tungsten(0), Lead(0), Uranium(0), 
  PbWO4(0), Polystyrene(0), LiquidArgon(0), Silicon(0), Quartz(0),
  theAbsorberMaterial(0), theActiveMaterial(0),
  experimentalHall_log(0), experimentalHall_phys(0),
  logicCalo(0), physiCalo(0),
  logicModule(0), physiModule(0),
  logicAbsorber(0), physiAbsorber(0),
  logicActive(0), physiActive(0),
  fieldMgr(0), uniformMagField(0), 
  detectorMessenger(0), 
  theSensitiveCalorimeter(0), 
  theVisAttAbsorber(0), theVisAttActive(0),

  // Default values.
  theIsCalHomogeneous( false ),    // Sampling calorimeter.
  theIsUnitInLambda( false ),      // Unit of length for the absorber total length.
  theAbsorberTotalLength( 2.0*m ), 
  theActiveLayerNumber( 50 ),
  theActiveLayerSize( 4.0*mm ),
  theReadoutLayerNumber( 50 ),
  theIsRadiusUnitInLambda( false ), // Unit of length for the radius bin size.
  theRadiusBinSize( 10.0*cm ),
  theRadiusBinNumber( 11 )
  
{
  // G4cout << " BEGIN  StatAccepTestDetectorConstruction::StatAccepTestDetectorConstruction()" << G4endl; //***DEBUG***

  fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  // materials
  DefineMaterials();
  theAbsorberMaterial = Iron;
  theActiveMaterial   = Polystyrene;

  detectorMessenger = new StatAccepTestDetectorMessenger(this);

  // G4cout << " END  StatAccepTestDetectorConstruction::StatAccepTestDetectorConstruction()" << G4endl; //***DEBUG***
}


StatAccepTestDetectorConstruction::~StatAccepTestDetectorConstruction() {
  delete detectorMessenger;
  delete uniformMagField;
}


G4VPhysicalVolume* StatAccepTestDetectorConstruction::Construct() {
  // G4cout << " BEGIN  StatAccepTestDetectorConstruction::Construct()" << G4endl; //***DEBUG***

  return ConstructCalorimeter();
}


void StatAccepTestDetectorConstruction::DefineMaterials() { 

  // G4cout << " BEGIN  StatAccepTestDetectorConstruction::DefineMaterials()" 
  //	    << G4endl; //***DEBUG***

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density, pressure, temperature, fractionmass;
  G4String name, symbol;
  G4int nel, natoms;

  //--- elements

  a = 1.01*g/mole;
  G4Element* elH = new G4Element(name="Hydrogen", symbol="H2", z=1., a);

  a = 2.01*g/mole;
  //G4Element* elD = new G4Element(name="Deuterium", symbol="D", z=1., a);

  a = 4.*g/mole;
  //G4Element* elHe = new G4Element(name="Helium", symbol="He", z=2., a);

  a = 6.94*g/mole;
  //G4Element* elLi = new G4Element(name="Lithium", symbol="Li", z=3., a);

  a = 9.01*g/mole;
  //G4Element* elBe = new G4Element(name="Berillium", symbol="Be", z=4., a);

  a = 12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);

  a = 14.01*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen", symbol="N2", z=7., a);

  a = 16.*g/mole;
  G4Element* elO = new G4Element(name="Oxygen", symbol="O2", z=8., a);

  a = 20.18*g/mole;
  //G4Element* elNe = new G4Element(name="Neon", symbol="Ne", z=10., a);

  a = 22.99*g/mole;
  //G4Element* elNa = new G4Element(name="Sodium", symbol="Na", z=11., a);

  a = 26.98*g/mole;
  //G4Element* elAl = new G4Element(name="Aluminium", symbol="Al", z=13., a);

  a = 28.085*g/mole;
  G4Element* elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);

  a = 40.08*g/mole;
  //G4Element* elCa = new G4Element(name="Calcium", symbol="Ca", z=20., a);

  a = 55.850*g/mole;
  //G4Element* elFe = new G4Element(name="Iron", symbol="Fe", z=26., a);

  a = 63.54*g/mole;
  //G4Element* elCu = new G4Element(name="Copper", symbol="Cu", z=29., a);

  a = 183.85*g/mole;
  G4Element* elW = new G4Element(name="Tungstenm", symbol="W", z=74., a);

  a = 207.19*g/mole;
  G4Element* elPb = new G4Element(name="Lead", symbol="Pb", z=82., a);

  a = 238.03*g/mole;
  //G4Element* elU = new G4Element(name="Uranium", symbol="U", z=92., a);

  //--- simple materials

  density = 2.7*g/cm3;
  a = 26.98*g/mole;
  //G4Material* Aluminium = new G4Material(name="Aluminium", z=13., a, density);
  
  // Iron has a  X0 = 1.7585 cm  and  lambda_I = 16.760 cm.   
  density = 7.87*g/cm3;
  a = 55.85*g/mole;
  Iron = new G4Material(name="Iron", z=26., a, density);

  // Copper has a  X0 = 1.4353 cm  and  lambda_I = 15.056 cm.   
  density = 8.96*g/cm3;
  a = 63.54*g/mole;
  Copper = new G4Material(name="Copper", z=29., a, density);

  // Tungsten has a  X0 = 0.35 cm  and  lambda_I = 9.5855 cm. 
  density = 19.3*g/cm3;
  a = 183.85*g/mole;
  Tungsten = new G4Material(name="Tungsten", z=74., a, density);

  // Lead has a  X0 = 0.56120 cm  and  lambda_I = 17.092 cm.  
  density = 11.35*g/cm3;
  a = 207.19*g/mole;
  Lead = new G4Material(name="Lead", z=82., a, density);

  // Uranium has a  X0 = 0.31662 cm  and  lambda_I = 10.501 cm.  
  density =  18.95*g/cm3;
  a = 238.03*g/mole;
  Uranium = new G4Material(name="Uranium", z=92., a, density);

  density = 1.4*g/cm3;
  a = 39.95*g/mole;
  LiquidArgon = new G4Material(name="LiquidArgon", z=18., a, density);

  density = 0.002*g/cm3;
  a = 39.95*g/mole;
  //G4Material* ArgonGas = new G4Material(name="ArgonGas", z=18., a, density);

  density = 2.33*g/cm3;
  a = 28.085*g/mole;
  Silicon = new G4Material(name="Silicon", z=14., a, density);
  
  density = 8.96*g/cm3;
  a = 58.69*g/mole;
  //G4Material* Nickel = new G4Material(name="Nickel", z=28., a, density);

  //--- mixtures

  density = 1.290*mg/cm3;
  G4Material* Air = new G4Material(name="Air", density, nel=2);
  Air->AddElement(elN, 0.7);
  Air->AddElement(elO, 0.3);

  density     = 1.e-5*g/cm3;
  pressure    = 2.e-2*bar;
  temperature = STP_Temperature;  // From PhysicalConstants.h .
  Vacuum = new G4Material(name="Vacuum", density, nel=1,
			  kStateGas, temperature, pressure);
  Vacuum->AddMaterial(Air, fractionmass=1.);

  // Plastic scintillator tiles (used both in CMS hadron calorimeter
  // and ATLAS hadron barrel calorimeter).
  density = 1.032*g/cm3;
  Polystyrene = new G4Material(name="Polystyrene", density, nel=2);
  Polystyrene->AddElement(elC, natoms=19);
  Polystyrene->AddElement(elH, natoms=21);

  // PbWO4 CMS crystals. It has a  X0 = 0.89 cm  and  lambda_I = 22.4 cm. 
  density = 8.28*g/cm3;
  PbWO4 = new G4Material(name="PbWO4", density, nel=3);
  PbWO4->AddElement(elPb, natoms=1);
  PbWO4->AddElement(elW,  natoms=1);
  PbWO4->AddElement(elO,  natoms=4);

  Quartz = new G4Material(name="Quartz", density=2.200*g/cm3, nel=2);
  Quartz->AddElement(elSi, 1);
  Quartz->AddElement(elO , 2);

  // G4cout << " END  StatAccepTestDetectorConstruction::DefineMaterials()" 
  //	    << G4endl; //***DEBUG***

}


G4VPhysicalVolume* StatAccepTestDetectorConstruction::ConstructCalorimeter() {

  // G4cout << " BEGIN  StatAccepTestDetectorConstruction::ConstructCalorimeter()" 
  //	    << G4endl; //***DEBUG***

  if ( ! areParametersOK() ) {
    G4cout << " StatAccepTestDetectorConstruction::ConstructCalorimeter() : ***ERROR*** "
           << G4endl << "\t PARAMETERS NOT WELL-DEFINED! GEOMETRY UNCHANGED."
           << G4endl;
    return experimentalHall_phys;
  }

  // Clean old geometry, if any.
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4double lambda = 0.0, X0 = 0.0;
  if ( theIsUnitInLambda ) {
    if ( theAbsorberMaterial == Iron ) {
      lambda = 16.760*cm;
      X0     = 1.7585*cm; 
    } else if ( theAbsorberMaterial == Copper ) {
      lambda = 15.056*cm;
      X0     = 1.4353*cm; 
    } else if ( theAbsorberMaterial == Tungsten ) {
      lambda = 9.5855*cm;
      X0     = 0.35*cm; 
    } else if ( theAbsorberMaterial == Lead ) {
      lambda = 17.092*cm;
      X0     = 0.56120*cm; 
    } else if ( theAbsorberMaterial == PbWO4 ) {
      lambda = 22.4*cm;
      X0     = 0.89*cm; 
    } else if ( theAbsorberMaterial == Uranium ) {
      lambda = 10.501*cm;
      X0     = 0.31662*cm; 
    } else {
      std::cout << "ERROR: absorber material not recognized" << std::endl;
    }
  }

  //------------------- volumes --------------------------

  double absorberTotalLength = theAbsorberTotalLength;
  if ( theIsUnitInLambda ) {
     absorberTotalLength *= lambda; 
  }

  // --- experimental hall (world volume)
  //     beam line along the Z-axis
  G4double expHall_x = 10.0*m;  // half dimension along x 
  G4double expHall_y = 10.0*m;  // half dimension along y
  G4double expHall_z = 10.0*m;  // half dimension along z

  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);

  experimentalHall_log = new G4LogicalVolume(experimentalHall_box, // solid 
                                             Vacuum,               // material
                                             "expHall_log",        // name
                                             0,                    // field manager
                                             0,                    // sensitive detector
                                             0);                   // user limits

  experimentalHall_phys = new G4PVPlacement(0,                     // rotation
                                            G4ThreeVector(),       // translation
                                            "expHall",             // name
                                            experimentalHall_log,  // logical volume
                                            0,                     // mother physical volume
                                            false,                 // boolean operation
                                            0);                    // copy number
  
  // --- Detector
  // The idea is to use Replica placement. 
  // To do that, we have to define two extra volumes: the "calorimeter" volume 
  // and the "module". The former, which has the world as its mother volume, 
  // is the mother of the module volume. The calorimeter volume is completely 
  // filled by a number (theActiveLayerNumber) of replicas of the module volume. 
  // A module volume, in its turn, is the mother volume of the absorber layer + 
  // active layer. 

  //            --- absorber layer : logical
  G4double xAbsorber = absorberTotalLength / 2.0;    // half dimension along x
  G4double yAbsorber = absorberTotalLength / 2.0;    // half dimension along y
  G4double zAbsorber = absorberTotalLength / static_cast<double>( theActiveLayerNumber );
  // In the case of homogenous calorimeter the "active" part must be
  // subtracted because it is made of the same material.
  if ( theIsCalHomogeneous ) { 
    zAbsorber -= theActiveLayerSize; 
  }
  zAbsorber /= 2.0;                                  // half dimension along z

  G4Box* solidAbsorber = new G4Box("solidAbsorber", xAbsorber, yAbsorber, zAbsorber);

  logicAbsorber = new G4LogicalVolume(solidAbsorber,       // solid 
                                      theAbsorberMaterial, // material
                                      "logicAbsorber",     // name
                                      0,                   // field manager
                                      0,                   // sensitive detector
                                      0);                  // user limits

  //            --- active layer : logical
  G4double xActive = xAbsorber;                 // half dimension along x 
  G4double yActive = yAbsorber;                 // half dimension along y 
  G4double zActive = theActiveLayerSize / 2.0;  // half dimension along z 

  G4Box* solidActive = new G4Box("solidActive", xActive, yActive, zActive);

  logicActive = new G4LogicalVolume(solidActive,           // solid 
                                    theActiveMaterial,     // material
                                    "logicActive",         // name
                                    0,                     // field manager
                                    0,                     // sensitive detector
                                    0);                    // user limits

  //        --- module : logical
  G4double xModule = xAbsorber;            // half dimension along x
  G4double yModule = yAbsorber;            // half dimension along y
  G4double zModule = (zAbsorber+zActive);  // half dimension along z 

  G4Box* solidModule = new G4Box("solidModule", xModule, yModule, zModule);

  logicModule = new G4LogicalVolume(solidModule,    // solid 
                                    Lead,           // material, it does NOT matter
                                    "logicModule",  // name
                                    0,              // field manager
                                    0,              // sensitive detector
                                    0);             // user limits

  //    --- calorimeter : logical
  G4int numberOfModules = theActiveLayerNumber;
  G4double xCalo = xModule;                  // half dimension along x
  G4double yCalo = yModule;                  // half dimension along y
  G4double zCalo = numberOfModules*zModule;  // half dimension along z 

  G4Box* solidCalo = new G4Box("solidCalo", xCalo, yCalo, zCalo);

  logicCalo = new G4LogicalVolume(solidCalo,        // solid 
                                  Lead,             // material, it does NOT matter
                                  "logicCalo",      // name
                                  0,                // field manager
                                  0,                // sensitive detector
                                  0);               // user limits

  //            --- absorber layer : physical
  G4double zpos = - zActive;
  physiAbsorber = new G4PVPlacement(0,                       // rotation
                                    G4ThreeVector(0,0,zpos), // translation
                                    logicAbsorber,           // logical volume
                                    "physiAbsorber",         // name
                                    logicModule,             // mother logical volume
                                    false,                   // boolean operation
                                    1000);                   // copy number

  //            --- active layer : physical
  zpos += zAbsorber + zActive;
  physiActive = new G4PVPlacement(0,                       // rotation
                                  G4ThreeVector(0,0,zpos), // translation
                                  logicActive,             // logical volume
                                  "physiActive",           // name
                                  logicModule,             // mother logical volume
                                  false,                   // boolean operation
                                  2000);                   // copy number

  //        --- module : physical (using replica)
  physiModule = new G4PVReplica("Calo",                  // name
                                logicModule,             // logical volume
                                logicCalo,               // mother logical volume
                                kZAxis,                  // axis of replication
                                numberOfModules,         // number of replica
                                2*(zAbsorber+zActive) ); // (full) width of replica

  //    --- calorimeter : physical
  physiCalo = new G4PVPlacement(0,                     // rotation
                                G4ThreeVector(),       // translation
                                "physiCalo",           // its name
                                logicCalo,             // logical volume
                                experimentalHall_phys, // mother physical volume
                                false,                 // boolean operation
                                100);                  // copy number

  // G4cout << " StatAccepTestDetectorConstruction::ConstructCalorimeter() : DEBUG Info "
  //        << G4endl
  //        << "\t zAbsorber      = " << zAbsorber / mm <<  " mm " << G4endl
  //	    << "\t zActive        = " << zActive / mm <<  " mm " << G4endl
  //	    << "\t zModule        = " << zModule / mm << " mm " << G4endl
  //	    << "\t total Absorber = " << zAbsorber*numberOfModules / m << " m " << G4endl
  //	    << "\t zCalo          = " << zCalo / m << " m " << G4endl; //***DEBUG***

  // --- Sensitive detectors
  if ( ! theSensitiveCalorimeter ) { 
    theSensitiveCalorimeter = 
      new StatAccepTestSensitiveCalorimeter( "StatAccepTestSensitiveCalorimeter", 
					     numberOfModules );
  }
  theSensitiveCalorimeter->Update( numberOfModules );
  G4SDManager::GetSDMpointer()->AddNewDetector( theSensitiveCalorimeter );
  logicActive->SetSensitiveDetector( theSensitiveCalorimeter );

  // --- Visualization attributes
  experimentalHall_log->SetVisAttributes( G4VisAttributes::Invisible );
  // The World is not visualized.

  logicCalo->SetVisAttributes( G4VisAttributes::Invisible );
  // The calo is not visualized.

  logicModule->SetVisAttributes( G4VisAttributes::Invisible );
  // The calo is not visualized.

  if ( ! theVisAttAbsorber ) {
    theVisAttAbsorber = new G4VisAttributes( G4Colour(1.0,1.0,1.0) );
    theVisAttAbsorber->SetVisibility(true);
    theVisAttAbsorber->SetForceWireframe(true);
  }
  logicAbsorber->SetVisAttributes( theVisAttAbsorber );
  // The absorber layer will appear in white colour.
  // (the order of colours is: (red, green, blue) )
  
  if ( ! theVisAttActive ) {
    theVisAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
    theVisAttActive->SetVisibility(true);
    theVisAttActive->SetForceWireframe(true);
  }
  logicActive->SetVisAttributes( theVisAttActive );
  // The active layer will appear in yellow colour.
  
  // --- Initialize the Analysis object ---

  double radiusBinSize = theRadiusBinSize;
  if ( theIsRadiusUnitInLambda ) {
     radiusBinSize *= lambda; 
  }
  StatAccepTestAnalysis::getInstance()->init( numberOfModules, theReadoutLayerNumber,  
					      theRadiusBinNumber, radiusBinSize );

  // G4cout << " END  StatAccepTestDetectorConstruction::ConstructCalorimeter()" 
  //        << G4endl; //***DEBUG***

  return experimentalHall_phys;
}


G4bool StatAccepTestDetectorConstruction::areParametersOK() {

  bool isOk = true;
  if ( ! theAbsorberMaterial ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : UNDEFINED absorber material" << G4endl;
  }
  if ( ! theActiveMaterial ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : UNDEFINED active material" << G4endl;
  }
  if ( theAbsorberTotalLength <= 0.0 ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : theAbsorberTotalLength = " << theAbsorberTotalLength << G4endl;
  }
  if ( theActiveLayerNumber <= 0 ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : theActiveLayerNumber = " << theActiveLayerNumber << G4endl;
  }
  if ( theActiveLayerSize <= 0.0 ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : theActiveLayerSize = " << theActiveLayerSize << G4endl;
  }
  if ( theReadoutLayerNumber <= 0  ||  
       theActiveLayerNumber % theReadoutLayerNumber != 0 ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : theReadoutLayerNumber = " << theReadoutLayerNumber << G4endl;
  }
  if ( theRadiusBinSize <= 0.0 ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : theRadiusBinSize = " << theRadiusBinSize << G4endl;    
  }
  if ( theRadiusBinNumber <= 0 ) {
    isOk = false;
    G4cout << " StatAccepTestDetectorConstruction::areParametersOK() : theRadiusBinNumber = " << theRadiusBinNumber << G4endl;    
  }

  return isOk;
}


void StatAccepTestDetectorConstruction::SetMagField(G4double fieldValue) {
  if ( uniformMagField ) {
    delete uniformMagField;
  }
  if ( fabs( fieldValue ) > 0.0 ) {
    // Apply a global uniform magnetic field along the Y axis.
    // Notice that only if the magnetic field is not zero, the Geant4
    // transportion in field gets activated.

    uniformMagField = new G4UniformMagField( G4ThreeVector(0.0, fieldValue, 0.0) );

    fieldMgr->SetDetectorField( uniformMagField );
    fieldMgr->CreateChordFinder( uniformMagField );

  } 
}


void StatAccepTestDetectorConstruction::SetAbsorberMaterial(const G4String name) {

  if ( name == "Fe" ||
       name == "Iron" || name == "iron" ) { 
    theAbsorberMaterial = Iron;
  } else if ( name == "Cu" ||
 	      name == "Copper" || name == "copper" ) { 
    theAbsorberMaterial = Copper;
  } else if ( name == "Pb" ||
 	      name == "Lead" || name == "lead" ) { 
    theAbsorberMaterial = Lead;
  } else if ( name == "PbWO4" ) {
    theAbsorberMaterial = PbWO4;
  } else if ( name == "W" ||
 	      name == "Tungsten" || name == "tungsten" ) { 
    theAbsorberMaterial = Tungsten;
  } else if ( name == "U" ||
 	      name == "Uranium" || name == "uranium" ) { 
    theAbsorberMaterial = Uranium;
  } else {
    G4cout << G4endl << G4endl
	   << "WARNING: the name of the material has not been recognized!" << G4endl
	   << "     ===> the default  * Iron *  will be used." 
	   << G4endl << G4endl;  
    theAbsorberMaterial = Iron;
  }
  
  logicAbsorber->SetMaterial( theAbsorberMaterial );
  
  // G4cout << " Absorber Material = " << logicAbsorber->GetMaterial()->GetName() 
  //	    << G4endl;
  
}


void StatAccepTestDetectorConstruction::SetActiveMaterial(const G4String name) {

  if ( name == "Scintillator" || name == "scintillator" ) { 
    theActiveMaterial = Polystyrene;
  } else if ( name == "LAr" ||
 	      name == "LiquidArgon" || name == "liquidArgon" ) { 
    theActiveMaterial = LiquidArgon;
  } else if ( name == "PbWO4" ) {
    theActiveMaterial = PbWO4;
  } else if ( name == "Si" ||
 	      name == "Silicon" || name == "silicon" ) { 
    theActiveMaterial = Silicon;
  } else if ( name == "Quartz" || name == "quartz" ) { 
    theActiveMaterial = Quartz;
  } else {
    G4cout << G4endl << G4endl
	   << "WARNING: the name of the material has not been recognized!" << G4endl
	   << "     ===> the default  * Scintillator *  will be used." 
	   << G4endl << G4endl;  
    theActiveMaterial = Polystyrene;
  }
  
  logicActive->SetMaterial( theActiveMaterial );
  
  // G4cout << " Active Material = " << logicActive->GetMaterial()->GetName() 
  //        << G4endl;
  
}


void StatAccepTestDetectorConstruction::UpdateGeometry() {

  // G4cout << " BEGIN  StatAccepTestDetectorConstruction::UpdateGeometry" << G4endl; //***DEBUG***

  G4RunManager::GetRunManager()->DefineWorldVolume( ConstructCalorimeter() );

  PrintParameters();

  // G4cout << " END  StatAccepTestDetectorConstruction::UpdateGeometry" << G4endl; //***DEBUG***
}


void StatAccepTestDetectorConstruction::PrintParameters() {

  G4cout << G4endl << G4endl
         << " ------  StatAccepTestDetectorConstruction::PrintParameters() ------ " 
	 << G4endl
         << " Absorber Material = ";
  if ( theAbsorberMaterial ) {
    G4cout << theAbsorberMaterial->GetName();
  } else {
    G4cout << " UNDEFINED ";
  }
  G4cout << G4endl << " Active Material   = ";
  if ( theActiveMaterial ) {
    G4cout << theActiveMaterial->GetName();
  } else {
    G4cout << " UNDEFINED ";
  }
  G4cout << G4endl << " Is the Calorimeter Homogeneous ? " << theIsCalHomogeneous;
  G4cout << G4endl << " Is the Unit in Lambda ? " << theIsUnitInLambda;
  G4cout << G4endl << " Absorber Total Length = ";
  if ( theIsUnitInLambda ) {
    G4cout << theAbsorberTotalLength << "  lambdas";
  } else {
    G4cout << theAbsorberTotalLength / m << " m";
  }
  G4cout << G4endl << " Active Layer Number   = " << theActiveLayerNumber;
  G4cout << G4endl << " Active Layer Size     = " << theActiveLayerSize/mm << " mm";
  G4cout << G4endl << " Readout Layer Number  = " << theReadoutLayerNumber;
  G4cout << G4endl << " Is the Radius Unit in Lambda ? " << theIsRadiusUnitInLambda;
  G4cout << G4endl << " Radius Bin Size       = ";
  if ( theIsRadiusUnitInLambda ) {
    G4cout << theRadiusBinSize << "  lambdas";
  } else {
    G4cout << theRadiusBinSize / mm << " mm";
  }
  G4cout << G4endl << " Radius Bin Number     = " << theRadiusBinNumber;
  G4cout << G4endl << " Magnetic field [T]    = ";
  if ( uniformMagField ) {
    G4cout << uniformMagField->GetConstantFieldValue() / tesla;
  } else {
    G4cout << "(0,0,0)";
  }

  G4cout << G4endl << " -------------------------------------------------------- "
         << G4endl << G4endl;

}

// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: test19DetectorMessenger.cc,v 1.3 2001-05-29 13:57:02 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Detector Construction Messenger for visualization testing.
// John Allison 25th April 1997

#include "test19DetectorMessenger.hh"

#include "test19DetectorConstruction.hh"
#include "MyDetectorConstruction.hh"

#include "G4StateManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4VPhysicalVolume.hh"

#include "BuildCalorimeter.hh"
#include "BuildGeom_Example1.hh"
#include "BuildGeom_Example2.hh"
#include "BuildParametrised.hh"

#ifdef ATLAS
#include "ATLASdetector.hh"
#endif

test19DetectorMessenger::test19DetectorMessenger
(test19DetectorConstruction * test19Det)
:test19Detector (test19Det)
{
  G4UIcommand * command;
  G4UIparameter * param;

  command = new G4UIcommand ("/test19det/", this);
  command -> SetGuidance ("test19 detector choice.");
  fpTest19DetCommandDirectory = command;

  command = new G4UIcommand ("/test19det/detector",this);
  command -> SetGuidance ("test19 detector choice.");
  param = new G4UIparameter ("detector id", 'i', true);
  param -> SetDefaultValue (-1);
  command -> SetParameter (param);
  fpDetectorCommand = command;
}

test19DetectorMessenger::~test19DetectorMessenger () {
  delete fpDetectorCommand;
  delete fpTest19DetCommandDirectory;
}

void test19DetectorMessenger::SetNewValue
(G4UIcommand * command, G4String newValues)
{
  if (command == fpDetectorCommand) {
    G4int id;
    const char* aString = newValues;
    G4std::istrstream is((char*) aString) ; is >> id;
#ifdef ATLAS
    const G4int idMax = 5;
#else
    const G4int idMax = 4;
#endif
    if (id < 0 || id > idMax) {
      G4cout << "Available detectors:"
	   << "\n0) Part of original test calorimeter"
	   << "\n1) Example1 (LBNO, no rotation)"
	   << "\n2) Example2 (embryo LBNO, rotated volumes)"
	   << "\n3) A parametrised volume"
	   << "\n4) MyDetector"
#ifdef ATLAS
	   << "\n5) ATLAS (selected part)"
#endif
	   << "\nChoose by specifying integer parameter."
	   << G4endl;
    }
    else {
      G4VPhysicalVolume* pGeom;
      G4VUserDetectorConstruction* detector;
#ifdef ATLAS
      ATLASdetector* atlas;
#endif
      switch (id) {
      default:
      case 0: pGeom = BuildCalorimeter (); break;
      case 1: pGeom = BuildGeom_Example1(); break;
      case 2: pGeom = BuildGeom_Example2(); break;
      case 3: pGeom = BuildParametrised(); break;
      case 4:
	detector = new MyDetectorConstruction;
	//G4cout << 
	//  "Now further commands before constructing MyDetector."
	//  "\nType continue to Construct MyDetector..."
	//     << G4endl;
	//G4StateManager::GetStateManager () -> Pause ();
	pGeom = detector -> Construct ();
	break;
#ifdef ATLAS
      case 5:
	atlas = new ATLASdetector();
	//    G4UImanager* UI = G4UImanager::GetUIpointer ();
	//    UI -> ApplyCommand ("/atlas/innerDetector/barrelSilicon 1");
	//    UI -> ApplyCommand ("/atlas/innerDetector/pixel 1");
	// Build ATLAS detector geometry
	G4cout << 
	  "Now further ATLAS commands before constructing."
	  "\nType continue to Construct ATLAS..."
	     << G4endl;
	G4StateManager::GetStateManager () -> Pause ();
	pGeom = atlas -> buildATLAS ();
	G4cout << "ATLAS geometrical definition Done." << G4endl;
	break;
#endif
      }
      test19Detector -> SetDetector (pGeom);
    }
  }
}

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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//

#include "G4ORNLAdrenal.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Processor/GDMLProcessor.h"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"

G4ORNLAdrenal::G4ORNLAdrenal()
{
}

G4ORNLAdrenal::~G4ORNLAdrenal()
{
  sxp.Finalize();
}

G4VPhysicalVolume* G4ORNLAdrenal::ConstructAdrenal(G4VPhysicalVolume* mother, G4String sex, G4bool sensitivity)
{
  // Initialize GDML Processor
  sxp.Initialize();
  config.SetURI( "gdmlData/"+sex+"/ORNLAdrenal.gdml" );
  config.SetSetupName( "Default" );
  sxp.Configure( &config );

  // Run GDML Processor
  sxp.Run();
 
  G4LogicalVolume* logicAdrenal = (G4LogicalVolume *)GDMLProcessor::GetInstance()->GetLogicalVolume("AdrenalVolume");

  G4ThreeVector position = (G4ThreeVector)*GDMLProcessor::GetInstance()->GetPosition("AdrenalPos");
  G4RotationMatrix* rm = (G4RotationMatrix*)GDMLProcessor::GetInstance()->GetRotation("AdrenalRot");
  
G4PhysicalVolumeStore::DeRegister((G4VPhysicalVolume*)GDMLProcessor::GetInstance()->GetWorldVolume());

  // Define rotation and position here!
  G4VPhysicalVolume* physAdrenal = new G4PVPlacement(rm,position,
      			       "physicalAdrenal",
  			       logicAdrenal,
			       mother,
			       false,
			       0);


  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicAdrenal->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }


  // Visualization Attributes
  G4VisAttributes* AdrenalVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  AdrenalVisAtt->SetForceSolid(true);
  logicAdrenal->SetVisAttributes(AdrenalVisAtt);

  G4cout << "Adrenal created !!!!!!" << G4endl;
  
  return physAdrenal;
}

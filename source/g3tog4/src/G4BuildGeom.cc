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
// $Id: G4BuildGeom.cc,v 1.15 2001-07-16 15:38:21 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// modified by I. Hrivnacova, 13.10.99 

#include "g4std/iomanip"
#include "g4std/fstream"
#include "globals.hh"
#include "G3toG4.hh"
#include "G3MatTable.hh"
#include "G3MedTable.hh"
#include "G3RotTable.hh"
#include "G3VolTable.hh"
#include "G3PartTable.hh"
#include "G3DetTable.hh"
#include "G3toG4BuildTree.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

extern G4std::ofstream ofile;

void G3CLRead(G4String &, char *);
void checkVol(G4LogicalVolume*, G4int);
void checkVol();


G4LogicalVolume* G4BuildGeom(G4String& inFile){

  G4int irot=0;
  G4gsrotm(0, 90, 0, 90, 90, 0, 0);

  G4cout << "Instantiated unit rotation matrix irot=" << irot << G4endl;
 
  // Read the call List and interpret to Generate Geant4 geometry

  G4cout << "Reading the call List file " << inFile << "..." << G4endl;

  G3CLRead(inFile, 0);

  G3Part.PrintAll();

  G3Det.PrintAll();

  G3Vol.PrintAll();

  G4cout << "Call List file read completed. Build geometry" << G4endl;

  // Build the geometry

  G3VolTableEntry* topVTE = G3Vol.GetFirstVTE();
  G4cout << "G3toG4 top level volume is " << topVTE->GetName() << G4endl;

  // modified
  G3toG4BuildTree(topVTE, 0);

  // Retrieve the top-level G3toG4 logical mother volume pointer

  G4LogicalVolume* topLV = topVTE->GetLV();

  // position the top logical volume
  // (in Geant3 the top volume is not positioned)
  // 
  new G4PVPlacement(0, G4ThreeVector(), topLV->GetName(), topLV, 0, false, 0);

  // mark as invisible

  topLV->SetVisAttributes(G4VisAttributes::Invisible);
    
  G4cout << "Top-level G3toG4 logical volume " << topLV->GetName() << " "
         << *(topLV->GetVisAttributes()) << G4endl;
        
        // check the geometry here

  #ifdef G3G4DEBUG
    G4cout << "scan through G4LogicalVolumeStore:" << G4endl;
    checkVol();
  #endif

  return topLV;
}

void checkVol()
{
  G4LogicalVolumeStore* theStore = G4LogicalVolumeStore::GetInstance();
  G4LogicalVolume* ll = (*theStore)[0];
  G4int level=0;
  checkVol(ll, level);
}

void checkVol(G4LogicalVolume* _lvol, G4int level)
{
  G4LogicalVolume* _ldvol;
  G4VPhysicalVolume* _pdvol;
  level++;
  
  G4int ndau = _lvol -> GetNoDaughters();
  
  G4cout << "G44LogicalVolume " << _lvol->GetName() << " at level " << level
	 << " contains " << ndau << " daughters." << G4endl;
  for (G4int idau=0; idau<ndau; idau++){
    _pdvol = _lvol-> GetDaughter(idau);
    _ldvol = _pdvol -> GetLogicalVolume();
    G4cout << "G4VPhysical volume " << G4std::setw(5) << _pdvol -> GetName() 
	 << " (G4LogicalVolume " << G4std::setw(5) << _ldvol->GetName() << ")" 
	 << G4endl;
    checkVol(_ldvol, level);
  }
  return;
}












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
// $Id: Tst33IStoreBuilder.cc,v 1.11 2003-11-25 10:20:25 gcosmo Exp $
// GEANT4 tag 
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// Tst33IStoreBuilder.cc
//
// ----------------------------------------------------------------------

#include "Tst33IStoreBuilder.hh"
#include "G4IStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4GeometryCell.hh"
#include "globals.hh"
#include "Tst33VGeometry.hh"

Tst33IStoreBuilder::Tst33IStoreBuilder()
{}

Tst33IStoreBuilder::~Tst33IStoreBuilder()
{}

G4VIStore *Tst33IStoreBuilder::CreateIStore(Tst33VGeometry *samplegeo) {
  // create an importance store and fill it with the importance
  // per cell values
  const G4VPhysicalVolume &pworld = samplegeo->GetWorldVolume();
  G4IStore *istore=0;
  istore = new G4IStore(pworld);
  if (!istore) {
    G4Exception("Tst33IStoreBuilder::CreateIStore new failed to create G4IStore!");
    }
  // adding GeometryCell for world volume. ReplicaNumer = 0, since  "geomvol-V05-00-01 !
  G4GeometryCell gWorldCell(pworld, 0);
  istore->AddImportanceGeometryCell(1, gWorldCell);
  
  G4int i=1;
  for (i=1; i <= 19; ++i) {
    G4double imp = pow(2.0,i-1);
    G4GeometryCell gCell(samplegeo->GetGeometryCell(i, ""));
    if (i==19) {
	imp = pow(2.0,17);
    }
    else {
      G4GeometryCell gCellMinus(samplegeo->GetGeometryCell(i, "I1-"));
      G4GeometryCell gCellPlus(samplegeo->GetGeometryCell(i, "I1+"));
    
      istore->AddImportanceGeometryCell(imp, gCellMinus);
      istore->AddImportanceGeometryCell(imp, gCellPlus);
    }
    istore->AddImportanceGeometryCell(imp, gCell);
  }
  return istore;
}


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
#ifndef LXeWLSSlab_H
#define LXeWLSSlab_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "LXeDetectorConstruction.hh"

class LXeWLSSlab : public G4PVPlacement
{
public:
  LXeWLSSlab(G4RotationMatrix *pRot,
		const G4ThreeVector &tlate,
		G4LogicalVolume *pMotherLogical,
		G4bool pMany,
		G4int pCopyNo,
		LXeDetectorConstruction* c);
private:
  void CopyValues();
  
  LXeDetectorConstruction* constructor;

  G4bool updated;

  static G4LogicalVolume* ScintSlab_log;

  G4int nfibers;
  G4double scint_x;
  G4double scint_y;
  G4double scint_z;
  G4double slab_z;
};

#endif

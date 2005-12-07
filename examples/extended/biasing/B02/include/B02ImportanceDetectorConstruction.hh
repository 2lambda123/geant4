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
// $Id: B02ImportanceDetectorConstruction.hh,v 1.4 2005-12-07 18:05:55 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef B02ImportanceDetectorConstruction_hh 
#define B02ImportanceDetectorConstruction_hh  B02ImportanceDetectorConstruction_hh 

#include "globals.hh"
#include <map>
#include "G4GeometryCell.hh"
#include "B02PVolumeStore.hh"

class G4VPhysicalVolume;



class B02ImportanceDetectorConstruction
{
public:
  B02ImportanceDetectorConstruction();
  ~B02ImportanceDetectorConstruction();

  const G4VPhysicalVolume &GetPhysicalVolumeByName(const G4String& name) const;
  G4VPhysicalVolume &GetWorldVolume() const;
  G4String ListPhysNamesAsG4String();
  G4String GetCellName(G4int i);
  G4GeometryCell GetGeometryCell(G4int i);

private:
  void Construct();
  B02PVolumeStore fPVolumeStore;
  G4VPhysicalVolume *fWorldVolume;
};

#endif

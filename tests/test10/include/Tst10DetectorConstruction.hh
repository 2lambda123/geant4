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
// $Id: Tst10DetectorConstruction.hh,v 1.4 2004-01-25 14:04:27 grichine Exp $
// ------------------------------------------------------------
//  GEANT 4 class header file 
//
//      This class is a class derived from G4VUserDetectorConstruction
//      for constructing all particles and processes.
//
//  History
//        first version              09 Sept. 1998 by S.Magni
// ------------------------------------------------------------

#ifndef Tst10DetectorConstruction_h
#define Tst10DetectorConstruction_h 1

#include "Tst10DetectorMessenger.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class Tst10DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    Tst10DetectorConstruction();
    ~Tst10DetectorConstruction();

  public:
     G4VPhysicalVolume* Construct();
     G4VPhysicalVolume* SelectDetector (const G4String& val);
     void               SwitchDetector();
     void               SetMaterial();
     G4double           GetHallSize(){return fHallSize;};
  private:
     Tst10DetectorMessenger* detectorMessenger;
     G4VSolid* aVolume;
     G4VPhysicalVolume* PhysicalVolume;
     G4Material* Water;
     G4Material* Water1;
     G4OpticalSurface* aSurface;
     G4double fHallSize;
};

#endif

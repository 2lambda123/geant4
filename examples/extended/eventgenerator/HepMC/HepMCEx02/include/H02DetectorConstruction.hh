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
// ====================================================================
//
//   H02DetectorConstruction.hh
//   $Id: H02DetectorConstruction.hh,v 1.2 2003-12-09 15:31:04 gunter Exp $
//
// ====================================================================
#ifndef H02_DETECTOR_CONSTRUCTION_H
#define H02_DETECTOR_CONSTRUCTION_H
 
#include "G4Types.hh"
#include "G4VUserDetectorConstruction.hh"

class H02DetectorConstruction : public G4VUserDetectorConstruction {
public:
  H02DetectorConstruction();
  ~H02DetectorConstruction(); 

  virtual G4VPhysicalVolume* Construct(); 

};

#endif

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
// $Id: TiaraDimensions.hh,v 1.3 2003-06-25 09:12:38 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
//
// Class TiaraDimensions
//

#ifndef TiaraDimensions_hh
#define TiaraDimensions_hh TiaraGeometry_hh

#include "globals.hh"
#include "G4ThreeVector.hh"

class TiaraDimensions {
public:
  TiaraDimensions();
  ~TiaraDimensions();

  // World volume
  G4double worldHalfLength;
  G4double worldHalfWidth;

  // basic dimensions
  G4double targetPosZ;
  G4double distTargetWall;
  G4double distTargetEndA;
  G4double distTargetExperiment;
  G4double distTargetEndB;
  
  // beam pipe
  G4double pipeRadius;

  // radius iron A
  G4double radiusIronA;
  
  // experiment width
  G4double widthExperiment;

  // detector
  G4double detectorRadius;
  G4double detectorHalfHight;

  // source detector
  G4double srcDetectorWidth;

};

#endif

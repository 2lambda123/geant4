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
// $Id: geomdefs.hh,v 1.4 2001-07-11 10:00:38 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ----------------------------------------------------------------------
// Constants, typedefs, enums for Geometry Section
//
// History:
// 30.06.95 P.Kent

#ifndef GeomDefs_hh
#define GeomDefs_hh

#include "globals.hh"

// `Infinity' - Distance returned for no intersection etc.
static const G4double kInfinity = 9.0E99;

// Thickness of shapes for Inside function / tracking.
// Should be greater than largest math error from the shape 
// distance calculation routines.
// Tolerance is centred on surface: Inside routine uses a
//                                  tolerance dx +/- kTol/2
// Note: values not `tuned', and because of approximations kRadtolerance and
//       kAngTolerance may not always be used as an exact radius
static const G4double kCarTolerance = 1E-9*mm;
static const G4double kRadTolerance = 1E-9*mm;
static const G4double kAngTolerance = 1E-9*rad;

// Minimum cosine of angle between surface normal & track direction
// for exiting normal optimisation
static const double kMinExitingNormalCosine = 1E-3;

// Define axes for function params etc.
// X/Y/ZAxis = Normal Catesian axes
// Rho = Radial axis in cylindrical polar
// Radial3D = Radial axis in spherical polar
// Phi = Phi axis in cylindrical polar
enum EAxis {kXAxis,kYAxis,kZAxis,kRho,kRadial3D,kPhi};

// G4VSolid::Inside return codes
// kSurface => within tolerance of exact surface
enum EInside {kOutside,kSurface,kInside};

// kNormal = (G4PVPlacement) Conventional positioning
// kReplica = (G4PVReplica)  Consumed parameterised case
//                           => Distances & location computed with
//                              simple formulae & MOTHER volume(s)
//                              must also be checked
// kParameterised = (G4PVParameterised) General parameterised volume
//                           => Distance & location computed to volumes
//                              after setup/modification via user object
enum EVolume {kNormal,kReplica,kParameterised};

// Default max size of Navigation history
//
static const G4int kHistoryMax    = 15;

// History increase stride of Navigation history
//
static const G4int kHistoryStride = 16;

// Voxel stack depth maximum [no resizing]
//
static const G4int kNavigatorVoxelStackMax = 3;

#endif /* GeomDefs_hh */

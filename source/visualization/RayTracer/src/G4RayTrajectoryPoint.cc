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
// $Id: G4RayTrajectoryPoint.cc,v 1.4 2004-06-09 07:40:53 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//

///////////////////////
//G4RayTrajectoryPoint.cc
///////////////////////

// Export extern symbols for DLLs build
//
#define G4RAY_TRAJECTORY_POINT_EXPORT 1

#include"G4RayTrajectoryPoint.hh"

G4Allocator<G4RayTrajectoryPoint> G4RayTrajectoryPointAllocator;

G4RayTrajectoryPoint :: G4RayTrajectoryPoint()
{;}

G4RayTrajectoryPoint :: ~G4RayTrajectoryPoint()
{;}


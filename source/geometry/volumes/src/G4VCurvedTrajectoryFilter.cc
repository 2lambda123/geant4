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
// $Id: G4VCurvedTrajectoryFilter.cc,v 1.5 2003-06-16 16:54:59 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4VCurvedTrajectoryFilter.hh"

G4VCurvedTrajectoryFilter::G4VCurvedTrajectoryFilter()
  : fpFilteredPoints(0)
{
}

G4VCurvedTrajectoryFilter::~G4VCurvedTrajectoryFilter()
{
}

std::vector<G4ThreeVector>* 
G4VCurvedTrajectoryFilter::GimmeThePointsAndForgetThem()
{
  std::vector<G4ThreeVector>* tmp = fpFilteredPoints;
  // ParticleChangeForTransport invokes this method (via
  // PropagatorInField) at every Step, even if the step did not
  // involve PropagatorInField. Must, therefore, ensure that points
  // submitted by previous invocations of PIF are not
  // copied. Therefore the points must be cleared. (Note that the
  // responsibility for deleting the vector lies with the
  // SmoothTrajctoryPoint, which is the vector's final destination.)
  // (jacek 08/11/2002)
  fpFilteredPoints = 0;
  return tmp;
}

void
G4VCurvedTrajectoryFilter::CreateNewTrajectorySegment( )
{
  if (fpFilteredPoints)
  {
    // GimmePoints has not been called (it would have set the
    // pointer to NULL), therefore nobody has taken charge of the
    // points and they will never be deleted!
    G4cout << "!!!!!!!! Filter: auxiliary points are being memory leaked !!!!!"
           << G4endl;
  }
  fpFilteredPoints = new std::vector<G4ThreeVector>;
}    

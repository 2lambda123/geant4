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
// $Id: G4HelixExplicitEuler.cc,v 1.5 2003-10-31 14:35:54 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//  Helix Explicit Euler: x_1 = x_0 + helix(h)
//  with helix(h) being a helix piece of length h
//  most simple approach for solving linear differential equations.
//  Take the current derivative and add it to the current position.
//
//  W.Wander <wwc@mit.edu> 12/09/97 
// -------------------------------------------------------------------

#include "G4HelixExplicitEuler.hh"
#include "G4ThreeVector.hh"

void
G4HelixExplicitEuler::DumbStepper( const G4double  yIn[],
				   G4ThreeVector   Bfld,
				   G4double  h,
				   G4double  yOut[])
{
  AdvanceHelix(yIn, Bfld, h, yOut);

  // NormaliseTangentVector( yOut );  // this could harm more than it helps 
}  

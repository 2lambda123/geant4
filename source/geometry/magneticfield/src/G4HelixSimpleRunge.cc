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
// $Id: G4HelixSimpleRunge.cc,v 1.5 2001-07-11 09:59:12 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "G4HelixSimpleRunge.hh"
#include "G4ThreeVector.hh"

//
//  Simple Runge:
//        x_1 = x_0 + h * ( dx( t_0+h/2, x_0 + h/2 * dx( t_0, x_0) ) )
//  W.Wander <wwc@mit.edu> 12/09/97 
//

// -------------------------------------------------------------------------

// second order solver
// take the derivative at a position to be assumed at the middle of the
// Step and add it to the current position.
//

void
G4HelixSimpleRunge::DumbStepper( const G4double  yIn[],
				 G4ThreeVector   Bfld,
				 G4double        h,
				 G4double        yOut[])
{
  const G4int nvar = 6 ;
  G4double yTemp[nvar];   // , yAdd[nvar];
  G4ThreeVector Bfld_midpoint;

  AdvanceHelix( yIn, Bfld, 0.5 * h, yTemp);
  
  // now obtain the new field value at the new point
  MagFieldEvaluate(yTemp, Bfld_midpoint);      

  AdvanceHelix( yIn, Bfld_midpoint, h, yOut);
  
  // NormaliseTangentVector( yOut );           
  
  return ;
}  

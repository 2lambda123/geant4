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
// $Id: G4HelixHeum.cc,v 1.5 2003-10-31 14:35:54 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//  Simple Heum:
//        x_1 = x_0 + h *
//                1/4 * dx(t0,x0)  +
//                3/4 * dx(t0+2/3*h, x0+2/3*h*(dx(t0+h/3,x0+h/3*dx(t0,x0)))) 
//
//  Third order solver.
//
//  W.Wander <wwc@mit.edu> 12/09/97 
// -------------------------------------------------------------------

#include "G4HelixHeum.hh"
#include "G4ThreeVector.hh"

void
G4HelixHeum::DumbStepper( const G4double  yIn[],
			  G4ThreeVector   Bfld,
			  G4double        h,
			  G4double        yOut[])
{
  const G4int nvar = 6 ;

  G4ThreeVector Bfield_Temp, Bfield_Temp2;
  G4double yTemp[6], yAdd1[6], yAdd2[6] , yTemp2[6];

  G4int i;

  AdvanceHelix( yIn, Bfld, h, yAdd1 );
  
  AdvanceHelix( yIn, Bfld, h/3.0, yTemp );
  MagFieldEvaluate(yTemp,Bfield_Temp);

  AdvanceHelix( yIn, Bfield_Temp, (2.0 / 3.0) * h, yTemp2 );
  
  MagFieldEvaluate(yTemp2,Bfield_Temp2);

  AdvanceHelix( yIn, Bfield_Temp2, h, yAdd2 );

  for( i = 0; i < nvar; i++ ) {
    yOut[i] = ( 0.25 * yAdd1[i] + 0.75 * yAdd2[i]);
  }

  // NormaliseTangentVector( yOut );           
}  

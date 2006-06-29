//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4BREPIOtest.cc,v 1.6 2006-06-29 18:43:05 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// GEANT 4 source file
// ----------------------------------------------------------------------
#include "G4BREPSolid.hh"
#include "G4BREPSolidBox.hh"
#include "G4BREPSolidCone.hh"
#include "G4BREPSolidCylinder.hh"
#include "G4BREPSolidOpenPCone.hh"
#include "G4BREPSolidPCone.hh"
#include "G4BREPSolidPolyhedra.hh"
#include "G4BREPSolidSphere.hh"
#include "G4BREPSolidTorus.hh"

#include <iostream>


int main()
{
  G4cout << "Testing generic BREP solid:" << G4endl;
  G4BREPSolid gbs("Generic BREP");
  G4cout << gbs << G4endl;
  
  G4cout << "Testing BREP Box solid:" << G4endl;
  G4BREPSolidBox bb("BREP Box",
                     G4Point3D( 1, 1, 1), G4Point3D( 1,-1, 1), G4Point3D(-1,-1, 1), G4Point3D(-1, 1, 1),
                     G4Point3D( 1, 1,-1), G4Point3D( 1,-1,-1), G4Point3D(-1,-1,-1), G4Point3D(-1, 1,-1)
                   );
  G4cout << bb << G4endl;
  
  G4cout << "Testing BREP Cone solid:" << G4endl;
  G4BREPSolidCone bc("BREP Cone",
                      G4ThreeVector( 0, 0, 0 ), G4ThreeVector( 0, 0, 1 ), G4ThreeVector( 1, 0, 0 ),
				              10*mm, 5*mm,	10*mm
                    );
  G4cout << bc << G4endl;
  
  G4cout << "Testing BREP Cylinder solid:" << G4endl;
  G4BREPSolidCylinder bcyl("BREP Cylinder",
                      G4ThreeVector( 0, 0, 0 ), G4ThreeVector( 0, 0, 1 ), G4ThreeVector( 1, 0, 0 ),
				              10*mm, 5*mm
                    );
  G4cout << bcyl << G4endl;
  
  G4cout << "Testing BREP Open PolyCone solid:" << G4endl;
  
  G4double zVals[4] = {  -12000*mm, -11500*mm, -11500*mm, -11000*mm  }; 
  G4double rMins[4] = {     100*mm,      0*mm,      0*mm,    100*mm  };
  G4double rMaxs[4] = {     100*mm,   2000*mm,   1000*mm,    100*mm  };

  G4BREPSolidOpenPCone bopc( "BREPS OpenPCone",
                             0.0*rad, twopi*rad,
                             4, zVals[0], zVals,
                             rMins, rMaxs
                           );
  
  G4cout << bopc << G4endl;
  
  G4cout << "Testing BREP PolyCone solid:" << G4endl;
  
  G4BREPSolidPCone bpc( "BREP PolyCone",
                        0.0*rad, twopi*rad,
                        4, zVals[0], zVals,
                        rMins, rMaxs
                      );
  
  G4cout << bpc << G4endl;
  
  G4cout << "Testing BREP Polyhedra solid:" << G4endl;
  
#define VECSIZE 3
  G4int Sides = 4;
  G4double RMINVec[VECSIZE]  = {    0,    0,    0 };
  G4double RMAXVec[VECSIZE]  = {  700,    0,  700 };
  G4double Z_Values[VECSIZE] = { 3500, 4000, 4500 };

  G4BREPSolidPolyhedra bph( "BREP Polyhedra",
                            pi/2, 2*pi, Sides, VECSIZE, 3500,
                            Z_Values, RMINVec, RMAXVec
                          ); 
  
  G4cout << bph << G4endl;
 
  G4cout << "Testing BREP Sphere solid:" << G4endl;
  G4BREPSolidSphere bsph("BREP Sphere",
                           G4ThreeVector( 0, 0, 0 ), G4ThreeVector( 1, 0, 0 ), G4ThreeVector( 0, 0, 1 ), 10*mm
                          );
  G4cout << bsph << G4endl;
  
  G4cout << "Testing BREP Torus solid:" << G4endl;
  G4BREPSolidTorus btor("BREP Torus",
                           G4ThreeVector( 0, 0, 0 ), G4ThreeVector( 1, 0, 0 ), G4ThreeVector( 0, 0, 1 ), 5*mm, 10*mm
                          );
  G4cout << btor << G4endl;
  
  return 0;
}

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
//////////////////////////////////////////////////////////////////////////
// $Id: G4BREPSolidCylinderTest.cc,v 1.11 2006-06-29 18:43:11 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//////////////////////////////////////////////////////////////////////////
//
//
// BREP solid test, create by L. Broglia, 20/10/98
// modification of old G4Gerep test
//

#include "G4Timer.hh"
#include <cmath>
#include <fstream>
#include "G4ios.hh" 
#include "G4BREPSolid.hh"
#include "G4BREPSolidCylinder.hh"


int main()
{
  
  G4cout << "\n ============   Cylinder test   ================"; 
  
  G4BREPSolidCylinder *myCyl= new G4BREPSolidCylinder("MyCylBox",
						      G4ThreeVector(0,0,0),
						      G4ThreeVector(0,0,1),
						      G4ThreeVector(1,0,0),
						      100.0               ,
						      1000.0               );
  
  G4cout << "\n\nCylinder created ! ";
  
  // -> Check methods :
  //  - Inside
  //  - DistanceToIn
  //  - DistanceToOut

  G4ThreeVector Pt[4];
  Pt[0] = G4ThreeVector(    1,    1,  100);
  Pt[1] = G4ThreeVector( 1000,    0, 5000);
  Pt[2] = G4ThreeVector(  -50,  -50,-5000);
  Pt[3] = G4ThreeVector(    0,    0,  200);

  G4ThreeVector Dir[4];
  Dir[0] = G4ThreeVector(    1,    0,    0);
  Dir[1] = G4ThreeVector(    0,    1,    0);
  Dir[2] = G4ThreeVector(    0,    0,    1);
  Dir[3] = G4ThreeVector(    1,    1,   -1);

  EInside in[4];
  G4double dist[4][3];

  G4cout<<"\n\n";

  for (G4int a = 0; a<4; a++)
  {
    in[a] = myCyl->Inside(Pt[a]);

    G4cout<<"----------------------------\n\n";

    G4cout<<"x="<<Pt[a].x()
	  <<"  y="<<Pt[a].y()<<"  z="<<Pt[a].z();
      
    if( in[a] == kInside )
    {
      G4cout <<" is inside"<<G4endl;

      dist[a][1] = myCyl->DistanceToOut(Pt[a]);
      G4cout<<"\nDistance to out is :"<<dist[a][1]<<G4endl;

      G4cout << "\nDir   : x=" << Dir[a].x() 
	   << " y=" << Dir[a].y() 
	   << " z=" << Dir[a].z()<<G4endl;
      dist[a][2] = myCyl->DistanceToOut(Pt[a], Dir[a]);
      G4cout<<"Distance to out is :"<<dist[a][2]<<G4endl;

    }
    else
    {
      G4cout <<" is outside"<<G4endl;

      dist[a][1] = myCyl->DistanceToIn(Pt[a]);
      G4cout<<"\nDistance to in is :"<<dist[a][1]<<G4endl;

      G4cout << "\nDir   : x=" << Dir[a].x() 
	   << " y=" << Dir[a].y() 
	   << " z=" << Dir[a].z()<<G4endl;
      dist[a][2] = myCyl->DistanceToIn(Pt[a], Dir[a]);
      G4cout<<"Distance to in is :"<<dist[a][2];
    }
    G4cout<<G4endl;
  }



  return EXIT_SUCCESS;
}

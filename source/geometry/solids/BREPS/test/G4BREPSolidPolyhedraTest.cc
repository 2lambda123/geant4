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
// $Id: G4BREPSolidPolyhedraTest.cc,v 1.12 2006-06-29 18:43:17 gunter Exp $
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
#include "G4PhysicalConstants.hh"
#include "G4ios.hh" 
#include "G4Axis2Placement3D.hh"
#include "G4BREPSolid.hh"
#include "G4BREPSolidPolyhedra.hh"


int main()
{
  
  G4double RMINVec[5];
  RMINVec[0] = 10;
  RMINVec[1] = 10;
  RMINVec[2] = 60;
  RMINVec[3] = 30;
  RMINVec[4] = 30;

  G4double RMAXVec[5];
  RMAXVec[0] = 50;
  RMAXVec[1] = 50;
  RMAXVec[2] = 100;
  RMAXVec[3] = 100; 
  RMAXVec[4] = 80;
   
  G4double Z_Values[5];
  Z_Values[0] = 0;
  Z_Values[1] = 10;  
  Z_Values[2] = 20;
  Z_Values[3] = 30;
  Z_Values[4] = 40;


  G4cout << "\n=======     PolyGon test      ========"<<G4endl;

  G4BREPSolidPolyhedra *MyPGone = new G4BREPSolidPolyhedra ("MyPolyhedra",
							    0            ,
							    2*pi         ,
							    4            ,
							    5            ,
							    0            ,
							    Z_Values     ,
							    RMINVec      ,
							    RMAXVec       );
  G4cout << "\n\nPgon (G4BREPSolid-Polyhedra) created ! "<<G4endl;
  // -> Check methods :
  //  - Inside
  //  - DistanceToIn
  //  - DistanceToOut

  
  EInside in;
  
  G4cout<<"\n\n==================================================";
  G4ThreeVector  pt(0, -110, 20);
  for (G4int y = -110; y<=110; y+=10)
  {
    pt.setY(y);
    in = MyPGone->Inside(pt);
    
    G4cout << "\nx=" << pt.x() << "  y=" << pt.y() << "  z=" << pt.z();
    
    if( in == kInside )
      G4cout <<" is inside";
    else
      if( in == kOutside )
	G4cout <<" is outside";
      else
	G4cout <<" is on the surface";
  }

  G4cout<<"\n\n==================================================";
  G4ThreeVector  start( 0, 0, -5);
  G4ThreeVector dir1(1, 0, 0);
  G4ThreeVector dir2(1, 1, 0);
  G4double   d1, d2;
  G4double   z;
  
  G4cout<<"\nPdep is (0, 0, z)";
  G4cout<<"\nDir1 is (1, 0, 0)\n";
  G4cout<<"\nDir2 is (1, 1, 0)\n";

  for(z=-5; z<=45; z+=5)
  {
    start.setZ(z);

    in = MyPGone->Inside(start);
    G4cout<< "x=" << start.x() << "  y=" << start.y() << "  z=" << start.z();
    
    if( in == kInside )
    {
      G4cout <<" is inside";

      d1 = MyPGone->DistanceToOut(start, dir1);
      G4cout<<"  distance to out1 ="<<d1;
      d2 = MyPGone->DistanceToOut(start, dir2);
      G4cout<<"  distance to out2 ="<<d2<<G4endl;
    }
    else if( in == kOutside )
    {
      G4cout <<" is outside";

      d1 = MyPGone->DistanceToIn(start, dir1);
      G4cout<<"  distance to in1 ="<<d1;
      d2 = MyPGone->DistanceToIn(start, dir2);
      G4cout<<"  distance to in2 ="<<d2<<G4endl;
    }
    else
      G4cout <<" is on the surface"<< G4endl;
  }
 
  G4cout<<"\n\n==================================================";
  G4ThreeVector start3( -110, -110, -5);
  G4ThreeVector dir3( 1, 1, 0);
  G4double   d3;
  
  G4cout<<"\nPdep is (-110, -110, z)";
  G4cout<<"\nDir is (1, 1, 0)\n";

  for(z=-5; z<=45; z+=5)
  {
    start3.setZ(z);

    in = MyPGone->Inside(start3);
    G4cout<<"x=" << start3.x() << "  y=" << start3.y() << "  z=" << start3.z();
    
    if( in == kInside )
    {
      G4cout <<" is inside";

      d3 = MyPGone->DistanceToOut(start3, dir3);
      G4cout<<"  distance to out ="<<d3<<G4endl;
    }
    else if( in == kOutside )
    {
      G4cout <<" is outside";

      d3= MyPGone->DistanceToIn(start3, dir3);
      G4cout<<"  distance to in ="<<d3<<G4endl;
    }
    else
      G4cout <<" is on the surface"<< G4endl;
  }

  G4cout << G4endl << G4endl;
}


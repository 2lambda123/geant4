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
//////////////////////////////////////////////////////////////////////////
// $Id: testG4BREPSolidOpenPCone.cc,v 1.6 2004-12-02 09:31:27 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//////////////////////////////////////////////////////////////////////////
//
//
// BREP solid test, create by L. Broglia, 20/10/98
// modification of old G4Gerep test
//


#include <cmath>
#include <fstream>
#include "G4ios.hh" 
#include "G4BREPSolid.hh"
#include "G4BREPSolidOpenPCone.hh"
#include "G4Timer.hh"

#include <iomanip>

G4int main(G4int argc, char **argv)
{
  G4Timer timer;
  const G4int noZplanes= 8; 
  
  double RMINVec[noZplanes];
  RMINVec[0] = 30;
  RMINVec[1] = 30;
  RMINVec[2] =  0;
  RMINVec[3] =  0;
  RMINVec[4] =  0;  
  RMINVec[5] =  0;
  RMINVec[6] = 40;
  RMINVec[7] = 40;  

  double RMAXVec[noZplanes];
  RMAXVec[0] = 70;
  RMAXVec[1] = 70;
  RMAXVec[2] = 70;
  RMAXVec[3] = 40;
  RMAXVec[4] = 40;
  RMAXVec[5] = 80;
  RMAXVec[6] = 80;
  RMAXVec[7] = 60; 

  double Z_Values[noZplanes];
  Z_Values[0] =-20;
  Z_Values[1] =-10;
  Z_Values[2] =-10;
  Z_Values[3] =  0;
  Z_Values[4] = 10;
  Z_Values[5] = 20;
  Z_Values[6] = 30;
  Z_Values[7] = 40;

  G4double start_angle= 0.0;
  G4double opening_angle=  pi;

  G4double zstart= Z_Values[0]; 

  G4cout << "\n=======     PCon test      ========";

  G4BREPSolidOpenPCone *MyPCone = new G4BREPSolidOpenPCone ("MyPCone",
						    start_angle,
						    opening_angle,
						    noZplanes,
						    zstart,
						    Z_Values,
						    RMINVec,
						    RMAXVec );
  
  G4cout << "\n\nOpen PCone created ! "<<G4endl;
  G4cout << "Variety is G4BREPSolidOpenPolycone"<<G4endl;

  G4cout << "Its parameters are: "<<G4endl;

  ///////////////////////////////////////////////////
  // Temporary
  for (G4int x = 0; x < noZplanes; x++)
  {
    G4cout <<    " Z[" << x << "]=" << std::setw(5) << Z_Values[x];
    G4cout << " Rmin[" << x << "]=" << std::setw(5) << RMINVec[x];
    G4cout << " Rmax[" << x << "]=" << std::setw(5) << RMAXVec[x]<<G4endl;
  }

  G4cout<<" start   angle ="<<start_angle<<G4endl;
  G4cout<<" opening angle ="<<opening_angle<<G4endl;
  G4cout<<" zstart =" << zstart << G4endl;


  // -> Check methods :
  //  - Inside
  //  - DistanceToIn
  //  - DistanceToOut

  
  EInside in;
  
  G4cout<<"\n\n==================================================";
  G4Point3D  pt(0, -100, 24);
  G4double y; 
  for (y = -100; y<=100; y+=10)
  {
    pt.setY(y);
    in = MyPCone->Inside(pt);
    
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
  G4Point3D  start( 0, 0, -30);
  G4Vector3D dir(1, 1, 0);
  G4double   d;
  
  G4cout<<"\nPdep is (0, 0, z)";
  G4cout<<"\nDir is (1, 1, 0)\n";

  G4double z;
  for(z=-30; z<=50; z+=5)
  {
    start.setZ(z);

    in = MyPCone->Inside(start);
    G4cout<< "x=" << start.x() << "  y=" << start.y() << "  z=" << start.z();
    
    if( in == kInside )
    {
      G4cout <<" is inside";

      d = MyPCone->DistanceToOut(start, dir);
      G4cout<<"  distance to out="<<d;
      d = MyPCone->DistanceToOut(start);
      G4cout<<"  closest distance to out="<<d<<G4endl;
    }
    else if( in == kOutside ) 
    {
      G4cout <<" is outside";

      d = MyPCone->DistanceToIn(start, dir);
      G4cout<<"  distance to in="<<d;
      d = MyPCone->DistanceToIn(start);
      G4cout<<"  closest distance to in="<<d<<G4endl;
    }
    else
      G4cout <<" is on the surface"<<G4endl;

  }

  G4cout<<"\n\n==================================================";
  G4Point3D  start2( 0, -100, -30);
  G4Vector3D dir2(0, 1, 0);
  G4double   d2;

  G4cout<<"\nPdep is (0, -100, z)";
  G4cout<<"\nDir is (0, 1, 0)\n";

  for(z=-30; z<=50; z+=5)
  {
    G4cout<<"  z="<<z;
    start2.setZ(z);
    d2 = MyPCone->DistanceToIn(start2, dir2);
    G4cout<<"  distance to in="<<d2;
    d2 = MyPCone->DistanceToIn(start2);
    G4cout<<"  closest distance to in="<<d2<<G4endl;
  }

  G4cout<<"\n\n==================================================";
  G4Point3D  start3( 0, 0, -50);
  G4Vector3D dir3(0, 0, 1);
  G4double   d3;

  G4cout<<"\nPdep is (0, y, -50)";
  G4cout<<"\nDir is (0, 0, 1)\n";

  for(y=-0; y<=90; y+=5)
  {
    G4cout<<"  y="<<y;
    start3.setY(y);
    d3 = MyPCone->DistanceToIn(start3, dir3);
    G4cout<<"  distance to in="<<d3<<G4endl;
  }
  
  
  return EXIT_SUCCESS;
}

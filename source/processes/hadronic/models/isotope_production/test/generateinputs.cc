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
#include <fstream>
#include <iostream>
#include <sstream>
#include "globals.hh"

main()
{
  G4String name = "isoinput";
  G4String theName;
  int z[5] = {29, 74, 82, 30, 14};
  int isos[5] = {2, 5, 4, 5, 3};
  int *a[5];
  int energy[11] = {1,10,20,30,40,50,60,70,80,90,99};
  for(int ii=0; ii<5; ii++)
  {
    a[ii] = new int[isos[ii]];
  }
  a[0][0] = 63;
  a[0][1] = 65;
  a[1][0] = 180;
  a[1][1] = 182;
  a[1][2] = 183;
  a[1][3] = 184;
  a[1][4] = 186;
  a[2][0] = 204;
  a[2][1] = 206;
  a[2][2] = 207;
  a[2][3] = 208;
  a[3][0] = 64;
  a[3][1] = 66;
  a[3][2] = 67;
  a[3][3] = 68;
  a[3][4] = 70;
  a[4][0] = 28;
  a[4][1] = 29;
  a[4][2] = 30;
  for(ii=0; ii<5; ii++)
  {
    for(int i=0; i<isos[ii]; i++)
    {
      for(int i3=0;i3<11;i3++)
      {
        std::ostringstream ost;
        ost << a[ii][i]<<"_"<<z[ii]<<"_";
        if(energy[i3]<2) ost<<0;
        ost<<energy[i3];
        G4String * biff = new G4String(ost.str());
        theName = name + *biff;
        cout << theName<<G4endl;
        std::ofstream outFile( theName, std::ios::out);
        outFile <<  a[ii][i]<<G4endl<<z[ii]<<G4endl<<1<<G4endl<<1000000<<G4endl
                <<1<<G4endl<<energy[i3]<<G4endl;
      }
    }
  }
}

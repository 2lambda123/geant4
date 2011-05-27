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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
#include "G4HEPlot.hh"
#include "globals.hh"

int main()
{
  G4int i,j;
  G4String aPIPFile = "PionPlus.plot";
  G4String aKPFile = "KaonPlus.plot";
  G4String aFitFile = "Fith23.plot";

  G4HEPlot* plot = new G4HEPlot[11];

  plot[ 1].Init(20, 0., 0.05);
  plot[ 2].Init(20, 0., 0.05);
  plot[ 3].Init(20, 0., 0.05);
  plot[ 4].Init(20, 0., 0.05);
  plot[ 5].Init(20, 0., 0.05);
  plot[ 6].Init(20, 0., 0.05);
  plot[ 7].Init(20, 0., 0.05);
  plot[ 8].Init(20, 0., 0.05);
  plot[ 9].Init(50, 0., 0.05);
  plot[10].Init(50, 0., 0.05);

  for(i=1; i<=2; i++)
    {
      for(j=1; j<=5; j++)
        {
          G4int ii = (j-1)*2 + i;      
          if(i==1) plot[ii].GetFromFile(j, aPIPFile);      
          if(i==2) plot[ii].GetFromFile(j, aKPFile);
        }
    }
  for(i=1; i<=10; i++)
    {
      plot[i].DumpToFile(i, aFitFile);
      plot[i].Print("hist",100+i);
    }
  delete plot;
}



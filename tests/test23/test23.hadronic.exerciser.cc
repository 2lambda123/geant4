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
// $Id: test23.hadronic.exerciser.cc,v 1.2 2004-03-18 11:02:24 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "globals.hh"
#include "G4UnitsTable.hh"
#include <vector>

static void OutputCases
(G4int N,  // Number of events per case.
 const std::vector <G4String> & particleNameList,
 const std::vector <G4double> & energyList,
 const std::vector <G4String> & materialNameList) {

  for (size_t iMaterial = 0;
       iMaterial < materialNameList.size ();
       iMaterial++) {
    for (size_t iEnergy = 0;
	 iEnergy < energyList.size ();
	 iEnergy++) {
      for (size_t iParticle = 0;
	   iParticle < particleNameList.size ();
	   iParticle++) {

	G4cout
	  << "\n#\n# " << particleNameList [iParticle]
	  << " at " << G4BestUnit (energyList [iEnergy], "Energy")
	  << " in " << materialNameList [iMaterial]
	  << "\n#";

	G4cout
	  << "\n/gun/particle " << particleNameList [iParticle]
	  << "\n/gun/energy " <<  G4BestUnit (energyList [iEnergy], "Energy")
	  << "\n/mydet/SelectMaterial " << materialNameList [iMaterial]
	  << "\n/run/beamOn " << N;

      }
    }
  }
}


int main (int argc, char** argv) {


  std::vector <G4String> materialNameList;
  //materialNameList.push_back ("Pb");
  //materialNameList.push_back ("Al");
  //materialNameList.push_back ("Air");

  G4int N = 1000;
  if (argc > 1)
  {
    if (strcmp (argv[1], "large_N") == 0)
    {
      N = 100000;
      materialNameList.push_back ("Pb");
    }
    else materialNameList.push_back ("Al");
  }
  else materialNameList.push_back ("Al");

  G4UnitDefinition::BuildUnitsTable();

  G4cout <<
    "#"
    "\n# Auto-generated test input file for test23 of CHIPS Capture."
    "\n#"
    "\n/control/verbose 2"
    "\n# /run/verbose 2"
    "\n/run/particle/setCut 1 m"
    "\n/run/initialize"
    "\n/gun/direction 1 0 0";
  
  std::vector <G4String> particleNameList;
  particleNameList.push_back ("mu-");
  //particleNameList.push_back ("pi-");
  //particleNameList.push_back ("kaon-");
  //particleNameList.push_back ("anti_proton");
  //particleNameList.push_back ("sigma-");
  //particleNameList.push_back ("xi-");
  //particleNameList.push_back ("omega-");
  //particleNameList.push_back ("anti_sigma+");

  std::vector <G4double> energyList;
  energyList.push_back (1. * MeV);

  OutputCases (N, particleNameList, energyList, materialNameList);

  //particleNameList.clear ();
  //particleNameList.push_back ("proton");
  //particleNameList.push_back ("neutron");

  //energyList.clear ();
  //energyList.push_back (500 * MeV);

  //materialNameList.clear ();
  //materialNameList.push_back ("Pb");
  //materialNameList.push_back ("Al");
  //materialNameList.push_back ("Air");

  //OutputCases (N, particleNameList, energyList, materialNameList);

  //particleNameList.clear ();
  //particleNameList.push_back ("neutron");
  //particleNameList.push_back ("triton");
  //particleNameList.push_back ("deuteron");
  //particleNameList.push_back ("alpha");

  //energyList.clear ();
  //energyList.push_back (100 * MeV);

  //materialNameList.clear ();
  //materialNameList.push_back ("Pb");
  //materialNameList.push_back ("Al");
  //materialNameList.push_back ("Air");

  //OutputCases (N, particleNameList, energyList, materialNameList);

  G4cout << G4endl;
}

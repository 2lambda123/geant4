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
// $Id: paraMaker.cc,v 1.15 2010-05-17 23:49:43 mkelsey Exp $
// Geant4 tag: $Name: not supported by cvs2svn $
//
// 20100412  M. Kelsey -- Modify paraMaker[Truncated] to take buffer as argument
// 20100517  M. Kelsey -- BUG FIX:  Must check for array boundary "if (Z>=70)"

#include "G4InuclSpecialFunctions.hh"

void 
G4InuclSpecialFunctions::paraMaker(G4double Z,
	   std::pair<std::vector<G4double>, std::vector<G4double> >& parms) {
  G4int verboseLevel(0);

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::paraMaker" << G4endl;
  }

  // calculates the coefficients for the phenomenological formulas for
  // coulumb barier, c.s. etc needed for evaporators

  const G4double Z1[5] = {10.0, 20.0, 30.0, 50.0, 70.0};
  const G4double AP[5] = {0.42, 0.58, 0.68, 0.77, 0.80};
  const G4double CP[5] = {0.50, 0.28, 0.20, 0.15, 0.10};
  const G4double AA[5] = {0.68, 0.82, 0.91, 0.97, 0.98};
  const G4double CA[5] = {0.10, 0.10, 0.10, 0.08, 0.06};

  // Set up input buffer for results
  std::vector<G4double>& AK = parms.first; 
  AK.resize(6); AK.clear();

  std::vector<G4double>& CPA = parms.second;
  CPA.resize(6); CPA.clear();

  AK[0] = 0.0;
  CPA[0] = 0.0;
  G4double AK2 = 0.0;
  G4double CP2 = 0.0;
  G4double AK6 = 0.0; 
  G4double CP6 = 0.0;

  if (Z < 10.0) {
    AK2=0.42;
    CP2=0.5;
    AK6=0.68;
    CP6=0.1;

  } else if(Z >= 70.0) {
    AK6=0.98;  // normal
    CP6=0.06;
    AK2=0.8;
    CP2=0.1;
    // AK6=1.1; // modified
    // CP6=0.0;

  } else {

    for (G4int i = 1; i < 5; i++) {

      if (Z <= Z1[i]) {
	G4double Z2 = 1.0 / (Z1[i] - Z1[i - 1]);
	AK2 = ((AP[i] - AP[i - 1]) * Z + AP[i - 1] * Z1[i] - AP[i] * Z1[i - 1]) * Z2;
	CP2 = ((CP[i] - CP[i - 1]) * Z + CP[i - 1] * Z1[i] - CP[i] * Z1[i - 1]) * Z2;
	AK6 = ((AA[i] - AA[i - 1]) * Z + AA[i - 1] * Z1[i] - AA[i] * Z1[i - 1]) * Z2;
	CP6 = ((CA[i] - CA[i - 1]) * Z + CA[i - 1] * Z1[i] - CA[i] * Z1[i - 1]) * Z2;

	break;
      };
    };
  };

  AK[1] = AK2;
  AK[5] = AK6;
  CPA[1] = CP2;
  CPA[5] = CP6;
  AK[2] = AK2 + 0.06;
  CPA[2] = CP2 * 0.5;
  AK[3] = AK2 + 0.12;
  CPA[3] = CP2 / 3.0;  
  AK[4] = AK6 - 0.06;
  CPA[4] = 4.0 * CP6 / 3.0;

  return;	// Buffer filled
}

void 
G4InuclSpecialFunctions::paraMakerTruncated(G4double Z,
				    std::pair<G4double,G4double>& parms) {
  G4int verboseLevel(0);

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::paraMakerTruncated" << G4endl;
  }

  // truncated version of the previous one
  const G4double Z1[5] = {10.0, 20.0, 30.0, 50.0, 70.0};
  const G4double AP[5] = {0.42, 0.58, 0.68, 0.77, 0.8};
  const G4double CP[5] = {0.5, 0.28, 0.2, 0.15, 0.1};

  // Set up buffers for output
  G4double& AK2=parms.first; AK2=0.;
  G4double& CP2=parms.second; CP2=0.;

  if (Z < 10.0) {
    AK2=0.42;
    CP2=0.5;

  } else if (Z >= 70.0) {
    AK2=0.8;
    CP2=0.1;

  } else {

    for (G4int i = 1; i < 5; i++) {

      if (Z < Z1[i]) {
	G4double Z2 = 1.0 / (Z1[i] - Z1[i - 1]);
	AK2 = ((AP[i] - AP[i - 1]) * Z + AP[i - 1] * Z1[i] - AP[i] * Z1[i - 1]) * Z2;
	CP2 = ((CP[i] - CP[i - 1]) * Z + CP[i - 1] * Z1[i] - CP[i] * Z1[i - 1]) * Z2;

	break;
      };
    };
  };

  return;	// Buffer filled
}

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
#include "G4InuclSpecialFunctions.hh"
#include "Randomize.hh"

G4double G4InuclSpecialFunctions::getAL(G4double A) {
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::getAL" << G4endl;
  }

  return 0.76 + 2.2 / std::pow(A, 0.333333);
}

G4double G4InuclSpecialFunctions::csNN(G4double e) {
  G4int verboseLevel = 2;
 
  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::csNN" << G4endl;
  }

  G4double snn;

  if (e < 40.0) {
    snn = -1174.8 / (e * e) + 3088.5 / e + 5.3107;

  } else {
    snn = 93074.0 / (e * e) - 11.148 / e + 22.429;
  };

  return snn; 
}

G4double G4InuclSpecialFunctions::csPN(G4double e) {
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::csPN" << G4endl;
  }

  G4double spn;

  if (e < 40.0) {
    spn = -5057.4 / (e * e) + 9069.2 / e + 6.9466;

  } else {
    spn = 239380.0 / (e * e) + 1802.0 / e + 27.147;
  };

  return spn; 
}

G4double G4InuclSpecialFunctions::FermiEnergy(G4double A, G4double Z, G4int ntype) {
  // calculates the nuclei Fermi energy for 0 - neutron and 1 - proton
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::FermiEnergy" << G4endl;
  }

  const G4double C = 55.4;
  G4double Ef;

  if (ntype == 0) {
    Ef = C * std::pow((A - Z) / A, 0.666667);

  } else {
    Ef = C * std::pow(Z / A, 0.666667);
  };

  return Ef; 
}

G4double G4InuclSpecialFunctions::inuclRndm() { 
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::inuclRndm" << G4endl;
  }

  G4double rnd = G4UniformRand(); 

  return rnd;
} 

G4double G4InuclSpecialFunctions::randomGauss(G4double sigma) {
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::randomGauss" << G4endl;
  }

  const G4double eps = 1.0e-6;
  const G4double twopi = 6.2831854;
  G4double r1 = inuclRndm();
  r1 = r1 > eps ? r1 : eps;
  G4double r2 = inuclRndm();
  r2 = r2 > eps ? r2 : eps;
  r2 = r2 < 1.0 - eps ? r2 : 1.0 - eps; 

  return sigma * std::sin(twopi * r1) * std::sqrt(-2.0 * std::log(r2)); 
} 

G4double G4InuclSpecialFunctions::randomPHI() { 
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::randomPHI" << G4endl;
  }

  const G4double twopi = 6.2831853;

  return twopi * inuclRndm();
} 

std::pair<G4double, G4double> G4InuclSpecialFunctions::randomCOS_SIN() {
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::randomCOS_SIN" << G4endl;
  }

  G4double CT = 1.0 - 2.0 * inuclRndm();

  return std::pair<G4double, G4double>(CT, std::sqrt(1.0 - CT * CT));
}

G4CascadeMomentum G4InuclSpecialFunctions::generateWithFixedTheta(G4double ct,
									G4double p) {
  G4int verboseLevel = 2;

  if (verboseLevel > 3) {
    G4cout << " >>> G4InuclSpecialFunctions::generateWithFixedTheta" << G4endl;
  }

  G4CascadeMomentum momr;
  G4double phi = randomPHI();
  G4double pt = p * std::sqrt(std::fabs(1.0 - ct * ct));
  G4CascadeMomentum mom1;
  momr[1] = pt * std::cos(phi);
  momr[2] = pt * std::sin(phi);
  momr[3] = p * ct;

  return momr;
}

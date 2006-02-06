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
// $Id: G4QNuMuNuclearCrossSection.hh,v 1.4 2006-02-06 09:35:57 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// GEANT4 physics class: G4QNuMuNuclearCrossSection -- header file
// M.V. Kossov, CERN-ITEP(Moscow), 20-DEC-2005
// The last update: M.V. Kossov, CERN/ITEP (Moscow) 20-DEC-2005
//
// Short description: this G4 singletone class calculates (nu_mu,mu) Nuclear cross section
// (Energy limit: E<320GeV->badExtrapolation) for a particular isotope (proportional to A)
// ****************************************************************************************

#ifndef G4QNuMuNuclearCrossSection_h
#define G4QNuMuNuclearCrossSection_h 1

#include "G4ParticleTable.hh"
#include "G4NucleiProperties.hh"
#include "G4NucleiPropertiesTable.hh"
#include <vector>
#include "Randomize.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4VQCrossSection.hh"

class G4QNuMuNuclearCrossSection : public G4VQCrossSection
{
protected:

  G4QNuMuNuclearCrossSection()  {};

public:

  ~G4QNuMuNuclearCrossSection()  {};

  static G4VQCrossSection* GetPointer(); // Gives a pointer to this singletone

  G4double ThresholdEnergy(G4int Z, G4int N, G4int PDG=14);

  G4double CalculateCrossSection(G4bool CS, G4int F, G4int I, G4int PDG, G4int Z,
                                                               G4int N, G4double Momentum);

  G4int    GetExchangePDGCode();

  G4double GetDirectPart(G4double Q2);

  G4double GetNPartons(G4double Q2);

  G4double GetQEL_ExchangeQ2();

  G4double GetNQE_ExchangeQ2();

  // Get static members
  G4double GetLastTOTCS() {return lastSig;}
  G4double GetLastQELCS() {return lastQEL;}

private:
  G4int    GetFunctions(G4int z, G4int n, G4double* t, G4double* q, G4double* e);
  G4double HighEnergyTX(G4double lE);
  G4double HighEnergyQE(G4double lE);

// Body
private:
  static G4bool    onlyCS;   // flag to calculate only CS (not TX & QE)
  static G4double  lastSig;  // Last calculated total cross section
  static G4double  lastQEL;  // Last calculated quasi-elastic cross section
  static G4int     lastL;    // Last bin used in the cross section TheLastBin
  static G4double  lastE;    // Last energy used in the cross section Energy
  static G4double* lastEN;   // Pointer to the last array of the energy axis
  static G4double* lastTX;   // Pointer to the last array of the total CS function
  static G4double* lastQE;   // Pointer to the last array of the QE CS function
};

#endif

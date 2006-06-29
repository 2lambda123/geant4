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
//
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// GEANT4 physics class: G4QElasticCrossSection -- header file
// M.V. Kossov, ITEP(Moscow), 24-OCT-01
// The last update: M.V. Kossov, CERN/ITEP (Moscow) 25-Sept-03
//

#ifndef G4QElasticCrossSection_h
#define G4QElasticCrossSection_h 1

#include "G4VCrossSectionDataSet.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4ParticleTable.hh"
#include "G4NucleiProperties.hh"
#include "G4NucleiPropertiesTable.hh"
#include "G4QPDGCode.hh"
#include "G4QException.hh"
#include <vector>
#include "Randomize.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4VQCrossSection.hh"

class G4QElasticCrossSection : public G4VQCrossSection
{
protected:

  G4QElasticCrossSection()  {}               // Constructor

public:

  ~G4QElasticCrossSection() {}

  static G4VQCrossSection* GetPointer(); // Gives a pointer to this singletone

  G4double CalculateCrossSection(G4bool CS, G4int F, G4int I, G4int pPDG, G4int Z, G4int N,
                                                                              G4double pP);

  G4double GetExchangeT(G4int tZ, G4int tN, G4int pPDG); // Randomizes -t=Q2

private:
  G4double GetPTables(G4double lpP, G4double lPm, G4int PDG, G4int tZ, G4int tN); // newLP
  G4double GetTabValues(G4double lp, G4int pPDG, G4int tgZ, G4int tgN); // return CS(Si/Bi)
  G4double GetQ2max(G4int pPDG, G4int tgZ, G4int tgN, G4double pP); // return -t=Q2

// Body
private:
  // --- Data formating AMDB (define the precalculated table structure) ---
  static const G4int nPoints;// #of points in the AMDB tables     
  static const G4int nLast;  // the Last element in the table
  static G4double    lPMin;  // Min tabulated logarithmic Momentum  
  static G4double    lPMax;  // Max tabulated logarithmic Momentum  
  static G4double    dlnP;   // Log step in the table     
  // ---- Local (for particular pP, pPDG, tZ, tN) -----
  static G4bool    onlyCS;   // flag to calculate only CS (not S1/B1,S2/B2,S3/B3)
  static G4double  lastSIG;  // Last calculated cross section
  static G4double  lastLP;   // Last log(mom_of_the_incident_hadron)
  static G4double  lastTM;   // Last t_maximum                       
  static G4double  theSS;    // The Last squared slope of first difruction 
  static G4double  theS1;    // The Last mantissa of first difruction 
  static G4double  theB1;    // The Last slope of first difruction    
  static G4double  theS2;    // The Last mantissa of second difruction
  static G4double  theB2;    // The Last slope of second difruction   
  static G4double  theS3;    // The Last mantissa of third difruction 
  static G4double  theB3;    // The Last slope of third difruction    
  // ---- Global (AMBD of P-dependent tables for pPDG,tZ,tN) -----
  static G4int     lastPDG;  // Last PDG code of the projectile
  static G4int     lastTZ;   // Last atomic number of the target
  static G4int     lastTN;   // Last number of neutrons of the target
  static G4double  lastPIN;  // Last initialized max momentum
  static G4double* lastCST;  // Last cross-section table
  static G4double* lastPAR;  // Last parameters for functional calculation
  static G4double* lastSST;  // E-dep of squared slope of the first difruction	
  static G4double* lastS1T;  // E-dep of mantissa of the first difruction	
  static G4double* lastB1T;  // E-dep of the slope of the first difruction
  static G4double* lastS2T;  // E-dep of mantissa of the second difruction
  static G4double* lastB2T;  // E-dep of the slope of theSecond difruction
  static G4double* lastS3T;  // E-dep of mantissa of the third difruction	
  static G4double* lastB3T;  // E-dep of the slope of the third difruction
 }; 					
#endif

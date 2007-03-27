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
// GEANT4 physics class: G4QuasiFreeRatios -- header file
// M.V. Kossov, ITEP(Moscow), 24-OCT-01
// The last update: M.V. Kossov, CERN/ITEP (Moscow) 15-Oct-2006
//

#ifndef G4QuasiFreeRatios_h
#define G4QuasiFreeRatios_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include <vector>
//#include "G4DynamicParticle.hh"
//#include "G4Element.hh"
//#include "G4ParticleTable.hh"
//#include "G4NucleiProperties.hh"
//#include "G4NucleiPropertiesTable.hh"
#include "G4QPDGCode.hh"
#include "G4QHadron.hh"
#include "G4VCrossSectionDataSet.hh"
#include "G4QElasticCrossSection.hh"


class G4QuasiFreeRatios
{
 protected:

  G4QuasiFreeRatios()  {}                 // Constructor

 public:

  ~G4QuasiFreeRatios() {}                 // Destructor

  static G4QuasiFreeRatios* GetPointer(); // Gives a pointer to this singletone

  // Pair(QuasiFree/Inelastic,QuasiElastic/QuasiFree)
  std::pair<G4double,G4double> GetRatios(G4double pIU, G4int prPDG, G4int tgZ, G4int tgN);
  // scatter (pPDG,p4M) on a virtual nucleon (NPDG,N4M), result: final pair(newN4M,newp4M)
  // if(newN4M.e()==0.) - below threshold, XS=0, no scattering of the progectile happened
  std::pair<G4LorentzVector,G4LorentzVector> Scatter(G4int NPDG, G4LorentzVector N4M,
                                                     G4int pPDG, G4LorentzVector p4M);

 private:
  // These working member functions are in CHIPS units and must not be used externally
  G4double GetQF2IN_Ratio(G4double TotCS_mb, G4int A); // QuasiFree/Inelastic (fast)
  std::pair<G4double,G4double> GetElTot(G4double pGeV,G4int PDG,G4int Z,G4int N);//El,To,mb
  G4double CalcQF2IN_Ratio(G4double TCSmb, G4int A); // R=QuasuFree/Inelastic (sig_t in mb)
  std::pair<G4double,G4double> FetchElTot(G4double pGeV,G4int PDG,G4bool F);//(E,T)fromAMDB
  std::pair<G4double,G4double> CalcElTot(G4double pGeV,G4int Index);//(sigE,sigT)(Index)

 // Body
 private:
}; 					
#endif

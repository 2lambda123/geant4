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
// $Id: G4TauMinus.cc,v 1.10 2004-02-13 05:53:01 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation, based on object model of
//      4th April 1996, G.Cosmo
//      7 July 1996                   H.Kurashige 
// **********************************************************************

#include <fstream>
#include <iomanip>

#include "G4TauMinus.hh"

#include "G4PhaseSpaceDecayChannel.hh"
#include "G4TauLeptonicDecayChannel.hh"
#include "G4DecayTable.hh"

// ######################################################################
// ###                          TAUMINUS                             ###
// ######################################################################

G4TauMinus::G4TauMinus(
       const G4String&     aName,        G4double            mass,
       G4double            width,        G4double            charge,   
       G4int               iSpin,        G4int               iParity,    
       G4int               iConjugation, G4int               iIsospin,   
       G4int               iIsospin3,    G4int               gParity,
       const G4String&     pType,        G4int               lepton,      
       G4int               baryon,       G4int               encoding,
       G4bool              stable,       G4double            lifetime,
       G4DecayTable        *decaytable )
 : G4VLepton( aName,mass,width,charge,iSpin,iParity,
              iConjugation,iIsospin,iIsospin3,gParity,pType,
              lepton,baryon,encoding,stable,lifetime,decaytable )
{
  SetParticleSubType("tau");
  SetPDGStable(false);

  //create Decay Table 
  G4DecayTable*   table = GetDecayTable();
  if (table!=NULL) delete table;
  table = new G4DecayTable();

  // create decay channels
  G4VDecayChannel** mode = new G4VDecayChannel*[6];
  // tau- -> mu- + anti_nu_mu + nu_tau
  mode[0] = new G4TauLeptonicDecayChannel("tau-",0.174,"mu-");
  // tau- -> e- + anti_nu_e + nu_tau
  mode[1] = new G4TauLeptonicDecayChannel("tau-",0.178,"e-");
  // tau- -> pi- + nu_tau
  mode[2] = new G4PhaseSpaceDecayChannel("tau-",0.113,2,"pi-","nu_tau");
  // tau- -> pi0 + pi0 + pi- + nu_tau
  mode[3] = new G4PhaseSpaceDecayChannel("tau-",0.252,3,"pi0","pi-","nu_tau");
  // tau- -> pi0 + pi0 + pi- + nu_tau
  mode[4] = new G4PhaseSpaceDecayChannel();
  mode[4]->SetParent("tau-");
  mode[4]->SetBR(0.093);
  mode[4]->SetNumberOfDaughters(4);
  mode[4]->SetDaughter(0,"pi0");
  mode[4]->SetDaughter(1,"pi0");
  mode[4]->SetDaughter(2,"pi-");
  mode[4]->SetDaughter(3,"nu_tau");
  // tau- -> pi- + pi- + pi+ + nu_tau
  mode[5] = new G4PhaseSpaceDecayChannel();
  mode[5]->SetParent("tau-");
  mode[5]->SetBR(0.098);
  mode[5]->SetNumberOfDaughters(4);
  mode[5]->SetDaughter(0,"pi-");
  mode[5]->SetDaughter(1,"pi-");
  mode[5]->SetDaughter(2,"pi+");
  mode[5]->SetDaughter(3,"nu_tau");

  for (G4int index=0; index <6; index++ ) table->Insert(mode[index]);  
  delete [] mode;
  
  SetDecayTable(table);
}

// ......................................................................
// ...                 static member definitions                      ...
// ......................................................................
//     
//    Arguments for constructor are as follows
//               name             mass          width         charge
//             2*spin           parity  C-conjugation
//          2*Isospin       2*Isospin3       G-parity
//               type    lepton number  baryon number   PDG encoding
//             stable         lifetime    decay table 

G4TauMinus G4TauMinus::theTauMinus(
		"tau-",    1.77705*GeV,  2.265e-9*MeV,    -1.*eplus, 
		    1,               0,             0,          
		    0,               0,             0,             
	     "lepton",               1,             0,          15,
		 true,     295.6e-6*ns,          NULL
);

G4TauMinus* G4TauMinus::TauMinusDefinition() {return &theTauMinus;}

G4TauMinus*  G4TauMinus::TauMinus()
{ 
  return &theTauMinus; 
}



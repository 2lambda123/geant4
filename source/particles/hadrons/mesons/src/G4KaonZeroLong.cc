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
// $Id: G4KaonZeroLong.cc,v 1.13 2003-06-16 16:57:45 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: first implementation, based on object model of
//      4th April 1996, G.Cosmo
//                              H.Kurashige   7 Jul 96
// **********************************************************************

#include <fstream>
#include <iomanip>

#include "G4KaonZeroLong.hh"

#include "G4PhaseSpaceDecayChannel.hh"
#include "G4KL3DecayChannel.hh"
#include "G4DecayTable.hh"
// ######################################################################
// ###                      KAONZEROLONG                              ###
// ######################################################################

G4KaonZeroLong::G4KaonZeroLong(
       const G4String&     aName,        G4double            mass,
       G4double            width,        G4double            charge,   
       G4int               iSpin,        G4int               iParity,    
       G4int               iConjugation, G4int               iIsospin,   
       G4int               iIsospin3,    G4int               gParity,
       const G4String&     pType,        G4int               lepton,      
       G4int               baryon,       G4int               encoding,
       G4bool              stable,       G4double            lifetime,
       G4DecayTable        *decaytable )
 : G4VMeson( aName,mass,width,charge,iSpin,iParity,
             iConjugation,iIsospin,iIsospin3,gParity,pType,
             lepton,baryon,encoding,stable,lifetime,decaytable )
{
  SetParticleSubType("kaon");
  // Anti-particle of K0Long is K0Long itself  
  SetAntiPDGEncoding(encoding);
  //create Decay Table 
  G4DecayTable*   table = GetDecayTable();
  if (table!=NULL) delete table;
  table = new G4DecayTable();

 // create decay channels
  G4VDecayChannel** mode = new G4VDecayChannel*[6];
  // kaon0L -> pi0 + pi0 + pi0
  mode[0] = new G4PhaseSpaceDecayChannel("kaon0L",0.211,3,"pi0","pi0","pi0");
  // kaon0L -> pi0 + pi+ + pi-
  mode[1] = new G4PhaseSpaceDecayChannel("kaon0L",0.126,3,"pi0","pi+","pi-");
  // kaon0L -> pi- + e+ + nu_e (Ke3) 
  mode[2] = new G4KL3DecayChannel("kaon0L",0.194,"pi-","e+","nu_e");
  // kaon0L -> pi+ + e- + anti_nu_e (Ke3) 
  mode[3] = new G4KL3DecayChannel("kaon0L",0.194,"pi+","e-","anti_nu_e");
  // kaon0L -> pi- + mu+ + nu_mu (Kmu3) 
  mode[4] = new G4KL3DecayChannel("kaon0L",0.136,"pi-","mu+","nu_mu");
  // kaon0L -> pi+ + mu- + anti_nu_mu (Kmu3) 
  mode[5] = new G4KL3DecayChannel("kaon0L",0.136,"pi+","mu-","anti_nu_mu");

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

G4KaonZeroLong G4KaonZeroLong::theKaonZeroLong(
	     "kaon0L",    0.497672*GeV,  1.273e-14*MeV,         0.0, 
		    0,              -1,             0,          
		    1,               0,             0,             
	      "meson",               0,             0,         130,
	 	false,         51.7*ns,          NULL
);

G4KaonZeroLong* G4KaonZeroLong::KaonZeroLongDefinition(){return &theKaonZeroLong;}
G4KaonZeroLong* G4KaonZeroLong::KaonZeroLong(){return &theKaonZeroLong;}


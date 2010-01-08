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
// $Id: G4Dineutron.cc,v 1.1 2010-01-08 23:19:41 mkelsey Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ------------------------------------------------------------
//      Bertini Cascade dineutron class implementation file
//
//      History: first implementation, inspired by G4Proton
//      17 Nov 2009:  Michael Kelsey
// ----------------------------------------------------------------

#include "G4Dineutron.hh"

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

// ######################################################################
// ###                          DINEUTRON                              ###
// ######################################################################
G4Dineutron* G4Dineutron::theInstance = 0;

G4Dineutron* G4Dineutron::Definition() {
  if (0 != theInstance) return theInstance;

  // search in particle table
  const G4String name = "dineutron";
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4Ions* anInstance =  reinterpret_cast<G4Ions*>(pTable->FindParticle(name));
  
  if (0 == anInstance) {			// create particle
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
  // use constants in CLHEP
  //  static const double   proton_mass_c2 = 938.27231 * MeV;

    G4double nnmass = 2.*neutron_mass_c2;
    anInstance = new G4Ions(name,   nnmass, 0.0*MeV, 0,
			       2,       +1,       0,          
        	               2,       -2,       0,             
		       "nucleus",        0,      +2, 0, /* ? 100000020 */
      	                    true,     -1.0,    NULL,
      	                   false, "static",       0,
			    0.0);
  }

  theInstance = reinterpret_cast<G4Dineutron*>(anInstance);
  return theInstance;
}

// Simple call-throughs
G4Dineutron* G4Dineutron::DineutronDefinition() { return Definition(); }
G4Dineutron* G4Dineutron::Dineutron()           { return Definition(); }

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
//
// $Id: G4PiMinusStopAl.cc,v 1.6 2001-08-01 17:12:35 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     G4PiMinusStopAl
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it)
// 
//      Creation date: 8 May 1998
//
//      Modifications: 
// -------------------------------------------------------------------

#include "G4ios.hh"

#include "G4PiMinusStopAl.hh"

#include "g4rw/tpordvec.h"
#include "g4rw/tvordvec.h"
#include "g4rw/cstring.h"

#include "globals.hh"
#include "Randomize.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4ParticleTypes.hh"
#include "G4ReactionKinematics.hh"
#include "G4DynamicParticleVector.hh"
#include "G4LorentzVector.hh"
#include "G4NucleiPropertiesTable.hh"
#include "G4PiMinusStopMaterial.hh"
#include "G4DistributionGenerator.hh"

// np/pp production ratio
// Experimental values: 
// R(np/pp) = average (E. Gadioli et al., Phys Rev C36 (1987) 741)
G4double G4PiMinusStopAl::npRatio = 4;
 
// Average numbers of final nucleons detected, for N-pair absorption
// (R. Madey  et al., Phys Rev C25 (1982) 3050
G4double G4PiMinusStopAl::nFinalNucleons = 1.67;

// Kinetic energy (MeV) distributions measured for coincident nucleon 
// emission
// (R. Madey  et al., Phys Rev C25 (1982) 3050

G4int G4PiMinusStopAl::eKinEntries = 10;

G4double G4PiMinusStopAl::eKinData[10] = { 0.23, 0.58, 1.24, 1.52,
                                     1.67, 
				     1.52, 1.24, 0.95, 0.58, 0.23};

G4double G4PiMinusStopAl::eKin[11] = { 5.2, 15., 27., 41.5,
                                  49.6,
				  57.7, 79.3, 94.4, 114., 125., 140.};


// Opening angle distributions measured for coincident nucleon emission
// (P.Heusi et al., Nucl. Phys. A407 (1983) 429

G4int G4PiMinusStopAl::angleEntries = 7;

G4double G4PiMinusStopAl::angleData[7] = 
{ 1.43, 1.67, 2.62, 4.29, 7.62, 11.90, 14.76 };

G4double G4PiMinusStopAl::angle[8] = { 1.308997, 1.570796, 1.832596, 2.094395, 
				  2.356194, 2.617994, 2.967060, 3.1415927 };



// Constructor

G4PiMinusStopAl::G4PiMinusStopAl()
  
{
  // Cluster size: nucleon pair, alpha, triton etc.
  // First implementation: interaction with nucleon pair only
  _clusterSize = 2;

  // R ratio
  theR = 1. / (1. + npRatio);

  _definitions = new G4RWTPtrOrderedVector<G4ParticleDefinition>();
  _momenta = new G4RWTPtrOrderedVector<G4LorentzVector>();

  G4RWTValOrderedVector<double> eKinVector;
  G4RWTValOrderedVector<double> eKinDataVector;
  int i;
  for (i=0; i<eKinEntries; i++)
    {
      eKinVector.insert(eKin[i]);
      eKinDataVector.insert(eKinData[i]);
    }
  eKinVector.insert(eKin[eKinEntries]);
  _distributionE = new G4DistributionGenerator(eKinVector,eKinDataVector);

  G4RWTValOrderedVector<double> angleVector;
  G4RWTValOrderedVector<double> angleDataVector;
  for (i=0; i<angleEntries; i++)
    {
      angleVector.insert(angle[i]);
      angleDataVector.insert(angleData[i]);
    }
  angleVector.insert(angle[angleEntries]);
  _distributionAngle = new G4DistributionGenerator(angleVector,angleDataVector);
}


// Destructor

G4PiMinusStopAl::~G4PiMinusStopAl()
{}

G4double G4PiMinusStopAl::FinalNucleons()
{
  return nFinalNucleons;
}


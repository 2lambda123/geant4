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
// $Id: G4PiMinusStopC.cc,v 1.8 2002-12-12 19:18:38 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     G4PiMinusStopC
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it)
// 
//      Creation date: 8 May 1998
//
//      Modifications: 
// -------------------------------------------------------------------

#include "G4ios.hh"

#include "G4PiMinusStopC.hh"

#include "g4std/vector"

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
// R(np/pp) = 6.3 +- 1.4 (P. Heusi et al. (Nucl. Phys. A407, 429 [1983])
// R = 5.0 +- 1.5 (Ozaki et al.)
// R = 8.8 +- 1.3 (Lee et al.)
// R = 2.4 +- 1.0 (Nordberg et al.)
// Use average value of the first three ones
G4double G4PiMinusStopC::npRatio = 6.7;
 
// Average numbers of final nucleons detected, for N-pair absorption
// R. Madey et al., Phys. Rev. C25(1982) 3050 
G4double G4PiMinusStopC::nFinalNucleons = 1.77;

// Kinetic energy (MeV) distributions measured for coincident nucleon 
// emission
// P. Heusi et al., Nucl. Phys. A407(1983) 429

G4int G4PiMinusStopC::eKinEntries = 21;

G4double G4PiMinusStopC::eKinData[21] = { 0.031, 0.045, 0.06, 0.09, 
				      0.11, 0.116, 0.14, 
				      0.18,  0.21,  0.25, 
				      0.3,  0.32,  0.31, 0.3, 
				      0.25,  0.18,  0.16, 
				      0.15, 0.08,  0.05, 0.01};

G4double G4PiMinusStopC::eKin[22] = {  8., 17.5,  21., 24.5,  
				  27.5,  31.,  33.5,  
				  37.5,  42.5,  46.4,
				  49.,  52.5,  57.5, 62.5,
				  67.5,  72.5,  75., 
				  77.5,  82.5,  87.5,  92.5, 110. };

//G4double G4PiMinusStopC::eKin[22] = { 15.,  20.,  22., 25.,  
//				  30.,  32.,  36.,  
//				  40.,  45.,  48.,
//				  50.,  54.,  60., 65.,
//				  70.,  75.,  78., 
//				  80.,  86.,  90.,  95., 100. };

// Opening angle distributions measured for coincident nucleon emission
// R. Hartmann et al., Nucl. Phys. A308 (1978) 345

G4int G4PiMinusStopC::angleEntries = 7;

G4double G4PiMinusStopC::angleData[7] = 
{ 1.43, 1.67, 2.62, 4.29, 7.62, 11.90, 14.76 };

G4double G4PiMinusStopC::angle[8] = { 1.308997, 1.570796, 1.832596, 2.094395, 
				  2.356194, 2.617994, 2.967060, 3.1415927 };



// Constructor

G4PiMinusStopC::G4PiMinusStopC()
  
{
  // Cluster size: nucleon pair, alpha, triton etc.
  // First implementation: interaction with nucleon pair only
  _clusterSize = 2;

  // R ratio
  theR = 1. / (1. + npRatio);

  _definitions = new G4std::vector<G4ParticleDefinition*>();
  _momenta = new G4std::vector<G4LorentzVector*>();

  G4std::vector<double> eKinVector;
  G4std::vector<double> eKinDataVector;
  int i;
  for (i=0; i<eKinEntries; i++)
    {
      eKinVector.push_back(eKin[i]);
      eKinDataVector.push_back(eKinData[i]);
    }
  eKinVector.push_back(eKin[eKinEntries]);
  _distributionE = new G4DistributionGenerator(eKinVector,eKinDataVector);

  G4std::vector<double> angleVector;
  G4std::vector<double> angleDataVector;
  for (i=0; i<angleEntries; i++)
    {
      angleVector.push_back(angle[i]);
      angleDataVector.push_back(angleData[i]);
    }
  angleVector.push_back(angle[angleEntries]);
  _distributionAngle = new G4DistributionGenerator(angleVector,angleDataVector);
}


// Destructor

G4PiMinusStopC::~G4PiMinusStopC()
{}

G4double G4PiMinusStopC::FinalNucleons()
{
  return nFinalNucleons;
}


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
// $Id: exampleB01.cc,v 1.19 2003-08-25 12:32:29 dressel Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - exampleB01
//
// --------------------------------------------------------------
// Comments
//
// This example intends to show how to use importance sampling and scoring
// in the mass (tracking) geometry.
// A simple geometry consisting of a 180 cm high concrete cylinder
// divided into 18 slabs of 10cm each is created. 
// Importance values are assigned to the 18 concrete slabs in the
// detector construction class for simplicity.
// Pairs of G4GeometryCell and importance values are stored in
// the importance store.
// The G4Scorer is used for the scoring. This is a top level
// class using the frame work provided for scoring.
// 

// --------------------------------------------------------------

#include <iostream>

#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "B01DetectorConstruction.hh"
#include "B01PhysicsList.hh"
#include "B01PrimaryGeneratorAction.hh"

// Files specific for biasing and scoring
#include "G4Scorer.hh"
#include "G4MassGeometrySampler.hh"
#include "G4IStore.hh"
#include "G4VWeightWindowStore.hh"
#include "G4WeightWindowAlgorithm.hh"

// a score table
#include "G4ScoreTable.hh"


int main(int argc, char **argv)
{  

  G4int mode = 0;
  if (argc>1) {
    mode = atoi(argv[1]);
  }

  std::ostream *myout = &G4cout;
  G4int numberOfEvent = 100;

  G4long myseed = 345354;

  HepRandom::setTheSeed(myseed);

  G4RunManager *runManager = new G4RunManager;
  
  // create the detector      ---------------------------
  B01DetectorConstruction *detector = new B01DetectorConstruction();
  runManager->SetUserInitialization(detector);
  //  ---------------------------------------------------
  runManager->SetUserInitialization(new B01PhysicsList);
  runManager->SetUserAction(new B01PrimaryGeneratorAction);
  runManager->Initialize();

  // pointers for importance store, weight window sotre
  // and weight window algorithm
  G4VIStore *aIstore = 0;
  G4VWeightWindowStore *aWWstore = 0;
  G4VWeightWindowAlgorithm *wwAlg = 0;


  // use a scorer
  G4Scorer scorer; 

  // create sampler for biasing and scoring 
  // in the mass geometry

  G4MassGeometrySampler mgs("neutron");
  mgs.PrepareScoring(&scorer);

  if (mode == 0) { 
    // prepare for importance sampling
    aIstore = detector->CreateImportanceStore();
    mgs.PrepareImportanceSampling(aIstore, 0);
  }
  else {
    // prepare for weight window technique
    // in this case the algoritm is initialized such that
    // the weight window tehcnique does exactly the same as
    // the importance sampling technique, therefore
    // the place of action ( the locations where 
    // splitting and Russian roulette are to be applied)
    // is chosen to be on the boundary between cells.
    aWWstore = detector->CreateWeightWindowStore();

    wwAlg = new G4WeightWindowAlgorithm(1, // upper limit factor
					1, // survival factor 
					100); // max. number of splitting
      
    mgs.PrepareWeightWindow(aWWstore, wwAlg, 
			    onBoundary); // place of action
  }
  mgs.Configure();

  runManager->BeamOn(numberOfEvent);

  // print a table of the scores
  G4ScoreTable sp(aIstore);
  sp.Print(scorer.GetMapGeometryCellCellScorer(), myout);
  
  if (aIstore) {
    delete aIstore;
  }
  if (aWWstore) {
    delete aWWstore;
  }
  if (wwAlg) {
    delete wwAlg;
  }
  return 0;
}

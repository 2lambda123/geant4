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
// $Id: G4Scorer.hh,v 1.2 2006-06-29 18:09:01 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4Scorer
//
// Class description:
//
// This is a simple scorer using the G4CellScorerStore and 
// G4CellStoreScorer.
// 

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------

#ifndef G4Scorer_hh
#define G4Scorer_hh G4Scorer_hh

#include "G4VScorer.hh"
#include "G4CellScorerStore.hh"
#include "G4CellStoreScorer.hh"

class G4Scorer : public G4VScorer {
public: // with description
  G4Scorer();

  virtual ~G4Scorer();

  virtual void Score(const G4Step &aStep, const G4GeometryCellStep &aPStep);
    // called to score a track for every step

  const G4MapGeometryCellCellScorer &GetMapGeometryCellCellScorer() const ;
    // returns a reference to the store containing the scores

private:
  G4Scorer(const G4Scorer &);
  G4Scorer &operator=(const G4Scorer &);
  G4CellScorerStore fCellScorerStore;  
  G4CellStoreScorer fCellStoreScorer;
};

#endif

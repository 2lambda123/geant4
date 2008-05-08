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
// $Id: G4DeuteronEvaporationProbability.hh,v 1.10 2008-05-08 10:09:22 quesada Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999) 
//
// J. M. Quesada (Apr. 2008) unused items have been removed (AlphaParam, BetaParam, CCoefficient, ExcitEnegies, ExcitSpins) 


#ifndef G4DeuteronEvaporationProbability_h
#define G4DeuteronEvaporationProbability_h 1


#include "G4EvaporationProbability.hh"
#include "G4DeuteronCoulombBarrier.hh"

class G4DeuteronEvaporationProbability : public G4EvaporationProbability
{
public:
  // Only available constructor
  G4DeuteronEvaporationProbability();

  ~G4DeuteronEvaporationProbability() {}
private:  
  // Copy constructor
  G4DeuteronEvaporationProbability(const G4DeuteronEvaporationProbability &right);

  const G4DeuteronEvaporationProbability & operator=(const G4DeuteronEvaporationProbability &right);
  G4bool operator==(const G4DeuteronEvaporationProbability &right) const;
  G4bool operator!=(const G4DeuteronEvaporationProbability &right) const;

private:

  G4DeuteronCoulombBarrier theCoulombBarrier;
};
#endif

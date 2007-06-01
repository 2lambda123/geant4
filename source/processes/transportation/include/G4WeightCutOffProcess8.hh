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
// $Id: G4WeightCutOffProcess8.hh,v 1.2 2007-06-01 07:53:26 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4WeightCutOffProcess8
//
// Class description:
//
// Process for biasing particle-change cutoff.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4WeightCutOffProcess8_hh 
#define G4WeightCutOffProcess8_hh G4WeightCutOffProcess8_hh

#include "G4VProcess.hh"
#include "G4VTrackTerminator.hh"
#include "G4GeometryCell.hh"

class G4VGCellFinder;
class G4VIStore;

class G4WeightCutOffProcess8 : public G4VProcess
{

public:  // with description

  G4WeightCutOffProcess8(G4double wsurvival,
                        G4double wlimit,
                        G4double isource,
                        G4VIStore *istore,
                        const G4VGCellFinder &aGCellFinder,
                        const G4String &aName = "WeightCutOffProcess8");
    // create a G4ParticleChange

  virtual ~G4WeightCutOffProcess8();
    // delete the G4ParticleChange

  virtual G4double 
  PostStepGetPhysicalInteractionLength(const G4Track& aTrack,
                                       G4double   previousStepSize,
                                       G4ForceCondition* condition);
    // make the process beeing forced

  virtual G4VParticleChange * PostStepDoIt(const G4Track&, 
                                           const G4Step&);


  const G4String &GetName() const;

public:  // without description

  //  no operation in  AtRestDoIt and  AlongStepDoIt

  virtual G4double 
  AlongStepGetPhysicalInteractionLength(const G4Track&,
                                        G4double  ,
                                        G4double  ,
                                        G4double& ,
                                        G4GPILSelection*);
  
  virtual G4double 
  AtRestGetPhysicalInteractionLength(const G4Track& ,
                                     G4ForceCondition*);

  virtual G4VParticleChange* AtRestDoIt(const G4Track&, const G4Step&);
  virtual G4VParticleChange* AlongStepDoIt(const G4Track&, const G4Step&);
  
private:

  G4WeightCutOffProcess8(const G4WeightCutOffProcess8 &);
  G4WeightCutOffProcess8 &operator=(const G4WeightCutOffProcess8 &);

  G4ParticleChange *fParticleChange;
  G4double fWeightSurvival;
  G4double fWeightLimit;
  G4double fSourceImportance;
  G4VIStore *fIStore;
  const G4VGCellFinder &fGCellFinder;

};

#endif

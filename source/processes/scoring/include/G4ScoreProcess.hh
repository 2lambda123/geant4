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
// $Id: G4ScoreProcess.hh,v 1.1 2006-11-20 10:02:06 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4ScoreProcess
//
// Class description:
//
// Used internally by scoring in the "mass" world.
// This is a forced post step process messaging a "scorer" 
// derived from G4VScorer.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4ScoreProcess_hh 
#define G4ScoreProcess_hh G4ScoreProcess_hh 

#include "G4VProcess.hh"
#include "G4VTrackTerminator.hh"

class G4VScorer;

class G4Step;
class G4Navigator;
class G4TransportationManager;
class G4PathFinder;
class G4VTouchable;

#include "G4FieldTrack.hh"
#include "G4TouchableHandle.hh"


class G4ScoreProcess : public G4VProcess, public G4VTrackTerminator
{

public:  // with description

  explicit G4ScoreProcess(G4VScorer &aScorer,
                           const G4String &aName = "ScoreProcess", G4bool para = false);
    // take reference to scorer and coppy particle name and
    // create a G4ParticleChange

  virtual ~G4ScoreProcess();
    // delete the G4ParticleChange


  //--------------------------------------------------------------
  // Set Paralle World
  //--------------------------------------------------------------

  void SetParallelWorld(G4String parallelWorldName);
  void SetParallelWorld(G4VPhysicalVolume* parallelWorld);

  //--------------------------------------------------------------
  //     Process interface
  //--------------------------------------------------------------

  void StartTracking(G4Track*);
  


  virtual G4double 
  PostStepGetPhysicalInteractionLength(const G4Track& aTrack,
                                       G4double   previousStepSize,
                                       G4ForceCondition* condition);
    // make processed being forced

  virtual G4VParticleChange * PostStepDoIt(const G4Track&, const G4Step&);
    // message "scorer" with  G4Step and a G4GeometryCellStep from the "mass" 
    // geometry

  virtual void KillTrack() const;

    // to be called by the importance process if the track should
    // be killed after scoring
  virtual const G4String &GetName() const ;

public:  // without description

  // no operation in  AtRestDoIt and  AlongStepDoIt

  virtual G4double 
  AlongStepGetPhysicalInteractionLength(const G4Track&,
                                        G4double  ,
                                        G4double  ,
                                        G4double& ,
                                        G4GPILSelection*);
  
  virtual G4double 
  AtRestGetPhysicalInteractionLength(const G4Track&,
                                     G4ForceCondition*);
  
  virtual G4VParticleChange* AtRestDoIt(const G4Track&,
                                        const G4Step&);
  virtual G4VParticleChange* AlongStepDoIt(const G4Track&,
                                           const G4Step&);
  
private:

  G4ScoreProcess(const G4ScoreProcess &);
  G4ScoreProcess &operator=(const G4ScoreProcess &);
  
private:

  void CopyStep(const G4Step & step);

  G4Step * fGhostStep;
  G4StepPoint * fGhostPreStepPoint;
  G4StepPoint * fGhostPostStepPoint;

  G4ParticleChange *fParticleChange;

  G4VScorer &fScorer;  
  mutable G4bool fKillTrack;


  G4TransportationManager* fTransportationManager;
  G4PathFinder*        fPathFinder;

  // -------------------------------
  // Navigation in the Ghost World:
  // -------------------------------
  G4String             fGhostWorldName;
  G4VPhysicalVolume*   fGhostWorld;
  G4Navigator*         fGhostNavigator;
  G4int                fNavigatorID;
  G4TouchableHandle    fOldGhostTouchable;
  G4TouchableHandle    fNewGhostTouchable;
  G4FieldTrack         fFieldTrack;
  G4double             fGhostSafety;
  G4bool               fOnBoundary;

  G4bool               paraflag;


};

#endif

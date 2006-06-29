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
// $Id: TiaraSim.cc,v 1.9 2006-06-29 15:45:31 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "TiaraSim.hh"
#include <iostream>

#include "CLHEP/Random/Random.h"

#include "G4RunManager.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VUserPhysicsList.hh"

#include "G4VUserDetectorConstruction.hh"

#include "TiaraPrimaryGeneratorAction.hh"
#include "G4VisExecutive.hh"
#include "G4UItcsh.hh"
#include "TiaraVisEventAction.hh"
#include "TiaraVisRunAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "TiaraEnergyCutProcess.hh"
#include "G4UserStackingAction.hh"
#include "G4UserEventAction.hh"
#include "TiaraStackingAction.hh"


TiaraSim *TiaraSim::fTiaraSim = 0;
TiaraSim &TiaraSim::GetTiaraSim() {
  if (!fTiaraSim) {
    fTiaraSim = new TiaraSim;
  }
  return *fTiaraSim;
}

TiaraSim::TiaraSim():
  fVisManager(0),
  fSession(new G4UItcsh),
  fGeometry(0),
  fPrimary(0),
  fPhysics(0),
  fCutProcessVector(),
  fPlacers(),
  fStackingAction(0),
  fUserEventAction(0)
{

  frunMgr = new G4RunManager;
  
}

void TiaraSim::SetGeometry(G4VUserDetectorConstruction *geo){
  fGeometry = geo;
}


void TiaraSim::AddTiaraEventAction(G4UserEventAction *eventAction) {
  fUserEventAction = eventAction;
}

void TiaraSim::
SetPhysicsList(G4VUserPhysicsList *physicsList) {
  fPhysics = physicsList;
  frunMgr->SetUserInitialization(fPhysics);
}

void TiaraSim::
SetPrimaryGenerator(G4VUserPrimaryGeneratorAction *primGen){
  fPrimary = primGen;
}



void TiaraSim::AddParticleCut(const std::string &particle,
			      G4double cut) {
  if (!fStackingAction) {
    fStackingAction = new TiaraStackingAction;
    frunMgr->SetUserAction(fStackingAction);
  }
  TiaraEnergyCutProcess *cutProcess = new TiaraEnergyCutProcess(cut);
  fCutProcessVector.push_back(cutProcess);
  fPlacers.push_back(G4ProcessPlacer(particle));
  fPlacers[fPlacers.size() - 1].
    AddProcessAsLastDoIt(cutProcess);
  fStackingAction->AddParticleCut(particle, cut);
}

void TiaraSim::AddVisRunAction() {
  fVisManager = new G4VisExecutive; 
  frunMgr->SetUserAction(new TiaraVisRunAction);
}
void TiaraSim::initializeVisManager() {
  if (fVisManager) {
    fVisManager->Initialize();
  }else {
    G4cout << "TiaraSim::initializeVisManager(): no VisManager!" << G4endl;
  }
};
void TiaraSim::initialize(){
  G4bool ready(true);
  if (!fPrimary) {
    G4cout << "TiaraSim::initialize: no primary generator set" << G4endl;
    ready = false;
  } 
  if (ready) {
    frunMgr->SetUserInitialization(fGeometry);
    frunMgr->SetUserAction(fPrimary);
    if (fUserEventAction) {
      frunMgr->SetUserAction(fUserEventAction);
    }
    if (fVisManager) {
      fVisManager->Initialize();
    }
    frunMgr->Initialize();
  }
}

void TiaraSim::startSession(){
  fSession.SessionStart();
}


void TiaraSim::BeamOn(G4int nEvents){
  frunMgr->BeamOn(nEvents);
}

TiaraSim::~TiaraSim(){
  for (size_t i = 0; i < fCutProcessVector.size(); i++) {
    fPlacers[i].RemoveProcess(fCutProcessVector[i]);
    delete fCutProcessVector[i];
  }
  if (fStackingAction) {
    delete fStackingAction;
  }
  if (fVisManager) {
    delete fVisManager;
  }
  delete frunMgr;
}



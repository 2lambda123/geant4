#include "TiaraSim.hh"
#include "g4std/iostream"

#include "CLHEP/Random/Random.h"

#include "G4RunManager.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VUserPhysicsList.hh"

#include "G4VUserDetectorConstruction.hh"

#include "TiaraPrimaryGeneratorAction.hh"
#include "TiaraVisManager.hh"
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
  fVisManager = new TiaraVisManager; 
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
  if (!fPhysics) {
    G4cout << "TiaraSim::initialize: no physics list set" << G4endl;
    ready = false;
  }
  if (ready) {
    frunMgr->SetUserInitialization(fGeometry);
    frunMgr->SetUserInitialization(fPhysics);
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
  for (G4int i = 0; i < fCutProcessVector.size(); i++) {
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



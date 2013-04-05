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
/// \file hadronic/Hadr01/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
// $Id$
//
/////////////////////////////////////////////////////////////////////////
//
// EventAction
//
// Created: 21.06.2008 V.Ivanchenko
//
// Modified:
//
////////////////////////////////////////////////////////////////////////
// 

#include "EventAction.hh"
#include "G4Event.hh"
#include "EventActionMessenger.hh"
#include "HistoManager.hh"

#include "G4UImanager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

EventAction::EventAction():
  fPrintModulo(100),
  fSelected(0),
  fDebugStarted(false)
{
  fEventMessenger = new EventActionMessenger(this);
  UI = G4UImanager::GetUIpointer();
  fSelectedEvents.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

EventAction::~EventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  // New event
  G4int nEvt = evt->GetEventID();

  if(fSelected>0) {
    for(G4int i=0; i<fSelected; ++i) {
      if(nEvt == fSelectedEvents[i]) {
        UI->ApplyCommand("/random/saveThisEvent");
        UI->ApplyCommand("/tracking/verbose  2");
        fDebugStarted = true;
        break;
      }
    }
  }

  // Initialize user actions
  if(G4int(nEvt/fPrintModulo)*fPrintModulo == nEvt) {
    G4cout << "EventAction: Event # "
           << nEvt << " started" << G4endl;
  }
  HistoManager::GetPointer()->BeginOfEvent(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::EndOfEventAction(const G4Event* evt)
{
  if(fDebugStarted) {
    UI->ApplyCommand("/tracking/verbose  0");
    fDebugStarted = false;
    G4cout << "EventAction: Event ended" << G4endl;
  }
  HistoManager* man = HistoManager::GetPointer();
  man->EndOfEvent(); 
  if(man->GetVerbose() > 1) {
    G4cout << "EventAction: Event " << evt->GetEventID() << " ended" 
	   << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

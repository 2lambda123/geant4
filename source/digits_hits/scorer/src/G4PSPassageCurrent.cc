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
// $Id: G4PSPassageCurrent.cc,v 1.1 2007-07-11 01:31:03 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4PSPassageCurrent
#include "G4PSPassageCurrent.hh"
#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring number of tracks,
//   where only tracks passing through the geometry are taken 
//  into account.
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 
///////////////////////////////////////////////////////////////////////////////

G4PSPassageCurrent::G4PSPassageCurrent(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),fCurrentTrkID(-1),fCurrent(0)
{;}

G4PSPassageCurrent::~G4PSPassageCurrent()
{;}

G4bool G4PSPassageCurrent::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  if ( IsPassed(aStep) ) {
    fCurrent = aStep->GetPreStepPoint()->GetWeight();
    G4int index = GetIndex(aStep);
    EvtMap->add(index,fCurrent);
  }

  return TRUE;
}

G4bool G4PSPassageCurrent::IsPassed(G4Step* aStep){
  G4bool Passed = FALSE;

  G4bool IsEnter = aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary;
  G4bool IsExit  = aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary;

  G4int  trkid  = aStep->GetTrack()->GetTrackID();

  if ( IsEnter &&IsExit ){         // Passed at one step
    Passed = TRUE;                 
  }else if ( IsEnter ){            // Enter a new geometry
    fCurrentTrkID = trkid;         // Resetting the current track.
  }else if ( IsExit ){             // Exit a current geometry
    if ( fCurrentTrkID == trkid ) {
      Passed = TRUE;               // if the track is same as entered.
    }
  }else{                           // Inside geometry
    if ( fCurrentTrkID == trkid ){ // Adding the track length to current one ,
    }
  }
  return Passed;
}

void G4PSPassageCurrent::Initialize(G4HCofThisEvent* HCE)
{
  fCurrentTrkID = -1;

  EvtMap = new G4THitsMap<G4double>(detector->GetName(),
				    GetName());
  if ( HCID < 0 ) HCID = GetCollectionID(0);
  HCE->AddHitsCollection(HCID,EvtMap);

}

void G4PSPassageCurrent::EndOfEvent(G4HCofThisEvent*)
{
}

void G4PSPassageCurrent::clear(){
  EvtMap->clear();
}

void G4PSPassageCurrent::DrawAll()
{;}

void G4PSPassageCurrent::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() <<G4endl; 
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  cell current : " << *(itr->second)
	   << G4endl;
  }
}


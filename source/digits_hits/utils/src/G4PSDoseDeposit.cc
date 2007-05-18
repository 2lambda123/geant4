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
// $Id: G4PSDoseDeposit.cc,v 1.5 2007-05-18 00:00:38 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4PSDoseDeposit
#include "G4PSDoseDeposit.hh"
#include "G4VSolid.hh"
#include "G4UnitsTable.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only energy deposit.
// 
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 
///////////////////////////////////////////////////////////////////////////////

G4PSDoseDeposit::G4PSDoseDeposit(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1)
{;}

G4PSDoseDeposit::~G4PSDoseDeposit()
{;}

G4bool G4PSDoseDeposit::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double volume  = aStep->GetPreStepPoint()->GetPhysicalVolume()
                     ->GetLogicalVolume()->GetSolid()->GetCubicVolume();
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double dose    = edep / ( density * volume );
  dose *= aStep->GetPreStepPoint()->GetWeight(); 
  G4int  index = GetIndex(aStep);
  EvtMap->add(index,dose);  
  return TRUE;
}

void G4PSDoseDeposit::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
				    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void G4PSDoseDeposit::EndOfEvent(G4HCofThisEvent*)
{;}

void G4PSDoseDeposit::clear()
{
  EvtMap->clear();
}

void G4PSDoseDeposit::DrawAll()
{;}

void G4PSDoseDeposit::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  dose deposit: " << G4BestUnit(*(itr->second),"Dose")
	   << G4endl;
  }
}


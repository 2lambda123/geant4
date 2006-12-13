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
// $Id: A01EventAction.cc,v 1.2 2006-12-13 15:49:05 gunter Exp $
// --------------------------------------------------------------
//

#include "A01EventAction.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

A01EventAction::A01EventAction(G4bool val)
{
  ifPara = val;
  for(int i=0;i<4;i++)
  {
    realWorldID[i] = -1;
    paraWorldID[i] = -1;
  }
}

A01EventAction::~A01EventAction()
{
}

void A01EventAction::BeginOfEventAction(const G4Event*)
{
  G4String colName;
  if(realWorldID[0]<0) 
  {
    realWorldID[0] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="MassWorld/NofStep");
    realWorldID[1] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="MassWorld/TrackLength");
    realWorldID[2] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="MassWorld/NofSecondary");
    realWorldID[3] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="MassWorld/EnergyDeposit");
    if(ifPara) {
      paraWorldID[0] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="ParallelWorld/NofStep");
      paraWorldID[1] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="ParallelWorld/TrackLength");
      paraWorldID[2] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="ParallelWorld/NofSecondary");
      paraWorldID[3] = G4SDManager::GetSDMpointer()->GetCollectionID(colName="ParallelWorld/EnergyDeposit");
    }
  }
}

void A01EventAction::EndOfEventAction(const G4Event* evt)
{
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  if(!HCE) return;

  G4double rw[4];
  G4double pw[4];

  for(int i=0;i<4;i++)
  {
    rw[i] = 0.;
    pw[i] = 0.;
    G4THitsMap<G4double>* hm = (G4THitsMap<G4double>*)(HCE->GetHC(realWorldID[i]));
    if(hm) rw[i] = Total(hm);
    if(ifPara) {
      hm = (G4THitsMap<G4double>*)(HCE->GetHC(paraWorldID[i]));
      if(hm) pw[i] = Total(hm);
    }
  }

  G4PrimaryParticle* primary = evt->GetPrimaryVertex(0)->GetPrimary(0);
  G4cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << G4endl;
  G4cout << ">>> Event " << evt->GetEventID() << " >>> Simulation truth : "
         << primary->GetG4code()->GetParticleName()
         << " momentum : " << primary->GetMomentum()/MeV << " (MeV)" << G4endl;
  G4cout << "No. of steps in REAL world :               " << std::setw(8) << rw[0]
         << " --- in PARALLEL world : " << std::setw(8) << pw[0] << G4endl;
  G4cout << "Total track length (mm) in REAL world :    " << std::setw(8) << rw[1]/mm
         << " --- in PARALLEL world : " << std::setw(8) << pw[1]/mm << G4endl;
  G4cout << "No. of secondaries in REAL world :         " << std::setw(8) << rw[2]
         << " --- in PARALLEL world : " << std::setw(8) << pw[2] << G4endl;
  G4cout << "Total energy deposit (MeV) in REAL world : " << std::setw(8) << rw[3]/MeV
         << " --- in PARALLEL world : " << std::setw(8) << pw[3]/MeV << G4endl;
  G4cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << G4endl;
}

G4double A01EventAction::Total(G4THitsMap<G4double>* hm)
{
  G4double tot = 0.;
  std::map<G4int,G4double*>::iterator itr = hm->GetMap()->begin();
  for(;itr!=hm->GetMap()->end();itr++)
  { tot += *(itr->second); }
  return tot;
}

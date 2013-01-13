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
// $Id$
//

#include "G4HCofThisEvent.hh"

__thread G4Allocator<G4HCofThisEvent> *anHCoTHAllocator_G4MT_TLS_ = 0;

G4HCofThisEvent::G4HCofThisEvent()
{  ;;;   if (!anHCoTHAllocator_G4MT_TLS_) anHCoTHAllocator_G4MT_TLS_ = new G4Allocator<G4HCofThisEvent>  ; G4Allocator<G4HCofThisEvent> &anHCoTHAllocator = *anHCoTHAllocator_G4MT_TLS_;  ;;;  
  HC = new std::vector<G4VHitsCollection*>;
}

G4HCofThisEvent::G4HCofThisEvent(G4int cap)
{  ;;;   if (!anHCoTHAllocator_G4MT_TLS_) anHCoTHAllocator_G4MT_TLS_ = new G4Allocator<G4HCofThisEvent>  ; G4Allocator<G4HCofThisEvent> &anHCoTHAllocator = *anHCoTHAllocator_G4MT_TLS_;  ;;;  
  HC = new std::vector<G4VHitsCollection*>;
  for(G4int i=0;i<cap;i++)
  {
    HC->push_back((G4VHitsCollection*)0);
  }
}

G4HCofThisEvent::~G4HCofThisEvent()
{  ;;;   if (!anHCoTHAllocator_G4MT_TLS_) anHCoTHAllocator_G4MT_TLS_ = new G4Allocator<G4HCofThisEvent>  ; G4Allocator<G4HCofThisEvent> &anHCoTHAllocator = *anHCoTHAllocator_G4MT_TLS_;  ;;;  
  //HC->clearAndDestroy();
  for(size_t i=0;i<HC->size();i++)
  { delete (*HC)[i]; }
  HC->clear();
  delete HC;
}

void G4HCofThisEvent::AddHitsCollection(G4int HCID,G4VHitsCollection * aHC)
{  ;;;   if (!anHCoTHAllocator_G4MT_TLS_) anHCoTHAllocator_G4MT_TLS_ = new G4Allocator<G4HCofThisEvent>  ; G4Allocator<G4HCofThisEvent> &anHCoTHAllocator = *anHCoTHAllocator_G4MT_TLS_;  ;;;  
  if(HCID>=0 && HCID<G4int(HC->size()))
  { (*HC)[HCID] = aHC; }
}



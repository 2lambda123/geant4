//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4VPrimitiveSensitivity.cc,v 1.3 2005-11-16 22:41:28 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4VPrimitiveSensitivity
#include "G4VPrimitiveSensitivity.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"


G4VPrimitiveSensitivity::G4VPrimitiveSensitivity(G4String name, G4int depth)
 :primitiveName(name),detector(0),filter(0),verboseLevel(0),indexDepth(depth)
{;} 

G4VPrimitiveSensitivity::~G4VPrimitiveSensitivity()
{;}

G4int G4VPrimitiveSensitivity::GetCollectionID(G4int)
{
  if(detector)
   return G4SDManager::GetSDMpointer()
    ->GetCollectionID(detector->GetName()+"/"+primitiveName); 
  else
   return -1;
}

void G4VPrimitiveSensitivity::Initialize(G4HCofThisEvent*)
{;}

void G4VPrimitiveSensitivity::EndOfEvent(G4HCofThisEvent*)
{;}

void G4VPrimitiveSensitivity::clear()
{;}

void G4VPrimitiveSensitivity::DrawAll()
{;}

void G4VPrimitiveSensitivity::PrintAll()
{;}

G4int G4VPrimitiveSensitivity::GetIndex(G4Step* aStep)
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  return th->GetReplicaNumber(indexDepth);
}


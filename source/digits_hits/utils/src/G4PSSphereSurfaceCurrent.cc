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
// $Id: G4PSSphereSurfaceCurrent.cc,v 1.2 2005-11-16 23:24:08 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4PSSphereSurfaceCurrent
#include "G4PSSphereSurfaceCurrent.hh"
#include "G4StepStatus.hh"
#include "G4Track.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only Surface Current.
//  Current version assumes only for G4Sphere shape. 
//
// Surface is defined  at the inside of sphere.
// Direction                  -Rmin   +Rmax
//   0  IN || OUT            ->|<-     |
//   1  IN                   ->|       |
//   2  OUT                    |<-     |
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 
///////////////////////////////////////////////////////////////////////////////

G4PSSphereSurfaceCurrent::G4PSSphereSurfaceCurrent(G4String name, 
					 G4int direction, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),fDirection(direction)
{;}

G4PSSphereSurfaceCurrent::~G4PSSphereSurfaceCurrent()
{;}

G4bool G4PSSphereSurfaceCurrent::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4VSolid * solid = 
    preStep->GetPhysicalVolume()->GetLogicalVolume()->GetSolid();
  if( solid->GetEntityType() != "G4Sphere" ){
    G4Exception("G4PSSphereSurfaceCurrentScorer. - Solid type is not supported.");
    return FALSE;
  }
  G4Sphere* sphereSolid = (G4Sphere*)(solid);

  G4int dirFlag =IsSelectedSurface(aStep,sphereSolid);
  if ( dirFlag > 0 ) {
    G4int index = GetIndex(aStep);
    G4double square = 
      sphereSolid->GetInsideRadius()*sphereSolid->GetDeltaPhiAngle()/radian
      *sphereSolid->GetInsideRadius()*sphereSolid->GetDeltaThetaAngle()/radian;
    G4TouchableHandle theTouchable = preStep->GetTouchableHandle();
    G4ThreeVector pdirection = preStep->GetMomentumDirection();
    G4ThreeVector localdir  = 
      theTouchable->GetHistory()->GetTopTransform().TransformAxis(pdirection);
    G4double current = preStep->GetWeight(); // Current (Particle Weight)
    current = current/square;  // Current with angle.

    if ( fDirection == fCurrent_InOut || fDirection == dirFlag ){
      EvtMap->add(index,current);
    }
  }

  return TRUE;
}

G4int G4PSSphereSurfaceCurrent::IsSelectedSurface(G4Step* aStep, G4Sphere* sphereSolid){

  G4TouchableHandle theTouchable = 
    aStep->GetPreStepPoint()->GetTouchableHandle();
  
  if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary ){
    // Entering Geometry
    G4ThreeVector stppos1= aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localpos1 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos1);
    //G4cout << " Enter " << localpos1 << " " <<GetIndex(aStep)<<G4endl;
    G4double localR2 = localpos1.x()*localpos1.x()
                      +localpos1.y()*localpos1.y()
                      +localpos1.z()*localpos1.z();
    G4double InsideRadius2 = 
      sphereSolid->GetInsideRadius()*sphereSolid->GetInsideRadius();
    if(fabs( localR2 - InsideRadius2 ) < kCarTolerance ){
      return fCurrent_In;
    }
  }

  if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary ){
    // Exiting Geometry
    G4ThreeVector stppos2= aStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector localpos2 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos2);
    G4double localR2 = localpos2.x()*localpos2.x()
                      +localpos2.y()*localpos2.y()
                      +localpos2.z()*localpos2.z();
    G4double InsideRadius2 = 
      sphereSolid->GetInsideRadius()*sphereSolid->GetInsideRadius();
    if(fabs( localR2 - InsideRadius2 ) < kCarTolerance ){
      return fCurrent_Out;
    }
  }

  return -1;
}

void G4PSSphereSurfaceCurrent::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(detector->GetName(), GetName());
  if ( HCID < 0 ) HCID = GetCollectionID(0);
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void G4PSSphereSurfaceCurrent::EndOfEvent(G4HCofThisEvent*)
{;}

void G4PSSphereSurfaceCurrent::clear(){
  EvtMap->clear();
}

void G4PSSphereSurfaceCurrent::DrawAll()
{;}

void G4PSSphereSurfaceCurrent::PrintAll()
{
  G4cout << " PrimitiveSenstivity " << GetName() <<G4endl; 
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  current  : " << *(itr->second) /mm2
	   << G4endl;
  }
}


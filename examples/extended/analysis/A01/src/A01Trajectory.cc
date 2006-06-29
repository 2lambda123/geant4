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
// $Id: A01Trajectory.cc,v 1.6 2006-06-29 16:33:11 gunter Exp $
// --------------------------------------------------------------
//

#include "A01Trajectory.hh"
#include "G4TrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ThreeVector.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"

G4Allocator<A01Trajectory> myTrajectoryAllocator;

A01Trajectory::A01Trajectory()
{
   fpParticleDefinition = 0;
   ParticleName = "";
   PDGCharge = 0;
   PDGEncoding = 0;
   fTrackID = 0;
   fParentID = 0;
   positionRecord = 0;
}

A01Trajectory::A01Trajectory(const G4Track* aTrack)
{
   fpParticleDefinition = aTrack->GetDefinition();
   ParticleName = fpParticleDefinition->GetParticleName();
   PDGCharge = fpParticleDefinition->GetPDGCharge();
   PDGEncoding = fpParticleDefinition->GetPDGEncoding();
   fTrackID = aTrack->GetTrackID();
   fParentID = aTrack->GetParentID();
   positionRecord = new A01TrajectoryPointContainer();
   positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
}

A01Trajectory::A01Trajectory(A01Trajectory & right)
    : G4VTrajectory()
{
  ParticleName = right.ParticleName;
  fpParticleDefinition = right.fpParticleDefinition;
  PDGCharge = right.PDGCharge;
  PDGEncoding = right.PDGEncoding;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  positionRecord = new A01TrajectoryPointContainer();
  for(int i=0;i<(int)right.positionRecord->size();i++)
  {
    G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }
}

A01Trajectory::~A01Trajectory()
{
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
}

void A01Trajectory::ShowTrajectory() const
{
   G4cout << G4endl << "TrackID =" << fTrackID
        << ":ParentID=" << fParentID << G4endl;
   G4cout << "Particle name : " << ParticleName
        << "  Charge : " << PDGCharge << G4endl;
   G4cout << "  Current trajectory has " << positionRecord->size()
        << " points." << G4endl;

   for( size_t i=0 ; i < positionRecord->size() ; i++){
       G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
       G4cout << "Point[" << i << "]"
            << " Position= " << aTrajectoryPoint->GetPosition() << G4endl;
   }
}

void A01Trajectory::ShowTrajectory(std::ostream& o) const
{
    G4VTrajectory::ShowTrajectory(o);
}


void A01Trajectory::DrawTrajectory(G4int /*i_mode*/) const
{

   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   G4ThreeVector pos;

   G4Polyline pPolyline;
   for (int i = 0; i < (int)positionRecord->size() ; i++) {
     G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
     pos = aTrajectoryPoint->GetPosition();
     pPolyline.push_back( pos );
   }

   G4Colour colour(0.75,0.75,0.75);    // LightGray
   if(fpParticleDefinition==G4Gamma::GammaDefinition())
      colour = G4Colour(0.,1.,1.);     // Cyan
   else if(fpParticleDefinition==G4Electron::ElectronDefinition()
         ||fpParticleDefinition==G4Positron::PositronDefinition())
      colour = G4Colour(1.,1.,0.);      // Yellow
   else if(fpParticleDefinition==G4MuonMinus::MuonMinusDefinition()
         ||fpParticleDefinition==G4MuonPlus::MuonPlusDefinition())
      colour = G4Colour(1.,0.,1.);      // Magenta
   else if(fpParticleDefinition->GetParticleType()=="meson")
   {
      if(PDGCharge!=0.)
         colour = G4Colour(1.,0.,0.);   // Red
      else
         colour = G4Colour(0.5,0.,0.);  // HalfRed
   }
   else if(fpParticleDefinition->GetParticleType()=="baryon")
   {
      if(PDGCharge!=0.)
         colour = G4Colour(1.,0.78,0.); // Orange
      else
         colour = G4Colour(0.5,0.39,0.);// HalfOrange
   }

   G4VisAttributes attribs(colour);
   pPolyline.SetVisAttributes(attribs);
   if(pVVisManager) pVVisManager->Draw(pPolyline);
}

void A01Trajectory::AppendStep(const G4Step* aStep)
{
   positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
                                 GetPosition() ));
}

G4ParticleDefinition* A01Trajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

void A01Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  A01Trajectory* seco = (A01Trajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  {
    positionRecord->push_back((*(seco->positionRecord))[i]);
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();

}



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
// $Id: G4Trajectory.cc,v 1.1 2002-11-25 10:13:12 dressel Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// ---------------------------------------------------------------
//
// G4Trajectory.cc
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Makoto  Asai   (e-mail: asai@kekvax.kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
// ---------------------------------------------------------------


#include "G4Trajectory.hh"
#include "G4TrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"

G4Allocator<G4Trajectory> aTrajectoryAllocator;

///////////////////////////////////////////
G4Trajectory::G4Trajectory()
:  positionRecord(0), fTrackID(0), fParentID(0),
   PDGEncoding( 0 ), PDGCharge(0.0), ParticleName("")
///////////////////////////////////////////
{
   // Unused: G4ParticleDefinition * fpParticleDefinition = 0;
}

///////////////////////////////////////////
G4Trajectory::G4Trajectory(const G4Track* aTrack)
///////////////////////////////////////////
{
   G4ParticleDefinition * fpParticleDefinition = aTrack->GetDefinition();
   ParticleName = fpParticleDefinition->GetParticleName();
   PDGCharge = fpParticleDefinition->GetPDGCharge();
   PDGEncoding = fpParticleDefinition->GetPDGEncoding();
   fTrackID = aTrack->GetTrackID();
   fParentID = aTrack->GetParentID();
   //   positionRecord = new G4RWTPtrOrderedVector<G4VTrajectoryPoint>;
   positionRecord = new TrajectoryPointContainer();
   positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
}

//////////////////////////////////////////
G4Trajectory::G4Trajectory(G4Trajectory & right)
//////////////////////////////////////////
{
  ParticleName = right.ParticleName;
  PDGCharge = right.PDGCharge;
  PDGEncoding = right.PDGEncoding;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  //  positionRecord = new G4RWTPtrOrderedVector<G4VTrajectoryPoint>;
  //  G4std::vector<G4VTrajectoryPoint*> *positionRecord;
  positionRecord = new TrajectoryPointContainer();

  for(size_t i=0;i<right.positionRecord->size();i++)
  {
    G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }
}


/////////////////////////////
G4Trajectory::~G4Trajectory()
/////////////////////////////
{
  //  positionRecord->clearAndDestroy();
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
}

///////////////////////////////////
void G4Trajectory::ShowTrajectory() const
///////////////////////////////////
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

///////////////////////////////////////////////
void G4Trajectory::DrawTrajectory(G4int i_mode) const
///////////////////////////////////////////////
{

   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   G4ThreeVector pos;

   if(i_mode>=0)
   {
     G4Polyline pPolyline;
     for (size_t i = 0; i < positionRecord->size() ; i++) {
       G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[i]);
       pos = aTrajectoryPoint->GetPosition();
       pPolyline.push_back( pos );
     }

     G4Colour colour;
     if (ParticleName == "proton") {
       colour = G4Colour(1,0,1); // magenta
     }
     else if (ParticleName == "neutron") {
       colour = G4Colour(0,1,1); // cyan
     }
     else if (ParticleName == "gamma") {
       colour = G4Colour(1,1,0); // yellow
     }
     else {
       if(PDGCharge<0.) 
	 colour = G4Colour(1.,0.,0.); // red
       else if(PDGCharge>0.) 
	 colour = G4Colour(0.,0.,1.); // blue
       else 
	 colour = G4Colour(0.,1.,0.); // green
     }
     G4VisAttributes attribs(colour);
     pPolyline.SetVisAttributes(attribs);
     if(pVVisManager) pVVisManager->Draw(pPolyline);
   }

   if(i_mode!=0)
   {
     for(size_t j=0; j<positionRecord->size(); j++) {
       G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*positionRecord)[j]);
       pos = aTrajectoryPoint->GetPosition();
       G4Circle circle( pos );
       circle.SetScreenSize(0.001*i_mode);
       circle.SetFillStyle(G4Circle::filled);
       G4Colour colSpot(0.,0.,0.);
       G4VisAttributes attSpot(colSpot);
       circle.SetVisAttributes(attSpot);
       if(pVVisManager) pVVisManager->Draw(circle);
     }
   }

}

////////////////////////////////////////////
void G4Trajectory::AppendStep(const G4Step* aStep)
////////////////////////////////////////////
{
   positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
                                 GetPosition() ));
}
  
/////////////////////////////////////////////
G4ParticleDefinition* G4Trajectory::GetParticleDefinition()
/////////////////////////////////////////////
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

/////////////////////////////////////////////
void G4Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
/////////////////////////////////////////////
{
  if(!secondTrajectory) return;

  G4Trajectory* seco = (G4Trajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(G4int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  { 
    positionRecord->push_back((*(seco->positionRecord))[i]);
    //    positionRecord->push_back(seco->positionRecord->removeAt(1));
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();
}



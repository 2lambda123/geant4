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
// $Id: G4SmoothTrajectory.cc,v 1.13 2004-11-18 23:10:10 perl Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// ---------------------------------------------------------------
//
// G4SmoothTrajectory.cc
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Makoto  Asai   (e-mail: asai@kekvax.kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
// ---------------------------------------------------------------

#include "G4SmoothTrajectory.hh"
#include "G4SmoothTrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include <strstream>

G4Allocator<G4SmoothTrajectory> aSmoothTrajectoryAllocator;

G4SmoothTrajectory::G4SmoothTrajectory()
:  positionRecord(0), fTrackID(0), fParentID(0),
   PDGEncoding( 0 ), PDGCharge(0.0), ParticleName(""),
   initialMomentum( G4ThreeVector() )
{;}

G4SmoothTrajectory::G4SmoothTrajectory(const G4Track* aTrack)
{
   G4ParticleDefinition * fpParticleDefinition = aTrack->GetDefinition();
   ParticleName = fpParticleDefinition->GetParticleName();
   PDGCharge = fpParticleDefinition->GetPDGCharge();
   PDGEncoding = fpParticleDefinition->GetPDGEncoding();
   fTrackID = aTrack->GetTrackID();
   fParentID = aTrack->GetParentID();
   initialMomentum = aTrack->GetMomentum();
   positionRecord = new TrajectoryPointContainer();
   // Following is for the first trajectory point
   positionRecord->push_back(new G4SmoothTrajectoryPoint(aTrack->GetPosition()));

   // The first point has no auxiliary points, so set the auxiliary
   // points vector to NULL (jacek 31/10/2002)
   positionRecord->push_back(new G4SmoothTrajectoryPoint(aTrack->GetPosition(), NULL));
}

G4SmoothTrajectory::G4SmoothTrajectory(G4SmoothTrajectory & right):G4VTrajectory()
{
  ParticleName = right.ParticleName;
  PDGCharge = right.PDGCharge;
  PDGEncoding = right.PDGEncoding;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  initialMomentum = right.initialMomentum;
  positionRecord = new TrajectoryPointContainer();

  for(size_t i=0;i<right.positionRecord->size();i++)
  {
    G4SmoothTrajectoryPoint* rightPoint = (G4SmoothTrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4SmoothTrajectoryPoint(*rightPoint));
  }
}

G4SmoothTrajectory::~G4SmoothTrajectory()
{
  //  positionRecord->clearAndDestroy();
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
}

void G4SmoothTrajectory::ShowTrajectory(std::ostream& os) const
{
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::ShowTrajectory(os);
  // ... or override with your own code here.
}

void G4SmoothTrajectory::DrawTrajectory(G4int i_mode) const
{
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::DrawTrajectory(i_mode);
  // ... or override with your own code here.
}

const std::map<G4String,G4AttDef>* G4SmoothTrajectory::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("G4SmoothTrajectory",isNew);
  if (isNew) {

    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"Track ID","Bookkeeping","","G4int");

    G4String PID("PID");
    (*store)[PID] = G4AttDef(PID,"Parent ID","Bookkeeping","","G4int");

    G4String PN("PN");
    (*store)[PN] = G4AttDef(PN,"Particle Name","Physics","","G4String");

    G4String Ch("Ch");
    (*store)[Ch] = G4AttDef(Ch,"Charge","Physics","","G4double");

    G4String PDG("PDG");
    (*store)[PDG] = G4AttDef(PDG,"PDG Encoding","Physics","","G4int");

    G4String IMom("IMom");
    (*store)[IMom] = G4AttDef(IMom, "Momentum of track at start of trajectory",
			      "Physics","","G4ThreeVector");

    G4String IMag("IMag");
    (*store)[IMag] = G4AttDef(IMag, "Magnitude of momentum of track at start of trajectory",
			      "Physics","","G4double");

    G4String NTP("NTP");
    (*store)[NTP] = G4AttDef(NTP,"No. of points","Physics","","G4int");

  }
  return store;
}


std::vector<G4AttValue>* G4SmoothTrajectory::CreateAttValues() const
{
  char c[100];
  std::ostrstream s(c,100);

  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  s.seekp(std::ios::beg);
  s << fTrackID << std::ends;
  values->push_back(G4AttValue("ID",c,""));

  s.seekp(std::ios::beg);
  s << fParentID << std::ends;
  values->push_back(G4AttValue("PID",c,""));

  values->push_back(G4AttValue("PN",ParticleName,""));

  s.seekp(std::ios::beg);
  s << PDGCharge << std::ends;
  values->push_back(G4AttValue("Ch",c,""));

  s.seekp(std::ios::beg);
  s << PDGEncoding << std::ends;
  values->push_back(G4AttValue("PDG",c,""));

  s.seekp(std::ios::beg);
  s << G4BestUnit(initialMomentum,"Energy") << std::ends;
  values->push_back(G4AttValue("IMom",c,""));

  s.seekp(std::ios::beg);
  s << initialMomentum.mag() << std::ends;
  values->push_back(G4AttValue("IMag",c,""));

  s.seekp(std::ios::beg);
  s << GetPointEntries() << std::ends;
  values->push_back(G4AttValue("NTP",c,""));

  return values;
}

void G4SmoothTrajectory::AppendStep(const G4Step* aStep)
{
  // (jacek 30/10/2002)
  positionRecord->push_back(
      new G4SmoothTrajectoryPoint(aStep->GetPostStepPoint()->GetPosition(),
				  aStep->GetPointerToVectorOfAuxiliaryPoints()));
}
  
G4ParticleDefinition* G4SmoothTrajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

void G4SmoothTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  G4SmoothTrajectory* seco = (G4SmoothTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(G4int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  { 
    positionRecord->push_back((*(seco->positionRecord))[i]);
    //    positionRecord->push_back(seco->positionRecord->removeAt(1));
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();
}



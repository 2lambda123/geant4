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
// $Id: G4Trajectory.cc,v 1.21 2003-05-23 15:18:41 asaim Exp $
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
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include "g4std/strstream"

G4Allocator<G4Trajectory> aTrajectoryAllocator;

G4Trajectory::G4Trajectory()
:  positionRecord(0), fTrackID(0), fParentID(0),
   PDGEncoding( 0 ), PDGCharge(0.0), ParticleName(""),
   initialMomentum( G4ThreeVector() )
{;}

G4Trajectory::G4Trajectory(const G4Track* aTrack)
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
   positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
}

G4Trajectory::G4Trajectory(G4Trajectory & right):G4VTrajectory()
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
    G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }
}

G4Trajectory::~G4Trajectory()
{
  //  positionRecord->clearAndDestroy();
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();

  delete positionRecord;
}

void G4Trajectory::ShowTrajectory(G4std::ostream& os) const
{
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::ShowTrajectory(os);
  // ... or override with your own code here.
}

void G4Trajectory::DrawTrajectory(G4int i_mode) const
{
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::DrawTrajectory(i_mode);
  // ... or override with your own code here.
}

const G4std::map<G4String,G4AttDef>* G4Trajectory::GetAttDefs() const
{
  G4bool isNew;
  G4std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("G4Trajectory",isNew);
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

    G4String NTP("NTP");
    (*store)[NTP] = G4AttDef(NTP,"No. of points","Physics","","G4int");

  }
  return store;
}

G4std::vector<G4AttValue>* G4Trajectory::CreateAttValues() const
{
  char c[100];
  G4std::ostrstream s(c,100);

  G4std::vector<G4AttValue>* values = new G4std::vector<G4AttValue>;

  s.seekp(G4std::ios::beg);
  s << fTrackID << G4std::ends;
  values->push_back(G4AttValue("ID",c,""));

  s.seekp(G4std::ios::beg);
  s << fParentID << G4std::ends;
  values->push_back(G4AttValue("PID",c,""));

  values->push_back(G4AttValue("PN",ParticleName,""));

  s.seekp(G4std::ios::beg);
  s << PDGCharge << G4std::ends;
  values->push_back(G4AttValue("Ch",c,""));

  s.seekp(G4std::ios::beg);
  s << PDGEncoding << G4std::ends;
  values->push_back(G4AttValue("PDG",c,""));

  s.seekp(G4std::ios::beg);
  s << G4BestUnit(initialMomentum,"Energy") << G4std::ends;
  values->push_back(G4AttValue("IMom",c,""));

  s.seekp(G4std::ios::beg);
  s << GetPointEntries() << G4std::ends;
  values->push_back(G4AttValue("NTP",c,""));

  return values;
}

void G4Trajectory::AppendStep(const G4Step* aStep)
{
   positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
                                 GetPosition() ));
}
  
G4ParticleDefinition* G4Trajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

void G4Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
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



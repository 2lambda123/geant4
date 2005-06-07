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
// $Id: RE01TrackerHit.cc,v 1.3 2005-06-07 10:53:17 perl Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//


#include "RE01TrackerHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"


G4Allocator<RE01TrackerHit> RE01TrackerHitAllocator;

RE01TrackerHit::RE01TrackerHit()
{;}

RE01TrackerHit::~RE01TrackerHit()
{;}

RE01TrackerHit::RE01TrackerHit(const RE01TrackerHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  trackID = right.trackID;
}

const RE01TrackerHit& RE01TrackerHit::operator=(const RE01TrackerHit &right)
{
  edep = right.edep;
  pos = right.pos;
  trackID = right.trackID;
  return *this;
}

G4int RE01TrackerHit::operator==(const RE01TrackerHit &right) const
{
  return (this==&right) ? 1 : 0;
}

void RE01TrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

const std::map<G4String,G4AttDef>* RE01TrackerHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("RE01TrackerHit",isNew);
  if (isNew) {
    G4String HitType("HitType");
    (*store)[HitType] = G4AttDef(HitType,"Hit Type","Bookkeeping","","G4String");

    G4String TrackID("TrackID");
    (*store)[TrackID] = G4AttDef(TrackID,"Track ID","Bookkeeping","","G4int");

    G4String ZCellID("ZCellID");
    (*store)[ZCellID] = G4AttDef(ZCellID,"Z Cell ID","Bookkeeping","","G4int");

    G4String PhiCellID("PhiCellID");
    (*store)[PhiCellID] = G4AttDef(PhiCellID,"Phi Cell ID","Bookkeeping","","G4int");

    G4String Energy("Energy");
    (*store)[Energy] = G4AttDef(Energy,"Energy Deposited","Physics","G4BestUnit","G4double");

    G4String ETrack("ETrack");
    (*store)[ETrack] = G4AttDef(ETrack,"Energy Deposited By Track","Physics","G4BestUnit","G4double");

    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position",
		      "Physics","G4BestUnit","G4ThreeVector");

    G4String LVol("LVol");
    (*store)[LVol] = G4AttDef(LVol,"Logical Volume","Bookkeeping","","G4String");
  }
  return store;
}

std::vector<G4AttValue>* RE01TrackerHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","RE01TrackerHit",""));

  values->push_back
    (G4AttValue("TrackID",G4UIcommand::ConvertToString(trackID),""));

  values->push_back
    (G4AttValue("ZCellID"," ",""));

  values->push_back
    (G4AttValue("PhiCellID"," ",""));

  values->push_back
    (G4AttValue("Energy",G4BestUnit(edep,"Energy"),""));

  G4double noEnergy = 0.*MeV;
  values->push_back
    (G4AttValue("ETrack",G4BestUnit(noEnergy,"Energy"),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(pos,"Length"),""));

  values->push_back
    (G4AttValue("LVol"," ",""));
  
   return values;
}

void RE01TrackerHit::Print()
{
  G4cout << "TrackID " << trackID << "   Position " << pos << "       : " << edep/keV << " [keV]" << G4endl;
}



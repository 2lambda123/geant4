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

#include "ExN04MuonHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


G4Allocator<ExN04MuonHit> ExN04MuonHitAllocator;

ExN04MuonHit::ExN04MuonHit()
{;}

ExN04MuonHit::~ExN04MuonHit()
{;}

ExN04MuonHit::ExN04MuonHit(const ExN04MuonHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
}

const ExN04MuonHit& ExN04MuonHit::operator=(const ExN04MuonHit &right)
{
  edep = right.edep;
  pos = right.pos;
  return *this;
}

G4int ExN04MuonHit::operator==(const ExN04MuonHit &right) const
{
  return (this==&right) ? 1 : 0;
}

void ExN04MuonHit::Draw()
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

void ExN04MuonHit::Print()
{;}



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
// $Id: Tst52TrackerHit.cc,v 1.1 2007-04-12 12:00:17 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Susanna Guatelli (guatelli@ge.infn.it)
//

#include "Tst52TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<Tst52TrackerHit> Tst52TrackerHitAllocator;

Tst52TrackerHit::Tst52TrackerHit() {}


Tst52TrackerHit::~Tst52TrackerHit() {}


Tst52TrackerHit::Tst52TrackerHit(const Tst52TrackerHit& right)
  : G4VHit()
{
  edep      = right.edep;
  voxel_hit = right.voxel_hit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const Tst52TrackerHit& Tst52TrackerHit::operator=(const Tst52TrackerHit& right)
{
  edep      = right.edep;
  voxel_hit = right.voxel_hit;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int Tst52TrackerHit::operator==(const Tst52TrackerHit& right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Tst52TrackerHit::Draw()
{
//   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
//   if(pVVisManager)
//   {
//     G4Circle circle(pos);
//     circle.SetScreenSize(2.);
//     circle.SetFillStyle(G4Circle::filled);
//     G4Colour colour(1.,0.,0.);
//     G4VisAttributes attribs(colour);
//     circle.SetVisAttributes(attribs);
//     pVVisManager->Draw(circle);
//   }
}

void Tst52TrackerHit::Print()
{
  G4cout << "  energy deposit: " << edep/MeV
	 << "  voxel: " << voxel_hit << G4endl;
}


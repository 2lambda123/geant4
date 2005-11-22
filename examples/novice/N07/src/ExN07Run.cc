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
// $Id: ExN07Run.cc,v 1.4 2005-11-22 22:20:55 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "ExN07Run.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

ExN07Run::ExN07Run()
{
  G4String detName[6] = {"Calor-A_abs","Calor-A_gap","Calor-B_abs","Calor-B_gap","Calor-C_abs","Calor-C_gap"};
  G4String primNameSum[6] = {"eDep","nGamma","nElectron","nPositron","trackLength","nStep"};
  G4String primNameMin[3] = {"minEkinGamma","minEkinElectron","minEkinPositron"};

  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  G4String fullName;
  for(size_t i=0;i<6;i++)
  {
    for(size_t j=0;j<6;j++)
    {
      fullName = detName[i]+"/"+primNameSum[j];
      colIDSum[i][j] = SDMan->GetCollectionID(fullName);
    }
    for(size_t k=0;k<3;k++)
    {
      fullName = detName[i]+"/"+primNameMin[k];
      colIDMin[i][k] = SDMan->GetCollectionID(fullName);
    }
  }
}

ExN07Run::~ExN07Run()
{;}

void ExN07Run::RecordEvent(const G4Event* evt)
{
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if(!HCE) return;
  numberOfEvent++;
  for(size_t i=0;i<6;i++)
  {
    for(size_t j=0;j<6;j++)
    {
      G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(colIDSum[i][j]));
      mapSum[i][j] += *evtMap;
    }
    for(size_t k=0;k<3;k++)
    {
      G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(colIDMin[i][k]));
      std::map<G4int,G4double*>::iterator itr = evtMap->GetMap()->begin();
      for(; itr != evtMap->GetMap()->end(); itr++)
      {
        G4int key = (itr->first);
        G4double val = *(itr->second);
        G4double* mapP = mapMin[i][k][key];
        if( mapP && (val>*mapP) ) continue;
        mapMin[i][k].set(key,val);
      }
    }
  }
}

G4double ExN07Run::GetTotal(const G4THitsMap<G4double> &map) const
{
  G4double tot = 0.;
  std::map<G4int,G4double*>::iterator itr = map.GetMap()->begin();
  for(; itr != map.GetMap()->end(); itr++) 
  { tot += *(itr->second); }
  return tot;
}

G4double ExN07Run::FindMinimum(const G4THitsMap<G4double> &map) const
{
  G4double val = DBL_MAX;
  std::map<G4int,G4double*>::iterator itr = map.GetMap()->begin();
  for(; itr != map.GetMap()->end(); itr++) 
  { if(val>*(itr->second)) val = *(itr->second); }
  return val;
}


  

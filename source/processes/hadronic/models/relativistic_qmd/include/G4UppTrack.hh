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

#ifndef G4UPPTRACK_H
#define G4UPPTRACK_H


#include "G4KineticTrack.hh"
#include "G4Nucleon.hh"
#include "G4VKineticNucleon.hh"
#include "G4LorentzVector.hh"




class G4UppTrack : public G4KineticTrack
{
public:

  G4UppTrack() 
    : numberOfCollisions(0), changedFlag(true), nonInteractionGroup(0) {}

  G4UppTrack(const G4KineticTrack& aTrack) 
    : G4KineticTrack(aTrack), numberOfCollisions(0), 
      changedFlag(true), nonInteractionGroup(0), localTime(0) {};

  G4UppTrack(const G4Nucleon& aNucleon, const G4int g=0) :
    numberOfCollisions(0), changedFlag(true), 
    nonInteractionGroup(g), localTime(0)
    { SetDefinition(aNucleon.GetDefinition());
      SetPosition(aNucleon.GetPosition());
      Set4Momentum(aNucleon.Get4Momentum());  };

  void setChanged(G4bool newFlag) 
    { changedFlag = newFlag; } 
  G4bool hasChanged() const 
    { return changedFlag; }
  
  void setNonInteractionGroup(G4int groupId) 
    { nonInteractionGroup=groupId; }
  G4int getNonInteractionGroup() const 
    { return nonInteractionGroup; }

  void setNumberOfCollisions(G4int n) 
    { numberOfCollisions=n; }
  G4int getNumberOfCollisions() const 
    { return numberOfCollisions; }

  void addLastInteractionPartner(const G4UppTrack* particlePtr) 
    { lastInteractionPartners.push_back(particlePtr); }
  G4bool isLastInteractionPartner(const G4UppTrack* particlePtr) const;
  void clearLastInteractionPartners() 
    { lastInteractionPartners.clear(); }

  G4LorentzVector Get4Position() const
     { return G4LorentzVector(GetPosition(),localTime); }
  void Set4Position(G4LorentzVector& a4Pos) 
     { SetPosition(a4Pos.vect()); localTime = a4Pos.t(); }
  void setLocalTime(const G4double newLocalTime) { localTime = newLocalTime; }

  void dump() const;

private:

  G4double localTime;
  G4int numberOfCollisions;
  G4bool changedFlag;
  vector<const G4UppTrack*> lastInteractionPartners;
  G4int nonInteractionGroup;

};


#endif // G4UPPTRACK_H



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

#include "G4UppActionCollision.hh"


G4UppActionCollision::G4UppActionCollision(const G4double collisionTime,
					   const G4UppTrackVector& allTracks,
					   const G4UppTrackVector& collidingParticles,
					   G4VScatterer& aScatterer)
  : collPart(collidingParticles)
{
  setActionTime(collisionTime);
  allTracksPtr = &allTracks;
  scattererPtr = &aScatterer;
}


G4UppTrackChange* G4UppActionCollision::perform(const G4UppTrackVector& allTracks) const
{
  G4cout << "(debug) performing collison (";
  for (G4int j=0; j<collPart.size(); j++) {
    G4cout << collPart[j]->GetDefinition()->GetParticleName() << " ";
  }
  G4cout << ")" << G4endl;
  // if (collPart[1]->GetDefinition()->GetParticleName()=="pi-") 
  
  G4int arraySize = collPart.size();
  assert(arraySize==2);

  G4KineticTrackVector* newPart = scattererPtr->Scatter(*collPart[0],*collPart[1]);
  if (newPart==NULL) {
    G4cout << "(debug) no collision" << G4endl;
    return NULL;
  }

  for (G4int i=0; i<arraySize; i++) {
    collPart[i]->clearLastInteractionPartners();
    collPart[i]->setChanged(true);
    for (G4int j=0; j<arraySize; j++) {
      collPart[i]->addLastInteractionPartner(collPart[j]);
    }
  }
  // G4KineticTrack* testpart=collPart[0];
  // collPart[0]->GetDefinition()->GetParticleName();
  // testpart->GetDefinition()->GetParticleName();

  G4UppTrackChange* theChange = new G4UppTrackChange;
  theChange->oldParticles = collPart;
  assert(newPart->size()>0);
  for (G4int k=0; k<newPart->size(); k++) {
    G4UppTrack* TrackPtr = new G4UppTrack(*(*newPart)[k]);
    TrackPtr->setLocalTime(allTracks.getGlobalTime());
    theChange->newParticles.push_back(TrackPtr);
  }

  return theChange;
}


G4bool G4UppActionCollision::isValid() const
{
  for (G4int i=0; i<collPart.size(); i++) 
    if (collPart[i]->hasChanged()) return false;
  return true;
}



void G4UppActionCollision::dump() const
{
  G4cout << "Action: COLL ( ";
  for (G4int i=0; i<collPart.size(); i++) {
    G4int part_index = allTracksPtr->getIndex(collPart[i]);
    G4cout << part_index << ",";
    G4cout << collPart[i]->GetDefinition()->GetParticleName() << ",";
    G4cout << collPart[i]->GetActualMass() << " ";
  }
  G4cout << ") at " << getActionTime()*c_light/fermi << G4endl;
}



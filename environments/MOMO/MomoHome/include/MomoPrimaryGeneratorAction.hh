// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// 1998 Dec 10, updated for geant4.0.0, Hajime Yoshida
//  2002 March Geant4.4.0

#ifndef MomoPrimaryGeneratorAction_h
#define MomoPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class MomoPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MomoPrimaryGeneratorAction();
    ~MomoPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* particleGun;
};

#endif



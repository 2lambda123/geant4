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
// $Id: G4PrimaryTransformer.hh,v 1.7 2004-07-07 15:01:15 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4PromaryTransformer_h 
#define G4PromaryTransformer_h 1

#include "G4TrackVector.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"
class G4Event;
class G4PrimaryVertex;
#include "G4PrimaryParticle.hh"

// class description:
//
//  This class is exclusively used by G4EventManager for the conversion
// from G4PrimaryVertex/G4PrimaryParticle to G4DynamicParticle/G4Track.
//

class G4PrimaryTransformer
{
  public:
    G4PrimaryTransformer();
    ~G4PrimaryTransformer();
    
    G4TrackVector* GimmePrimaries(G4Event* anEvent, G4int trackIDCounter=0);

  private:
    G4TrackVector TV;
    G4ParticleTable* particleTable;
    G4int verboseLevel;
    G4int trackID;

    G4bool unknownParticleDefined;
    G4ParticleDefinition* unknown;

  public:
    inline void SetVerboseLevel(G4int vl)
    { verboseLevel = vl; };

  private:
    void GenerateTracks(G4PrimaryVertex* primaryVertex);
    void GenerateSingleTrack(G4PrimaryParticle* primaryParticle,
              G4double x0,G4double y0,G4double z0,G4double t0,G4double wv);
    void SetDecayProducts(G4PrimaryParticle* mother,
                            G4DynamicParticle* motherDP);
    inline G4ParticleDefinition* GetDefinition(G4PrimaryParticle*pp)
    { 
      G4ParticleDefinition* partDef = pp->GetG4code();
      if(!partDef) partDef = particleTable->FindParticle(pp->GetPDGcode());
      return partDef;
    };
};

#endif



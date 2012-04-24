//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// INCL++ intra-nuclear cascade model
// Pekka Kaitaniemi, CEA and Helsinki Institute of Physics
// Davide Mancusi, CEA
// Alain Boudard, CEA
// Sylvie Leray, CEA
// Joseph Cugnon, University of Liege
//
// INCL++ revision: v5.0.5
//
#define INCLXX_IN_GEANT4_MODE 1

#include "globals.hh"

#include "G4INCLParticle.hh"
#include "G4INCLIChannel.hh"
#include "G4INCLNucleus.hh"

#ifndef REFLECTIONCHANNEL_HH_
#define REFLECTIONCHANNEL_HH_
namespace G4INCL {
  class FinalState;

  class ReflectionChannel : public IChannel {
  public:
    ReflectionChannel(Nucleus *n, Particle *p);
    virtual ~ReflectionChannel();

    FinalState* getFinalState();

  private:
    Nucleus *theNucleus;
    Particle *theParticle;
  };
}
#endif //REFLECTIONCHANNEL_HH_

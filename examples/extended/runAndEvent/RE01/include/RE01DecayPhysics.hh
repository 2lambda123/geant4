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
// $Id: RE01DecayPhysics.hh,v 1.1 2004-11-26 07:37:39 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

#ifndef RE01DecayPhysics_h
#define RE01DecayPhysics_h 1

// #include "globals.hh"
// #include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"


#include "G4Decay.hh"
#include "G4UnknownDecay.hh"

class RE01DecayPhysics : public G4VPhysicsConstructor
{
  public: 
    RE01DecayPhysics(const G4String& name = "decay");
    virtual ~RE01DecayPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();

    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type 
    virtual void ConstructProcess();

  protected:
    G4Decay fDecayProcess;
    G4UnknownDecay fUnknownDecay;
};


#endif









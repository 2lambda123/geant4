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
//    ***********************
//    *                     *
//    *    RemSimDecay.cc   *
//    *                     *          
//    ***********************
//
// $Id: RemSimDecay.cc,v 1.2 2004-05-22 12:57:06 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author:Susanna Guatelli, guatelli@ge.infn.it 
//

#include "RemSimDecay.hh"
#include "G4ProcessManager.hh"
#include "globals.hh"
#include "G4ios.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

RemSimDecay::RemSimDecay(const G4String& name): G4VPhysicsConstructor(name)
{ }

RemSimDecay::~RemSimDecay()
{ }

void RemSimDecay::ConstructProcess()
{
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator -> reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      
      if (theDecayProcess -> IsApplicable(*particle)) 
	{ 
	  pmanager -> AddProcess(theDecayProcess);
	  // set ordering for PostStepDoIt and AtRestDoIt
	  pmanager -> SetProcessOrdering(theDecayProcess, idxPostStep);
	  pmanager -> SetProcessOrdering(theDecayProcess, idxAtRest);
	}
    }
}

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
// File name:     RadmonPhysicsSteppingAction.cc
// Creation date: Nov 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonPhysicsSteppingAction.cc,v 1.2 2006-06-28 13:56:43 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//

// Include files
#include "RadmonPhysicsSteppingAction.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

void                                            RadmonPhysicsSteppingAction :: OnUserStepping(const G4Step * step)
{
 G4Track * track(step->GetTrack());
 G4ParticleDefinition * particle(track->GetDefinition());
 G4ProcessManager * manager(particle->GetProcessManager());
 
 if (manager->GetProcessListLength()==0)
  track->SetTrackStatus(fStopAndKill);
}


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
// ====================================================================
//
//   ExN04PrimaryGeneratorAction.cc
//   $Id: ExN04PrimaryGeneratorAction.cc,v 1.3 2003-12-09 15:30:47 gunter Exp $
//
// ====================================================================
#include "ExN04PrimaryGeneratorAction.hh"
#include "ExN04PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4PythiaInterface.hh"

//////////////////////////////////////////////////////////
ExN04PrimaryGeneratorAction::ExN04PrimaryGeneratorAction()
//////////////////////////////////////////////////////////
{
  // default generator is particle gun.
  currentGenerator= particleGun= new G4ParticleGun();
  currentGeneratorName= "particleGun";
  hepmcAscii= new HepMCG4AsciiReader();
  pythiaGen= new HepMCG4PythiaInterface();

  gentypeMap["particleGun"]= particleGun;
  gentypeMap["hepmcAscii"]= hepmcAscii;
  gentypeMap["pythia"]= pythiaGen;

  messenger= new ExN04PrimaryGeneratorMessenger(this);  
}

///////////////////////////////////////////////////////////
ExN04PrimaryGeneratorAction::~ExN04PrimaryGeneratorAction()
///////////////////////////////////////////////////////////
{
  delete messenger;
}

/////////////////////////////////////////////////////////////////////
void ExN04PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
/////////////////////////////////////////////////////////////////////
{
  if(currentGenerator)
    currentGenerator-> GeneratePrimaryVertex(anEvent);
  else 
    G4Exception("generator is not instanciated.");
}


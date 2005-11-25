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
// $Id: EM_GNPhysics.cc,v 1.2 2005-11-25 15:38:50 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   EMPhysics
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 09.11.2005 G.Folger: standard EM is now seperate
//
//----------------------------------------------------------------------------
//
#include "EM_GNPhysics.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"


EM_GNPhysics::
EM_GNPhysics(const G4String& name) : G4VPhysicsConstructor(name) {}

EM_GNPhysics::
~EM_GNPhysics() {}

void EM_GNPhysics::
ConstructParticle()
{
  theEMStandardPhysics.ConstructParticle();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}

void EM_GNPhysics::
ConstructProcess()
{
  theEMStandardPhysics.ConstructProcess();
  theEMPhysics.Build();
  #ifndef NO_ELECTRO_AND_GAMMA_NUCLEAR
  theGNPhysics.Build();
  #endif
}



// 2002 by J.P. Wellisch

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
// $Id: Tst50Particles.hh,v 1.2 2003-05-17 18:11:52 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// History:
// -----------
// 22 Feb 2003 MGP          Created
//
// -------------------------------------------------------------------

// Class description:
// System test for e/gamma, particles for PhysicsList
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef TST50PARTICLES_HH
#define TST50PARTICLES_HH 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class Tst50Particles : public G4VPhysicsConstructor {
public: 

  Tst50Particles(const G4String& name = "particles");
  
  virtual ~Tst50Particles();
  
  virtual void ConstructParticle();
  
  // This method is dummy
  virtual void ConstructProcess() {};
  
};

#endif









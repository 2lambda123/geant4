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
// $Id: RemSimParticles.hh,v 1.4 2004-05-22 12:57:04 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author:Susanna Guatelli, guatelli@ge.infn.it 
//

#ifndef REMSIMPARTICLES_HH
#define REMSIMPARTICLES_HH 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class RemSimParticles : public G4VPhysicsConstructor {
public: 

  RemSimParticles(const G4String& name = "particles");
  
  virtual ~RemSimParticles();
  
  virtual void ConstructParticle();
  
  // This method is dummy
  virtual void ConstructProcess() {};  
};
#endif









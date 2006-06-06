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
// $Id: G4LHEPStoppingPhysics.hh,v 1.2 2006-06-06 16:47:44 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4LHEPStoppingPhysics
//
// Author: 30 April 2006 V. Ivanchenko
//
// Modified:
//
//----------------------------------------------------------------------------
//

#ifndef G4LHEPStoppingPhysics_h
#define G4LHEPStoppingPhysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class G4MuonMinusCaptureAtRest;
class G4PionMinusAbsorptionAtRest;
class G4KaonMinusAbsorption;
class G4AntiProtonAnnihilationAtRest;
class G4AntiNeutronAnnihilationAtRest;

class G4LHEPStoppingPhysics : public G4VPhysicsConstructor
{
public: 
  G4LHEPStoppingPhysics(const G4String& name = "stopping",
			G4int ver = 1);
  virtual ~G4LHEPStoppingPhysics();

public: 
  // This method will be invoked in the Construct() method. 
  // each particle type will be instantiated
  virtual void ConstructParticle();
 
  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type 
  virtual void ConstructProcess();

private:

  G4MuonMinusCaptureAtRest* muProcess;
  G4PionMinusAbsorptionAtRest* piProcess;
  G4KaonMinusAbsorption* kProcess;
  G4AntiProtonAnnihilationAtRest* apProcess;
  G4AntiNeutronAnnihilationAtRest* anProcess;

  G4int    verbose;
  G4bool   wasActivated;
};

#endif









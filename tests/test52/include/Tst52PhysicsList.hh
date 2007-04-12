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
// $Id: Tst52PhysicsList.hh,v 1.2 2007-04-12 14:51:52 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: guatelli@ge.infn.it
//
//

// Class description:
// System test for e/gamma, standard photon processes for PhysicsList
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef TST52PHYSICSLIST_HH
#define TST52PHYSICSLIST_HH 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class Tst52PhysicsListMessenger;
class Tst52ElectronStandard;
class Tst52ElectronEEDL;
class Tst52ElectronPenelope;
class Tst52PositronStandard;
class Tst52PositronPenelope;
class Tst52PhotonEPDL;

class Tst52PhysicsList: public G4VModularPhysicsList {
public:
  
  Tst52PhysicsList();

  virtual ~Tst52PhysicsList();

  virtual void SetCuts();
  
  // Register PhysicsList chunks
  void AddPhysicsList(const G4String& name);

  // Production thresholds, expressed in range
  void SetParticleCut(G4double value);

  // Change the value of facrange
  void SetFacRange(G4double value);


private:
  G4bool electronIsRegistered;
  G4bool positronIsRegistered;
  G4bool photonIsRegistered;
  G4double cutForGamma;
  G4double cutForElectron;

  Tst52PhysicsListMessenger* messenger; 

  Tst52ElectronStandard* electron_physics_standard; 
  Tst52ElectronEEDL* electron_physics_eedl;
  Tst52ElectronPenelope* electron_physics_penelope;

  Tst52PositronStandard* positron_physics_standard;

  Tst52PositronPenelope* positron_physics_penelope;

  Tst52PhotonEPDL* photon_physics_epdl;

  G4String electron_value;
  G4String positron_value;
};

#endif








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
#ifndef DummyModel_hh
#define DummyModel_hh

#include "G4VFastSimulationModel.hh"

class DummyModel : public G4VFastSimulationModel {
public:
  DummyModel(const G4String& aName, G4LogicalVolume* vol) : G4VFastSimulationModel(aName, vol) {}

  ~DummyModel() {}
  
  G4bool IsApplicable(const G4ParticleDefinition&) {return true;}
  G4bool ModelTrigger(const G4FastTrack &) {return true;}
  void DoIt(const G4FastTrack&, G4FastStep&) {}

};

#endif

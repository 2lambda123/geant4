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
// $Id: Em10PhysicsListMessenger.hh,v 1.3 2007-05-31 18:23:42 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Em10PhysicsListMessenger_h
#define Em10PhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class Em10PhysicsList;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Em10PhysicsListMessenger: public G4UImessenger
{
  public:
    Em10PhysicsListMessenger(Em10PhysicsList*);
   ~Em10PhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    Em10PhysicsList*          Em10List;

    G4UIcmdWithADoubleAndUnit* setMaxStepCmd;

    G4UIcmdWithADoubleAndUnit* cutGCmd;
    G4UIcmdWithADoubleAndUnit* cutECmd;
    G4UIcmdWithADoubleAndUnit* cutPCmd;
    G4UIcmdWithADoubleAndUnit* rCmd;
    G4UIcmdWithADoubleAndUnit* eCmd;
    G4UIcmdWithADoubleAndUnit* eMinEnergyCmd;
    G4UIcmdWithADoubleAndUnit* gMinEnergyCmd;

    G4UIcmdWithADoubleAndUnit* ElectronCutCmd;
    G4UIcmdWithADoubleAndUnit* PositronCutCmd;
    G4UIcmdWithADoubleAndUnit* GammaCutCmd;

    G4UIcmdWithADoubleAndUnit* RadiatorCutCmd;
    G4UIcmdWithADoubleAndUnit* DetectorCutCmd;
    G4UIcmdWithAString*        XTRModelCmd;
};

#endif


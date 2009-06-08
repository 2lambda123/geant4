//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: SteppingAction.hh,v 1.3 2009-06-08 12:58:13 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class DetectorConstruction;
class RunAction;
class PrimaryGeneratorAction;
class EventAction;
class HistoManager;

class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(DetectorConstruction*, RunAction*, PrimaryGeneratorAction*,
                   EventAction*, HistoManager*);
   ~SteppingAction();

    void UserSteppingAction(const G4Step*);
    
    G4double BirksAttenuation(const G4Step*);
    
  private:
    DetectorConstruction* detector;
    RunAction*              runAct;
    PrimaryGeneratorAction* primary;        
    EventAction*            eventAct;
    HistoManager*           histoManager;
    
    G4bool           first;
    G4LogicalVolume* lvol_world;
    G4LogicalVolume* lvol_slayer;
    G4LogicalVolume* lvol_layer;
    G4LogicalVolume* lvol_fiber;

    G4double calorThickness, calorSizeYZ, superLayerThick;
    G4double dxPixel, dyPixel;
    G4int    nyPixelsMax;  
    
    G4bool trigger;
    G4double rmax, seuil;            
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

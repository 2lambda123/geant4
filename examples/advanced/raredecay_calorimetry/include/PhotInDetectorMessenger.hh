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
//
// $Id: PhotInDetectorMessenger.hh,v 1.5 2006-06-29 16:24:43 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef PhotInDetectorMessenger_h
#define PhotInDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "PhotInDetectorConstruction.hh"
#include "PhotInPrimaryGeneratorAction.hh"
#include "PhotInEventAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Material.hh"
#include "G4ParticleTable.hh"

class PhotInDetectorMessenger: public G4UImessenger
{
  public:
    PhotInDetectorMessenger(PhotInDetectorConstruction* detector,
                             PhotInPrimaryGeneratorAction* generator);
    virtual ~PhotInDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    virtual G4String GetCurrentValue(G4UIcommand * command);
    
  private:
    PhotInDetectorConstruction* PhotInDetector;
    PhotInPrimaryGeneratorAction* PhotInGenerator;
    
    G4UIdirectory*             PhotInDir;
    G4UIcmdWithAString*        setPartCmd;
    G4UIcmdWithADouble*        setEnergyCmd;
    G4UIcmdWithAString*        AddMaterCmd;
    G4UIcmdWithAString*        AbsMaterCmd;
    G4UIcmdWithAString*        GapMaterCmd;
    G4UIcmdWithAnInteger*      numLayerCmd;
    G4UIcmdWithAnInteger*      numSlabsCmd;
    G4UIcmdWithABool*          SerialCmd;
    G4UIcmdWithAnInteger*      verboseCmd;
};


#endif


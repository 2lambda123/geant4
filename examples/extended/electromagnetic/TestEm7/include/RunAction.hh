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
// $Id: RunAction.hh,v 1.15 2010-09-17 18:45:43 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class DetectorConstruction;
class PhysicsList;
class HistoManager;
class PrimaryGeneratorAction;

class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
public:
  RunAction(DetectorConstruction*, PhysicsList*, HistoManager*,
	    PrimaryGeneratorAction*);
  virtual ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void FillTallyEdep(G4int n, G4double e)  {fTallyEdep[n] += e;};
  void FillEdep(G4double de, G4double eni) {fEdeptot += de; fEniel += eni;};
    
  void AddProjRange (G4double x) 
  {fProjRange += x; fProjRange2 += x*x; fRange++;};
  void AddPrimaryStep() {fNbPrimarySteps++;};
                   
private:  
    
  DetectorConstruction*   fDetector;
  PhysicsList*            fPhysics;
  HistoManager*           fHistoManager;
  PrimaryGeneratorAction* fKinematic;
  G4double*               fTallyEdep;   
  G4double                fProjRange, fProjRange2;
  G4double                fEdeptot, fEniel;
  G4int                   fNbPrimarySteps;
  G4int                   fRange;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


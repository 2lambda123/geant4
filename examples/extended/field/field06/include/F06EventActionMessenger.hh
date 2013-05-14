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
// $Id$
//
/// \file field/field06/include/F06EventActionMessenger.hh
/// \brief Definition of the F06EventActionMessenger class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef F06EventActionMessenger_h
#define F06EventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class F06EventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class F06EventActionMessenger: public G4UImessenger
{
  public:

    F06EventActionMessenger(F06EventAction*);
    virtual ~F06EventActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:

    F06EventAction* fEventAction;   

    G4UIcmdWithAnInteger* fSetVerboseCmd;
    G4UIcmdWithAString*   fDrawCmd;
    G4UIcmdWithAnInteger* fPrintCmd;
};

#endif

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
// $Id: RunActionMessenger.hh,v 1.3 2006-01-13 14:20:27 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger: public G4UImessenger
{
  public:
    RunActionMessenger(RunAction*);
   ~RunActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    RunAction*          Run;
    
    G4UIdirectory*        runDir;
    G4UIcmdWith3Vector*   accCmd;
    G4UIcmdWithAnInteger* verbCmd;
            
    G4UIdirectory*        histoDir;        
    G4UIcmdWithAString*   factoryCmd;
    G4UIcmdWithAString*   typeCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

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
// $Id: Tst38DetectorMessenger.hh,v 1.1 2006-05-02 10:27:31 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef Tst38DetectorMessenger_h
#define Tst38DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "Tst38DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "G4ios.hh"

class Tst38DetectorConstruction;
//class G4UIdirectory;
//class G4UIcmdWithAString;

class Tst38DetectorMessenger: public G4UImessenger
{
  public:
    Tst38DetectorMessenger(Tst38DetectorConstruction* myDC);
    void SetNewValue(G4UIcommand* command,G4String newValues);
  private:
    Tst38DetectorConstruction * myDetector;
    G4UIdirectory *      mydetDir;
    G4UIcmdWithAString * selMatCmd;

};

#endif


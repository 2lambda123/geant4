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
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//      For information related to this code contact: Alex Howard
//      e-mail: alexander.howard@cern.ch
// --------------------------------------------------------------
// Comments
//
//                  Underground Advanced
//               by A. Howard and H. Araujo 
//                    (27th November 2001)
//
// StackingActionMessenger header
// --------------------------------------------------------------

#ifndef DMXStackingActionMessenger_h
#define DMXStackingActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DMXStackingAction;
class G4UIcmdWithABool;


class DMXStackingActionMessenger: public G4UImessenger {

  public:
    DMXStackingActionMessenger(DMXStackingAction*);
    ~DMXStackingActionMessenger();
    
  public:
    void SetNewValue(G4UIcommand* , G4String);

  private:
    DMXStackingAction* DMXAction;

  private:
    G4UIcmdWithABool* KillGammasCmd;
    
};

#endif


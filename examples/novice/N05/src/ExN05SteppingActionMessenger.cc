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
// $Id: ExN05SteppingActionMessenger.cc,v 1.4 2002-01-09 17:24:20 ranjard Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "ExN05SteppingActionMessenger.hh"
#include "ExN05SteppingAction.hh"

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

ExN05SteppingActionMessenger::ExN05SteppingActionMessenger(ExN05SteppingAction* SA)
:SteppingAction(SA)
{
  stepDirectory = new G4UIdirectory("/step/");
  stepDirectory->SetGuidance("Step draw control command.");

  drawStepCmd = new G4UIcmdWithABool("/step/draw",this);
  drawStepCmd->SetGuidance("Draw each step on the fly. (default = true)");
  drawStepCmd->SetParameterName("draw", true);
  drawStepCmd->SetDefaultValue(true);
}

void ExN05SteppingActionMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if( command->GetCommandName() == "draw" )
  {
    G4int vl;
    const char* t = newValues;
    G4std::istrstream is((char*)t);
    is >> vl;
    SteppingAction->SetDrawFlag(vl!=0);
  }
}


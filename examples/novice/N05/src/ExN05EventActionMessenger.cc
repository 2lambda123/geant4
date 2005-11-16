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
// $Id: ExN05EventActionMessenger.cc,v 1.6 2005-11-16 08:33:26 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "ExN05EventActionMessenger.hh"

#include <sstream>

#include "ExN05EventAction.hh"
#include "G4UIcmdWithABool.hh"

ExN05EventActionMessenger::ExN05EventActionMessenger(ExN05EventAction* EA)
:EventAction(EA)
{
  // uses "/event" directory already
  // defined in G4EvManMessenger
  drawEventCmd = new G4UIcmdWithABool("/event/Draw",this);
  drawEventCmd->SetGuidance("Set drawFlag to Draw an event. (default = false)");
  drawEventCmd->SetParameterName("Draw", false);
  drawEventCmd->SetDefaultValue(false);
}

void ExN05EventActionMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if( command->GetCommandName() == "Draw" )
  {
    G4int vl;
    const char* t = newValues;
    std::istringstream is(t);
    is >> vl;
    EventAction->SetDrawFlag(vl!=0);
  }
}


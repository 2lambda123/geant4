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
// $Id: G4Logo.cc,v 1.1 2005-03-03 16:45:34 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "globals.hh"
#include "G4VisExecutive.hh"
#include "G4VisExtent.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4LogoVisAction.hh"

int main() {

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize ();

  visManager->SetUserAction(new G4LogoVisAction);

  G4UImanager* UI = G4UImanager::GetUIpointer ();
  UI->ApplyCommand ("/control/execute G4Logo.g4m");

  G4UIsession* session = new G4UIterminal(new G4UItcsh);
  session->SessionStart();

  delete session;
  delete visManager;
}

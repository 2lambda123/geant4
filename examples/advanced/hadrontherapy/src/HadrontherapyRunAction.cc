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
// $Id: HadrontherapyRunAction.cc,v 3.0, September 2004;
// -----------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// -----------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone, F. Di Rosa, G. Russo
// Laboratori Nazionali del Sud - INFN, Catania, Italy
//
// ----------------------------------------------------------

#include "HadrontherapyRunAction.hh"
#include "HadrontherapyEventAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "HadrontherapyDetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4Timer.hh"
#include "HadrontherapyRunAction.hh"

HadrontherapyRunAction::HadrontherapyRunAction()
{
}

HadrontherapyRunAction::~HadrontherapyRunAction()
{ 
}

void HadrontherapyRunAction::BeginOfRunAction(const G4Run*)
{ 	
   G4RunManager::GetRunManager()->SetRandomNumberStore(true);
}

void HadrontherapyRunAction::EndOfRunAction(const G4Run*)
{
}




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
// $Id: G4UserRunAction.cc,v 1.6 2003-05-21 21:06:01 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "G4UserRunAction.hh"

G4UserRunAction::G4UserRunAction()
{;}

G4UserRunAction::~G4UserRunAction()
{;}

G4Run* G4UserRunAction::GenerateRun()
{ return 0; }

void G4UserRunAction::BeginOfRunAction(const G4Run*)
{;}

void G4UserRunAction::EndOfRunAction(const G4Run*)
{;}


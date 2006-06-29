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
//
// $Id: PhotInRunAction.hh,v 1.3 2006-06-29 16:24:57 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef PhotInRunAction_h
#define PhotInRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "PhotInRun.hh"
#include "PhotInConstants.hh"

#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ios.hh"
#include "G4Run.hh"
#include "G4UnitsTable.hh"

class PhotInRunAction : public G4UserRunAction
{
  public:
    PhotInRunAction();
   ~PhotInRunAction();

  public:
    G4Run* GenerateRun();
    void EndOfRunAction(const G4Run*);
};

#endif


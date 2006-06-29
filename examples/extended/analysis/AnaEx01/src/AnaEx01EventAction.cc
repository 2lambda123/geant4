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
// $Id: AnaEx01EventAction.cc,v 1.6 2006-06-29 16:33:53 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#ifdef G4ANALYSIS_USE
#include "AnaEx01AnalysisManager.hh"
#endif

#include "AnaEx01EventAction.hh"

AnaEx01EventAction::AnaEx01EventAction(
 AnaEx01AnalysisManager* aAnalysisManager
):fAnalysisManager(aAnalysisManager){}

AnaEx01EventAction::~AnaEx01EventAction(){}

void AnaEx01EventAction::BeginOfEventAction(const G4Event* aEvent){
#ifdef G4ANALYSIS_USE
  if(fAnalysisManager) fAnalysisManager->BeginOfEvent(aEvent);
#endif
}

void AnaEx01EventAction::EndOfEventAction(const G4Event* aEvent) {
#ifdef G4ANALYSIS_USE
  if(fAnalysisManager) fAnalysisManager->EndOfEvent(aEvent);
#endif
}


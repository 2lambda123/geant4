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
// **********************************************************************
// *                                                                    *
// *                    GEANT 4 xray_telescope advanced example         *
// *                                                                    *
// * MODULE:            XrayTelSteppingAction.cc                        *
// * -------                                                            *
// *                                                                    *
// * Version:           0.4                                             *
// * Date:              06/11/00                                        *
// * Author:            R Nartallo                                      *
// * Organisation:      ESA/ESTEC, Noordwijk, THe Netherlands           *
// *                                                                    *
// **********************************************************************
// 
// CHANGE HISTORY
// --------------
//
// 30.11.2000 R. Nartallo
// - Add pre-processor directives to compile without analysis option
//
// 16.11.2000 A. Pfeiffer
// - Implementation of analysis manager call
//
// 06.11.2000 R.Nartallo
// - First implementation of xray_telescope Physics list
// - Based on Chandra and XMM models
// 
//
// **********************************************************************

#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"

#include "globals.hh"

#include <assert.h>
#include "g4std/fstream"
#include "g4std/iomanip"
#include "g4std/iostream"
#include "g4std/vector"

#include "XrayTelSteppingAction.hh"
#include "XrayTelAnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XrayTelSteppingAction::XrayTelSteppingAction(
					     G4std::vector<G4double*>* enEnergy, 
					     G4std::vector<G4ThreeVector*>* enDirect,
					     G4bool* dEvent,
					     XrayTelAnalysisManager* aAnalysisManager)
  : enteringEnergy(enEnergy),
    enteringDirection(enDirect),drawEvent(dEvent),
  fAnalysisManager(aAnalysisManager)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XrayTelSteppingAction::~XrayTelSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XrayTelSteppingAction::UserSteppingAction(const G4Step*)
{
  const G4SteppingManager* pSM = fpSteppingManager;
  G4Track* fTrack = pSM->GetTrack();
  G4int StepNo = fTrack->GetCurrentStepNumber();

  if(StepNo >= 10000) fTrack->SetTrackStatus(fStopAndKill);

  G4String volName; 
  if ( fTrack->GetVolume() ) 
    volName =  fTrack->GetVolume()->GetName(); 
  G4String nextVolName;
  if ( fTrack->GetNextVolume() ) 
    nextVolName =  fTrack->GetNextVolume()->GetName();
 
  G4ThreeVector pos = fTrack->GetPosition();

  //--- Entering Detector
  if(volName != "Detector_P" && nextVolName == "Detector_P") {

    enteringEnergy->push_back ( new G4double (fTrack->GetKineticEnergy()) );
    enteringDirection->push_back (new G4ThreeVector (pos));

    // now we want to do some analysis at this step ... 
    // call back to the analysis-manger to do the analysis ...

#ifdef G4ANALYSIS_USE
    if(fAnalysisManager) fAnalysisManager->Step(fpSteppingManager);
#endif

    *drawEvent = true;
  }
}










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
// $Id: RemSimAnalysisMessenger.cc
// GEANT4 tag $Name: radioprotection-V07-00-01
//
// Author: Susanna Guatelli (Susanna.Guatelli@ge.infn.it)
//
// History:
// -----------
// 23 Nov 2004 Susanna Guatelli    Created
//
// -------------------------------------------------------------------

#ifdef G4ANALYSIS_USE
#include "RemSimAnalysisMessenger.hh"

RemSimAnalysisMessenger::RemSimAnalysisMessenger(RemSimAnalysisManager* analysisManager)
  :remsimAnalysis(analysisManager)
  
{ 
  analysisDir = new G4UIdirectory("/analysis/");
  analysisDir->SetGuidance("analysis control.");
  
  outputFileType = new G4UIcmdWithAString("/analysis/formatFile",this);
  outputFileType -> SetGuidance("specify the format: hbook or xml");
  outputFileType -> SetCandidates("hbook xml");
  outputFileType -> SetDefaultValue("hbook");
  outputFileType -> AvailableForStates(G4State_Idle);
}

RemSimAnalysisMessenger::~RemSimAnalysisMessenger()
{
  delete outputFileType;
  delete analysisDir; 
}

void RemSimAnalysisMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{   
  if(command == outputFileType)
    {
      remsimAnalysis -> SetFormat(newValue);
    }
 }

#endif














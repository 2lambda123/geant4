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
///////////////////////////////////////////////////////////////////////////////
// File: CCalSeppingAction.cc
// Description: Study profiling during the steps
///////////////////////////////////////////////////////////////////////////////
#include "CCalSteppingAction.hh"

#include "G4SDManager.hh"
#include "G4StepPoint.hh"
#include <iostream>
#include "G4ThreeVector.hh"
#include <math.h>

#ifdef G4ANALYSIS_USE  
#include "CCalAnalysis.hh"
#endif


CCalSteppingAction::CCalSteppingAction(){

#ifdef G4ANALYSIS_USE  
  CCalAnalysis* analysis = CCalAnalysis::getInstance();
  timeHistoMaxBin=analysis->maxbin();
#else
  timeHistoMaxBin=1;
#endif

  int i; 
  for (i=0; i<200; i++) {timeDeposit[i] = 0.;}
  for (i=0; i<70;  i++) {LateralProfile[i] = 0.;}

}


CCalSteppingAction::~CCalSteppingAction(){
  G4cout << "CCalSteppingAction deleted" << G4endl;
}
  

void CCalSteppingAction::UserSteppingAction(const G4Step* aStep){

  G4StepPoint*  PostStepPoint= aStep->GetPostStepPoint(); 
  G4StepPoint*  PreStepPoint= aStep->GetPreStepPoint(); 
  int TSliceID;

  TSliceID = static_cast<int>( (PostStepPoint->GetGlobalTime() ) / nanosecond);
  TSliceID = TSliceID<timeHistoMaxBin ? TSliceID : timeHistoMaxBin-1;
  timeDeposit[TSliceID] += aStep->GetTotalEnergyDeposit() / GeV;

  G4ThreeVector HitPoint = 0.5*(PostStepPoint->GetPosition()+
				PreStepPoint->GetPosition());	
  // Because the beam axis has been defined as the x-axis, 
  // the lateral displacement is given in terms of the y and z positions. 
  double perp = std::sqrt(HitPoint.y()*HitPoint.y()+HitPoint.z()*HitPoint.z());
  int radialPosition = std::min(69,int(perp/cm));
  LateralProfile[radialPosition] += aStep->GetTotalEnergyDeposit() / GeV;
  
}


void CCalSteppingAction::endOfEvent(){

#ifdef G4ANALYSIS_USE  
  CCalAnalysis* analysis = CCalAnalysis::getInstance();
  analysis->InsertLateralProfile(LateralProfile);  
  analysis->InsertTime(timeDeposit); 
#endif
  
  int i=0;
  for (i=0; i<70; i++){LateralProfile[i] = 0.;}
  for (i=0; i<200; i++){timeDeposit[i] = 0.;}
  
}  

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
// $Id: Em8SteppingAction.cc,v 1.8 2004-12-03 09:45:37 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "Em8DetectorConstruction.hh"
#include "G4EnergyLossTables.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"
#include "Em8SteppingAction.hh"
#include "Em8PrimaryGeneratorAction.hh"
#include "Em8EventAction.hh"
#include "Em8RunAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include <iomanip>
#include "G4UImanager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em8SteppingAction::Em8SteppingAction(Em8DetectorConstruction* DET,
                                     Em8EventAction* EA,
                                     Em8RunAction* RA)
:detector (DET),eventaction (EA),runaction (RA),
 IDold(-1) ,evnoold(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em8SteppingAction::~Em8SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em8SteppingAction::UserSteppingAction(const G4Step* aStep)
{ 

  G4double Theta,Thetaback,Ttrans,Tback,Tsec,Egamma,yend,zend,rend ;
  // G4double Tkin, Edep, xend;
  G4int evno = eventaction->GetEventno() ; 

  IDnow = evno+10000*(aStep->GetTrack()->GetTrackID())+
          100000000*(aStep->GetTrack()->GetParentID()); 
  if(IDnow != IDold)
  {
   IDold=IDnow ;
   if(
      (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e-") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) )
       ||
      (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e+") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) )
     )
        runaction->Fillvertexz(aStep->GetTrack()->GetVertexPosition().x());

   if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber")
   {
    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e-") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) ) 
    {
        eventaction->AddCharged() ;
        eventaction->AddE() ;
        Tsec = aStep->GetTrack()->GetKineticEnergy() ;  // !!!!!!!!!!!!
        Tsec += aStep->GetTotalEnergyDeposit() ;        // !!!!!!!!!!!!
        runaction->FillTsec(Tsec) ;
    }
    else
    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e+") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) ) 
    {
        eventaction->AddCharged() ;
        eventaction->AddP() ;
        Tsec = aStep->GetTrack()->GetKineticEnergy() ;  // !!!!!!!!!!!!
        Tsec += aStep->GetTotalEnergyDeposit() ;        // !!!!!!!!!!!!
        runaction->FillTsec(Tsec) ;
    }
    else
    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "gamma") &&
       ((aStep->GetTrack()->GetTrackID() != 1) ||
       (aStep->GetTrack()->GetParentID() != 0)) ) 
    {
        eventaction->AddNeutral() ;
    }
   }
  }

  if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber")
  {
    if(((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e-") 
              ||
       ((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "e+"))  
          eventaction->CountStepsCharged() ;

    if ((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName()) == "gamma") 
          eventaction->CountStepsNeutral() ;
  }

  if (
      (((aStep->GetTrack()->GetTrackID() == 1) &&
        (aStep->GetTrack()->GetParentID()== 0)) ||
        (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
        GetParticleName() ==
        Em8PrimaryGeneratorAction::GetPrimaryName())) 
        &&
        (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber")
        &&
        (aStep->GetTrack()->GetNextVolume()->GetName()=="World") &&
        (aStep->GetPostStepPoint()->GetProcessDefinedStep()
              ->GetProcessName() == "Transportation") &&
        (aStep->GetTrack()->GetMomentumDirection().x()>0.)
                                                        )
     {
       eventaction->SetTr();
       Theta = std::acos(aStep->GetTrack()->GetMomentumDirection().x()) ;
       runaction->FillTh(Theta) ;
       Ttrans = aStep->GetTrack()->GetKineticEnergy() ;
       runaction->FillTt(Ttrans) ;
       yend= aStep->GetTrack()->GetPosition().y() ;
       zend= aStep->GetTrack()->GetPosition().z() ;
       rend = std::sqrt(yend*yend+zend*zend) ;
       runaction->FillR(rend);
     }
       
  if (
      (((aStep->GetTrack()->GetTrackID() == 1) &&
        (aStep->GetTrack()->GetParentID()== 0)) ||
      (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->
       GetParticleName() ==
       Em8PrimaryGeneratorAction::GetPrimaryName())) 
       &&
      (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber") &&
      (aStep->GetTrack()->GetNextVolume()->GetName()=="World") &&
      (aStep->GetPostStepPoint()->GetProcessDefinedStep()
               ->GetProcessName() == "Transportation") &&
      (aStep->GetTrack()->GetMomentumDirection().z()<0.)
                                                        )
     {
       eventaction->SetRef();
       Thetaback = std::acos(aStep->GetTrack()->GetMomentumDirection().x()) ;
       Thetaback -= 0.5*pi ;
       runaction->FillThBack(Thetaback) ;
       Tback  = aStep->GetTrack()->GetKineticEnergy() ;
       runaction->FillTb(Tback) ;
     }
 

  if (
      ((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="Absorber") &&
      (aStep->GetTrack()->GetNextVolume()->GetName()=="World") &&
      (aStep->GetPostStepPoint()->GetProcessDefinedStep()
               ->GetProcessName() == "Transportation") &&
      (aStep->GetTrack()->GetMomentumDirection().x()>0.) &&
      (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()
       ->GetParticleName() == "gamma"))
     )
     {
       Egamma = aStep->GetTrack()->GetKineticEnergy() ;
       runaction->FillGammaSpectrum(Egamma) ;
     }
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


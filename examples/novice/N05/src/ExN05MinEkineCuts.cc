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
// $Id: ExN05MinEkineCuts.cc,v 1.5 2003-05-28 11:20:25 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//	GEANT 4 class implementation file 
// --------------------------------------------------------------
//                   15 April 1998 M.Maire
// --------------------------------------------------------------

#include "ExN05MinEkineCuts.hh"

#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"
#include "G4EnergyLossTables.hh"

ExN05MinEkineCuts::ExN05MinEkineCuts(const G4String& aName)
  : ExN05SpecialCuts(aName)
{
   if (verboseLevel>1) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
   SetProcessType(fUserDefined);
}

ExN05MinEkineCuts::~ExN05MinEkineCuts()
{}

ExN05MinEkineCuts::ExN05MinEkineCuts(ExN05MinEkineCuts&)
  : ExN05SpecialCuts()
{}

 
G4double ExN05MinEkineCuts::PostStepGetPhysicalInteractionLength(
                             const G4Track& aTrack,
			     G4double ,
			     G4ForceCondition* condition
			    )
{
  // condition is set to "Not Forced"
  *condition = NotForced;

   G4double     proposedStep = DBL_MAX;
   // get the pointer to UserLimits
   G4UserLimits* pUserLimits = aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
   const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
   G4ParticleDefinition* aParticleDef = aTrack.GetDefinition();
  
   if (pUserLimits) {
     //min kinetic energy
     G4double temp = DBL_MAX;
     G4double    eKine     = aParticle->GetKineticEnergy();
     G4Material* aMaterial = aTrack.GetMaterial();
     G4double eMin = pUserLimits->GetUserMinEkine(aTrack);

     G4double    rangeNow = DBL_MAX;
     if (aParticleDef->GetPDGCharge() != 0.0) {
       rangeNow = G4EnergyLossTables::GetRange(aParticleDef,eKine,aMaterial);
     }
     if (eKine < eMin ) {
       proposedStep = 0.;
     } else if (aParticleDef->GetPDGCharge() != 0.0) {
       // charged particles only
       G4double rangeMin = G4EnergyLossTables::GetRange(aParticleDef,eMin,aMaterial);
       temp = rangeNow - rangeMin;
       if (proposedStep > temp) proposedStep = temp;        
     }
   }
   return proposedStep;
}

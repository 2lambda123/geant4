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
// $Id: ExN05SpecialCuts.cc,v 1.7 2004-05-09 11:02:59 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class implementation file 
//
// ------------------------------------------------------------
//                  15 April 1998  M.Maire
// ------------------------------------------------------------

#include "ExN05SpecialCuts.hh"
#include "G4VParticleChange.hh"
#include "G4Track.hh"
#include "G4Step.hh"

ExN05SpecialCuts::ExN05SpecialCuts(const G4String& aName)
  : G4VProcess(aName)
{
   if (verboseLevel>1) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}

ExN05SpecialCuts::~ExN05SpecialCuts() 
{                                     
}                                     

G4VParticleChange*
ExN05SpecialCuts::PostStepDoIt( const G4Track& aTrack, const G4Step& )
{
  //
  // Stop the current particle, if requested by G4UserLimits 
  // 			    			    			    
  aParticleChange.Initialize(aTrack);
  aParticleChange.ProposeEnergy(0.) ;
  aParticleChange.SetLocalEnergyDeposit (aTrack.GetKineticEnergy()) ;
  aParticleChange.ProposeTrackStatus(fStopButAlive);
  return &aParticleChange;
}

G4double
ExN05SpecialCuts::
PostStepGetPhysicalInteractionLength(const G4Track&,G4double,G4ForceCondition*)
{
  return DBL_MAX;
}

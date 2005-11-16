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
// $Id: G4SDKineticEnergyFilter.cc,v 1.1 2005-11-16 23:04:04 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4VSensitiveDetector
#include "G4SDKineticEnergyFilter.hh"
#include "G4Step.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This is the class of a filter to be associated with a
// sensitive detector. 
//
//
// Created: 2005-11-14  Tsukasa ASO.
// 
///////////////////////////////////////////////////////////////////////////////

G4SDKineticEnergyFilter::G4SDKineticEnergyFilter(G4String name,
						 G4double elow, 
						 G4double ehigh)
  :G4VSDFilter(name),fLowEnergy(elow),fHighEnergy(ehigh)
{;}

G4SDKineticEnergyFilter::~G4SDKineticEnergyFilter()
{;}

G4bool G4SDKineticEnergyFilter::Accept(const G4Step* aStep) const
{
  G4double kinetic = aStep->GetTrack()->GetKineticEnergy();
  if ( kinetic < fLowEnergy  ) return FALSE;
  if ( kinetic > fHighEnergy ) return FALSE;
  return TRUE;
}

void G4SDKineticEnergyFilter::SetKineticEnergy(G4double elow, G4double ehigh){
  fLowEnergy  = elow;
  fHighEnergy = ehigh;
}

void G4SDKineticEnergyFilter::show() {
  G4cout << " G4SDKineticEnergyFilter:: "
	 << " LowE  " << fLowEnergy/MeV << " MeV "
	 << " HighE " << fHighEnergy/MeV << " MeV " <<G4endl;
}



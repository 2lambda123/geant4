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
// $Id: G4GammaNuclearReaction.hh,v 1.10 2003-07-01 16:32:50 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// GEANT4 physics class: G4GammaNuclearReaction -- header file
// Created: J.P. Wellisch, 2000/08/18 
// The last update: J.P. Wellisch, Thu Jun  6 2002.
//
#ifndef G4GammaNuclearReaction_h
#define G4GammaNuclearReaction_h

#include "globals.hh"
#include "G4HadronicInteraction.hh"
#include "G4ChiralInvariantPhaseSpace.hh"
#include "G4Gamma.hh"

class G4GammaNuclearReaction : public G4HadronicInteraction
{
  public: 
    virtual ~G4GammaNuclearReaction()
    {
    }
    
    virtual G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack, 
    G4Nucleus& aTargetNucleus);

  private:
    G4ChiralInvariantPhaseSpace theModel;
};

inline
G4HadFinalState * G4GammaNuclearReaction::
ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& aTargetNucleus)
{
  if(aTrack.GetDefinition() != G4Gamma::GammaDefinition())
  {
    G4Exception("Called G4GammaNuclearReaction for particle other than gamma");
  }
  return theModel.ApplyYourself(aTrack, aTargetNucleus);
}

#endif

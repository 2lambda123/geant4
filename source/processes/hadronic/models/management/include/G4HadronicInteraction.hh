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
// $Id: G4HadronicInteraction.hh,v 1.9 2009-01-24 11:56:27 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Interaction  abstract base class
// This class is the base class for the model classes.
// It sorts out the energy-range for the models and provides
// class utilities.
// original by H.P. Wellisch
// Modified by J.L.Chuma, TRIUMF, 21-Mar-1997
// Last modified: 3-Apr-1997
// Added units to energy initialization: J.L. Chuma  04-Apr-97
// Modified by J.L.Chuma, 05-May-97  to Initialize theBlockedCounter
// Modified by J.L.Chuma, 08-Jul-97 to implement the Nucleus changes
// Adding a registry for memory management of hadronic models, HPW 22-Mar-99
// 23-Jan-2009 V.Ivanchenko move constructor and destructor to the body
//                          and reorder methods in the header 

// Class Description
// This is the base class for all hadronic interaction models in geant4.
// If you want to implement a new way of producing a final state, please,
//  inherit from here.
// Class Description - End
 
#ifndef G4HadronicInteraction_h
#define G4HadronicInteraction_h 1
 
#include "G4HadFinalState.hh"
#include "G4ReactionDynamics.hh"
#include "G4Material.hh"
#include "G4Nucleus.hh"
#include "G4Track.hh"
#include "G4HadProjectile.hh"

class G4HadronicInteraction
{
public: // With description

    
  G4HadronicInteraction(const G4String& modelName = "HadronicModel");
    
  virtual ~G4HadronicInteraction();
    
  virtual G4HadFinalState *ApplyYourself(const G4HadProjectile &aTrack, 
					 G4Nucleus & targetNucleus ) = 0;
  // This is the interface to implement for final state production code.
   
  virtual G4bool IsApplicable(const G4HadProjectile &/*aTrack*/, 
			      G4Nucleus & /*targetNucleus*/)
  {  return true;}
 
  inline G4double GetMinEnergy() const
  { return theMinEnergy; }
    
  virtual G4double GetMinEnergy( const G4Material *aMaterial,
				 const G4Element *anElement ) const;
   
  inline void SetMinEnergy( const G4double anEnergy )
  { theMinEnergy = anEnergy; }
    
  virtual void SetMinEnergy( G4double anEnergy, G4Element *anElement );
    
  virtual void SetMinEnergy( G4double anEnergy, G4Material *aMaterial );
    
  inline G4double GetMaxEnergy() const
  { return theMaxEnergy; }
    
  virtual G4double GetMaxEnergy( const G4Material *aMaterial,
				 const G4Element *anElement ) const;
    
  inline void SetMaxEnergy( const G4double anEnergy )
  { theMaxEnergy = anEnergy; }
    
  virtual void SetMaxEnergy( G4double anEnergy, G4Element *anElement );
    
  virtual void SetMaxEnergy( G4double anEnergy, G4Material *aMaterial );
  
  inline const G4HadronicInteraction *GetMyPointer() const
  { return this; }

  inline G4int GetVerboseLevel() const
  { return verboseLevel; }

  inline void SetVerboseLevel( G4int value )
  { verboseLevel = value; }

  inline const G4String& GetModelName() const
  { return theModelName; }
    
public: // Without description

  virtual void DeActivateFor( G4Material *aMaterial );
    
  virtual void ActivateFor( G4Material *aMaterial ) 
  { 
    Block(); 
    SetMaxEnergy(GetMaxEnergy(), aMaterial);
    SetMinEnergy(GetMinEnergy(), aMaterial);
  }

  virtual void DeActivateFor( G4Element *anElement ); 
  virtual void ActivateFor( G4Element *anElement )
  { 
    Block(); 
    SetMaxEnergy(GetMaxEnergy(), anElement);
    SetMinEnergy(GetMinEnergy(), anElement);
  }

  virtual G4bool IsBlocked( const G4Material *aMaterial ) const;

  virtual G4bool IsBlocked( const G4Element *anElement) const;

  inline G4bool operator==(const G4HadronicInteraction &right ) const
  { return ( this == (G4HadronicInteraction *) &right ); }
    
  inline G4bool operator!=(const G4HadronicInteraction &right ) const
  { return ( this != (G4HadronicInteraction *) &right ); }
    
private:
    
  G4HadronicInteraction(const G4HadronicInteraction &right );
    
  const G4HadronicInteraction& operator=(const G4HadronicInteraction &right);

protected:

  inline G4bool IsBlocked() const { return isBlocked;}
  inline void Block() { isBlocked = true; }
    
  G4HadFinalState theParticleChange;
  // the G4HadFinalState object which is modified and returned
  // by address by the ApplyYourself method,
  // (instead of aParticleChange as found in G4VProcess)
    
  G4int verboseLevel;
  // control flag for output messages
  // 0: silent
  // 1: warning messages
  // 2: more
  // (instead of verboseLevel as found in G4VProcess)
    
  G4ReactionDynamics theReactionDynamics;
    
  // these two have global validity
  // units are assumed to be MeV
    
  G4double theMinEnergy;
  G4double theMaxEnergy;
    
  G4bool isBlocked;

  G4String theModelName;
    
private:
        
   std::vector<std::pair<G4double, G4Material *> > theMinEnergyList;
   std::vector<std::pair<G4double, G4Material *> > theMaxEnergyList;
   std::vector<std::pair<G4double, G4Element *> > theMinEnergyListElements;
   std::vector<std::pair<G4double, G4Element *> > theMaxEnergyListElements;
   std::vector<G4Material *> theBlockedList;
   std::vector<G4Element *> theBlockedListElements;
};
 
#endif

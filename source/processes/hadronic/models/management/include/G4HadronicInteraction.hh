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
// $Id: G4HadronicInteraction.hh,v 1.3 2003-11-03 17:54:18 hpw Exp $
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

// Class Description
// This is the base class for all hadronic interaction models in geant4.
// If you want to implement a new way of producing a final state, please inherit
// from here.
// Class Description - End
 
#ifndef G4HadronicInteraction_h
#define G4HadronicInteraction_h 1
 
#include "G4HadFinalState.hh"
#include "G4DynamicParticle.hh"
#include "G4ReactionDynamics.hh"
#include "G4Material.hh"
#include "G4Nucleus.hh"
#include "G4Track.hh"
#include "G4HadProjectile.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4HadronicException.hh"

 class G4HadronicInteraction
 {
 public:
    
    G4HadronicInteraction() :
      verboseLevel(0), theMinEnergy(0.0*GeV), 
      theMaxEnergy(25.0*GeV), isBlocked(false)
    { 
      G4HadronicInteractionRegistry::RegisterMe(this);
    }
    
    virtual ~G4HadronicInteraction()
    {
      G4HadronicInteractionRegistry::RemoveMe(this);
    }
    
 private:
    
    inline G4HadronicInteraction(
     const G4HadronicInteraction &right )
    { *this = right; }
    
    inline const G4HadronicInteraction & operator=(
     const G4HadronicInteraction &right )
    { 
     G4String text = "unintended use of G4HadronicInteraction::operator=";
     throw G4HadronicException(__FILE__, __LINE__, text); 
     return right;
    }
    
 public:
    
    inline G4bool operator==(
     const G4HadronicInteraction &right ) const
    { return ( this == (G4HadronicInteraction *) &right ); }
    
    inline G4bool operator!=(
     const G4HadronicInteraction &right ) const
    { return ( this != (G4HadronicInteraction *) &right ); }
    
    inline G4double GetMinEnergy() const
    { return theMinEnergy; }
    
    G4double GetMinEnergy( const G4Material *aMaterial,
                           const G4Element *anElement ) const;
    
    inline void SetMinEnergy( const G4double anEnergy )
    { theMinEnergy = anEnergy; }
    
    void SetMinEnergy( G4double anEnergy,
                       G4Element *anElement );
    
    void SetMinEnergy( G4double anEnergy,
                       G4Material *aMaterial );
    
    inline G4double GetMaxEnergy() const
    { return theMaxEnergy; }
    
    G4double GetMaxEnergy( const G4Material *aMaterial,
                           const G4Element *anElement ) const;
    
    inline void SetMaxEnergy( const G4double anEnergy )
    { theMaxEnergy = anEnergy; }
    
    void SetMaxEnergy( G4double anEnergy,
                       G4Element *anElement );
    
    void SetMaxEnergy( G4double anEnergy,
                       G4Material *aMaterial );
  
    inline const G4HadronicInteraction *GetMyPointer() const
    { return this; }

    inline G4int GetVerboseLevel() const
    { return verboseLevel; }

    inline void SetVerboseLevel( G4int value )
    { verboseLevel = value; }

public: // With description
    // This is the interface to implement for final state production code.
    
    virtual G4HadFinalState *ApplyYourself(
            const G4HadProjectile &aTrack, G4Nucleus & targetNucleus ) = 0;
public: // Without description

    void DeActivateFor( G4Material *aMaterial );
    
    void ActivateFor( G4Material *aMaterial ) 
    { 
      Block(); 
      SetMaxEnergy(GetMaxEnergy(), aMaterial);
      SetMinEnergy(GetMinEnergy(), aMaterial);
    }

    void DeActivateFor( G4Element *anElement ); 
    void ActivateFor( G4Element *anElement )
    { 
      Block(); 
      SetMaxEnergy(GetMaxEnergy(), anElement);
      SetMinEnergy(GetMinEnergy(), anElement);
    }

    G4bool IsBlocked( const G4Material *aMaterial ) const;

    G4bool IsBlocked( const G4Element *anElement) const;
    
 protected:
    
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
    
    G4bool IsBlocked() const { return isBlocked;}
    void Block() { isBlocked = true; }
    G4bool isBlocked;
    
 private:
        
    std::vector<std::pair<G4double, G4Material *> > theMinEnergyList;
    std::vector<std::pair<G4double, G4Material *> > theMaxEnergyList;
    std::vector<std::pair<G4double, G4Element *> > theMinEnergyListElements;
    std::vector<std::pair<G4double, G4Element *> > theMaxEnergyListElements;
    std::vector<G4Material *> theBlockedList;
    std::vector<G4Element *> theBlockedListElements;
 };
 
#endif

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
// $Id: G4DMesonMinus.hh,v 1.4 2001-07-11 10:01:46 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//
//      Created,             Hisaya Kurashige, 15 June 1997
// **********************************************************************
//  Change both methods to get the pointer into non-inlined H.Kurashige 4 Aug. 1998
// ----------------------------------------------------------------

// Each class inheriting from G4VMeson
// corresponds to a particle type; one and only one
// instance for each class is guaranteed.

#ifndef G4DMesonMinus_h
#define G4DMesonMinus_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VMeson.hh"

// ######################################################################
// ###                         DMesonMinus                            ###
// ######################################################################

class G4DMesonMinus : public G4VMeson
{
 private:
   static G4DMesonMinus theDMesonMinus;
   static G4double  theDMesonMinusLengthCut;
   static G4double* theDMesonMinusKineticEnergyCuts;

 private: // constructors are hide as private  
   G4DMesonMinus(
       const G4String&     aName,        G4double            mass,
       G4double            width,        G4double            charge,   
       G4int               iSpin,        G4int               iParity,    
       G4int               iConjugation, G4int               iIsospin,   
       G4int               iIsospin3,    G4int               gParity,
       const G4String&     pType,        G4int               lepton,      
       G4int               baryon,       G4int               encoding,
       G4bool              stable,       G4double            lifetime,
       G4DecayTable        *decaytable
   );

 public:
   virtual ~G4DMesonMinus() {}

   static G4DMesonMinus* DMesonMinusDefinition();
   static G4DMesonMinus* DMesonMinus();
   static G4double GetCuts() {return theDMesonMinusLengthCut;}   
   static G4double* GetCutsInEnergy() {return theDMesonMinusKineticEnergyCuts;};

   virtual void SetCuts(G4double aCut); 
};

inline void G4DMesonMinus::SetCuts(G4double aCut)
{
  G4ParticleWithCuts::SetCuts(aCut);
  theDMesonMinusLengthCut = theCutInMaxInteractionLength;  
  theDMesonMinusKineticEnergyCuts = theKineticEnergyCuts;
}

#endif

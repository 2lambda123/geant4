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
// $Id: G4Electron.hh,v 1.5 2001-07-11 10:01:52 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History: first implementation, based on object model of
//      4-th April 1996, G.Cosmo
// ****************************************************************
//  Added particle definitions, H.Kurashige, 19 April 1996
//  Added SetCuts implementation for Electron, L.Urban, 30 May 1996
//  Added not static GetEnergyCuts() and GetLengthCuts(), G.Cosmo, 11 July 1996
// ----------------------------------------------------------------

// Each class inheriting from G4VLepton
// corresponds to a particle type; one and only one
// instance for each class is guaranteed.

#ifndef G4Electron_h
#define G4Electron_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VLepton.hh"
class G4Positron;
// ######################################################################
// ###                         ELECTRON                               ###
// ######################################################################

class G4Electron : public G4VLepton
{
 friend class G4Positron;
 private:
   static G4Electron theElectron;
   static G4double theElectronLengthCut;
   static G4double* theElectronKineticEnergyCuts;

 protected:  
   G4double ComputeLoss(G4double AtomicNumber, G4double KineticEnergy) const;
   void BuildRangeVector(
				  const G4Material* aMaterial,
				  const G4LossTable* aLossTable,
				  G4double       maxEnergy,     
				  G4double       aMass,
                                  G4RangeVector* rangeVector
                        );

 private: //hide constructor as private
   G4Electron(
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
   virtual ~G4Electron(){}

   static G4Electron* ElectronDefinition();
   static G4Electron* Electron();
   static G4double GetCuts() {return theElectronLengthCut;}   
   static G4double* GetCutsInEnergy() {return theElectronKineticEnergyCuts;};

   virtual void SetCuts(G4double aCut); 
   virtual void RestoreCuts(G4double cutInLength,
			    const G4double* cutInEnergy );
      
};

inline void G4Electron::SetCuts(G4double aCut)
{
  CalcEnergyCuts(aCut);
  theElectronLengthCut = theCutInMaxInteractionLength;  
  theElectronKineticEnergyCuts = theKineticEnergyCuts;
  
}

inline void G4Electron::RestoreCuts(G4double cutInLength,
			    const G4double* cutInEnergy )
{
  G4ParticleWithCuts::RestoreCuts(cutInLength, cutInEnergy);
  theElectronLengthCut = theCutInMaxInteractionLength;  
  theElectronKineticEnergyCuts = theKineticEnergyCuts;
}

inline G4Electron* G4Electron::Electron()
{  return &theElectron; }
 
#endif











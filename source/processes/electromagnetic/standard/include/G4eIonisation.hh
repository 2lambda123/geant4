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
// $Id: G4eIonisation.hh,v 1.8 2001-07-11 10:03:29 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//      History: based on object model of
//      2nd December 1995, G.Cosmo
//      ---------- G4eIonisation physics process -----------
//                by Laszlo Urban, 20 March 1997 
// ************************************************************
// It is the first implementation of the NEW IONISATION     
// PROCESS. ( delta rays + continuous energy loss)
// It calculates the ionisation for e+/e-.      
// ************************************************************
//
// 10/02/00  modifications , new e.m. structure, L.Urban
// ------------------------------------------------------------
 
#ifndef G4eIonisation_h
#define G4eIonisation_h 1
 
#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4VeEnergyLoss.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsLinearVector.hh"
 
 
class G4eIonisation : public G4VeEnergyLoss 
 
{
  public:
 
    G4eIonisation(const G4String& processName = "eIoni"); 

   ~G4eIonisation();

    G4bool IsApplicable(const G4ParticleDefinition&); 
    
    void BuildPhysicsTable(const G4ParticleDefinition& aParticleType);
        
    void BuildLossTable(const G4ParticleDefinition& aParticleType);

    void BuildLambdaTable(const G4ParticleDefinition& aParticleType);
    
    void PrintInfoDefinition();
   
    G4double GetMeanFreePath(const G4Track& track,
                             G4double previousStepSize,
                             G4ForceCondition* condition ) ;
 
    G4VParticleChange *PostStepDoIt(const G4Track& track,         
                                    const G4Step& Step ) ;                 

    G4double GetLambda(
                   G4double KineticEnergy,G4Material* material);

  protected:

    virtual G4double ComputeMicroscopicCrossSection(
                            const G4ParticleDefinition& aParticleType,
                            G4double KineticEnergy,
                            G4double AtomicNumber,
                            G4double DeltaThreshold);
                            
    G4PhysicsTable* theMeanFreePathTable;

  private:

  // hide assignment operator 
  G4eIonisation & operator=(const G4eIonisation &right);
  G4eIonisation(const G4eIonisation&);

  private:

 //   G4PhysicsTable* theMeanFreePathTable;

    static G4double LowerBoundLambda ; // bining for lambda table
    static G4double UpperBoundLambda ;
    static G4int    NbinLambda ;
    G4double LowestKineticEnergy,HighestKineticEnergy ;
    G4int    TotBin ;

  public:

    static void SetLowerBoundLambda(G4double val) {LowerBoundLambda = val;};
    static void SetUpperBoundLambda(G4double val) {UpperBoundLambda = val;};
    static void SetNbinLambda(G4int n) {NbinLambda = n;};
    static G4double GetLowerBoundLambda() { return LowerBoundLambda;}; 
    static G4double GetUpperBoundLambda() { return UpperBoundLambda;}; 
    static G4int GetNbinLambda() {return NbinLambda;};

};
 
#include "G4eIonisation.icc"
 
#endif
 

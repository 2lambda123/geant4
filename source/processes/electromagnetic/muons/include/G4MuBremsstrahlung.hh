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
// $Id: G4MuBremsstrahlung.hh,v 1.9 2001-07-11 10:03:25 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//      History: first implementation, based on object model of
//      2nd December 1995, G.Cosmo
//      -------- G4MuBremsstrahlung physics process ---------
//                by Laszlo Urban, September 1997
// ************************************************************
// 10/02/00  modifications , new e.m. structure, L.Urban
//

#ifndef G4MuBremsstrahlung_h
#define G4MuBremsstrahlung_h 1

#include "G4ios.hh" 
#include "globals.hh"
#include "Randomize.hh" 
#include "G4VMuEnergyLoss.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4OrderedTable.hh" 
#include "G4PhysicsTable.hh"
#include "G4PhysicsLogVector.hh"
 
class G4MuBremsstrahlung : public G4VMuEnergyLoss
 
{ 
  public:
 
     G4MuBremsstrahlung(const G4String& processName = "MuBrems");
 
    ~G4MuBremsstrahlung();

     G4bool IsApplicable(const G4ParticleDefinition&);

     void BuildPhysicsTable(const G4ParticleDefinition& ParticleType);

     void BuildLossTable(const G4ParticleDefinition& ParticleType);

     void BuildLambdaTable(const G4ParticleDefinition& ParticleType);

     void PrintInfoDefinition() ;

     G4double GetMeanFreePath(const G4Track& track,
                              G4double previousStepSize,
                              G4ForceCondition* condition ) ;
 
     G4VParticleChange *PostStepDoIt(const G4Track& track,
                                     const G4Step& Step  ) ;                 

     G4double GetDMicroscopicCrossSection(
                                      const G4ParticleDefinition* ParticleType,
                                            G4double KineticEnergy, 
                                            G4double AtomicNumber,
                                            G4double AtomicMass,  
                                            G4double GammaEnergy);

  protected:

     G4double ComputeMeanFreePath( const G4ParticleDefinition* ParticleType,
                                           G4double KineticEnergy, 
                                           const G4Material* aMaterial);

     void ComputePartialSumSigma(  const G4ParticleDefinition* ParticleType,
                                           G4double KineticEnergy,
                                           const G4Material* aMaterial);

     virtual G4double ComputeMicroscopicCrossSection(
                                      const G4ParticleDefinition* ParticleType,
                                            G4double KineticEnergy, 
                                            G4double AtomicNumber,
                                            G4double AtomicMass,  
                                            G4double GammaEnergyCut);

     virtual G4double ComputeDMicroscopicCrossSection(
                                      const G4ParticleDefinition* ParticleType,
                                            G4double KineticEnergy, 
                                            G4double AtomicNumber,
                                            G4double AtomicMass,  
                                            G4double GammaEnergy);

  private:

     G4MuBremsstrahlung & operator=(const G4MuBremsstrahlung &right);
     G4MuBremsstrahlung(const G4MuBremsstrahlung&);

     G4double ComputeBremLoss(const G4ParticleDefinition* ParticleType,
                                G4double Z,G4double A,
                                G4double T, G4double Cut);

     G4Element* SelectRandomAtom(G4Material* aMaterial) const;

     void MakeSamplingTables( const G4ParticleDefinition* ParticleType );

  private:

     G4PhysicsTable* theMeanFreePathTable ;              

     G4OrderedTable PartialSumSigma;   

     static G4double LowerBoundLambda ; // bining for lambda table
     static G4double UpperBoundLambda ;
     static G4int    NbinLambda ;
     G4double LowestKineticEnergy,HighestKineticEnergy ;
     G4int    TotBin ;

     const G4double* GammaCutInKineticEnergy;

     G4double GammaCutInKineticEnergyNow;

     // tables for sampling ..............
     static G4int nzdat,ntdat,NBIN ;
     static G4double zdat[5],adat[5],tdat[8] ;
     static G4double ya[1001],proba[5][8][1001] ;
     static G4double CutFixed ;

  public:

    static void SetLowerBoundLambda(G4double val) {LowerBoundLambda = val;};
    static void SetUpperBoundLambda(G4double val) {UpperBoundLambda = val;};
    static void SetNbinLambda(G4int n) {NbinLambda = n;};
    static G4double GetLowerBoundLambda() { return LowerBoundLambda;};
    static G4double GetUpperBoundLambda() { return UpperBoundLambda;};
    static G4int GetNbinLambda() {return NbinLambda;};

};

#include "G4MuBremsstrahlung.icc"
  
#endif

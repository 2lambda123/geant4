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
// $Id: G4VMuEnergyLoss.hh,v 1.9 2003-06-16 17:01:45 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// -------------------------------------------------------------------
//      GEANT 4 class header file 
//
//      History: first implementation, based on object model of
//      2nd December 1995, G.Cosmo
//      ---------- G4VMuEnergyLoss physics process -----------
//                by Laszlo Urban, September 1997
// ********************************************************************
// It is the implementation of the NEW UNIFIED ENERGY LOSS PROCESS.
// It calculates the continuous energy loss for muons.
// Processes giving contribution to the continuous loss :
//   ionisation (= cont.ion.loss + delta ray production)
//   bremsstrahlung
//   e+e- pair production
//   can be added more easily ..........
// This class creates static muplus/muminus dE/dx and range tables ,
// which tables can be used by other processes.
// ************************************************************
// 27-05-98 some corrections by L.Urban
// 23-10-98 cleanup L.Urban
// 10-02-00 corrections due to new e.m. structure L.Urban
// 10-09-01 loss+ mechanism has been implemented (subcutoff delta rays), L.Urban
// 28-09-01 suppression of theMuonPlus ..etc..data members (mma)
// 29-10-01 all static functions no more inlined (mma)
// 16-01-03 Migrade to cut per region (V.Ivanchenko)
// ------------------------------------------------------------

#ifndef G4VMuEnergyLoss_h
#define G4VMuEnergyLoss_h 1

#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include "globals.hh"
#include "Randomize.hh"
#include "G4VEnergyLoss.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VParticleChange.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsLinearVector.hh"
#include "G4MaterialCutsCouple.hh"

class G4EnergyLossMessenger ;

class G4VMuEnergyLoss : public G4VEnergyLoss

{
  public:

    G4VMuEnergyLoss(const G4String& );

    virtual ~G4VMuEnergyLoss();

    G4bool IsApplicable(const G4ParticleDefinition&);

    void BuildDEDXTable(const G4ParticleDefinition& aParticleType);

    G4double GetContinuousStepLimit(
                                    const G4Track& track,
                                    G4double previousStepSize,
                                    G4double currentMinimumStep,
                                    G4double& currentSafety) ;

    G4VParticleChange* AlongStepDoIt(const G4Track& track ,const G4Step& Step) ;

    virtual G4double GetMeanFreePath( const G4Track& track,
                                      G4double previousStepSize,
                                      G4ForceCondition* condition
                                                         ) = 0 ;

    virtual G4VParticleChange* PostStepDoIt(
                                 const G4Track& track,const G4Step& Step) = 0 ;

  protected:



  private:

  // hide  assignment operator
    G4VMuEnergyLoss(G4VMuEnergyLoss &);
    G4VMuEnergyLoss & operator=(const G4VMuEnergyLoss &right);

    G4double GetConstraints(const G4DynamicParticle *aParticle,
                            const G4MaterialCutsCouple* couple);


  protected:

    virtual G4double SecondaryEnergyThreshold(size_t index) = 0;

    G4PhysicsTable* theLossTable ;

  private:

    G4PhysicsTable* theDEDXTable ;
    G4PhysicsTable* theRangeTable ;
    G4PhysicsTable* theRangeCoeffATable ;
    G4PhysicsTable* theRangeCoeffBTable ;
    G4PhysicsTable* theRangeCoeffCTable ;

    G4PhysicsTable** RecorderOfProcess;
    G4int CounterOfProcess;


    // fdEdx=(-dE/dx)
    // computed in GetConstraints at every call;
    G4double fdEdx;

    // fRangeNow is the actual range of the particle
    //  computed in GetConstraints
    G4double fRangeNow ;

    // EnergyBinNumber,RangeCoeffA,... are needed to compute range
    G4int EnergyBinNumber ;
    G4double RangeCoeffA,RangeCoeffB,RangeCoeffC ;


  //    static part of the class
  public:  // With description

    static void  SetNbOfProcesses(G4int nb);
    // Sets number of processes giving contribution to the energy loss

    static void  PlusNbOfProcesses();
    // Increases number of processes giving contribution to the energy loss

    static void  MinusNbOfProcesses();
    // Decreases number of processes giving contribution to the energy loss

    static G4int GetNbOfProcesses();
    // Gets number of processes giving contribution to the energy loss
    // ( default value = 3)

    static void SetLowerBoundEloss(G4double val);
    static void SetUpperBoundEloss(G4double val);
    static void SetNbinEloss(G4int nb);

    static G4double GetLowerBoundEloss();
    static G4double GetUpperBoundEloss();
    static G4int    GetNbinEloss();

  protected:

    static G4PhysicsTable* theDEDXmuplusTable;
    static G4PhysicsTable* theDEDXmuminusTable;
    static G4PhysicsTable* theRangemuplusTable;
    static G4PhysicsTable* theRangemuminusTable;

    static G4PhysicsTable* theInverseRangemuplusTable;
    static G4PhysicsTable* theInverseRangemuminusTable;

    static G4PhysicsTable* theLabTimemuplusTable;
    static G4PhysicsTable* theLabTimemuminusTable;

    static G4PhysicsTable* theProperTimemuplusTable;
    static G4PhysicsTable* theProperTimemuminusTable;


  //  processes inherited from G4muEnergyLoss
  //   register themselves  in the static array Recorder
    static G4int NbOfProcesses;
    static G4PhysicsTable** RecorderOfmuplusProcess;
    static G4PhysicsTable** RecorderOfmuminusProcess;
    static G4int CounterOfmuplusProcess;
    static G4int CounterOfmuminusProcess;

  private:

    static G4int NbinEloss;               // number of bins in table
    static G4double LowerBoundEloss;
    static G4double UpperBoundEloss;
    static G4double RTable,LOGRTable;    // LOGRTable=log(UpperBoundEloss-
                                         // LowerBoundEloss)/NbinEloss
                                         // RTable = exp(LOGRTable)

    static G4PhysicsTable* themuplusRangeCoeffATable;
    static G4PhysicsTable* themuplusRangeCoeffBTable;
    static G4PhysicsTable* themuplusRangeCoeffCTable;
    static G4PhysicsTable* themuminusRangeCoeffATable;
    static G4PhysicsTable* themuminusRangeCoeffBTable;
    static G4PhysicsTable* themuminusRangeCoeffCTable;

    static G4EnergyLossMessenger* eLossMessenger;

    static G4double cN;    // coeff to compute nb of deltas
    static G4int Ndeltamax;              // upper limit for nb of subcutoff
                                         // delta rays in one step

};

#include "G4VMuEnergyLoss.icc"

#endif

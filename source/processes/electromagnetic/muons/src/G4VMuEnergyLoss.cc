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
// $Id: G4VMuEnergyLoss.cc,v 1.29 2003-06-16 17:01:52 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// --------------------------------------------------------------
//      GEANT 4 class implementation file 
//
//      History: first implementation, based on object model of
//      2nd December 1995, G.Cosmo
//      ---------- G4VMuEnergyLoss physics process -----------
//                by Laszlo Urban, September 1997
// **************************************************************
// It is the implementation of the NEW UNIFIED ENERGY LOSS PROCESS.
// It calculates the energy loss of muons.
// **************************************************************
//
// corrections by L.Urban on 27/05/98 (other corrs come soon!)
// cleanup  L.Urban on 23/10/98
// corrections due to new e.m. structure L.Urban 10/02/00
// signature in GetLossWithFluct changed L.Urban 30/10/00
// 29/05/01 V.Ivanchenko minor changes to provide ANSI -wall compilation
// 10/09/01 L.Urban : loss+ mechanism (subcutoff delta rays) implemented
// 12/09/01 min.delta cut is set as rcut/100 + some optimisation, L.Urban
// 17-09-01 migration of Materials to pure STL (mma)
// 28-09-01 suppression of theMuonPlus ..etc..data members (mma)
// 29-10-01 all static functions no more inlined (mma)
// 08-11-01 some small cosmetics , L.Urban
// 06-02-02 bug fixed at subcutoff definition, L.Urban
// 26-02-02 bug fixed in TouchebleHandle definition, V.Ivanchenko
// 29-05-02 bug fixed in N of subcutoff delta, V.Ivanchenko
// 16-01-03 Migrade to cut per region (V.Ivanchenko)
// 25-03-03 add finalRangeRequested (mma)
// 09-04-03 finalRange is region aware (V.Ivanchenko)
// --------------------------------------------------------------
 

#include "G4VMuEnergyLoss.hh"
#include "G4EnergyLossTables.hh"
#include "G4Poisson.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4ProductionCutsTable.hh"

// Initialisation of static members *******************************************

G4int       G4VMuEnergyLoss::NbOfProcesses           = 3 ;
G4PhysicsTable** G4VMuEnergyLoss::RecorderOfmuplusProcess  =
                                           new G4PhysicsTable*[10] ;
G4PhysicsTable** G4VMuEnergyLoss::RecorderOfmuminusProcess =
                                           new G4PhysicsTable*[10] ;
G4int       G4VMuEnergyLoss::CounterOfmuplusProcess  = 0 ;
G4int       G4VMuEnergyLoss::CounterOfmuminusProcess = 0 ;


G4PhysicsTable* G4VMuEnergyLoss::theDEDXmuplusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theRangemuplusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theInverseRangemuplusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theLabTimemuplusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theProperTimemuplusTable = 0 ;

G4PhysicsTable* G4VMuEnergyLoss::theDEDXmuminusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theRangemuminusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theInverseRangemuminusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theLabTimemuminusTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::theProperTimemuminusTable = 0 ;

G4PhysicsTable* G4VMuEnergyLoss::themuplusRangeCoeffATable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::themuplusRangeCoeffBTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::themuplusRangeCoeffCTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::themuminusRangeCoeffATable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::themuminusRangeCoeffBTable = 0 ;
G4PhysicsTable* G4VMuEnergyLoss::themuminusRangeCoeffCTable = 0 ;

G4double G4VMuEnergyLoss::LowerBoundEloss = 1.*keV ;
G4double G4VMuEnergyLoss::UpperBoundEloss = 1000000.*TeV ;
G4int    G4VMuEnergyLoss::NbinEloss = 150 ;
G4double G4VMuEnergyLoss::RTable,G4VMuEnergyLoss::LOGRTable;

G4double G4VMuEnergyLoss::cN       = 0.077*MeV*cm2/g ;
G4int    G4VMuEnergyLoss::Ndeltamax = 100                   ;


G4EnergyLossMessenger* G4VMuEnergyLoss::eLossMessenger = 0 ;

// constructor and destructor

G4VMuEnergyLoss::G4VMuEnergyLoss(const G4String& processName)
   : G4VEnergyLoss (processName),
     theLossTable(0),
     theRangeCoeffATable(0),
     theRangeCoeffBTable(0),
     theRangeCoeffCTable(0)
{
}

G4VMuEnergyLoss::~G4VMuEnergyLoss()
{
     if(theLossTable) {
        theLossTable->clearAndDestroy();
        delete theLossTable; theLossTable = 0;
     }

}

void  G4VMuEnergyLoss::SetNbOfProcesses(G4int nb) {NbOfProcesses=nb;}
    // Sets number of processes giving contribution to the energy loss

void  G4VMuEnergyLoss::PlusNbOfProcesses()        {NbOfProcesses++ ;}
    // Increases number of processes giving contribution to the energy loss

void  G4VMuEnergyLoss::MinusNbOfProcesses()       {NbOfProcesses-- ;}
    // Decreases number of processes giving contribution to the energy loss

G4int G4VMuEnergyLoss::GetNbOfProcesses()         {return NbOfProcesses;}
    // Gets number of processes giving contribution to the energy loss
    // ( default value = 3)

void G4VMuEnergyLoss::SetLowerBoundEloss(G4double val) {LowerBoundEloss=val;}
void G4VMuEnergyLoss::SetUpperBoundEloss(G4double val) {UpperBoundEloss=val;}
void G4VMuEnergyLoss::SetNbinEloss(G4int nb)           {NbinEloss=nb;}

G4double G4VMuEnergyLoss::GetLowerBoundEloss() {return LowerBoundEloss;}
G4double G4VMuEnergyLoss::GetUpperBoundEloss() {return UpperBoundEloss;}
G4int    G4VMuEnergyLoss::GetNbinEloss()       {return NbinEloss;}

void G4VMuEnergyLoss::BuildDEDXTable(
                         const G4ParticleDefinition& aParticleType)
{

  //  calculate data members LOGRTable,RTable first
  G4double lrate = log(UpperBoundEloss/LowerBoundEloss);
  LOGRTable=lrate/NbinEloss;
  RTable   =exp(LOGRTable);

  const G4ProductionCutsTable* theCoupleTable=
        G4ProductionCutsTable::GetProductionCutsTable();
  size_t numOfCouples = theCoupleTable->GetTableSize();

  //set physically consistent value for finalRange
  //  and parameters for en.loss step limit
  if (finalRangeRequested > 0.) { finalRange = finalRangeRequested;}

  G4bool MakeTable ;
  ParticleMass = aParticleType.GetPDGMass() ;
  G4double Charge = aParticleType.GetPDGCharge()/eplus ;

  MakeTable = false ;

  if ( (Charge > 0.) && (CounterOfmuplusProcess==NbOfProcesses) )
       MakeTable = true ;
  else if ( (Charge < 0.) && (CounterOfmuminusProcess==NbOfProcesses) )
       MakeTable = true ;


  if( MakeTable )
  {
   // Build energy loss table as a sum of the energy loss due to the
   //           different processes.

    if( Charge >0.)
    {
      RecorderOfProcess=RecorderOfmuplusProcess;
      CounterOfProcess=CounterOfmuplusProcess;

      if(CounterOfProcess == NbOfProcesses)
      {
        if(theDEDXmuplusTable)
        { theDEDXmuplusTable->clearAndDestroy();
          delete theDEDXmuplusTable; }

        theDEDXmuplusTable = new G4PhysicsTable(numOfCouples);
        theDEDXTable = theDEDXmuplusTable;
      }
    }
    else
    {
      RecorderOfProcess=RecorderOfmuminusProcess;
      CounterOfProcess=CounterOfmuminusProcess;

      if(CounterOfProcess == NbOfProcesses)
      {
        if(theDEDXmuminusTable)
        { theDEDXmuminusTable->clearAndDestroy();
          delete theDEDXmuminusTable; }
        theDEDXmuminusTable = new G4PhysicsTable(numOfCouples);
        theDEDXTable = theDEDXmuminusTable;
      }
    }

    if(CounterOfProcess == NbOfProcesses)
    {
      //  loop for materials
      G4double LowEdgeEnergy , Value ;
      G4bool isOutRange ;
      G4PhysicsTable* pointer ;

      for (size_t J=0; J<numOfCouples; J++)
      {
        // create physics vector and fill it
        G4PhysicsLogVector* aVector = new G4PhysicsLogVector(
                    LowerBoundEloss, UpperBoundEloss, NbinEloss);
        // loop for the kinetic energy
        for (G4int i=0; i<NbinEloss; i++)
        {
          LowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;
          Value = 0. ;
          for (G4int process=0; process < NbOfProcesses; process++)
          {
            pointer= RecorderOfProcess[process];
            Value += (*pointer)[J]->
                               GetValue(LowEdgeEnergy,isOutRange) ;
          }
          aVector->PutValue(i,Value) ;
        }
        theDEDXTable->insert(aVector) ;
      }

      //  reset counter to zero ..................
      if( Charge >0.)
        CounterOfmuplusProcess=0 ;
      else
        CounterOfmuminusProcess=0 ;

      ParticleMass = aParticleType.GetPDGMass() ;

      if(Charge > 0.)
      {
        // Build range table
        theRangemuplusTable = BuildRangeTable(
                  theDEDXmuplusTable,theRangemuplusTable,
                  LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build lab/proper time tables
        theLabTimemuplusTable = BuildLabTimeTable(theDEDXmuplusTable,
                          theLabTimemuplusTable,
                          LowerBoundEloss,UpperBoundEloss,NbinEloss);
        theProperTimemuplusTable = BuildProperTimeTable(theDEDXmuplusTable,
                          theProperTimemuplusTable,
                          LowerBoundEloss,UpperBoundEloss,NbinEloss);

      // Build coeff tables for the energy loss calculation
        themuplusRangeCoeffATable = BuildRangeCoeffATable(theRangemuplusTable,
                              themuplusRangeCoeffATable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

        themuplusRangeCoeffBTable = BuildRangeCoeffBTable(theRangemuplusTable,
                              themuplusRangeCoeffBTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

        themuplusRangeCoeffCTable = BuildRangeCoeffCTable(theRangemuplusTable,
                              themuplusRangeCoeffCTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

        // invert the range table
        theInverseRangemuplusTable = BuildInverseRangeTable(theRangemuplusTable,
                               themuplusRangeCoeffATable,
                               themuplusRangeCoeffBTable,
                               themuplusRangeCoeffCTable,
                               theInverseRangemuplusTable,
                               LowerBoundEloss,UpperBoundEloss,NbinEloss);
      }
      else
      {
        // Build range table
        theRangemuminusTable = BuildRangeTable(
                  theDEDXmuminusTable,theRangemuminusTable,
                  LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build lab/proper time tables
        theLabTimemuminusTable = BuildLabTimeTable(theDEDXmuminusTable,
                          theLabTimemuminusTable,
                          LowerBoundEloss,UpperBoundEloss,NbinEloss);
        theProperTimemuminusTable = BuildProperTimeTable(theDEDXmuminusTable,
                          theProperTimemuminusTable,
                          LowerBoundEloss,UpperBoundEloss,NbinEloss);

      // Build coeff tables for the energy loss calculation
        themuminusRangeCoeffATable = BuildRangeCoeffATable(theRangemuminusTable,
                              themuminusRangeCoeffATable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

        themuminusRangeCoeffBTable = BuildRangeCoeffBTable(theRangemuminusTable,
                              themuminusRangeCoeffBTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

        themuminusRangeCoeffCTable = BuildRangeCoeffCTable(theRangemuminusTable,
                              themuminusRangeCoeffCTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

        // invert the range table
        theInverseRangemuminusTable = BuildInverseRangeTable(theRangemuminusTable,
                               themuminusRangeCoeffATable,
                               themuminusRangeCoeffBTable,
                               themuminusRangeCoeffCTable,
                               theInverseRangemuminusTable,
                               LowerBoundEloss,UpperBoundEloss,NbinEloss);
        // invert the range table
      }

    }

    // make the energy loss and the range table available
    G4EnergyLossTables::Register(&aParticleType,
      (Charge > 0)? theDEDXmuplusTable: theDEDXmuminusTable,
      (Charge > 0)? theRangemuplusTable: theRangemuminusTable,
      (Charge > 0)? theInverseRangemuplusTable: theInverseRangemuminusTable,
      (Charge > 0)? theLabTimemuplusTable: theLabTimemuminusTable,
      (Charge > 0)? theProperTimemuplusTable: theProperTimemuminusTable,
      LowerBoundEloss, UpperBoundEloss, 1.,NbinEloss);


 // if((subSecFlag) && (aParticleType.GetParticleName()=="mu+"))
 // {
 //   G4cout << G4endl;
 //   G4cout.precision(5) ;
 //   G4cout << " hIoni    Minimum Delta cut in range=" << MinDeltaCutInRange/mm
 //          << "  mm." << G4endl;
 //   G4cout << G4endl;
 //   G4cout << "           material        min.delta energy(keV) " << G4endl;
 //   G4cout << G4endl;
 // }

    if(MinDeltaEnergy) {delete [] MinDeltaEnergy; MinDeltaEnergy=0;}
    MinDeltaEnergy = new G4double [numOfCouples];
    if(LowerLimitForced) {delete [] LowerLimitForced; LowerLimitForced=0;}
    LowerLimitForced = new G4bool [numOfCouples];
    G4double Tlowerlimit = 1.*keV ;
    for (size_t mat=0; mat<numOfCouples; mat++)
      {
        LowerLimitForced[mat] = false ;

        // create array for the min. delta cuts in kinetic energy
        G4double rcut = (*(theCoupleTable->GetRangeCutsVector(1)))[mat];
        if(!setMinDeltaCutInRange) MinDeltaCutInRange = rcut/10.0;
        MinDeltaEnergy[mat] = G4EnergyLossTables::GetPreciseEnergyFromRange(
                                G4Electron::Electron(),
                                MinDeltaCutInRange,
                                theCoupleTable->GetMaterialCutsCouple(mat));

        if(MinDeltaEnergy[mat]<Tlowerlimit) MinDeltaEnergy[mat] = Tlowerlimit ;

        G4double ecut = (*(theCoupleTable->GetEnergyCutsVector(1)))[mat];
        if(MinDeltaEnergy[mat]>ecut) MinDeltaEnergy[mat] = ecut;

   //  if((subSecFlag) && (aParticleType.GetParticleName()=="mu+"))
   //  {
   //    G4cout << std::setw(20) << (*theMaterialTable)[mat]->GetName()
   //         << std::setw(15) << MinDeltaEnergy[mat]/keV ;
   //      if(LowerLimitForced[mat])
   //           G4cout << "  lower limit forced." << G4endl;
   //      else
   //           G4cout << G4endl ;
   //  }
      }
  }
}


G4double G4VMuEnergyLoss::GetConstraints(const G4DynamicParticle *aParticle,
                                         const G4MaterialCutsCouple* couple)
{

  // returns the Step limit
  // dRoverRange is the max. allowed relative range loss in one Step
  // it calculates dEdx and the range as well....

  G4double KineticEnergy,StepLimit;
  G4bool isOutRange ;
  G4int bin ;

  if(aParticle->GetDefinition()->GetPDGCharge()>0.)
  {
    theDEDXTable = theDEDXmuplusTable ;
    theRangeTable = theRangemuplusTable ;
    theRangeCoeffATable=themuplusRangeCoeffATable ;
    theRangeCoeffBTable=themuplusRangeCoeffBTable ;
    theRangeCoeffCTable=themuplusRangeCoeffCTable ;
  }
  else
  {
    theDEDXTable = theDEDXmuminusTable ;
    theRangeTable = theRangemuminusTable ;
    theRangeCoeffATable=themuminusRangeCoeffATable ;
    theRangeCoeffBTable=themuminusRangeCoeffBTable ;
    theRangeCoeffCTable=themuminusRangeCoeffCTable ;
  }


  G4double Thigh = UpperBoundEloss/RTable ;
  KineticEnergy = aParticle->GetKineticEnergy();

  bin = G4int(log(KineticEnergy/LowerBoundEloss)/LOGRTable) ;
  EnergyBinNumber = bin ;

  size_t index = couple->GetIndex() ;

  if( KineticEnergy < LowerBoundEloss )
    {
      fdEdx = sqrt(KineticEnergy/LowerBoundEloss)*
             (*theDEDXTable)(index)->GetValue(LowerBoundEloss,isOutRange) ;

      fRangeNow = sqrt(KineticEnergy/LowerBoundEloss)*
             (*theRangeTable)(index)->GetValue(LowerBoundEloss,isOutRange) ;

      StepLimit = fRangeNow ;
    }
  else if (KineticEnergy > Thigh )
    {
      fdEdx = (*theDEDXTable)(index)->GetValue(Thigh,isOutRange);
      fRangeNow = (*theRangeTable)(index)->GetValue(Thigh,isOutRange);
      if (fdEdx > 0.) fRangeNow += (KineticEnergy-Thigh)/fdEdx;
      StepLimit = c1lim*fRangeNow;
    }
  else
    {
         fdEdx = (*theDEDXTable)(index)->
                            GetValue(KineticEnergy,isOutRange) ;

         RangeCoeffA = (*(*theRangeCoeffATable)(index))(EnergyBinNumber) ;
         RangeCoeffB = (*(*theRangeCoeffBTable)(index))(EnergyBinNumber) ;
         RangeCoeffC = (*(*theRangeCoeffCTable)(index))(EnergyBinNumber) ;

         fRangeNow = (RangeCoeffA*KineticEnergy+RangeCoeffB)
                    *KineticEnergy+RangeCoeffC ;

  //   compute the Step limit ..............
     G4double r = std::min(finalRange, couple->GetProductionCuts()
                 ->GetProductionCut(idxG4ElectronCut));
     if(fRangeNow>r)
       {
         StepLimit = dRoverRange*fRangeNow + r*(1.0 - dRoverRange)*(2.0 - r/fRangeNow);

        //  randomise this value
         if(rndmStepFlag) StepLimit = r+(StepLimit-r)*G4UniformRand() ;
         if(StepLimit > fRangeNow) StepLimit = fRangeNow ;
       }
       else
         StepLimit = fRangeNow ;
  }

  return StepLimit ;

}

G4VParticleChange* G4VMuEnergyLoss::AlongStepDoIt(
                              const G4Track& trackData,const G4Step& stepData)
{
  // compute the energy loss after a Step

  static const G4double faclow = 1.5 ;

  // get particle and material pointers from trackData
  const G4DynamicParticle* aParticle = trackData.GetDynamicParticle();
  G4double E      = aParticle->GetKineticEnergy() ;
  G4double Charge = aParticle->GetDefinition()->GetPDGCharge();

  const G4MaterialCutsCouple* couple = trackData.GetMaterialCutsCouple();
  const G4Material* aMaterial = couple->GetMaterial();
  size_t index = couple->GetIndex();

  G4double Step = stepData.GetStepLength();

  aParticleChange.Initialize(trackData);

  // do not track further if kin.energy < 1. eV
  const G4double MinKineticEnergy = 1.*eV;
  const G4double linLossLimit = 0.05 ;

  G4double MeanLoss, finalT;

  if      (E < MinKineticEnergy)    finalT = 0.;
  else if ( E< faclow*LowerBoundEloss)
  {
    if (Step >= fRangeNow)  finalT = 0.;
    else                    finalT = E*(1.-Step/fRangeNow) ;
  }

  else if (E>=UpperBoundEloss) finalT = E - Step*fdEdx;

  else if (Step >= fRangeNow)  finalT = 0.;

  else
  {
    if(Step/fRangeNow < linLossLimit) finalT = E-Step*fdEdx ;
    else
    {
       if (Charge<0.) finalT = G4EnergyLossTables::GetPreciseEnergyFromRange(
                             G4MuonMinus::MuonMinus(),fRangeNow-Step,couple);
       else           finalT = G4EnergyLossTables::GetPreciseEnergyFromRange(
                             G4MuonPlus::MuonPlus(),fRangeNow-Step,couple);
    }
  }

  if(finalT < MinKineticEnergy) finalT = 0. ;

  MeanLoss = E-finalT ;

  //   subcutoff delta ray production start
  if((subSecFlag) && (trackData.GetCurrentStepNumber() > 1))
  {
    G4double MinDeltaEnergyNow,Tc,TmintoProduceDelta,w,ww ;
    G4double rcut,T0,presafety,postsafety,safety,delta,Tmax,mass ;
    G4double fragment = Step;
    G4double frperstep = 1.0;
    G4double x1,y1,z1,dx,dy,dz,dTime,time0,DeltaTime;
    //G4double epsil = MinKineticEnergy/2. ;

    MinDeltaEnergyNow = MinDeltaEnergy[index] ;
    Tc=SecondaryEnergyThreshold(index);
    const G4ParticleDefinition* aParticleType=aParticle->GetDefinition() ;
    mass=aParticleType->GetPDGMass() ;
    w=mass+electron_mass_c2 ;
    ww=2.*mass-MinDeltaEnergyNow ;
    TmintoProduceDelta=0.5*(sqrt(ww*ww+2.*w*w*MinDeltaEnergyNow/
                       electron_mass_c2)-ww) ;

    if((E > TmintoProduceDelta) && (MeanLoss > MinDeltaEnergyNow)
                                   && (finalT > MinKineticEnergy))
    {
    // max. possible delta energy
      Tmax = 2.*electron_mass_c2*E*(E+2.*mass)/
           (mass*mass+2.*electron_mass_c2*(E+mass)+
            electron_mass_c2*electron_mass_c2) ;

      rcut=couple->GetProductionCuts()->GetProductionCut(1);

      if (Tc > Tmax) Tc=Tmax ;
    // generate subcutoff delta rays only if Tc>MinDeltaEnergyNow!
      if ((Tc > MinDeltaEnergyNow) && (Tmax > MinDeltaEnergyNow))
      {
        presafety  = stepData.GetPreStepPoint()->GetSafety() ;
     // postsafety = stepData.GetPostStepPoint()->GetSafety() ;

        G4Navigator *navigator=
         G4TransportationManager::GetTransportationManager()
                                   ->GetNavigatorForTracking();
        postsafety =
          navigator->ComputeSafety(stepData.GetPostStepPoint()->GetPosition());

        safety = std::min(presafety,postsafety) ;
        if (safety < rcut)
        {

          x1=stepData.GetPreStepPoint()->GetPosition().x();
          y1=stepData.GetPreStepPoint()->GetPosition().y();
          z1=stepData.GetPreStepPoint()->GetPosition().z();
          dx=stepData.GetPostStepPoint()->GetPosition().x()-x1 ;
          dy=stepData.GetPostStepPoint()->GetPosition().y()-y1 ;
          dz=stepData.GetPostStepPoint()->GetPosition().z()-z1 ;
          time0=stepData.GetPreStepPoint()->GetGlobalTime();
          dTime=stepData.GetPostStepPoint()->GetGlobalTime()-time0;

          if ((presafety<rcut)&&(postsafety<rcut))
          {
            fragment = Step ;
            frperstep=1. ;
          }
          else if(presafety<rcut)
          {
            delta=presafety*Step/(postsafety-presafety) ;
            fragment=rcut*(Step+delta)/postsafety-delta ;
            frperstep=fragment/Step;
          }
          else if(postsafety<rcut)
          {
            delta=postsafety*Step/(presafety-postsafety) ;
            fragment=rcut*(Step+delta)/presafety-delta ;
            x1 += dx;
            y1 += dy;
            z1 += dz;
            time0 += dTime ;

            frperstep=-fragment/Step;
          }
          if(fragment>0.)
          {
            T0=G4EnergyLossTables::GetPreciseEnergyFromRange(
                                             G4Electron::Electron(),
                                             std::min(presafety,postsafety),
                                             couple) ;

        // absolute lower limit for T0
            if((T0<MinDeltaEnergyNow)||(LowerLimitForced[index]))
                             T0=MinDeltaEnergyNow ;

        // compute nb of delta rays to be generated
        // approximate value based on Bethe-Bloch and
        //   assuming an 1/E**2 delta spectrum
            G4double deldedx=cN*aMaterial->GetDensity()*
                         ((E+mass)*(E+mass)*log(Tc/T0)/(E*(E+mass))) ;
            G4double delToverTc=1.-T0/Tc ;
            G4int N = G4int(deldedx*fragment*delToverTc/(T0*log(Tc/T0))+0.5) ;
            if(N > Ndeltamax) N = Ndeltamax ;

            G4double Px,Py,Pz ;
            G4ThreeVector ParticleDirection ;
            ParticleDirection=aParticle->
                                   GetMomentumDirection() ;
            Px =ParticleDirection.x() ;
            Py =ParticleDirection.y() ;
            Pz =ParticleDirection.z() ;

            G4int subdelta = 0;

            if (N > 0)
            {
              G4double Tkin,Etot,P,T,p,costheta,sintheta,phi,dirx,diry,dirz,
                   Pnew,urandom;
          //delTkin,delLoss,rate,
          //G4StepPoint *point ;

              Tkin = E ;
              Etot = Tkin+mass ;
              P    = sqrt(Tkin*(Etot+mass)) ;

              aParticleChange.SetNumberOfSecondaries(N);
              do {
                subdelta += 1 ;

                Tmax = 2.*electron_mass_c2*Tkin*(Tkin+2.*mass)/
                      (mass*mass+2.*electron_mass_c2*(Tkin+mass)+
                        electron_mass_c2*electron_mass_c2) ;

                if(Tc>Tmax) Tc = Tmax ;

               //check if there is enough energy ....
                if((Tkin>TmintoProduceDelta)&&(Tc > T0)&&(MeanLoss>0.))
                {
                  T=T0/(1.-delToverTc*G4UniformRand()) ;
                  if(T > MeanLoss) T=MeanLoss ;
                  MeanLoss -= T ;
                  p=sqrt(T*(T+2.*electron_mass_c2)) ;

                  costheta = T*(Etot+electron_mass_c2)/(P*p) ;
                  if(costheta<-1.) costheta=-1.;
                  if(costheta> 1.) costheta= 1.;

                  phi=twopi*G4UniformRand() ;
                  sintheta=sqrt(1.-costheta*costheta);
                  dirx=sintheta*cos(phi);
                  diry=sintheta*sin(phi);
                  dirz=costheta;

                  urandom = G4UniformRand() ;
               // distribute x,y,z along Pre-Post !
                  G4double xd,yd,zd ;
                  xd=x1+frperstep*dx*urandom ;
                  yd=y1+frperstep*dy*urandom ;
                  zd=z1+frperstep*dz*urandom ;
                  G4ThreeVector DeltaPosition(xd,yd,zd) ;
                  DeltaTime=time0+frperstep*dTime*urandom ;

                  G4ThreeVector DeltaDirection(dirx,diry,dirz) ;
                  DeltaDirection.rotateUz(ParticleDirection);

                  G4DynamicParticle* theDelta = new G4DynamicParticle ;
                  theDelta->SetDefinition(G4Electron::Electron());
                  theDelta->SetKineticEnergy(T);

                  theDelta->SetMomentumDirection(DeltaDirection.x(),
                              DeltaDirection.y(),DeltaDirection.z());

               // update initial particle,fill ParticleChange
                  Tkin -= T ;
                  Px =(P*ParticleDirection.x()-p*DeltaDirection.x()) ;
                  Py =(P*ParticleDirection.y()-p*DeltaDirection.y()) ;
                  Pz =(P*ParticleDirection.z()-p*DeltaDirection.z()) ;
                  Pnew = sqrt(Px*Px+Py*Py+Pz*Pz) ;
                  Px /= Pnew ;
                  Py /= Pnew ;
                  Pz /= Pnew ;
                  P  = Pnew ;
                  G4ThreeVector ParticleDirectionnew(Px,Py,Pz) ;
                  ParticleDirection = ParticleDirectionnew;

                  G4Track* deltaTrack =
                        new G4Track(theDelta,DeltaTime,DeltaPosition);
                  deltaTrack->
                   SetTouchableHandle(stepData.GetPreStepPoint()->GetTouchableHandle()) ;

                  deltaTrack->SetParentID(trackData.GetTrackID()) ;
                  aParticleChange.AddSecondary(deltaTrack) ;

                }

              } while (subdelta<N) ;

             // update the particle direction and kinetic energy
              if(subdelta > 0)
                aParticleChange.SetMomentumChange(Px,Py,Pz) ;
              E = Tkin ;
            }
          }
        }
      }
    }
  }
  //   end of subcutoff business

  finalT = E - MeanLoss ;
  if(finalT < MinKineticEnergy) finalT = 0. ;

  //now the loss with fluctuation
  if ((EnlossFlucFlag) && (finalT > 0.) && (finalT < E)&&(E > LowerBoundEloss))

  {
    finalT = E-GetLossWithFluct(aParticle,couple,1.,MeanLoss,Step);
    if (finalT < 0.) finalT = 0. ;
  }

  // kill the particle if the kinetic energy <= 0
  if (finalT <= 0. )
  {
      finalT = 0.;
      aParticleChange.SetStatusChange(fStopButAlive);
  }

 // aParticleChange.SetNumberOfSecondaries(0);
  aParticleChange.SetEnergyChange(finalT);
  aParticleChange.SetLocalEnergyDeposit(E-finalT);

  return &aParticleChange;
}

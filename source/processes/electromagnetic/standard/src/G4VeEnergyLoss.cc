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
// $Id: G4VeEnergyLoss.cc,v 1.28 2003-03-10 12:22:02 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

// -----------------------------------------------------------------------------
// 18/11/98  , L. Urban
//  It is a modified version of G4VeEnergyLoss:
//  continuous energy loss with generation of subcutoff delta rays
// 02/02/99  important correction in AlongStepDoIt , L.Urban
// 28/04/99  bug fixed (unit independece now),L.Urban
// 10/02/00  modifications , new e.m. structure, L.Urban
// 23/01/01  bug fixed in AlongStepDoIt , L.Urban
// 27/03/01 : commented out the printing of subcutoff energies
// 28/05/01  V.Ivanchenko minor changes to provide ANSI -wall compilation
// 11/09/01  minor correction in 'subcutoff' delta generation, L.Urban
// 12/09/01  min.delta cut is set as rcut/100 + some optimisation, L.Urban
// 17-09-01, migration of Materials to pure STL (mma)
// 29-10-01 all static functions no more inlined (mma)
// 08-11-01 Charge,lastCharge not data members, L.Urban
// 06-02-02 bug fixed in MinDeltaCutInRange computation, L.Urban
// 26-02-02 bug fixed in TouchebleHandle definition, V.Ivanchenko
// 15-01-03 Migrade to cut per region (V.Ivanchenko)
// 10-03-03 remove tails of old cuts (V.Ivanchenko)
// -----------------------------------------------------------------------------


#include "G4VeEnergyLoss.hh"
#include "G4Poisson.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4ProductionCutsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Initialisation of static data members
// -------------------------------------

G4int            G4VeEnergyLoss::NbOfProcesses  = 2;
G4int            G4VeEnergyLoss::CounterOfElectronProcess = 0;
G4int            G4VeEnergyLoss::CounterOfPositronProcess = 0;
G4PhysicsTable** G4VeEnergyLoss::RecorderOfElectronProcess =
                                           new G4PhysicsTable*[10];
G4PhysicsTable** G4VeEnergyLoss::RecorderOfPositronProcess =
                                           new G4PhysicsTable*[10];
                                           
G4PhysicsTable*  G4VeEnergyLoss::theDEDXElectronTable         = 0;
G4PhysicsTable*  G4VeEnergyLoss::theDEDXPositronTable         = 0;
G4PhysicsTable*  G4VeEnergyLoss::theRangeElectronTable        = 0;
G4PhysicsTable*  G4VeEnergyLoss::theRangePositronTable        = 0;
G4PhysicsTable*  G4VeEnergyLoss::theInverseRangeElectronTable = 0;
G4PhysicsTable*  G4VeEnergyLoss::theInverseRangePositronTable = 0;
G4PhysicsTable*  G4VeEnergyLoss::theLabTimeElectronTable      = 0;
G4PhysicsTable*  G4VeEnergyLoss::theLabTimePositronTable      = 0;
G4PhysicsTable*  G4VeEnergyLoss::theProperTimeElectronTable   = 0;
G4PhysicsTable*  G4VeEnergyLoss::theProperTimePositronTable   = 0;

G4PhysicsTable*  G4VeEnergyLoss::theeRangeCoeffATable         = 0;
G4PhysicsTable*  G4VeEnergyLoss::theeRangeCoeffBTable         = 0;
G4PhysicsTable*  G4VeEnergyLoss::theeRangeCoeffCTable         = 0;
G4PhysicsTable*  G4VeEnergyLoss::thepRangeCoeffATable         = 0;
G4PhysicsTable*  G4VeEnergyLoss::thepRangeCoeffBTable         = 0;
G4PhysicsTable*  G4VeEnergyLoss::thepRangeCoeffCTable         = 0;

G4double G4VeEnergyLoss::LowerBoundEloss =0.1*keV ;
G4double G4VeEnergyLoss::UpperBoundEloss = 100.*TeV ;
G4int    G4VeEnergyLoss::NbinEloss = 150 ;
G4double G4VeEnergyLoss::RTable,G4VeEnergyLoss::LOGRTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// constructor and destructor
 
G4VeEnergyLoss::G4VeEnergyLoss(const G4String& processName)
   : G4VEnergyLoss (processName),
     theLossTable(0),
     MinKineticEnergy(1.*eV),
     theDEDXTable(0),
     linLossLimit(0.05),
     cN(0.077*MeV*cm2/g),
     Ndeltamax(100)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VeEnergyLoss::~G4VeEnergyLoss()
{
     if (theLossTable) 
       {
         theLossTable->clearAndDestroy();
         delete theLossTable; theLossTable = 0;
       }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void  G4VeEnergyLoss::SetNbOfProcesses(G4int nb)
      {NbOfProcesses=nb;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void  G4VeEnergyLoss::PlusNbOfProcesses()
      {NbOfProcesses++ ;}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void  G4VeEnergyLoss::MinusNbOfProcesses()
      {NbOfProcesses-- ;}
      
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4VeEnergyLoss::GetNbOfProcesses()
      {return NbOfProcesses;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4VeEnergyLoss::SetLowerBoundEloss(G4double val)
     {LowerBoundEloss=val;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void G4VeEnergyLoss::SetUpperBoundEloss(G4double val)
     {UpperBoundEloss=val;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void G4VeEnergyLoss::SetNbinEloss(G4int nb)
     {NbinEloss=nb;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4VeEnergyLoss::GetLowerBoundEloss()
         {return LowerBoundEloss;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4VeEnergyLoss::GetUpperBoundEloss()
         {return UpperBoundEloss;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4VeEnergyLoss::GetNbinEloss()
      {return NbinEloss;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4VeEnergyLoss::BuildDEDXTable(
                         const G4ParticleDefinition& aParticleType)
{
  ParticleMass = aParticleType.GetPDGMass();

  //  calculate data members LOGRTable,RTable first
  G4double lrate = log(UpperBoundEloss/LowerBoundEloss);
  LOGRTable=lrate/NbinEloss;
  RTable   =exp(LOGRTable);

  const G4ProductionCutsTable* theCoupleTable=
        G4ProductionCutsTable::GetProductionCutsTable();
  size_t numOfCouples = theCoupleTable->GetTableSize();

  //set physically consistent value for finalRange
  //and parameters for en.loss step limit

  for (size_t idxMate=0; idxMate<numOfCouples; idxMate++)
     {
      G4double rcut = theCoupleTable->GetMaterialCutsCouple(idxMate)
                       ->GetProductionCuts()->GetProductionCut(idxG4ElectronCut);

      if (finalRange > rcut) finalRange = rcut;
     }


  c1lim = dRoverRange ;
  c2lim = 2.*(1.-dRoverRange)*finalRange ;
  c3lim = -(1.-dRoverRange)*finalRange*finalRange;

  // Build energy loss table as a sum of the energy loss due to the
  // different processes.

  // create table if there is no table or there is a new cut value

  if       (&aParticleType==G4Electron::Electron())
                {theDEDXTable= theDEDXElectronTable;}
  else if  (&aParticleType==G4Positron::Positron())
                {theDEDXTable= theDEDXPositronTable;}

  if ( !theDEDXTable || CutsWhereModified() )

  {
  // create table for the total energy loss

   if (&aParticleType==G4Electron::Electron())
    {
      RecorderOfProcess=RecorderOfElectronProcess;
      CounterOfProcess=CounterOfElectronProcess;
      if (CounterOfProcess == NbOfProcesses)
        {
         if (theDEDXElectronTable)
           {
             theDEDXElectronTable->clearAndDestroy();
             delete theDEDXElectronTable;
           }
         theDEDXElectronTable = new G4PhysicsTable(numOfCouples);
         theDEDXTable = theDEDXElectronTable;
        }
    }
   if (&aParticleType==G4Positron::Positron())
    {
     RecorderOfProcess=RecorderOfPositronProcess;
     CounterOfProcess=CounterOfPositronProcess;
     if (CounterOfProcess == NbOfProcesses)
       {
        if (theDEDXPositronTable)
          {
            theDEDXPositronTable->clearAndDestroy();
            delete theDEDXPositronTable;
          }
        theDEDXPositronTable = new G4PhysicsTable(numOfCouples);
        theDEDXTable = theDEDXPositronTable;
       }
    }

   if (CounterOfProcess == NbOfProcesses)
    {
     // fill the tables
     // loop for materials
     G4double LowEdgeEnergy , Value;
     G4bool isOutRange;
     G4PhysicsTable* pointer;

     for (size_t J=0; J<numOfCouples; J++)
        {
         // create physics vector and fill it

         G4PhysicsLogVector* aVector = new G4PhysicsLogVector(
                    LowerBoundEloss, UpperBoundEloss, NbinEloss);

         // loop for the kinetic energy

         for (G4int i=0; i<NbinEloss; i++)
            {
              LowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;
              //here comes the sum of the different tables created by the
              //processes (ionisation,bremsstrahlung,etc...)
              Value = 0.;
              for (G4int process=0; process < NbOfProcesses; process++)
                 {
                   pointer= RecorderOfProcess[process];
                   Value += (*pointer)[J]->GetValue(LowEdgeEnergy,isOutRange);
                 }

              aVector->PutValue(i,Value) ;
            }

         theDEDXTable->insert(aVector) ;

        }


     //reset counter to zero
     if (&aParticleType==G4Electron::Electron()) CounterOfElectronProcess=0;
     if (&aParticleType==G4Positron::Positron()) CounterOfPositronProcess=0;

     ParticleMass = aParticleType.GetPDGMass();

     if (&aParticleType==G4Electron::Electron())
     {
       // Build range table
       theRangeElectronTable = BuildRangeTable(theDEDXElectronTable,
                                               theRangeElectronTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build lab/proper time tables
       theLabTimeElectronTable = BuildLabTimeTable(theDEDXElectronTable,
                         theLabTimeElectronTable,
                         LowerBoundEloss,UpperBoundEloss,NbinEloss);
       theProperTimeElectronTable = BuildProperTimeTable(theDEDXElectronTable,
                            theProperTimeElectronTable,
                            LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build coeff tables for the energy loss calculation
       theeRangeCoeffATable = BuildRangeCoeffATable(theRangeElectronTable,
                             theeRangeCoeffATable,
                             LowerBoundEloss,UpperBoundEloss,NbinEloss);

       theeRangeCoeffBTable = BuildRangeCoeffBTable(theRangeElectronTable,
                             theeRangeCoeffBTable,
                             LowerBoundEloss,UpperBoundEloss,NbinEloss);

       theeRangeCoeffCTable = BuildRangeCoeffCTable(theRangeElectronTable,
                             theeRangeCoeffCTable,
                             LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // invert the range table
       theInverseRangeElectronTable = BuildInverseRangeTable(
                              theRangeElectronTable,
                              theeRangeCoeffATable,
                              theeRangeCoeffBTable,
                              theeRangeCoeffCTable,
                              theInverseRangeElectronTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);
     }
     if (&aParticleType==G4Positron::Positron())
     {
        // Build range table
       theRangePositronTable = BuildRangeTable(theDEDXPositronTable,
                                               theRangePositronTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);


       // Build lab/proper time tables
       theLabTimePositronTable = BuildLabTimeTable(theDEDXPositronTable,
                         theLabTimePositronTable,
                         LowerBoundEloss,UpperBoundEloss,NbinEloss);
       theProperTimePositronTable = BuildProperTimeTable(theDEDXPositronTable,
                            theProperTimePositronTable,
                            LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build coeff tables for the energy loss calculation
       thepRangeCoeffATable = BuildRangeCoeffATable(theRangePositronTable,
                             thepRangeCoeffATable,
                             LowerBoundEloss,UpperBoundEloss,NbinEloss);

       thepRangeCoeffBTable = BuildRangeCoeffBTable(theRangePositronTable,
                             thepRangeCoeffBTable,
                             LowerBoundEloss,UpperBoundEloss,NbinEloss);

       thepRangeCoeffCTable = BuildRangeCoeffCTable(theRangePositronTable,
                             thepRangeCoeffCTable,
                             LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // invert the range table
       theInverseRangePositronTable = BuildInverseRangeTable(
                              theRangePositronTable,
                              thepRangeCoeffATable,
                              thepRangeCoeffBTable,
                              thepRangeCoeffCTable,
                              theInverseRangePositronTable,
                              LowerBoundEloss,UpperBoundEloss,NbinEloss);

     }

     // make the energy loss and the range table available
     G4EnergyLossTables::Register(&aParticleType,
       (&aParticleType==G4Electron::Electron())?
       theDEDXElectronTable: theDEDXPositronTable,
       (&aParticleType==G4Electron::Electron())?
       theRangeElectronTable: theRangePositronTable,
       (&aParticleType==G4Electron::Electron())?
       theInverseRangeElectronTable: theInverseRangePositronTable,
       (&aParticleType==G4Electron::Electron())?
       theLabTimeElectronTable: theLabTimePositronTable,
       (&aParticleType==G4Electron::Electron())?
       theProperTimeElectronTable: theProperTimePositronTable,
       LowerBoundEloss, UpperBoundEloss, 1.,NbinEloss);

     // create array for the min. delta cuts in kinetic energy
     G4double absLowerLimit = 1.*keV ;


//     if((subSecFlag) && (&aParticleType==G4Electron::Electron()))
//     {
//       G4cout << G4endl;
//       G4cout.precision(5) ;
//       G4cout << " eIoni   Minimum Delta cut in range="
//              << MinDeltaCutInRange/mm
//              << "  mm." << G4endl;
//       G4cout <<  G4endl;
//       G4cout << "           material       min.delta energy(keV) " << G4endl;
//       G4cout << G4endl;
//     }

     if(MinDeltaEnergy) {delete [] MinDeltaEnergy; MinDeltaEnergy=0;}
     MinDeltaEnergy = new G4double [numOfCouples];
     if(LowerLimitForced) {delete [] LowerLimitForced; LowerLimitForced=0;}
     LowerLimitForced = new G4bool [numOfCouples];

     for(size_t mat=0; mat<numOfCouples; mat++)
       {
       // create array for the min. delta cuts in kinetic energy
        G4double ecut = (*(theCoupleTable->GetEnergyCutsVector(idxG4ElectronCut)))[mat];
        if(!setMinDeltaCutInRange) MinDeltaCutInRange = ecut/10.0;
        MinDeltaEnergy[mat] = G4EnergyLossTables::GetPreciseEnergyFromRange(
                                G4Electron::Electron(),
                                MinDeltaCutInRange,
                                theCoupleTable->GetMaterialCutsCouple(mat));

        if(MinDeltaEnergy[mat]<absLowerLimit) MinDeltaEnergy[mat] = absLowerLimit;

        if(MinDeltaEnergy[mat]>ecut) MinDeltaEnergy[mat]=ecut;

       }
     }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* G4VeEnergyLoss::AlongStepDoIt( const G4Track& trackData,
                                                  const G4Step&  stepData)
{
 // compute the energy loss after a Step

  static const G4double faclow = 1.5 ;
  static const G4double   Tlow = 1.0*keV;

  // get particle and material pointers from trackData
  const G4DynamicParticle* aParticle = trackData.GetDynamicParticle();
  G4double Charge = aParticle->GetDefinition()->GetPDGCharge()/eplus;
  G4double E      = aParticle->GetKineticEnergy() ;

  const G4MaterialCutsCouple* couple = trackData.GetMaterialCutsCouple();
  const G4Material* aMaterial = couple->GetMaterial();
  G4int index = couple->GetIndex();
  G4double Step = stepData.GetStepLength();

  aParticleChange.Initialize(trackData);

  G4double MeanLoss, finalT;

  if (E < MinKineticEnergy)   finalT = 0.;

  else if (E<faclow*LowerBoundEloss)
  {
    if (Step >= fRangeNow)  finalT = 0.;
    else finalT = E*(1.-sqrt(Step/fRangeNow)) ;
  }

  else if (E>=UpperBoundEloss) finalT = E - Step*fdEdx;

  else if (Step >= fRangeNow)  finalT = 0.;

  else
  {
    if((Step/fRangeNow < linLossLimit)||(E < Tlow)) finalT = E-Step*fdEdx ;
    else
    {
      if (Charge<0.) finalT = G4EnergyLossTables::GetPreciseEnergyFromRange
                             (G4Electron::Electron(),fRangeNow-Step,couple);
      else           finalT = G4EnergyLossTables::GetPreciseEnergyFromRange
                             (G4Positron::Positron(),fRangeNow-Step,couple);
     }
  }

  if(finalT < MinKineticEnergy) finalT = 0. ;

  MeanLoss = E - finalT ;

  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //  start of subcutoff generation

 // do not generate subdeltas for the initial step !! (time..)
 if((subSecFlag) && (trackData.GetCurrentStepNumber() > 1))
 {
  G4double MinDeltaEnergyNow = MinDeltaEnergy[index] ;
  G4double TmintoProduceDelta=0.5*(3.-Charge)*MinDeltaEnergyNow ;
  if((E > TmintoProduceDelta) && (MeanLoss > MinDeltaEnergyNow)
                                   && (finalT > MinKineticEnergy))
  {
    G4double T0,presafety,postsafety,safety,delta;
    G4double fragment = Step;
    G4double frperstep= 1.0;
    G4double x1,y1,z1,dx,dy,dz,dTime,time0,DeltaTime;
    //G4double epsil= MinKineticEnergy/2. ;

    G4double Tc = SecondaryEnergyThreshold(index);
    G4double rcut=couple->GetProductionCuts()->GetProductionCut(idxG4ElectronCut);

    if(Charge < 0.)
    {
      if(Tc > 0.5*E) Tc=0.5*E ;
    }
    else
    {
      if(Tc > E) Tc=E ;
    }
    // generate subcutoff delta rays only if Tc>MinDeltaEnergy!
    if(Tc > MinDeltaEnergyNow)
    {
      presafety  = stepData.GetPreStepPoint()->GetSafety() ;

      G4Navigator *navigator=
         G4TransportationManager::GetTransportationManager()
                                   ->GetNavigatorForTracking();
      postsafety =
          navigator->ComputeSafety(stepData.GetPostStepPoint()->GetPosition());

      safety=G4std::min(presafety,postsafety);

      if(safety<rcut)
      {
        T0=G4EnergyLossTables::GetPreciseEnergyFromRange(
                     G4Electron::Electron(),safety,couple) ;

        // absolute lower limit for T0
     //   if(T0<MinDeltaEnergyNow) T0=MinDeltaEnergyNow ;
        if((T0<MinDeltaEnergyNow)||(LowerLimitForced[index]))
                              T0=MinDeltaEnergyNow ;

 // ..................................................................

        x1=stepData.GetPreStepPoint()->GetPosition().x();
        y1=stepData.GetPreStepPoint()->GetPosition().y();
        z1=stepData.GetPreStepPoint()->GetPosition().z();
        dx=stepData.GetPostStepPoint()->GetPosition().x()-x1 ;
        dy=stepData.GetPostStepPoint()->GetPosition().y()-y1 ;
        dz=stepData.GetPostStepPoint()->GetPosition().z()-z1 ;
        time0=stepData.GetPreStepPoint()->GetGlobalTime();
        dTime=stepData.GetPostStepPoint()->GetGlobalTime()-time0;

        if((presafety<rcut)&&(postsafety<rcut))
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
        // compute nb of delta rays to be generated
        // from the de/dx formula (approximately)
        // and assuming an 1/T**2 delta energy spectrum

        G4double delToverTc=1.-T0/Tc ;
        G4double deldedx=cN*aMaterial->GetDensity()*
                         ((E+electron_mass_c2)*(E+electron_mass_c2)*
                         log(Tc/T0)/(E*(E+electron_mass_c2))) ;
        G4int N=G4int(deldedx*fragment*delToverTc/(T0*log(Tc/T0))+0.5) ;

        if(N > Ndeltamax)
           N = Ndeltamax ;
        G4double Px,Py,Pz ;
        G4ThreeVector ParticleDirection ;
        ParticleDirection=stepData.GetPreStepPoint()->
                                   GetMomentumDirection() ;
        Px =ParticleDirection.x() ;
        Py =ParticleDirection.y() ;
        Pz =ParticleDirection.z() ;

        G4int subdelta = 0;

        if(N > 0)
        {
          G4double Tkin,Etot,P,T,p,costheta,sintheta,phi,dirx,diry,dirz,
                   Pnew,sumT,urandom ;
          //delTkin,delLoss,rate,
          //G4StepPoint *point ;

          sumT=0.;

          Tkin = E ;
          Etot = Tkin+electron_mass_c2 ;
          P    = sqrt(Tkin*(Etot+electron_mass_c2)) ;

          aParticleChange.SetNumberOfSecondaries(N);
          do {
               subdelta += 1 ;

               if((Charge<0.)&&(Tc>0.5*Tkin)) Tc=0.5*Tkin ;
               if((Charge>0.)&&(Tc>    Tkin)) Tc=    Tkin ;

               //check if there is enough energy ....
               if((Tkin>TmintoProduceDelta)&&(Tc > T0)&&(MeanLoss>0.))
               {
                 delToverTc=1.-T0/Tc ;
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

               sumT += T ;

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
               deltaTrack->SetTouchableHandle(stepData.GetPreStepPoint()
	                                               ->GetTouchableHandle());
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
 // ................................................................
     }
    }
  }
 }

  //    end of subcutoff generation
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
     if (Charge < 0.) aParticleChange.SetStatusChange(fStopAndKill);
     else             aParticleChange.SetStatusChange(fStopButAlive);
   }

  aParticleChange.SetEnergyChange(finalT);
  aParticleChange.SetLocalEnergyDeposit(E-finalT);

  return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




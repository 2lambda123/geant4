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
// $Id: G4VhEnergyLoss.cc,v 1.42 2003-04-08 18:04:25 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

// -----------------------------------------------------------------------------
// 07/10/98 bug fixes + some cleanup , L.Urban 
// 22/10/98 cleanup , L.Urban
// 07/12/98 works for ions as well+ bug corrected, L.Urban
// 02/02/99 several bugs fixed, L.Urban
// 01/03/99 creation of sub-cutoff delta rays, L.Urban
// 28/04/99 bug fixed in DoIt , L.Urban
// 10/02/00 modifications , new e.m. structure, L.Urban
// 18/07/00 bug fix in AlongStepDoIt V.Ivanchenko
// 10/08/00 V.Ivanchenko change AlongStepDoIt and
//          add EnergyLossFluctuation in order to simulate
//          energy losses of ions
// 17/08/00 V.Ivanchenko change EnergyLossFluctuation
// 18/08/00 V.Ivanchenko bug fixed in GetConstrained
// 23/01/01 bug fixed in AlongStepDoIt , L.Urban
// 27/03/01 commented out the printing of subcutoff energies
// 28/05/01 V.Ivanchenko minor changes to provide ANSI -wall compilation
// 10/09/01 bugfix in subcutoff delta generation, L.Urban
// 12/09/01 min.delta cut is set as rcut/100 + some optimisation, L.Urban
// 17-09-01 migration of Materials to pure STL (mma)
// 29-10-01 all static functions no more inlined (mma)
// 08-11-01 BuildDEDXTable not static,Charge local variable, L.Urban
// 09-11-01 cosmetics; 80 columns everywhere (mma)
// 06-02-02 bug fixed in MinDeltaCutInRange computation, L.Urban
// 26-02-02 bug fixed in TouchebleHandle definition, V.Ivanchenko
// 29-05-02 bug fixed in N of subcutoff delta, V.Ivanchenko
// 10-06-02 bug fixed for stopping hadrons, V.Ivanchenko
// 15-01-03 Migrade to cut per region (V.Ivanchenko)
// 25-03-03 add finalRangeRequested (mma)
// 07-04-03 add verbosity (V.Ivanchenko)
// 08-04-03 finalRange is region aware (V.Ivanchenko)
// -----------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VhEnergyLoss.hh"
#include "G4EnergyLossTables.hh"
#include "G4Poisson.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4ProcessManager.hh"
#include "G4ProductionCutsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int            G4VhEnergyLoss::NbOfProcesses    = 1;

G4int            G4VhEnergyLoss::CounterOfProcess = 0;
G4PhysicsTable** G4VhEnergyLoss::RecorderOfProcess =
                                           new G4PhysicsTable*[10];

G4int            G4VhEnergyLoss::CounterOfpProcess = 0;
G4PhysicsTable** G4VhEnergyLoss::RecorderOfpProcess =
                                           new G4PhysicsTable*[10];

G4int            G4VhEnergyLoss::CounterOfpbarProcess = 0;
G4PhysicsTable** G4VhEnergyLoss::RecorderOfpbarProcess =
                                           new G4PhysicsTable*[10];

G4PhysicsTable* G4VhEnergyLoss::theDEDXpTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theDEDXpbarTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theRangepTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theRangepbarTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theInverseRangepTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theInverseRangepbarTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theLabTimepTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theLabTimepbarTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theProperTimepTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::theProperTimepbarTable = NULL;

G4PhysicsTable* G4VhEnergyLoss::thepRangeCoeffATable = NULL;
G4PhysicsTable* G4VhEnergyLoss::thepRangeCoeffBTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::thepRangeCoeffCTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::thepbarRangeCoeffATable = NULL;
G4PhysicsTable* G4VhEnergyLoss::thepbarRangeCoeffBTable = NULL;
G4PhysicsTable* G4VhEnergyLoss::thepbarRangeCoeffCTable = NULL;

G4PhysicsTable* G4VhEnergyLoss::theDEDXTable = NULL;

G4double G4VhEnergyLoss::LowerBoundEloss = 1.*keV;
G4double G4VhEnergyLoss::UpperBoundEloss = 100.*TeV;
G4int    G4VhEnergyLoss::NbinEloss = 100;
G4double G4VhEnergyLoss::RTable,G4VhEnergyLoss::LOGRTable;

G4double G4VhEnergyLoss::cN       = 0.077*MeV*cm2/g;
G4int    G4VhEnergyLoss::Ndeltamax = 100;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VhEnergyLoss::G4VhEnergyLoss(const G4String& processName)
   : G4VEnergyLoss (processName),
     theLossTable (NULL),
     MinKineticEnergy(1.*eV),
     linLossLimit(0.05)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VhEnergyLoss::~G4VhEnergyLoss()
{
     if(theLossTable) {
        theLossTable->clearAndDestroy();
        delete theLossTable; theLossTable = 0;
     }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  G4VhEnergyLoss::SetNbOfProcesses(G4int nb)
      {NbOfProcesses=nb;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  G4VhEnergyLoss::PlusNbOfProcesses()
      {NbOfProcesses++ ;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  G4VhEnergyLoss::MinusNbOfProcesses()
      {NbOfProcesses-- ;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4VhEnergyLoss::GetNbOfProcesses()
      {return NbOfProcesses;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  G4VhEnergyLoss::SetLowerBoundEloss(G4double val)
      {LowerBoundEloss=val;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  G4VhEnergyLoss::SetUpperBoundEloss(G4double val)
      {UpperBoundEloss=val;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  G4VhEnergyLoss::SetNbinEloss(G4int nb)
      {NbinEloss=nb;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4VhEnergyLoss::GetLowerBoundEloss()
         {return LowerBoundEloss;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4VhEnergyLoss::GetUpperBoundEloss()
         {return UpperBoundEloss;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int G4VhEnergyLoss::GetNbinEloss()
      {return NbinEloss;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4VhEnergyLoss::BuildDEDXTable(
                         const G4ParticleDefinition& aParticleType)
{
  if(0 < verboseLevel) {
    G4cout << "G4VhEnergyLoss::BuildDEDXTable() for process "
           << GetProcessName() << " and particle "
           << aParticleType.GetParticleName() << G4endl;
  }

  //  calculate data members LOGRTable,RTable first
  G4double lrate = log(UpperBoundEloss/LowerBoundEloss);
  LOGRTable=lrate/NbinEloss;
  RTable   =exp(LOGRTable);

  const G4ProductionCutsTable* theCoupleTable=
        G4ProductionCutsTable::GetProductionCutsTable();
  size_t numOfCouples = theCoupleTable->GetTableSize();

  //set physically consistent value for finalRange
  //and parameters for en.loss step limit
  if (finalRangeRequested > 0.) { finalRange = finalRangeRequested;}
/*
  else
   {
    for (size_t idxMate=0; idxMate<numOfCouples; idxMate++)
     {
      G4double rcut = theCoupleTable->GetMaterialCutsCouple(idxMate)
                       ->GetProductionCuts()->GetProductionCut(1);

      if (finalRange > rcut) finalRange = rcut;
     }
   }
  c1lim = dRoverRange;
  c2lim = 2.*(1.-dRoverRange)*finalRange;
  c3lim = -(1.-dRoverRange)*finalRange*finalRange;
*/
  // create table if there is no table or there is a new cut value
  // create/fill proton or antiproton tables depending on the charge
  G4double Charge = aParticleType.GetPDGCharge()/eplus;
  ParticleMass = aParticleType.GetPDGMass() ;

  if (Charge>0.) {theDEDXTable= theDEDXpTable;}
  else           {theDEDXTable= theDEDXpbarTable;}

  G4String pname = aParticleType.GetParticleName();
  if( !theDEDXTable || (CutsWhereModified() &&
      (pname == "proton" || pname == "anti_proton")) )

  {
   // Build energy loss table as a sum of the energy loss due to the
   //              different processes.
   if (Charge >0.)
    {
      RecorderOfProcess=RecorderOfpProcess;
      CounterOfProcess=CounterOfpProcess;

      if(CounterOfProcess == NbOfProcesses)
      {
        if(theDEDXpTable) {theDEDXpTable->clearAndDestroy();
                           delete theDEDXpTable;}
        theDEDXpTable = new G4PhysicsTable(numOfCouples);
        theDEDXTable  = theDEDXpTable;
      }
    }
   else
    {
      RecorderOfProcess=RecorderOfpbarProcess;
      CounterOfProcess=CounterOfpbarProcess;

      if(CounterOfProcess == NbOfProcesses)
      {
        if(theDEDXpbarTable) {theDEDXpbarTable->clearAndDestroy();
                              delete theDEDXpbarTable;}
        theDEDXpbarTable = new G4PhysicsTable(numOfCouples);
        theDEDXTable     = theDEDXpbarTable;
      }
    }

   if(CounterOfProcess == NbOfProcesses)
    {
     //  loop for materials
     //
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
          G4double LowEdgeEnergy = aVector->GetLowEdgeEnergy(i);
          G4double Value = 0. ;

          // loop for the contributing processes
          for (G4int process=0; process < NbOfProcesses; process++)
          {
            pointer= RecorderOfProcess[process];
            Value += (*pointer)[J]->GetValue(LowEdgeEnergy,isOutRange);
          }

          aVector->PutValue(i,Value);
        }

        theDEDXTable->insert(aVector);
      }

      // reset counter to zero
      if(Charge >0.) CounterOfpProcess=0;
      else           CounterOfpbarProcess=0;

      if(Charge > 0.)
      {
       // Build range table
       //
       theRangepTable = BuildRangeTable(theDEDXpTable, theRangepTable,
                                   LowerBoundEloss,UpperBoundEloss,NbinEloss);
       // Build lab/proper time tables
       //
       theLabTimepTable = BuildLabTimeTable(theDEDXpTable, theLabTimepTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);
       theProperTimepTable = BuildProperTimeTable(theDEDXpTable,
                                                  theProperTimepTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build coeff tables for the energy loss calculation
       //
       thepRangeCoeffATable = BuildRangeCoeffATable(theRangepTable,
                                                    thepRangeCoeffATable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       thepRangeCoeffBTable = BuildRangeCoeffBTable(theRangepTable,
                                                    thepRangeCoeffBTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       thepRangeCoeffCTable = BuildRangeCoeffCTable(theRangepTable,
                                                    thepRangeCoeffCTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // invert the range table
       //
       theInverseRangepTable = BuildInverseRangeTable(theRangepTable,
                                     thepRangeCoeffATable,
                                     thepRangeCoeffBTable,
                                     thepRangeCoeffCTable,
                                     theInverseRangepTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

      }
     else
      {
        // Build range table
	//
       theRangepbarTable = BuildRangeTable(theDEDXpbarTable, theRangepbarTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build lab/proper time tables
       //
       theLabTimepbarTable = BuildLabTimeTable(theDEDXpbarTable,
                                               theLabTimepbarTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);
       theProperTimepbarTable = BuildProperTimeTable(theDEDXpbarTable,
                                                     theProperTimepbarTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build coeff tables for the energy loss calculation
       //
       thepbarRangeCoeffATable = BuildRangeCoeffATable(theRangepbarTable,
                                                        thepbarRangeCoeffATable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       thepbarRangeCoeffBTable = BuildRangeCoeffBTable(theRangepbarTable,
                                                        thepbarRangeCoeffBTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       thepbarRangeCoeffCTable = BuildRangeCoeffCTable(theRangepbarTable,
                                                        thepbarRangeCoeffCTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // invert the range table
       //
       theInverseRangepbarTable = BuildInverseRangeTable(theRangepbarTable,
                                     thepbarRangeCoeffATable,
                                     thepbarRangeCoeffBTable,
                                     thepbarRangeCoeffCTable,
                                     theInverseRangepbarTable,
                                     LowerBoundEloss,UpperBoundEloss,NbinEloss);
      }
    }
  }

  // make the energy loss and the range table available

  G4EnergyLossTables::Register(&aParticleType,
       (Charge>0)? theDEDXpTable: theDEDXpbarTable,
       (Charge>0)? theRangepTable: theRangepbarTable,
       (Charge>0)? theInverseRangepTable: theInverseRangepbarTable,
       (Charge>0)? theLabTimepTable: theLabTimepbarTable,
       (Charge>0)? theProperTimepTable: theProperTimepbarTable,
       LowerBoundEloss, UpperBoundEloss,
       proton_mass_c2/aParticleType.GetPDGMass(),NbinEloss);


  if(MinDeltaEnergy) {delete [] MinDeltaEnergy; MinDeltaEnergy=0;}
  MinDeltaEnergy = new G4double [numOfCouples];
  if(LowerLimitForced) {delete [] LowerLimitForced; LowerLimitForced=0;}
  LowerLimitForced = new G4bool [numOfCouples];
  G4double Tlowerlimit = 1.*keV;
  for (size_t mat=0; mat<numOfCouples; mat++)
    {
     // create array for the min. delta cuts in kinetic energy
     G4double ecut = (*(theCoupleTable->GetEnergyCutsVector(1)))[mat];
     if(!setMinDeltaCutInRange) MinDeltaCutInRange = ecut/10.0;
     MinDeltaEnergy[mat] = G4EnergyLossTables::GetPreciseEnergyFromRange(
                              G4Electron::Electron(),
			      MinDeltaCutInRange,
                              theCoupleTable->GetMaterialCutsCouple(mat));
     if(MinDeltaEnergy[mat]<Tlowerlimit) MinDeltaEnergy[mat]=Tlowerlimit;

     if(MinDeltaEnergy[mat]>ecut) MinDeltaEnergy[mat]=ecut;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4VhEnergyLoss::GetConstraints(const G4DynamicParticle *aParticle,
                                        const G4MaterialCutsCouple * couple)
{
 // returns the Step limit
 // dRoverRange is the max. allowed relative range loss in one step
 // it calculates dEdx and the range as well....

 G4double KineticEnergy = aParticle->GetKineticEnergy();
 G4double massratio=proton_mass_c2/(aParticle->GetMass());
 G4double Tscaled = KineticEnergy*massratio;
 G4double Charge  = aParticle->GetCharge()/eplus;
 G4double ChargeSquare = Charge*Charge;

 if (Charge>0.)
  {
   fRangeNow=G4EnergyLossTables::GetRange(G4Proton::Proton(),Tscaled,couple);
   fdEdx    =G4EnergyLossTables::GetDEDX (G4Proton::Proton(),Tscaled,couple);
  }
 else
  {
   fRangeNow=G4EnergyLossTables::GetRange(G4AntiProton::AntiProton(),
                                                             Tscaled,couple);
   fdEdx    =G4EnergyLossTables::GetDEDX (G4AntiProton::AntiProton(),
                                                             Tscaled,couple);
  }
 fdEdx     *= ChargeSquare;
 fRangeNow /= (ChargeSquare*massratio);

 // compute the (random) Step limit
 //
 G4double r = G4std::min(finalRange, couple->GetProductionCuts()
                 ->GetProductionCut(idxG4ElectronCut));
 G4double StepLimit;
 if (fRangeNow > r)
  {
    StepLimit = dRoverRange*fRangeNow + r*(1.0 - dRoverRange)*(2.0 - r/fRangeNow);
    //   StepLimit = (c1lim*fRangeNow+c2lim+c3lim/fRangeNow);

    //  randomise this value
    if (rndmStepFlag) StepLimit=finalRange+(StepLimit-finalRange)*G4UniformRand();
    if (StepLimit > fRangeNow) StepLimit = fRangeNow;
  }
 else StepLimit = fRangeNow;

 return StepLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* G4VhEnergyLoss::AlongStepDoIt(
                              const G4Track& trackData,const G4Step& stepData)
{
 // compute the energy loss after a step
 //
 aParticleChange.Initialize(trackData);

 const G4MaterialCutsCouple* couple = trackData.GetMaterialCutsCouple();
 const G4Material* aMaterial = couple->GetMaterial();
 G4int index = couple->GetIndex();

 const G4DynamicParticle*   aParticle = trackData.GetDynamicParticle();
 G4double Charge = aParticle->GetCharge()/eplus;
 G4double ChargeSquare = Charge*Charge;
 G4double mass=aParticle->GetMass();

 // get the actual (true) Step length from stepData
 G4double Step = stepData.GetStepLength();

 G4double E = aParticle->GetKineticEnergy();
 G4double MeanLoss = 0.;

 if (E < MinKineticEnergy) MeanLoss = E;
 else
  {
   if(Step >= fRangeNow)   MeanLoss = E;
   else if((E > UpperBoundEloss)||(E <= LowerBoundEloss)) MeanLoss = Step*fdEdx;
   else
    {
     if (Step>linLossLimit*fRangeNow)
      {
       G4double massratio = proton_mass_c2/mass;
       G4double rscaled = fRangeNow*massratio*ChargeSquare;
       G4double sscaled =   Step   *massratio*ChargeSquare;

       if (Charge>0.)
        {
          MeanLoss = G4EnergyLossTables::GetPreciseEnergyFromRange(
                                         G4Proton::Proton(),
                                         rscaled        ,couple) -
                     G4EnergyLossTables::GetPreciseEnergyFromRange(
                                         G4Proton::Proton(),
                                         rscaled-sscaled,couple);
        }
       else
        {
          MeanLoss = G4EnergyLossTables::GetPreciseEnergyFromRange(
                                         G4AntiProton::AntiProton(),
                                         rscaled        ,couple) -
                     G4EnergyLossTables::GetPreciseEnergyFromRange(
                                         G4AntiProton::AntiProton(),
                                         rscaled-sscaled,couple);
        }
       MeanLoss /= massratio;
      }
     else MeanLoss = Step*fdEdx;
    }
  }
 G4double finalT = E - MeanLoss;

 //   subcutoff delta ray production start
 //
 if((subSecFlag) && (trackData.GetCurrentStepNumber() > 1))
 {
  G4double delta;
  G4double fragment = Step;
  G4double frperstep = 1.0;
  G4double x1,y1,z1,dx,dy,dz,dTime,time0,DeltaTime;

  G4double MinDeltaEnergyNow = MinDeltaEnergy[index] ;
  G4double Tc = SecondaryEnergyThreshold(index);

  G4double w=mass+electron_mass_c2 ;
  G4double ww=2.*mass-MinDeltaEnergyNow ;
  G4double TmintoProduceDelta=0.5*(sqrt(ww*ww+2.*w*w*MinDeltaEnergyNow/
                                        electron_mass_c2)-ww);

  if((E > TmintoProduceDelta) && (MeanLoss > MinDeltaEnergyNow)
                              && (finalT > MinKineticEnergy))
  {
   // max. possible delta energy
   G4double Tmax = 2.*electron_mass_c2*E*(E+2.*mass)/
                   (mass*mass+2.*electron_mass_c2*(E+mass)+
                   electron_mass_c2*electron_mass_c2);
   G4double rcut=couple->GetProductionCuts()->GetProductionCut(1);
   if (Tc > Tmax) Tc=Tmax;

   // generate subcutoff delta rays only if Tc>MinDeltaEnergyNow
   if ((Tc > MinDeltaEnergyNow) && (Tmax > MinDeltaEnergyNow))
    {
     G4double presafety = stepData.GetPreStepPoint()->GetSafety();
     G4Navigator* navigator =
                          G4TransportationManager::GetTransportationManager()
                                                   ->GetNavigatorForTracking();
     G4double postsafety =
          navigator->ComputeSafety(stepData.GetPostStepPoint()->GetPosition());
     G4double safety = G4std::min(presafety,postsafety);

     if (safety < rcut)
      {
        x1=stepData.GetPreStepPoint()->GetPosition().x();
        y1=stepData.GetPreStepPoint()->GetPosition().y();
        z1=stepData.GetPreStepPoint()->GetPosition().z();
        dx=stepData.GetPostStepPoint()->GetPosition().x()-x1;
        dy=stepData.GetPostStepPoint()->GetPosition().y()-y1;
        dz=stepData.GetPostStepPoint()->GetPosition().z()-z1;
        time0=stepData.GetPreStepPoint()->GetGlobalTime();
        dTime=stepData.GetPostStepPoint()->GetGlobalTime()-time0;

        if ((presafety<rcut)&&(postsafety<rcut))
         {
          fragment = Step;
          frperstep=1.;
         }
        else if (presafety<rcut)
         {
          delta=presafety*Step/(postsafety-presafety) ;
          fragment=rcut*(Step+delta)/postsafety-delta ;
          frperstep=fragment/Step;
         }
        else if(postsafety<rcut)
         {
          delta=postsafety*Step/(presafety-postsafety);
          fragment=rcut*(Step+delta)/presafety-delta ;
          x1 += dx;
          y1 += dy;
          z1 += dz;
          time0 += dTime ;
          frperstep=-fragment/Step;
         }

        if (fragment>0.)
         {
          G4double T0=G4EnergyLossTables::GetPreciseEnergyFromRange(
                                             G4Electron::Electron(),
                                             G4std::min(presafety,postsafety),
                                             couple);
          // absolute lower limit for T0
    	  if((T0<MinDeltaEnergyNow)||(LowerLimitForced[index]))
                             T0=MinDeltaEnergyNow;

          //compute nb of delta rays to be generated
          //approximate value based on Bethe-Bloch and
          //assuming an 1/E**2 delta spectrum

          G4double deldedx=cN*aMaterial->GetDensity()*
                         ((E+mass)*(E+mass)*log(Tc/T0)/(E*(E+mass)));
          G4double delToverTc=1.-T0/Tc ;
          G4int N = G4int(deldedx*fragment*delToverTc/(T0*log(Tc/T0))+0.5);
          if(N > Ndeltamax) N = Ndeltamax;

          G4ThreeVector ParticleDirection = aParticle->GetMomentumDirection();
          G4double Px =ParticleDirection.x();
          G4double Py =ParticleDirection.y();
          G4double Pz =ParticleDirection.z();

          G4int subdelta = 0;

          if (N > 0)
           {
            G4double T,p,costheta,sintheta,phi,dirx,diry,dirz,
                   Pnew,urandom;
            G4double Tkin = E;
            G4double Etot = Tkin+mass;
            G4double P    = sqrt(Tkin*(Etot+mass));

            aParticleChange.SetNumberOfSecondaries(N);
            do {
                subdelta += 1;
                Tmax = 2.*electron_mass_c2*Tkin*(Tkin+2.*mass)/
                      (mass*mass+2.*electron_mass_c2*(Tkin+mass)+
                        electron_mass_c2*electron_mass_c2);
                if(Tc>Tmax) Tc = Tmax;

                //check if there is enough energy ....
                if((Tkin>TmintoProduceDelta)&&(Tc > T0)&&(MeanLoss>0.))
                 {
                  delToverTc=1.-T0/Tc;
                  T=T0/(1.-delToverTc*G4UniformRand());
                  if(T > MeanLoss) T=MeanLoss;
                  MeanLoss -= T;
                  p=sqrt(T*(T+2.*electron_mass_c2));

                  costheta = T*(Etot+electron_mass_c2)/(P*p);
                  if(costheta<-1.) costheta=-1.;
                  if(costheta> 1.) costheta= 1.;

                  phi=twopi*G4UniformRand();
                  sintheta=sqrt(1.-costheta*costheta);
                  dirx=sintheta*cos(phi);
                  diry=sintheta*sin(phi);
                  dirz=costheta;

                  urandom = G4UniformRand() ;
                  // distribute x,y,z along Pre-Post !
                  G4double xd=x1+frperstep*dx*urandom;
                  G4double yd=y1+frperstep*dy*urandom;
                  G4double zd=z1+frperstep*dz*urandom;
                  G4ThreeVector DeltaPosition(xd,yd,zd);
                  DeltaTime=time0+frperstep*dTime*urandom;

                  G4ThreeVector DeltaDirection(dirx,diry,dirz);
                  DeltaDirection.rotateUz(ParticleDirection);

                  G4DynamicParticle* theDelta = new G4DynamicParticle;
                  theDelta->SetDefinition(G4Electron::Electron());
                  theDelta->SetKineticEnergy(T);

                  theDelta->SetMomentumDirection(DeltaDirection.x(),
                              DeltaDirection.y(),DeltaDirection.z());

                  // update initial particle,fill ParticleChange
                  Tkin -= T;
                  Px =(P*ParticleDirection.x()-p*DeltaDirection.x());
                  Py =(P*ParticleDirection.y()-p*DeltaDirection.y());
                  Pz =(P*ParticleDirection.z()-p*DeltaDirection.z());
                  Pnew = sqrt(Px*Px+Py*Py+Pz*Pz);
                  Px /= Pnew;
                  Py /= Pnew;
                  Pz /= Pnew;
                  P   = Pnew;
                  G4ThreeVector ParticleDirectionnew(Px,Py,Pz);
                  ParticleDirection = ParticleDirectionnew;

                  G4Track* deltaTrack =
                                new G4Track(theDelta,DeltaTime,DeltaPosition);
                  deltaTrack->SetTouchableHandle(stepData.GetPreStepPoint()
		                                      ->GetTouchableHandle());

                  deltaTrack->SetParentID(trackData.GetTrackID());

                  aParticleChange.AddSecondary(deltaTrack);
                 }

               } while (subdelta<N);

            // update the particle direction and kinetic energy
            if(subdelta > 0) aParticleChange.SetMomentumChange(Px,Py,Pz);
            E = Tkin;
           }
          }
         }
       }
     }
  }
 //   end of subcutoff business

 finalT = E - MeanLoss;
 if(finalT < MinKineticEnergy) finalT = 0.;

 //  now the loss with fluctuation
 if ((EnlossFlucFlag) && (finalT > 0.) && (finalT < E)&&(E > LowerBoundEloss))
  {
   finalT = E -
        EnergyLossFluctuation(aParticle,couple,ChargeSquare,MeanLoss,Step);
   if (finalT < 0.) finalT = 0.;
  }

 //  kill the particle if the kinetic energy <= 0
 if (finalT <= 0.)
  {
   finalT = 0.;
   if(!aParticle->GetDefinition()->GetProcessManager()->GetAtRestProcessVector()->size())
          aParticleChange.SetStatusChange(fStopAndKill);
   else   aParticleChange.SetStatusChange(fStopButAlive);
  }

  aParticleChange.SetEnergyChange(finalT);
  aParticleChange.SetLocalEnergyDeposit(E-finalT);

  return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4VhEnergyLoss::EnergyLossFluctuation(
                                         const G4DynamicParticle* aParticle,
                                         const G4MaterialCutsCouple* couple,
                                               G4double ChargeSquare,
                                               G4double MeanLoss,
                                               G4double Step)
{
  return GetLossWithFluct(aParticle,couple,ChargeSquare,MeanLoss,Step);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

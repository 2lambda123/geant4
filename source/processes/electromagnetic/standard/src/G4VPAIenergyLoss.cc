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
// $Id: G4VPAIenergyLoss.cc,v 1.9 2004-09-08 10:31:07 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -----------------------------------------------------------
//      GEANT 4 class implementation file 
//
//      History: based on object model of
//      2nd December 1995, G.Cosmo
//      ---------- G4VPAIenergyLoss physics process -----------
//                by V. Grichine, 30 Nov 1997 
// **************************************************************
// It is the first implementation of the NEW UNIFIED ENERGY LOSS PROCESS.
// It calculates the energy loss of charged hadrons.
// **************************************************************
//
// corrected by V. Grichine on 24/11/97
// corrected by L. Urban    on 27/05/98   ( other corrections come soon!)
// 10/02/00  modifications , new e.m. structure, L.Urban
// 02/03/00  initialisation of theDEDXTable
// 17-09-01, migration of Materials to pure STL (mma)
// 10-03-03 remove tails of old cuts (V.Ivanchenko)
//
 

#include "G4VPAIenergyLoss.hh"
#include "G4PAIonisation.hh"
#include "G4EnergyLossTables.hh"

////////////////////////////////////////////////////////////////////////////
//
// Initialisation of static members 

G4int            G4VPAIenergyLoss::NbOfProcesses      = 1 ;
G4PhysicsTable** G4VPAIenergyLoss::RecorderOfpProcess =
                                           new G4PhysicsTable*[10] ;
G4int       G4VPAIenergyLoss::CounterOfpProcess = 0 ;

G4PhysicsTable* G4VPAIenergyLoss::theDEDXpTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theRangepTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theInverseRangepTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theLabTimepTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theProperTimepTable = NULL ;

G4PhysicsTable* G4VPAIenergyLoss::thepRangeCoeffATable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::thepRangeCoeffBTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::thepRangeCoeffCTable = NULL ;

G4PhysicsTable** G4VPAIenergyLoss::RecorderOfpbarProcess =
                                           new G4PhysicsTable*[10] ;
G4int       G4VPAIenergyLoss::CounterOfpbarProcess = 0 ;

G4PhysicsTable* G4VPAIenergyLoss::theDEDXpbarTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theRangepbarTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theInverseRangepbarTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theLabTimepbarTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::theProperTimepbarTable = NULL ;

G4PhysicsTable* G4VPAIenergyLoss::thepbarRangeCoeffATable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::thepbarRangeCoeffBTable = NULL ;
G4PhysicsTable* G4VPAIenergyLoss::thepbarRangeCoeffCTable = NULL ;

// G4PhysicsTable* G4VPAIenergyLoss::fPAItransferBank = NULL ;

G4PhysicsTable* G4VPAIenergyLoss::theDEDXTable = NULL ;

G4double G4VPAIenergyLoss::LowerBoundEloss= 1.00*keV ;
G4double G4VPAIenergyLoss::UpperBoundEloss= 100.*TeV ;
G4int G4VPAIenergyLoss::NbinEloss =100 ;
G4double G4VPAIenergyLoss::RTable,G4VPAIenergyLoss::LOGRTable;


// constructor and destructor
 
G4VPAIenergyLoss::G4VPAIenergyLoss(const G4String& processName)
   : G4VEnergyLoss (processName),
     dToverTini(0.20),   // max.relative range loss in one Step = 20%
     theElectron ( G4Electron::Electron() ),
     theProton ( G4Proton::Proton() ),
     theAntiProton ( G4AntiProton::AntiProton() )
{
     theLossTable = NULL ;

  //  calculate data members LOGRTable,RTable first
  G4double lrate ;
  lrate = log(UpperBoundEloss/LowerBoundEloss) ;
  LOGRTable=lrate/NbinEloss;
  RTable   =exp(LOGRTable);

}

G4VPAIenergyLoss::~G4VPAIenergyLoss() 
{
     if(theLossTable) {
        theLossTable->clearAndDestroy();
        delete theLossTable;
     }

}

/////////////////////////////////////////////////////////////////////////
//

G4double G4VPAIenergyLoss::GetMaxKineticEnergy() {return UpperBoundEloss;}
G4double G4VPAIenergyLoss::GetMinKineticEnergy() {return LowerBoundEloss;}
G4int    G4VPAIenergyLoss::GetBinNumber()        {return NbinEloss;} 

void  G4VPAIenergyLoss::SetNbOfProcesses(G4int nb) {NbOfProcesses=nb;}
void  G4VPAIenergyLoss::PlusNbOfProcesses()        {NbOfProcesses++ ;}
void  G4VPAIenergyLoss::MinusNbOfProcesses()       {NbOfProcesses-- ;}
G4int G4VPAIenergyLoss::GetNbOfProcesses()         {return NbOfProcesses;}

void G4VPAIenergyLoss::SetLowerBoundEloss(G4double val) {LowerBoundEloss=val;}
void G4VPAIenergyLoss::SetUpperBoundEloss(G4double val) {UpperBoundEloss=val;}
void G4VPAIenergyLoss::SetNbinEloss(G4int nb)		{NbinEloss=nb;}

G4double G4VPAIenergyLoss::GetLowerBoundEloss() {return LowerBoundEloss;}
G4double G4VPAIenergyLoss::GetUpperBoundEloss() {return UpperBoundEloss;}
G4int    G4VPAIenergyLoss::GetNbinEloss()	{return NbinEloss;}
 
/////////////////////////////////////////////////////////////////////////
//
//

void G4VPAIenergyLoss::BuildDEDXTable(const G4ParticleDefinition& aParticleType)
{

  G4bool MakeTable = false ;
     
// Create tables only if there is a new cut value !

  // create/fill proton or antiproton tables depending on the charge of the particle
  G4double Charge = aParticleType.GetPDGCharge();

  if (Charge>0.) 
  {
    theDEDXTable= theDEDXpTable;
  } 
  else 
  {
    theDEDXTable= theDEDXpbarTable;
  }

  if ( CutsWhereModified() || (theDEDXTable==NULL))
  {
    MakeTable = true ;
  }
  
  if( MakeTable )
  {

// Build energy loss table as a sum of the energy loss due to the
//              different processes.                                           
//
//  different processes.                                           

//  create table for the total energy loss

    G4int numOfMaterials = G4Material::GetNumberOfMaterials();

  G4PhysicsTable** RecorderOfProcess;
  int CounterOfProcess;

 if( Charge >0.)    
 {
    RecorderOfProcess=RecorderOfpProcess;
    CounterOfProcess=CounterOfpProcess;

    if(CounterOfProcess == NbOfProcesses)
    {
  // create tables
      if(theDEDXpTable)
      { theDEDXpTable->clearAndDestroy();
        delete theDEDXpTable; }
      theDEDXpTable = new G4PhysicsTable(numOfMaterials);
      theDEDXTable = theDEDXpTable;
    }
  }
  else
  {
    RecorderOfProcess=RecorderOfpbarProcess;
    CounterOfProcess=CounterOfpbarProcess;

    if(CounterOfProcess == NbOfProcesses)
    {
  // create tables
      if(theDEDXpbarTable)
      { theDEDXpbarTable->clearAndDestroy();
        delete theDEDXpbarTable; }
      theDEDXpbarTable = new G4PhysicsTable(numOfMaterials);
      theDEDXTable = theDEDXpbarTable;
    }
  }


  if(CounterOfProcess == NbOfProcesses)
  {
 // fill the tables

//  loop for materials
    G4double LowEdgeEnergy , Value ;
    G4bool isOutRange ;

    G4PhysicsTable* pointer ;


    for (G4int J=0; J<numOfMaterials; J++)
    {
      // create physics vector and fill it

      G4PhysicsLogVector* aVector = new G4PhysicsLogVector(
                    LowerBoundEloss, UpperBoundEloss, NbinEloss);   

      // loop for the kinetic energy
   
      for (G4int i=0; i<NbinEloss; i++)
  
      {
        LowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;      
//     here comes the sum of the different tables created by the  
//     processes (ionisation,etc...)              

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
 
  //  reset counter to zero 

  if( Charge >0.)    CounterOfpProcess=0 ;
  else  CounterOfpbarProcess=0 ;

      ParticleMass = aParticleType.GetPDGMass() ;

      if(Charge > 0.)
      {
       // Build range table
       theRangepTable = BuildRangeTable(theDEDXpTable,
                        theRangepTable,
                        LowerBoundEloss,UpperBoundEloss,NbinEloss);
       // Build lab/proper time tables
       theLabTimepTable = BuildLabTimeTable(theDEDXpTable,
                         theLabTimepTable,
                         LowerBoundEloss,UpperBoundEloss,NbinEloss);
       theProperTimepTable = BuildProperTimeTable(theDEDXpTable,
                            theProperTimepTable,
                            LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build coeff tables for the energy loss calculation
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
       theRangepbarTable = BuildRangeTable(theDEDXpbarTable,
                        theRangepbarTable,
                        LowerBoundEloss,UpperBoundEloss,NbinEloss);
       // Build lab/proper time tables
       theLabTimepbarTable = BuildLabTimeTable(theDEDXpbarTable,
                         theLabTimepbarTable,
                         LowerBoundEloss,UpperBoundEloss,NbinEloss);
       theProperTimepbarTable = BuildProperTimeTable(theDEDXpbarTable,
                            theProperTimepbarTable,
                            LowerBoundEloss,UpperBoundEloss,NbinEloss);

       // Build coeff tables for the energy loss calculation
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
    (Charge>0)?
      theDEDXpTable: theDEDXpbarTable,
    (Charge>0)?
      theRangepTable: theRangepbarTable,
    (Charge>0)?
      theInverseRangepTable: theInverseRangepbarTable,
    (Charge>0)?
      theLabTimepTable: theLabTimepbarTable,
    (Charge>0)?
      theProperTimepTable: theProperTimepbarTable,
    LowerBoundEloss, UpperBoundEloss,
    proton_mass_c2/aParticleType.GetPDGMass(),NbinEloss);

}


//////////////////////////////////////////////////////////////////////
   


























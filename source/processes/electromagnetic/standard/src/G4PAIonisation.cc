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
// $Id: G4PAIonisation.cc,v 1.20 2001-10-24 16:27:44 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// **************************************************************
//
// 17.09.01 migration of Materials to pure STL (mma)
// 28.05.01 V.Ivanchenko minor changes to provide ANSI -wall compilation 
// 17.05.01 V. Grichine, low energy extension down to 10*keV of proton
// 12.07.00 V.Grichine - modifications in BuildPAIonisationTable 
// 11.07.00 V.Grichine - GetRandomEnergyTransfer, and PostStepDoIt
//                        modifications
// 03.07.00 V.Grichine - modifications in AlongStepDoIt
// 08-04-98 remove 'traking cut' of the ionizing particle, MMa
// 30-11-97 V. Grichine
//
// **************************************************************

#include "G4PAIonisation.hh"
#include "G4PAIxSection.hh"

const G4double G4PAIonisation:: LowestKineticEnergy = 10.0*keV ; // 100.0*MeV  ;
const G4double G4PAIonisation::HighestKineticEnergy =  10.*TeV ; // 1000.0*MeV ;  
G4int G4PAIonisation::TotBin = 200  ;  // 50

      // create physics vector and fill it
      
G4PhysicsLogVector* 
G4PAIonisation::fProtonEnergyVector = new G4PhysicsLogVector(LowestKineticEnergy,
							   HighestKineticEnergy,
							   TotBin);
  



//////////////////////////////////////////////////////////////////////////////
//
// constructor and destructor
//
 
G4PAIonisation::G4PAIonisation( const G4String& materialName,
                                const G4String& processName)
   : G4VPAIenergyLoss(processName),
     theElectron ( G4Electron::Electron() )
{
  G4int  iMat ;
  theMeanFreePathTable  = NULL; 
  lastCutInRange = 0 ;
  static const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  G4int numberOfMat = G4Material::GetNumberOfMaterials() ;
  for(iMat=0;iMat<numberOfMat;iMat++)
  {
    if(materialName == (*theMaterialTable)[iMat]->GetName() )
    {
      fMatIndex = (*theMaterialTable)[iMat]->GetIndex() ;
      break ;
    }
  }
  if(iMat == numberOfMat)
  {
    G4Exception("Invalid material name in G4PAIonisation constructor") ;
  }
  ComputeSandiaPhotoAbsCof() ;
  BuildPAIonisationTable() ;


  // G4cout<<"G4PAIonisation constructor is called"<<G4endl ;
}

///////////////////////////////////////////////////////////////////////////
//
//

G4PAIonisation::~G4PAIonisation() 
{
   if ( theMeanFreePathTable )
   {
      theMeanFreePathTable->clearAndDestroy();
      delete theMeanFreePathTable;
   }
   if( fPAItransferBank )    
   {
     fPAItransferBank->clearAndDestroy() ;
     delete fPAItransferBank ;
   }
   for(G4int i=0;i<fSandiaIntervalNumber;i++)
   {
     delete[] fSandiaPhotoAbsCof[i] ;
   }
   delete[] fSandiaPhotoAbsCof ;
 
   //  if(fProtonEnergyVector) delete fProtonEnergyVector ; 
}
 
/////////////////////////////////////////////////////////////////////////
//
//


void G4PAIonisation::ComputeSandiaPhotoAbsCof()
{
   G4int i, j, numberOfElements ;

  static const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // 

   G4SandiaTable thisMaterialSandiaTable(fMatIndex) ;
   numberOfElements = (*theMaterialTable)[fMatIndex]->
                                              GetNumberOfElements() ;
   G4int* thisMaterialZ = new G4int[numberOfElements] ;
   for(i=0;i<numberOfElements;i++)
   {
         thisMaterialZ[i] = (G4int)(*theMaterialTable)[fMatIndex]->
                                      GetElement(i)->GetZ() ;
   }
   fSandiaIntervalNumber = thisMaterialSandiaTable.SandiaIntervals
                           (thisMaterialZ,numberOfElements) ;
   
   fSandiaIntervalNumber = thisMaterialSandiaTable.SandiaMixing
                           ( thisMaterialZ ,
                             (*theMaterialTable)[fMatIndex]->GetFractionVector() ,
        		     numberOfElements,fSandiaIntervalNumber) ;
   
   fSandiaPhotoAbsCof = new G4double*[fSandiaIntervalNumber] ;

   for(i=0;i<fSandiaIntervalNumber;i++)
   {
     fSandiaPhotoAbsCof[i] = new G4double[5] ;
   }
   for(i=0;i<fSandiaIntervalNumber;i++)
   {
      fSandiaPhotoAbsCof[i][0] = thisMaterialSandiaTable.
                                  GetPhotoAbsorpCof(i+1,0) ; // keV ;

                                               // G4double energyCof = keV ;
      for(j=1;j<5;j++)
      {
           fSandiaPhotoAbsCof[i][j] = thisMaterialSandiaTable.
	                              GetPhotoAbsorpCof(i+1,j)*
                 (*theMaterialTable)[fMatIndex]->GetDensity() ;
	    // *(cm2/g)*energyCof ;
	    // energyCof *= keV ;
      }
   }
   delete[] thisMaterialZ ;
}


 
////////////////////////////////////////////////////////////////////////
//
//  just call BuildLossTable+BuildLambdaTable
//

void 
G4PAIonisation::BuildPhysicsTable(const G4ParticleDefinition& aParticleType)

{
    G4double Charge = aParticleType.GetPDGCharge();
    //G4double Chargesquare = Charge*Charge ;     
    CutInRange = aParticleType.GetLengthCuts(); 

    //  BuildLossTable(aParticleType) ;
 
    if(Charge>0.)
    {
       RecorderOfpProcess[CounterOfpProcess] = (*this).theLossTable ;
       CounterOfpProcess++;
    }
    else
    {
       RecorderOfpbarProcess[CounterOfpbarProcess] = (*this).theLossTable ;
       CounterOfpbarProcess++;
    }
    if( !EqualCutVectors(CutInRange, lastCutInRange))
    {
       lastCutInRange =  CopyCutVectors(lastCutInRange,CutInRange) ;
       BuildLambdaTable(aParticleType) ;
    }
    //  G4VPAIenergyLoss::BuildDEDXTable(aParticleType) ;
}

////////////////////////////////////////////////////////////////////////////
//
// Build tables for the ionization energy loss
//  the tables are built for MATERIALS
//                           *********

void
G4PAIonisation::BuildPAIonisationTable()
{
   G4double LowEdgeEnergy , ionloss ;
   G4double massRatio, tau, Tmax, Tmin, Tkin, deltaLow, gamma, bg2 ;

   if ( theLossTable)
   {
      theLossTable->clearAndDestroy();
      delete theLossTable;
   }
   theLossTable = new G4PhysicsTable();
   
   if( fPAItransferBank )    
   {
     fPAItransferBank->clearAndDestroy() ;
     delete fPAItransferBank ;
   }
   fPAItransferBank = new G4PhysicsTable(TotBin) ;
     
   //create physics vector then fill it ....

   G4PhysicsLogVector* aVector = new G4PhysicsLogVector( LowestKineticEnergy, 
							 HighestKineticEnergy,
							 TotBin               ) ;

   

   DeltaCutInKineticEnergyNow = 100*keV ; // From gas detector experience

   Tmin = fSandiaPhotoAbsCof[0][0] ;      // low energy Sandia interval

   //   G4cout<<"Tmin = "<<Tmin/eV<<" eV"<<G4endl<<G4endl ;

   deltaLow = 0.5*eV ;                    // shift from Tmin for stability

   for (G4int i = 0 ; i < TotBin ; i++)  //The loop for the kinetic energy 
   {
      LowEdgeEnergy = fProtonEnergyVector->GetLowEdgeEnergy(i) ;
      
      tau = LowEdgeEnergy/proton_mass_c2 ;

      //    if(tau < 0.01)  tau = 0.01 ;
 
      gamma = tau +1. ;

      //   G4cout<<"gamma = "<<gamma<<endl ;

      bg2 = tau*(tau + 2. ) ;
      massRatio = electron_mass_c2/proton_mass_c2 ;

      Tmax = 2.*electron_mass_c2*bg2/(1.+2.*gamma*massRatio+massRatio*massRatio) ;

      Tkin = DeltaCutInKineticEnergyNow ;

      if ( DeltaCutInKineticEnergyNow > Tmax)         // was <
      {
         Tkin = Tmax ;
      }
      if ( Tkin < Tmin + deltaLow )  // low energy safety       
      {
         Tkin = Tmin + deltaLow ;
      }
      G4PAIxSection protonPAI( fMatIndex,
                               Tkin,
                               bg2,
                               fSandiaPhotoAbsCof,
                               fSandiaIntervalNumber  ) ;
	    
      ionloss = protonPAI.GetMeanEnergyLoss() ;   //  total <dE/dx>

   // G4cout<<"ionloss = "<<ionloss*cm/keV<<" keV/cm"<<endl ;
   // G4cout<<"n1 = "<<protonPAI.GetIntegralPAIxSection(1)*cm<<" 1/cm"<<endl ;
	    // G4cout<<"protonPAI.GetSplineSize() = "<<
            // protonPAI.GetSplineSize()<<G4endl ;

      G4PhysicsFreeVector* transferVector = new 
                             G4PhysicsFreeVector(protonPAI.GetSplineSize()) ;

      for(G4int k=0;k<protonPAI.GetSplineSize();k++)
      {
         transferVector->PutValue( k ,
                                        protonPAI.GetSplineEnergy(k+1),
                                        protonPAI.GetIntegralPAIxSection(k+1) ) ;
      }  
      if ( ionloss <= 0.)  ionloss = DBL_MIN ;

      aVector->PutValue(i,ionloss) ;

      fPAItransferBank->insertAt(i,transferVector) ;

            // delete[] transferVector ;
    }                                        // end of Tkin loop
    theLossTable->insert(aVector);
                                              // end of material loop
   // G4cout<<"G4PAIonisation::BuildPAIonisationTable() have been called"<<G4endl ;
   // G4cout<<"G4PAIonisation::BuildLossTable() have been called"<<G4endl ;
}

///////////////////////////////////////////////////////////////////////
//
// Build mean free path tables for the delta ray production process
//     tables are built for MATERIALS 
//

void 
G4PAIonisation::BuildLambdaTable(const G4ParticleDefinition& aParticleType)
{
    G4double LowEdgeEnergy , Value ,sigma ;
    //G4bool isOutRange ;
    const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
    const G4double BigValue = DBL_MAX ;

    G4int numOfMaterials = G4Material::GetNumberOfMaterials();    //create table

    if (theMeanFreePathTable) 
    {
       theMeanFreePathTable->clearAndDestroy();
       delete theMeanFreePathTable;
    }
    theMeanFreePathTable = new G4PhysicsTable(numOfMaterials);

    // get electron and particle cuts in kinetic energy

    DeltaCutInKineticEnergy = theElectron->GetEnergyCuts() ;
    ParticleCutInKineticEnergy = aParticleType.GetEnergyCuts() ;

    for (G4int J=0 ; J < numOfMaterials; J++)  // loop for materials 
    { 
       //create physics vector then fill it ....

       G4PhysicsLogVector* aVector = new G4PhysicsLogVector( LowestKineticEnergy, 
							     HighestKineticEnergy,
							     TotBin             ) ;

       // compute the (macroscopic) cross section first
 
       const G4Material* material= (*theMaterialTable)[J] ;
        
       const G4ElementVector* theElementVector= material->GetElementVector() ;
       const G4double* theAtomicNumDensityVector =
                         material->GetAtomicNumDensityVector();
       const G4int NumberOfElements = material->GetNumberOfElements() ;
 
       // get the electron kinetic energy cut for the actual material,
       //  it will be used in ComputeMicroscopicCrossSection
       // ( it is the SAME for ALL the ELEMENTS in THIS MATERIAL )

       DeltaCutInKineticEnergyNow = DeltaCutInKineticEnergy[J] ;

       for ( G4int i = 0 ; i < TotBin ; i++ )
       {
          LowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;
          sigma = 0. ;
          for (G4int iel=0; iel<NumberOfElements; iel++ )
          {
              sigma +=  theAtomicNumDensityVector[iel]*
                        ComputeMicroscopicCrossSection(aParticleType,
                        LowEdgeEnergy,
                       (*theElementVector)[iel]->GetZ() ) ;
          }
          // mean free path = 1./macroscopic cross section

           Value = sigma <= 0 ? BigValue: 1./sigma ;     

           aVector->PutValue(i, Value) ;
        }
        theMeanFreePathTable->insert(aVector);
    }
}

////////////////////////////////////////////////////////////////////////////
//
// Cross section formula is OK for spin=0 and 1/2 only !
// Calculates the microscopic cross section in GEANT4 internal units
// ( it is called for elements , AtomicNumber = Z )
//

G4double 
G4PAIonisation::
ComputeMicroscopicCrossSection( const G4ParticleDefinition& aParticleType,
                                      G4double KineticEnergy ,
                                      G4double AtomicNumber               )
{
    G4double TotalEnergy, betasquare,
             MaxKineticEnergyTransfer,
	     TotalCrossSection, tempvar ;
    const G4double SmallCrossSection = DBL_MIN;

    ParticleMass=aParticleType.GetPDGMass() ; // get particle data 
    TotalEnergy=KineticEnergy + ParticleMass;

    betasquare = KineticEnergy*(TotalEnergy+ParticleMass)   //  kinematics
                 /(TotalEnergy*TotalEnergy);

    tempvar = ParticleMass+electron_mass_c2;

    MaxKineticEnergyTransfer = 2.*electron_mass_c2*KineticEnergy
                     *(TotalEnergy+ParticleMass)
                     /(tempvar*tempvar+2.*electron_mass_c2*KineticEnergy);

    //  total cross section

    if( MaxKineticEnergyTransfer > DeltaCutInKineticEnergyNow )
    {
       tempvar=DeltaCutInKineticEnergyNow/MaxKineticEnergyTransfer;
       TotalCrossSection = (1.-tempvar*(1.-betasquare*log(tempvar)))
                           /DeltaCutInKineticEnergyNow;
                           // +term for spin=1/2 particle
       if(aParticleType.GetPDGSpin() == 1)
       {
          TotalCrossSection +=  0.5
                            *(MaxKineticEnergyTransfer-DeltaCutInKineticEnergyNow)
                            /(TotalEnergy*TotalEnergy);
       }
       TotalCrossSection = twopi_mc2_rcl2 * AtomicNumber
                           *TotalCrossSection/betasquare;
    }
    else
    {
       TotalCrossSection=SmallCrossSection ;
    }
    return TotalCrossSection ;
}
 
///////////////////////////////////////////////////////////////////////////
//
// Units are expressed in GEANT4 internal units.
//
 
G4VParticleChange* 
G4PAIonisation::PostStepDoIt( const G4Track& trackData,   
                              const G4Step& stepData          )         
{
    G4double kinE,  massRatio, scaledTkin, energyTransfer, finalTkin ;

    aParticleChange.Initialize(trackData) ;
    const G4DynamicParticle* aParticle = trackData.GetDynamicParticle() ;

    if( trackData.GetMaterial()->GetIndex() != fMatIndex )
    {
      return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);
    }    
    kinE = aParticle->GetKineticEnergy() ;
    massRatio = proton_mass_c2/aParticle->GetDefinition()->GetPDGMass() ;
    scaledTkin = kinE*massRatio ;
    energyTransfer = GetRandomEnergyTransfer(scaledTkin) ;
    if( energyTransfer < 0.0 )
    {
      //  G4cout<<"PAI::energyTransfer = "<<energyTransfer/keV<<" keV"<<G4endl ;
      energyTransfer = 0.0 ;
    }
    finalTkin = kinE - energyTransfer ;

    //  kill the particle if the kinetic energy <= 0  

    if (finalTkin < 0.0 )
    {
      finalTkin = 0.0;
      if (aParticle->GetDefinition()->GetParticleName() == "proton")
      {
               aParticleChange.SetStatusChange( fStopAndKill ) ;
      }
      else     aParticleChange.SetStatusChange( fStopButAlive ) ; 
    } 
    aParticleChange.SetNumberOfSecondaries(0) ;
    aParticleChange.SetEnergyChange( finalTkin ) ;
    aParticleChange.SetLocalEnergyDeposit (energyTransfer) ;
      
    return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);
    //  return &aParticleChange ;
}


/////////////////////////////////////////////////////////////////////////
//
// compute the energy loss after a Step 
//

G4VParticleChange* G4PAIonisation::AlongStepDoIt( const G4Track& trackData,
                                                   const G4Step& stepData    ) 
{
  //  G4cout<<"G4PAIonisation::AlongStepDoIt is called"<<G4endl ;
  
  const G4DynamicParticle* aParticle;
  G4Material* aMaterial;
  //G4bool isOut;
  //G4double E,ScaledE,finalT,Step,Tbin,rangebin, delta ;
  G4double E,ScaledE,finalT,Step,delta;
  //const G4double smallLoss=DBL_MIN;
  const G4double BigRange = DBL_MAX ;
  //G4int index ;
  //G4double cc,discr ; 

  aParticleChange.Initialize(trackData) ;
  aMaterial = trackData.GetMaterial() ;
  size_t index = aMaterial->GetIndex() ;

  // get the actual (true) Step length from stepData 
  // there is no loss for Step=0. !

  Step = stepData.GetStepLength() ;

  if( Step == 0. || index != fMatIndex ) return &aParticleChange ; 
 
  //  G4cout<<"step = "<<Step/mm<<" mm"<<endl ;

  // get particle and material pointers from trackData
 
  aParticle = trackData.GetDynamicParticle() ;


  E = aParticle->GetKineticEnergy() ;

  G4double Charge = aParticle->GetDefinition()->GetPDGCharge() ;

  G4double Chargesquare = Charge*Charge ;

  G4double MassRatio = proton_mass_c2/aParticle->GetDefinition()->GetPDGMass() ;

  ScaledE = E*MassRatio ;

  ParticleCutInKineticEnergyNow =
               (aParticle->GetDefinition()->GetEnergyCuts())[index] ;

  if(Step >= BigRange)
  {
    finalT = E ;
    fMeanLoss = 0. ;
  }
  else  // here comes the 'real' energy loss calculation (material is NOT vacuum)
  {
	  //  fMeanLoss = ScaledE-0.5*(discr-RangeCoeffB)/RangeCoeffA ;

          //  now the loss with fluctuation
   delta  = GetLossWithFluct(Step,aParticle,aMaterial) ;
   if ( delta < 0.0 )
   {
     //     G4cout<<"PAI::delta = "<<delta/keV<<" keV"<<G4endl ;
     delta = 0.0 ;
   }
   finalT = E - delta ;

   if (finalT < 0.0) finalT = 0. ;

   fMeanLoss *= Chargesquare ;
    
  }
  //  kill the particle if the kinetic energy <= 0  

  if (finalT <= 0. )
    {
      finalT = 0.;
      if (aParticle->GetDefinition()->GetParticleName() == "proton")
               aParticleChange.SetStatusChange(fStopAndKill);
      else     aParticleChange.SetStatusChange(fStopButAlive); 
    } 

  aParticleChange.SetNumberOfSecondaries(0);
  aParticleChange.SetEnergyChange( finalT ) ;
  aParticleChange.SetLocalEnergyDeposit(E-finalT) ;

  return &aParticleChange ;

}

///////////////////////////////////////////////////////////////////////
//
// Returns random energy loss from step

G4double  
G4PAIonisation::GetLossWithFluct( G4double Step,
                                   const G4DynamicParticle* aParticle,
                                   G4Material* aMaterial               )
{  
  G4int iTkin, iTransfer  ;
  G4long numOfCollisions;
  //  G4long iCollision, numOfCollisions ;
  //G4int       index = aMaterial->GetIndex() ;
  //G4bool isOutRange ;

  // G4cout<<"G4VPAIenergyLoss::GetLossWithFluct"<<G4endl ;

  G4double loss = 0.0, charge2 ;
  //G4double transfer, position, E1, E2, W1, W2, W, firstMu, secondMu ;
  G4double position, E1, E2, W1, W2, W;
  G4double      Tkin = aParticle->GetKineticEnergy() ;
  G4double MassRatio = proton_mass_c2/aParticle->GetDefinition()->GetPDGMass() ;
  G4double charge = aParticle->GetDefinition()->GetPDGCharge() ;
  charge2         = charge*charge ;
  G4double TkinScaled = Tkin*MassRatio ;

  for(iTkin=0;iTkin<G4PAIonisation::GetBinNumber();iTkin++)
  {
    if(TkinScaled < fProtonEnergyVector->GetLowEdgeEnergy(iTkin)) // <= ?
    {
      break ;
    } 
  }
  G4int iPlace = iTkin - 1 ; // index*(G4PAIonisation::GetBinNumber()) +

  //  G4cout<<"iPlace = "<<iPlace<<endl ;

  if(iTkin == G4PAIonisation::GetBinNumber()) // Fermi plato, try from left
  {
    numOfCollisions = RandPoisson::
    shoot((*(*fPAItransferBank)(iPlace))(0)*Step*charge2) ;
    
    //     G4cout<<"numOfCollisions = "<<numOfCollisions<<G4endl ;

    while(numOfCollisions)
    {
      position = (*(*fPAItransferBank)(iPlace))(0)*G4UniformRand() ;

      for(iTransfer=0;;iTransfer++)
      {
        if(position >= (*(*fPAItransferBank)(iPlace))(iTransfer)) break ;
      }
      //  loss += (*fPAItransferBank)(iPlace)->GetLowEdgeEnergy(iTransfer) ;
      loss += GetEnergyTransfer(iPlace,position,iTransfer);
      numOfCollisions-- ;
    }
  }
  else
  {
    if(iTkin == 0) // Tkin is too small, trying from right only
    {
      numOfCollisions = RandPoisson::
                        shoot((*(*fPAItransferBank)(iPlace+1))(0)*Step*charge2) ;

      //  G4cout<<"numOfCollisions = "<<numOfCollisions<<G4endl ;

      while(numOfCollisions)
      {
        position = (*(*fPAItransferBank)(iPlace+1))(0)*G4UniformRand() ;

        for(iTransfer=0;;iTransfer++)
        {
          if(position >= (*(*fPAItransferBank)(iPlace+1))(iTransfer)) break ;
        }
	//  loss += (*fPAItransferBank)(iPlace+1)->GetLowEdgeEnergy(iTransfer) ;
        loss += GetEnergyTransfer(iPlace+1,position,iTransfer);
        numOfCollisions-- ;
      }
    } 
    else // general case: Tkin between two vectors of the material
    {
      E1 = fProtonEnergyVector->GetLowEdgeEnergy(iTkin - 1) ; 
      E2 = fProtonEnergyVector->GetLowEdgeEnergy(iTkin)     ;
       W = 1.0/(E2 - E1) ;
      W1 = (E2 - TkinScaled)*W ;
      W2 = (TkinScaled - E1)*W ;

      // G4cout<<"(*(*fPAItransferBank)(iPlace))(0) = "<<
      //   (*(*fPAItransferBank)(iPlace))(0)<<G4endl ;
      // G4cout<<"(*(*fPAItransferBank)(iPlace+1))(0) = "<<
      //     (*(*fPAItransferBank)(iPlace+1))(0)<<G4endl ;

      numOfCollisions = RandPoisson::shoot(
                     ( (*(*fPAItransferBank)(iPlace))(0)*W1 + 
                     (*(*fPAItransferBank)(iPlace+1))(0)*W2 )*Step*charge2) ;

      //  G4cout<<"numOfCollisions = "<<numOfCollisions<<endl ;

      while(numOfCollisions)
      {
        position =( (*(*fPAItransferBank)(iPlace))(0)*W1 + 
                    (*(*fPAItransferBank)(iPlace+1))(0)*W2 )*G4UniformRand() ;

        // G4cout<<position<<"\t" ;

        for(iTransfer=0;;iTransfer++)
        {
          if( position >=
          ( (*(*fPAItransferBank)(iPlace))(iTransfer)*W1 + 
            (*(*fPAItransferBank)(iPlace+1))(iTransfer)*W2) )
          {
	      break ;
	  }
        }
	// loss += (*fPAItransferBank)(iPlace)->GetLowEdgeEnergy(iTransfer) ; 
        loss += GetEnergyTransfer(iPlace,position,iTransfer);
        numOfCollisions-- ;    
      }
    }
  } 
  //  G4cout<<"PAI loss = "<<loss/keV<<" keV"<<endl ; 

  return loss ;
}

///////////////////////////////////////////////////////////////////////
//
// Returns random PAI energy transfer according to passed scaled kinetic
// energy of particle

G4double  
G4PAIonisation::GetRandomEnergyTransfer( G4double scaledTkin )
{  
  // G4cout<<"G4VPAIenergyLoss::GetRandomEnergyTransfer"<<G4endl ;

  G4int iTkin, iTransfer, iPlace  ;
  G4double transfer = 0.0, position, E1, E2, W1, W2, W ;

  for(iTkin=0;iTkin<G4PAIonisation::GetBinNumber();iTkin++)
  {
    if(scaledTkin < fProtonEnergyVector->GetLowEdgeEnergy(iTkin))  break ;
  }
  iPlace = iTkin - 1 ;

  //  G4cout<<"iPlace = "<<iPlace<<endl ;

  if(iTkin == G4PAIonisation::GetBinNumber()) // Fermi plato, try from left
  {
      position = (*(*fPAItransferBank)(iPlace))(0)*G4UniformRand() ;

      for(iTransfer=0;;iTransfer++)
      {
        if(position >= (*(*fPAItransferBank)(iPlace))(iTransfer)) break ;
      }
      //  transfer = (*fPAItransferBank)(iPlace)->GetLowEdgeEnergy(iTransfer) ;
      transfer = GetEnergyTransfer(iPlace,position,iTransfer);
  }
  else
  {
    if(iTkin == 0) // Tkin is too small, trying from right only
    {
      position = (*(*fPAItransferBank)(iPlace+1))(0)*G4UniformRand() ;

      for(iTransfer=0;;iTransfer++)
      {
        if(position >= (*(*fPAItransferBank)(iPlace+1))(iTransfer)) break ;
      }
      // transfer = (*fPAItransferBank)(iPlace+1)->GetLowEdgeEnergy(iTransfer) ;
      transfer = GetEnergyTransfer(iPlace+1,position,iTransfer);
    } 
    else // general case: Tkin between two vectors of the material
    {
      E1 = fProtonEnergyVector->GetLowEdgeEnergy(iTkin - 1) ; 
      E2 = fProtonEnergyVector->GetLowEdgeEnergy(iTkin)     ;
      W  = 1.0/(E2 - E1) ;
      W1 = (E2 - scaledTkin)*W ;
      W2 = (scaledTkin - E1)*W ;

      position =( (*(*fPAItransferBank)(iPlace))(0)*W1 + 
                    (*(*fPAItransferBank)(iPlace+1))(0)*W2 )*G4UniformRand() ;

        // G4cout<<position<<"\t" ;

      for(iTransfer=0;;iTransfer++)
      {
          if( position >=
          ( (*(*fPAItransferBank)(iPlace))(iTransfer)*W1 + 
            (*(*fPAItransferBank)(iPlace+1))(iTransfer)*W2) ) break ;
      }
      //  transfer = (*fPAItransferBank)(iPlace)->GetLowEdgeEnergy(iTransfer) ; 
      transfer = GetEnergyTransfer(iPlace,position,iTransfer);
    }
  } 
  //  G4cout<<"PAI transfer = "<<transfer/keV<<" keV"<<endl ; 
  if(transfer < 0.0 ) transfer = 0.0 ;
  return transfer ;
}

///////////////////////////////////////////////////////////////////////
//
// Returns random PAI energy transfer according to passed scaled kinetic
// energy of particle

G4double  
G4PAIonisation::GetEnergyTransfer( G4int iPlace, G4double position, G4int iTransfer )
{ 
  G4double x1, x2, y1, y2, result ;

  if(iTransfer == 0)
  {
    result = (*fPAItransferBank)(iPlace)->GetLowEdgeEnergy(iTransfer) ;
  }  
  else
  {
    y1 = (*(*fPAItransferBank)(iPlace))(iTransfer-1) ;
    y2 = (*(*fPAItransferBank)(iPlace))(iTransfer) ;

    x1 = (*fPAItransferBank)(iPlace)->GetLowEdgeEnergy(iTransfer-1) ;
    x2 = (*fPAItransferBank)(iPlace)->GetLowEdgeEnergy(iTransfer) ;

    if ( x1 == x2 )    result = x2 ;
    else
    {
      if ( y1 == y2  ) result = x1 + (x2 - x1)*G4UniformRand() ;
      else
      {
        result = x1 + (position - y1)*(x2 - x1)/(y2 - y1) ;
      }
    }
  }
  return result ;
}

//
//
/////////////////////////////////////////////////////////////////////////












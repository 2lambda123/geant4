// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4LowEnergyIonisation.cc,v 1.48 2001-05-25 17:26:16 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// -------------------------------------------------------------
//      GEANT 4 class implementation file 
//
//      For information related to this code contact:
//      CERN, IT Division, ASD group
//      --- G4LowEnergyIonisation physics process for electrons
//                by Alessandra Forti May 1999  
// **************************************************************
// - 11.04.2000 VL
//   Changing use of float and G4float casts to G4double casts 
//   because of problems with optimisation (bug ?)
//   10.04.2000 VL
// - Correcting Fluorescence transition probabilities in order to take into account 
//   non-radiative transitions. No Auger electron simulated yet: energy is locally deposited.
//   10.04.2000 VL
// - Correction of incident electron final momentum direction
//   07.04.2000 VL+LU
// - First implementation of continuous energy loss
//   22.03.2000 VL
// - 1 bug corrected in SelectRandomAtom method (units)
//   17.02.2000 Veronique Lefebure
// - 5 bugs corrected: 
//   *in Fluorescence, 2 bugs affecting 
//   . localEnergyDeposition and
//   . number of emitted photons that was then always 1 less
//   *in EnergySampling method: 
//   . expon = Parms[13]+1; (instead of uncorrect -1)
//   . rejection /= Parms[6];(instead of uncorrect Parms[7])
//   . Parms[6] is apparently corrupted in the data file (often = 0)  
//     -->Compute normalisation into local variable rejectionMax
//     and use rejectionMax  in stead of Parms[6]
//
// Added Livermore data table construction methods A. Forti
// Modified BuildMeanFreePath to read new data tables A. Forti
// Added EnergySampling method A. Forti
// Modified PostStepDoIt to insert sampling with EEDL data A. Forti
// Added SelectRandomAtom A. Forti
// Added map of the elements A. Forti
// 20.09.00 V.Ivanchenko update fluctuations 
// 24.04.01 V.Ivanchenko remove RogueWave 
// 22.05.01 V.Ivanchenko update calculation of delta-ray kinematic + 
//                       clean up the code 
//                                                                 
// --------------------------------------------------------------
 
// This Class Header
#include "G4LowEnergyIonisation.hh"

// Collaborating Class Headers
#include "G4EnergyLossTables.hh"
#include "G4Gamma.hh"
#include "G4ProcessManager.hh"
#include "G4UnitsTable.hh"
#include "g4std/fstream"

typedef G4std::vector<G4DynamicParticle*> G4ParticleVector;

//    
//    

G4LowEnergyIonisation::G4LowEnergyIonisation(const G4String& processName)
   : G4eLowEnergyLoss(processName),
     allAtomShellCrossSec(0),
     theFluorTransitionTable(0),
     theSamplingCoeffTable(0),
     theBindingEnergyTable(0),
     ZNumVec(0),
     ZNumVecFluor(0),
     theMeanFreePathTable(0),
     CutForLowEnergySecondaryPhotons(0.),
     CutForLowEnergySecondaryElectrons(0.),
     MeanFreePath(0.)
{ 
    lowestKineticEnergy  = GetLowerBoundEloss();
    highestKineticEnergy = GetUpperBoundEloss();
    TotBin = GetNbinEloss();
}

//    

G4LowEnergyIonisation::~G4LowEnergyIonisation() 
{
     if (theMeanFreePathTable) {
        theMeanFreePathTable->clearAndDestroy();
        delete theMeanFreePathTable;
     }

  if (allAtomShellCrossSec) {

    delete allAtomShellCrossSec;
  }

  if (theBindingEnergyTable) {

    delete theBindingEnergyTable;
  }

  if (theFluorTransitionTable) {

    delete theFluorTransitionTable;
  }

  if(theSamplingCoeffTable){

    delete theSamplingCoeffTable;
  }

  if(ZNumVec){

    ZNumVec->clear();
    delete ZNumVec;
  }

  if(ZNumVecFluor){

    ZNumVecFluor->clear();
    delete ZNumVecFluor;
  }
}

//    

void G4LowEnergyIonisation::SetCutForLowEnSecPhotons(G4double cut)
{
  CutForLowEnergySecondaryPhotons = cut;
}

//    

void G4LowEnergyIonisation::SetCutForLowEnSecElectrons(G4double cut){

  CutForLowEnergySecondaryElectrons = cut;
}

//    

void G4LowEnergyIonisation::BuildPhysicsTable(const G4ParticleDefinition& aParticleType)
//  just call BuildLossTable+BuildLambdaTable
{
  
  BuildZVec();

  BuildShellCrossSectionTable();
  
  BuildFluorTransitionTable();
  
  BuildBindingEnergyTable();
  
  BuildSamplingCoeffTable();

  BuildLossTable(aParticleType) ;


  if(&aParticleType==G4Electron::Electron())
  {
   RecorderOfElectronProcess[CounterOfElectronProcess] = (*this).theLossTable ;
   CounterOfElectronProcess++;
   PrintInfoDefinition();  
   }
   else
   {
    RecorderOfPositronProcess[CounterOfPositronProcess] = (*this).theLossTable ;
    CounterOfPositronProcess++;
   }

 
   BuildLambdaTable(aParticleType) ;
 
   BuildDEDXTable(aParticleType);
 
 
}

//    

void G4LowEnergyIonisation::BuildLossTable(const G4ParticleDefinition& aParticleType) 
{
  // Build tables for the ionization energy loss
  //  the tables are built for *MATERIALS*

  G4double LowEdgeEnergy; 
  G4double ionloss = 0.0;
  
  // material properties
  ParticleMass = aParticleType.GetPDGMass();
  
  //  create table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  G4int numOfMaterials = theMaterialTable->length();
  
  if (theLossTable) { 
     theLossTable->clearAndDestroy();
     delete theLossTable;
  }
  
  theLossTable = new G4PhysicsTable(numOfMaterials);
  
  //  loop for materials
  
  for (G4int J=0; J<numOfMaterials; J++){
    
      // create physics vector and fill it
      G4PhysicsLogVector* aVector = new G4PhysicsLogVector(lowestKineticEnergy,
			  highestKineticEnergy,TotBin);
      // get material parameters needed for the energy loss calculation
      const G4Material* material= (*theMaterialTable)[J];

      G4double Tcut = G4Electron::Electron()
                    ->GetCutsInEnergy()[material->GetIndex()] ;
      G4cout<<"*** LEion using Tcut ="<<Tcut<<G4endl;
      const G4ElementVector* theElementVector = material->GetElementVector();
      const G4int NumberOfElements = material->GetNumberOfElements() ;
      const G4double* theAtomicNumDensityVector = material->
                          GetAtomicNumDensityVector() ;
      
      // now comes the loop for the kinetic energy values
      for (G4int i = 0 ; i < TotBin ; i++) {
        LowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;

        ionloss = 0. ;

        // electron
        if (&aParticleType==G4Electron::Electron()) {

            // loop for elements in the material
            for (G4int iel=0; iel<NumberOfElements; iel++ ) {
              G4double Z = (*theElementVector)(iel)->GetZ();

              const oneAtomTable* oneAtomCS
                    = (*allAtomShellCrossSec)[ZNumVec->index(Z)];

              // contributions from the subshells 
              for(size_t ish=0; ish<oneAtomCS->size(); ish++) {
                ionloss += GetShellEnergyLosswithCut(Z,ish,LowEdgeEnergy,Tcut)*
                           theAtomicNumDensityVector[iel] ;
              }
            }
	   
        }
        if (ionloss <= 0.) ionloss = 0.;
        aVector->PutValue(i,ionloss) ;
      }

      theLossTable->insert(aVector);
  }

}

//    

// CONSTRUCT THE CS TABLE FOR THE ELEMENTS MAPPED IN ZNUMVEC USING EEDL DATA
void G4LowEnergyIonisation::BuildShellCrossSectionTable(){

   if (allAtomShellCrossSec) {

    delete allAtomShellCrossSec;
   }

   allAtomShellCrossSec = new allAtomTable();
   G4int dataNum = 2;
 
   for(size_t TableInd = 0; TableInd < ZNumVec->size(); TableInd++){

     G4int AtomInd = (G4int) (*ZNumVec)[TableInd];

     oneAtomTable* oneAtomShellCS = util.BuildSecondLevelTables(AtomInd, dataNum, "ioni/ion-ss-cs-");
     
     //     allAtomShellCrossSec->insert(oneAtomShellCS);
     allAtomShellCrossSec->push_back(oneAtomShellCS);
   
   }//end for on atoms
}
// CONSTRUCT THE CROSS SECTION TABLE FOR ALL THE ELEMENTS
void G4LowEnergyIonisation::BuildBindingEnergyTable(){

  if (theBindingEnergyTable) {

    delete theBindingEnergyTable;
  }

  G4int dataNum = 2;
  theBindingEnergyTable = util.BuildSecondLevelTables(0,dataNum,"fluor/binding");
}

//    
// CONSTRUCT THE TABLE FOR THE ELEMENTS MAPPED IN ZNUMVECFLUOR DIFFERENT 
// FROM ZNUMVEC BECAUSE THERE IS NO FLUORESCENCE FOR THE FIRST 5 ELEMENTS. 
void G4LowEnergyIonisation::BuildFluorTransitionTable(){

  if (theFluorTransitionTable) {
    
    delete theFluorTransitionTable;
  }
  
  theFluorTransitionTable = new allAtomTable();
  ZNumVecFluor = new G4DataVector(*ZNumVec);
  G4int dataNum = 3;
  
  for(size_t TableInd = 0; TableInd < ZNumVec->size(); TableInd++){
    G4int AtomInd = (G4int) (*ZNumVec)[TableInd];
    if(AtomInd > 5){
      
      oneAtomTable* oneAtomShellFL = util.BuildSecondLevelTables(AtomInd, dataNum, "fluor/fl-tr-pr-");
      //      theFluorTransitionTable->insert(oneAtomShellFL);
      theFluorTransitionTable->push_back(oneAtomShellFL);
    }
    else{
      ZNumVecFluor->remove(AtomInd);
    }
  }//end for on atoms
}


//    
// BUILD THE TABLE OF COEFFICIENT FOR THE ELEMENTS MAPPED IN ZNUMVEC
void G4LowEnergyIonisation::BuildSamplingCoeffTable(){

  if (theSamplingCoeffTable) {
    
    delete theSamplingCoeffTable;
  }
  
  theSamplingCoeffTable = new allAtomTable();
  
  G4int dataNum = 17;

  for(size_t TableInd = 0; TableInd < ZNumVec->size(); TableInd++){
    G4int AtomInd = (G4int) (*ZNumVec)[TableInd];
    oneAtomTable* oneAtomShellSc = util.BuildSecondLevelTables(AtomInd, 
                                   dataNum, "ioni/io-co-");
    
    theSamplingCoeffTable->push_back(oneAtomShellSc);

  }//end for on atoms
}


//    
//
// vector mapping the elements of the material table 
// 
void G4LowEnergyIonisation::BuildZVec(){

  const G4MaterialTable* theMaterialTable=G4Material::GetMaterialTable();
  G4int numOfMaterials = theMaterialTable->length();

  if(ZNumVec){

    ZNumVec->clear();
    delete ZNumVec;
  }

  ZNumVec = new G4DataVector(); 
  for (G4int J=0 ; J < numOfMaterials; J++){ 
 
    const G4Material* material= (*theMaterialTable)[J];        
    const G4ElementVector* theElementVector = material->GetElementVector();
    const G4int NumberOfElements = material->GetNumberOfElements() ;

    for (G4int iel=0; iel<NumberOfElements; iel++ ){

      G4double Zel = (*theElementVector)(iel)->GetZ();

      if(!ZNumVec->contains(Zel)) ZNumVec->push_back(Zel);
    }
  }
}

//    

void G4LowEnergyIonisation::BuildLambdaTable(const G4ParticleDefinition& aParticleType)
{
  // Build mean free path tables for the delta ray production process
  //     tables are built for MATERIALS 

  G4double LowEdgeEnergy, Value, SIGMA;

  //create table
  const G4MaterialTable* theMaterialTable=G4Material::GetMaterialTable();
  G4int numOfMaterials = theMaterialTable->length();

  if (theMeanFreePathTable) { theMeanFreePathTable->clearAndDestroy();
                              delete theMeanFreePathTable;
                            }

  theMeanFreePathTable = new G4PhysicsTable(numOfMaterials);

  // get electron  cuts in kinetic energy
  G4double* DeltaCutInKineticEnergy=G4Electron::Electron()->GetCutsInEnergy();
 

  // loop for materials 

  for (G4int J=0 ; J < numOfMaterials; J++) { 
     //create physics vector then fill it ....

     G4PhysicsLogVector* aVector = new G4PhysicsLogVector(
               lowestKineticEnergy, highestKineticEnergy, TotBin);

     // compute the (macroscopic) cross section first
 
     const G4Material* material= (*theMaterialTable)[J];        
     const 
     G4ElementVector* theElementVector = material->GetElementVector();
     const
     G4double* theAtomicNumDensityVector=material->GetAtomicNumDensityVector();
     const
     G4int NumberOfElements = material->GetNumberOfElements() ;
 
     // get the electron kinetic energy cut for the actual material,
     // it will be used in ComputeMicroscopicCrossSection
     // (--> it will be the same for all the elements in this material )
     G4double DeltaThreshold = DeltaCutInKineticEnergy[J] ;

     for (G4int i = 0 ; i < TotBin ; i++) {

       LowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;
       SIGMA = 0.;           
       for (G4int iel=0; iel<NumberOfElements; iel++ ) {

           SIGMA += theAtomicNumDensityVector[iel]*
             ComputeCrossSectionWithCut( (*theElementVector)(iel)->GetZ(),
			                  LowEdgeEnergy, DeltaThreshold);
       }
       // mean free path = 1./macroscopic cross section
       Value = SIGMA > DBL_MIN ? 1./SIGMA : DBL_MAX;     
       aVector->PutValue(i, Value) ;       
     }
     // vector is filled
     theMeanFreePathTable->insert(aVector);
  }
}

//    

G4double G4LowEnergyIonisation::ComputeCrossSection(const G4double AtomIndex,
						    const G4double IncEnergy){
  // calculates the microscopic cross section from subshell cross sections
  //(it is called for elements , AtomicNumber = Z ) all the subshell
  // cross section are computed at the energy IncEnergy with a log-log 
  // interpolation as reported by stepanek and then summed to get the 
  // total cross section 
 
  G4double TotalCrossSection = 0.0;

  const oneAtomTable* oneAtomCS
    = (*allAtomShellCrossSec)[ZNumVec->index(AtomIndex)];

  for(size_t ind = 0; ind < oneAtomCS->size(); ind++){

    G4double crossSec = 0;
    G4DataVector* EnergyVector = (*(*oneAtomCS)[ind])[0];
    G4DataVector* CrossSecVector = (*(*oneAtomCS)[ind])[1];

    if(IncEnergy < (*EnergyVector)[1]){ // First element is the shell number

      crossSec = 0;
    }

    else{

      crossSec = util.DataLogInterpolation(IncEnergy, 
                (*EnergyVector), (*CrossSecVector))*barn;

    }

    TotalCrossSection += crossSec;
  }


  return TotalCrossSection ;
}

//    

G4double G4LowEnergyIonisation::ComputeCrossSectionWithCut(
                                            const G4double AtomIndex,
                                            const G4double IncEnergy,
				            const G4double CutEnergy)
{
  G4double TotalCrossSection = 0.0;

  const oneAtomTable* oneAtomCS
    = (*allAtomShellCrossSec)[ZNumVec->index(AtomIndex)];

   //loop on shells
  for(size_t ind = 0; ind < oneAtomCS->size(); ind++){

    // G4double crossSec = 0;
    // G4DataVector* EnergyVector = (*(*oneAtomCS)[ind])[0];
    // G4DataVector* CrossSecVector = (*(*oneAtomCS)[ind])[1];

    TotalCrossSection += GetShellCrossSectionwithCut(AtomIndex,ind,IncEnergy,
                                                     CutEnergy);
  }

  return TotalCrossSection ;
}

//    

G4double G4LowEnergyIonisation::GetShellCrossSection(
                                               const G4double AtomicNumber,
                                               const G4int ind,
					       const G4double KineticEnergy)
{
  // gives the microscopic cross section for the subshell 
 
  const oneAtomTable* oneAtomCS
    = (*allAtomShellCrossSec)[ZNumVec->index(AtomicNumber)];

  G4double crossSec = 0.0;
  G4int oneSize = oneAtomCS->size();

  if( ind >= oneSize ) return crossSec ;

  G4DataVector* EnergyVector = (*(*oneAtomCS)[ind])[0];
  G4DataVector* CrossSecVector = (*(*oneAtomCS)[ind])[1];

  if(KineticEnergy < (*EnergyVector)[1]){ // First element is the shell number

    crossSec = 0;
  } else{

    crossSec = util.DataLogInterpolation(KineticEnergy, (*EnergyVector), 
                                        (*CrossSecVector))*barn;
  }

  return crossSec;
}

//    

G4double G4LowEnergyIonisation::GetShellCrossSectionwithCut(
                                             const G4double AtomicNumber,
                                             const G4int index,
					     const G4double KineticEnergy,
                                             const G4double Tcut)
{
  // it gives the total cross section for the subshell
  // (integration between Tcut and Tmax)

  G4double minEn = 0.1*eV ;
  G4double xsec = 0. ;

  //  shortcut ..........................
  if(2.*Tcut >= KineticEnergy) return xsec ;

  G4double CrossSection = GetShellCrossSection(AtomicNumber,index,
                                               KineticEnergy) ;

  //  shortcut ..........................
  if(CrossSection <= 0.) return xsec ;

  oneAtomTable* oneAtomCoeffTable = 
               (*theSamplingCoeffTable)[ZNumVec->index(AtomicNumber)];

  oneShellTable* oneShellCoeffTable =
               (*oneAtomCoeffTable)[index];
  G4double BindingEn =
               (*(*(*theBindingEnergyTable)[AtomicNumber-1])[1])[index];

  G4double maxEn = 0.5*(KineticEnergy-BindingEn) ;
  G4double Tlim = Tcut+BindingEn ;

  // shortcut ................................
  if((Tcut >= maxEn)||((minEn+BindingEn) >= maxEn)) return xsec ;

  const G4int CoeffNumber = oneShellCoeffTable->size();

  const G4DataVector* energyVec = (*oneShellCoeffTable)[0];
  const G4int LastPar = energyVec->size()-1;
  G4DataVector Parms;

  for(G4int ind = 1; ind < CoeffNumber-2; ind++){

    const G4DataVector* oneCoeffVec = (*oneShellCoeffTable)[ind];

    if(KineticEnergy < (*energyVec)[0]){
      Parms.push_back((*oneCoeffVec)[0]);
    }

    else if(KineticEnergy > (*energyVec)[LastPar]){

      Parms.push_back((*oneCoeffVec)[LastPar]);
    }

    else{

      G4double par = util.DataSemiLogInterpolation(KineticEnergy,
                                                  (*energyVec),(*oneCoeffVec));
      Parms.push_back(par);
    }
  }


  // First cut
  G4double fstCut = (*(*oneShellCoeffTable)[CoeffNumber-2])[0] == 0.? DBL_MAX :
                    (*(*oneShellCoeffTable)[CoeffNumber-2])[0]*MeV;

  // Second cut
  G4double sndCut = (*(*oneShellCoeffTable)[CoeffNumber-1])[0] == 0.? DBL_MAX :
                    (*(*oneShellCoeffTable)[CoeffNumber-1])[0]*MeV;

  G4double low,high,aa,bb,saa,sbb ;

  // for the normalization ***************************
  G4double norm = 0. ;

  // Area of the first function ALWAYS computed*******
  high = maxEn+BindingEn;
  if(fstCut < maxEn){
    high = fstCut+BindingEn;
  }
  low = BindingEn+minEn;

  aa=1./low ;
  bb=1./high ;
  saa = aa, sbb = bb;
 
  for (G4int ii = 1; ii < 7; ii++){
    norm += Parms[ii-1]*(saa-sbb)/G4double(ii) ;
    saa *= aa ;
    sbb *= bb ;
  }
 

  // Area of the second function and  the third function
  //
  if (maxEn >= sndCut){

    // Second Function: B1*energy**B2
    //
    G4double expona = Parms[13]+1;

    norm += (Parms[12]/expona)*(pow(sndCut,expona)-pow(fstCut,expona));

    // Third Function: sum_2(Ci/(energy+Binding)^i)  i = 2,3,4,5
    //
    aa = 1./(sndCut+BindingEn); // equivalent to 1./low in the first function
    bb = 1./(maxEn+BindingEn);  // equivalent to 1./high in the first function

    saa = aa, sbb = bb;

    for(G4int kk = 1; kk < 5; kk++){

      norm += Parms[kk+6]*(saa-sbb)/G4double(kk);
      saa *= aa;
      sbb *= bb;
    }
  }

  else if(maxEn >= fstCut){

    // two functions used.
    //
    // Third Function: sum_2(Ci/(energy+Binding)^i)  i = 2,3,4,5
    //
    aa = 1./(fstCut+BindingEn); // equivalent to 1./low in the first function
    bb = 1./(maxEn+BindingEn);  // equivalent to 1./high in the first function
    saa = aa, sbb = bb;

    for(G4int kk = 1; kk < 5; kk++){

      norm += Parms[kk+6]*(saa-sbb)/G4double(kk);
      saa *= aa;
      sbb *= bb;
    }

  }
  //  end of normalization ***********************************

    // start of the integration

  // contribution from 1st function ?
  if(Tcut < fstCut)
  {
    low = G4std::max(minEn+BindingEn,Tlim) ;
    high= G4std::min(fstCut+BindingEn,maxEn+BindingEn) ; 
      aa  = 1./low ;
      bb  = 1./high ;
      saa = aa ;
      sbb = bb ;
      for(G4int i=1; i<7; i++)
      {
        xsec += Parms[i-1]*(saa-sbb)/G4double(i) ;
        saa *= aa ;
        sbb *= bb ;
      }
  }  
  
  // 3 functions , contributions from 2nd and 3rd function
  if(maxEn >= sndCut)
  {
    // 3rd function
    if(Tcut < sndCut)
    {
      low = G4std::max(fstCut,Tcut) ;
      high= G4std::min(sndCut,maxEn) ;

      G4double expon = Parms[13]+1. ;
      xsec += (Parms[12]/expon)*(pow(high,expon)-pow(low,expon)) ;
    }

    // 2nd function
    low = G4std::max(sndCut+BindingEn,Tlim) ;
    high= maxEn+BindingEn ;
    
    aa = 1./low ;
    bb = 1./high ;
    saa = aa ;
    sbb = bb ;
    for(G4int i=1; i<5; i++)
    {
      xsec += Parms[i+6]*(saa-sbb)/G4double(i) ;
      saa *= aa ;
      sbb *= bb ;
    }
  }
  // 2 functions , contr. from 2nd
  else if(maxEn >= fstCut)
  {
    low = G4std::max(fstCut+BindingEn,Tlim) ;
    high= maxEn+BindingEn ;

    aa = 1./low ;
    bb = 1./high ;
    saa = aa ;
    sbb = bb ;
    for(G4int i=1; i<5; i++)
    {
      xsec += Parms[i+6]*(saa-sbb)/G4double(i) ;
      saa *= aa ;
      sbb *= bb ;
    }
  }  

  return xsec*CrossSection/norm ;
}

//    

G4double G4LowEnergyIonisation::GetShellEnergyLosswithCut(
                                              const G4double AtomicNumber,
                                              const G4int index,
				              const G4double KineticEnergy,
                                              const G4double Tcut)
{
  // it computes the energy loss for the shell
  // ( integrartion of  T*dSigma/dT between Tmin and Tcut)

  G4double minEn = 0.1*eV ;
  G4double loss = 0. ;

  //  shortcut ..........................
  if(Tcut <= 0.) return loss ;

  G4double CrossSection = GetShellCrossSection(AtomicNumber,index,
                                               KineticEnergy) ;

  //  shortcut ..........................
  if(CrossSection <= 0.) return loss ;

  oneAtomTable* oneAtomCoeffTable = 
               (*theSamplingCoeffTable)[ZNumVec->index(AtomicNumber)];

  oneShellTable* oneShellCoeffTable =
               (*oneAtomCoeffTable)[index];
  G4double BindingEn =
               (*(*(*theBindingEnergyTable)[AtomicNumber-1])[1])[index];

  G4double maxEn = 0.5*(KineticEnergy-BindingEn) ;
  G4double Tlim = Tcut+BindingEn ;

  const G4int CoeffNumber = oneShellCoeffTable->size();

  const G4DataVector* energyVec = (*oneShellCoeffTable)[0];
  const G4int LastPar = energyVec->size()-1;
  G4DataVector Parms;

  for(G4int ind = 1; ind < CoeffNumber-2; ind++){

    const G4DataVector* oneCoeffVec = (*oneShellCoeffTable)[ind];

    if(KineticEnergy < (*energyVec)[0]){
      Parms.push_back((*oneCoeffVec)[0]);
    }

    else if(KineticEnergy > (*energyVec)[LastPar]){

      Parms.push_back((*oneCoeffVec)[LastPar]);
    }

    else{

      G4double par = util.DataSemiLogInterpolation(KineticEnergy,
                                                  (*energyVec),(*oneCoeffVec));
      Parms.push_back(par);
    }
  }


  // First cut
  G4double fstCut = (*(*oneShellCoeffTable)[CoeffNumber-2])[0] == 0.? DBL_MAX :
                    (*(*oneShellCoeffTable)[CoeffNumber-2])[0]*MeV;

  // Second cut
  G4double sndCut = (*(*oneShellCoeffTable)[CoeffNumber-1])[0] == 0.? DBL_MAX :
                    (*(*oneShellCoeffTable)[CoeffNumber-1])[0]*MeV;

  G4double low,high,aa,bb,saa,sbb,w ;

  // for the normalization ************************************
  G4double norm = 0. ;

  // Area of the first function ALWAYS computed****************
  high = maxEn+BindingEn;
  if(fstCut < maxEn){
    high = fstCut+BindingEn;
  }
  low = BindingEn+minEn;

  aa=1./low ;
  bb=1./high ;
  saa = aa, sbb = bb;
 
  for (G4int ii = 1; ii < 7; ii++){
    norm += Parms[ii-1]*(saa-sbb)/G4double(ii) ;
    saa *= aa ;
    sbb *= bb ;
  }
 

  // Area of the second function and  the third function
  //
  if (maxEn >= sndCut){

    // Second Function: B1*energy**B2
    //
    G4double expona = Parms[13]+1;

    norm += (Parms[12]/expona)*(pow(sndCut,expona)-pow(fstCut,expona));

    // Third Function: sum_2(Ci/(energy+Binding)^i)  i = 2,3,4,5
    //
    aa = 1./(sndCut+BindingEn); // equivalent to 1./low in the first function
    bb = 1./(maxEn+BindingEn);  // equivalent to 1./high in the first function

    saa = aa, sbb = bb;

    for(G4int kk = 1; kk < 5; kk++){

      norm += Parms[kk+6]*(saa-sbb)/G4double(kk);
      saa *= aa;
      sbb *= bb;
    }
  }

  else if(maxEn >= fstCut){

    // two functions used.
    //
    // Third Function: sum_2(Ci/(energy+Binding)^i)  i = 2,3,4,5
    //
    aa = 1./(fstCut+BindingEn); // equivalent to 1./low in the first function
    bb = 1./(maxEn+BindingEn);  // equivalent to 1./high in the first function
    saa = aa, sbb = bb;

    for(G4int kk = 1; kk < 5; kk++){

      norm += Parms[kk+6]*(saa-sbb)/G4double(kk);
      saa *= aa;
      sbb *= bb;
    }

  }
  //  end of normalization *****************************************

    low = minEn+BindingEn ;

    high= G4std::min(fstCut+BindingEn,maxEn+BindingEn) ; 
    high= G4std::min(high,Tlim) ; 

      aa  = 1./low ;
      bb  = 1./high ;
      loss += Parms[0]*(log(high/low)+BindingEn*(bb-aa)) ;

      saa = aa ;
      sbb = bb ;

      for(G4int i=2; i<7; i++)
      {
        w = (saa-sbb)/G4double(i-1) ;
        saa *= aa ;
        sbb *= bb ;
        w += BindingEn*(sbb-saa)/G4double(i) ;
        loss += Parms[i-1]*w ;
      }
  
  if(Tcut > fstCut)
  {

    // 3 functions , contributions from 2nd and 3rd function
    if(maxEn >= sndCut)
    {
      // 3rd function
        low = fstCut ;
        high= G4std::min(sndCut,maxEn) ;
        high= G4std::min(high,Tcut) ;

        G4double expon = Parms[13]+2. ;
        loss += (Parms[12]/expon)*(pow(high,expon)-pow(low,expon)) ;

      // 2nd function
      if(Tcut > sndCut)
      {
        low = sndCut+BindingEn ;
        high= G4std::min(maxEn+BindingEn,Tlim) ;

        aa  = 1./low ;
        bb  = 1./high ;
        loss += Parms[7]*(log(high/low)+BindingEn*(bb-aa)) ;

        saa = aa ;
        sbb = bb ;

        for(G4int i=2; i<5; i++)
        {
          w = (saa-sbb)/G4double(i-1) ;
          saa *= aa ;
          sbb *= bb ;
          w += BindingEn*(sbb-saa)/G4double(i) ;
          loss += Parms[i+6]*w ;
        }
    
      }
    }
    // 2 functions , contr. from 2nd
    else if(maxEn >= fstCut)
    {
        low = fstCut+BindingEn ;
        high= G4std::min(maxEn+BindingEn,Tlim) ;

        aa  = 1./low ;
        bb  = 1./high ;
        loss += Parms[7]*(log(high/low)+BindingEn*(bb-aa)) ;

        saa = aa ;
        sbb = bb ;

        for(G4int i=2; i<5; i++)
        {
          w = (saa-sbb)/G4double(i-1) ;
          saa *= aa ;
          sbb *= bb ;
          w += BindingEn*(sbb-saa)/G4double(i) ;
          loss += Parms[i+6]*w ;
        }

    }  

  }

  // return loss*CrossSection/norm ;

  loss *= CrossSection/norm ;

  loss += BindingEn*(CrossSection -
          GetShellCrossSectionwithCut(AtomicNumber,index,KineticEnergy,Tcut)); 
  return loss ;
}

//     
 
G4VParticleChange* G4LowEnergyIonisation::PostStepDoIt(const G4Track& track,   
						       const G4Step&  step)
{

  // Fluorescence (as reported by J.Stepanek):
  // J. Stepanek " A program to determine the radiation spectra due 
  // to a single atomic subshell ionisation by a particle or due to 
  // deexcitation or decay of radionuclides", 
  // Comp. Phys. Comm. 1206 pp 1-19 (1997)
  // 

  aParticleChange.Initialize(track);
  
  G4Material*               aMaterial = track.GetMaterial() ;
  const G4DynamicParticle*  aParticle = track.GetDynamicParticle() ;

  //  get kinetic energy cut for the electron
  G4double* DeltaCutInKineticEnergy= G4Electron::Electron()->GetCutsInEnergy();
  G4double  DeltaThreshold = DeltaCutInKineticEnergy[aMaterial->GetIndex()];
 
  G4double KineticEnergy = aParticle->GetKineticEnergy();

  // kill e- if Tkin is too small ......
  if(KineticEnergy <= DeltaThreshold)
  { 
    if(KineticEnergy < 0.) {
      G4cout << "G4LowEnergyIonisation: negative kinetic energy (eV) = " 
             << KineticEnergy/eV << G4endl; 
      KineticEnergy = 0.0;
    }
    aParticleChange.SetEnergyChange(0.0);  
    aParticleChange.SetLocalEnergyDeposit(KineticEnergy);
    aParticleChange.SetStatusChange(fStopAndKill);
    return G4VContinuousDiscreteProcess::PostStepDoIt(track,step);
  }

  // Create lists of pointers to DynamicParticles (photons and electrons) 
  G4ParticleVector photvec;
  G4ParticleVector elecvec;

  // select randomly one element constituing the material.
  G4Element* anElement = SelectRandomAtom(aParticle, aMaterial);
  G4int AtomIndex = (G4int) anElement->GetZ();

  // First Ionised subshell chosen basing on subshell integrated 
  // cross section EPDL97 partial sum method
  // Select the subshell: it returns the subshell index in the table.
  G4int subShellIndex = SelectRandomShell(AtomIndex, KineticEnergy, 
                                          DeltaThreshold);

  // There are no delta-ray production
  if(subShellIndex < 0) {
    return G4VContinuousDiscreteProcess::PostStepDoIt(track,step); 
  }
  
  // Get the subshell energy
  G4FirstLevel* theBindEnVec = (*theBindingEnergyTable)[AtomIndex-1];
  G4int thePrimaryShell = (G4int) (*(*theBindEnVec)[0])[subShellIndex];
  G4double BindingEn = (*(*theBindEnVec)[1])[subShellIndex];

  // Kinematic inside Atomic potential 
  G4ThreeVector ParticleDirection = aParticle->GetMomentumDirection();
  G4double totEnergy = KineticEnergy + ParticleMass + 2.0*BindingEn;
  G4double totMomentum = sqrt(totEnergy*totEnergy - ParticleMass*ParticleMass);
  G4double deltaKinEnergy = 0.0;


  // Energy Sampling
  deltaKinEnergy = EnergySampling(AtomIndex, subShellIndex,
                                    KineticEnergy, DeltaThreshold);
  deltaKinEnergy+= 2.0*BindingEn;

  // one more test for pathology cases
  if( deltaKinEnergy < DeltaThreshold + 2.0*BindingEn) {
    deltaKinEnergy -= 2.0*BindingEn;
    if(deltaKinEnergy < 0.0) deltaKinEnergy = 0.0;
    aParticleChange.SetEnergyChange(KineticEnergy - deltaKinEnergy);  
    aParticleChange.SetLocalEnergyDeposit(deltaKinEnergy);
    return G4VContinuousDiscreteProcess::PostStepDoIt(track,step);
  }

  // sampling of scattering angle neglecting atomic motion
  G4double deltaMomentum = sqrt(deltaKinEnergy * 
                          (deltaKinEnergy + 2.0*ParticleMass ));
   
  G4double costheta = deltaKinEnergy * (totEnergy + ParticleMass)
                    /(deltaMomentum * totMomentum);

  if      (costheta < -1.) costheta = -1.;
  else if (costheta > +1.) costheta = +1.;
  G4double sintheta = sqrt(1. - costheta*costheta);
  G4double phi  = twopi * G4UniformRand(); 
  G4double dirx = sintheta * cos(phi);
  G4double diry = sintheta * sin(phi);
  G4double dirz = costheta;

  // Rotate to incident electron direction
  G4ThreeVector deltaDir(dirx,diry,dirz);
  deltaDir.rotateUz(ParticleDirection);
  dirx = deltaDir.x();
  diry = deltaDir.y();
  dirz = deltaDir.z();

  // Find out new incident electron direction
  G4double finalPx = totMomentum*ParticleDirection.x() - deltaMomentum*dirx; 
  G4double finalPy = totMomentum*ParticleDirection.y() - deltaMomentum*diry; 
  G4double finalPz = totMomentum*ParticleDirection.z() - deltaMomentum*dirz; 

  // Take into account atomic motion
  costheta = 2.0*G4UniformRand() - 1.0;
  sintheta = sqrt(1. - costheta*costheta);
  phi  = twopi * G4UniformRand(); 
  G4double del = sqrt(BindingEn *(BindingEn + 2.0*ParticleMass))/deltaMomentum;
  dirx += del* sintheta * cos(phi);
  diry += del* sintheta * sin(phi);
  dirz += del* costheta;
  G4double norm = 1.0/sqrt(dirx*dirx + diry*diry + dirz*dirz);
  
  G4double finalKineticEnergy = KineticEnergy - deltaKinEnergy;
  deltaKinEnergy -= 2.0*BindingEn;

  // create G4DynamicParticle object for delta ray
  G4DynamicParticle* theDeltaRay = new G4DynamicParticle();
  theDeltaRay->SetKineticEnergy(deltaKinEnergy);
  theDeltaRay->SetMomentumDirection(dirx*norm,diry*norm,dirz*norm); 
  theDeltaRay->SetDefinition(G4Electron::Electron());
  elecvec.push_back(theDeltaRay);
     
  G4double theEnergyDeposit = BindingEn;

  if(finalKineticEnergy < 0.){

    G4cout << "Tkin=" << KineticEnergy/eV << "  Tdel=" << deltaKinEnergy/eV
	   << "  BindingEn=" << BindingEn/eV << "  ***********" << G4endl;
  }
    
  // FLUORESCENCE
  // load the transition probability table for the element
  // theTable[i][j][k] 
  // i = subshell, j = type of information (second shell, transition energy , 
  // transition probability), k = previous vectors.
    
  // Fluorescence data start from element 6

  if(thePrimShVec.size() != 0) thePrimShVec.clear();
  thePrimShVec.push_back(thePrimaryShell);
  if(AtomIndex > 5){
      
      G4bool ThereAreShells = TRUE;
      G4int AtomInd = ZNumVecFluor->index(AtomIndex);
      oneAtomTable* oneAtomFluorTrans = (*theFluorTransitionTable)[AtomInd];
      
      while(ThereAreShells) {
	
	// Select the second transition from another subshell
	// fluorPar[0] = SubShell 
	// fluorPar[1] = Sec SubShell (if there is), 
	// fluorPar[2] = Transition Probability
	// fluorPar[3] = Transition Energy
	// the same for augerPar
	
	G4double fluorPar[3] = {0};
	
  // SelectRandomTransition argument is oneAtomTable loop on shells is inside
	ThereAreShells = SelectRandomTransition(thePrimaryShell, 
						fluorPar, 
						oneAtomFluorTrans);
	
	
	// Daugther dynamic particle
	G4DynamicParticle* newPart;
	
	// Direction of the outcoming particle isotropic selection
	G4double newcosTh = 1-2*G4UniformRand();
	G4double newsinTh = sqrt(1-newcosTh*newcosTh);
	G4double newPhi = twopi*G4UniformRand();
	
	G4double dirx, diry, dirz;
	dirz = newcosTh;
	diry = newsinTh*cos(newPhi);
	dirx = newsinTh*sin(newPhi);
	G4ThreeVector newPartDirection(dirx, diry, dirz);
	
	if(ThereAreShells) {
	  
	  thePrimaryShell = (G4int) fluorPar[0];
	  
	  if(fluorPar[2] >= CutForLowEnergySecondaryPhotons){
	    
	    theEnergyDeposit -= fluorPar[2]*MeV;
	    newPart = new G4DynamicParticle (G4Gamma::Gamma(), 
					     newPartDirection, 
					     fluorPar[2]);
	    
	    photvec.push_back(newPart);
	  }
	} else {
	  
	  /////Energy deposition vl
	  ////=================NEW================vl
	  
	  /*
	  // last shell transition from continuum
	  G4int k = 0;
	  while(thePrimaryShell != (*(*theBindEnVec)[0])[k]){
	    k++;
	  }
	  
	  G4double lastTransEnergy = (*(*theBindEnVec)[1])[k];
	  thePrimaryShell = (G4int) fluorPar[0];
	  
	  if(lastTransEnergy >= CutForLowEnergySecondaryPhotons){
	    
	    theEnergyDeposit -= lastTransEnergy*MeV;
	    
	    newPart = new G4DynamicParticle(G4Gamma::Gamma(), 
					    newPartDirection, 
					    lastTransEnergy);
	    
	    photvec.push_back(newPart);
	  }
	    
	  thePrimShVec.insert(thePrimaryShell);
	  */
	}
      }
    } //END OF THE CHECK ON ATOMIC NUMBER

  G4int numOfElec = elecvec.size();
  G4int numOfPhot = photvec.size();
  G4int numOfDau = numOfElec + numOfPhot;
  aParticleChange.SetNumberOfSecondaries(numOfDau);
  G4int l = 0;

  for(l = 0; l<numOfElec; l++ ){
    aParticleChange.AddSecondary(elecvec[l],true);
  }
    
  for(l = 0; l < numOfPhot; l++) {    
    aParticleChange.AddSecondary(photvec[l],true); 
  }

  photvec.clear();
  elecvec.clear();
    
  // fill ParticleChange 
  // changed energy and momentum of the actual particle

  if(finalKineticEnergy > DeltaThreshold){
    G4double fac = 1.0/sqrt(finalPx*finalPx+finalPy*finalPy+finalPz*finalPz);
    finalPx *= fac;
    finalPy *= fac;
    finalPz *= fac;

    aParticleChange.SetMomentumChange(finalPx,finalPy,finalPz);
    aParticleChange.SetEnergyChange(finalKineticEnergy);
    if(theEnergyDeposit < 0.) {
      G4cout << "G4LowEnergyIonisation: Negative energy deposit: " 
             << theEnergyDeposit/eV << " eV" << G4endl;
      theEnergyDeposit = 0.0;
    }
    aParticleChange.SetLocalEnergyDeposit(theEnergyDeposit);
   
  } else {
    theEnergyDeposit += finalKineticEnergy ;
    if(theEnergyDeposit < 0.){
      G4cout << "G4LowEnergyIonisation: Negative energy deposit: " 
             << theEnergyDeposit/eV << " eV" << G4endl; 
      G4cout << "                       finalKineticEnergy= " 
             << finalKineticEnergy/eV << " eV" << G4endl;
      theEnergyDeposit = 0.0;
    }
    aParticleChange.SetLocalEnergyDeposit(theEnergyDeposit);
    aParticleChange.SetEnergyChange(0.);
    aParticleChange.SetStatusChange(fStopAndKill);
  }
    
  return G4VContinuousDiscreteProcess::PostStepDoIt(track,step);
}

//    

//
// selection of the first ionized subshell: partial sum method
// 
G4int G4LowEnergyIonisation::SelectRandomShell(const G4int AtomIndex
                                             , const G4double IncEnergy
					     , const G4double CutEnergy)
{  
  G4double partialSum = 0;
  G4double totalSum = ComputeCrossSectionWithCut(AtomIndex,IncEnergy,
                                                 CutEnergy);
  if(totalSum<=0.) return -1;
  
  G4double rval = totalSum*G4UniformRand();
  const oneAtomTable* oneAtomCS 
    = (*allAtomShellCrossSec)[ZNumVec->index(AtomIndex)];

  for(size_t ind = 0; ind < oneAtomCS->size(); ind++) {

    G4double crossSec;
    G4DataVector* EnergyVector = (*(*oneAtomCS)[ind])[0];
    // G4DataVector* CrossSecVector = (*(*oneAtomCS)[ind])[1];
    if(IncEnergy < (*EnergyVector)[0]){ //First element is the shell number

      crossSec = 0;
    }

    else{

      crossSec = GetShellCrossSectionwithCut(AtomIndex,ind,IncEnergy,
                                             CutEnergy);
    }
    
    partialSum += crossSec;

    if(rval <= partialSum) return ind;
  }
  
  G4Exception("LEIonisation: Cannot select a shell");
  return 0;
}

//    

//
// Element selction in the material already in the standard processes
// 
G4Element* G4LowEnergyIonisation::SelectRandomAtom(
                          const G4DynamicParticle* aDynamicParticle, 
				G4Material* aMaterial)
{
  // select randomly 1 element within the material
  G4double KineticEnergy = aDynamicParticle->GetKineticEnergy();
  const G4int NumberOfElements = aMaterial->GetNumberOfElements();
  const G4ElementVector* theElementVector = aMaterial->GetElementVector();
  if (NumberOfElements == 1) return (*theElementVector)(0);

  const G4double* theAtomNumDensityVector = 
                  aMaterial->GetAtomicNumDensityVector();

  G4double PartialSumSigma = 0. ;

  G4double rval = G4UniformRand()/MeanFreePath;
 
  // get electron  cuts in kinetic energy
  G4double* DeltaCutInKineticEnergy = G4Electron::Electron()
                                    ->GetCutsInEnergy();
  for ( G4int i=0 ; i < NumberOfElements ; i++ ) { 

    G4double DeltaThreshold = DeltaCutInKineticEnergy[i] ;
    G4double crossSection;
    if (KineticEnergy <  lowestKineticEnergy)

      crossSection = 0. ;

    else {

      if (KineticEnergy > highestKineticEnergy) 
                          KineticEnergy = 0.99*highestKineticEnergy;

      G4int AtomIndex = (G4int) (*theElementVector)(i)->GetZ();
      crossSection = ComputeCrossSectionWithCut(AtomIndex, KineticEnergy,
                                                DeltaThreshold);
    }

    PartialSumSigma += theAtomNumDensityVector[i] * crossSection;

    if (rval <= PartialSumSigma) return ((*theElementVector)(i));

  }
  return (*theElementVector)(0);
}

//    

//
// Select a random transition with the transition probabilities and 
// the partial sum method using EADL data (A. Forti)
//

G4bool G4LowEnergyIonisation::SelectRandomTransition(G4int thePrimShell, 
					G4double* TransParam,
				  const oneAtomTable* TransitionTable)
{
  
  G4int SubShellCol = 0, ProbCol = 1, EnergyCol = 2;
  // transitionTable contains all the transition probabilities of one atom:
  // loop on subshell is inside the method.

  // when the last subshell is reached CollIsFull becomes FALSE.
  G4bool ColIsFull = FALSE;
  G4int ShellNum = 0;
  //  G4double TotalSum = 0; 
  G4int maxNumOfShells = TransitionTable->size()-1;

  if(thePrimShell <= 0) {
     G4cerr<<"*** Unvalid Primary shell: "<<thePrimShell<<G4endl;
     return FALSE;
  }   
  if(thePrimShell <= (*(*(*TransitionTable)[maxNumOfShells])[0])[0]){

    while(thePrimShell != (*(*(*TransitionTable)[ShellNum])[0])[0]){
  
      if(ShellNum == maxNumOfShells){
	break;
      }
      
      ShellNum++;
    }

    // TransProb is the index of the loop and of the table of transition. 
    // It starts from 1 because the first element of the data table is 
    // the primary shell id number and not a transition probability: 
    // it must not be added to TotalSum. 
  
    G4int TransProb = 1;
    // Include non-radiative transitions (vl):
    ////for(TransProb = 1; TransProb < (*(*TransitionTable)[ShellNum])[ProbCol]->length(); TransProb++){ 
    ////  TotalSum += (*(*(*TransitionTable)[ShellNum])[ProbCol])[TransProb];
    ////}
    ////G4double PartialProb = G4UniformRand()*TotalSum;
    ////
    G4double PartialProb = G4UniformRand();
    G4double PartSum = 0;
    
    TransProb = 1; 
    G4int trSize = (*(*TransitionTable)[ShellNum])[ProbCol]->size();
    while(TransProb < trSize){
      
      PartSum += (*(*(*TransitionTable)[ShellNum])[ProbCol])[TransProb];
      
      if(PartialProb <= PartSum){
	
	TransParam[0] = (*(*(*TransitionTable)[ShellNum])[SubShellCol])[TransProb];
	TransParam[1] = (*(*(*TransitionTable)[ShellNum])[ProbCol])[TransProb];
	TransParam[2] = (*(*(*TransitionTable)[ShellNum])[EnergyCol])[TransProb];
	ColIsFull = TRUE;
	break;
      }
      
      TransProb++;
    }

  }
  else{

    ColIsFull = FALSE;
  }

  return ColIsFull;
}

//    

G4double G4LowEnergyIonisation::EnergySampling(const G4int AtomicNumber, 
					       const G4int ShellIndex, 
					       const G4double KinEn
					     , const G4double deltaRayMinE)
{
  // Sampling Method: acceptation - rejection method from
  // EGS4 W.R. Nelson et al. The EGS4 Code System. SLAC-Report-265 , 
  // December 1985 
  // Fit functions:
  //
  // first function: sum_i A_i/(en+bind)^i  i=2,7 used in all 
  // the cases/subshells (see below)
  //
  // second function: B_1*(en)**B_2 used in 9 cases/subshells (no references)
  //
  // third function: sum_i C_i/(en+bind)^i i=2,5 used in 462 cases/subshells 
  // (see below)
  //
  //Y.K.Kim M.E.Rudd http://physics.nist.gov/PhysRefData/Ionization/intro.html
  // First and Third functions are a modified version: degree of monomial 
  // in the truncated sum is from 2 to 7 instead that from 2 to 6. 
  // method A. Forti.

  //  1) Load Coefficients (I need Z number and the index of the shell)
  oneAtomTable* oneAtomCoeffTable = 
               (*theSamplingCoeffTable)[ZNumVec->index(AtomicNumber)];

  oneShellTable* oneShellCoeffTable = (*oneAtomCoeffTable)[ShellIndex];
  const G4double BindingEn = 
               (*(*(*theBindingEnergyTable)[AtomicNumber-1])[1])[ShellIndex];

  // 2) Interpolate coefficients (I need the incoming electron kinetic energy)
  const G4int CoeffNumber = oneShellCoeffTable->size();

  const G4DataVector* energyVec = (*oneShellCoeffTable)[0];
  const G4int LastPar = energyVec->size()-1;
  G4DataVector Parms;

  for(G4int ind = 1; ind < CoeffNumber-2; ind++){

    const G4DataVector* oneCoeffVec = (*oneShellCoeffTable)[ind]; 

    if(KinEn < (*energyVec)[0]){
      Parms.push_back((*oneCoeffVec)[0]);
    }

    else if(KinEn > (*energyVec)[LastPar]){

      Parms.push_back((*oneCoeffVec)[LastPar]);
    }

    else{

      // NO REFERENCES for SEMI-LOG interpolation it is used 
      // because coefficients can be negative. 

      G4double par = util.DataSemiLogInterpolation(KinEn,(*energyVec),
                                                  (*oneCoeffVec));
      Parms.push_back(par);
    }
  }

  const G4double minEn = deltaRayMinE;
  
  // cuts in energy are always the same
  // First cut
  G4double fstCutbuf = (*(*oneShellCoeffTable)[CoeffNumber-2])[0]*MeV;
  if(fstCutbuf<0.01*eV) fstCutbuf = DBL_MAX;
  const G4double fstCut = fstCutbuf;

  // Second cut
  G4double sndCutbuf = (*(*oneShellCoeffTable)[CoeffNumber-1])[0]*MeV;
  if(sndCutbuf<fstCutbuf) sndCutbuf = DBL_MAX;
  const G4double sndCut = sndCutbuf;
  
  // 2') order of parameters:
  //     * Parms[0] = par1 LET
  //     * Parms[1] = par2 LET
  //     * Parms[2] = par3 LET
  //     * Parms[3] = par4 LET
  //     * Parms[4] = par5 LET
  //     * Parms[5] = par6 LET
  //     * Parms[6] = max rej 1
  //     * Parms[7] = b1 HET      
  //     * Parms[8] = b2 HET
  //     * Parms[9] = b3 HET
  //     * Parms[10] = b4 HET
  //     * Parms[11] = max rej 2
  //     * Parms[12] = c1 MET
  //     * Parms[13] = c2 MET

  // 3) Compute partial areas (with functions here the cut is used)
  // **************************************************************
  // Sampling energy limits and areas variable declaration
  G4double maxEn = (KinEn-BindingEn)/2;
  if(minEn>=maxEn) {
     G4cout << "G4LowEnergyIonisation::EnergySampling not enought energy left!"
            << G4endl;
     G4cout << " minEn - kinEn - B"
            << minEn << " "
	    << KinEn << " "
	    << BindingEn
	    << G4endl;
     return 0.;
  }
  G4double area1 = 0.,  area2 = 0., area3 = 0.;

  // **************************************************************
  // AREAS
  // **************************************************************
  // compute here the weights of the two regions (areas)
  // temporary solution, but needed for consistent sampling
  
  // NOTATION the number order of the functions follows the energy 
  // increasing order even if what is called second function should 
  // be the third following the number of cases in which it appears (only 9)

  //
  // First Function: sum_2(A_i/(energy+Binding)^i)  i = 2,3,4,5,6,7
  //
  G4double rejectionMax = 0. ;
  if(minEn>=fstCut){
     area1 = 0.;
  }   
  else{
     G4double high = maxEn+BindingEn;
     if(fstCut < maxEn){ high = fstCut+BindingEn;}
     G4double low = BindingEn+minEn;
     G4double aa=1./low ;
     G4double bb=1./high ;
     G4double saa = aa, sbb = bb;
     G4double llow = low;
  
    for (G4int ii = 1; ii < 7; ii++){

         area1 += Parms[ii-1]*(saa-sbb)/G4double(ii) ;
         saa *= aa ;
         sbb *= bb ;
         //
         //function itself at the minimum value
         //
         rejectionMax += Parms[ii-1]/llow;
         llow *= low ;
     }
     if(area1<0) G4cerr<<"Problem: area1 = "<<area1<<G4endl;
  }
  //
  // Area of the second and third functions
  //
  G4double whatCut = 0.;
  if (maxEn >= sndCut){
    
    //
    // three functions used. 
    //
    if(minEn>sndCut){
       area2 = 0.;
    }   
    else{   
       G4double low = fstCut;
       if(minEn>low) low = minEn;
       // 
       // Second Function: B1*energy**B2
       //
       G4double expon = Parms[13]+1;
       area2 = (Parms[12]/expon)*(pow(sndCut,expon)-pow(low,expon));
    }  
    if(area2<0) G4cerr<<"Problem: area2 = "<<area2<<G4endl;

    // Third Function: sum_2(Ci/(energy+Binding)^i)  i = 2,3,4,5 
    // it is the same as the first function but with two degree less
    //
    G4double low = sndCut;
    if(minEn>low) low = minEn;

    G4double aa = 1./(low+BindingEn); 
    // equivalent to 1./low in the first function 

    G4double bb = 1./(maxEn+BindingEn);  
    // equivalent to 1./high in the first function
    
    G4double saa = aa, sbb = bb;
    
    for(G4int kk = 1; kk < 5; kk++){ 
      
      // area3: integral of the normalized third function
      area3 += Parms[kk+6]*(saa-sbb)/G4double(kk);
      saa *= aa;
      sbb *= bb;     
    }
    if(area3<0) G4cerr<<"Problem: area3 = "<<area3<<G4endl;

    whatCut = low;
  }
  
  else if(maxEn >= fstCut){ 
    
    // two functions used.
    //
    // In this case the area2 keeps her initial value area2 = 0.
    //
    // Third Function: sum_2(Ci/(energy+Binding)^i)  i = 2,3,4,5 
    // it is the same as the first function but with two degree less
    //
    G4double low = fstCut;
    if(minEn>fstCut)low = minEn;
       
    G4double aa = 1./(low+BindingEn); 
    // equivalent to 1./low in the first function 

    G4double bb = 1./(maxEn+BindingEn);  
    // equivalent to 1./high in the first function

    G4double saa = aa, sbb = bb;
    G4int kk;
    for( kk = 1; kk < 5; kk++){

      // area3: integral of the normalized third function
      G4double buf = Parms[kk+6]*(saa-sbb)/G4double(kk);
      area3+= buf;
      saa *= aa;
      sbb *= bb;     
    }
    if(area3<0) {
       G4cerr<<"Problem: area3 bis= "<<area3<<G4endl;
       G4cout<<" low "<< low<<G4endl;
       G4cout<<" maxEn "<<maxEn<<G4endl;
       for(G4int  kk = 1; kk < 5; kk++){
        G4cout<<" kk par"<<kk<<"  " <<Parms[kk+6]<<G4endl;
       }
    }   
     
    whatCut = low;
  }
   
  // **************************************************************
  // SAMPLING 
  // **************************************************************
  
  G4double areaTot = area1+area2+area3;
  G4double areaDue = area1+area2;
  G4int which = 0;
  // 4) Generate a random number .to select the region of work
  G4double rand1 = areaTot*G4UniformRand();
  
  // 5) Sampling
  G4double sample = 0;
  
  if(rand1 <= area1){
    // Sampling from the first function
    
    // Low energy transfer 
    G4double rejection = 0;
    which = 1;
    G4double rand2;
    G4double Ka;
    do{
      
      rand2 = G4UniformRand();
      Ka = 0;
      
      if(fstCut < maxEn){
	
	Ka = (BindingEn + fstCut)/(minEn+BindingEn);
      }
      
      else{
	
	Ka = (BindingEn + maxEn)/(minEn+BindingEn);
      }
      
      sample = (minEn + BindingEn)*pow(Ka,rand2)-BindingEn;
      
      G4double arg = sample + BindingEn;
      
      rejection = Parms[0]/arg+Parms[1]/pow(arg,2)+Parms[2]/pow(arg,3)+
	Parms[3]/pow(arg,4)+Parms[4]/pow(arg,5)+Parms[5]/pow(arg,6);
      
      //rejection /= Parms[6];
      rejection /= rejectionMax;
      
    }while(rejection < G4UniformRand());
    /*
    if( sample<minEn ) {
        G4cout<<" sample = "<< sample
	      <<" minEn = "<<minEn
	      <<" BindingEn = "<<BindingEn
	      <<" Ka = "<<Ka
	      <<" rand2 = "<<rand2
	      <<" fstCut = "<<fstCut
	      <<" sndCut = "<<sndCut
	      <<" maxEn = "<<maxEn
	      <<" area1 = "<<area1
	      <<" rand1 = "<<rand1
	      <<G4endl;
    }
    */
  }
  
  else if(rand1 > area1 && rand1 <= areaDue){
    
    //Sampling from the second function only 9 subshells
    G4double expon = Parms[13]+1;
    G4double low = fstCut;
    if(minEn>low) low = minEn;
    G4double norm = (pow(sndCut,expon)-pow(low,expon));
    G4double sum = norm*G4UniformRand()+pow(low,expon);
    G4double exponInv = 1/expon;
    sample = pow(sum,exponInv);
    which = 2;
  }
  
  
  else if (rand1 > areaDue && rand1 <= areaTot){ 
    
    // Sampling from the third function 462 cases of which 9 with the second
    G4double rejection = 0;
    which = 3;
    do{
      
      G4double rand2 = G4UniformRand();
      G4double Ka = 0;
      
      if(whatCut < maxEn){
	
	Ka = (BindingEn + maxEn)/(whatCut+BindingEn);
      }
      
      sample = (whatCut + BindingEn)*pow(Ka,rand2)-BindingEn;
      
      G4double arg = sample + BindingEn;
      
      rejection = Parms[7]/arg+Parms[8]/pow(arg,2)+Parms[9]/pow(arg,3)
                + Parms[10]/pow(arg,4);
      
      rejection /= Parms[11];
      
    }while(rejection < G4UniformRand()); 
  }
  if(sample<minEn) {
    G4cout << " Error in energy sampling: sampleE<minEn: "
           <<sample<<" < "<<minEn<<"  "<<which<<G4endl;
  }
  return sample;
}

//    

void G4LowEnergyIonisation::PrintInfoDefinition()
{
  G4String comments = "Low energy ionisation code ";
           comments += "with implementation of the continuous dE/dx part.";  
           comments += "\n At present it can be used for electrons ";
           comments += " in the energy range [250eV,100GeV]";
           comments += 
                "\n the process must work with G4LowEnergyBremsstrahlung";
  G4cout << GetProcessName() << ":  " << comments << G4endl;
}

//    



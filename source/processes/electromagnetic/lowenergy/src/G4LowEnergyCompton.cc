// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4LowEnergyCompton.cc,v 1.25 2001-05-25 17:08:30 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 class implementation file
//      CERN Geneva Switzerland
//
//      For information related to this code contact:
//      CERN, IT Division, ASD group
//      ------------ G4LowEnergyCompton low energy modifications --------
//                   by Alessandra Forti, October 1998
// **************************************************************
// Added Livermore data table construction methods A. Forti
// Modified BuildMeanFreePath to read new data tables A. Forti
// Modified PostStepDoIt to insert sampling with EPDL97 data A. Forti
// Added SelectRandomAtom A. Forti
// Added map of the elements A. Forti
// 24.04.01 V.Ivanchenko remove RogueWave 
// --------------------------------------------------------------

// This Class Header
#include "G4LowEnergyCompton.hh"

// Collaborating Class Headers
#include "G4EnergyLossTables.hh"
#include "G4Electron.hh"

// constructor
 
G4LowEnergyCompton::G4LowEnergyCompton(const G4String& processName)
  : G4VDiscreteProcess(processName),
    theCrossSectionTable(0),
    theScatteringFunctionTable(0),
    theMeanFreePathTable(0),
    ZNumVec(0),
    lowestEnergyLimit (250*eV),              // initialization
    highestEnergyLimit(100*GeV),
    numbBinTable(200)
{
   if (verboseLevel>0) {
     G4cout << GetProcessName() << " is created "<< G4endl;
     G4cout << "lowestEnergy: " << lowestEnergyLimit/keV << "keV ";
     G4cout << "highestEnergy: " << highestEnergyLimit/TeV << "TeV " << G4endl;
   }
}
 
// destructor
 
G4LowEnergyCompton::~G4LowEnergyCompton()
{
   if (theCrossSectionTable) {

      delete theCrossSectionTable;
   }

   if (theScatteringFunctionTable) {

      delete theScatteringFunctionTable;
   }

   if (theMeanFreePathTable) {
      theMeanFreePathTable->clearAndDestroy();
      delete theMeanFreePathTable;
   }

   if(ZNumVec){
     ZNumVec->clear();
     delete ZNumVec;
   }
}
 
 
// methods.............................................................................

void G4LowEnergyCompton::BuildPhysicsTable(const G4ParticleDefinition& GammaType){

  BuildZVec();

  // Build microscopic cross section table and mean free path table
  BuildCrossSectionTable();

  // Build mean free path table for the Compton Scattering process
  BuildMeanFreePathTable();

  // build the scattering function table
  BuildScatteringFunctionTable();

}
// BUILD THE CS TABLE FOR THE ELEMENTS MAPPED IN ZNUMVEC
void G4LowEnergyCompton::BuildCrossSectionTable(){
 
  if (theCrossSectionTable) {
    
    delete theCrossSectionTable; 
  }
  
  theCrossSectionTable = new G4SecondLevel();
  G4int dataNum = 2;
  
  for(size_t TableInd = 0; TableInd < ZNumVec->size(); TableInd++){
    
    G4int AtomInd = (G4int) (*ZNumVec)[TableInd];
    
    G4FirstLevel* oneAtomCS = util.BuildFirstLevelTables(AtomInd, dataNum, "comp/ce-cs-");
    
    //    theCrossSectionTable->insert(oneAtomCS);
    theCrossSectionTable->push_back(oneAtomCS);
    
  }//end for on atoms
}
// BUILD THE SF TABLE FOR THE ELEMENTS MAPPED IN ZNUMVEC
void G4LowEnergyCompton::BuildScatteringFunctionTable(){

  if (theScatteringFunctionTable) {
    
    delete theScatteringFunctionTable; 
  }

  theScatteringFunctionTable = new G4SecondLevel();
  G4int dataNum = 2;
 
  for(size_t TableInd = 0; TableInd < ZNumVec->size(); TableInd++){

    G4int AtomInd = (G4int) (*ZNumVec)[TableInd];

    G4FirstLevel* oneAtomSF = util.BuildFirstLevelTables(AtomInd, dataNum, "comp/ce-sf-");
     
    //     theScatteringFunctionTable->insert(oneAtomSF);
     theScatteringFunctionTable->push_back(oneAtomSF);
   
  }//end for on atoms
}
// vector mapping the elements in the material table
void G4LowEnergyCompton::BuildZVec(){

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

      if(ZNumVec->contains(Zel) == FALSE){
	ZNumVec->push_back(Zel);
      }  else{
	continue;
      }
    }
  }
}


G4VParticleChange* G4LowEnergyCompton::PostStepDoIt(const G4Track& aTrack, const G4Step&  aStep){

//
// The scattered gamma energy is sampled according to Klein - Nishina formula.
// And then Accepted or rejected basing of the Scattering Function multiplied by factor 
// from Klein - Nishina formula. Expression of the angular distribution as Klein Nishina 
// angular and energy distribution and Scattering fuctions is taken from
// D. E. Cullen "A simple model of photon transport" Nucl. Instr. Meth. 
// Phys. Res. B 101 (1995). Method of sampling with form factors is different 
// data are interpolated while in the article they are fitted.
// Reference to the article is from J. Stepanek New Photon, Positron
// and Electron Interaction Data for GEANT in Energy Range from 1 eV to 10
// TeV (draft). 
// The random number techniques of Butcher & Messel are used 
// (Nuc Phys 20(1960),15).
// GEANT4 internal units
//
  aParticleChange.Initialize(aTrack);
  
  // Dynamic particle quantities  
  const G4DynamicParticle* aDynamicGamma = aTrack.GetDynamicParticle();
  G4double GammaEnergy0 = aDynamicGamma->GetKineticEnergy();
  if(GammaEnergy0 <= lowestEnergyLimit){
    
    aParticleChange.SetStatusChange(fStopAndKill);
    aParticleChange.SetEnergyChange(0.);
    aParticleChange.SetLocalEnergyDeposit(GammaEnergy0);
    
    return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);

  }


  G4double E0_m = GammaEnergy0 / electron_mass_c2 ;
  G4ParticleMomentum GammaDirection0 = aDynamicGamma->GetMomentumDirection();

  // Select randomly one element 
  G4Material* aMaterial = aTrack.GetMaterial();
  //  const G4int numOfElem = aMaterial->GetNumberOfElements();
  
  G4Element* theElement = SelectRandomAtom(aDynamicGamma, aMaterial);
  G4int elementZ = (G4int) theElement->GetZ();
  G4double epsilon, epsilonsq, onecost, sint2, greject ;

  G4double epsilon0 = 1./(1. + 2*E0_m) , epsilon0sq = epsilon0*epsilon0;
  G4double alpha1   = - log(epsilon0)  , alpha2 = 0.5*(1.- epsilon0sq);
  G4double ScatteringFunction, x;
  G4double wlGamma = h_Planck*c_light/GammaEnergy0;
  
  // sample the energy rate of the scattered gamma 
  do{
    
    if ( alpha1/(alpha1+alpha2) > G4UniformRand()){
      
      epsilon   = exp(-alpha1*G4UniformRand());  // pow(epsilon0,G4UniformRand())
      epsilonsq = epsilon*epsilon; 
    }
    else{
      
      epsilonsq = epsilon0sq + (1.- epsilon0sq)*G4UniformRand();
      epsilon   = sqrt(epsilonsq);
    }
    
    onecost = (1.- epsilon)/(epsilon*E0_m);
    sint2   = onecost*(2.-onecost);
    
    x = sqrt(onecost/2)/(wlGamma/cm);

    const G4FirstLevel* oneAtomSF
	  = (*theScatteringFunctionTable)[ZNumVec->index(elementZ)];

    ScatteringFunction = util.DataLogInterpolation(x, (*(*oneAtomSF)[0]), 
						   (*(*oneAtomSF)[1]));
    greject = (1. - epsilon*sint2/(1.+ epsilonsq))*ScatteringFunction;
    
  }  while(greject < G4UniformRand()*elementZ);

  G4double cosTeta = 1. - onecost , sinTeta = sqrt (sint2);
  G4double Phi     = twopi * G4UniformRand() ;
  G4double dirx = sinTeta*cos(Phi) , diry = sinTeta*sin(Phi) , dirz = cosTeta ;

  //
  // update G4VParticleChange for the scattered gamma 
  //
  
  G4ThreeVector GammaDirection1 ( dirx,diry,dirz );
  GammaDirection1.rotateUz(GammaDirection0);
  aParticleChange.SetMomentumChange( GammaDirection1 ) ;
  G4double GammaEnergy1 = epsilon*GammaEnergy0;
  if (GammaEnergy1 > 0.)
    {
      aParticleChange.SetEnergyChange( GammaEnergy1 ) ;
    }
  else
    {    
      aParticleChange.SetEnergyChange(0.) ;
      aParticleChange.SetStatusChange(fStopAndKill);
      
    }
  
  //
  // kinematic of the scattered electron
  //
  
  G4double ElecKineEnergy = GammaEnergy0 - GammaEnergy1 ;
  
  if (G4EnergyLossTables::GetRange(G4Electron::Electron(), ElecKineEnergy, aMaterial)
      >= G4std::min(G4Electron::GetCuts(), aStep.GetPostStepPoint()->GetSafety())){

    G4double ElecMomentum = sqrt(ElecKineEnergy*(ElecKineEnergy+2.*electron_mass_c2));
    G4ThreeVector ElecDirection((GammaEnergy0*GammaDirection0 - 
				 GammaEnergy1*GammaDirection1)*(1./ElecMomentum));
    
    // create G4DynamicParticle object for the electron.  
    G4DynamicParticle* aElectron= new G4DynamicParticle (G4Electron::Electron(),
							  ElecDirection, ElecKineEnergy) ;
    aParticleChange.SetNumberOfSecondaries(1);
    aParticleChange.AddSecondary( aElectron );
    aParticleChange.SetLocalEnergyDeposit (0.); 
  }
  else{
    
    aParticleChange.SetNumberOfSecondaries(0);
    aParticleChange.SetLocalEnergyDeposit (ElecKineEnergy);
   }
#ifdef G4VERBOSE
  if(verboseLevel > 0){
    G4cout<<"LE Compton Effect PostStepDoIt"<<G4endl;
  }
#endif  

  return G4VDiscreteProcess::PostStepDoIt( aTrack, aStep);
}


// used log-log interpolation instead of linear interpolation to build the MFP 
// as reported in the stepanek paper 
void G4LowEnergyCompton::BuildMeanFreePathTable(){

  if (theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy(); delete theMeanFreePathTable; }

  // material
  G4double NumbOfMaterials = G4Material::GetNumberOfMaterials();
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable() ;
  G4Material* material;

  // MeanFreePath 
  G4double LowEdgeEnergy, Value;
  theMeanFreePathTable = new G4PhysicsTable(NumbOfMaterials);
  G4PhysicsLogVector* ptrVector;

  for ( G4int J = 0 ; J < NumbOfMaterials; J++ ) { // For each material 
  
    //create physics vector then fill it ....
    ptrVector = new  G4PhysicsLogVector(lowestEnergyLimit, highestEnergyLimit, numbBinTable);
    
    material = (*theMaterialTable)(J);
    const G4ElementVector* theElementVector = material->GetElementVector();
    const G4double* theAtomNumDensityVector = material->GetAtomicNumDensityVector();   
    
    for ( G4int i = 0 ; i < numbBinTable ; i++ ){ 
      //For each energy
      
      LowEdgeEnergy = ptrVector->GetLowEdgeEnergy(i);
      
      const G4double BigPath= DBL_MAX;
      G4double SIGMA = 0 ;
      for ( size_t k=0 ; k < material->GetNumberOfElements() ; k++ ){ 

	G4int AtomIndex = (G4int) (*theElementVector)(k)->GetZ();
	const G4FirstLevel* oneAtomCS
	  = (*theCrossSectionTable)[ZNumVec->index(AtomIndex)];
	
	G4double interCrsSec = util.DataLogInterpolation(LowEdgeEnergy, 
							 (*(*oneAtomCS)[0]), 
							 (*(*oneAtomCS)[1]))*barn;
	SIGMA += theAtomNumDensityVector[k]*interCrsSec;
      }       
      
      Value = SIGMA<=0.0 ? BigPath : 1./SIGMA ;

      ptrVector->PutValue( i , Value ) ;

    }
    
    theMeanFreePathTable->insertAt( J , ptrVector );
  }
}

// METHOD BELOW  FROM STANDARD E_M PROCESSES CODE MODIFIED TO USE 
// LIVERMORE DATA (using log-log interpolation as reported in stepanek paper)
G4Element* G4LowEnergyCompton::SelectRandomAtom(const G4DynamicParticle* aDynamicGamma,
                                               G4Material* aMaterial){
  // select randomly 1 element within the material 
  G4double GammaEnergy = aDynamicGamma->GetKineticEnergy();
  const G4int NumberOfElements = aMaterial->GetNumberOfElements();
  const G4ElementVector* theElementVector = aMaterial->GetElementVector();

  if (NumberOfElements == 1) return (*theElementVector)(0);

  const G4double* theAtomNumDensityVector = aMaterial->GetAtomicNumDensityVector();
 
  G4double PartialSumSigma = 0.;

  G4double rval = 0;
  rval = G4UniformRand()/meanFreePath;

  for ( G4int i=0 ; i < NumberOfElements ; i++ ){ 

    G4double crossSection;
    if (GammaEnergy <  lowestEnergyLimit)
      crossSection = 0. ;
    else {
      if (GammaEnergy > highestEnergyLimit) GammaEnergy = 0.99*highestEnergyLimit ;
      
      G4int AtomIndex = (G4int) (*theElementVector)(i)->GetZ();
      const G4FirstLevel* oneAtomCS
	= (*theCrossSectionTable)[ZNumVec->index(AtomIndex)];

      crossSection =  util.DataLogInterpolation(GammaEnergy, 
						(*(*oneAtomCS)[0]), 
						(*(*oneAtomCS)[1]))*barn;
    }

    PartialSumSigma += theAtomNumDensityVector[i] * crossSection;
    if(rval <= PartialSumSigma) return ((*theElementVector)(i));
  }

  return (*theElementVector)(0);
}



















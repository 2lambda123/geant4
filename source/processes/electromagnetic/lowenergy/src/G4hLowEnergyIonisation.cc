// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// -------------------------------------------------------------
//      GEANT 4 class implementation file 
//
//      For information related to this code contact:
//      CERN, IT Division, ASD group
//      History: based on object model of
//      2nd December 1995, G.Cosmo
//      ---------- G4hLowEnergyIonisation physics process -------
//                by Vladimir Ivanchenko, 14 July 1999 
//                was made on the base of G4hIonisation class
//                developed by Laszlo Urban
// ************************************************************
// It is the extention of the ionisation process for the slow 
// charged hadrons.
// ************************************************************
// 28 July   1999 V.Ivanchenko cleen up
// 17 August 1999 G.Mancinelli added ICRU parametrisations for protons  
// 20 August 1999 G.Mancinelli added ICRU tables for alpha 
// 31 August 1999 V.Ivanchenko update and cleen up 
// 30 Sept.  1999 V.Ivanchenko minor upgrade 
// 12 Dec.   1999 S. Chauvie added Barkas correction 
// 19 Jan.   2000 V.Ivanchenko minor changing in Barkas corrections
// 02 April  2000 S. Chauvie linearization of Barkas effect
// 03 April  2000 V.Ivanchenko Nuclear Stopping power for antiprotons
// 23 May    2000 MG Pia  Clean up for QAO model 
// 24 May    2000 MG Pia  Code properly indented to improve legibility
// 17 July   2000 V.Ivanchenko Bug in scaling AlongStepDoIt method
// 25 July   2000 V.Ivanchenko New design iteration
// 17 August 2000 V.Ivanchenko Add ion fluctuation models
// 18 August 2000 V.Ivanchenko Bug fixed in GetConstrain
// 22 August 2000 V.Ivanchenko Insert paramStepLimit and
//                reorganise access to Barkas and Bloch terms  
// 04 Sept.  2000 V.Ivanchenko rename fluctuations
// 05 Sept.  2000 V.Ivanchenko clean up
// 03 Oct.   2000 V.Ivanchenko CodeWizard clean up
// 03 Nov.   2000 V.Ivanchenko MinKineticEnergy=LowestKineticEnergy=10eV
// 05 Nov.   2000 MG Pia - Removed const cast previously introduced to get
//                the code compiled (const G4Material* now introduced in 
//                electromagnetic/utils utils-V02-00-03 tag)
//                (this is going back and forth, to cope with Michel's
//                utils tag not being accepted yet by system testing)
// 21 Nov.  2000 V.Ivanchenko Fix a problem in fluctuations
// 23 Nov.  2000 V.Ivanchenko Ion type fluctuations only for charge>0
// 10 May   2001 V.Ivanchenko Clean up againist Linux compilation with -Wall
// 23 May   2001 V.Ivanchenko Minor fix in PostStepDoIt
// -----------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4hLowEnergyIonisation.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "G4Poisson.hh"
#include "G4UnitsTable.hh"
#include "G4EnergyLossTables.hh"
#include "G4Material.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hLowEnergyIonisation::G4hLowEnergyIonisation(const G4String& processName)
  : G4hLowEnergyLoss(processName),
    theBetheBlochModel(0),
    theProtonModel(0),
    theAntiProtonModel(0),
    theIonEffChargeModel(0),
    theNuclearStoppingModel(0),
    theIonChuFluctuationModel(0),
    theIonYangFluctuationModel(0),
    theProtonTable("ICRU_R49p"),
    theAntiProtonTable("ICRU_R49p"),
    theNuclearTable("ICRU_R49"),
    nStopping(true),
    theBarkas(true),
    theMeanFreePathTable(0),
    paramStepLimit (0.005)
{ 
  InitializeMe();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hLowEnergyIonisation::InitializeMe()
{
  LowestKineticEnergy  = 10.0*eV ;
  HighestKineticEnergy = 100.0*TeV ;
  MinKineticEnergy     = 10.0*eV ; 
  TotBin = 200 ;
  protonLowEnergy      = 1.*keV ; 
  protonHighEnergy     = 2.*MeV ;
  antiProtonLowEnergy  = 1.*keV ;
  antiProtonHighEnergy = 2.*MeV ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hLowEnergyIonisation::~G4hLowEnergyIonisation() 
{
  if (theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }
  if(theBetheBlochModel)delete theBetheBlochModel;
  if(theProtonModel)delete theProtonModel;
  if(theAntiProtonModel)delete theAntiProtonModel;
  if(theNuclearStoppingModel)delete theNuclearStoppingModel;
  if(theIonEffChargeModel)delete theIonEffChargeModel;
  if(theIonChuFluctuationModel)delete theIonChuFluctuationModel;
  if(theIonYangFluctuationModel)delete theIonYangFluctuationModel;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hLowEnergyIonisation::SetElectronicStoppingPowerModel(
                             const G4ParticleDefinition* aParticle,
                             const G4String& dedxTable)
  // This method defines the ionisation parametrisation method via its name 
{
  if(0 < aParticle->GetPDGCharge()) {
    SetProtonElectronicStoppingPowerModel(dedxTable) ;
  } else {
    SetAntiProtonElectronicStoppingPowerModel(dedxTable) ;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hLowEnergyIonisation::InitializeParametrisation() 

{
  G4Proton* theProton = G4Proton::Proton();
  G4AntiProton* theAntiProton = G4AntiProton::AntiProton();

  // cuts for  electron 
  G4Electron* theElectron = G4Electron::Electron();
  deltaCutInKineticEnergy = theElectron->GetCutsInEnergy() ;

  // Define models for parametrisation of electronic energy losses
  theBetheBlochModel = new G4hBetheBlochModel("Bethe-Bloch") ;
  theProtonModel = new G4hParametrisedLossModel(theProtonTable) ;
  theAntiProtonModel = new G4QAOLowEnergyLoss(theAntiProtonTable) ;
  theNuclearStoppingModel = new G4hNuclearStoppingModel(theNuclearTable) ;
  theIonEffChargeModel = new G4hIonEffChargeSquare("Ziegler1988") ;
  theIonChuFluctuationModel = new G4IonChuFluctuationModel("Chu") ;
  theIonYangFluctuationModel = new G4IonYangFluctuationModel("Yang") ;

  // Energy limits for parametrisation of electronic energy losses
  protonLowEnergy = G4std::max(protonLowEnergy,
                               theProtonModel->LowEnergyLimit(theProton)) ;   

  G4double x1 = theBetheBlochModel->LowEnergyLimit(theProton) ;   
  G4double x2 = theProtonModel->HighEnergyLimit(theProton) ;   
  if(protonHighEnergy < x1) protonHighEnergy = x1 ;
  if(protonHighEnergy > x2) protonHighEnergy = x2 ;
  
  antiProtonLowEnergy = G4std::max(antiProtonLowEnergy,
                      theAntiProtonModel->LowEnergyLimit(theAntiProton)) ;   

  x2 = theAntiProtonModel->HighEnergyLimit(theAntiProton) ;   
  if(antiProtonHighEnergy < x1) antiProtonHighEnergy = x1 ;
  if(antiProtonHighEnergy > x2) antiProtonHighEnergy = x2 ;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hLowEnergyIonisation::BuildPhysicsTable(
                       const G4ParticleDefinition& aParticleType) 
  
  //  just call BuildLossTable+BuildLambdaTable
{
  InitializeParametrisation() ;
  G4Proton* theProton = G4Proton::Proton();
  G4AntiProton* theAntiProton = G4AntiProton::AntiProton();
  G4Electron* theElectron = G4Electron::Electron();

  charge = aParticleType.GetPDGCharge()/eplus ;
  chargeSquare = charge*charge ;

  G4double electronCutInRange = theElectron->GetCuts(); 

  if(0.0 < charge) 
    {

      if( (ptableElectronCutInRange != electronCutInRange)  
	  || (theDEDXpTable == 0))
	{
	  BuildLossTable(*theProton) ;
	  RecorderOfpProcess[CounterOfpProcess] = theLossTable ;
	  CounterOfpProcess++;
	}

    } else{

      if( (pbartableElectronCutInRange != electronCutInRange)  
	  || (theDEDXpbarTable == 0))
	{
	  BuildLossTable(*theAntiProton) ;
	  RecorderOfpbarProcess[CounterOfpbarProcess] = theLossTable ;
	  CounterOfpbarProcess++;
	}
    }
  
  BuildLambdaTable(aParticleType) ;

  BuildDEDXTable(aParticleType) ;

  if((&aParticleType == theProton) ) PrintInfoDefinition() ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hLowEnergyIonisation::BuildLossTable(
                             const G4ParticleDefinition& aParticleType) 
{
  
  // Inicialisation
  G4double lowEdgeEnergy , ionloss, ionlossBB, paramB ;
  G4double lowEnergy, highEnergy;
  G4Proton* theProton = G4Proton::Proton();

  if(aParticleType == *theProton) {
    lowEnergy = protonLowEnergy ;
    highEnergy = protonHighEnergy ;
    charge = 1.0 ;
  } else {
    lowEnergy = antiProtonLowEnergy ;
    highEnergy = antiProtonHighEnergy ;
    charge = -1.0 ;
  }
  chargeSquare = 1.0 ;

  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  
  //  create table
  G4int numOfMaterials = theMaterialTable->length();
  
  if ( theLossTable) {
    theLossTable->clearAndDestroy();
    delete theLossTable;
  }

  theLossTable = new G4PhysicsTable(numOfMaterials);
  
  //  loop for materials  
  for (G4int j=0; j<numOfMaterials; j++) {

    // create physics vector and fill it  
    G4PhysicsLogVector* aVector = new G4PhysicsLogVector(LowestKineticEnergy, 
                                                         HighestKineticEnergy, 
                                                         TotBin);
      
    // get material parameters needed for the energy loss calculation  
    const G4Material* material= (*theMaterialTable)[j];

    // low energy of Bethe-Bloch formula for this material
    G4double highE = G4std::max(highEnergy,theBetheBlochModel->
                                LowEnergyLimit(&aParticleType,material)) ;

    if ( charge > 0.0 ) {
      ionloss = ProtonParametrisedDEDX(material,highE) ;
    } else {
      ionloss = AntiProtonParametrisedDEDX(material,highE) ;
    }

    ionlossBB = theBetheBlochModel->TheValue(&aParticleType,material,highE) ;
    ionlossBB -= DeltaRaysEnergy(material,highE,proton_mass_c2) ;

    if(theBarkas) {
      ionlossBB += BarkasTerm(material,highE)*charge ;
      ionlossBB += BlochTerm(material,highE,1.0) ;
    }
    paramB =  ionloss/ionlossBB - 1.0 ; 

    // now comes the loop for the kinetic energy values      
    for (G4int i = 0 ; i < TotBin ; i++) {
      lowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;

      // low energy part for this material, parametrised energy loss formulae
      if ( lowEdgeEnergy < highE ) {
	 
        if ( charge > 0.0 ) {
          ionloss = ProtonParametrisedDEDX(material,lowEdgeEnergy) ;
	} else {
          ionloss = AntiProtonParametrisedDEDX(material,lowEdgeEnergy) ;
	}

      } else {
	    
        // high energy part for this material, Bethe-Bloch formula
        ionloss = theBetheBlochModel->TheValue(theProton,material, 
                                               lowEdgeEnergy) ; 

        ionloss -= DeltaRaysEnergy(material,lowEdgeEnergy,proton_mass_c2) ;

        if(theBarkas) {
          ionloss += BarkasTerm(material,lowEdgeEnergy)*charge ;
          ionloss += BlochTerm(material,lowEdgeEnergy,1.0) ;
        }
	ionloss *= (1.0 + paramB*highEnergy/lowEdgeEnergy) ;
      }      
	  
      // now put the loss into the vector
      aVector->PutValue(i,ionloss) ;
    }
    // Insert vector for this material into the table
    theLossTable->insert(aVector) ;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hLowEnergyIonisation::BuildLambdaTable(
                       const G4ParticleDefinition& aParticleType) 
  
{
  // Build mean free path tables for the delta ray production process
  //     tables are built for MATERIALS 
  
  G4double lowEdgeEnergy, value;
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  charge = aParticleType.GetPDGCharge()/eplus ;
  chargeSquare = charge*charge ;
    
  //create table
  G4int numOfMaterials = theMaterialTable->length();
  
  if (theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }
  
  theMeanFreePathTable = new G4PhysicsTable(numOfMaterials);
  
  // get electron and particle cuts in kinetic energy
  
  // deltaCutInKineticEnergy = theElectron->GetCutsInEnergy() ;
  
  // loop for materials 
  
  for (G4int J=0 ; J < numOfMaterials; J++) { 

    //create physics vector then fill it ....      
    G4PhysicsLogVector* aVector = new G4PhysicsLogVector(LowestKineticEnergy, 
                                                         HighestKineticEnergy, 
                                                         TotBin);
      
    // compute the (macroscopic) cross section first
      
    const G4Material* material = (*theMaterialTable)[J];   
    const G4ElementVector* theElementVector = 
                           material->GetElementVector() ;
    const G4double* theAtomicNumDensityVector = 
                           material->GetAtomicNumDensityVector();
    const G4int NumberOfElements = material->GetNumberOfElements() ;
    G4double excEnergy = material->GetIonisation()->GetMeanExcitationEnergy();
      
      // get the electron kinetic energy cut for the actual material,
      //  it will be used in ComputeMicroscopicCrossSection
      // ( it is the SAME for ALL the ELEMENTS in THIS MATERIAL )
      //   ------------------------------------------------------
      
    // Cut in Delta energy is limited by exitation energy
    G4double deltaCut = G4std::max(excEnergy,deltaCutInKineticEnergy[J]) ;
      
    for ( G4int i = 0 ; i < TotBin ; i++ ) {
      lowEdgeEnergy = aVector->GetLowEdgeEnergy(i) ;
      chargeSquare = theIonEffChargeModel->
                     TheValue(&aParticleType,material,lowEdgeEnergy) ;
	  
      G4double sigma = 0.0 ;  
      for (G4int iel=0; iel<NumberOfElements; iel++ ) {
	sigma += theAtomicNumDensityVector[iel]*
                 chargeSquare*
		 ComputeMicroscopicCrossSection(
                        aParticleType,
			lowEdgeEnergy,
                        (*theElementVector)(iel)->GetZ(),
                        deltaCut ) ;
      }
	  
      // mean free path = 1./macroscopic cross section
	  
      value = sigma<=0 ? DBL_MAX : 1./sigma ;     
	  
      aVector->PutValue(i, value) ;
    }
      
    theMeanFreePathTable->insert(aVector);
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::ComputeMicroscopicCrossSection(
                           const G4ParticleDefinition& aParticleType,
			         G4double kineticEnergy,
				 G4double atomicNumber,
				 G4double deltaCutInEnergy) const
{
  //******************************************************************
  // cross section formula is OK for spin=0, 1/2, 1 only !
  // *****************************************************************
  
  // calculates the microscopic cross section in GEANT4 internal units
  //    ( it is called for elements , AtomicNumber = z )
  
  G4double energy, beta2, tmax, var ;
  G4double totalCrossSection = 0.0 ;
  
  G4double particleMass = aParticleType.GetPDGMass() ;
  
  // get particle data ...................................
  
  energy = kineticEnergy + particleMass;
  
  // some kinematics......................
  
  beta2  = kineticEnergy*(energy+particleMass) / (energy*energy);
  var    = particleMass+electron_mass_c2;
  tmax   = 2.0*electron_mass_c2*kineticEnergy * (energy+particleMass)
         / (var*var + 2.0*electron_mass_c2*kineticEnergy) ;
  
  // now you can calculate the total cross section 
  
  if( tmax > deltaCutInEnergy ) {
    
    var=deltaCutInEnergy/tmax;
    totalCrossSection = (1.0 - var*(1.0 - beta2*log(var))) / deltaCutInEnergy ;
    G4double spin = aParticleType.GetPDGSpin() ;
    
    // +term for spin=1/2 particle
    if( 0.5 == spin )
      totalCrossSection +=  0.5 * (tmax - deltaCutInEnergy) / (energy*energy);
    
    // +term for spin=1 particle
    else if( 0.9 < spin )
      totalCrossSection += -log(var)/(3.0*deltaCutInEnergy) +
	(tmax - deltaCutInEnergy) * ( (5.0+ 1.0/var)*0.25 / (energy*energy) - 
	 beta2 / (tmax * deltaCutInEnergy) ) / 3.0 ;
    
    totalCrossSection *= twopi_mc2_rcl2 * atomicNumber / beta2 ;
  }
  
  return totalCrossSection ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::GetConstraints(
                                 const G4DynamicParticle* particle,
				 const G4Material* material) 
{
  // returns the Step limit
  // dEdx is calculated as well as the range  
  // based on Effective Charge Approach
 
  G4Proton* theProton = G4Proton::Proton();
  G4AntiProton* theAntiProton = G4AntiProton::AntiProton();

  G4double stepLimit = 0.0 ;
  G4double dx, highEnergy;
  
  G4double massRatio = proton_mass_c2/(particle->GetMass()) ;
  G4double kineticEnergy = particle->GetKineticEnergy() ;


  charge = (particle->GetCharge())/eplus ;
  
  // Scale the kinetic energy
  
  G4double tscaled = kineticEnergy*massRatio ; 
  chargeSquare = theIonEffChargeModel->TheValue(particle,material) ;
  
  if(charge > 0.0) {
    
    highEnergy = protonHighEnergy ;

    //Very low energy dE/dx assumed to be according to Free Electron Gas Model
    if(tscaled < MinKineticEnergy) {
      fdEdx     = 0.5 * ProtonParametrisedDEDX(material, MinKineticEnergy) 
                * sqrt(tscaled/MinKineticEnergy) ;
      fRangeNow = tscaled/fdEdx ;
      fdEdx    *= chargeSquare ;
      dx        = fRangeNow/paramStepLimit ;

      // Normal energy
    } else {

      fRangeNow = G4EnergyLossTables::GetRange(theProton, tscaled, material) ;
      dx = G4EnergyLossTables::GetRange(theProton, highEnergy, material) ;

      if(tscaled > highEnergy) {
        fdEdx = G4EnergyLossTables::GetDEDX(theProton, tscaled, material) 
              * chargeSquare ;
        // Correction for positive ions
        if(theBarkas && 1.0 < charge) {
          G4double loss = BarkasTerm(material,tscaled)*(charge -1.0) 
                        * chargeSquare ;
          loss  += BlochTerm(material,tscaled,chargeSquare) ; 
          loss  -= BlochTerm(material,tscaled,1.0) ; 
          fdEdx += loss ;
	}

	// Parametrisation - recalculate dE/dx
      } else {    
        fdEdx = ProtonParametrisedDEDX(material, tscaled) * chargeSquare ;
      }
    }
    
    // Antiprotons and negative hadrons 
  } else {

    highEnergy = antiProtonHighEnergy ;

    //Very low energy dE/dx assumed to be constant
    if(tscaled < MinKineticEnergy) {
      fdEdx     = AntiProtonParametrisedDEDX(material, MinKineticEnergy)
                * sqrt(tscaled/MinKineticEnergy) ;
      fRangeNow = tscaled/fdEdx ;
      fdEdx    *= chargeSquare ;
      dx        = fRangeNow/paramStepLimit ;

      // Normal energy
    } else {

      fRangeNow = G4EnergyLossTables::GetRange(theAntiProton, tscaled, material) ;
      dx = G4EnergyLossTables::GetRange(theAntiProton, highEnergy, material) ;

      if(tscaled > highEnergy) {
        fdEdx = G4EnergyLossTables::GetDEDX(theAntiProton, tscaled, material) 
              * chargeSquare ;
    
      // For Bragg's peak dE/dx is recalculated
      } else {
        fdEdx = AntiProtonParametrisedDEDX(material, tscaled) * chargeSquare ;
      }
    }
  }
  
  // scaling back
  fRangeNow /= (chargeSquare*massRatio) ;
  dx        /= (chargeSquare*massRatio) ;
  stepLimit  = fRangeNow ;
  
  // compute the (random) Step limit in standard energy range
  if(tscaled > highEnergy ) {
    if(fRangeNow > finalRange) {
      stepLimit = (c1lim*fRangeNow+c2lim+c3lim/fRangeNow) ;
      
      //  randomise this value
      if(rndmStepFlag) stepLimit = finalRange + 
                                  (stepLimit-finalRange)*G4UniformRand() ;
    }
    // cut step in front of Bragg's peak
    if(stepLimit > fRangeNow - dx*0.9) stepLimit = fRangeNow - dx*0.9 ;

  // Step limit in low energy range
  } else {
    stepLimit = G4std::min(fRangeNow, dx*paramStepLimit) ;    
  }

  return stepLimit ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* G4hLowEnergyIonisation::AlongStepDoIt(
                                           const G4Track& trackData, 
                                           const G4Step& stepData) 
{
  // compute the energy loss after a step 
  G4Proton* theProton = G4Proton::Proton();
  G4AntiProton* theAntiProton = G4AntiProton::AntiProton();
  G4double finalT = 0.0 ;
  
  aParticleChange.Initialize(trackData) ;
  G4Material* material = trackData.GetMaterial() ;
  
  // get the actual (true) Step length from stepData 
  const G4double step = stepData.GetStepLength() ;
  
  const G4DynamicParticle* particle = trackData.GetDynamicParticle() ;
  
  G4double kineticEnergy = particle->GetKineticEnergy() ;
  G4double massRatio = proton_mass_c2/(particle->GetMass()) ;
  G4double tscaled= kineticEnergy*massRatio ; 
  G4double eloss = 0.0 ;
  G4double nloss = 0.0 ;

  
    // very small particle energy
  if(kineticEnergy < MinKineticEnergy) {

    eloss = kineticEnergy ;  

    // particle energy outside tabulated energy range
  } else if( kineticEnergy > HighestKineticEnergy) { 
    eloss = step*fdEdx ; 
 
    // proton parametrisation model  
  } else if(tscaled < protonHighEnergy && charge > 0.0) {
    
    if(nStopping) nloss = 
      (theNuclearStoppingModel->TheValue(particle, material))*step ;
    
    G4double eFinal = kineticEnergy - step*fdEdx - nloss ;
    
    if(0.0 < eFinal) {
      eloss = (fdEdx + 
               ProtonParametrisedDEDX(material,eFinal*massRatio)*chargeSquare)
            *  step * 0.5 ;
    } else {
      eloss = kineticEnergy - nloss ;
    }

    // antiproton parametrisation model
  } else if(tscaled < antiProtonHighEnergy && charge < 0.0) {
    
    if(nStopping) nloss = 
      (theNuclearStoppingModel->TheValue(particle, material))*step ;
    
    G4double eFinal = kineticEnergy - step*fdEdx - nloss ;
    
    if(0.0 < eFinal) {
      eloss = (fdEdx + 
            AntiProtonParametrisedDEDX(material,eFinal*massRatio)*chargeSquare)
            *  step * 0.5 ;
    } else {
      eloss = kineticEnergy - nloss ;
    }

    // big step
  } else if(step >= fRangeNow ) {
    eloss = kineticEnergy ;

    // tabulated range 
  } else {
    
    // step longer than linear step limit 
    if(step > linLossLimit*fRangeNow) {
      
      G4double rscaled= fRangeNow*massRatio*chargeSquare ;
      G4double sscaled=   step   *massRatio*chargeSquare ;
      
      if(charge > 0.0) {
        eloss = G4EnergyLossTables::GetPreciseEnergyFromRange(
                                    theProton,rscaled, material) -
	        G4EnergyLossTables::GetPreciseEnergyFromRange(
                                    theProton,rscaled-sscaled,material) ;
       
      } else {
        eloss = G4EnergyLossTables::GetPreciseEnergyFromRange(
                                    theAntiProton,rscaled,material) -
	        G4EnergyLossTables::GetPreciseEnergyFromRange(
                                    theAntiProton,rscaled-sscaled,material) ;
      }
      eloss /= massRatio ;

    // step shorter than linear step limit       
    } else {
      eloss = step*fdEdx ;
    }
  }
  
  finalT = kineticEnergy - eloss - nloss ;

  if( EnlossFlucFlag && 0.0 < eloss ) {
    
    //  now the electron loss with fluctuation
    eloss = ElectronicLossFluctuation(particle, material, eloss, step) ;
    finalT = kineticEnergy - eloss - nloss ;    
  }
  
  //  stop particle if the kinetic energy <= MinKineticEnergy
  if (finalT <= MinKineticEnergy )
    {
      finalT = 0.0 ;
      if( "proton" == (particle->GetDefinition()->GetParticleName()) )
	aParticleChange.SetStatusChange(fStopAndKill);
      else  
	aParticleChange.SetStatusChange(fStopButAlive); 
    } 
  
  aParticleChange.SetEnergyChange( finalT ) ;
  aParticleChange.SetLocalEnergyDeposit(kineticEnergy-finalT) ;
  
  return &aParticleChange ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::ProtonParametrisedDEDX(
                                 const G4Material* material, 
				       G4double kineticEnergy) const
{
  G4Proton* theProton = G4Proton::Proton();
  G4double eloss = 0.0;

    // Free Electron Gas Model  
  if(kineticEnergy < protonLowEnergy) {
    eloss = (theProtonModel->TheValue(theProton, material, protonLowEnergy))
          * sqrt(kineticEnergy/protonLowEnergy) ;
    
    // Parametrisation
  } else {
    eloss = theProtonModel->TheValue(theProton, material, kineticEnergy) ;
  }
  
  // Delta rays energy
  eloss -= DeltaRaysEnergy(material,kineticEnergy,proton_mass_c2) ;

  if(eloss < 0.0) eloss = 0.0 ;  
  
  return eloss ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::AntiProtonParametrisedDEDX(
                                 const G4Material* material, 
				       G4double kineticEnergy) const
{
  G4AntiProton* theAntiProton = G4AntiProton::AntiProton();
  G4double eloss = 0.0 ;
  //  G4double goldenRule = 1.0 ;

  // Choose the model
  G4VLowEnergyModel * theModel ;
  if(theAntiProtonModel->IsInCharge(theAntiProton,material)) {
    theModel = theAntiProtonModel ;
  } else { 
    theModel = theProtonModel ;
    // goldenRule = AntiProtonGoldenRule(kineticEnergy) ;
  }

    // Free Electron Gas Model is not used for antiprotons  
  if(kineticEnergy < antiProtonLowEnergy) {
    eloss = theModel->TheValue(theAntiProton, material, antiProtonLowEnergy);
    //          * sqrt(kineticEnergy/protonLowEnergy) ;
    
    // Parametrisation using golden rule for antiprotons
  } else {
    eloss = theModel->TheValue(theAntiProton, material, kineticEnergy) ;
  }
    
  // Taken into account golden rule for antiprotons
  //eloss *= goldenRule ;

    // Proton model is used
  if(theBarkas && (theModel == theProtonModel)) 
                  eloss -= 2.0*BarkasTerm(material, kineticEnergy);
  
  // Delta rays energy
  eloss -= DeltaRaysEnergy(material,kineticEnergy,proton_mass_c2) ;

  if(eloss < 0.0) eloss = 0.0 ;  
  
  return eloss ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::DeltaRaysEnergy(
                                 const G4Material* material, 
                                       G4double kineticEnergy,
                                       G4double particleMass) const
{
  G4double dloss = 0.0 ;

  G4double deltaCutNow = deltaCutInKineticEnergy[(material->GetIndex())] ;   
  G4double electronDensity = material->GetElectronDensity();
  G4double eexc = material->GetIonisation()->GetMeanExcitationEnergy();

  G4double tau = kineticEnergy/particleMass ;    
  G4double rateMass = electron_mass_c2/particleMass ;
  
  // some local variables 
  
  G4double gamma,bg2,beta2,tmax,x ;
  
  gamma = tau + 1.0 ;
  bg2 = tau*(tau+2.0) ;
  beta2 = bg2/(gamma*gamma) ;
  tmax = 2.*electron_mass_c2*bg2/(1.0+2.0*gamma*rateMass+rateMass*rateMass) ;

  // Validity range for delta electron cross section
  G4double deltaCut = G4std::max(deltaCutNow, eexc);
    
  if ( deltaCut < tmax) {
    x = deltaCut / tmax ;
    dloss = ( beta2 * (x - 1.0) - log(x) ) * twopi_mc2_rcl2 
          * electronDensity / beta2 ;
  }
  return dloss ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* G4hLowEnergyIonisation::PostStepDoIt(
                                           const G4Track& trackData,   
					   const G4Step& stepData) 
{
  // Units are expressed in GEANT4 internal units.
  
  G4double KineticEnergy,TotalEnergy,TotalMomentum,
           betasquare,MaxKineticEnergyTransfer,
           DeltaKineticEnergy,DeltaTotalMomentum,costheta,sintheta,phi,
           dirx,diry,dirz,finalKineticEnergy,finalPx,finalPy,finalPz,
           x,xc,grej,Psquare,Esquare,summass,rate,finalMomentum ;
  
  aParticleChange.Initialize(trackData) ;
  G4Material* aMaterial = trackData.GetMaterial() ;
  G4double Eexc = aMaterial->GetIonisation()->GetMeanExcitationEnergy();
  
  const G4DynamicParticle* aParticle = trackData.GetDynamicParticle() ;
  
  ParticleMass=aParticle->GetDefinition()->GetPDGMass();
  KineticEnergy=aParticle->GetKineticEnergy();
  TotalEnergy=KineticEnergy + ParticleMass ;
  Psquare=KineticEnergy*(TotalEnergy+ParticleMass) ;
  Esquare=TotalEnergy*TotalEnergy ;
  summass = ParticleMass + electron_mass_c2 ;    
  G4ParticleMomentum ParticleDirection = aParticle->GetMomentumDirection() ;
  
  //  get kinetic energy cut for the electron....
  G4double DeltaCutInKineticEnergyNow = 
           deltaCutInKineticEnergy[aMaterial->GetIndex()];

  // some kinematics......................
  
  betasquare=Psquare/Esquare ;
  MaxKineticEnergyTransfer = 2.*electron_mass_c2*Psquare
    /(summass*summass+2.*electron_mass_c2*KineticEnergy);

  // Validity range for delta electron cross section
  G4double DeltaCut = G4std::max(DeltaCutInKineticEnergyNow,Eexc);
    
  // sampling kinetic energy of the delta ray 
  
  if( MaxKineticEnergyTransfer <= DeltaCut )
    {
      // pathological case (it should not happen ,
      // there is no change at all).....
      
      return &aParticleChange;
      //return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);
    }
  else
    {
      // normal case ......................................
      xc   = DeltaCut / MaxKineticEnergyTransfer ;
      rate = MaxKineticEnergyTransfer / TotalEnergy ;
      rate = rate*rate ;
      G4double spin = aParticle->GetDefinition()->GetPDGSpin() ;   
      
      // sampling follows ...      
      do {
	x=xc/(1.-(1.-xc)*G4UniformRand());

        if(0.0 == spin) {
          grej = 1.0 - betasquare * x ;

        } else if (0.5 == spin) {
          grej = (1.0 - betasquare * x + 0.5*x*x*rate) / (1.0 + 0.5 * rate) ;

        } else {
          grej = (1.0 - betasquare * x ) * (1.0 + x/ (3.0*xc)) + 
	    x * x * rate * (1.0 + 0.5 * x / xc) / 3.0 / 
	    (1.0 + 1.0/(3.0*xc) + rate *(1.0+ 0.5/xc) /3.0) ;
        }

      } while( G4UniformRand() > grej );
    }
  
  DeltaKineticEnergy = x * MaxKineticEnergyTransfer ;
  
  if(DeltaKineticEnergy <= 0.)
    return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);
  
  DeltaTotalMomentum = sqrt(DeltaKineticEnergy * (DeltaKineticEnergy +
						  2. * electron_mass_c2 )) ;
  TotalMomentum = sqrt(Psquare) ;
  costheta = DeltaKineticEnergy * (TotalEnergy + electron_mass_c2)
    /(DeltaTotalMomentum * TotalMomentum) ;
  
  //  protection against costheta > 1 or < -1   ---------------
  if ( costheta < -1. ) 
    costheta = -1. ;
  if ( costheta > +1. ) 
    costheta = +1. ;
  
  //  direction of the delta electron  ........
  phi = twopi * G4UniformRand() ; 
  sintheta = sqrt((1.+costheta)*(1.-costheta));
  dirx = sintheta * cos(phi) ;
  diry = sintheta * sin(phi) ;
  dirz = costheta ;
  
  G4ThreeVector DeltaDirection(dirx,diry,dirz) ;
  DeltaDirection.rotateUz(ParticleDirection) ;
  
  // create G4DynamicParticle object for delta ray
  G4DynamicParticle *theDeltaRay = new G4DynamicParticle;
  theDeltaRay->SetKineticEnergy( DeltaKineticEnergy );
  theDeltaRay->SetMomentumDirection(DeltaDirection.x(),
                                    DeltaDirection.y(),
                                    DeltaDirection.z()); 
  theDeltaRay->SetDefinition(G4Electron::Electron());
  
  // fill aParticleChange 
  finalKineticEnergy = KineticEnergy - DeltaKineticEnergy ;
  G4double Edep = 0 ;
  
  if (finalKineticEnergy > MinKineticEnergy)
    {
      finalPx = TotalMomentum*ParticleDirection.x()
	- DeltaTotalMomentum*DeltaDirection.x();
      finalPy = TotalMomentum*ParticleDirection.y()
	- DeltaTotalMomentum*DeltaDirection.y();
      finalPz = TotalMomentum*ParticleDirection.z()
	- DeltaTotalMomentum*DeltaDirection.z();
      finalMomentum =
	sqrt(finalPx*finalPx+finalPy*finalPy+finalPz*finalPz) ;
      finalPx /= finalMomentum ;
      finalPy /= finalMomentum ;
      finalPz /= finalMomentum ;
      
      aParticleChange.SetMomentumChange( finalPx,finalPy,finalPz );
    }
  else
    {
      finalKineticEnergy = 0. ;
      Edep = finalKineticEnergy ;
      if (aParticle->GetDefinition()->GetParticleName() == "proton")
	aParticleChange.SetStatusChange(fStopAndKill);
      else  aParticleChange.SetStatusChange(fStopButAlive);
    }
  
  aParticleChange.SetEnergyChange( finalKineticEnergy );
  aParticleChange.SetNumberOfSecondaries(1);   
  aParticleChange.AddSecondary( theDeltaRay );
  aParticleChange.SetLocalEnergyDeposit (Edep);
  
  return G4VContinuousDiscreteProcess::PostStepDoIt(trackData,stepData);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::ComputeDEDX(
                                 const G4ParticleDefinition* aParticle,
                                 const G4Material* material, 
				 G4double kineticEnergy) 
{  
  G4Proton* theProton = G4Proton::Proton();
  G4AntiProton* theAntiProton = G4AntiProton::AntiProton();
  G4double dedx = 0.0 ;
    
  G4double tscaled = kineticEnergy*proton_mass_c2/(aParticle->GetPDGMass()) ; 
  charge  = aParticle->GetPDGCharge() ;
  
  if(charge>0.0) {
    if(tscaled > protonHighEnergy) {
      dedx=G4EnergyLossTables::GetDEDX(theProton,tscaled,material) ;

    } else {
      dedx=ProtonParametrisedDEDX(material,tscaled) ;
    }

  } else {
    if(tscaled > antiProtonHighEnergy) {
      dedx=G4EnergyLossTables::GetDEDX(theAntiProton,tscaled,material); 
      
    } else {
      dedx=AntiProtonParametrisedDEDX(material,tscaled) ;
    }
  }    
  dedx *= theIonEffChargeModel->TheValue(aParticle, material, kineticEnergy) ;
  
  return dedx ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::BarkasTerm(const G4Material* material,
  				                  G4double kineticEnergy) const
//Function to compute the Barkas term for protons:
//
//Ref. Z_1^3 effect in the stopping power of matter for charged particles
//     J.C Ashley and R.H.Ritchie
//     Physical review B Vol.5 No.7 1 April 1972 pagg. 2393-2397 
//
{
  static double FTable[47][2] = { 
   { 0.02, 21.5},
   { 0.03, 20.0},	
   { 0.04, 18.0},	
   { 0.05, 15.6},
   { 0.06, 15.0},	
   { 0.07, 14.0},	
   { 0.08, 13.5},	
   { 0.09, 13.},
   { 0.1,  12.2},  	
   { 0.2,  9.25},  	
   { 0.3,  7.0},  
   { 0.4,  6.0},
   { 0.5,  4.5}, 	
   { 0.6,  3.5},	
   { 0.7,  3.0},  
   { 0.8,  2.5},
   { 0.9,  2.0},  	
   { 1.0,  1.7},  	
   { 1.2,  1.2},	
   { 1.3,  1.0},  
   { 1.4,  0.86},  	
   { 1.5,  0.7},	
   { 1.6,  0.61},  	
   { 1.7,  0.52},  
   { 1.8,  0.5},	
   { 1.9,  0.43},	
   { 2.0,  0.42},  	
   { 2.1,  0.3},
   { 2.4,  0.2},	
   { 3.0,  0.13},  	
   { 3.08, 0.1},	
   { 3.1,  0.09}, 
   { 3.3,  0.08},  
   { 3.5,  0.07},  	
   { 3.8,  0.06},	
   { 4.0,  0.051}, 
   { 4.1,  0.04},  	
   { 4.8,  0.03}, 
   { 5.0,  0.024},
   { 5.1,  0.02},
   { 6.0,  0.013},
   { 6.5,  0.01},
   { 7.0,  0.009}, 	
   { 7.1,  0.008},
   { 8.0,  0.006}, 	
   { 9.0,  0.0032}, 
   { 10.0, 0.0025} };

  // Information on particle and material
  G4double kinE  = kineticEnergy ;
  if(0.5*MeV > kinE) kinE = 0.5*MeV ; 
  G4double gamma = 1.0 + kinE / proton_mass_c2 ;
  G4double beta2 = 1.0 - 1.0/(gamma*gamma) ;
  if(0.0 >= beta2) return 0.0;
  
  G4double BarkasTerm = 0.0;
  G4double AMaterial = 0.0;
  G4double ZMaterial = 0.0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int numberOfElements = material->GetNumberOfElements();
  
  for (G4int i = 0; i<numberOfElements; i++) {

    AMaterial = (*theElementVector)(i)->GetA()*mole/g;
    ZMaterial = (*theElementVector)(i)->GetZ();
    
    G4double X = 137.0 * 137.0 * beta2 / ZMaterial;
  
    // Variables to compute L_1
    G4double Eta0Chi = 0.8;
    G4double EtaChi = Eta0Chi * ( 1.0 + 6.02*pow( ZMaterial,-1.19 ) );
    G4double W = ( EtaChi * pow( ZMaterial,1.0/6.0 ) ) / sqrt(X); 
    G4double FunctionOfW = FTable[46][1]*FTable[46][0]/W ;
    
    for(G4int j=0; j<47; j++) {
    
      if( W < FTable[j][0] ) {
    
        if(0 == j) {
     	  FunctionOfW = FTable[0][1] ;

        } else {
          FunctionOfW = (FTable[j][1] - FTable[j-1][1]) * (W - FTable[j-1][0]) 
	              / (FTable[j][0] - FTable[j-1][0])
	              +  FTable[j-1][1] ;
	}
    
        break;
      }
    
    }
    
    BarkasTerm += FunctionOfW /( sqrt(ZMaterial * X) * X);
  }

  BarkasTerm *= twopi_mc2_rcl2 * (material->GetElectronDensity()) / beta2 ;

  return BarkasTerm;
}
        
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::BlochTerm(const G4Material* material,
                                                 G4double kineticEnergy,
                                                 G4double cSquare) const
//Function to compute the Bloch term for protons:
//
//Ref. Z_1^3 effect in the stopping power of matter for charged particles
//     J.C Ashley and R.H.Ritchie
//     Physical review B Vol.5 No.7 1 April 1972 pagg. 2393-2397 
//
{
  G4double eloss = 0.0 ;
  G4double gamma = 1.0 + kineticEnergy / proton_mass_c2 ;
  G4double beta2 = 1.0 - 1.0/(gamma*gamma) ;
  G4double y = cSquare / (137.0*137.0*beta2) ;

  if(y < 0.05) {
    eloss = 1.202 ;
     
  } else {
    eloss = 1.0 / (1.0 + y) ;
    G4double de = eloss ;

    for(G4int i=2; de>eloss*0.01; i++) {
      de = 1.0/( i * (i*i + y)) ;
      eloss += de ;
    }
  }
  eloss *= -1.0 * y * cSquare * twopi_mc2_rcl2 * 
            (material->GetElectronDensity()) / beta2 ;
 
  return eloss;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hLowEnergyIonisation::ElectronicLossFluctuation(
                                 const G4DynamicParticle* particle,
                                 const G4Material* material,
                                       G4double meanLoss,
                                       G4double step) const
//  calculate actual loss from the mean loss
//  The model used to get the fluctuation is essentially the same 
// as in Glandz in Geant3.
{
  // data members to speed up the fluctuation calculation
  //  G4int imat ;
  //  G4double f1Fluct,f2Fluct,e1Fluct,e2Fluct,rateFluct,ipotFluct;
  //  G4double e1LogFluct,e2LogFluct,ipotLogFluct;

  static const G4double minLoss = 1.*eV ;
  static const G4double kappa = 10. ;
  static const G4double theBohrBeta2 = 50.0 * keV/proton_mass_c2 ;

  G4int    imaterial   = material->GetIndex() ; 
  G4double ipotFluct   = material->GetIonisation()->GetMeanExcitationEnergy() ;
  G4double electronDensity = material->GetElectronDensity() ;
  G4double zeff = electronDensity/(material->GetTotNbOfAtomsPerVolume()) ;

  // get particle data
  G4double tkin   = particle->GetKineticEnergy();
  G4double particleMass = particle->GetMass() ;
  G4double deltaCutInKineticEnergyNow = deltaCutInKineticEnergy[imaterial];

  // shortcut for very very small loss 
  if(meanLoss < minLoss) return meanLoss ;

  // Validity range for delta electron cross section
  G4double threshold = G4std::max(deltaCutInKineticEnergyNow,ipotFluct);
  G4double loss, siga;

  G4double rmass = electron_mass_c2/particleMass;
  G4double tau   = tkin/particleMass; 
  G4double tau1 = tau+1.0; 
  G4double tau2 = tau*(tau+2.);
  G4double tmax    = 2.*electron_mass_c2*tau2/(1.+2.*tau1*rmass+rmass*rmass);

  if (tmax <= ipotFluct) tmax = ipotFluct ;
  
  if(tmax > threshold) tmax = threshold;
  G4double beta2 = tau2/(tau1*tau1);

  // Gaussian fluctuation 
  if(meanLoss > kappa*tmax)
  {
    siga = tmax * (1.0-0.5*beta2) * step * twopi_mc2_rcl2 
         * electronDensity / beta2 ;

    // High velocity or negatively charged particle
    if( beta2 > 3.0*theBohrBeta2*zeff || charge < 0.0) {
      siga = sqrt( siga * chargeSquare ) ;

    // Low velocity - additional ion charge fluctuations according to
    // Q.Yang et al., NIM B61(1991)149-155.
    } else {
      G4double chu = theIonChuFluctuationModel->TheValue(particle, material);
      G4double yang = theIonYangFluctuationModel->TheValue(particle, material);
      siga = sqrt( siga * (chargeSquare * chu + yang)) ;
    }

    loss = G4RandGauss::shoot(meanLoss,siga) ;
    if(loss < 0.) loss = 0. ;
    return loss ;
  }

  // Non Gaussian fluctuation 
  G4double mLoss = meanLoss/chargeSquare;
  static const G4double probLim = 0.01 ;
  static const G4double sumaLim = -log(probLim) ;
  static const G4double alim = 10.;

  G4double suma,w1,w2,C,e0,lossc,w;
  G4double a1,a2,a3;
  G4int p1,p2,p3;
  G4int nb;
  G4double corrfac, na,alfa,rfac,namean,sa,alfa1,ea,sea;
  G4double dp3;

  G4double f1Fluct     = material->GetIonisation()->GetF1fluct();
  G4double f2Fluct     = material->GetIonisation()->GetF2fluct();
  G4double e1Fluct     = material->GetIonisation()->GetEnergy1fluct();
  G4double e2Fluct     = material->GetIonisation()->GetEnergy2fluct();
  G4double e1LogFluct  = material->GetIonisation()->GetLogEnergy1fluct();
  G4double e2LogFluct  = material->GetIonisation()->GetLogEnergy2fluct();
  G4double rateFluct   = material->GetIonisation()->GetRateionexcfluct();
  G4double ipotLogFluct= material->GetIonisation()->GetLogMeanExcEnergy();

  w1 = tmax/ipotFluct;
  w2 = log(2.*electron_mass_c2*tau2);

  C = mLoss*(1.-rateFluct)/(w2-ipotLogFluct-beta2);

  a1 = C*f1Fluct*(w2-e1LogFluct-beta2)/e1Fluct;
  a2 = C*f2Fluct*(w2-e2LogFluct-beta2)/e2Fluct;
  if(tmax > ipotFluct)
    a3 = rateFluct*mLoss*(tmax-ipotFluct)/(ipotFluct*tmax*log(w1));
  else
  {
     a1 /= 1.-rateFluct ;
     a2 /= 1.-rateFluct ;
     a3  = 0. ;
  } 

  suma = a1+a2+a3;

  loss = 0. ;

  if(suma < sumaLim)             // very small Step
    {
      e0 = material->GetIonisation()->GetEnergy0fluct();

      if(tmax == ipotFluct)
      {
        a3 = mLoss/e0;

        if(a3>alim)
        {
          siga=sqrt(a3) ;
          p3 = G4std::max(0,int(G4RandGauss::shoot(a3,siga)+0.5));
        }
        else
          p3 = G4Poisson(a3);

        loss = p3*e0 ;

        if(p3 > 0)
          loss += (1.-2.*G4UniformRand())*e0 ;

      }
      else
      {
        tmax = tmax-ipotFluct+e0 ;
        a3 = mLoss*(tmax-e0)/(tmax*e0*log(tmax/e0));

        if(a3>alim)
        {
          siga=sqrt(a3) ;
          p3 = G4std::max(0,int(G4RandGauss::shoot(a3,siga)+0.5));
        }
        else
          p3 = G4Poisson(a3);

        if(p3 > 0)
        {
          w = (tmax-e0)/tmax ;
          if(p3 > nmaxCont2)
          {
            dp3 = G4float(p3) ;
            corrfac = dp3/G4float(nmaxCont2) ;
            p3 = nmaxCont2 ;
          }
          else
            corrfac = 1. ;

          for(G4int i=0; i<p3; i++) loss += 1./(1.-w*G4UniformRand()) ;
          loss *= e0*corrfac ;  
        }        
      }
    }
    
  else                              // not so small Step
    {
      // excitation type 1
      if(a1>alim)
      {
        siga=sqrt(a1) ;
        p1 = G4std::max(0,int(G4RandGauss::shoot(a1,siga)+0.5));
      }
      else
       p1 = G4Poisson(a1);

      // excitation type 2
      if(a2>alim)
      {
        siga=sqrt(a2) ;
        p2 = G4std::max(0,int(G4RandGauss::shoot(a2,siga)+0.5));
      }
      else
        p2 = G4Poisson(a2);

      loss = p1*e1Fluct+p2*e2Fluct;
 
      // smearing to avoid unphysical peaks
      if(p2 > 0)
        loss += (1.-2.*G4UniformRand())*e2Fluct;   
      else if (loss>0.)
        loss += (1.-2.*G4UniformRand())*e1Fluct;   

      // ionisation .......................................
     if(a3 > 0.)
     {
      if(a3>alim)
      {
        siga=sqrt(a3) ;
        p3 = G4std::max(0,int(G4RandGauss::shoot(a3,siga)+0.5));
      }
      else
        p3 = G4Poisson(a3);

      lossc = 0.;
      if(p3 > 0)
      {
        na = 0.; 
        alfa = 1.;
        if (p3 > nmaxCont2)
        {
          dp3        = G4float(p3);
          rfac       = dp3/(G4float(nmaxCont2)+dp3);
          namean     = G4float(p3)*rfac;
          sa         = G4float(nmaxCont1)*rfac;
          na         = G4RandGauss::shoot(namean,sa);
          if (na > 0.)
          {
            alfa   = w1*G4float(nmaxCont2+p3)/
                    (w1*G4float(nmaxCont2)+G4float(p3));
            alfa1  = alfa*log(alfa)/(alfa-1.);
            ea     = na*ipotFluct*alfa1;
            sea    = ipotFluct*sqrt(na*(alfa-alfa1*alfa1));
            lossc += G4RandGauss::shoot(ea,sea);
          }
        }

        nb = G4int(G4float(p3)-na);
        if (nb > 0)
        {
          w2 = alfa*ipotFluct;
          w  = (tmax-w2)/tmax;      
          for (G4int k=0; k<nb; k++) lossc += w2/(1.-w*G4UniformRand());
        }
      }        
      loss += lossc;  
     }
    } 
  loss *= chargeSquare;

  return loss ;
}
        
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4hLowEnergyIonisation::PrintInfoDefinition() const
{
  G4String comments = "  Knock-on electron cross sections . ";
  comments += "\n        Good description above the mean excitation energy.\n";
  comments += "        Delta ray energy sampled from  differential Xsection.";
  
  G4cout << G4endl << GetProcessName() << ":  " << comments
         << "\n        PhysicsTables from " << LowestKineticEnergy / eV << " eV " 
         << " to " << HighestKineticEnergy / TeV << " TeV "
         << " in " << TotBin << " bins."
 << "\n        Electronic stopping power model is  " 
 << theProtonTable
         << "\n        from " << protonLowEnergy / keV << " keV "
         << " to " << protonHighEnergy / MeV << " MeV " << "." << G4endl ;
  G4cout << "\n        Parametrisation model for antiprotons is  "
         << theAntiProtonTable
         << "\n        from " << antiProtonLowEnergy / keV << " keV "
         << " to " << antiProtonHighEnergy / MeV << " MeV " << "." << G4endl ;
  if(theBarkas){
  G4cout << "        Parametrization of the Barkas effect is switched on." 
         << G4endl ;
  }
  if(nStopping) {
  G4cout << "        Nuclear stopping power model is " << theNuclearTable 
         << G4endl ; 
  }

  G4bool printHead = true;

  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  G4int numOfMaterials = theMaterialTable->length();
  
  // loop for materials 
  
  for (G4int j=0 ; j < numOfMaterials; j++) { 

    const G4Material* material= (*theMaterialTable)[j];
    G4double deltaCutNow = deltaCutInKineticEnergy[(material->GetIndex())] ;   
    G4double eexc = material->GetIonisation()->GetMeanExcitationEnergy();

    if(eexc > deltaCutNow) {
      if(printHead) {
        printHead = false ;

        G4cout << "           material       min.delta energy(keV) " << G4endl;
        G4cout << G4endl;
      }

      G4cout << G4std::setw(20) << material->GetName()
	     << G4std::setw(15) << eexc/keV << G4endl;
    }
  }    
}

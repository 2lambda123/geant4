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
// $Id: hEnergyLossTest.cc,v 1.6 2004-12-01 19:37:16 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//-----------------------------------------------------------------
#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include "g4templates.hh"
#include "globals.hh"
#include "G4Timer.hh"     
#include "G4eEnergyLoss.hh"
#include "G4eIonisation.hh"
#include "G4hEnergyLoss.hh"
#include "G4hIonisation.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4GRSVolume.hh"
#include "G4Box.hh"
#include "G4ProcessManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4GPILSelection.hh"

//    It tests the G4hEnergyLoss,G4hIonisation processes -----------
//    created by L.Urban on 06/06/97 --------------------------
//
//    Modifications:
//    23-09-97: geometry adapted for the touchable.
//    22-04-98: adapted for G4VparticleChange, ProcessVectorTypeIndex.  mma
//

G4VPhysicalVolume* BuildVolume(G4Material* matworld)
//  it builds a simple box filled with material matword .......
{
  G4Box *myWorldBox= new G4Box ("WBox",10000.*cm,10000.*cm,10000.*cm);

  G4LogicalVolume *myWorldLog = new G4LogicalVolume(myWorldBox,matworld,
                                                    "WLog",0,0,0) ;

  G4PVPlacement *myWorldPhys = new G4PVPlacement(0,G4ThreeVector(),
                                                 "WPhys",
                                                 myWorldLog,
                                                 0,false,0) ;

  
  return myWorldPhys ;

}
                                              

int main()
{
  //-------- set output format-------
   G4cout.setf( std::ios::scientific, std::ios::floatfield );
  //---write results to the file  hloss -----
   std::ofstream outFile("hloss", std::ios::out ) ;
   outFile.setf( std::ios::scientific, std::ios::floatfield );

  //--------- Material definition ---------
  G4double a, z, ez, density ,temperature,pressure;
  G4State state ;
  G4String name, symbol;
  G4int nel;

  a = 9.012*g/mole;
  density = 1.848*g/cm3;
  G4Material* Be = new G4Material(name="Beryllium", z=4. , a, density);

  a = 26.98*g/mole;
  density = 2.7*g/cm3;
  G4Material* Al = new G4Material(name="Aluminium", z=13., a, density);

  a = 28.09*g/mole;
  density = 2.33*g/cm3;
  G4Material* Si = new G4Material(name="Silicon", z=14., a, density);

  G4Element*   elH = new G4Element ("Hydrogen", "H", 1. ,  1.01*g/mole);

  a = 14.01*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen", symbol="N", ez=7., a);
  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxigen", symbol="O", ez=8., a);
  density = 1.29e-03*g/cm3;
  state = kStateGas ;
  temperature = 273.*kelvin ;
  pressure = 1.*atmosphere ;
  G4Material* Air = new G4Material(name="Air", density, nel=2 ,
                                   state ,temperature , pressure ) ;
  Air->AddElement(elN, .7);
  Air->AddElement(elO, .3);

  G4Material* H2O = new G4Material ("Water" , 1.*g/cm3, 2);
  H2O->AddElement(elH,2);
  H2O->AddElement(elO,1);

  a = 55.85*g/mole;
  density = 7.87*g/cm3;
  G4Material* Fe = new G4Material(name="Iron", z=26., a, density);

  a = 196.97*g/mole;
  density = 19.32*g/cm3;
  G4Material* Au = new G4Material(name="Gold", z=79., a, density);

  a = 207.19*g/mole;
  density = 11.35*g/cm3;
  G4Material* Pb = new G4Material(name="Lead", z=82., a, density);

  a = 0. ;          
  density = 0. ;         
  G4Material* Vac= new G4Material(name="Vacuum",z=0., a, density,kVacuum);

  const G4MaterialTable* theMaterialTable ;
  G4Material* apttoMaterial ;
  G4String MaterialName ; 
  G4Timer theTimer ;
//--------- Particle definition ---------

  G4ParticleDefinition* theGamma = G4Gamma::GammaDefinition();
  G4ParticleDefinition* theElectron = G4Electron::ElectronDefinition();
  G4ParticleDefinition* thePositron = G4Positron::PositronDefinition();
  G4ParticleDefinition* theProton = G4Proton::ProtonDefinition();
  G4ParticleDefinition* theAntiProton = G4AntiProton::AntiProtonDefinition();
  G4ParticleDefinition* thePionPlus = G4PionPlus::PionPlusDefinition();
  G4ParticleDefinition* thePionMinus = G4PionMinus::PionMinusDefinition();
  G4ParticleDefinition* theKaonPlus = G4KaonPlus::KaonPlusDefinition();
  G4ParticleDefinition* theKaonMinus = G4KaonMinus::KaonMinusDefinition();

  G4double* GammaKineticEnergyCuts ;
  G4double* ElectronKineticEnergyCuts ;
  G4double* PositronKineticEnergyCuts ;
  G4double* ParticleKineticEnergyCuts ;

  theMaterialTable = G4Material::GetMaterialTable() ;

  G4double cutinrange,CutInRangeele,CutInRangepos ;

  G4eIonisation theElectronIonisation , thePositronIonisation ;
  G4ProcessManager* theElectronProcessManager = theElectron->GetProcessManager();
  theElectronProcessManager->AddProcess(&theElectronIonisation,-1,0,0) ;
  G4ProcessManager* thePositronProcessManager = thePositron->GetProcessManager();
  thePositronProcessManager->AddProcess(&thePositronIonisation,-1,0,0) ;

  G4ParticleWithCuts* theParticle ;
  G4GPILSelection selection;
    
  G4double energy, momentum, mass;
  G4ProcessVector* palongget ;
  G4ProcessVector* palongdo ;
  G4ProcessVector* ppostget ;
  G4ProcessVector* ppostdo ;

  G4String confirm ;

  G4cout << " Do you want the proton as particle (yes/no)? " << std::flush;
  G4cin >> confirm ;
  if(confirm == "yes")
  {
    mass=theProton->GetPDGMass();
    theParticle = theProton;
  }
  else
  {
     G4cout << " Do you want the antiproton as particle (yes/no)? " << std::flush;
     G4cin >> confirm ;
     if(confirm == "yes")
     {
        mass=theAntiProton->GetPDGMass();
        theParticle = theAntiProton;
     }
     else
     {
      G4cout << " Do you want the pi+ as particle (yes/no)? " << std::flush;
      G4cin >> confirm ;
      if(confirm == "yes")
      {
      mass=thePionPlus->GetPDGMass();
      theParticle = thePionPlus;
      }
      else
      {
        G4cout << " Do you want the pi- as particle (yes/no)? " << std::flush;
        G4cin >> confirm ;
        if(confirm == "yes")
        {
        mass=thePionMinus->GetPDGMass();
        theParticle = thePionMinus;
        } 
        else
        {
          G4cout << " Do you want the K+ as particle (yes/no)? " << std::flush;
          G4cin >> confirm ;
          if(confirm == "yes")
          {
          mass=theKaonPlus->GetPDGMass();
          theParticle = theKaonPlus;
          }
          else
          {
            G4cout << " Do you want the K- as particle (yes/no)? " << std::flush;
            G4cin >> confirm ;
            if(confirm == "yes")
            {
            mass=theKaonMinus->GetPDGMass();
            theParticle = theKaonMinus;
            }
            else
            {
             G4cout << " There is no other particle in the test." << G4endl;
             return EXIT_FAILURE;
            }
          }
        }
       }
      }
     }
     
  
    energy = 1.*GeV + mass ;
    momentum=std::sqrt(energy*energy-mass*mass) ;
  
    G4ParticleMomentum theMomentum(momentum,0.,0.);
  
    G4double pModule = theMomentum.mag();

    G4DynamicParticle aParticle(theParticle,energy,theMomentum);

    aParticle.SetKineticEnergy(energy-mass);


  G4hIonisation theParticleIonisation  ;
  G4ProcessManager* theParticleProcessManager = theParticle->GetProcessManager();
  theParticleProcessManager->AddProcess(&theParticleIonisation,-1,0,0) ;

  G4ForceCondition cond ;
  G4ForceCondition* condition = &cond ;

  G4double currentSafety ;
  G4double& refsafety=currentSafety;


  G4cout << "cut for GAMMA in mm =" ;
  G4cin >> cutinrange ; cutinrange *= mm; 
  theGamma->SetCuts(cutinrange) ;
    G4cout << "gamma,cut in range(mm)=" << theGamma->GetCuts()/mm << G4endl ;
    outFile << "  ---------------------------------------" << G4endl ;
    outFile << "  gamma,cut in range(mm)=" << theGamma->GetCuts()/mm << G4endl ;

  GammaKineticEnergyCuts = theGamma->GetCutsInEnergy() ;
  for (G4int icut=0; icut<theMaterialTable->length(); icut++)
  {
    G4cout << "material index=" << icut << "  kin.energy cut(MeV)=" << 
         GammaKineticEnergyCuts[icut]/MeV << G4endl ;
    outFile << "  material index=" << icut << "  kin.energy cut(MeV)=" << 
         GammaKineticEnergyCuts[icut]/MeV << G4endl ;
  }


  G4cout << "cut for ELECTRON in mm =" ;
  G4cin >> cutinrange ; cutinrange *= mm; 
  CutInRangeele = cutinrange ;
  theElectron->SetCuts(cutinrange) ;
    G4cout << "electron,cut in range(mm)=" << theElectron->GetCuts()/mm << G4endl ;
    outFile << "  ---------------------------------------" << G4endl ;
    outFile << "  electron,cut in range(mm)=" << theElectron->GetCuts()/mm << G4endl ;

  ElectronKineticEnergyCuts = theElectron->GetCutsInEnergy() ;
  for ( icut=0; icut<theMaterialTable->length(); icut++)
  {
    G4cout << "material index=" << icut << "  kin.energy cut(MeV)=" << 
         ElectronKineticEnergyCuts[icut]/MeV << G4endl ;
    outFile << "  material index=" << icut << "  kin.energy cut(MeV)=" << 
         ElectronKineticEnergyCuts[icut]/MeV << G4endl ;
  }

  G4cout << "cut for POSITRON in mm =" ;
  G4cin >> cutinrange ; cutinrange *= mm; 
  CutInRangepos = cutinrange ;
  thePositron->SetCuts(cutinrange) ;
    G4cout << "positron,cut in range(mm)=" << thePositron->GetCuts()/mm << G4endl ;
    outFile << "  ---------------------------------------" << G4endl ;
    outFile << "  positron,cut in range(mm)=" << thePositron->GetCuts()/mm << G4endl ;

  PositronKineticEnergyCuts = thePositron->GetCutsInEnergy() ;
  for ( icut=0; icut<theMaterialTable->length(); icut++)
  {
    G4cout << "material index=" << icut << "  kin.energy cut(MeV)=" << 
         PositronKineticEnergyCuts[icut]/MeV << G4endl ;
    outFile << "  material index=" << icut << "  kin.energy cut(MeV)=" << 
         PositronKineticEnergyCuts[icut]/MeV << G4endl ;
  }

  G4cout << "cut for hadrons in mm =" ;
  G4cin >> cutinrange ; cutinrange *= mm; 
  theParticle->SetCuts(cutinrange) ;
  G4cout << "after particle setcuts " << G4endl;


    G4cout << "cut in range(mm)=" << theParticle->GetLengthCuts()/mm << G4endl ;
    outFile << "  ---------------------------------------" << G4endl ;
    outFile << "  cut in range(mm)=" << theParticle->GetLengthCuts()/mm << G4endl ;

  ParticleKineticEnergyCuts = theParticle->GetEnergyCuts() ;

  for ( icut=0; icut<theMaterialTable->length(); icut++)
  {
    G4cout << "material index=" << icut << "  kin.energy cut(MeV)=" << 
         ParticleKineticEnergyCuts[icut]/MeV << G4endl ;
    outFile << "  material index=" << icut << "  kin.energy cut(MeV)=" << 
         ParticleKineticEnergyCuts[icut]/MeV << G4endl ;
  }

    G4cout << "  ------         ----- " << G4endl ;
    outFile << "  " << G4endl;
    outFile << " ionisation test  **************************************" << G4endl ;
    outFile << "  " << G4endl;
    outFile << "   particle = " << 
             aParticle.GetDefinition()->GetParticleName() << G4endl ;
    outFile << G4endl;
    
    palongget = aParticle.GetDefinition()->GetProcessManager()
                                 ->GetAlongStepProcessVector(typeGPIL);
    ppostget = aParticle.GetDefinition()->GetProcessManager()
                                 ->GetPostStepProcessVector(typeGPIL);
    palongdo = aParticle.GetDefinition()->GetProcessManager()
                                 ->GetAlongStepProcessVector(typeDoIt);
    ppostdo = aParticle.GetDefinition()->GetProcessManager()
                                 ->GetPostStepProcessVector(typeDoIt);

//---------------------------------- Physics --------------------------------

  G4int itry=1, Ntry=1, Nstart, ir;
  G4double r ;

//**************************************************************************
  const G4int Nbin=113 ;
  G4double TkinMeV[Nbin]  =
              {0.00001,0.000015,0.00002,0.00003,0.00004,0.00005,0.00006,0.00008,
               0.0001,0.00015,0.0002,0.0003,0.0004,0.0005,0.0006,0.0008,
               0.001,0.0015,0.002,0.003,0.004,0.005,0.006,0.008,
               0.01,0.015,0.02,0.03,0.04,0.05,0.06,0.08,
               0.1,0.15,0.2,0.3,0.4,0.5,0.6,0.8,
               1.,1.5,2.,3.,4.,5.,6.,8.,
               10.,15.,20.,30.,40.,50.,60.,80.,
               100.,150.,200.,300.,400.,500.,600.,800.,
               1.0e3,1.5e3,2.0e3,3.0e3,4.0e3,5.0e3,6.0e3,8.0e3,
               1.0e4,1.5e4,2.0e4,3.0e4,4.0e4,5.0e4,6.0e4,8.0e4,
               1.0e5,1.5e5,2.0e5,3.0e5,4.0e5,5.0e5,6.0e5,8.0e5,
               1.0e6,1.5e6,2.0e6,3.0e6,4.0e6,5.0e6,6.0e6,8.0e6,
               1.0e7,1.5e7,2.0e7,3.0e7,4.0e7,5.0e7,6.0e7,8.0e7,
               1.0e8,1.5e8,2.0e8,3.0e8,4.0e8,5.0e8,6.0e8,8.0e8,
               1.0e9} ;
               for (G4int k=0; k<Nbin; k++) TkinMeV[k] *= MeV; 
         
    G4int J=-1 ;

    G4double lambda,trueStep,geomStep,stepLimit,
             previousStepSize,currentMinimumStep ;
    G4ParticleChange* aParticleChange ;
    
    G4double T,dEdx,range ;

    NEXTMATERIAL: ;
    J = J+1 ;
    if ( J >= theMaterialTable->length() )
      { G4cout << "that was the last material in the table --> STOP" << G4endl;
        return EXIT_FAILURE ; }  

    apttoMaterial = (*theMaterialTable)[ J ] ;
    MaterialName = apttoMaterial->GetName() ; 
    G4cout << "material=" << MaterialName << G4endl ;
    G4cout << "Do you want the Energyloss test 1. for this material?" << G4endl ;
    G4cout << "type a positive number if the answer is YES" << G4endl ;
    G4cout << "type a negative number if the answer is NO " << G4endl ;
    G4int icont ;
    G4cin >> icont ;
    if ( icont < 0 )
        goto NEXTMATERIAL ;

//---------- Volume definition ---------------------

    G4VPhysicalVolume* myVolume ;

    myVolume = BuildVolume(apttoMaterial) ;

//--------- track and Step definition (for this test ONLY!)------------
    G4ThreeVector aPosition(0.,0.,0.);
    const G4ThreeVector aDirection(0.,0.,1.) ;
    G4double aTime = 0. ;

    G4Track* tracke = new G4Track(&aParticle,aTime,aPosition) ;
    G4Track& trackele = (*tracke) ;
    //(*tracke).SetVolume(myVolume) ;
    G4GRSVolume* touche = new G4GRSVolume(myVolume, NULL, aPosition);   
    (*tracke).SetTouchable(touche);        
    (*tracke).SetMomentumDirection(aDirection) ;


    G4Step* Step = new G4Step() ;
    G4Step& Step = (*Step) ;
    tracke->SetStep(Step);

    G4StepPoint* aPoint = new G4StepPoint();
    (*aPoint).SetPosition(aPosition) ;
    G4double safety = 10000.*cm ;
    (*aPoint).SetSafety(safety) ;

    (*Step).SetPostStepPoint(aPoint) ;
   
//**************************************************************************

    G4cout <<  G4endl;
    G4cout <<"  " << MaterialName  << "  Energyloss test 1." << G4endl ;
    G4cout << " ++++++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    G4cout << G4endl ;
    G4cout << "kin.en.(MeV)    dE/dx(MeV/mm)    range(mm)    Step(mm)" << G4endl ;
    G4cout << G4endl ;
 
    outFile <<  G4endl;
    outFile <<"  " << MaterialName  << "  Energyloss test 1." << G4endl ;
    outFile << " +++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    outFile << G4endl ;
    outFile << "kin.en.(MeV)    dE/dx(MeV/mm)    range(mm)    Step(mm)" << G4endl ;
    outFile << G4endl ;
 

    for ( G4int i=0 ; i<Nbin ; i++)
    {
      trueStep = cutinrange ; 
      previousStepSize = cutinrange ;
      currentMinimumStep = trueStep ;
      (*tracke).SetKineticEnergy(TkinMeV[i]) ;
      stepLimit = (*palongget)(0)->AlongStepGetPhysicalInteractionLength( 
                                                         trackele,            
                                                         previousStepSize,
                                                         currentMinimumStep,
                                                         refsafety,
                                                         &selection) ;
 
      dEdx = theParticleIonisation.GetdEdx() ;
      range = theParticleIonisation.GetRangeNow() ;
      T = TkinMeV[i] ;

       G4cout <<" " <<  T/MeV << "  " << dEdx/(MeV/mm) << "  " ;
       G4cout << range/mm << "  " << stepLimit/mm << G4endl ; 

       outFile <<" " <<  T/MeV << "  " << dEdx/(MeV/mm) << "  " ;
       outFile << range/mm << "  " << stepLimit/mm << G4endl ; 

    }

    G4cout <<  G4endl;
    outFile << G4endl;

    ENERGYLOSS2: ;

    G4cout << "material=" << MaterialName << G4endl ;
    G4cout << "Do you want the Energyloss test 2. for this material?" << G4endl ;
    G4cout << "type a positive number if the answer is YES" << G4endl ;
    G4cout << "type a negative number if the answer is NO " << G4endl ;
    G4cin >> icont ;
    if ( icont < 0 )
        goto ENERGYLOSS3 ;

    G4double TMeV,stepmm,stepmx,meanloss,lossnow ;
  

    G4cout << "give an energy value in MeV " ;
    G4cin >> TMeV ; TMeV *= MeV;

      trueStep = cutinrange ; 
      previousStepSize = cutinrange ;
      currentMinimumStep = trueStep ;
      (*tracke).SetKineticEnergy(TMeV) ;
       stepmx = (*palongget)(0)->AlongStepGetPhysicalInteractionLength(
                                              trackele,
                                              previousStepSize,
                                              currentMinimumStep,
                                              refsafety,
                                              &selection);

    G4cout << " give a steplength in mm , the max. meaningful Step is " << stepmx/mm << " mm" <<G4endl;
    G4cout << "Step:" ;
    G4cin >> stepmm ; stepmm *= mm;
 
   (*Step).SetTrack(tracke) ;
   (*Step).SetStepLength(stepmm);

 
      aParticleChange = (G4ParticleChange*)
                        ((*palongdo)(0)->AlongStepDoIt(trackele,Step));
      meanloss = theParticleIonisation.GetMeanLoss() ;
      lossnow = TMeV-(*aParticleChange).GetEnergyChange();

    G4cout <<  G4endl;
    G4cout <<"  " << MaterialName  << "  Energyloss test 2." << G4endl ;
    G4cout << " ++++++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    G4cout << G4endl ;
    G4cout << "kin.en.(MeV)    Step(mm)   meanloss(MeV)  act.loss(MeV)" << G4endl ;
    G4cout << TMeV/MeV << "   " << stepmm/mm << "  " << meanloss/MeV << "  " << lossnow/MeV << G4endl ;
    G4cout << " status change:" << (*aParticleChange).GetStatusChange() << G4endl ;
    G4cout << G4endl ;
 
    outFile <<  G4endl;
    outFile <<"  " << MaterialName  << "  Energyloss test 2." << G4endl ;
    outFile << " +++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    outFile << G4endl ;
    outFile << "kin.en.(MeV)    Step(mm)   meanloss(MeV)  act.loss(MeV)" << G4endl ;
    outFile << TMeV/MeV << "   " << stepmm/mm << "  " << meanloss/MeV << "  " << lossnow/MeV << G4endl ;
    outFile << " status change:" << (*aParticleChange).GetStatusChange() << G4endl ;
    outFile << G4endl ;
 
    goto ENERGYLOSS2 ;

    ENERGYLOSS3: ;

    G4cout << "material=" << MaterialName << G4endl ;
    G4cout << "Do you want the Energyloss test 3. for this material?" << G4endl ;
    G4cout << "type a positive number if the answer is YES" << G4endl ;
    G4cout << "type a negative number if the answer is NO " << G4endl ;
    G4cin >> icont ;
    if ( icont < 0 )
        goto DELTARAY1 ;

    G4cout << "give an energy value in MeV " ;
    G4cin >> TMeV ; TMeV *= MeV;

      trueStep = cutinrange ; 
      previousStepSize = cutinrange ;
      currentMinimumStep = trueStep ;
      (*tracke).SetKineticEnergy(TMeV) ;
       stepmx = (*palongget)(0)->AlongStepGetPhysicalInteractionLength(
                                              trackele,
                                              previousStepSize,
                                              currentMinimumStep,
                                              refsafety,
                                              &selection);

    G4cout << " give a steplength in mm , the max. meaningful Step is " << stepmx/mm << " mm" <<G4endl;
    G4cout << "Step:" ;
    G4cin >> stepmm ; stepmm *= mm;
 
   (*Step).SetTrack(tracke) ;
   (*Step).SetStepLength(stepmm);


    G4cout << " give number of events you want " ;
    G4int nbev,ibev ;
    G4cin >> nbev ;

    meanloss=0.;
    theTimer.Start();

    for ( ibev=0; ibev<nbev; ibev++)
    { 
      aParticleChange = (G4ParticleChange*)
                        ((*palongdo)(0)->AlongStepDoIt(trackele,Step));
      lossnow = TMeV-(*aParticleChange).GetEnergyChange();

    meanloss += lossnow ;
   }

    theTimer.Stop();
    meanloss /= nbev ;
    G4cout <<  G4endl;
    G4cout <<"  " << MaterialName  << "  Energyloss test 3." << G4endl ;
    G4cout << " ++++++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    G4cout << G4endl ;
    G4cout << "kin.en.(MeV)    Step(mm)   meanloss(MeV) time/event(sec) " << G4endl ;
    G4cout << TMeV/MeV << "   " << stepmm/mm << "  " << meanloss/MeV << "  " << 
    theTimer.GetUserElapsed()/nbev << G4endl ;
    G4cout << G4endl ;
 
    outFile <<  G4endl;
    outFile <<"  " << MaterialName  << "  Energyloss test 3." << G4endl ;
    outFile << " +++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    outFile << G4endl ;
    outFile << "kin.en.(MeV)    Step(mm)   meanloss(MeV) time/event(sec) " << G4endl ;
    outFile << TMeV/MeV << "   " << stepmm/mm << "  " << meanloss/MeV << "  " << 
    theTimer.GetUserElapsed()/nbev << G4endl ;
    outFile << G4endl ;
 
    goto ENERGYLOSS3 ;

    DELTARAY1: ;
    G4cout << "material=" << MaterialName << G4endl ;
    G4cout << "Do you want the delta ray test 1. for this material?" << G4endl ;
    G4cout << "type a positive number if the answer is YES" << G4endl ;
    G4cout << "type a negative number if the answer is NO " << G4endl ;
    G4cin >> icont ;
    if ( icont < 0 )
        goto DELTARAY2 ;


    G4cout <<  G4endl;
    G4cout <<"  " << MaterialName  << "  delta ray test 1." << G4endl ;
    G4cout << " ++++++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    G4cout << G4endl ;
    G4cout << "kin.en.(MeV)      mean free path(mm)" << G4endl ;
    G4cout << G4endl ;
 
    outFile <<  G4endl;
    outFile <<"  " << MaterialName  << "  delta ray test 1." << G4endl ;
    outFile << " +++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    outFile << G4endl ;
    outFile << "kin.en.(MeV)      mean free path(mm)" << G4endl ;
    outFile << G4endl ;

    for ( i=0 ; i<Nbin ; i++)
    {

      previousStepSize = cutinrange ;
      (*tracke).SetKineticEnergy(TkinMeV[i]) ;
      stepLimit = theParticleIonisation.GetMeanFreePath(                                           
                                                         trackele,            
                                                         previousStepSize,
                                                         condition) ;                                           

      T = TkinMeV[i] ;

      G4cout <<" " <<  T/MeV << "      " <<  stepLimit/mm << G4endl ;

      outFile <<" " <<  T/MeV << "      " << stepLimit/mm << G4endl ;
 
    }

    G4cout <<  G4endl;
    outFile << G4endl;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    DELTARAY2: ;

    G4cout << "material=" << MaterialName << G4endl ;
    G4cout << "Do you want the deltaray test 2. for this material?" << G4endl ;
    G4cout << "type a positive number if the answer is YES" << G4endl ;
    G4cout << "type a negative number if the answer is NO " << G4endl ;
    G4cin >> icont ;

    G4double newenergy,dx,dy,dz,Tdelta,ddx,ddy,ddz ;
    G4int nd ;
    const G4ThreeVector* momdir ;
    G4ParticleMomentum ddir ;

    if ( icont < 0 )
        goto BREMS1 ;  

    G4cout << "give an energy value in MeV " ;
    G4cin >> TMeV ; TMeV *= MeV;
                           
     stepmm = 1.*mm ;

   (*Step).SetTrack(tracke) ;
   (*Step).SetStepLength(stepmm);


      (*tracke).SetKineticEnergy(TMeV) ;
      aParticleChange = (G4ParticleChange*)
                        ((*ppostdo)(0)->PostStepDoIt(trackele,Step));

     newenergy=(*aParticleChange).GetEnergyChange() ;
     momdir=(*aParticleChange).GetMomentumChange();
     dx = (*momdir).x();
     dy = (*momdir).y();
     dz = (*momdir).z();
     nd=aParticleChange->GetNumberOfSecondaries();
 
    if(nd>0)
    {
     Tdelta=aParticleChange->GetSecondary(0)->GetKineticEnergy();
     ddir=aParticleChange->GetSecondary(0)->
                              GetMomentumDirection();
     ddx = (ddir).x();
     ddy = (ddir).y();
     ddz = (ddir).z();
    }

    G4cout <<  G4endl;
    G4cout <<"  " << MaterialName  << "  delta ray test 2." << G4endl ;
    G4cout << " ++++++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    G4cout << G4endl ;
    G4cout << "T=" << TMeV/MeV << "   newT=" << newenergy/MeV << "  (MeV)" << G4endl ;
    G4cout << " status change:" << (*aParticleChange).GetStatusChange() << G4endl ;
    if(nd>0)
    G4cout << "Tdelta=" << Tdelta/MeV << G4endl ;
    G4cout << "new direction:" << dx << "  " << dy << "  " << dz << G4endl;
    if(nd>0)
    G4cout << "delta direction:" << ddx << "  " << ddy << "  " << ddz << G4endl ;
    G4cout << G4endl ;
 
    outFile <<  G4endl;
    outFile <<"  " << MaterialName  << "  delta ray test 2." << G4endl ;
    outFile << " +++++++++++++++++++++++++++++++++++++++++" << G4endl ;
    outFile << G4endl ;
    outFile << "T=" << TMeV/MeV << "   newT=" << newenergy/MeV << "   (MeV)" << G4endl;
    outFile << " status change:" << (*aParticleChange).GetStatusChange() << G4endl ;
    if(nd>0)
    outFile << "Tdelta=" << Tdelta/MeV << G4endl ;
    outFile << "new direction:" << dx << "  " << dy << "  " << dz << G4endl;
    if(nd>0)
    outFile << "delta direction:" << ddx << "  " << ddy << "  " << ddz << G4endl ;
    outFile << G4endl ;

    (*aParticleChange).Clear();

    goto DELTARAY2 ;

    BREMS1: ;

    if( J < theMaterialTable->length()-1 )
       goto NEXTMATERIAL ;
 
  return EXIT_SUCCESS;
}

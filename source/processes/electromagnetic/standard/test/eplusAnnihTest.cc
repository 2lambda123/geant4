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
// $Id: eplusAnnihTest.cc,v 1.3 2001-07-11 10:03:33 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------------
#include "G4ios.hh"
#include "g4std/fstream"
#include "g4std/iomanip"

#include "g4templates.hh"

#include "G4Material.hh"

#include "G4ProcessManager.hh"
#include "G4eplusAnnihilation.hh"

#include "G4DynamicParticle.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4GRSVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"

#include "G4Step.hh"

//    it tests the G4eplusAnnihilation  ----------------
//    ---------- M.Maire on 19/03/97 -------------------
//
//  Modifs: 22-09-97

int main()
{
  //-------- set output format-------
   G4cout.setf( G4std::ios::scientific, G4std::ios::floatfield );
  //-------- write results onto a file --------
   G4std::ofstream outFile( "Annihilation.out", G4std::ios::out);
   outFile.setf( G4std::ios::scientific, G4std::ios::floatfield );

  //
  //--------- Materials definition ---------
  //
  G4Material* Al = new G4Material("Aluminium", 13.,  26.98*g/mole,  2.7 *g/cm3 );
  G4Material* Fe = new G4Material("Iron",      26.,  55.85*g/mole,  7.87*g/cm3 );
  G4Material* Pb = new G4Material("Lead",      82., 207.19*g/mole, 11.35*g/cm3 );

  G4Element*   H = new G4Element ("Hydrogen", "H", 1. ,  1.01*g/mole);
  G4Element*   O = new G4Element ("Oxygen"  , "O", 8. , 16.00*g/mole);
  G4Material* wa = new G4Material ("Water" , 1.*g/cm3, 2);
  wa->AddElement(H,2);
  wa->AddElement(O,1);

  G4Element::DumpInfo(); 
  G4Material::DumpInfo();

  static const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // fix a material
  G4int imat = 0;

  // Geometry definitions
  //
    G4Box* theFrame = new G4Box ("Frame",10*m, 10*m, 10*m);

    G4LogicalVolume* LogicalFrame = new G4LogicalVolume(theFrame,
                                                  (*theMaterialTable)(imat),
						  "LFrame", 0, 0, 0);

    G4PVPlacement* PhysicalFrame = new G4PVPlacement(0,G4ThreeVector(),
                                          "PFrame",LogicalFrame,0,false,0);

  //--------- Particle definition ---------
  //
  G4ParticleDefinition* Gamma = G4Gamma::GammaDefinition();
  G4ParticleDefinition* Electron = G4Electron::ElectronDefinition();
  G4ParticleDefinition* Positron = G4Positron::PositronDefinition();

  //--------- Processes definition ---------
  //
  G4ProcessManager* thePositronProcessManager = Positron->GetProcessManager();

  G4eplusAnnihilation theAnnihilation;
  thePositronProcessManager->AddProcess(&theAnnihilation,0,-1,0);

  G4ForceCondition* condition;

  // ------- set cut and Build CrossSection Tables -------
  //
  Gamma->SetCuts(1.*mm);
  Electron->SetCuts(1.*mm);
  Positron->SetCuts(1.*mm);

  // -------- create 1 Dynamic Particle ----

  G4double positronenergy = 10.*MeV;
  G4ParticleMomentum positrondirection(0.,0.,1.);
  G4DynamicParticle aPositron(G4Positron::Positron(),positrondirection,positronenergy);
  

  //--------- track definition (for this test ONLY!)------------
  //
    G4ThreeVector aPosition(0.,0.,0.);
    G4double aTime = 0. ;

    G4Track* ptrack = new G4Track(&aPositron,aTime,aPosition) ;
    G4Track& aTrack = (*ptrack) ;
    //ptrack->SetVolume(PhysicalFrame);
    G4GRSVolume* touche = new G4GRSVolume(PhysicalFrame, NULL, aPosition);   
    ptrack->SetTouchable(touche);

  // -------- create 1 Step (for this test only)----  

    G4Step* Step = new G4Step();  G4Step& aStep = (*Step);
    Step->SetTrack(ptrack);
    ptrack->SetStep(Step);
  
  // ---------- Print the tables

  G4Material* apttoMaterial ;
  G4String MaterialName ;
  G4double meanFreePath ;
  G4int nkin = 15; 
  G4double Tkin[15];
  Tkin[ 0] =  10.*keV; Tkin[ 1] = 50.*keV; Tkin[ 2] =100.*keV; Tkin[ 3] = 200.*keV; 
  Tkin[ 4] = 500.*keV; Tkin[ 5] =  1.*MeV; Tkin[ 6] =  5.*MeV; Tkin[ 7] =  10.*MeV;
  Tkin[ 8] = 100.*MeV; Tkin[ 9] =500.*MeV; Tkin[10] =  1.*GeV; Tkin[11] =  10.*GeV;
  Tkin[12] = 100.*GeV; Tkin[13] =500.*GeV; Tkin[14] =  1.*TeV;

  for ( G4int J = 0 ; J < theMaterialTable->length() ; J++ )
  {
    apttoMaterial = (*theMaterialTable)[ J ] ;
    MaterialName  = apttoMaterial->GetName() ; 

    LogicalFrame->SetMaterial(apttoMaterial); 

    outFile << " " << G4endl;   // Print table of mean free path
    outFile <<"  " << MaterialName  << "    Annihilation Mean Free Path in cm" << G4endl;
    outFile << "----------------------------------------------------" << G4endl;
    outFile << " " << G4endl;
    outFile << "kinetic energy (MeV)     mean free path (cm)" << G4endl ;
    outFile << " " << G4endl;

    for ( G4int i=0 ; i<nkin ; i++)
      {
       aPositron.SetKineticEnergy(Tkin[i]);

       meanFreePath = theAnnihilation.GetMeanFreePath(aTrack, 1., condition);


       outFile <<"  " <<  Tkin[i]/MeV << "            " << meanFreePath/cm << G4endl ;
      }
    outFile << " " << G4endl;
   }
  outFile << " -----------------------------------------------------------" << G4endl;

  //
  // --------- Test the PostStepDoIt for the positron annihilation
  //
  outFile << " -----------------------------------------------------------" << G4endl;
  outFile << " -------------------- PostStepDoIt -------------------------" << G4endl;
  outFile << " -----------------------------------------------------------" << G4endl;

    apttoMaterial = (*theMaterialTable)(imat);
    LogicalFrame->SetMaterial(apttoMaterial); 
    G4double Energy = 10.*MeV;
    aPositron.SetKineticEnergy(Energy);
    aPositron.SetMomentumDirection(0., 0., 1.);
    G4VParticleChange* adummy;
    G4ParticleChange* aFinalParticle;
    G4Track* aCreatedParticle;
 
    G4int iteration = 0;   
    while (iteration < 10)
       {
        adummy         = theAnnihilation.PostStepDoIt(aTrack, aStep);
        aFinalParticle = (G4ParticleChange*)adummy;

        // check the kinematic
        //
        G4double Tkin0 = aPositron.GetKineticEnergy();
        G4double Px0   = aPositron.GetMomentum().x() ,
                 Py0   = aPositron.GetMomentum().y() ,
                 Pz0   = aPositron.GetMomentum().z() ;
        outFile << " Initial Positron : Tkin= " << Tkin0/MeV << "   Px= " << Px0/MeV 
                << "   Py= " << Py0/MeV << "   Pz= " << Pz0/MeV << G4endl << G4endl ;

        G4String  Name;
        G4double  Px2 , Py2 , Pz2 , Etot2 ;
        G4double sEtot = Tkin0 + 2*electron_mass_c2 , sPx = Px0 , sPy = Py0 , sPz = Pz0 ;

        G4int NumberOfSecondary = aFinalParticle->GetNumberOfSecondaries();

        for (G4int i=0; i < NumberOfSecondary; i++) {
           aCreatedParticle = aFinalParticle->GetSecondary(i) ;
           Name  =  aCreatedParticle->GetDefinition()->GetParticleName();
           Etot2 =  aCreatedParticle->GetTotalEnergy() ;
           Px2   = (aCreatedParticle->GetMomentum()).x() ;
           Py2   = (aCreatedParticle->GetMomentum()).y() ;
           Pz2   = (aCreatedParticle->GetMomentum()).z() ;
           outFile << " No: " << i << " (" << Name << ")     : Etot= " << Etot2/MeV   
                   << "   Px= " << Px2/MeV << "   Py= " << Py2/MeV << "   Pz= " << Pz2/MeV
                   << G4endl;

          // NOTE - Secondaries are normally deleted by the track destructor !
          delete aFinalParticle->GetSecondary(i);

          // Balance
             sEtot -= Etot2;  sPx -= Px2;  sPy -= Py2;  sPz -= Pz2;
        }

        G4double Edep  =  aFinalParticle->GetLocalEnergyDeposit();
        outFile << " Local Energ deposit= " << Edep/MeV << G4endl ;
        sEtot -= Edep;

        outFile << G4endl;
        outFile << " Balance        : Ener= " << sEtot/MeV
                << "   Px= " << sPx/MeV << "   Py= " << sPy/MeV 
                << "   Pz= " << sPz/MeV << G4endl; 
        outFile << " -----------------------------------------------------------" << G4endl;

        // 'Build' a new initial positron
        //
        Energy = Etot2;
        G4double P = sqrt(Px2*Px2 + Py2*Py2 + Pz2*Pz2);

        aPositron.SetKineticEnergy(Energy);
        aPositron.SetMomentumDirection(Px2/P, Py2/P, Pz2/P);

        iteration++; 

   } ;


  //
  // --------- Test the AtRestDoIt for the Positron Annihilation
  //
  outFile << " -----------------------------------------------------------" << G4endl;
  outFile << " ---------------------- AtRestDoIt -------------------------" << G4endl;
  outFile << " -----------------------------------------------------------" << G4endl;


    Energy = 0.*GeV;
    aPositron.SetKineticEnergy(Energy);
    aPositron.SetMomentumDirection(0., 0., 0.);
 
    iteration = 0;   
    while (iteration < 10)
       {
        adummy         = theAnnihilation.AtRestDoIt(aTrack, aStep);
        aFinalParticle = (G4ParticleChange*)adummy;
        
        // check the kinematic
        //
        G4double Tkin0 = aPositron.GetKineticEnergy();
        G4double Px0   = aPositron.GetMomentum().x() ,
                 Py0   = aPositron.GetMomentum().y() ,
                 Pz0   = aPositron.GetMomentum().z() ;
        outFile << " Initial Positron : Tkin= " << Tkin0/MeV 
                << "   Px= " << Px0/MeV << "   Py= " << Py0/MeV  << "   Pz= " << Pz0/MeV 
                << G4endl << G4endl ;


        G4String  Name;
        G4double  Px2 , Py2 , Pz2 , Etot2 ;
        G4double sEtot = Tkin0 + 2*electron_mass_c2 , sPx = Px0 , sPy = Py0 , sPz = Pz0 ;

        G4int NumberOfSecondary = aFinalParticle->GetNumberOfSecondaries();

        for (G4int i=0; i < NumberOfSecondary; i++) {
           aCreatedParticle = aFinalParticle->GetSecondary(i) ;
           Name  =  aCreatedParticle->GetDefinition()->GetParticleName();
           Etot2 =  aCreatedParticle->GetTotalEnergy() ;
           Px2   = (aCreatedParticle->GetMomentum()).x() ;
           Py2   = (aCreatedParticle->GetMomentum()).y() ;
           Pz2   = (aCreatedParticle->GetMomentum()).z() ;
           outFile << " No: " << i << " (" << Name << ")     : Etot= " << Etot2/MeV   
                << "   Px= " << Px2/MeV << "   Py= " << Py2/MeV << "   Pz= " << Pz2/MeV 
                << G4endl;

          // NOTE - Secondaries are normally deleted by the track destructor !
          delete aFinalParticle->GetSecondary(i);

          // Balance
             sEtot -= Etot2;  sPx -= Px2;  sPy -= Py2;  sPz -= Pz2;
        }

        G4double Edep  =  aFinalParticle->GetLocalEnergyDeposit();
        outFile << " Local Energ deposit= " << Edep/MeV << G4endl ;
        sEtot -= Edep;

        outFile << G4endl;
        outFile << " Balance        : Ener= " << sEtot
                << "   Px= " << sPx/MeV << "   Py= " << sPy/MeV
                << "   Pz= " << sPz/MeV << G4endl; 
        outFile << " -----------------------------------------------------------" << G4endl;

        iteration++; 

   } ;

  return EXIT_SUCCESS;
}

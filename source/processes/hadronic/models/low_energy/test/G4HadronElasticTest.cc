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
// $Id: G4HadronElasticTest.cc,v 1.3 2001-07-11 10:06:53 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Test of Low-Energy Elastic Scattering class
//
// F.W. Jones, TRIUMF, 04-JUN-96
// Modified for new ParticleDefinition H.kurashige 04-JUL-96
// Modified 09-JUL-96 by FWJ: further changes for new ParticleDefinition.
//
// 27-MAR-97 F.W.Jones: first version for Alpha release
//


#include "G4ios.hh"
#include <stdio.h>
#include "G4Material.hh"
#include "G4ProcessManager.hh"
#include "G4HadronElasticProcess.hh"
#include "G4LElastic.hh"
#include "G4DynamicParticle.hh"
#include "Randomize.hh"
#include "G4PionPlus.hh"
#include "G4Proton.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"

#ifdef G4_SOLVE_TEMPLATES
#include "g4templates.hh"
#endif

int main()
{
// Material definition

// Note: due to a bug in the constructor, the state argument is required
   G4Material Copper("copper", 29., 63.54*g/mole, 8.96*g/cm3, kStateSolid);

//   G4cout << "Dumping material table info:" << G4endl;
//   Copper.DumpInfo();

// Particle definition

   G4Proton* proton = G4Proton::ProtonDefinition();
// proton->DumpTable();
   G4ProcessManager* theProtonProcessManager = proton->GetProcessManager();

   G4PionPlus* pionplus = G4PionPlus::PionPlusDefinition();
// pionplus->DumpTable();
   G4ProcessManager* thePionPlusProcessManager = pionplus->GetProcessManager();

// Process definition

   G4HadronElasticProcess theLElasticProcess;
   G4LElastic theLElastic;
   theLElasticProcess.RegisterMe(&theLElastic);

   //   theLElasticProcess.SetVerboseLevel(2);
   //   theLElasticProcess.setCrossSectionDataVerboseLevel(2);

   theProtonProcessManager->AddDiscreteProcess(&theLElasticProcess);
   theLElasticProcess.BuildPhysicsTable(*proton);
   theLElasticProcess.DumpPhysicsTable(*proton);
   thePionPlusProcessManager->AddDiscreteProcess(&theLElasticProcess);
   theLElasticProcess.BuildPhysicsTable(*pionplus);
   theLElasticProcess.DumpPhysicsTable(*pionplus);

// G4MuEnergyLoss theEnergyLossProcess;
// theProcessManager->AddContinuousProcess(&theEnergyLossProcess);

// Dynamic Particle Definition
   G4DynamicParticle aProton(proton, G4ParticleMomentum(1.,0.,0.), 
                             1.*GeV);
   G4DynamicParticle aPionPlus(pionplus, G4ParticleMomentum(1.,0.,0.), 
                             1.*GeV);
// Physics

   G4double T;
   G4cout << "Enter proton kinetic energy in GeV: ";
   G4cin >> T;
   aProton.SetKineticEnergy(T*GeV);
   aPionPlus.SetKineticEnergy(T*GeV);
   G4double p = aProton.GetTotalMomentum()/GeV;
   G4cout << "proton momentum=" << p << G4endl;
   G4cout << "pionplus momentum=" << aPionPlus.GetTotalMomentum()/GeV << G4endl;
   G4double px, py, pz;
   G4cout << "Enter momentum direction px/p, py/p, pz/p: ";
   G4cin >> px >> py >> pz;
   aProton.SetMomentumDirection(G4ParticleMomentum(px, py, pz));
   aProton.DumpInfo();
   aPionPlus.SetMomentumDirection(G4ParticleMomentum(px, py, pz));
   aPionPlus.DumpInfo();

   G4double meanFreePath;
   meanFreePath = theLElasticProcess.GetMeanFreePathBasic(&aProton, &Copper);
   G4cout << G4endl << "Proton mean free path = " << meanFreePath << 
           " mm" << G4endl;
   meanFreePath = theLElasticProcess.GetMeanFreePathBasic(&aPionPlus, &Copper);
   G4cout << G4endl << "PionPlus mean free path = " << meanFreePath <<
           " mm" << G4endl;

   G4int nevent;
   G4cout << G4endl << "Enter number of events: ";
   G4cin >> nevent;

   G4Step aStep;

   G4double truePathLength = 1.;
   //   G4VParticleChange* pch;
   G4ParticleChange* pch;
   G4double pxnew, pynew, pznew;
   G4double px1, py1, pz1, px2, py2, pz2;
   px1 = py1 = pz1 = px2 = py2 = pz2 = 0.;

   if (nevent > 10) G4cout << "Printing first 10 events only" << G4endl;
   G4cout << "event#     Px     Py     Pz" << G4endl;
   
   G4Box* theFrame = new G4Box ("Frame",10*m, 10*m, 10*m);
   G4LogicalVolume* LogicalFrame = new G4LogicalVolume(theFrame,
                                                       &Copper,
                                                      "LFrame", 0, 0, 0);
   G4PVPlacement* PhysicalFrame = new G4PVPlacement(0,G4ThreeVector(),
                                                    "PFrame",LogicalFrame,0,false,0);
//   G4ParticleMomentum theDirection(0.,0.,1.);
   G4ThreeVector aPosition(0.,0.,0.);
   G4double aTime = 0.0;
//   G4DynamicParticle* aParticle =
//      new G4DynamicParticle( &aProton, theDirection, incomingEnergy );
   G4Track* aTrack = new G4Track( &aProton, aTime, aPosition );
   //   aTrack->SetVolume( PhysicalFrame );
   aStep.SetTrack( aTrack );

   
    for (G4int ievent = 1; ievent <= nevent; ievent++) {
      pch = theLElasticProcess.PostStepDoIt( *aTrack, aStep );
//      pch = theLElasticProcess.DoIt(&aProton, &Copper, aStep);
      G4double pxi = p*pch->GetMomentumChange()->x();
      G4double pyi = p*pch->GetMomentumChange()->y();
      G4double pzi = p*pch->GetMomentumChange()->z();
      if (ievent <= 10) printf("%d %g %g %g\n", ievent, pxi, pyi, pzi);
      px1 += pxi; py1 += pyi; pz1 += pzi;
      px2 += pxi*pxi; py2 += pyi*pyi; pz2 += pzi*pzi;
   }
   px1 = px1/nevent; py1 = py1/nevent; pz1 = pz1/nevent;
   px2 = px2/nevent - px1*px1;
   py2 = py2/nevent - py1*py1;
   pz2 = pz2/nevent - pz1*pz1;
   px2 = sqrt(abs(px2)); py2 = sqrt(abs(py2)); pz2 = sqrt(abs(pz2));
   G4cout << G4endl << "           mean    sdev" << G4endl;
   G4cout << "Px    " << px1 << "    " << px2 << G4endl;
   G4cout << "Py    " << py1 << "    " << py2 << G4endl;
   G4cout << "Pz    " << pz1 << "    " << pz2 << G4endl;

   return EXIT_SUCCESS;
}

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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: GeneralCrossSectionTest.cc,v 1.1 2004-03-05 13:19:31 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Temporary prototype (Not used for calculations or testing yet. M.K.)
// Test of Cross-Sections (Based on G4CrossSectionDataTest.cc)
//


#include "G4PionPlus.hh"
#include "G4Proton.hh"
#include "G4Element.hh"

#include "G4HadronElasticProcess.hh"
//#include "G4HadronInelasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"

#include "G4ProtonInelasticProcess.hh"
#include "G4PionPlusInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"

#include "G4CrossSectionDataStore.hh"
#include "G4HadronCrossSections.hh"
#include "G4HadronElasticDataSet.hh"
#include "G4HadronFissionDataSet.hh"
#include "G4HadronCaptureDataSet.hh"
#include "G4HadronInelasticDataSet.hh"

#include "G4HadronCrossSectionPlugin.hh"

#ifdef G4_SOLVE_TEMPLATES
#include "g4templates.hh"
#endif


int main()
{
// Element definition

   G4cout << " 1 copper" << G4endl;
   G4cout << " 2 uranium" << G4endl;
   G4int choice;
   G4cin >> choice;

   G4Element* theElement;
   G4Material* theMaterial;
   switch (choice) {
    case 1:
      theElement = new G4Element("Be    ", "Be", 4, 63.54*g/mole);
      theMaterial = new G4Material("copper", 4., 63.54*g/mole, 8.96*g/cm3, 
                                   kStateSolid);
      break;
    case 2:
      theElement = new G4Element("uranium", "U", 92, 238.03*g/mole);
      theMaterial = new G4Material("uranium", 92., 238.03*g/mole, 18.95*g/cm3, 
                                   kStateSolid);
      break;
   }
   //   G4cout << "Dumping element info:" << G4endl;
   //   theElement->DumpInfo();
   //   G4cout << "Dumping material info:" << G4endl;
   //   theMaterial->DumpInfo();

// Particle definition

   G4cout << " 1 proton" << G4endl;
   G4cout << " 2 pion+" << G4endl;
   G4cout << " 3 neutron" << G4endl;
   G4cout << " 4 kaon+" << G4endl;
   G4cout << " 5 kaon0short" << G4endl;
   G4cout << " 6 pion-" << G4endl;
   G4cin >> choice;

   G4ParticleDefinition* theParticleDefinition;
   G4VProcess* theProcess;

   switch (choice) {
    case 1:
      theParticleDefinition = G4Proton::ProtonDefinition();
      theProcess = new G4ProtonInelasticProcess("Inelastic");
      break;
    case 2:
      theParticleDefinition = G4PionPlus::PionPlusDefinition();
      theProcess = new G4PionPlusInelasticProcess("Inelastic");
      break;
    case 3:
      theParticleDefinition = G4Neutron::NeutronDefinition();
      theProcess = new G4NeutronInelasticProcess("Inelastic");
      break;
    case 4:
      theParticleDefinition = G4KaonPlus::KaonPlusDefinition();
      theProcess = new G4KaonPlusInelasticProcess("Inelastic");
      break;
    case 5:
      theParticleDefinition = G4KaonZeroShort::KaonZeroShortDefinition();
      theProcess = new G4KaonZeroSInelasticProcess("Inelastic");
      break;
    case 6:
      theParticleDefinition = G4PionMinus::PionMinusDefinition();
      break;
   }
   //   G4cout << "Dumping particle info:" << G4endl;
   //   theParticleDefinition->DumpTable();

   G4double ekin;
   G4cout << "Kinetic energy in GeV: "<<G4endl;
   G4cin >> ekin;

// Make a dynamic particle too
   G4DynamicParticle* theDynamicParticle;
   theDynamicParticle = new G4DynamicParticle(theParticleDefinition,
                                              G4ParticleMomentum(1.,0.,0.), 
                                              ekin*GeV);
// Process definition

   G4cout << " 1 elastic" << G4endl;
   G4cout << " 2 fission" << G4endl;
   G4cout << " 3 capture" << G4endl;
   G4cout << " 4 inelastic" << G4endl;
   G4cin >> choice;

   G4CrossSectionDataStore theCrossSectionDataStore;

   G4HadronElasticDataSet theElasticDataSet;
   G4HadronFissionDataSet theFissionDataSet;
   theFissionDataSet.SetVerboseLevel(2);
   G4HadronCaptureDataSet theCaptureDataSet;
   G4HadronInelasticDataSet theInelasticDataSet;

   G4HadronCrossSectionPlugin theCrossSectionPlugin;
   theCrossSectionPlugin.SetVerboseLevel(2);

   G4double sig = 0, mfp = 0;

   switch (choice) {
    case 1:
      theProcess = new G4HadronElasticProcess("ELASTIC");
      ((G4HadronElasticProcess*)theProcess)->
         SetCrossSectionDataStore(&theCrossSectionDataStore);
      theCrossSectionDataStore.AddDataSet(&theElasticDataSet);
      theCrossSectionDataStore.AddDataSet(&theCrossSectionPlugin);
      mfp = ((G4HadronElasticProcess*)theProcess)->
               GetMeanFreePathBasic(theDynamicParticle, theMaterial);
      break;
    case 2:
      theProcess = new G4HadronFissionProcess("Fission");
      ((G4HadronFissionProcess*)theProcess)->
         SetCrossSectionDataStore(&theCrossSectionDataStore);
      theCrossSectionDataStore.AddDataSet(&theFissionDataSet);
      //      theCrossSectionDataStore.AddDataSet(&theCrossSectionPlugin);
      mfp = ((G4HadronFissionProcess*)theProcess)->
               GetMeanFreePathBasic(theDynamicParticle, theMaterial);
      break;
    case 3:
      theProcess = new G4HadronCaptureProcess("Capture");
      ((G4HadronCaptureProcess*)theProcess)->
         SetCrossSectionDataStore(&theCrossSectionDataStore);
      theCrossSectionDataStore.AddDataSet(&theCaptureDataSet);
      theCrossSectionDataStore.AddDataSet(&theCrossSectionPlugin);
      mfp = ((G4HadronCaptureProcess*)theProcess)->
               GetMeanFreePathBasic(theDynamicParticle, theMaterial);
      break;
    case 4:
      //      theProcess = new G4HadronInelasticProcess("Inelastic",
      //                                                theParticleDefinition);
      ((G4HadronInelasticProcess*)theProcess)->
         SetCrossSectionDataStore(&theCrossSectionDataStore);
      theCrossSectionDataStore.AddDataSet(&theInelasticDataSet);
      theCrossSectionDataStore.AddDataSet(&theCrossSectionPlugin);
      //      mfp = ((G4HadronInelasticProcess*)theProcess)->
      //               GetMeanFreePathBasic(theDynamicParticle, theMaterial);
      break;
   }

   //   sig = theCrossSectionDataStore.GetCrossSection(theDynamicParticle,
   //                                                  theElement);
   sig = theCrossSectionDataStore.GetCrossSection(theDynamicParticle,
                                                  theElement, 273*kelvin);
   G4cout << theProcess->GetProcessName() << " cross section for " << 
           theParticleDefinition->GetParticleName() <<
           " on " << theElement->GetName() << G4endl;
   G4cout << sig/millibarn << " mb" << G4endl;
   G4cout << "Mean free path = " << mfp << " mm" << G4endl;
}

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4HadronInelasticQBBC.cc,v 1.10 2008-09-17 18:19:33 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4HadronInelasticQBBC
//
// Author: 11 April 2006 V. Ivanchenko
//
// Modified:
// 05.07.2006 V.Ivanchenko fix problem of initialisation of HP
// 15.04.2007 V.Ivanchenko include quasi-elastic and change FTF low energy
//
//----------------------------------------------------------------------------
//

#include "G4HadronInelasticQBBC.hh"

#include "G4HadronInelasticProcess.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"

#include "G4PiNuclearCrossSection.hh"
#include "G4BGGNucleonInelasticXS.hh"
#include "G4BGGPionInelasticXS.hh"

#include "G4TheoFSGenerator.hh"
#include "G4QStringChipsParticleLevelInterface.hh"
#include "G4StringChipsInterface.hh"
#include "G4QGSMFragmentation.hh"
#include "G4LundStringFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

#include "G4BinaryCascade.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4LFission.hh"
#include "G4LCapture.hh"

#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPFission.hh"
#include "G4NeutronHPCapture.hh"

#include "G4UInelasticCrossSection.hh"

G4HadronInelasticQBBC::G4HadronInelasticQBBC(const G4String& name, 
    G4int ver, G4bool ftf, G4bool bert, G4bool chips, G4bool hp, G4bool glauber)
  : G4VPhysicsConstructor(name), verbose(ver), ftfFlag(ftf), bertFlag(bert), 
    chipsFlag(chips), hpFlag(hp), glFlag(glauber), wasActivated(false)
{
  if(verbose > 1) G4cout << "### HadronInelasticQBBC" << G4endl;
  theHPXSecI = 0;
  theHPXSecC = 0;
  theHPXSecF = 0;
  theCascade = 0;
  theCHIPSCascade   = 0;
  theQuasiElastic   = 0;
  theQGStringDecay  = 0;
  theQGStringModel  = 0;
  theFTFBStringDecay = 0;
  theFTFBStringModel = 0;
  theFTFCStringDecay = 0;
  theFTFCStringModel = 0;
}

G4HadronInelasticQBBC::~G4HadronInelasticQBBC()
{
  delete theCascade;
  delete theCHIPSCascade;
  delete theQuasiElastic;
  delete theQGStringDecay;
  delete theQGStringModel;
  delete theFTFBStringDecay;
  delete theFTFCStringDecay;
  delete theFTFBStringModel;
  delete theFTFCStringModel;
  delete theHPXSecI;
  delete theHPXSecC;
  delete theHPXSecF;
}

void G4HadronInelasticQBBC::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();
}

void G4HadronInelasticQBBC::ConstructProcess()
{
  if(wasActivated) return;
  wasActivated = true;

  if(verbose > 1) 
    G4cout << "### HadronInelasticQBBC Construct Process" << G4endl;

  G4double minEstring  = 4.0*GeV;
  G4double maxEcascade = 5.0*GeV;
  //  G4double minFtf      = 7.5*GeV;

  //Binary
  G4HadronicInteraction* theBIC = new G4BinaryCascade();
  theBIC->SetMinEnergy(0.0);
  theBIC->SetMaxEnergy(maxEcascade);

  //Bertini
  G4HadronicInteraction* theBERT = new G4CascadeInterface();
  theBERT->SetMinEnergy(0.0);
  theBERT->SetMaxEnergy(maxEcascade);

  //CHIPS
  G4HadronicInteraction* theCHIPS = new G4StringChipsInterface();
  theCHIPS->SetMinEnergy(0.0);
  theCHIPS->SetMaxEnergy(maxEcascade);

  //QGS
  theCascade = new G4BinaryCascade();
  theCHIPSCascade = new G4QStringChipsParticleLevelInterface;
  G4TheoFSGenerator* theQGSModel = new G4TheoFSGenerator("QGSC");
  theQGStringModel  = new G4QGSModel< G4QGSParticipants >;
  theQGStringDecay  = new G4ExcitedStringDecay(new G4QGSMFragmentation());
  theQGStringModel->SetFragmentationModel(theQGStringDecay);
  theQGSModel->SetTransport(theCHIPSCascade);

  theQuasiElastic = new G4QuasiElasticChannel();
  theQGSModel->SetQuasiElasticChannel(theQuasiElastic);
  theQGSModel->SetHighEnergyGenerator(theQGStringModel);
  theQGSModel->SetMinEnergy(minEstring);
  theQGSModel->SetMaxEnergy(100*TeV);

  //FTFB
  G4TheoFSGenerator* theFTFBModel = new G4TheoFSGenerator("FTFB");
  theFTFBStringModel = new G4FTFModel();
  theFTFBStringDecay = new G4ExcitedStringDecay(new G4LundStringFragmentation());
  theFTFBStringModel->SetFragmentationModel(theFTFBStringDecay);

  theFTFBModel->SetTransport(theCascade);
  theFTFBModel->SetHighEnergyGenerator(theFTFBStringModel);
  theFTFBModel->SetMinEnergy(minEstring);
  theFTFBModel->SetMaxEnergy(100*TeV);

  //FTFC
  G4TheoFSGenerator* theFTFCModel = new G4TheoFSGenerator("FTFC");
  theFTFCStringModel = new G4FTFModel();
  theFTFCStringDecay = new G4ExcitedStringDecay(new G4LundStringFragmentation());
  theFTFCStringModel->SetFragmentationModel(theFTFCStringDecay);

  theFTFCModel->SetTransport(theCHIPSCascade);
  theFTFCModel->SetHighEnergyGenerator(theFTFCStringModel);
  theFTFCModel->SetMinEnergy(minEstring);
  theFTFCModel->SetMaxEnergy(100*TeV);

  theParticleIterator->reset();
  while( (*theParticleIterator)() ) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String pname = particle->GetParticleName();
    if(verbose > 1) G4cout << "### HadronInelasticQBBC:  " << pname << G4endl;
    if(pname == "anti_lambda"  ||
       pname == "anti_neutron" ||
       pname == "anti_omega-"  || 
       pname == "anti_proton"  || 
       pname == "anti_sigma-"  || 
       pname == "anti_sigma+"  || 
       pname == "anti_xi-"  || 
       pname == "anti_xi0"  || 
       pname == "kaon-"     || 
       pname == "kaon+"     || 
       pname == "kaon0S"    || 
       pname == "kaon0L"    || 
       pname == "lambda"    || 
       pname == "neutron"   || 
       pname == "omega-"    || 
       pname == "pi-"       || 
       pname == "pi+"       || 
       pname == "proton"    || 
       pname == "sigma-"    || 
       pname == "sigma+"    || 
       pname == "xi-"       || 
       pname == "xi0") {
      
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4HadronInelasticProcess* hp = 
	new G4HadronInelasticProcess("hInelastic", particle);
      pmanager->AddDiscreteProcess(hp);

      if(pname == "proton") {
	hp->AddDataSet(&theXSecP);

        if(ftfFlag) hp->RegisterMe(theFTFBModel);
	else        hp->RegisterMe(theQGSModel);
 
	if(bertFlag) hp->RegisterMe(theBERT);
	else         hp->RegisterMe(theBIC);
      
        if(glFlag)
	  hp->AddDataSet(new G4BGGNucleonInelasticXS(particle));

      } else if(pname == "neutron") {
	hp->AddDataSet(&theXSecN);
        if(ftfFlag) hp->RegisterMe(theFTFBModel);
	else        hp->RegisterMe(theQGSModel);

	G4HadronCaptureProcess* theNeutronCapture = 
	  new G4HadronCaptureProcess("nCapture");
	G4HadronFissionProcess* theNeutronFission = 
	  new G4HadronFissionProcess("nFission");
	pmanager->AddDiscreteProcess(theNeutronCapture);
	pmanager->AddDiscreteProcess(theNeutronFission);

	G4double emin = 0.0;
	if(hpFlag) {
	  emin = 19.5*MeV;
          theHPXSecI = new G4NeutronHPInelasticData;
          theHPXSecC = new G4NeutronHPCaptureData;
	  theHPXSecF = new G4NeutronHPFissionData;
	  hp->AddDataSet(theHPXSecI);
	  theNeutronCapture->AddDataSet(theHPXSecC);
	  theNeutronFission->AddDataSet(theHPXSecF);
	  hp->RegisterMe(new G4NeutronHPInelastic());
	  theNeutronCapture->RegisterMe(new G4NeutronHPCapture());
	  theNeutronFission->RegisterMe(new G4NeutronHPFission());
	}

        G4HadronicInteraction* theB;
	if(bertFlag) theB = new G4CascadeInterface();
	else         theB = new G4BinaryCascade();
	theB->SetMinEnergy(emin);
	theB->SetMaxEnergy(maxEcascade);
	hp->RegisterMe(theB);

        if(glFlag)
	  hp->AddDataSet(new G4BGGNucleonInelasticXS(particle));

        G4HadronicInteraction* theC = new G4LCapture();       	
	theC->SetMinEnergy(emin);
	theNeutronCapture->RegisterMe(theC);

        G4HadronicInteraction* theF = new G4LFission();
	theF->SetMinEnergy(emin);
	theNeutronFission->RegisterMe(theF);

      } else if(pname == "pi-" || pname == "pi+") {
	hp->AddDataSet(&thePiCross);
        if(ftfFlag) hp->RegisterMe(theFTFBModel);
	else        hp->RegisterMe(theQGSModel);

        hp->RegisterMe(theBERT);
        //if(bertFlag) hp->RegisterMe(theBERT);
	//else         hp->RegisterMe(theBIC);

	if(glFlag) 
	  hp->AddDataSet(new G4BGGPionInelasticXS(particle));

      } else if(pname == "kaon-"     || 
		pname == "kaon+"     || 
		pname == "kaon0S"    || 
		pname == "kaon0L") {
        hp->RegisterMe(theFTFCModel);
        hp->RegisterMe(theBERT);
	//hp->AddDataSet(new G4UInelasticCrossSection(particle));

      } else if(pname == "lambda"    || 
		pname == "sigma-"    || 
		pname == "sigma+"    || 
		pname == "xi-"       || 
		pname == "xi0") {

	hp->RegisterMe(theFTFCModel);
	hp->RegisterMe(theBERT);
	//hp->AddDataSet(new G4UInelasticCrossSection(particle));

      } else if(pname == "anti_proton" || pname == "anti_neutron") {
	hp->RegisterMe(theFTFCModel);
        hp->RegisterMe(theCHIPS);
	//hp->AddDataSet(new G4UInelasticCrossSection(particle));

      } else {
	hp->RegisterMe(theFTFCModel);
        hp->RegisterMe(theCHIPS);
	//hp->AddDataSet(new G4UInelasticCrossSection(particle));
      }

      if(verbose > 1)
	G4cout << "### HadronInelasticQBBC: " << hp->GetProcessName()
	       << " added for " << pname << G4endl;
    }
  }
}

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
// $Id: G4HadronInelasticQBBC.cc,v 1.4 2006-05-09 11:41:56 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4HadronInelasticQBBC
//
// Author: 11 April 2006 V. Ivanchenko
//
// Modified:
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

#include "G4TheoFSGenerator.hh"
#include "G4StringChipsParticleLevelInterface.hh"
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

#include "G4HadronProcessStore.hh"

G4HadronInelasticQBBC::G4HadronInelasticQBBC(const G4String& name, 
					     G4int ver, G4bool ftf, G4bool bert, 
					     G4bool chips, G4bool hp)
  : G4VPhysicsConstructor(name), verbose(ver), ftfFlag(ftf), bertFlag(bert), 
    chipsFlag(chips), hpFlag(hp), wasActivated(false)
{
  if(verbose > 1) G4cout << "### HadronInelasticQBBC" << G4endl;
  store = G4HadronProcessStore::Instance();
}

G4HadronInelasticQBBC::~G4HadronInelasticQBBC()
{
  if(wasActivated) {
    delete theCHIPSCascade;
    delete theQGStringDecay;
    delete theQGStringModel;
    delete theFTFStringDecay;
    delete theFTFStringModel;
  }
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

  if(verbose > 1) G4cout << "### HadronInelasticQBBC Construct Process" << G4endl;

  G4double minEstring  = 8*GeV;
  G4double maxEcascade = 10.*GeV;

  //Bertini
  G4HadronicInteraction* theBERT = new G4CascadeInterface();
  theBERT->SetMinEnergy(0.0);
  theBERT->SetMaxEnergy(maxEcascade);

  //CHIPS
  G4HadronicInteraction* theCHIPS = new G4StringChipsInterface();
  theCHIPS->SetMinEnergy(0.0);
  theCHIPS->SetMaxEnergy(maxEcascade);

  //QGSC
  G4TheoFSGenerator* theQGSModel = new G4TheoFSGenerator();
  theQGStringModel  = new G4QGSModel< G4QGSParticipants >;
  theQGStringDecay  = new G4ExcitedStringDecay(new G4QGSMFragmentation());
  theQGStringModel->SetFragmentationModel(theQGStringDecay);
  theCHIPSCascade   = new G4StringChipsParticleLevelInterface;
  theQGSModel->SetTransport(theCHIPSCascade);
  theQGSModel->SetHighEnergyGenerator(theQGStringModel);
  theQGSModel->SetMinEnergy(minEstring);
  theQGSModel->SetMaxEnergy(100*TeV);

  //FTFC
  G4TheoFSGenerator* theFTFModel = new G4TheoFSGenerator();
  theFTFStringModel = new G4FTFModel();
  theFTFStringDecay = new G4ExcitedStringDecay(new G4LundStringFragmentation());
  theFTFStringModel->SetFragmentationModel(theFTFStringDecay);
  theFTFModel->SetTransport(theCHIPSCascade);
  theFTFModel->SetHighEnergyGenerator(theFTFStringModel);
  theFTFModel->SetMinEnergy(minEstring);
  theFTFModel->SetMaxEnergy(100*TeV);

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
        if(ftfFlag) Register(particle,hp,theFTFModel,"FTF");
	else        Register(particle,hp,theQGSModel,"QGS");

	if(bertFlag)       Register(particle,hp,theBERT,"Bertini");
        else if(chipsFlag) Register(particle,hp,theCHIPS,"CHIPS");
	else {
	  G4HadronicInteraction* theBIC = new G4BinaryCascade();
	  theBIC->SetMinEnergy(0.0);
	  theBIC->SetMaxEnergy(maxEcascade);
	  Register(particle,hp,theBIC,"Binary");
	}

      } else if(pname == "neutron") {
	hp->AddDataSet(&theXSecN);
        if(ftfFlag) Register(particle,hp,theFTFModel,"FTF");
	else        Register(particle,hp,theQGSModel,"QGS");

	G4HadronCaptureProcess* theNeutronCapture = 
	  new G4HadronCaptureProcess("nCapture");
	G4HadronFissionProcess* theNeutronFission = 
	  new G4HadronFissionProcess("nFission");
	pmanager->AddDiscreteProcess(theNeutronCapture);
	pmanager->AddDiscreteProcess(theNeutronFission);

	G4double emin = 0.0;
	if(hpFlag) {
	  emin = 19.9;
	  hp->AddDataSet(&theHPXSecI);
	  theNeutronCapture->AddDataSet(&theHPXSecC);
	  theNeutronFission->AddDataSet(&theHPXSecF);
          G4NeutronHPInelastic* hpi = new G4NeutronHPInelastic();
          G4NeutronHPCapture* hpc = new G4NeutronHPCapture();
          G4NeutronHPFission* hpf = new G4NeutronHPFission();
	  Register(particle,hp,hpi,"HP");
	  Register(particle,theNeutronCapture,hpc,"HP");
	  Register(particle,theNeutronFission,hpf,"HP");
	}

	if(bertFlag)       Register(particle,hp,theBERT,"Bertini");
        else if(chipsFlag) Register(particle,hp,theCHIPS,"CHIPS");
	else {
	  G4HadronicInteraction* theBIC = new G4BinaryCascade();
	  theBIC->SetMinEnergy(emin);
	  theBIC->SetMaxEnergy(maxEcascade);
	  Register(particle,hp,theBIC,"Binary");
	}

	G4HadronicInteraction* theC = new G4LCapture();
	theC->SetMinEnergy(emin);
	theC->SetMaxEnergy(maxEcascade);
	Register(particle,theNeutronCapture,theC,"LCapture");

	G4HadronicInteraction* theF = new G4LFission();
	theF->SetMinEnergy(emin);
	theF->SetMaxEnergy(maxEcascade);
	Register(particle,theNeutronFission,theF,"LFission");

      } else if(pname == "pi-" || pname == "pi+") {
	hp->AddDataSet(&thePiCross);
        if(ftfFlag) Register(particle,hp,theFTFModel,"FTF");
	else        Register(particle,hp,theQGSModel,"QGS");

        if(chipsFlag) Register(particle,hp,theCHIPS,"CHIPS");
	else          Register(particle,hp,theBERT,"Bertini");

      } else if(pname == "kaon-"     || 
		pname == "kaon+"     || 
		pname == "kaon0S"    || 
		pname == "kaon0L") {
        if(ftfFlag) Register(particle,hp,theFTFModel,"FTF");
	else        Register(particle,hp,theQGSModel,"QGS");

        if(chipsFlag) Register(particle,hp,theCHIPS,"CHIPS");
	else          Register(particle,hp,theBERT,"Bertini");

      } else if(pname == "lambda"    || 
		pname == "sigma-"    || 
		pname == "sigma+"    || 
		pname == "xi-"       || 
		pname == "xi0") {
	Register(particle,hp,theFTFModel,"FTF");
        if(chipsFlag) Register(particle,hp,theCHIPS,"CHIPS");
	else          Register(particle,hp,theBERT,"Bertini");

      } else if(pname == "anti_proton" || pname == "anti_neutron") {
	Register(particle,hp,theFTFModel,"FTF");
        Register(particle,hp,theCHIPS,"CHIPS");

      } else {
	Register(particle,hp,theFTFModel,"FTF");
        Register(particle,hp,theCHIPS,"CHIPS");
      }

      if(verbose > 1)
	G4cout << "### HadronInelasticQBBC: " << hp->GetProcessName()
	       << " added for " << pname << G4endl;
    }
  }
}

void G4HadronInelasticQBBC::Register(G4ParticleDefinition* p, 
				     G4HadronicProcess* hp, 
				     G4HadronicInteraction* hi, 
				     const G4String& m)
{
  hp->RegisterMe(hi);
  store->Register(hp,p,hi,m);
  if(verbose > 1)
    G4cout << "### QBBC: Register new model " << m 
	   << " for " << p->GetParticleName() << " and " << hp->GetProcessName()
	   << " E(GeV) " << hi->GetMinEnergy()/GeV 
	   << " - " << hi->GetMaxEnergy()/GeV << G4endl;
}

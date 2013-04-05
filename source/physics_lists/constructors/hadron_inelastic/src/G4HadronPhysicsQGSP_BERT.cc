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
// $Id$
//
//---------------------------------------------------------------------------
//
// ClassName:   G4HadronPhysicsQGSP_BERT
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 23.11.2005 G.Folger: migration to non static particles
// 08.06.2006 V.Ivanchenko: remove stopping
// 20.06.2006 G.Folger: Bertini applies to Kaons, i.e. use SetMinEnergy instead of SetMinPionEnergy
// 25.04.2007 G.Folger: Add code for quasielastic
// 10.12.2007 G.Folger: Add projectilediffrative option for proton/neutron, off by default
// 31.10.2012 A.Ribon: Use G4MiscBuilder
// 19.03.2013 A.Ribon: Replace LEP with FTFP
//
//----------------------------------------------------------------------------
//
#include <iomanip>   

#include "G4HadronPhysicsQGSP_BERT.hh"

#include "globals.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4IonConstructor.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(G4HadronPhysicsQGSP_BERT);


G4HadronPhysicsQGSP_BERT::G4HadronPhysicsQGSP_BERT(G4int)
    :  G4VPhysicsConstructor("hInelastic QGSP_BERT")
    , theNeutrons(0)
    , theLEPNeutron(0)
    , theFTFPNeutron(0)
    , theQGSPNeutron(0)
    , theBertiniNeutron(0)
    , thePiK(0)
    , theFTFPPiK(0)
    , theQGSPPiK(0)
    , theBertiniPiK(0)
    , thePro(0)
    , theFTFPPro(0)
    , theQGSPPro(0) 
    , theBertiniPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , QuasiElastic(true)
{
}

G4HadronPhysicsQGSP_BERT::G4HadronPhysicsQGSP_BERT(const G4String& name, G4bool quasiElastic)
    :  G4VPhysicsConstructor(name)
    , theNeutrons(0)
    , theLEPNeutron(0)
    , theFTFPNeutron(0)
    , theQGSPNeutron(0)
    , theBertiniNeutron(0)
    , thePiK(0)
    , theFTFPPiK(0)
    , theQGSPPiK(0)
    , theBertiniPiK(0)
    , thePro(0)
    , theFTFPPro(0)
    , theQGSPPro(0) 
    , theBertiniPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , QuasiElastic(quasiElastic)
{
}

void G4HadronPhysicsQGSP_BERT::CreateModels()
{
  G4bool quasiElasticFTF= false;   // Use built-in quasi-elastic (not add-on)
  G4bool quasiElasticQGS= true;    // For QGS, it must use it.

  const G4double minQGSP = 12.0*GeV;
  const G4double maxFTFP = 25.0*GeV;
  const G4double minFTFP =  9.5*GeV;
  const G4double maxBERT =  9.9*GeV;
  const G4double minBERT =  0.0*GeV;

  theNeutrons=new G4NeutronBuilder;
  theNeutrons->RegisterMe(theQGSPNeutron=new G4QGSPNeutronBuilder(quasiElasticQGS));
  theQGSPNeutron->SetMinEnergy(minQGSP);   
  theNeutrons->RegisterMe(theFTFPNeutron=new G4FTFPNeutronBuilder(quasiElasticFTF));
  theFTFPNeutron->SetMinEnergy(minFTFP);
  theFTFPNeutron->SetMaxEnergy(maxFTFP);  
  // Exclude LEP only from Inelastic 
  //  -- Register it for other processes: Capture, Elastic
  theNeutrons->RegisterMe(theLEPNeutron=new G4LEPNeutronBuilder);
  theLEPNeutron->SetMinInelasticEnergy(0.0*GeV);
  theLEPNeutron->SetMaxInelasticEnergy(0.0*GeV);

  theNeutrons->RegisterMe(theBertiniNeutron=new G4BertiniNeutronBuilder);
  theBertiniNeutron->SetMinEnergy(minBERT);
  theBertiniNeutron->SetMaxEnergy(maxBERT);

  thePro=new G4ProtonBuilder;
  thePro->RegisterMe(theQGSPPro=new G4QGSPProtonBuilder(quasiElasticQGS));
  theQGSPPro->SetMinEnergy(minQGSP);   
  thePro->RegisterMe(theFTFPPro=new G4FTFPProtonBuilder(quasiElasticFTF));
  theFTFPPro->SetMinEnergy(minFTFP);
  theFTFPPro->SetMaxEnergy(maxFTFP);
  thePro->RegisterMe(theBertiniPro=new G4BertiniProtonBuilder);
  theBertiniPro->SetMaxEnergy(maxBERT);

  thePiK=new G4PiKBuilder;
  thePiK->RegisterMe(theQGSPPiK=new G4QGSPPiKBuilder(quasiElasticQGS));
  theQGSPPiK->SetMinEnergy(minQGSP);
  thePiK->RegisterMe(theFTFPPiK=new G4FTFPPiKBuilder(quasiElasticFTF));
  theFTFPPiK->SetMinEnergy(minFTFP);
  theFTFPPiK->SetMaxEnergy(maxFTFP);
  thePiK->RegisterMe(theBertiniPiK=new G4BertiniPiKBuilder);
  theBertiniPiK->SetMaxEnergy(maxBERT);

  theHyperon=new G4HyperonFTFPBuilder;

  theAntiBaryon=new G4AntiBarionBuilder;
  theAntiBaryon->RegisterMe(theFTFPAntiBaryon=new G4FTFPAntiBarionBuilder(quasiElasticFTF));
}

G4HadronPhysicsQGSP_BERT::~G4HadronPhysicsQGSP_BERT()
{
   delete theBertiniNeutron;
   delete theQGSPNeutron;
   delete theFTFPNeutron;
   delete theLEPNeutron;
   delete theNeutrons;
   delete theBertiniPiK;
   delete theQGSPPiK;
   delete theFTFPPiK;
   delete thePiK;
   delete theBertiniPro;
   delete theQGSPPro;
   delete theFTFPPro;
   delete thePro;
   delete theFTFPAntiBaryon;
   delete theAntiBaryon;
   delete theHyperon;
}

void G4HadronPhysicsQGSP_BERT::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
}

#include "G4ProcessManager.hh"
void G4HadronPhysicsQGSP_BERT::ConstructProcess()
{
  CreateModels();
  theNeutrons->Build();
  thePro->Build();
  thePiK->Build();
  theHyperon->Build(); 
  theAntiBaryon->Build();
}


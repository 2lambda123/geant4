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
//
// File name:     RadmonPhysicsNeutronBertini.cc
// Creation date: Nov 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonPhysicsNeutronBertini.cc,v 1.3 2009-06-04 13:45:31 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//

#include "RadmonPhysicsNeutronBertini.hh"

#include "G4Neutron.hh"

#include "G4ProcessManager.hh"

#include "G4HadronElasticProcess.hh"
#include "G4LElastic.hh"
#include "G4CascadeInterface.hh"

#include "G4LENeutronInelastic.hh"


#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4PreCompoundModel.hh"
#include "G4ExcitationHandler.hh"
#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4QGSMFragmentation.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4NeutronInelasticCrossSection.hh"
#include "G4LCapture.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4LFission.hh"
#include "G4HadronFissionProcess.hh"



RadmonVSubPhysicsListWithLabel *                RadmonPhysicsNeutronBertini :: New(void) const
{
 return new RadmonPhysicsNeutronBertini;
}



void                                            RadmonPhysicsNeutronBertini :: ConstructParticle(void)
{
 G4Neutron::NeutronDefinition();
}



void                                            RadmonPhysicsNeutronBertini :: ConstructProcess(void)
{
 G4ProcessManager * manager(G4Neutron::NeutronDefinition()->GetProcessManager());



 G4HadronElasticProcess * elasticProcess(new G4HadronElasticProcess());
 elasticProcess->RegisterMe(new G4LElastic());
 manager->AddDiscreteProcess(elasticProcess);


 G4CascadeInterface * bertiniCascadeModel(new G4CascadeInterface());
 // bertiniCascadeModel->SetMaxEnergy(3.2*GeV);
 bertiniCascadeModel->SetMaxEnergy(20.0*GeV);


 G4LENeutronInelastic * lepModel(new G4LENeutronInelastic());
 lepModel->SetMinEnergy(0.1*GeV); //thermal neutrons ( 100 MeV)
 // lepModel->SetMinEnergy(2.8*GeV); old setup
 lepModel->SetMaxEnergy(25.*GeV);



 G4TheoFSGenerator * qgspModel(new G4TheoFSGenerator());
 G4GeneratorPrecompoundInterface * cascade(new G4GeneratorPrecompoundInterface());
 cascade->SetDeExcitation(new G4PreCompoundModel(new G4ExcitationHandler()));
 qgspModel->SetTransport(cascade);
 G4QGSModel<G4QGSParticipants> * stringModel(new G4QGSModel<G4QGSParticipants>);
 stringModel->SetFragmentationModel(new G4ExcitedStringDecay(new G4QGSMFragmentation()));
 qgspModel->SetHighEnergyGenerator(stringModel);
 qgspModel->SetMinEnergy(20*GeV);
 qgspModel->SetMaxEnergy(100*TeV);

 G4NeutronInelasticProcess * inelasticProcess(new G4NeutronInelasticProcess());
 inelasticProcess->AddDataSet(new G4NeutronInelasticCrossSection());
 inelasticProcess->RegisterMe(bertiniCascadeModel);
 inelasticProcess->RegisterMe(lepModel);
 inelasticProcess->RegisterMe(qgspModel);
 manager->AddDiscreteProcess(inelasticProcess);
 


 G4LCapture * captureModel(new G4LCapture());
 captureModel->SetMinEnergy(0.*eV);
 captureModel->SetMaxEnergy(100.*TeV);
 G4HadronCaptureProcess * captureProcess(new G4HadronCaptureProcess());
 captureProcess->RegisterMe(captureModel);
 manager->AddDiscreteProcess(captureProcess);



 G4LFission * fissionModel(new G4LFission());
 fissionModel->SetMinEnergy(0.*eV);
 fissionModel->SetMaxEnergy(100.*TeV);
 G4HadronFissionProcess * fissionProcess(new G4HadronFissionProcess());
 fissionProcess->RegisterMe(fissionModel);
 manager->AddDiscreteProcess(fissionProcess);
}



void                                            RadmonPhysicsNeutronBertini :: SetCuts(void)
{
}





const RadmonPhysicsInfoList &                   RadmonPhysicsNeutronBertini :: Provides(void) const
{
 if (infoList.GetNPhysicsInfos()==0)
 {
  RadmonPhysicsInfo info;
  
  info.SetProcessName("Elastic");
  info.SetParticleDefinition(G4Neutron::NeutronDefinition());
  info.SetMinEnergy(0.*eV);
  info.SetMaxEnergy(DBL_MAX);
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("Inelastic");
  info.SetParticleDefinition(G4Neutron::NeutronDefinition());
  info.SetMinEnergy(0.*eV);
  info.SetMaxEnergy(100.*TeV);
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("Fission");
  info.SetParticleDefinition(G4Neutron::NeutronDefinition());
  info.SetMinEnergy(0.*eV);
  info.SetMaxEnergy(100.*TeV);
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("Capture");
  info.SetParticleDefinition(G4Neutron::NeutronDefinition());
  info.SetMinEnergy(0.*eV);
  info.SetMaxEnergy(100.*TeV);
  infoList.InsertPhysicsInfo(info);
 }
 
 return infoList;
}

#include "HadronPhysicsCASCADE_HP.hh"

#include "globals.hh"
#include "G4ios.hh"
#include "g4std/iomanip"   
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsCASCADE_HP::HadronPhysicsCASCADE_HP(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
  theNeutrons.RegisterMe(&theLHEPNeutron);
  theNeutrons.RegisterMe(&theCascadeNeutron);
  theNeutrons.RegisterMe(&theHPNeutron);
  theLHEPNeutron.SetMinEnergy(19.9*MeV);
  theLHEPNeutron.SetMinInelasticEnergy(2.8*GeV);
  theCascadeNeutron.SetMinEnergy(19.9*MeV);
  theCascadeNeutron.SetMaxEnergy(3.2*GeV);
  
  thePro.RegisterMe(&theLHEPPro);
  thePro.RegisterMe(&theCascadePro);
  theLHEPPro.SetMinEnergy(2.8*GeV);
  theCascadePro.SetMaxEnergy(3.2*GeV);

  thePiK.RegisterMe(&theLHEPPiK);
}

HadronPhysicsCASCADE_HP::~HadronPhysicsCASCADE_HP() {}

void HadronPhysicsCASCADE_HP::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"
void HadronPhysicsCASCADE_HP::ConstructProcess()
{
  theNeutrons.Build();
  thePro.Build();
  thePiK.Build();
  theMiscLHEP.Build();
  theStoppingHadron.Build();
  theHadronQED.Build();
}
// 2002 by J.P. Wellisch

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
#include "HadronPhysicsQGSC_LEAD.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsQGSC_LEAD::HadronPhysicsQGSC_LEAD(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
  theNeutrons.RegisterMe(&theQGSCNeutron);
  theNeutrons.RegisterMe(&theLEPNeutron);
  theLEPNeutron.SetMaxInelasticEnergy(25*GeV);
  theLEPNeutron.SetMinInelasticEnergy(4.99*GeV);
  theNeutrons.RegisterMe(&theLEADNeutron);

  thePro.RegisterMe(&theQGSCPro);
  thePro.RegisterMe(&theLEPPro);
  theLEPPro.SetMaxEnergy(25*GeV);
  theLEPPro.SetMinEnergy(4.99*GeV);
  thePro.RegisterMe(&theLEADProton);

  thePiK.RegisterMe(&theQGSCPiK);
  thePiK.RegisterMe(&theLEPPiK);
  theLEPPiK.SetMaxEnergy(25*GeV);
  theLEPPiK.SetMinEnergy(4.99*GeV);
  thePiK.RegisterMe(&theLEADPiK);
}

HadronPhysicsQGSC_LEAD::~HadronPhysicsQGSC_LEAD() {}

void HadronPhysicsQGSC_LEAD::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"
void HadronPhysicsQGSC_LEAD::ConstructProcess()
{
  theNeutrons.Build();
  thePro.Build();
  thePiK.Build();
  theMiscLHEP.Build();
  theStoppingHadron.Build();
  theHadronQED.Build();
}
// 2002 by J.P. Wellisch

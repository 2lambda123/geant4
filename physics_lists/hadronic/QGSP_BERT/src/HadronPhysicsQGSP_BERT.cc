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
#include "HadronPhysicsQGSP_BERT.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsQGSP_BERT::HadronPhysicsQGSP_BERT(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
  theNeutrons.RegisterMe(&theQGSPNeutron);
  theNeutrons.RegisterMe(&theLEPNeutron);
  theNeutrons.RegisterMe(&theBertiniNeutron);
  theLEPNeutron.SetMaxInelasticEnergy(25*GeV);
  theLEPNeutron.SetMinInelasticEnergy(9.5*GeV);
  theBertiniNeutron.SetMaxEnergy(9.9*GeV);
  theBertiniNeutron.SetMinEnergy(0.0*GeV);

  thePro.RegisterMe(&theQGSPPro);
  thePro.RegisterMe(&theLEPPro);
  thePro.RegisterMe(&theBertiniPro);
  theLEPPro.SetMaxEnergy(25*GeV);
  theLEPPro.SetMinEnergy(9.5*GeV);
  theBertiniPro.SetMaxEnergy(9.9*GeV);
  
  thePiK.RegisterMe(&theQGSPPiK);
  thePiK.RegisterMe(&theLEPPiK);
  thePiK.RegisterMe(&theBertiniPiK);
  theLEPPiK.SetMaxEnergy(25*GeV);
  theLEPPiK.SetMinPionEnergy(9.5*GeV);
  theBertiniPiK.SetMaxEnergy(9.9*GeV);
  
}

HadronPhysicsQGSP_BERT::~HadronPhysicsQGSP_BERT() {}

void HadronPhysicsQGSP_BERT::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"
void HadronPhysicsQGSP_BERT::ConstructProcess()
{
  theNeutrons.Build();
  thePro.Build();
  thePiK.Build();
  theMiscLHEP.Build();
  theStoppingHadron.Build();
  theHadronQED.Build();
}
// 2002 by J.P. Wellisch

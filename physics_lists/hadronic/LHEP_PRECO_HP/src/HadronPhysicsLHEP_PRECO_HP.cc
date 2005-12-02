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
#include "HadronPhysicsLHEP_PRECO_HP.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

HadronPhysicsLHEP_PRECO_HP::HadronPhysicsLHEP_PRECO_HP(const G4String& name)
                    :  G4VPhysicsConstructor(name) 
{
  theNeutrons.RegisterMe(&theLHEPNeutron);
  theLHEPNeutron.SetMinEnergy(19.9*MeV);
  theLHEPNeutron.SetMinInelasticEnergy(0.15*GeV);
  theNeutrons.RegisterMe(&thePrecoNeutron);
  thePrecoNeutron.SetMinEnergy(19.9*MeV);
  theNeutrons.RegisterMe(&theHPNeutron);
  
  thePro.RegisterMe(&theLHEPPro);
  theLHEPPro.SetMinEnergy(0.15*GeV);
  thePro.RegisterMe(&thePrecoPro);
  
  thePiK.RegisterMe(&theLHEPPiK);
}

HadronPhysicsLHEP_PRECO_HP::~HadronPhysicsLHEP_PRECO_HP() {}

void HadronPhysicsLHEP_PRECO_HP::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

#include "G4ProcessManager.hh"
void HadronPhysicsLHEP_PRECO_HP::ConstructProcess()
{
  theNeutrons.Build();
  thePro.Build();
  thePiK.Build();
  theMiscLHEP.Build();
  theStoppingHadron.Build();
}
// 2002 by J.P. Wellisch

//
// File name:     RadmonPhysicsPhotonStandard.cc
// Creation date: Nov 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonPhysicsPhotonStandard.cc,v 1.1 2005-11-10 08:15:26 capra Exp $
// Tag:           $Name: not supported by cvs2svn $
//

#include "RadmonPhysicsPhotonStandard.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "G4ProcessManager.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4StepLimiter.hh"

RadmonVSubPhysicsListWithLabel *                RadmonPhysicsPhotonStandard :: New(void) const
{
 return new RadmonPhysicsPhotonStandard;
}



void                                            RadmonPhysicsPhotonStandard :: ConstructParticle(void)
{
 G4Gamma::GammaDefinition();
 G4Electron::ElectronDefinition();
}



void                                            RadmonPhysicsPhotonStandard :: ConstructProcess(void)
{
 G4ProcessManager * manager(G4Gamma::GammaDefinition()->GetProcessManager());

 manager->AddDiscreteProcess(new G4PhotoElectricEffect);
 manager->AddDiscreteProcess(new G4ComptonScattering);
 manager->AddDiscreteProcess(new G4GammaConversion);
 manager->AddProcess(new G4StepLimiter(), ordInActive, ordInActive, 4);
}



void                                            RadmonPhysicsPhotonStandard :: SetCuts(void)
{
}





const RadmonPhysicsInfoList &                   RadmonPhysicsPhotonStandard :: Provides(void) const
{
 if (infoList.GetNPhysicsInfos()==0)
 {
  RadmonPhysicsInfo info;
  
  info.SetProcessName("PhotoElectric");
  info.SetParticleDefinition(G4Gamma::GammaDefinition());
  info.SetMinEnergy(0.1*keV);
  info.SetMaxEnergy(100.*GeV);
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("Compton");
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("GammaConversion");
  info.SetMinEnergy(2.0*electron_mass_c2);
  info.SetMaxEnergy(100.*GeV);
  infoList.InsertPhysicsInfo(info);

  info.SetProcessName("StepLimiter");
  info.SetMinEnergy(0.*eV);
  info.SetMaxEnergy(DBL_MAX);
  infoList.InsertPhysicsInfo(info);
 }
 
 return infoList;
}

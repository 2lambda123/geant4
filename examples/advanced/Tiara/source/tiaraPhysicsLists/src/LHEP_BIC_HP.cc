#include "LHEP_BIC_HP.hh"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>   

#include "GeneralPhysics.hh"
#include "EMPhysics.hh"
#include "MuonPhysics.hh"
#include "HadronPhysicsLHEP_BIC_HP.hh"
#include "IonPhysics.hh"

#include "G4DataQuestionaire.hh"

LHEP_BIC_HP::LHEP_BIC_HP():  G4VModularPhysicsList()
{
  // default cut value  (1.0mm) 
  // defaultCutValue = 1.0*mm;

  G4DataQuestionaire(photon);
  G4cout << "You are using the simulation engine: LHEP_BIC_HP 0.5"<<G4endl;
  G4cout <<G4endl<<G4endl;

  defaultCutValue = 0.7*mm;  
  SetVerboseLevel(1);

  // General Physics
  RegisterPhysics( new GeneralPhysics("general") );

  // EM Physics
  RegisterPhysics( new EMPhysics("standard EM"));

  // Muon Physics
  RegisterPhysics(  new MuonPhysics("muon"));

   // Hadron Physics
  RegisterPhysics(  new HadronPhysicsLHEP_BIC_HP("hadron"));

  // Ion Physics
  RegisterPhysics( new IonPhysics("ion"));


}

LHEP_BIC_HP::~LHEP_BIC_HP()
{
}

void LHEP_BIC_HP::SetCuts()
{
  if (verboseLevel >1){
    G4cout << "LHEP_BIC_HP::SetCuts:";
  }  
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 

  SetCutsWithDefault();   
  
  SetCutValue(kNuCut,"nu_e");
  SetCutValue(kNuCut,"nu_tau");
  SetCutValue(kNuCut,"nu_mu");
  SetCutValue(kNuCut,"anti_nu_e");
  SetCutValue(kNuCut,"anti_nu_tau");
  SetCutValue(kNuCut,"anti_nu_mu");
 
  G4VUserPhysicsList::DumpCutValuesTable();  
 
  
}



// 2002 by J.P. Wellisch

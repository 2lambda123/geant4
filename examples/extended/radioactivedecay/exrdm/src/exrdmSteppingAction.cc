#include "G4ios.hh"

#include "exrdmSteppingAction.hh"
#include "exrdmAnalysisManager.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

exrdmSteppingAction::exrdmSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

exrdmSteppingAction::~exrdmSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void exrdmSteppingAction::UserSteppingAction(const G4Step* fStep) 
{
#ifdef G4ANALYSIS_USE
  G4Track* fTrack = fStep->GetTrack();
  G4int StepNo = fTrack->GetCurrentStepNumber();
  if(StepNo >= 10000) fTrack->SetTrackStatus(fStopAndKill);
  if (StepNo == 1) {
    if ( (fTrack->GetDefinition()->GetParticleType() == "nucleus") && 
	 !( fTrack->GetDefinition()->GetPDGStable()) && 
	 fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Target" ) {
      G4String particleName = fTrack->GetDefinition()->GetParticleName();
      //G4double particleName = G4double(fTrack->GetDefinition()->GetPDGEncoding());
      G4double time = fStep->GetPreStepPoint()->GetGlobalTime() ;
	// - fStep->GetPreStepPoint()->GetLocalTime(); // do we have to take out the local time?
      G4double weight =  fStep->GetPreStepPoint()->GetWeight() ;
      // get the analysis manager
      exrdmAnalysisManager* analysis = exrdmAnalysisManager::getInstance();
      //      G4cout << particleName << " " << weight << " " << time/s << G4endl;
      analysis->AddIsotope(particleName, weight, time);
    }
    if (fTrack->GetTrackID() != 1 ){
      if (fTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay") {
	// emitted particles
	if (fTrack->GetDefinition()->GetParticleType() != "nucleus") {
	  G4String particleName = fTrack->GetDefinition()->GetParticleName();
	  //G4double particleName = G4double (fTrack->GetDefinition()->GetPDGEncoding());
	  G4double time = fStep->GetPreStepPoint()->GetGlobalTime() ; 
	  //- fStep->GetPreStepPoint()->GetLocalTime();
	  G4double weight = fStep->GetPreStepPoint()->GetWeight() ;   
	  G4double energy = fStep->GetPreStepPoint()->GetKineticEnergy();
	  //
	  exrdmAnalysisManager::getInstance()->AddParticle(particleName, energy, weight, time);
	}
      }
    }
  }
  // energy deposition: collect energy deposited by decay products only
  if (fTrack->GetTrackID() != 1 ) {
    if (fTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay") {	
      if (fStep->GetTotalEnergyDeposit() ) {
	G4double time = fStep->GetPreStepPoint()->GetGlobalTime() ;
	// - fStep->GetPreStepPoint()->GetLocalTime(); 
	//    G4double time = fStep->GetPreStepPoint()->GetLocalTime(); // time since the track was created
	G4double edep = fStep->GetTotalEnergyDeposit();
	G4double weight = fStep->GetPreStepPoint()->GetWeight() ; 
	if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Detector") edep = -edep;
	exrdmAnalysisManager::getInstance()->AddEnergy(edep,weight,time);
      }
    }
  }
#endif  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



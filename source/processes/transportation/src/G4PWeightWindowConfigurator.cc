#include "G4PWeightWindowConfigurator.hh"
#include "G4ParallelWWnTransportProcess.hh"
#include "G4ParallelWeightWindowProcess.hh" 
#include "G4WeightWindowAlgorithm.hh"
#include "G4ParallelWorld.hh"

G4PWeightWindowConfigurator::
G4PWeightWindowConfigurator(const G4String &particlename,
			    G4ParallelWorld &parallelWorld,
			    G4VWeightWindowStore &wwstore,
			    const G4VWeightWindowAlgorithm *wwAlg,
			    G4PlaceOfAction placeOfAction) :
  fPlacer(particlename),
  fPWorld(parallelWorld),
  fDeleteWWalg( ( ! wwAlg) ),
  fWWalgorithm(( (fDeleteWWalg) ? 
		 new G4WeightWindowAlgorithm(5,3,5) : wwAlg)),
  fExaminer(*fWWalgorithm, 
	    parallelWorld.GetParallelStepper(),  
	    wwstore),
  fParallelWWProcess(0),
  fPlaceOfAction(placeOfAction),
  fTrackTerminator(0)
{}

G4PWeightWindowConfigurator::
~G4PWeightWindowConfigurator()
{  
  if (fParallelWWProcess) {
    fPlacer.RemoveProcess(fParallelWWProcess);
    delete fParallelWWProcess;
    fTrackTerminator = 0;
    fParallelWWProcess = 0;
  }
  if (fDeleteWWalg) {
    delete fWWalgorithm;
  }
}

void G4PWeightWindowConfigurator::
Configure(G4VSamplerConfigurator *preConf)
{
  const G4VTrackTerminator *terminator = 0;
  if (preConf) {
    terminator = preConf->GetTrackTerminator();
  };

  if (fPlaceOfAction == onBoundary) {
    G4ParallelWWnTransportProcess *parallelWWnTransportProcess = 
      new G4ParallelWWnTransportProcess(fExaminer, 
					fPWorld.GetGeoDriver(), 
					fPWorld.GetParallelStepper(),
					terminator);
    fTrackTerminator = parallelWWnTransportProcess;
    fParallelWWProcess = parallelWWnTransportProcess;
  }
  else {
    G4ParallelWeightWindowProcess *parallelWeightWindowProcess = 
      new G4ParallelWeightWindowProcess(fExaminer, 
					fPWorld.GetParallelStepper(),
					terminator,
					fPlaceOfAction);
    fTrackTerminator = parallelWeightWindowProcess;
    fParallelWWProcess = parallelWeightWindowProcess;
  }

  fPlacer.AddProcessAsSecondDoIt(fParallelWWProcess);
}

const G4VTrackTerminator *G4PWeightWindowConfigurator::
GetTrackTerminator() const 
{
  return fTrackTerminator;
}


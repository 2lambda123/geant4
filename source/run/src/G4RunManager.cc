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
//
// $Id: G4RunManager.cc,v 1.78 2003-05-21 21:06:01 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

// On Sun, to prevent conflict with ObjectSpace, G4Timer.hh has to be
// loaded *before* globals.hh...
#include "G4Timer.hh"

#include "G4RunManager.hh"

#include "Randomize.hh"
#include "G4Run.hh"
#include "G4RunMessenger.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VUserPhysicsList.hh"
#include "G4UserRunAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ApplicationState.hh"
#include "G4StateManager.hh"
#include "G4VPersistencyManager.hh"
#include "G4UImanager.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessTable.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Material.hh"
#include "G4ExceptionHandler.hh"
#include "G4ios.hh"
#include "g4std/strstream"

G4RunManager* G4RunManager::fRunManager = 0;

G4RunManager* G4RunManager::GetRunManager()
{ return fRunManager; }

//G4int G4RunManager::RegisterInteruption(int interuptionSignal)
//{ 
//  if(signal(interuptionSignal,ReceiveInteruption)==SIG_ERR) return -1;
//  return 0;
//}

//void G4RunManager::ReceiveInteruption(int sig)
//{
//  if(!fRunManager) return;
//  G4ApplicationState state 
//   = G4StateManager::GetStateManager()->GetCurrentState();
//  if(state==G4State_GeomClosed || state==G4State_EventProc)
//  { fRunManager->AbortRun(true); }
//}

G4RunManager::G4RunManager()
:userDetector(0),physicsList(0),
 userRunAction(0),userPrimaryGeneratorAction(0),userEventAction(0),
 userStackingAction(0),userTrackingAction(0),userSteppingAction(0),
 geometryInitialized(false),physicsInitialized(false),cutoffInitialized(false),
 geometryNeedsToBeClosed(true),runAborted(false),initializedAtLeastOnce(false),
 geometryToBeOptimized(true),runIDCounter(0),verboseLevel(0),DCtable(0),
 currentRun(0),currentEvent(0),n_perviousEventsToBeStored(0),
 numberOfEventToBeProcessed(0),storeRandomNumberStatus(false),currentWorld(0)
{
  defaultExceptionHandler = new G4ExceptionHandler();
  if(fRunManager)
  { G4Exception("G4RunManager constructed twice."); }
  //G4UnitDefinition::BuildUnitsTable();
  fRunManager = this;
  eventManager = new G4EventManager();
  timer = new G4Timer();
  runMessenger = new G4RunMessenger(this);
  previousEvents = new G4std::vector<G4Event*>;
  defaultRegion = new G4Region("DefaultRegionForTheWorld");
  defaultRegion->SetProductionCuts(G4ProductionCutsTable::GetProductionCutsTable()->GetDefaultProductionCuts());
  G4ParticleTable::GetParticleTable()->CreateMessenger();
  G4ProcessTable::GetProcessTable()->CreateMessenger();
  randomNumberStatusDir = "./";
  versionString = " Geant4 version $Name: not supported by cvs2svn $\n                                (19-May-2003)";
  G4cout 
  << "**********************************************" << G4endl
  << versionString << G4endl
  << "             Copyright : Geant4 Collaboration" << G4endl
  << "**********************************************" << G4endl;
}

G4RunManager::~G4RunManager()
{
  if(verboseLevel>0) G4cout << "G4 kernel has come to Quit state." << G4endl;
  G4StateManager* pStateManager = G4StateManager::GetStateManager();
  pStateManager->SetNewState(G4State_Quit);

  if(verboseLevel>1) G4cout << "Deletion of G4 kernel class start." << G4endl;
  delete timer;
  delete runMessenger;
  G4GeometryManager::GetInstance()->OpenGeometry();
  physicsList->RemoveProcessManager();
  G4ParticleTable::GetParticleTable()->DeleteMessenger();
  G4ProcessTable::GetProcessTable()->DeleteMessenger();
  delete previousEvents;
  if(userDetector)
  {
    delete userDetector;
    if(verboseLevel>1) G4cout << "UserDetectorConstruction deleted." << G4endl;
  }
  if(physicsList)
  {
    delete physicsList;
    if(verboseLevel>1) G4cout << "UserPhysicsList deleted." << G4endl;
  }
  if(userRunAction)
  {
    delete userRunAction;
    if(verboseLevel>1) G4cout << "UserRunAction deleted." << G4endl;
  }
  if(userPrimaryGeneratorAction)
  {
    delete userPrimaryGeneratorAction;
    if(verboseLevel>1) G4cout << "UserPrimaryGenerator deleted." << G4endl;
  }
  G4SDManager* fSDM = G4SDManager::GetSDMpointerIfExist();
  if(fSDM)
  {
    delete fSDM;
    if(verboseLevel>1) G4cout << "G4SDManager deleted." << G4endl;
  }
  delete eventManager;
  if(verboseLevel>1) G4cout << "EventManager deleted." << G4endl;
  G4UImanager* pUImanager = G4UImanager::GetUIpointer();
  {
    if(pUImanager) delete pUImanager;
    if(verboseLevel>1) G4cout << "UImanager deleted." << G4endl;
  }
  if(pStateManager) 
  {
    delete pStateManager;
    if(verboseLevel>1) G4cout << "StateManager deleted." << G4endl;
  }
  delete defaultExceptionHandler;
  if(verboseLevel>1) G4cout << "RunManager is deleting." << G4endl;
}

void G4RunManager::BeamOn(G4int n_event,const char* macroFile,G4int n_select)
{
  G4bool cond = ConfirmBeamOnCondition();
  if(cond)
  {
    numberOfEventToBeProcessed = n_event;
    RunInitialization();
    if(n_event>0) DoEventLoop(n_event,macroFile,n_select);
    RunTermination();
  }
}

G4bool G4RunManager::ConfirmBeamOnCondition()
{
  G4StateManager* stateManager = G4StateManager::GetStateManager();

  G4ApplicationState currentState = stateManager->GetCurrentState();
  if(currentState!=G4State_PreInit && currentState!=G4State_Idle)
  {
    G4cerr << "Illegal application state - BeamOn() ignored." << G4endl;
    return false;
  }

  if(!initializedAtLeastOnce)
  {
    G4cerr << " Geant4 kernel should be initialized" << G4endl;
    G4cerr << "before the first BeamOn(). - BeamOn ignored." << G4endl;
    return false;
  }

  if(!geometryInitialized || !physicsInitialized || !cutoffInitialized)
  {
    if(verboseLevel>0)
    {
      G4cout << "Start re-initialization because " << G4endl;
      if(!geometryInitialized) G4cout << "  Geometry" << G4endl;
      if(!physicsInitialized)  G4cout << "  Physics processes" << G4endl;
      if(!cutoffInitialized)  G4cout << "  SetCuts" << G4endl;
      G4cout << "has been modified since last Run." << G4endl;
    }
    Initialize();
  }
  return true;
}

void G4RunManager::RunInitialization()
{
  currentRun = 0;
  if(userRunAction) currentRun = userRunAction->GenerateRun();
  if(!currentRun) currentRun = new G4Run();

  G4StateManager* stateManager = G4StateManager::GetStateManager();
  stateManager->SetNewState(G4State_GeomClosed);
  currentRun->SetRunID(runIDCounter);
  currentRun->SetNumberOfEventToBeProcessed(numberOfEventToBeProcessed);

  BuildPhysicsTables();

  currentRun->SetDCtable(DCtable);
  G4SDManager* fSDM = G4SDManager::GetSDMpointerIfExist();
  if(fSDM)
  { currentRun->SetHCtable(fSDM->GetHCtable()); }
  
  if(userRunAction) userRunAction->BeginOfRunAction(currentRun);
  if(geometryNeedsToBeClosed)
  {
    if(verboseLevel>1) G4cout << "Start closing geometry." << G4endl;
    G4GeometryManager* geomManager = G4GeometryManager::GetInstance();
    geomManager->OpenGeometry();
    geomManager->CloseGeometry(geometryToBeOptimized, verboseLevel>1);
    geometryNeedsToBeClosed = false;
  }

  //previousEvents->clearAndDestroy();
  for(size_t itr=0;itr<previousEvents->size();itr++)
  { delete (*previousEvents)[itr]; }
  previousEvents->clear();
  for(G4int i_prev=0;i_prev<n_perviousEventsToBeStored;i_prev++)
  { previousEvents->push_back((G4Event*)0); }

  runAborted = false;

  if(storeRandomNumberStatus) {
    G4String fileN = randomNumberStatusDir + "currentRun.rndm"; 
    HepRandom::saveEngineStatus(fileN);
  }
  
  if(verboseLevel>0) G4cout << "Start Run processing." << G4endl;
}

void G4RunManager::DoEventLoop(G4int n_event,const char* macroFile,G4int n_select)
{
  G4StateManager* stateManager = G4StateManager::GetStateManager();

  if(verboseLevel>0) 
  { timer->Start(); }

  G4String msg;
  if(macroFile!=0)
  { 
    if(n_select<0) n_select = n_event;
    msg = "/control/execute ";
    msg += macroFile;
  }
  else
  { n_select = -1; }

  G4int i_event;
  for( i_event=0; i_event<n_event; i_event++ )
  {
    stateManager->SetNewState(G4State_EventProc);

    currentEvent = GenerateEvent(i_event);

    eventManager->ProcessOneEvent(currentEvent);

    AnalyzeEvent(currentEvent);

    if(i_event<n_select) G4UImanager::GetUIpointer()->ApplyCommand(msg);
    stateManager->SetNewState(G4State_GeomClosed);
    StackPreviousEvent(currentEvent);
    currentEvent = 0;
    if(runAborted) break;
  }

  if(verboseLevel>0)
  {
    timer->Stop();
    G4cout << "Run terminated." << G4endl;
    G4cout << "Run Summary" << G4endl;
    if(runAborted)
    { G4cout << "  Run Aborted after " << i_event << " events processed." << G4endl; }
    else
    { G4cout << "  Number of events processed : " << n_event << G4endl; }
    G4cout << "  "  << *timer << G4endl;
  }
}

G4Event* G4RunManager::GenerateEvent(G4int i_event)
{
  if(!userPrimaryGeneratorAction)
  {
    G4Exception
    ("G4RunManager::BeamOn - G4VUserPrimaryGeneratorAction is not defined.");
  }

  G4Event* anEvent = new G4Event(i_event);

  if(storeRandomNumberStatus) {
    G4String fileN = randomNumberStatusDir + "currentEvent.rndm"; 
    HepRandom::saveEngineStatus(fileN);
  }  
    
  userPrimaryGeneratorAction->GeneratePrimaries(anEvent);
  return anEvent;
}

void G4RunManager::AnalyzeEvent(G4Event* anEvent)
{
  G4VPersistencyManager* fPersM = G4VPersistencyManager::GetPersistencyManager();
  if(fPersM) fPersM->Store(anEvent);
  currentRun->RecordEvent(anEvent);
}

void G4RunManager::RunTermination()
{
  G4StateManager* stateManager = G4StateManager::GetStateManager();

  //previousEvents->clearAndDestroy();
  for(size_t itr=0;itr<previousEvents->size();itr++)
  { delete (*previousEvents)[itr]; }
  previousEvents->clear();

  if(userRunAction) userRunAction->EndOfRunAction(currentRun);

  G4VPersistencyManager* fPersM = G4VPersistencyManager::GetPersistencyManager();
  if(fPersM) fPersM->Store(currentRun);
  delete currentRun;
  currentRun = 0;
  runIDCounter++;

  stateManager->SetNewState(G4State_Idle);
}

void G4RunManager::StackPreviousEvent(G4Event* anEvent)
{
  G4Event* evt;
  if(n_perviousEventsToBeStored==0)
  { evt = anEvent; }
  else
  {
    previousEvents->insert(previousEvents->begin(),anEvent);
    evt = previousEvents->back();
    previousEvents->pop_back();
  }
  delete evt;
}

void G4RunManager::Initialize()
{
  G4StateManager* stateManager = G4StateManager::GetStateManager();
  G4ApplicationState currentState = stateManager->GetCurrentState();
  if(currentState!=G4State_PreInit && currentState!=G4State_Idle)
  {
    G4cerr << "Illegal application state - "
         << "G4RunManager::Initialize() ignored." << G4endl;
    return;
  }

  stateManager->SetNewState(G4State_Init);
  if(!geometryInitialized) InitializeGeometry();
  if(!physicsInitialized) InitializePhysics();
  if(!cutoffInitialized) InitializeCutOff();
  stateManager->SetNewState(G4State_Idle);
  initializedAtLeastOnce = true;
}

void G4RunManager::InitializeGeometry()
{
  if(!userDetector)
  {
    G4Exception
    ("G4RunManager::InitializeGeometry - G4VUserDetectorConstruction is not defined.");
  }

  if(verboseLevel>1) G4cout << "userDetector->Construct() start." << G4endl;
  DefineWorldVolume(userDetector->Construct(),false);
  geometryInitialized = true;
}

void G4RunManager::InitializePhysics()
{
  if(physicsList)
  {
    if(verboseLevel>1) G4cout << "physicsList->Construct() start." << G4endl;
    physicsList->Construct();
  }
  else
  {
    G4Exception("G4VUserPhysicsList is not defined");
  }
  physicsInitialized = true;
}

void G4RunManager::InitializeCutOff()
{
  if(physicsList)
  {
    if(verboseLevel>1) G4cout << "physicsList->setCut() start." << G4endl;
    physicsList->SetCuts();
    cutoffInitialized = true;
  }
  else
  {
    G4Exception("G4VUserPhysicsList is not defined");
  }
}

void G4RunManager::BuildPhysicsTables()
{
  UpdateRegion();

  // Let G4ProductionCutsTable create new couples
  G4ProductionCutsTable::GetProductionCutsTable()->UpdateCoupleTable();

  if(G4ProductionCutsTable::GetProductionCutsTable()->IsModified())
  {
    physicsList->BuildPhysicsTable();
    G4ProductionCutsTable::GetProductionCutsTable()->PhysicsTableUpdated();
  }

  physicsList->DumpCutValuesTableIfRequested();
}

void G4RunManager::UpdateRegion()
{
  // Let G4RegionStore scan materials
  G4RegionStore::GetInstance()->UpdateMaterialList();
}
  
void G4RunManager::AbortRun(G4bool softAbort)
{
  // This method is valid only for GeomClosed or EventProc state
  G4ApplicationState currentState = 
    G4StateManager::GetStateManager()->GetCurrentState();
  if(currentState==G4State_GeomClosed || currentState==G4State_EventProc)
  {
    runAborted = true;
    if(currentState==G4State_EventProc && !softAbort)
    {
      currentEvent->SetEventAborted();
      eventManager->AbortCurrentEvent();
    }
  }
  else
  {
    G4cerr << "Run is not in progress. AbortRun() ignored." << G4endl;
  }
}

void G4RunManager::AbortEvent()
{
  // This method is valid only for EventProc state
  G4ApplicationState currentState = 
    G4StateManager::GetStateManager()->GetCurrentState();
  if(currentState==G4State_EventProc)
  {
    currentEvent->SetEventAborted();
    eventManager->AbortCurrentEvent();
  }
  else
  {
    G4cerr << "Event is not in progress. AbortEevnt() ignored." << G4endl;
  }
}

void G4RunManager::DefineWorldVolume(G4VPhysicalVolume* worldVol,
                                     G4bool topologyIsChanged)
{
  // The world volume MUST NOT have a region defined by the user
  if(worldVol->GetLogicalVolume()->GetRegion())
  {
    if(worldVol->GetLogicalVolume()->GetRegion()!=defaultRegion)
    {
      G4cerr << "The world volume has a user-defined region <" 
           << worldVol->GetLogicalVolume()->GetRegion()->GetName()
           << ">." << G4endl;
      G4Exception("G4RunManager::DefineWorldVolume",
                "RUN:WorldHasUserDefinedRegion",
                FatalException,
                "World would have a default region assigned by RunManager.");
    }
  }
  else
  {
    // Set the default region to the world
    G4LogicalVolume* worldLog = worldVol->GetLogicalVolume();
    worldLog->SetRegion(defaultRegion);
    defaultRegion->AddRootLogicalVolume(worldLog);
  }

  // Set the world volume, notify the Navigator and reset its state
  currentWorld = worldVol; 
  G4TransportationManager::GetTransportationManager()
      ->GetNavigatorForTracking()
      ->SetWorldVolume(worldVol);
  if (topologyIsChanged) ResetNavigator();

  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();
}

void G4RunManager::ResetNavigator() const
{
  G4StateManager*    stateManager = G4StateManager::GetStateManager();
  G4ApplicationState currentState = stateManager->GetCurrentState();
  
  if(!initializedAtLeastOnce) return;

  if( currentState != G4State_Idle )
  {
    G4cerr << " Geant4 kernel not in Idle state" << G4endl;
    G4cerr << " Navigator is not touched..."     << G4endl;
    return;
  }
  
  // We have to tweak the navigator's state in case a geometry has been
  // modified between runs. By the following calls we ensure that navigator's
  // state is reset properly. It is required the geometry to be closed
  // and previous optimisations to be cleared.

  G4GeometryManager* geomManager = G4GeometryManager::GetInstance();
  if(verboseLevel>1) G4cout << "Start closing geometry." << G4endl;
  geomManager->OpenGeometry();
  geomManager->CloseGeometry(geometryToBeOptimized, verboseLevel>1);

  G4ThreeVector center(0,0,0);
  G4Navigator* navigator =
      G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  navigator->LocateGlobalPointAndSetup(center,0,false);
}

void G4RunManager::rndmSaveThisRun()
{
  G4int runNumber = runIDCounter;
  if(currentRun == 0) runNumber--;        //state Idle; decrease runNumber
  if(!storeRandomNumberStatus || runNumber < 0) {
     G4cerr << "Warning from G4RunManager::rndmSaveThisRun():"
          << " there is no currentRun or its RandomEngineStatus is not available." 
	  << G4endl << "Command ignored." << G4endl;
     return;
  }
  
  G4String fileIn  = randomNumberStatusDir + "currentRun.rndm";
 
  G4std::ostrstream os;
  os << "run" << runNumber << ".rndm" << '\0';
  G4String fileOut = randomNumberStatusDir + os.str();  

  G4String copCmd = "/control/shell cp "+fileIn+" "+fileOut;
  G4UImanager::GetUIpointer()->ApplyCommand(copCmd);
  if(verboseLevel>0) G4cout << "currentRun.rndm is copied to file: " << fileOut << G4endl;    
}

void G4RunManager::rndmSaveThisEvent()
{
  if(!storeRandomNumberStatus || currentEvent == 0) {
     G4cerr << "Warning from G4RunManager::rndmSaveThisEvent():"
          << " there is no currentEvent or its RandomEngineStatus is not available."
	  << G4endl << "Command ignored." << G4endl;
     return;
  }
  
  G4String fileIn  = randomNumberStatusDir + "currentEvent.rndm";

  G4std::ostrstream os;
  os << "run" << currentRun->GetRunID() << "evt" << currentEvent->GetEventID()
     << ".rndm" << '\0';
  G4String fileOut = randomNumberStatusDir + os.str();       

  G4String copCmd = "/control/shell cp "+fileIn+" "+fileOut;
  G4UImanager::GetUIpointer()->ApplyCommand(copCmd);
  if(verboseLevel>0) G4cout << "currentEvent.rndm is copied to file: " << fileOut << G4endl;  
}
  
void G4RunManager::RestoreRandomNumberStatus(G4String fileN)
{
  G4String fileNameWithDirectory;
  if(fileN.index("/")==G4std::string::npos)
  { fileNameWithDirectory = randomNumberStatusDir+fileN; }
  else
  { fileNameWithDirectory = fileN; }
  
  HepRandom::restoreEngineStatus(fileNameWithDirectory);
  if(verboseLevel>0) G4cout << "RandomNumberEngineStatus restored from file: "
         << fileNameWithDirectory << G4endl;
  HepRandom::showEngineStatus();	 
}

void G4RunManager::DumpRegion(G4String rname) const
{
  G4Region* region = G4RegionStore::GetInstance()->GetRegion(rname);
  if(region) DumpRegion(region);
}

void G4RunManager::DumpRegion(G4Region* region) const
{
  if(!region)
  {
    for(size_t i=0;i<G4RegionStore::GetInstance()->size();i++)
    { DumpRegion((*(G4RegionStore::GetInstance()))[i]); }
  }
  else
  {
    G4cout << "Region " << region->GetName() << G4endl;
    G4cout << " Materials : ";
    G4std::vector<G4Material*>::const_iterator mItr = region->GetMaterialIterator();
    size_t nMaterial = region->GetNumberOfMaterials();
    for(size_t iMate=0;iMate<nMaterial;iMate++)
    {
      G4cout << (*mItr)->GetName() << " ";
      mItr++;
    }
    G4cout << G4endl;
    G4ProductionCuts* cuts = region->GetProductionCuts();
    G4cout << " Production cuts : "
           << " gamma " << G4BestUnit(cuts->GetProductionCut("gamma"),"Length")
           << "    e- " << G4BestUnit(cuts->GetProductionCut("e-"),"Length")
           << "    e+ " << G4BestUnit(cuts->GetProductionCut("e+"),"Length")
           << G4endl;
  }
}

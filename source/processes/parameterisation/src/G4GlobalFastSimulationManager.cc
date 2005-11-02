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
// $Id: G4GlobalFastSimulationManager.cc,v 1.14 2005-11-02 09:22:02 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//  
//---------------------------------------------------------------
//
//  G4GlobalFastSimulationManager.cc
//
//  Description:
//    A singleton class which manages the Fast Simulation managers 
//    attached to envelopes. Implementation.
//
//  History:
//    June 98: Verderi && MoraDeFreitas - "G4ParallelWorld" becomes
//             "G4FlavoredParallelWorld"; some method name changes;
//             GetFlavoredWorldForThis now returns a 
//             G4FlavoredParallelWorld pointer.
//    Feb 98: Verderi && MoraDeFreitas - First Implementation.
//    March 98: correction to instanciate dynamically the manager
//
//---------------------------------------------------------------

#include "G4GlobalFastSimulationManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4TransportationManager.hh"
#include "G4FastSimulationMessenger.hh"

G4GlobalFastSimulationManager* 
G4GlobalFastSimulationManager::fGlobalFastSimulationManager = 0;


G4GlobalFastSimulationManager* 
G4GlobalFastSimulationManager::GetGlobalFastSimulationManager()
{
  if(!fGlobalFastSimulationManager)
  {
    fGlobalFastSimulationManager = new G4GlobalFastSimulationManager;
    SetConcreteInstance(fGlobalFastSimulationManager);
  }
  return fGlobalFastSimulationManager;
}


G4GlobalFastSimulationManager::G4GlobalFastSimulationManager()
{
  // It starts closed.
  fClosed=true;
  // Initialises the G4FastSimulationMessenger
  fTheFastSimulationMessenger=new G4FastSimulationMessenger(this);
}

G4GlobalFastSimulationManager::~G4GlobalFastSimulationManager()
{
  delete fTheFastSimulationMessenger;
  fTheFastSimulationMessenger = 0;
}

void G4GlobalFastSimulationManager::FastSimulationNeedsToBeClosed()
{
  fClosed=false;
}

void G4GlobalFastSimulationManager::
AddFastSimulationManager(G4FastSimulationManager* fsmanager)
{
  ManagedManagers.push_back(fsmanager);
}

void G4GlobalFastSimulationManager::
RemoveFastSimulationManager(G4FastSimulationManager* fsmanager)
{
  ManagedManagers.remove(fsmanager);
}


void G4GlobalFastSimulationManager::CloseFastSimulation()
{
  if(fClosed) return;

  G4ParticleTable* theParticleTable;
  
  // Reset the NeededFlavoredWorlds List
  NeededFlavoredWorlds.clearAndDestroy();
  
  // We'll look for models for all particle types
  theParticleTable=G4ParticleTable::GetParticleTable();
  
  // Creates the first world volume clone.
  // G4VPhysicalVolume* aClone=
  GiveMeAWorldVolumeClone();
  
  G4cout << "Closing Fast-Simulation ..." << G4endl;

//=============================================================================
//== Treatment of ghost volumes is to be moved to
//== G4FastSimulationManagerProcess
//==
//==  for (G4int iParticle=0;
//==       iParticle<theParticleTable->entries(); iParticle++) {
//==    G4bool Needed = false;
//==    for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++)
//==      Needed = Needed || ManagedManagers[ifsm]->
//==        InsertGhostHereIfNecessary(aClone,
//==                                   *(theParticleTable->
//==                                     GetParticle(iParticle)));
//==    // if some FSM inserted a ghost, keep this clone.
//==    if(Needed) {
//==      NeededFlavoredWorlds.push_back(new 
//==         G4FlavoredParallelWorld(theParticleTable->GetParticle(iParticle),
//==                                 aClone));
//==      // and prepare a new one.
//==      aClone=GiveMeAWorldVolumeClone();
//==    }
//==  }

  fClosed=true;
}

G4VFlavoredParallelWorld* 
G4GlobalFastSimulationManager::
GetFlavoredWorldForThis(G4ParticleDefinition* particle)
{
  for (size_t ipw=0; ipw<NeededFlavoredWorlds.size(); ipw++)
    if(NeededFlavoredWorlds[ipw]->GetTheParticleType()==particle)
      return NeededFlavoredWorlds[ipw];
  return 0;
}

void 
G4GlobalFastSimulationManager::
ActivateFastSimulationModel(const G4String& aName)
{
  G4bool result = false;
  for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++)
    result = result || ManagedManagers[ifsm]->
                       ActivateFastSimulationModel(aName);
  if(result) 
    G4cout << "Model " << aName << " activated.";
  else
    G4cout << "Model " << aName << " not found.";
  G4cout << G4endl;
}

void 
G4GlobalFastSimulationManager::
InActivateFastSimulationModel(const G4String& aName)
{
  G4bool result = false;
  for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++)
    result = result || ManagedManagers[ifsm]->
                       InActivateFastSimulationModel(aName);
  if(result) 
    G4cout << "Model " << aName << " inactivated.";
  else
    G4cout << "Model " << aName << " not found.";
  G4cout << G4endl;
}

void 
G4GlobalFastSimulationManager::ListEnvelopes(const G4String& aName,
                                             listType theType)
{
  if(theType == ISAPPLICABLE) {
    for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++)
      ManagedManagers[ifsm]->ListModels(aName);
    return;
  }
  
  if(aName == "all") {
    G4int titled = 0;
    for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++) {
      if(theType == NAMES_ONLY) {
        if(!(titled++))
          G4cout << "Current Envelopes for Fast Simulation:\n";
        G4cout << "   "; 
        ManagedManagers[ifsm]->ListTitle();
        G4cout << G4endl;
      }
      else ManagedManagers[ifsm]->ListModels();
    }
  }
  else {
    for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++)
      if(aName == ManagedManagers[ifsm]->
         GetEnvelope()->GetName()){
        ManagedManagers[ifsm]->ListModels();
        break;
      }
  }
}

void 
G4GlobalFastSimulationManager::ListEnvelopes(const G4ParticleDefinition* aPD)
{
  for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++)
    ManagedManagers[ifsm]->ListModels(aPD);
}

G4VPhysicalVolume* 
G4GlobalFastSimulationManager::GiveMeAWorldVolumeClone()
{
  G4VPhysicalVolume* theParallelWorldPhysical;
  
  G4TransportationManager *transportationManager= 
    G4TransportationManager::GetTransportationManager();
  
  G4VSolid *parallelWorldSolid=transportationManager->
    GetNavigatorForTracking()->GetWorldVolume()->
    GetLogicalVolume()->GetSolid();
  
  G4Material *parallelWorldMaterial=transportationManager->
    GetNavigatorForTracking()->GetWorldVolume()->
    GetLogicalVolume()->GetMaterial();
  
  G4LogicalVolume *parallelWorldLog = 
    new G4LogicalVolume(parallelWorldSolid,
                        parallelWorldMaterial,
                        "ParallelWorldLogical",
                        0, 0, 0);
  
  theParallelWorldPhysical= new G4PVPlacement(0,G4ThreeVector(),
                                              "ParallelWorldPhysical",
                                              parallelWorldLog,
                                              0,false,0);
  
  return theParallelWorldPhysical;
}

G4bool 
G4GlobalFastSimulationManager::Notify(G4ApplicationState requestedState)
{ 
  G4StateManager * stateManager = G4StateManager::GetStateManager();
  if((stateManager->GetPreviousState()==G4State_Idle) &&
     (requestedState==G4State_GeomClosed) && (!fClosed) )
  {
    G4cerr
      << "ERROR - G4GlobalFastSimulationManager::Notify()" << G4endl
      << "        1) you are using ghost volumes;" << G4endl
      << "        2) In this case the G4GlobalFastSimulationManager" << G4endl
      << "           MUST be closed BEFORE closing the geometry;" << G4endl
      << "        3) To do this put in your code the call to" << G4endl
      << "           GetGlobalFastSimulationManager()->CloseFastSimulation();"
      << G4endl
      << "           just before closing the geometry." << G4endl;
    G4Exception("G4GlobalFastSimulationManager::Notify()", "InvalidState",
                FatalException, "Fast simulation must be closed.");
  }
  return true;
}

G4VFastSimulationModel* 
G4GlobalFastSimulationManager::
GetFastSimulationModel(const G4String& modelName,
                       const G4VFastSimulationModel* previousFound) const
{
  G4VFastSimulationModel* model = 0;
  // -- flag used to navigate accross the various managers;
  bool foundPrevious(false);
  for (size_t ifsm=0; ifsm<ManagedManagers.size(); ifsm++)
    {
      model = ManagedManagers[ifsm]->
              GetFastSimulationModel(modelName, previousFound, foundPrevious);
      if (model) break;
    }
  return model;
}

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4VModularPhysicsList.cc,v 1.4 2007-06-15 07:26:55 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class implementation file 
// ------------------------------------------------------------
// - Add  ReplacePhysics             14 Mar 2011 by H.Kurashige
// - Add  RemovePhysics               2 May 2011 by H.Kurashige
//  
#include "G4VModularPhysicsList.hh"
#include "G4StateManager.hh"


G4VModularPhysicsList::G4VModularPhysicsList()
  : G4VUserPhysicsList(),
    verboseLevel(0)
{
   physicsVector = new G4PhysConstVector();
}

G4VModularPhysicsList::~G4VModularPhysicsList()
{
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    delete (*itr);
  }
  physicsVector->clear();
  delete physicsVector;
}

void G4VModularPhysicsList::ConstructParticle()
{
  // create particles
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    (*itr)->ConstructParticle();;
  }
}


void G4VModularPhysicsList::ConstructProcess()
{
  AddTransportation();

  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    (*itr)->ConstructProcess();
  }
}



void G4VModularPhysicsList::RegisterPhysics(G4VPhysicsConstructor* fPhysics)
{
  G4StateManager*    stateManager = G4StateManager::GetStateManager();
  G4ApplicationState currentState = stateManager->GetCurrentState();
  if(!(currentState==G4State_PreInit)){
    G4Exception("G4VModularPhysicsList::RegisterPhysics",
		"RUN106", JustWarning,
                "Geant4 kernel is not PreInit state : Method ignored.");
    return;
  }

  G4String pName = fPhysics->GetPhysicsName();  
  G4int pType = fPhysics->GetPhysicsType(); 
  // If physics_type is equal to 0, 
  // following duplication check is omitted 
  // This is TEMPORAL treatment.
  if (pType == 0) {
    physicsVector->push_back(fPhysics);
#ifdef G4VERBOSE
    if (verboseLevel >1){
      G4cout << "G4VModularPhysicsList::RegisterPhysics: "
	     <<  pName  << "with type : " << pType 
	     << "  is added" 
	     << G4endl;
    }
#endif
    return;
  }
   
  // Check if physics with the physics_type same as one of given physics 
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    if ( pType == (*itr)->GetPhysicsType()) break;
  }
  if (itr!= physicsVector->end()) {
#ifdef G4VERBOSE
    if (verboseLevel >0){
      G4cout << "G4VModularPhysicsList::RegisterPhysics: "
	     << "a physics with given type already exists " 
	     << G4endl;
      G4cout << " Type = " << pType << " : " 
	     << "  existing physics is " << (*itr)->GetPhysicsName()  
	     << G4endl;
      G4cout << pName << " can not be registered "<<G4endl;
    }
#endif
    G4String comment ="Duplicate type for ";
    comment += pName;
    G4Exception("G4VModularPhysicsList::RegisterPhysics",
		"RUN107", JustWarning, comment); 
    return;
  }

  // register 
  physicsVector->push_back(fPhysics);

}    

void G4VModularPhysicsList::ReplacePhysics(G4VPhysicsConstructor* fPhysics)
{
  G4StateManager*    stateManager = G4StateManager::GetStateManager();
  G4ApplicationState currentState = stateManager->GetCurrentState();
  if(!(currentState==G4State_PreInit)){
    G4Exception("G4VModularPhysicsList::ReplacePhysics",
		"RUN106", JustWarning,
                "Geant4 kernel is not PreInit state : Method ignored.");
     return;
  }

  G4String pName = fPhysics->GetPhysicsName();  
  G4int pType = fPhysics->GetPhysicsType(); 
  // If physics_type is equal to 0, 
  // duplication check is omitted and just added. 
  // This is TEMPORAL treatment.
  if (pType == 0) {
    // register 
    physicsVector->push_back(fPhysics);
#ifdef G4VERBOSE
    if (verboseLevel >1){
      G4cout << "G4VModularPhysicsList::ReplacePhysics: "
	     <<  pName  << "with type : " << pType 
	     << "  is added" 
	     << G4endl;
    }
#endif
    return;
  }

  // Check if physics with the physics_type same as one of given physics 
  G4PhysConstVector::iterator itr= physicsVector->begin();
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    if ( pType == (*itr)->GetPhysicsType()) break;
  }
  if (itr == physicsVector->end()) {
    // register 
    physicsVector->push_back(fPhysics);
  } else {
#ifdef G4VERBOSE
    if (verboseLevel >1){
      G4cout << "G4VModularPhysicsList::ReplacePhysics: "
	     << (*itr)->GetPhysicsName()  << "with type : " << pType 
	     << "  is replaces with " << pName
	     << G4endl;
    }
#endif

    //  delete exsiting one   
    delete (*itr);
    // replace with given one
    (*itr) = fPhysics;

  }

  return;  
}    

void  G4VModularPhysicsList::RemovePhysics(G4int pType)
{
  G4StateManager*    stateManager = G4StateManager::GetStateManager();
  G4ApplicationState currentState = stateManager->GetCurrentState();
  if(!(currentState==G4State_PreInit)){
    G4Exception("G4VModularPhysicsList::RemovePhysics",
		"RUN106", JustWarning,
                "Geant4 kernel is not PreInit state : Method ignored.");
    return;
  }

  for (G4PhysConstVector::iterator itr = physicsVector->begin(); 
                                     itr!= physicsVector->end();) {
    if ( pType  == (*itr)->GetPhysicsType()) {
      G4String pName = (*itr)->GetPhysicsName();  
#ifdef G4VERBOSE
      if (verboseLevel > 1){
	G4cout << "G4VModularPhysicsList::RemovePhysics: "
	       <<  pName  << "  is removed" 
	       << G4endl;
      }
#endif
      physicsVector->erase(itr);
      break;
    } else {
      itr++;
    }
  }
}

void G4VModularPhysicsList::RemovePhysics(G4VPhysicsConstructor* fPhysics)
{
  G4StateManager*    stateManager = G4StateManager::GetStateManager();
  G4ApplicationState currentState = stateManager->GetCurrentState();
  if(!(currentState==G4State_PreInit)){
    G4Exception("G4VModularPhysicsList::RemovePhysics",
		"RUN106", JustWarning,
                "Geant4 kernel is not PreInit state : Method ignored.");
    return;
  }

  for (G4PhysConstVector::iterator itr = physicsVector->begin(); 
                                     itr!= physicsVector->end();) {
    if ( fPhysics == (*itr)) {
      G4String pName = (*itr)->GetPhysicsName();  
#ifdef G4VERBOSE
    if (verboseLevel > 1){
      G4cout << "G4VModularPhysicsList::RemovePhysics: "
	     <<  pName  << "  is removed" 
	     << G4endl;
    }
#endif
      physicsVector->erase(itr);
      break;
    } else {
      itr++;
    }
  }
}
void G4VModularPhysicsList::RemovePhysics(const G4String& name)
{
  G4StateManager*    stateManager = G4StateManager::GetStateManager();
  G4ApplicationState currentState = stateManager->GetCurrentState();
  if(!(currentState==G4State_PreInit)){
    G4Exception("G4VModularPhysicsList::RemovePhysics",
		"RUN106", JustWarning,
                "Geant4 kernel is not PreInit state : Method ignored.");
    return;
  }

  G4PhysConstVector::iterator itr= physicsVector->begin();
  for (G4PhysConstVector::iterator itr = physicsVector->begin(); 
                                     itr!= physicsVector->end();) {
    G4String pName = (*itr)->GetPhysicsName();  
    if ( name == pName) {
#ifdef G4VERBOSE
    if (verboseLevel > 1){
      G4cout << "G4VModularPhysicsList::RemovePhysics: "
	     <<  pName  << "  is removed" 
	     << G4endl;
    }
#endif
      physicsVector->erase(itr);
      break;
    } else {
      itr++;
    }
  }
}

const G4VPhysicsConstructor* G4VModularPhysicsList::GetPhysics(G4int idx) const
{
  G4int i;
  G4PhysConstVector::iterator itr= physicsVector->begin();
  for (i=0; i<idx && itr!= physicsVector->end() ; ++i) ++itr;
  if (itr!= physicsVector->end()) return (*itr);
  else return 0;
}

const G4VPhysicsConstructor* G4VModularPhysicsList::GetPhysics(const G4String& name) const
{
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    if ( name == (*itr)->GetPhysicsName()) break;
  }
  if (itr!= physicsVector->end()) return (*itr);
  else return 0;
}

const G4VPhysicsConstructor* G4VModularPhysicsList::GetPhysicsWithType(G4int pType) const
{
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    if ( pType == (*itr)->GetPhysicsType()) break;
  }
  if (itr!= physicsVector->end()) return (*itr);
  else return 0;
}


void G4VModularPhysicsList::SetVerboseLevel(G4int value)
{
  verboseLevel = value;
  
  // Loop over constructors
  G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin(); itr!= physicsVector->end(); ++itr) {
    (*itr)->SetVerboseLevel(verboseLevel);
  }

}

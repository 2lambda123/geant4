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
// $Id: G4StateManager.hh,v 1.9 2005-03-15 19:11:35 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//
//       ---------------- G4StateManager ----------------
//
// Authors: G.Cosmo, M.Asai - November 1996
//
// -------------------------------------------------------------
//
// Class Description:
//
// Class responsible for handling and updating the running state
// of the Geant4 application during its different phases.
// The class is a singleton, it can be accessed via the public
// method G4StateManager::GetStateManager().
//
// States of Geant4 are defined in G4ApplicationState.

// -------------------------------------------------------------

#ifndef G4StateManager_h
#define G4StateManager_h 1

#include <vector>
#include "globals.hh"
#include "G4ApplicationState.hh"
#include "G4VStateDependent.hh"
#include "G4VExceptionHandler.hh"

class G4StateManager
{

public: // with description

  static G4StateManager* GetStateManager();
    // The G4StateManager class is a singleton class and the pointer
    // to the only one existing object can be obtained by this static 
    // method.

protected:

  G4StateManager();

public:

  ~G4StateManager();

public: // with description

  G4ApplicationState GetCurrentState() const;
    // Returns the current state
  G4ApplicationState GetPreviousState() const;
    // Returns the previous state
  G4bool SetNewState(G4ApplicationState requestedState);
    // Set Geant4 to a new state.
    // In case the request is irregal, false will be returned
    // and the state of Geant4 will not be changed.
  G4bool SetNewState(G4ApplicationState requestedState, const char* msg);
    // Set Geant4 to a new state.
    // In case the request is irregal, false will be returned
    // and the state of Geant4 will not be changed.
    // "msg" is information associating to this state change
  G4bool RegisterDependent(G4VStateDependent* aDependent,G4bool bottom=false);
    // Register a concrete class of G4VStateDependent.
    // Registered concrete classes will be notified via
    // G4VStateDependent::Notify() method when the state of Geant4 changes.
    // False will be returned if registration fails.
  G4bool DeregisterDependent(G4VStateDependent* aDependent);
    // Remove the registration.
    // False will be returned if aDependent has not been registered.
  G4VStateDependent* RemoveDependent(const G4VStateDependent* aDependent);
    // Remove the registration.
    // Removed pointer is returned.
  G4String GetStateString(G4ApplicationState aState) const;
    // Utility method which returns a string of the state name.

public:

  inline void SetSuppressAbortion(G4int i);
  inline G4int GetSuppressAbortion() const;
  inline const char* GetMessage() const;
  inline void SetExceptionHandler(G4VExceptionHandler* eh);
  inline G4VExceptionHandler* GetExceptionHandler() const;

public:

  //void Pause();
  //void Pause(const char* msg);
  //void Pause(G4String msg);
  ////  G4UIsession::pauseSession() will be invoked. The argument string "msg"
  //// will be used as a prompt characters if the session is non-graphical.
  ////  This method can be invoked by any user action class during the event
  //// loop. After the user's interactions, control goes back to the caller.

private:

  G4StateManager(const G4StateManager &right);
  G4StateManager& operator=(const G4StateManager &right);
  G4int operator==(const G4StateManager &right) const;
  G4int operator!=(const G4StateManager &right) const;

private:

  static G4StateManager* theStateManager;
  G4ApplicationState theCurrentState;
  G4ApplicationState thePreviousState;
  std::vector<G4VStateDependent*> theDependentsList;
  G4VStateDependent* theBottomDependent;
  G4int suppressAbortion;
  const char* msgptr;
  G4VExceptionHandler* exceptionHandler;

};

#include "G4StateManager.icc"

#endif

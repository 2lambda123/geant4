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

#include "G4InteractorMessenger.hh"

#include "G4VInteractiveSession.hh"

/***************************************************************************/
G4VInteractiveSession::G4VInteractiveSession ()
{
  messenger = new G4InteractorMessenger(this);
}

/***************************************************************************/
G4VInteractiveSession::~G4VInteractiveSession() 
{
  delete messenger;
}

/***************************************************************************/
void G4VInteractiveSession::AddMenu (const char*,const char*)
{
}

/***************************************************************************/
void G4VInteractiveSession::AddButton (const char*,const char*,const char*)
{
}

/***************************************************************************/
void G4VInteractiveSession::AddInteractor (G4String a_name,
                                           G4Interactor a_interactor)
{
  interactors[a_name] = a_interactor;
}

/***************************************************************************/
G4Interactor G4VInteractiveSession::GetInteractor (G4String a_name)
{
  G4interactor_map::iterator it;
  if((it=interactors.find(a_name))==interactors.end()) return NULL;
  return (*it).second;  
}

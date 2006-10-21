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
// $Id: G4AttDefStore.hh,v 1.9 2006-10-21 10:12:45 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef G4ATTDEFSTORE_HH
#define G4ATTDEFSTORE_HH

#include "globals.hh"
#include <map>

class G4AttDef;

class G4AttDefStore
{
  public:

    static std::map<G4String,G4AttDef>*
    GetInstance(G4String storeName, G4bool& isNew);
      // Returns a pointer to the named store
      // and isNew is true if store is new.
      // The store keeps the ownership of the returned
      // pointer to the map.

    static const G4String& GetName(const std::map<G4String,G4AttDef>* store);
      // Returns name of given store.

    ~G4AttDefStore();
      // Destructor.

  protected:

    G4AttDefStore();

  private:

    G4AttDefStore(const G4AttDefStore&);
    G4AttDefStore& operator=(const G4AttDefStore&);

    static std::map<G4String,std::map<G4String,G4AttDef>*> m_stores;
    static G4AttDefStore* theInstance;
};

#endif //G4ATTDEFSTORE_H

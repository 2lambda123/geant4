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
// Factory to return pointer to Bertini cross-section table based on
// collision initial state (hadron type codes).
//
// Author:  Michael Kelsey (SLAC)

#include "G4CascadeChannelTables.hh"
#include "G4CascadeChannel.hh"
#include <map>
#include <algorithm>


// Singleton is created at beginning of job

G4CascadeChannelTables G4CascadeChannelTables::instance;


// Return cross-section table requested by user

const G4CascadeChannel* 
G4CascadeChannelTables::FindTable(G4int initialState) {
  return (tables.find(initialState) != tables.end()) ? tables[initialState] : 0;
}


// Register specified table in list, replacing previous version

void 
G4CascadeChannelTables::SaveTable(G4int initialState, G4CascadeChannel* table) {
  if (!table) return;		// Avoid unnecessary work

  if (tables[initialState]) delete tables[initialState];
  tables[initialState] = table;
}


// Convenience function for diagnostic output

void G4CascadeChannelTables::PrintTable(G4int initialState) {
  const G4CascadeChannel* tbl = GetTable(initialState);
  if (tbl) tbl->printTable();
}

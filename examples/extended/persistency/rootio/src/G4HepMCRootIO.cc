// $Id: G4HepMCRootIO.cc,v 1.2 2002-12-04 14:12:26 morita Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// File: G4HepMCRootIO.cc
//
// History:
//   '02.5.7  Youhei Morita  Initial creation

#include "G4HepMCRootIO.hh"

// Implementation of Store
bool G4HepMCRootIO::Store(HepMC::GenEvent* evt)
{
  // G4RootIOManager* pm =
  //     (G4RootIOManager*) G4PersistencyCenter::GetPersistencyCenter()
  //                                           ->CurrentPersistencyManager();

  // actual implementation should come in here

  return true;
}

// Implementation of Retrieve
bool G4HepMCRootIO::Retrieve(HepMC::GenEvent*& evt, int id)
{
  // bool st = false;
  evt = 0;

  // G4RootIOManager* pm =
  //      (G4RootIOManager*) G4PersistencyCenter::GetPersistencyCenter()
  //                                            ->CurrentPersistencyManager();

  // actual implementation should come in here

  return true;
}

// End of G4HepMCRootIO.cc


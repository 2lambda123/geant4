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
// $Id: G4HepRepFileViewer.cc,v 1.13 2005-10-13 17:23:42 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4HepRepFileViewer.hh"

#include "G4VSceneHandler.hh"

#include "G4HepRepFileSceneHandler.hh"

//HepRep
#include "G4HepRepFileXMLWriter.hh"

G4HepRepFileViewer::G4HepRepFileViewer
(G4VSceneHandler& sceneHandler, const G4String& name):
  G4VViewer(sceneHandler, sceneHandler.IncrementViewCount(), name) {
  hepRepXMLWriter = ((G4HepRepFileSceneHandler*)(&sceneHandler))->GetHepRepXMLWriter();
  // Make changes to view parameters for HepRep...
  fVP.SetCulling(false);
  fDefaultVP.SetCulling(false);
}

G4HepRepFileViewer::~G4HepRepFileViewer() {}

void G4HepRepFileViewer::SetView() {
#ifdef G4HEPREPFILEDEBUG
  G4cout << "G4HepRepFileViewer::SetView() called.=" << G4endl;
#endif
}

void G4HepRepFileViewer::ClearView() {
#ifdef G4HEPREPFILEDEBUG
  G4cout << "G4HepRepFileViewer::ClearView() called." << G4endl;
#endif
}

void G4HepRepFileViewer::DrawView() {
#ifdef G4HEPREPFILEDEBUG
  G4cout << "G4HepRepFileViewer::DrawView() called." << G4endl;
#endif
  NeedKernelVisit ();  // Always need to visit G4 kernel.
  ProcessView ();
}

void G4HepRepFileViewer::ShowView () {
#ifdef G4HEPREPFILEDEBUG
  G4cout << "G4HepRepFileViewer::ShowView" << G4endl;
#endif
  G4VViewer::ShowView();
  
  if (hepRepXMLWriter->isOpen)
    hepRepXMLWriter->close();
}

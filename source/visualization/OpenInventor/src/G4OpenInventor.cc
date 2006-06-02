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
// $Id: G4OpenInventor.cc,v 1.19 2006-06-02 06:56:32 gbarrand Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifdef G4VIS_BUILD_OI_DRIVER

// this :
#include "G4OpenInventor.hh"

#include "HEPVis/nodes/SoBox.h"
#include "HEPVis/nodes/SoTubs.h"
#include "HEPVis/nodes/SoCons.h"
#include "HEPVis/nodes/SoTrd.h"
#include "HEPVis/nodes/SoTrap.h"
#include "HEPVis/nodes/SoMarkerSet.h"
#include "HEPVis/nodes/SoImageWriter.h"
#include "HEPVis/nodekits/SoDetectorTreeKit.h"
#include "HEPVis/actions/SoGL2PSAction.h"
#include "HEPVis/actions/SoCounterAction.h"
#include "HEPVis/actions/SoAlternateRepAction.h"

#include "Geant4_SoPolyhedron.h"

#include "G4OpenInventorSceneHandler.hh"

G4OpenInventor::G4OpenInventor (
 const G4String name
,const G4String nickname
,G4VGraphicsSystem::Functionality f
)
:G4VGraphicsSystem(name,nickname,f)
,interactorManager(0)
{
}

G4OpenInventor::~G4OpenInventor () {}

void G4OpenInventor::SetInteractorManager (G4VInteractorManager* im) {
  interactorManager = im;
}
G4VInteractorManager* G4OpenInventor::GetInteractorManager () {
  return interactorManager;
}
G4VSceneHandler* G4OpenInventor::CreateSceneHandler (const G4String& name) {
  Initialize();
  G4VSceneHandler* p = new G4OpenInventorSceneHandler (*this, name);
  return    p;
}

void G4OpenInventor::InitNodes()
{
  SoBox::initClass();
  SoTubs::initClass();
  SoCons::initClass();
  SoTrd::initClass();
  SoTrap::initClass();
  SoDetectorTreeKit::initClass();
  HEPVis_SoMarkerSet::initClass();
  SoImageWriter::initClass();
  Geant4_SoPolyhedron::initClass();

  SoGL2PSAction::initClass();
  SoCounterAction::initClass();
  SoAlternateRepAction::initClass();
}



#endif

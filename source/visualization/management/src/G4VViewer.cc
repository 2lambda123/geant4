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
// $Id: G4VViewer.cc,v 1.22 2006-03-28 16:33:14 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  27th March 1996
// Abstract interface class for graphics views.

#include "G4VViewer.hh"

#include "G4ios.hh"
#include <sstream>

#include "G4VisManager.hh"
#include "G4VGraphicsSystem.hh"
#include "G4VSceneHandler.hh"
#include "G4Scene.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Transform3D.hh"

G4VViewer::G4VViewer (G4VSceneHandler& sceneHandler,
		      G4int id, const G4String& name):
fSceneHandler (sceneHandler),
fViewId (id),
//fModified (true),
fNeedKernelVisit (true)
{
  if (name == "") {
    std::ostringstream ost;
    ost << fSceneHandler.GetName () << '-' << fViewId;
    fName = ost.str();
  }
  else {
    fName = name;
  }
  fShortName = fName (0, fName.find (' '));
  fShortName.strip ();

  G4VisManager* pVisMan = G4VisManager::GetInstance();
  G4int xHint, yHint;
  pVisMan->GetWindowSizeHint(xHint, yHint);
  const G4String& XGeometryString = pVisMan->GetXGeometryString();
  fVP.SetWindowSizeHint(xHint,yHint);
  fVP.SetXGeometryString(XGeometryString);
  fDefaultVP.SetWindowSizeHint(xHint,yHint);
  fDefaultVP.SetXGeometryString(XGeometryString);
}

G4VViewer::~G4VViewer () {}

void G4VViewer::SetName (const G4String& name) {
  fName = name;
  fShortName = fName (0, fName.find (' '));
  fShortName.strip ();
}

const G4VisAttributes* G4VViewer::GetApplicableVisAttributes
(const G4VisAttributes* pVisAttribs) const {
  // If the pointer is null, pick up the default vis attributes from
  // the view parameters.
  if (!pVisAttribs)
    pVisAttribs = GetViewParameters ().GetDefaultVisAttributes ();
  return pVisAttribs;
}

void G4VViewer::NeedKernelVisit () {

  fNeedKernelVisit = true;

  // At one time I thought we'd better notify all viewers.  But I guess
  // each viewer can take care of itself, so the following code is
  // redundant (but keep it commented out for now).   (John Allison)
  // Notify all viewers that a kernel visit is required.
  // const G4ViewerList& viewerList = fSceneHandler.GetViewerList ();
  // G4ViewerListConstIterator i;
  // for (i = viewerList.begin(); i != viewerList.end(); i++) {
  //   (*i) -> SetNeedKernelVisit ();
  // }
  // ??...but, there's a problem in OpenGL Stored which seems to
  // require *all* viewers to revisit the kernel, so...
  /*
  const G4ViewerList& viewerList = fSceneHandler.GetViewerList ();
  G4ViewerListConstIterator i;
  for (i = viewerList.begin(); i != viewerList.end(); i++) {
    (*i) -> SetNeedKernelVisit (true);
  }
  */
  // Feb 2005 - commented out.  Let's fix OpenGL if necessary.
}

void G4VViewer::FinishView () {}

void G4VViewer::ShowView () {}

void G4VViewer::ProcessView ()
{
  // If ClearStore has been requested, e.g., if the scene has changed,
  // or if the concrete viewer has decided that it necessary to visit
  // the kernel, perhaps because the view parameters have changed
  // significantly (this should be done in the concrete viewer's
  // DrawView)...
  if (fNeedKernelVisit) {
    // Reset flag.  This must be done before ProcessScene to prevent
    // recursive calls when recomputing transients...
    fNeedKernelVisit = false;
    fSceneHandler.ProcessScene (*this);
  }
}

void G4VViewer::SetViewParameters (const G4ViewParameters& vp) {
  fVP = vp;
}

std::ostream& operator << (std::ostream& os, const G4VViewer& v) {
  os << "View " << v.fName << ":\n";
  os << v.fVP;
  return os;
}

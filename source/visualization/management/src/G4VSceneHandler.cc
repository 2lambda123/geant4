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
// $Id: G4VSceneHandler.cc,v 1.74 2006-11-06 11:40:08 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  19th July 1996
// Abstract interface class for graphics scenes.

#include "G4VSceneHandler.hh"

#include "G4ios.hh"
#include <sstream>

#include "G4VisManager.hh"
#include "G4VGraphicsSystem.hh"
#include "G4VViewer.hh"
#include "G4VSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4Polyline.hh"
#include "G4Scale.hh"
#include "G4Text.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4Polymarker.hh"
#include "G4Polyhedron.hh"
#include "G4NURBS.hh"
#include "G4Visible.hh"
#include "G4VisAttributes.hh"
#include "G4VModel.hh"
#include "G4TrajectoriesModel.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4Para.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4ModelingParameters.hh"
#include "G4VTrajectory.hh"
#include "G4VHit.hh"
#include "Randomize.hh"
#include "G4StateManager.hh"
#include "G4RunManager.hh"
#include "G4Transform3D.hh"

G4VSceneHandler::G4VSceneHandler (G4VGraphicsSystem& system, G4int id, const G4String& name):
  fSystem                (system),
  fSceneHandlerId        (id),
  fViewCount             (0),
  fpViewer               (0),
  fpScene                (0),
  fMarkForClearingTransientStore (true),  // Ready for first
					  // ClearTransientStoreIfMarked(),
					  // e.g., at end of run (see
					  // G4VisManager.cc).
  fReadyForTransients    (true),  // Only false while processing scene.
  fProcessingSolid       (false),
  fSecondPassRequested   (false),
  fSecondPass            (false),
  fpModel                (0),
  fpObjectTransformation (0),
  fNestingDepth          (0),
  fpVisAttribs           (0)
{
  G4VisManager* pVMan = G4VisManager::GetInstance ();
  fpScene = pVMan -> GetCurrentScene ();
  if (name == "") {
    std::ostringstream ost;
    ost << fSystem.GetName () << '-' << fSceneHandlerId;
    fName = ost.str();
  }
  else {
    fName = name;
  }
  fTransientsDrawnThisEvent = pVMan->GetTransientsDrawnThisEvent();
  fTransientsDrawnThisRun = pVMan->GetTransientsDrawnThisRun();
}

G4VSceneHandler::~G4VSceneHandler () {
  G4ViewerListIterator i;
  for (i = fViewerList.begin(); i != fViewerList.end(); ++i) {
    delete *i;
  }
}

void G4VSceneHandler::PreAddSolid (const G4Transform3D& objectTransformation,
                                  const G4VisAttributes& visAttribs) {
  fpObjectTransformation = &objectTransformation;
  fpVisAttribs = &visAttribs;
  fProcessingSolid = true;
}

void G4VSceneHandler::PostAddSolid () {
  fpObjectTransformation = 0;
  fpVisAttribs = 0;
  fProcessingSolid = false;
  if (fReadyForTransients) {
    fTransientsDrawnThisEvent = true;
    fTransientsDrawnThisRun = true;
  }
}

void G4VSceneHandler::BeginPrimitives
(const G4Transform3D& objectTransformation) {
  fNestingDepth++;
  if (fNestingDepth > 1)
    G4Exception("G4VSceneHandler::BeginPrimitives: Nesting detected."
		"\n  It is illegal to nest Begin/EndPrimitives.");
  fpObjectTransformation = &objectTransformation;
}

void G4VSceneHandler::EndPrimitives () {
  if (fNestingDepth <= 0)
    G4Exception("G4VSceneHandler::EndPrimitives: Nesting error");
  fNestingDepth--;
  fpObjectTransformation = 0;
  if (fReadyForTransients) {
    fTransientsDrawnThisEvent = true;
    fTransientsDrawnThisRun = true;
  }
}

void G4VSceneHandler::BeginPrimitives2D () {
  fNestingDepth++;
  if (fNestingDepth > 1)
    G4Exception("G4VSceneHandler::BeginPrimitives2D: Nesting detected."
		"\n  It is illegal to nest Begin/EndPrimitives.");
  // Not actually required for 2D operations but some drivers do an
  // initial transformation...
  fpObjectTransformation = &fIdentityTransformation;
}

void G4VSceneHandler::EndPrimitives2D () {
  if (fNestingDepth <= 0)
    G4Exception("G4VSceneHandler::EndPrimitives2D: Nesting error");
  fNestingDepth--;
  fpObjectTransformation = 0;
  if (fReadyForTransients) {
    fTransientsDrawnThisEvent = true;
    fTransientsDrawnThisRun = true;
  }
}

void G4VSceneHandler::BeginModeling () {
}

void G4VSceneHandler::EndModeling ()
{
  fpModel = 0;
}

void G4VSceneHandler::ClearStore () {
  // if (fpViewer) fpViewer -> NeedKernelVisit (true);
  // ?? Viewer is supposed to be smart enough to know when to visit
  // kernel, but a problem in OpenGL Stored seems to require a forced
  // kernel visit triggered by the above code.  John Allison Aug 2001
  // Feb 2005 - commented out.  Let's fix OpenGL if necessary.
}

void G4VSceneHandler::ClearTransientStore () {
}

void G4VSceneHandler::AddSolid (const G4Box& box) {
  RequestPrimitives (box);
// If your graphics system is sophisticated enough to handle a
//  particular solid shape as a primitive, in your derived class write a
//  function to override this.  (Note: some compilers warn that your
//  function "hides" this one.  That's OK.)
// Your function might look like this...
// void G4MyScene::AddSolid (const G4Box& box) {
// Get parameters of appropriate object, e.g.:
//   G4double dx = box.GetXHalfLength ();
//   G4double dy = box.GetYHalfLength ();
//   G4double dz = box.GetZHalfLength ();
// and Draw or Store in your display List.
}

void G4VSceneHandler::AddSolid (const G4Tubs& tubs) {
  RequestPrimitives (tubs);
}

void G4VSceneHandler::AddSolid (const G4Cons& cons) {
  RequestPrimitives (cons);
}

void G4VSceneHandler::AddSolid (const G4Trd& trd) {
  RequestPrimitives (trd);
}

void G4VSceneHandler::AddSolid (const G4Trap& trap) {
  RequestPrimitives (trap);
}

void G4VSceneHandler::AddSolid (const G4Sphere& sphere) {
  RequestPrimitives (sphere );
}

void G4VSceneHandler::AddSolid (const G4Para& para) {
  RequestPrimitives (para);
}

void G4VSceneHandler::AddSolid (const G4Torus& torus) {
  RequestPrimitives (torus);
}

void G4VSceneHandler::AddSolid (const G4Polycone& polycone) {
  RequestPrimitives (polycone);
}

void G4VSceneHandler::AddSolid (const G4Polyhedra& polyhedra) {
  RequestPrimitives (polyhedra);
}

void G4VSceneHandler::AddSolid (const G4VSolid& solid) {
  RequestPrimitives (solid);
}

void G4VSceneHandler::AddCompound (const G4VTrajectory& traj) {
  G4TrajectoriesModel* pTrModel =
    dynamic_cast<G4TrajectoriesModel*>(fpModel);
  if (!pTrModel) G4Exception
    ("G4VSceneHandler::AddCompound(const G4VTrajectory&): Not a G4TrajectoriesModel.");
  traj.DrawTrajectory(pTrModel->GetDrawingMode());
}

void G4VSceneHandler::AddCompound (const G4VHit& hit) {
  ((G4VHit&)hit).Draw(); // Cast to non-const because Draw is non-const!!!!
}

void G4VSceneHandler::AddViewerToList (G4VViewer* pViewer) {
  fViewerList.push_back (pViewer);
}

void G4VSceneHandler::AddPrimitive (const G4Scale& scale) {

  const G4double margin(0.01);
  // Fractional margin - ensures scale is comfortably inside viewing
  // volume.
  const G4double oneMinusMargin (1. - margin);

  const G4VisExtent& sceneExtent = fpScene->GetExtent();

  // Useful constants...
  const G4double length(scale.GetLength());
  const G4double halfLength(length / 2.);
  const G4double tickLength(length / 20.);
  const G4double piBy2(halfpi);

  // Get size of scene...
  const G4double xmin = sceneExtent.GetXmin();
  const G4double xmax = sceneExtent.GetXmax();
  const G4double ymin = sceneExtent.GetYmin();
  const G4double ymax = sceneExtent.GetYmax();
  const G4double zmin = sceneExtent.GetZmin();
  const G4double zmax = sceneExtent.GetZmax();

  // Create (empty) polylines having the same vis attributes...
  G4Polyline scaleLine, tick11, tick12, tick21, tick22;
  G4VisAttributes visAtts(*scale.GetVisAttributes());  // Long enough life.
  scaleLine.SetVisAttributes(&visAtts);
  tick11.SetVisAttributes(&visAtts);
  tick12.SetVisAttributes(&visAtts);
  tick21.SetVisAttributes(&visAtts);
  tick22.SetVisAttributes(&visAtts);

  // Add points to the polylines to represent an scale parallel to the
  // x-axis centred on the origin...
  G4Point3D r1(G4Point3D(-halfLength, 0., 0.));
  G4Point3D r2(G4Point3D( halfLength, 0., 0.));
  scaleLine.push_back(r1);
  scaleLine.push_back(r2);
  G4Point3D ticky(0., tickLength, 0.);
  G4Point3D tickz(0., 0., tickLength);
  tick11.push_back(r1 + ticky);
  tick11.push_back(r1 - ticky);
  tick12.push_back(r1 + tickz);
  tick12.push_back(r1 - tickz);
  tick21.push_back(r2 + ticky);
  tick21.push_back(r2 - ticky);
  tick22.push_back(r2 + tickz);
  tick22.push_back(r2 - tickz);
  G4Point3D textPosition(0., tickLength, 0.);

  // Transform appropriately...

  G4Transform3D transformation;
  if (scale.GetAutoPlacing()) {
    G4Transform3D rotation;
    switch (scale.GetDirection()) {
    case G4Scale::x:
      break;
    case G4Scale::y:
      rotation = G4RotateZ3D(piBy2);
      break;
    case G4Scale::z:
      rotation = G4RotateY3D(piBy2);
      break;
    }
    G4double sxmid(scale.GetXmid());
    G4double symid(scale.GetYmid());
    G4double szmid(scale.GetZmid());
    sxmid = xmin + oneMinusMargin * (xmax - xmin);
    symid = ymin + margin * (ymax - ymin);
    szmid = zmin + oneMinusMargin * (zmax - zmin);
    switch (scale.GetDirection()) {
    case G4Scale::x:
      sxmid -= halfLength;
      break;
    case G4Scale::y:
      symid += halfLength;
      break;
    case G4Scale::z:
      szmid -= halfLength;
      break;
    }
    G4Translate3D translation(sxmid, symid, szmid);
    transformation = translation * rotation;
  } else {
    if (fpModel) transformation = fpModel->GetTransformation();
  }

  // Draw...
  // We would like to call BeginPrimitives(transformation) here but
  // calling BeginPrimitives from within an AddPrimitive is not
  // allowed!  So we have to do our own transformation...
  AddPrimitive(scaleLine.transform(transformation));
  AddPrimitive(tick11.transform(transformation));
  AddPrimitive(tick12.transform(transformation));
  AddPrimitive(tick21.transform(transformation));
  AddPrimitive(tick22.transform(transformation));
  G4Text text(scale.GetAnnotation(),textPosition.transform(transformation));
  text.SetScreenSize(12.);
  AddPrimitive(text);
}

void G4VSceneHandler::AddPrimitive (const G4Polymarker& polymarker) {
  switch (polymarker.GetMarkerType()) {
  default:
  case G4Polymarker::dots:
    {
      for (size_t iPoint = 0; iPoint < polymarker.size (); iPoint++) {
	G4Circle dot (polymarker);
        dot.SetPosition (polymarker[iPoint]);
	dot.SetWorldSize  (0.);
	dot.SetScreenSize (0.1);  // Very small circle.
	AddPrimitive (dot);
      }
    }
    break;
  case G4Polymarker::circles:
    {
      for (size_t iPoint = 0; iPoint < polymarker.size (); iPoint++) {
	G4Circle circle (polymarker);
	circle.SetPosition (polymarker[iPoint]);
	AddPrimitive (circle);
      }
    }
    break;
  case G4Polymarker::squares:
    {
      for (size_t iPoint = 0; iPoint < polymarker.size (); iPoint++) {
	G4Square square (polymarker);
	square.SetPosition (polymarker[iPoint]);
	AddPrimitive (square);
      }
    }
    break;
  }
}

void G4VSceneHandler::RemoveViewerFromList (G4VViewer* pViewer) {
  fViewerList.remove(pViewer);
}

void G4VSceneHandler::SetScene (G4Scene* pScene) {
  fpScene = pScene;
  // Notify all viewers that a kernel visit is required.
  G4ViewerListIterator i;
  for (i = fViewerList.begin(); i != fViewerList.end(); i++) {
    (*i) -> SetNeedKernelVisit (true);
  }
}

void G4VSceneHandler::RequestPrimitives (const G4VSolid& solid) {
  BeginPrimitives (*fpObjectTransformation);
  G4NURBS* pNURBS = 0;
  G4Polyhedron* pPolyhedron = 0;
  const G4VisAttributes* pVisAttribs =
    fpViewer -> GetApplicableVisAttributes (fpVisAttribs);
  switch (fpViewer -> GetViewParameters () . GetRepStyle ()) {
  case G4ViewParameters::nurbs:
    pNURBS = solid.CreateNURBS ();
    if (pNURBS) {
      pNURBS -> SetVisAttributes
	(fpViewer -> GetApplicableVisAttributes (pVisAttribs));
      AddPrimitive (*pNURBS);
      delete pNURBS;
      break;
    }
    else {
      G4VisManager::Verbosity verbosity =
	G4VisManager::GetInstance()->GetVerbosity();
      if (verbosity >= G4VisManager::errors) {
	G4cout <<
	  "ERROR: G4VSceneHandler::RequestPrimitives"
	  "\n  NURBS not available for "
	       << solid.GetName () << G4endl;
	G4cout << "Trying polyhedron." << G4endl;
      }
    }
    // Dropping through to polyhedron...
  case G4ViewParameters::polyhedron:
  default:
    G4Polyhedron::SetNumberOfRotationSteps (GetNoOfSides (pVisAttribs));
    pPolyhedron = solid.GetPolyhedron ();
    G4Polyhedron::ResetNumberOfRotationSteps ();
    if (pPolyhedron) {
      pPolyhedron -> SetVisAttributes (pVisAttribs);
      AddPrimitive (*pPolyhedron);
    }
    else {
      G4VisManager::Verbosity verbosity =
	G4VisManager::GetInstance()->GetVerbosity();
      if (verbosity >= G4VisManager::errors) {
	G4cout <<
	  "ERROR: G4VSceneHandler::RequestPrimitives"
	  "\n  Polyhedron not available for " << solid.GetName () <<
	  ".\n  This means it cannot be visualized on most systems."
	  "\n  Contact the Visualization Coordinator." << G4endl;
      }
    }
    break;
  }
  EndPrimitives ();
}

void G4VSceneHandler::ProcessScene (G4VViewer&) {

  if (!fpScene) return;

  G4VisManager* visManager = G4VisManager::GetInstance();
  G4VisManager::Verbosity verbosity = visManager->GetVerbosity();

  fReadyForTransients = false;

  // Clear stored scene, if any, i.e., display lists, scene graphs.
  ClearStore ();

  // Reset fMarkForClearingTransientStore.  No need to clear transient
  // store since it has just been cleared above.  (Leaving
  // fMarkForClearingTransientStore true causes problems with
  // recomputing transients below.)  Restore it again at end...
  G4bool tmpMarkForClearingTransientStore = fMarkForClearingTransientStore;
  fMarkForClearingTransientStore = false;

  // Traverse geometry tree and send drawing primitives to window(s).

  const std::vector<G4VModel*>& runDurationModelList =
    fpScene -> GetRunDurationModelList ();

  if (runDurationModelList.size ()) {
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Traversing scene data..." << G4endl;
    }

    BeginModeling ();

    // Create modeling parameters from view parameters...
    G4ModelingParameters* pMP = CreateModelingParameters ();

    for (size_t i = 0; i < runDurationModelList.size (); i++) {
      G4VModel* pModel = runDurationModelList[i];
      // Note: this is not the place to take action on
      // pModel->GetTransformation().  The model must take care of
      // this in pModel->DescribeYourselfTo(*this).  See, for example,
      // G4PhysicalVolumeModel and /vis/scene/add/logo.
      pModel -> SetModelingParameters (pMP);
      SetModel (pModel);  // Store for use by derived class.
      pModel -> DescribeYourselfTo (*this);
      pModel -> SetModelingParameters (0);
    }

    // Repeat if required...
    if (fSecondPassRequested) {
      fSecondPass = true;
      for (size_t i = 0; i < runDurationModelList.size (); i++) {
	G4VModel* pModel = runDurationModelList[i];
	pModel -> SetModelingParameters (pMP);
	SetModel (pModel);  // Store for use by derived class.
	pModel -> DescribeYourselfTo (*this);
	pModel -> SetModelingParameters (0);
      }
      fSecondPass = false;
      fSecondPassRequested = false;
    }

    delete pMP;
    EndModeling ();

  }

  fpViewer->FinishView();  // Flush streams and/or swap buffers.

  fReadyForTransients = true;

  // Now (re-)do transients (trajectories, hits, user drawing, etc.)...
  if (fpScene->GetRecomputeTransients()) {  // ...if requested...

    // If reprocessing, allowed only in idle state...
    if (visManager->fReprocessing) {
      G4StateManager* stateManager = G4StateManager::GetStateManager();
      G4ApplicationState currentState = stateManager->GetCurrentState();
      if(currentState != G4State_Idle) {
	if (verbosity >= G4VisManager::warnings) {
	  G4cout << 
	    "Cannot refresh event during existing run."
	    "\n  Trajectories, etc., will be lost."
		 << G4endl;
	}
      }
    } else {

      visManager->SetReprocessing(true);

      // BeamOn way...
      // Uses run manager via UImanager->ApplyCommand("/run/beamOn") so
      // only makes sense if a run manager exists.  More than that - the
      // random number status strings are created by G4RunManager, so they
      // are null if a G4RunManager does not exist...
      G4RunManager* runManager = G4RunManager::GetRunManager();
      if (runManager) {
	if (visManager->GetEventCount()) {  // Prior event(s)...
	  if (fpScene->GetRefreshAtEndOfEvent()) {
	    // Check if transients have been drawn.  Note: Use the flag in
	    // the vis manager, which is set *after* a pass triggered by
	    // ClearTransientStoreIfMarked.  Avoids early processing.
	    if (visManager->GetTransientsDrawnThisEvent()) {
	      // Change "warnings" to "confirmations" if this settles!!!!!!!!!!
	      if (verbosity >= G4VisManager::warnings) {
		G4cout << 
		  "Recomputing transients generated by previous event..."
		  "\n  \"/vis/scene/transientsAction none\" to suppress."
		       << G4endl;
	      }
	      std::istringstream
		iss(visManager->GetBeginOfLastEventRandomStatus());
	      CLHEP::HepRandom::restoreFullState(iss);
	      visManager->SetReprocessing(true);
	      visManager->SetReprocessingLastEvent(true);
	      G4int runID = visManager->GetLastRunID();
	      runManager->SetRunIDCounter(runID);
	      runManager->BeamOn(1);
	    }
	  } else {
	    // Check if transients have been drawn.  Note: Use the flag in
	    // the vis manager, which is set *after* a pass triggered by
	    // ClearTransientStoreIfMarked.  Avoids early processing.
	    if (visManager->GetTransientsDrawnThisRun()) {
	      // Change "warnings" to "confirmations" if this settles!!!!!!!!!!
	      if (verbosity >= G4VisManager::warnings) {
		G4cout << 
		  "Recomputing transients generated by previous run..."
		  "\n  \"/vis/scene/transientsAction none\" to suppress."
		       << G4endl;
	      }
	      std::istringstream iss(visManager->GetBeginOfLastRunRandomStatus());
	      CLHEP::HepRandom::restoreFullState(iss);
	      G4int nEvents = visManager->GetEventCount();
	      visManager->SetReprocessing(true);
	      G4int runID = visManager->GetLastRunID();
	      runManager->SetRunIDCounter(runID);
	      runManager->BeamOn(nEvents);
	      if (!fpScene->GetRefreshAtEndOfRun()) {
		if (verbosity >= G4VisManager::warnings) {
		  G4cout <<
    "WARNING: Cannot refresh trajectories, etc., accumulated over more"
    "\n  than one runs.  Refreshed just the last run..."
			 << G4endl;
		}
	      }
	    }
	  }
	}
      }  // End of BeamOn way.

      /* Event copying way...
      G4StateManager* stateManager = G4StateManager::GetStateManager();
      G4EventManager* eventManager = G4EventManager::GetEventManager();
      if (fpScene->GetRefreshAtEndOfEvent()) {
	// Check if transients have been drawn.  Note: Use the flag in
	// the vis manager, which is set *after* a pass triggered by
	// ClearTransientStoreIfMarked.  Avoids early processing.
	if (visManager->GetTransientsDrawnThisEvent()) {
	  // Change "warnings" to "confirmations" if this settles!!!!!!!!!!
	  if (verbosity >= G4VisManager::warnings) {
	    G4cout << 
	      "Recomputing transients generated by previous event..."
	      "\n  \"/vis/scene/transientsAction none\" to suppress."
		   << G4endl;
	  }
	  std::istringstream iss(visManager->GetLastEventRandomStatus());
	  G4Event* event = visManager->GetLastEvent();
	  assert(event);  // Should always be non-zero.
	  CLHEP::HepRandom::restoreFullState(iss);
	  stateManager->SetNewState(G4State_GeomClosed);
	  eventManager->ProcessOneEvent(event);
	  stateManager->SetNewState(G4State_Idle);
	}
      } else {
	// Check if transients have been drawn.  Note: Use the flag in
	// the vis manager, which is set *after* a pass triggered by
	// ClearTransientStoreIfMarked.  Avoids early processing.
	if (visManager->GetTransientsDrawnThisRun()) {
	  // Change "warnings" to "confirmations" if this settles!!!!!!!!!!
	  if (verbosity >= G4VisManager::warnings) {
	    G4cout << 
	      "Recomputing transients generated by previous run..."
	      "\n  \"/vis/scene/transientsAction none\" to suppress."
		   << G4endl;
	  }
	  stateManager->SetNewState(G4State_GeomClosed);
	  std::vector<G4Event*>& events = visManager->fEvents;
	  assert(events.size());  // Should always be non-zero.
	  for (size_t i = 0; i < events.size(); ++i) {
	    std::istringstream iss(visManager->GetEventsRandomStatus()[i]);
	    G4Event* event = visManager->GetEvents()[i];
	    CLHEP::HepRandom::restoreFullState(iss);
	    eventManager->ProcessOneEvent(event);
	  }
	  stateManager->SetNewState(G4State_Idle);
	  if (!fpScene->GetRefreshAtEndOfRun()) {
	    if (verbosity >= G4VisManager::warnings) {
	      G4cout <<
     "WARNING: Cannot refresh trajectories, etc., accumulated over more"
     "\n  than one runs.  Refreshed just the last run..."
		     << G4endl;
	    }
	  }
	}
      } // End event copying way. */

      /* Event refreshing way...
      G4RunManager* runManager = G4RunManager::GetRunManager();
      if (fpScene->GetRefreshAtEndOfEvent()) {
	// Check if transients have been drawn.  Note: Use the flag in
	// the vis manager, which is set *after* a pass triggered by
	// ClearTransientStoreIfMarked.  Avoids early processing.
	if (visManager->GetTransientsDrawnThisEvent()) {
	  // Change "warnings" to "confirmations" if this settles!!!!!!!!!!
	  if (verbosity >= G4VisManager::warnings) {
	    G4cout << 
	      "Refreshing previous event..."
	      "\n  \"/vis/scene/transientsAction none\" to suppress."
		   << G4endl;
	  }
	  const std::vector<G4VModel*>& EOEModelList =
	    fpScene -> GetEndOfEventModelList ();
	  size_t nModels = EOEModelList.size();
	  if (nModels) {
	    const G4Event* event = runManager->GetPreviousEvent(1);
	    assert(event);  // Should always be non-zero.
	    G4ModelingParameters* pMP = CreateModelingParameters();
	    //pMP->SetEvent(event);  // Not implemented yet!!!
	    for (size_t i = 0; i < nModels; i++) {
	      G4VModel* pModel = EOEModelList[i];
	      pModel -> SetModelingParameters(pMP);
	      SetModel (pModel);  // Store for use by derived class.
	      pModel -> DescribeYourselfTo(*this);
	      pModel -> SetModelingParameters(0);
	    }
	    delete pMP;
	  }
	}
      } else {
	// Check if transients have been drawn.  Note: Use the flag in
	// the vis manager, which is set *after* a pass triggered by
	// ClearTransientStoreIfMarked.  Avoids early processing.
	if (visManager->GetTransientsDrawnThisRun()) {
	  // Change "warnings" to "confirmations" if this settles!!!!!!!!!!
	  if (verbosity >= G4VisManager::warnings) {
	    G4cout << 
	      "Refreshing events generated by previous run..."
	      "\n  \"/vis/scene/transientsAction none\" to suppress."
		   << G4endl;
	  }
	  size_t nEvents = visManager->GetEventCount();
	  assert(nEvents);  // Should always be non-zero.
	  G4ModelingParameters* pMP = CreateModelingParameters();
	  for (size_t i = 0; i < nEvents; ++i) {
	    const G4Event* event = runManager->GetPreviousEvent(nEvents - i);
	    assert(event);  // Should always be non-zero.
	    //pMP->SetEvent(event);  // Not implemented yet!!!
	    const std::vector<G4VModel*>& EOEModelList =
	      fpScene -> GetEndOfEventModelList ();
	    size_t nModels = EOEModelList.size();
	    for (size_t i = 0; i < nModels; i++) {
	      G4VModel* pModel = EOEModelList[i];
	      pModel -> SetModelingParameters(pMP);
	      SetModel (pModel);  // Store for use by derived class.
	      pModel -> DescribeYourselfTo(*this);
	      pModel -> SetModelingParameters(0);
	    }
	  }
	  delete pMP;
	  if (!fpScene->GetRefreshAtEndOfRun()) {
	    if (verbosity >= G4VisManager::warnings) {
	      G4cout <<
     "WARNING: Cannot refresh events accumulated over more"
     "\n  than one runs.  Refreshed just the last run..."
		     << G4endl;
	    }
	  }
	}
      } // End event refreshing way. */

    }  // End else if (visManager->fReprocessing)

    visManager->SetReprocessing(false);

  }  // End if (fpScene->GetRecomputeTransients())

  fMarkForClearingTransientStore = tmpMarkForClearingTransientStore;
}

G4ModelingParameters* G4VSceneHandler::CreateModelingParameters ()
{
  // Create modeling parameters from View Parameters...
  const G4ViewParameters& vp = fpViewer -> GetViewParameters ();

  // Convert drawing styles...
  G4ModelingParameters::DrawingStyle modelDrawingStyle =
    G4ModelingParameters::wf;
  switch (vp.GetDrawingStyle ()) {
  default:
  case G4ViewParameters::wireframe:
    modelDrawingStyle = G4ModelingParameters::wf;
    break;
  case G4ViewParameters::hlr:
    modelDrawingStyle = G4ModelingParameters::hlr;
    break;
  case G4ViewParameters::hsr:
    modelDrawingStyle = G4ModelingParameters::hsr;
    break;
  case G4ViewParameters::hlhsr:
    modelDrawingStyle = G4ModelingParameters::hlhsr;
    break;
  }

  // Decide if covered daughters are really to be culled...
  G4bool reallyCullCovered =
    vp.IsCullingCovered()   // Culling daughters depends also on...
    && !vp.IsSection ()     // Sections (DCUT) not requested.
    && !vp.IsCutaway ()     // Cutaways not requested.
    ;

  G4ModelingParameters* pModelingParams = new G4ModelingParameters
    (vp.GetDefaultVisAttributes (),
     modelDrawingStyle,
     vp.IsCulling (),
     vp.IsCullingInvisible (),
     vp.IsDensityCulling (),
     vp.GetVisibleDensity (),
     reallyCullCovered,
     vp.GetNoOfSides ()
     );

  pModelingParams->SetWarning
    (G4VisManager::GetInstance()->GetVerbosity() >= G4VisManager::warnings);

  pModelingParams->SetExplodeFactor(vp.GetExplodeFactor());
  pModelingParams->SetExplodeCentre(vp.GetExplodeCentre());

  pModelingParams->SetSectionPolyhedron(CreateSectionPolyhedron());
  pModelingParams->SetCutawayPolyhedron(CreateCutawayPolyhedron());
  // The polyhedron objects are deleted in the modeling parameters destructor.

  return pModelingParams;
}

const G4Polyhedron* G4VSceneHandler::CreateSectionPolyhedron()
{
  /* Disable for now.  Boolean processor not up to it.
  const G4ViewParameters& vp = fpViewer->GetViewParameters();
  if (vp.IsSection () ) {
    G4double radius = fpScene->GetExtent().GetExtentRadius();
    G4double safe = radius + fpScene->GetExtent().GetExtentCentre().mag();
    G4Box sectionBox("clipper",
		     safe, safe, 1.e-5 * radius);  // Thin in z-plane.
    G4Polyhedron* sectioner = sectionBox.CreatePolyhedron();
    const G4Plane3D& s = vp.GetSectionPlane ();
    G4double a = s.a();
    G4double b = s.b();
    G4double c = s.c();
    G4double d = s.d();
    G4Transform3D transform = G4TranslateZ3D(-d);
    const G4Normal3D normal(a,b,c);
    if (normal != G4Normal3D(0,0,1)) {
      const G4double angle = std::acos(normal.dot(G4Normal3D(0,0,1)));
      const G4Vector3D axis = G4Normal3D(0,0,1).cross(normal);
      transform = G4Rotate3D(angle, axis) * transform;
    }
    sectioner->Transform(transform);
    return sectioner;
  } else {
    return 0;
  }
  */
  return 0;
}

const G4Polyhedron* G4VSceneHandler::CreateCutawayPolyhedron()
{
  return 0;
}

const G4Colour& G4VSceneHandler::GetColour (const G4Visible& visible) {
  // Colour is determined by the applicable vis attributes.
  const G4Colour& colour = fpViewer ->
    GetApplicableVisAttributes (visible.GetVisAttributes ()) -> GetColour ();
  return colour;
}

const G4Colour& G4VSceneHandler::GetTextColour (const G4Text& text) {
  const G4VisAttributes* pVA = text.GetVisAttributes ();
  if (!pVA) {
    pVA = fpViewer -> GetViewParameters (). GetDefaultTextVisAttributes ();
  }
  const G4Colour& colour = pVA -> GetColour ();
  return colour;
}

G4double G4VSceneHandler::GetLineWidth(const G4Visible& visible)
{
  G4double lineWidth = fpViewer->
    GetApplicableVisAttributes(visible.GetVisAttributes())->GetLineWidth();
  if (lineWidth < 1.) lineWidth = 1.;
  lineWidth *= fpViewer -> GetViewParameters().GetGlobalLineWidthScale();
  if (lineWidth < 1.) lineWidth = 1.;
  return lineWidth;
}

G4ViewParameters::DrawingStyle G4VSceneHandler::GetDrawingStyle
(const G4VisAttributes* pVisAttribs) {
  // Drawing style is normally determined by the view parameters, but
  // it can be overriddden by the ForceDrawingStyle flag in the vis
  // attributes.
  G4ViewParameters::DrawingStyle style =
    fpViewer->GetViewParameters().GetDrawingStyle();
  if (pVisAttribs -> IsForceDrawingStyle ()) {
    G4VisAttributes::ForcedDrawingStyle forcedStyle =
      pVisAttribs -> GetForcedDrawingStyle ();
    // This is complicated because if hidden line and surface removal
    // has been requested we wish to preserve this sometimes.
    switch (forcedStyle) {
    case (G4VisAttributes::solid):
      switch (style) {
      case (G4ViewParameters::hlr):
	style = G4ViewParameters::hlhsr;
	break;
      case (G4ViewParameters::wireframe):
	style = G4ViewParameters::hsr;
	break;
      case (G4ViewParameters::hlhsr):
      case (G4ViewParameters::hsr):
      default:
	break;
      }	
      break;
    case (G4VisAttributes::wireframe):
    default:
      // But if forced style is wireframe, do it, because one of its
      // main uses is in displaying the consituent solids of a Boolean
      // solid and their surfaces overlap with the resulting Booean
      // solid, making a mess if hlr is specified.
      style = G4ViewParameters::wireframe;
      break;
    }
  }
  return style;
}

G4bool G4VSceneHandler::GetAuxEdgeVisible (const G4VisAttributes* pVisAttribs) {
  G4bool isAuxEdgeVisible = fpViewer->GetViewParameters().IsAuxEdgeVisible ();
  if (pVisAttribs -> IsForceAuxEdgeVisible()) isAuxEdgeVisible = true;
  return isAuxEdgeVisible;
}

G4double G4VSceneHandler::GetMarkerSize
(const G4VMarker& marker, 
 G4VSceneHandler::MarkerSizeType& markerSizeType)
{
  G4bool userSpecified = marker.GetWorldSize() || marker.GetScreenSize();
  const G4VMarker& defaultMarker =
    fpViewer -> GetViewParameters().GetDefaultMarker();
  G4double size = userSpecified ?
    marker.GetWorldSize() : defaultMarker.GetWorldSize();
  if (size) {
    // Draw in world coordinates.
    markerSizeType = world;
  }
  else {
    size = userSpecified ?
      marker.GetScreenSize() : defaultMarker.GetScreenSize();
    // Draw in screen coordinates.
    markerSizeType = screen;
  }
  if (size <= 1.) size = 1.;
  size *= fpViewer -> GetViewParameters().GetGlobalMarkerScale();
  if (size <= 1.) size = 1.;
  return size;
}

G4int G4VSceneHandler::GetNoOfSides(const G4VisAttributes* pVisAttribs)
{
  // No. of sides (lines segments per circle) is normally determined
  // by the view parameters, but it can be overriddden by the
  // ForceLineSegmentsPerCircle in the vis attributes.
  G4int lineSegmentsPerCircle = fpViewer->GetViewParameters().GetNoOfSides();
  if (pVisAttribs->GetForcedLineSegmentsPerCircle() > 0)
    lineSegmentsPerCircle = pVisAttribs->GetForcedLineSegmentsPerCircle();
  const G4int nSegmentsMin = 12;
  if (lineSegmentsPerCircle < nSegmentsMin) {
    lineSegmentsPerCircle = nSegmentsMin;
    G4cout <<
      "G4VSceneHandler::GetNoOfSides: attempt to set the"
      "\nnumber of line segements per circle < " << nSegmentsMin
         << "; forced to " << lineSegmentsPerCircle << G4endl;
  }
  return lineSegmentsPerCircle;
}

std::ostream& operator << (std::ostream& os, const G4VSceneHandler& s) {

  os << "Scene handler " << s.fName << " has "
     << s.fViewerList.size () << " viewer(s):";
  for (size_t i = 0; i < s.fViewerList.size (); i++) {
    os << "\n  " << *(s.fViewerList [i]);
  }

  if (s.fpScene) {
    os << "\n  " << *s.fpScene;
  }
  else {
    os << "\n  This scene handler currently has no scene.";
  }

  return os;
}

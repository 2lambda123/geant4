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
// $Id: G4VSceneHandler.cc,v 1.51 2006-02-09 16:32:42 allison Exp $
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
#include "G4UImanager.hh"
#include "Randomize.hh"

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
  fSecondPassRequested   (false),
  fSecondPass            (false),
  fReadyForTransients    (true),  // Only false while processing scene.
  fpModel                (0),
  fpObjectTransformation (0),
  fNestingDepth          (0),
  fpVisAttribs           (0),
  fCurrentDepth          (0),
  fpCurrentPV            (0),
  fpCurrentLV            (0)
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
}

G4VSceneHandler::~G4VSceneHandler () {
  G4ViewerListIterator i;
  for (i = fViewerList.begin(); i != fViewerList.end(); ++i) {
    delete *i;
  }
}

void G4VSceneHandler::EndModeling () {}

void G4VSceneHandler::PreAddSolid (const G4Transform3D& objectTransformation,
                                  const G4VisAttributes& visAttribs) {
  fpObjectTransformation = &objectTransformation;
  fpVisAttribs = &visAttribs;
}

void G4VSceneHandler::PostAddSolid () {
  fpObjectTransformation = 0;
  fpVisAttribs = 0;
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
  traj.DrawTrajectory(((G4TrajectoriesModel*)fpModel)->GetDrawingMode());
}

void G4VSceneHandler::AddCompound (const G4VHit& hit) {
  ((G4VHit&)hit).Draw(); // Cast to non-const because Draw is non-const!!!!
}

void G4VSceneHandler::AddViewerToList (G4VViewer* pViewer) {
  fViewerList.push_back (pViewer);
}

void G4VSceneHandler::EstablishSpecials (G4PhysicalVolumeModel& pvModel) {
  pvModel.DefinePointersToWorkingSpace (&fCurrentDepth,
					&fpCurrentPV,
					&fpCurrentLV,
					&fpCurrentMaterial);
}

void G4VSceneHandler::BeginModeling () {
}

void G4VSceneHandler::BeginPrimitives
(const G4Transform3D& objectTransformation) {
  if (!fpModel) G4Exception ("G4VSceneHandler::BeginPrimitives: NO MODEL!!!");
  fpObjectTransformation = &objectTransformation;
  fNestingDepth++;
  if (fNestingDepth > 1)
    G4Exception("G4VSceneHandler::BeginPrimitives: Nesting detected."
		"\n  It is illegal to nest Begin/EndPrimitives.");
}

void G4VSceneHandler::EndPrimitives () {
  if (fNestingDepth <= 0)
    G4Exception("G4VSceneHandler::EndPrimitives: Nesting error");
  fNestingDepth--;
  fpObjectTransformation = 0;
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
	G4Square Square (polymarker);
	Square.SetPosition (polymarker[iPoint]);
	AddPrimitive (Square);
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
  if (!fpModel)
    G4Exception ("G4VSceneHandler::RequestPrimitives: NO MODEL!!!");
  G4Polyhedron* pPolyhedron;
  G4NURBS*      pNURBS;
  BeginPrimitives (*fpObjectTransformation);
  switch (fpModel -> GetModelingParameters () -> GetRepStyle ()) {
  case G4ModelingParameters::nurbs:
    pNURBS = solid.CreateNURBS ();
    if (pNURBS) {
      pNURBS -> SetVisAttributes
	(fpViewer -> GetApplicableVisAttributes (fpVisAttribs));
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
  case G4ModelingParameters::polyhedron:
  default:
    G4Polyhedron::SetNumberOfRotationSteps
	(fpModel -> GetModelingParameters () -> GetNoOfSides ());
    pPolyhedron = solid.GetPolyhedron ();
    G4Polyhedron::ResetNumberOfRotationSteps ();
    if (pPolyhedron) {
      pPolyhedron -> SetVisAttributes
	(fpViewer -> GetApplicableVisAttributes (fpVisAttribs));
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

  fReadyForTransients = false;

  // Clear stored scene, if any, i.e., display lists, scene graphs.
  ClearStore ();

  // Traverse geometry tree and send drawing primitives to window(s).

  const std::vector<G4VModel*>& runDurationModelList =
    fpScene -> GetRunDurationModelList ();

  if (runDurationModelList.size ()) {
    G4VisManager::Verbosity verbosity =
      G4VisManager::GetInstance()->GetVerbosity();
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
      const G4ModelingParameters* tempMP =
	pModel -> GetModelingParameters ();
      // NOTE THAT pModel->GetModelingParameters() COULD BE ZERO.
      // (Not sure the above is necessary; but in future we might
      // want to take notice of the modeling parameters with which
      // the model was created.  For the time being we are ignoring
      // them and simply using the view parameters.  When the time
      // comes to do this, then perhaps there should be a default
      // set of modeling parameters in the view parameters for the
      // case of a zero modeling parameters pointer.)
      // (I think for the G4 Vis System we'll rely on view parameters
      // and convert using pMP = CreateModelingParameters () as above.)
      pModel -> SetModelingParameters (pMP);
      SetModel (pModel);  // Store for use by derived class.
      pModel -> DescribeYourselfTo (*this);
      pModel -> SetModelingParameters (tempMP);
    }

    // Repeat if required...
    if (fSecondPassRequested) {
      fSecondPass = true;
      for (size_t i = 0; i < runDurationModelList.size (); i++) {
	G4VModel* pModel = runDurationModelList[i];
	const G4ModelingParameters* tempMP =
	  pModel -> GetModelingParameters ();
	pModel -> SetModelingParameters (pMP);
	SetModel (pModel);  // Store for use by derived class.
	pModel -> DescribeYourselfTo (*this);
	pModel -> SetModelingParameters (tempMP);
      }
      fSecondPass = false;
      fSecondPassRequested = false;
    }

    delete pMP;
    SetModel (0);  // Flags invalid model.
    EndModeling ();

  } else {
    G4VisManager::Verbosity verbosity =
      G4VisManager::GetInstance()->GetVerbosity();
    if (verbosity >= G4VisManager::warnings) {
      G4cout <<
	"WARNING: G4VSceneHandler::ProcessScene: No run-duration models in"
	"\n  scene.  \"World\" will be added if you attempt to draw a view"
	     << G4endl;
    }
  }

  fReadyForTransients = true;

  // Now (re-)do trajectories and hits, if requested...
  const std::vector<G4VModel*>& endOfEventModelList =
    fpScene -> GetEndOfEventModelList ();
  if (endOfEventModelList.size() > 0) {
    G4VisManager* visManager = G4VisManager::GetInstance();
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if (fpScene->GetRefreshAtEndOfEvent()) {
      std::istringstream iss(visManager->fBeginOfLastEventRandomStatus);
      CLHEP::HepRandom::restoreFullState(iss);
      UImanager->ApplyCommand("/run/beamOn");
    } else {
      if (!fpScene->GetRefreshAtEndOfRun()) {
	G4cout <<
	  "WARNING: Cannot refresh trajectories, etc., accumulated over runs."
	  "\n  Refreshing just the last run..."
	       << G4endl;
      }
      std::istringstream iss(visManager->fBeginOfLastRunRandomStatus);
      CLHEP::HepRandom::restoreFullState(iss);
      std::ostringstream oss;
      oss << visManager->fEventCount;
      UImanager->ApplyCommand(G4String("/run/beamOn " + oss.str()));
    }
  }

}

G4ModelingParameters* G4VSceneHandler::CreateModelingParameters () {
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

  // Convert rep styles...
  G4ModelingParameters::RepStyle modelRepStyle =
    G4ModelingParameters::wireframe;
  if (vp.GetDrawingStyle () != G4ViewParameters::wireframe) {
    switch (vp.GetRepStyle ()) {
    default:
    case G4ViewParameters::polyhedron:
      modelRepStyle = G4ModelingParameters::polyhedron;
      break;
    case G4ViewParameters::nurbs:
      modelRepStyle = G4ModelingParameters::nurbs;
      break;
    }
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
     modelRepStyle,
     vp.IsCulling (),
     vp.IsCullingInvisible (),
     vp.IsDensityCulling (),
     vp.GetVisibleDensity (),
     reallyCullCovered,
     vp.GetNoOfSides (),
     vp.IsViewGeom (),
     vp.IsViewHits (),
     vp.IsViewDigis ()
     );

  return pModelingParams;
}

const G4Colour& G4VSceneHandler::GetColour (const G4Visible& visible) {
  // Colour is determined by the applicable vis attributes.
  const G4Colour& colour = fpViewer ->
    GetApplicableVisAttributes (visible.GetVisAttributes ()) ->  GetColour ();
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

G4double G4VSceneHandler::GetMarkerSize (const G4VMarker& marker, 
				  G4VSceneHandler::MarkerSizeType& markerSizeType) {
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
  if (size <= 0.) size = 1.;
  size *= fpViewer -> GetViewParameters().GetGlobalMarkerScale();
  return size;
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

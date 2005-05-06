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
// $Id: G4VisCommandsViewerSet.cc,v 1.34 2005-05-06 08:46:50 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

// /vis/viewer/set commands - John Allison  16th May 2000

#include "G4VisCommandsViewerSet.hh"

#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UnitsTable.hh"
#include "G4VisManager.hh"
#include "G4Polyhedron.hh"

G4VisCommandsViewerSet::G4VisCommandsViewerSet ():
  fLightsVector    (G4ThreeVector(1.,1.,1.)),
  fUpVector        (G4ThreeVector(0.,1.,0.)),
  fViewpointVector (G4ThreeVector(0.,0.,1.))
{
  G4bool omitable;
  G4UIparameter* parameter;

  fpCommandAll = new G4UIcmdWithAString ("/vis/viewer/set/all",this);
  fpCommandAll->SetGuidance
    ("Copies view parameters.");
  fpCommandAll->SetGuidance
    ("Copies view parameters (except the autoRefresh status) from"
     "\nfrom-viewer to current viewer.");
  fpCommandAll->SetParameterName ("from-viewer-name",omitable = false);

  fpCommandAutoRefresh = new G4UIcmdWithABool
    ("/vis/viewer/set/autoRefresh",this);
  fpCommandAutoRefresh->SetGuidance("Sets auto-refresh.");
  fpCommandAutoRefresh->SetGuidance
    ("If true, view is automatically refreshed after a change of"
     "\nview parameters.");
  fpCommandAutoRefresh->SetParameterName("auto-refresh",omitable = true);
  fpCommandAutoRefresh->SetDefaultValue(false);

  fpCommandAuxEdge = new G4UIcmdWithABool
    ("/vis/viewer/set/auxiliaryEdge",this);
  fpCommandAuxEdge->SetGuidance("Sets visibility of auxiliary edges");
  fpCommandAuxEdge->SetGuidance
    ("Auxiliary edges, i.e., those that are part of a curved surface,"
     "\nsometimes called soft edges, become visible/invisible.");
  fpCommandAuxEdge->SetParameterName("edge",omitable = true);
  fpCommandAuxEdge->SetDefaultValue(false);

  fpCommandCulling = new G4UIcommand("/vis/viewer/set/culling",this);
  fpCommandCulling->SetGuidance ("Set culling options.");
  fpCommandCulling->SetGuidance
    ("\"global\": enables/disables all other culling options.");
  fpCommandCulling->SetGuidance
    ("\"coveredDaughters\": culls, i.e., eliminates, volumes that would not"
     "\nbe seen because covered by ancester volumes in surface drawing mode,"
     "\nand then only if the ancesters are visible and opaque, and then only"
     "\nif no sections or cutaways are in operation.  Intended solely to"
     "\nimprove the speed of rendering visible volumes.");
  fpCommandCulling->SetGuidance
    ("\"invisible\": culls objects with the invisible attribute set.");
  fpCommandCulling->SetGuidance
    ("\"density\": culls volumes with density lower than threshold.  Useful"
     "\nfor eliminating \"container volumes\" with no physical correspondence,"
     "\nwhose material is usually air.  If this is selected, provide threshold"
     "\ndensity and unit (g/cm3 mg/cm3 or kg/m3)."
     );
  parameter = new G4UIparameter("culling-option",'s',omitable = false);
  parameter->SetParameterCandidates
    ("global coveredDaughters invisible density");
  fpCommandCulling->SetParameter(parameter);
  parameter = new G4UIparameter("action",'b',omitable = true);
  parameter->SetDefaultValue("true");
  fpCommandCulling->SetParameter(parameter);
  parameter = new G4UIparameter("density-threshold",'d',omitable = true);
  parameter->SetDefaultValue("0.01");
  fpCommandCulling->SetParameter(parameter);
  parameter = new G4UIparameter("unit",'s',omitable = true);
  parameter->SetParameterCandidates ("g/cm3, mg/cm3 kg/m3");
  parameter->SetDefaultValue("g/cm3");
  fpCommandCulling->SetParameter(parameter);

  fpCommandEdge = new G4UIcmdWithABool("/vis/viewer/set/edge",this);
  fpCommandEdge->SetGuidance("  default: true");
  fpCommandEdge->SetGuidance
    ("Edges become visible/invisible in surface mode.");
  fpCommandEdge->SetParameterName("edge",omitable = true);
  fpCommandEdge->SetDefaultValue(true);

  fpCommandGlobalMarkerScale = new G4UIcmdWithADouble
    ("/vis/viewer/set/globalMarkerScale", this);
  fpCommandGlobalMarkerScale -> SetGuidance
    ("Multiplies marker sizes by this factor.");
  fpCommandGlobalMarkerScale -> SetParameterName("scale-factorr",
						 omitable=true);
  fpCommandGlobalMarkerScale->SetDefaultValue(1.);

  fpCommandHiddenEdge =
    new G4UIcmdWithABool("/vis/viewer/set/hiddenEdge",this);
  fpCommandHiddenEdge->SetGuidance
    ("Edges become hidden/seen in wireframe or surface mode.");
  fpCommandHiddenEdge->SetParameterName("hidden-edge",omitable = true);
  fpCommandHiddenEdge->SetDefaultValue(true);

  fpCommandHiddenMarker =
    new G4UIcmdWithABool("/vis/viewer/set/hiddenMarker",this);
  fpCommandHiddenMarker->SetGuidance
    ("If true, closer objects hide markers. Otherwise, markers always show.");
  fpCommandHiddenMarker->SetParameterName("hidden-marker",omitable = true);
  fpCommandHiddenMarker->SetDefaultValue(true);

  fpCommandLightsMove = new G4UIcmdWithAString
    ("/vis/viewer/set/lightsMove",this);
  fpCommandLightsMove->SetGuidance
    ("Lights move with camera or with object");
  fpCommandLightsMove->SetParameterName("lightsMove",omitable = false);
  fpCommandLightsMove->SetCandidates
    ("cam camera with-camera obj object with-object");

  fpCommandLightsThetaPhi = new G4UIcommand
    ("/vis/viewer/set/lightsThetaPhi", this);
  fpCommandLightsThetaPhi->SetGuidance
    ("Set direction from target to lights.");
  parameter = new G4UIparameter("theta", 'd', omitable = true);
  parameter -> SetDefaultValue(60.);
  fpCommandLightsThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter("phi", 'd', omitable = true);
  parameter -> SetDefaultValue(45.);
  fpCommandLightsThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("deg");
  fpCommandLightsThetaPhi -> SetParameter (parameter);

  fpCommandLightsVector = new G4UIcommand
    ("/vis/viewer/set/lightsVector", this);
  fpCommandLightsVector->SetGuidance
    ("Set direction from target to lights.");
  parameter = new G4UIparameter("x", 'd', omitable = true);
  parameter -> SetDefaultValue (1);
  fpCommandLightsVector -> SetParameter (parameter);
  parameter = new G4UIparameter("y", 'd', omitable = true);
  parameter -> SetDefaultValue (1);
  fpCommandLightsVector -> SetParameter (parameter);
  parameter = new G4UIparameter ("z", 'd', omitable = true);
  parameter -> SetDefaultValue (1);
  fpCommandLightsVector -> SetParameter (parameter);

  fpCommandLineSegments = new G4UIcmdWithAnInteger
    ("/vis/viewer/set/lineSegmentsPerCircle",this);
  fpCommandLineSegments->SetGuidance
    ("Set number of sides per circle for polygon/polyhedron drawing.");
  fpCommandLineSegments->SetGuidance
 ("Refers to graphical representation of objects with curved lines/surfaces.");
  fpCommandLineSegments->SetParameterName("line-segments",omitable = true);
  fpCommandLineSegments->SetDefaultValue(24);

  fpCommandProjection = new G4UIcommand("/vis/viewer/set/projection",this);
  fpCommandProjection->SetGuidance
    ("Orthogonal or perspective projection.");
  parameter = new G4UIparameter("projection",'s',omitable = true);
  parameter->SetParameterCandidates("o orthogonal p perspective");
  parameter->SetDefaultValue("orthogonal");
  fpCommandProjection->SetParameter(parameter);
  parameter = new G4UIparameter("field-half-angle",'d',omitable = true);
  parameter->SetDefaultValue(30.);
  fpCommandProjection->SetParameter(parameter);
  parameter = new G4UIparameter("unit",'s',omitable = true);
  parameter->SetDefaultValue("deg");
  fpCommandProjection->SetParameter(parameter);

  fpCommandSectionPlane = new G4UIcommand("/vis/viewer/set/sectionPlane",this);
  fpCommandSectionPlane -> SetGuidance
    ("Set plane for drawing section (DCUT).");
  fpCommandSectionPlane -> SetGuidance
    ("E.g., for a y-z plane at x = 1 cm:"
     "\n\"/vis/viewer/set/sectionPlane on 1 0 0 cm 1 0 0\"."
     "\nTo turn off: /vis/viewer/set/sectionPlane off");
  parameter  =  new G4UIparameter("Selector",'c',true);
  parameter  -> SetDefaultValue  ("on");
  fpCommandSectionPlane->SetParameter(parameter);
  parameter  =  new G4UIparameter("x",'d',omitable = true);
  parameter  -> SetDefaultValue  (0);
  parameter  -> SetGuidance      ("Coordinate of point on the plane.");
  fpCommandSectionPlane->SetParameter(parameter);
  parameter  =  new G4UIparameter("y",'d',omitable = true);
  parameter  -> SetDefaultValue  (0);
  parameter  -> SetGuidance      ("Coordinate of point on the plane.");
  fpCommandSectionPlane->SetParameter(parameter);
  parameter  =  new G4UIparameter("z",'d',omitable = true);
  parameter  -> SetDefaultValue  (0);
  parameter  -> SetGuidance      ("Coordinate of point on the plane.");
  fpCommandSectionPlane->SetParameter(parameter);
  parameter  =  new G4UIparameter("unit",'s',omitable = true);
  parameter  -> SetDefaultValue  ("cm");
  parameter  -> SetGuidance      ("Unit of point on the plane.");
  fpCommandSectionPlane->SetParameter(parameter);
  parameter  =  new G4UIparameter("nx",'d',omitable = true);
  parameter  -> SetDefaultValue  (1);
  parameter  -> SetGuidance      ("Component of plane normal.");
  fpCommandSectionPlane->SetParameter(parameter);
  parameter  =  new G4UIparameter("ny",'d',omitable = true);
  parameter  -> SetDefaultValue  (0);
  parameter  -> SetGuidance      ("Component of plane normal.");
  fpCommandSectionPlane->SetParameter(parameter);
  parameter  =  new G4UIparameter("nz",'d',omitable = true);
  parameter  -> SetDefaultValue  (0);
  parameter  -> SetGuidance      ("Component of plane normal.");
  fpCommandSectionPlane->SetParameter(parameter);

  fpCommandStyle = new G4UIcmdWithAString ("/vis/viewer/set/style",this);
  fpCommandStyle->SetGuidance ("Set style of drawing.");
  fpCommandStyle->SetGuidance 
    ("(Hidden line drawing is controlled by \"/vis/viewer/set/hiddenEdge\".)");
  fpCommandStyle->SetParameterName ("style",omitable = false);
  fpCommandStyle->SetCandidates("w wireframe s surface");

  fpCommandUpThetaPhi = new G4UIcommand
    ("/vis/viewer/set/upThetaPhi", this);
  fpCommandUpThetaPhi -> SetGuidance ("Set up vector.");
  fpCommandUpThetaPhi -> SetGuidance
    ("Viewer will attempt always to show this direction upwards.");
  parameter = new G4UIparameter("theta", 'd', omitable = true);
  parameter -> SetDefaultValue (90.);
  fpCommandUpThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter("phi", 'd', omitable = true);
  parameter -> SetDefaultValue (90.);
  fpCommandUpThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("deg");
  fpCommandUpThetaPhi -> SetParameter (parameter);

  fpCommandUpVector = new G4UIcommand
    ("/vis/viewer/set/upVector", this);
  fpCommandUpVector -> SetGuidance ("Set up vector.");
  fpCommandUpVector -> SetGuidance
    ("Viewer will attempt always to show this direction upwards.");
  parameter = new G4UIparameter("x", 'd', omitable = true);
  parameter -> SetDefaultValue (0.);
  fpCommandUpVector -> SetParameter (parameter);
  parameter = new G4UIparameter("y", 'd', omitable = true);
  parameter -> SetDefaultValue (1.);
  fpCommandUpVector -> SetParameter (parameter);
  parameter = new G4UIparameter ("z", 'd', omitable = true);
  parameter -> SetDefaultValue (0.);
  fpCommandUpVector -> SetParameter (parameter);

  fpCommandViewpointThetaPhi = new G4UIcommand
    ("/vis/viewer/set/viewpointThetaPhi", this);
  fpCommandViewpointThetaPhi -> SetGuidance
    ("Set direction from target to camera.");
  fpCommandViewpointThetaPhi -> SetGuidance
  ("Also changes lightpoint direction if lights are set to move with camera.");
  parameter = new G4UIparameter("theta", 'd', omitable = true);
  parameter -> SetDefaultValue (60.);
  fpCommandViewpointThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter("phi", 'd', omitable = true);
  parameter -> SetDefaultValue (45.);
  fpCommandViewpointThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("deg");
  fpCommandViewpointThetaPhi -> SetParameter (parameter);

  fpCommandViewpointVector = new G4UIcommand
    ("/vis/viewer/set/viewpointVector", this);
  fpCommandViewpointVector -> SetGuidance
    ("Set direction from target to camera.");
  fpCommandViewpointVector -> SetGuidance
  ("Also changes lightpoint direction if lights are set to move with camera.");
  parameter = new G4UIparameter("x", 'd', omitable = true);
  parameter -> SetDefaultValue (1.);
  fpCommandViewpointVector -> SetParameter (parameter);
  parameter = new G4UIparameter("y", 'd', omitable = true);
  parameter -> SetDefaultValue (1.);
  fpCommandViewpointVector -> SetParameter (parameter);
  parameter = new G4UIparameter ("z", 'd', omitable = true);
  parameter -> SetDefaultValue (1.);
  fpCommandViewpointVector -> SetParameter (parameter);
}

G4VisCommandsViewerSet::~G4VisCommandsViewerSet() {
  delete fpCommandAll;
  delete fpCommandAuxEdge;
  delete fpCommandAutoRefresh;
  delete fpCommandCulling;
  delete fpCommandEdge;
  delete fpCommandGlobalMarkerScale;
  delete fpCommandHiddenEdge;
  delete fpCommandHiddenMarker;
  delete fpCommandLineSegments;
  delete fpCommandLightsMove;
  delete fpCommandLightsThetaPhi;
  delete fpCommandLightsVector;
  delete fpCommandProjection;
  delete fpCommandSectionPlane;
  delete fpCommandStyle;
  delete fpCommandUpThetaPhi;
  delete fpCommandUpVector;
  delete fpCommandViewpointThetaPhi;
  delete fpCommandViewpointVector;
}

G4String G4VisCommandsViewerSet::GetCurrentValue(G4UIcommand*) {
  return "";
}

void G4VisCommandsViewerSet::SetNewValue
(G4UIcommand* command,G4String newValue) {

  G4VisManager::Verbosity verbosity = fpVisManager->GetVerbosity();

  G4VViewer* currentViewer = fpVisManager->GetCurrentViewer();
  if (!currentViewer) {
    if (verbosity >= G4VisManager::errors) {
      G4cout << 
	"ERROR: G4VisCommandsViewerSet::SetNewValue: no current viewer."
	     << G4endl;
    }
    return;
  }

  G4ViewParameters vp = currentViewer->GetViewParameters();

  if (command == fpCommandAll) {
    G4VViewer* fromViewer = fpVisManager->GetViewer(newValue);
    if (!fromViewer) {
      if (verbosity >= G4VisManager::errors) {
	G4cout <<
	  "ERROR: G4VisCommandsViewerSet::SetNewValue: all:"
	  "\n  unrecognised from-viewer."
	       << G4endl;
      }
      return;
    }
    if (fromViewer == currentViewer) {
      if (verbosity >= G4VisManager::warnings) {
      G4cout <<
	"WARNING: G4VisCommandsViewerSet::SetNewValue: all:"
	"\n  from-viewer and current viewer are identical."
	     << G4endl;
      }
      return;
    }
    // Copy view parameters except for autoRefresh...
    G4bool currentAutoRefresh =
      currentViewer->GetViewParameters().IsAutoRefresh();
    vp = fromViewer->GetViewParameters();
    vp.SetAutoRefresh(currentAutoRefresh);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "View parameters of viewer \"" << currentViewer->GetName()
	     << "\"\n  set to those of viewer \"" << fromViewer->GetName()
	     << "\"."
	     << G4endl;
    }
  }

  else if (command == fpCommandAutoRefresh) {
    G4bool autoRefresh = G4UIcommand::ConvertToBool(newValue);
    vp.SetAutoRefresh(autoRefresh);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Views will ";
      if (!vp.IsAutoRefresh()) G4cout << "not ";
      G4cout << "be automatically refreshed after a change of view parameters."
	     << G4endl;
    }
  }

  else if (command == fpCommandAuxEdge) {
    vp.SetAuxEdgeVisible(G4UIcommand::ConvertToBool(newValue));
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Auxiliary edges will ";
      if (!vp.IsAuxEdgeVisible()) G4cout << "not ";
      G4cout << "be visible." << G4endl;
    }
  }

  else if (command == fpCommandCulling) {
    G4String cullingOption, stringFlag, unit;
    G4double density;
    G4bool boolFlag;
    std::istrstream is (newValue);
    is >> cullingOption >> stringFlag >> density >> unit;
    boolFlag = G4UIcommand::ConvertToBool(stringFlag);
    if (cullingOption == "global") {
      vp.SetCulling(boolFlag);
      if (verbosity >= G4VisManager::confirmations) {
	G4cout <<
	  "G4VisCommandsViewerSet::SetNewValue: culling: global culling flag"
	  " set to " << G4UIcommand::ConvertToString(boolFlag) <<
	  ".\n  Does not change specific culling flags."
	       << G4endl;
      }
    }
    else if (cullingOption == "coveredDaughters") {
      vp.SetCullingCovered(boolFlag);
      if (verbosity >= G4VisManager::confirmations) {
	G4cout <<
	  "G4VisCommandsViewerSet::SetNewValue: culling: culling covered"
	  "\n  daughters flag set to "
	       << G4UIcommand::ConvertToString(boolFlag) <<
	  ".  Daughters covered by opaque mothers"
	  "\n  will be culled, i.e., not drawn, if this flag is true."
	  "\n  Note: this is only effective in surface drawing style,"
	  "\n  and then only if the volumes are visible and opaque, and then"
	  "\n  only if no sections or cutways are in operation."
	       << G4endl;
      }
    }
    else if (cullingOption == "invisible") {
      vp.SetCullingInvisible(boolFlag);
      if (verbosity >= G4VisManager::confirmations) {
	G4cout <<
	  "G4VisCommandsViewerSet::SetNewValue: culling: culling invisible"
	  "\n  flag set to "
	       << boolFlag << G4UIcommand::ConvertToString(boolFlag) <<
	  ".  Volumes marked invisible will be culled,"
	  "\n  i.e., not drawn, if this flag is true."
	       << G4endl;
      }
    }
    else if (cullingOption == "density") {
      vp.SetDensityCulling(boolFlag);
      if (boolFlag) {
	density *= G4UnitDefinition::GetValueOf(unit);
	vp.SetVisibleDensity(density);
      }
      else {
	density = vp.GetVisibleDensity();
      }
      if (verbosity >= G4VisManager::confirmations) {
	G4cout <<
	  "G4VisCommandsViewerSet::SetNewValue: culling: culling by density"
	  "\n  flag set to " << G4UIcommand::ConvertToString(boolFlag) <<
	  ".  Volumes with density less than " <<
	  G4BestUnit(density,"Volumic Mass") <<
	  "\n  will be culled, i.e., not drawn, if this flag is true."
	       << G4endl;
      }
    }
    else {
      if (verbosity >= G4VisManager::errors) {
	G4cout <<
	  "ERROR: G4VisCommandsViewerSet::SetNewValue: culling:"
	  "\n  option not recognised."
	       << G4endl;
      }
    }
  }

  else if (command == fpCommandEdge) {
    G4ViewParameters::DrawingStyle existingStyle = vp.GetDrawingStyle();
    if (G4UIcommand::ConvertToBool(newValue)) {
      switch (existingStyle) {
      case G4ViewParameters::wireframe:
	break;
      case G4ViewParameters::hlr:
	break;
      case G4ViewParameters::hsr:
	vp.SetDrawingStyle(G4ViewParameters::hlhsr);
        break;
      case G4ViewParameters::hlhsr:
        break;
      }
    }
    else {
      switch (existingStyle) {
      case G4ViewParameters::wireframe:
	break;
      case G4ViewParameters::hlr:
	break;
      case G4ViewParameters::hsr:
	break;
      case G4ViewParameters::hlhsr:
	vp.SetDrawingStyle(G4ViewParameters::hsr);
	break;
      }
    }
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Drawing style of viewer \"" << currentViewer->GetName()
	     << "\" set to " << vp.GetDrawingStyle()
	     << G4endl;
    }
  }

  else if (command == fpCommandGlobalMarkerScale) {
    G4double globalMarkerScale
      = fpCommandGlobalMarkerScale->GetNewDoubleValue(newValue);
    vp.SetGlobalMarkerScale(globalMarkerScale);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Global Marker Scale changed to "
	     << vp.GetGlobalMarkerScale() << G4endl;
    }
  }

  else if (command == fpCommandHiddenEdge) {
    G4ViewParameters::DrawingStyle existingStyle = vp.GetDrawingStyle();
    if (G4UIcommand::ConvertToBool(newValue)) {
      switch (existingStyle) {
      case G4ViewParameters::wireframe:
	vp.SetDrawingStyle(G4ViewParameters::hlr);
	break;
      case G4ViewParameters::hlr:
	break;
      case G4ViewParameters::hsr:
	vp.SetDrawingStyle(G4ViewParameters::hlhsr);
        break;
      case G4ViewParameters::hlhsr:
        break;
      }
    }
    else {
      switch (existingStyle) {
      case G4ViewParameters::wireframe:
	break;
      case G4ViewParameters::hlr:
	vp.SetDrawingStyle(G4ViewParameters::wireframe);
	break;
      case G4ViewParameters::hsr:
	break;
      case G4ViewParameters::hlhsr:
	vp.SetDrawingStyle(G4ViewParameters::hsr);
	break;
      }
    }
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Drawing style of viewer \"" << currentViewer->GetName()
	     << "\" set to " << vp.GetDrawingStyle()
	     << G4endl;
    }
  }

  else if (command == fpCommandHiddenMarker) {
    G4bool hidden = G4UIcommand::ConvertToBool(newValue);
    if (hidden) vp.SetMarkerHidden();
    else vp.SetMarkerNotHidden();
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Markers will ";
      if (vp.IsMarkerNotHidden()) G4cout << "not ";
      G4cout << "be hidden under solid objects." << G4endl;
    }
  }

  else if (command == fpCommandLightsMove) {
    G4String s (newValue);
    if (s.find("cam") != G4String::npos) vp.SetLightsMoveWithCamera(true);
    else if(s.find("obj") != G4String::npos) vp.SetLightsMoveWithCamera(false);
    else {
      if (verbosity >= G4VisManager::errors) {
	G4cout << "ERROR: \"" << newValue << "\" not recognised."
	"  Looking for \"cam\" or \"obj\" in string." << G4endl;
      }
    }
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Lights move with ";
      if (vp.GetLightsMoveWithCamera())
	G4cout << "camera (object appears to rotate).";
      else G4cout << "object (the viewer appears to be moving).";
      G4cout << G4endl;
    }
  }

  else if (command == fpCommandLightsThetaPhi) {
    G4double theta, phi;
    ConvertToDoublePair(newValue, theta, phi);
    G4double x = std::sin (theta) * std::cos (phi);
    G4double y = std::sin (theta) * std::sin (phi);
    G4double z = std::cos (theta);
    fLightsVector = G4ThreeVector (x, y, z);
    vp.SetLightpointDirection(fLightsVector);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Lights direction set to "
	     << vp.GetLightpointDirection() << G4endl;
    }
  }

  else if (command == fpCommandLightsVector) {
    fLightsVector = G4UIcommand::ConvertTo3Vector(newValue);
    vp.SetLightpointDirection(fLightsVector);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Lights direction set to "
	     << vp.GetLightpointDirection() << G4endl;
    }
  }

  else if (command == fpCommandLineSegments) {
    G4int nSides = G4UIcommand::ConvertToInt(newValue);
    nSides = vp.SetNoOfSides(nSides);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout <<
	"Number of line segements per circle in polygon approximation is "
	     << nSides << G4endl;
    }
  }

  else if (command == fpCommandProjection) {
    G4double fieldHalfAngle;
    size_t iPos = 0;
    if (newValue[iPos] == 'o') {  // "orthogonal"
      fieldHalfAngle = 0.;
    }
    else if (newValue[iPos] == 'p') {  // "perspective"
      G4String dummy;
      G4String unit;
      std::istrstream is (newValue);
      is >> dummy >> fieldHalfAngle >> unit;
      fieldHalfAngle *= G4UIcommand::ValueOf(unit);
      if (fieldHalfAngle > 89.5 * deg || fieldHalfAngle <= 0.0) {
	if (verbosity >= G4VisManager::errors) {
	  G4cout <<
	    "ERROR: Field half angle should be 0 < angle <= 89.5 degrees.";
	  G4cout << G4endl;
	}
	return;
      }
    }
    else {
      if (verbosity >= G4VisManager::errors) {
	G4cout << "ERROR: \"" << newValue << "\" not recognised."
	  "  Looking for 'o' or 'p' first character." << G4endl;
      }
      return;
    }
    vp.SetFieldHalfAngle(fieldHalfAngle);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Projection style of viewer \"" << currentViewer->GetName()
	     << "\" set to ";
      if (fieldHalfAngle == 0.) {
	G4cout << "orthogonal.";
      }
      else {
	G4cout << "perspective\n  with half angle " << fieldHalfAngle / deg
	       << " degrees.";
      }
      G4cout << G4endl;
    }
  }

  else if (command == fpCommandSectionPlane) {
    G4String choice, unit;
    G4double x, y, z, nx, ny, nz;
    std::istrstream is (newValue);
    is >> choice >> x >> y >> z >> unit >> nx >> ny >> nz;

    G4int iSelector = -1;
    if (choice.compareTo("off",G4String::ignoreCase) == 0) iSelector = 0;
    if (choice.compareTo("on",G4String::ignoreCase) == 0) iSelector = 1;
    if (iSelector < 0) {
      if (verbosity >= G4VisManager::errors) {
	G4cout << "Choice not recognised (on/off)." << G4endl;
	G4cout << "Section drawing is currently: ";
	if (vp.IsSection ()) G4cout << "on";
	else                    G4cout << "off";
	G4cout << ".\nSection plane is currently: "
	       << vp.GetSectionPlane ();
	G4cout << G4endl;
      }
      return;
    }

    G4double F;
    switch (iSelector) {
    case 0:
      vp.UnsetSectionPlane();
      break;
    case 1:
      F = G4UIcommand::ValueOf(unit);
      x *= F; y *= F; z *= F;
      vp.SetSectionPlane(G4Plane3D(G4Normal3D(nx,ny,nz),
				   G4Point3D(x,y,z)));
      vp.SetViewpointDirection(G4Normal3D(nx,ny,nz));
      break;
    default:
      if (verbosity >= G4VisManager::errors) {
	G4cout << "ERROR: Choice not recognised (on/off)."
	       << G4endl;
      }
      break;
    }

    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Section drawing is now: ";
      if (vp.IsSection ()) G4cout << "on";
      else                    G4cout << "off";
      G4cout << ".\nSection plane is now: "
	     << vp.GetSectionPlane ();
      G4cout << G4endl;
    }
  }

  else if (command == fpCommandStyle) {
    G4ViewParameters::DrawingStyle existingStyle = vp.GetDrawingStyle();
    size_t iPos = 0;
    if (newValue[iPos] == 'w') {  // "wireframe"
      switch (existingStyle) {
      case G4ViewParameters::wireframe:
	break;
      case G4ViewParameters::hlr:
	break;
      case G4ViewParameters::hsr:
	vp.SetDrawingStyle(G4ViewParameters::wireframe);
        break;
      case G4ViewParameters::hlhsr:
	vp.SetDrawingStyle(G4ViewParameters::hlr);
        break;
      }
    }
    else if (newValue[iPos] == 's') {  // "surface"
      switch (existingStyle) {
      case G4ViewParameters::wireframe:
	vp.SetDrawingStyle(G4ViewParameters::hsr);
	break;
      case G4ViewParameters::hlr:
	vp.SetDrawingStyle(G4ViewParameters::hlhsr);
	break;
      case G4ViewParameters::hsr:
	break;
      case G4ViewParameters::hlhsr:
	break;
      }
    }
    else {
      if (verbosity >= G4VisManager::errors) {
	G4cout << "ERROR: \"" << newValue << "\" not recognised."
	  "  Looking for 'w' or 's' first character." << G4endl;
      }
      return;
    }
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Drawing style of viewer \"" << currentViewer->GetName()
	     << "\" set to " << vp.GetDrawingStyle()
	     << G4endl;
    }
  }

  else if (command == fpCommandUpThetaPhi) {
    G4double theta, phi;
    ConvertToDoublePair(newValue, theta, phi);
    G4double x = std::sin (theta) * std::cos (phi);
    G4double y = std::sin (theta) * std::sin (phi);
    G4double z = std::cos (theta);
    fUpVector = G4ThreeVector (x, y, z);
    vp.SetUpVector(fUpVector);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Up direction set to " << vp.GetUpVector() << G4endl;
    }
  }

  else if (command == fpCommandUpVector) {
    fUpVector = G4UIcommand::ConvertTo3Vector(newValue).unit();
    vp.SetUpVector(fUpVector);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Up direction set to " << vp.GetUpVector() << G4endl;
    }
  }

  else if (command == fpCommandViewpointThetaPhi) {
    G4double theta, phi;
    ConvertToDoublePair(newValue, theta, phi);
    G4double x = std::sin (theta) * std::cos (phi);
    G4double y = std::sin (theta) * std::sin (phi);
    G4double z = std::cos (theta);
    fViewpointVector = G4ThreeVector (x, y, z);
    vp.SetViewAndLights(fViewpointVector);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Viewpoint direction set to "
	     << vp.GetViewpointDirection() << G4endl;
      if (vp.GetLightsMoveWithCamera ()) {
	G4cout << "Lightpoint direction set to "
	       << vp.GetActualLightpointDirection () << G4endl;
      }
    }
  }

  else if (command == fpCommandViewpointVector) {
    fViewpointVector = G4UIcommand::ConvertTo3Vector(newValue).unit();
    vp.SetViewAndLights(fViewpointVector);
    if (verbosity >= G4VisManager::confirmations) {
      G4cout << "Viewpoint direction set to "
	     << vp.GetViewpointDirection() << G4endl;
      if (vp.GetLightsMoveWithCamera ()) {
	G4cout << "Lightpoint direction set to "
	       << vp.GetActualLightpointDirection () << G4endl;
      }
    }
  }

  else {
    if (verbosity >= G4VisManager::errors) {
      G4cout <<
	"ERROR: G4VisCommandsViewerSet::SetNewValue: unrecognised command."
	     << G4endl;
    }
    return;
  }

  SetViewParameters(currentViewer,vp);
}

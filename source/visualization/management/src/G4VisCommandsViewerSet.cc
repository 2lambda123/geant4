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
// $Id: G4VisCommandsViewerSet.cc,v 1.29 2005-01-26 16:30:50 johna Exp $
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

G4VisCommandsViewerSet::G4VisCommandsViewerSet ():
  fLightsVector    (G4ThreeVector(1.,1.,1.)),
  fUpVector        (G4ThreeVector(0.,1.,0.)),
  fViewpointVector (G4ThreeVector(0.,0.,1.))
{
  G4bool omitable;
  G4UIparameter* parameter;

  fpCommandAll = new G4UIcmdWithAString ("/vis/viewer/set/all",this);
  fpCommandAll->SetGuidance
    ("/vis/viewer/set/all <from-viewer-name>"
     "\nCopies view parameters (except the autoRefresh status) from"
     "\n  from-viewer to current viewer.");
  fpCommandAll->SetParameterName ("from-viewer-name",omitable = false);
  viewerNameCommands.push_back (fpCommandAll);

  fpCommandAutoRefresh = new G4UIcmdWithABool
    ("/vis/viewer/set/autoRefresh",this);
  fpCommandAutoRefresh->SetGuidance
    ("/vis/viewer/set/autoRefresh [true|false]");
  fpCommandAutoRefresh->SetGuidance("  default: false");
  fpCommandAutoRefresh->SetGuidance
    ("View is automatically refreshed after a change of view parameters.");
  fpCommandAutoRefresh->SetParameterName("auto-refresh",omitable = true);
  fpCommandAutoRefresh->SetDefaultValue(false);

  fpCommandAuxEdge = new G4UIcmdWithABool
    ("/vis/viewer/set/auxiliaryEdge",this);
  fpCommandAuxEdge->SetGuidance("/vis/viewer/set/auxiliaryEdge [true|false]");
  fpCommandAuxEdge->SetGuidance("  default: false");
  fpCommandAuxEdge->SetGuidance
    ("Auxiliary edges become visible/invisible.");
  fpCommandAuxEdge->SetParameterName("edge",omitable = true);
  fpCommandAuxEdge->SetDefaultValue(false);

  fpCommandCulling = new G4UIcommand("/vis/viewer/set/culling",this);
  fpCommandCulling->SetGuidance
    ("/vis/viewer/set/culling global|coveredDaughters|invisible|density"
     " [true|false] [density] [unit]");
  fpCommandCulling->SetGuidance
    (
     "  default: none true 0.01 g/cm3"
     "\n  global: enables other culling options/disables culling."
     "\n  coveredDaughters: does not send volumes that would not be seen on the"
     "\n    screen because covered by ancester volumes in surface drawing mode,"
     "\n    and then only if the volumes are visible and opaque, and then only if"
     "\n    no sections or cutways are in operation.  Intended solely to improve"
     "\n    the speed of rendering visible volumes."
     "\n  invisible: culls objects with invisible attribute."
     "\n  density: culls volumes with density lower than threshold.  Useful for"
     "\n    eliminating \"container volumes\" with no physical correspondence whose"
     "\n    material is usually air.  If this is selected, provide threshold"
     "\n    density and unit (g/cm3, mg/cm3 or kg/m3)."
     );
  parameter = new G4UIparameter("culling-option",'s',omitable = false);
  parameter->SetParameterCandidates
    ("global coveredDaughters invisible density");
  fpCommandCulling->SetParameter(parameter);
  parameter = new G4UIparameter("flag",'b',omitable = true);
  parameter->SetDefaultValue("true");
  fpCommandCulling->SetParameter(parameter);
  parameter = new G4UIparameter("density",'d',omitable = true);
  parameter->SetDefaultValue("0.01");
  fpCommandCulling->SetParameter(parameter);
  parameter = new G4UIparameter("unit",'s',omitable = true);
  parameter->SetDefaultValue("g/cm3");
  fpCommandCulling->SetParameter(parameter);

  fpCommandEdge = new G4UIcmdWithABool("/vis/viewer/set/edge",this);
  fpCommandEdge->SetGuidance("/vis/viewer/set/edge [true|false]");
  fpCommandEdge->SetGuidance("  default: true");
  fpCommandEdge->SetGuidance
    ("Edges become visible/invisible in surface mode.");
  fpCommandEdge->SetParameterName("edge",omitable = true);
  fpCommandEdge->SetDefaultValue(true);

  fpCommandGlobalMarkerScale = new G4UIcmdWithADouble
    ("/vis/viewer/set/globalMarkerScale", this);
  fpCommandGlobalMarkerScale -> SetGuidance
    ("/vis/viewer/set/globalMarkerScale [<scale-factor>]");
  fpCommandGlobalMarkerScale -> SetGuidance
    ("  default: 1");
  fpCommandGlobalMarkerScale -> SetGuidance
    ("Multiplies marker sizes by this factor.");
  fpCommandGlobalMarkerScale -> SetParameterName("scale-factorr",
						 omitable=true);
  fpCommandGlobalMarkerScale->SetDefaultValue(1.);

  fpCommandHiddenEdge =
    new G4UIcmdWithABool("/vis/viewer/set/hiddenEdge",this);
  fpCommandHiddenEdge->SetGuidance("/vis/viewer/set/hiddenEdge [true|false]");
  fpCommandHiddenEdge->SetGuidance("  default: true");
  fpCommandHiddenEdge->SetGuidance
    ("Edges become hidden/seen in wireframe or surface mode.");
  fpCommandHiddenEdge->SetParameterName("hidden-edge",omitable = true);
  fpCommandHiddenEdge->SetDefaultValue(true);

  fpCommandHiddenMarker =
    new G4UIcmdWithABool("/vis/viewer/set/hiddenMarker",this);
  fpCommandHiddenMarker->SetGuidance
    ("/vis/viewer/set/hiddenMarker [true|false]");
  fpCommandHiddenMarker->SetGuidance("  default: true");
  fpCommandHiddenMarker->SetGuidance
    ("Markers are hidden by (if true) or seen through (if false) closer objects.");
  fpCommandHiddenMarker->SetParameterName("hidden-marker",omitable = true);
  fpCommandHiddenMarker->SetDefaultValue(true);

  fpCommandLightsMove = new G4UIcmdWithAString
    ("/vis/viewer/set/lightsMove",this);
  fpCommandLightsMove->SetGuidance
    ("/vis/viewer/set/lightsMove with-camera|with-object");
  fpCommandLightsMove->SetGuidance
    ("Note: parameter will be parsed for \"cam\" or \"obj\".");
  fpCommandLightsMove->SetParameterName("lightsMove",omitable = false);

  fpCommandLightsThetaPhi = new G4UIcommand
    ("/vis/viewer/set/lightsThetaPhi", this);
  fpCommandLightsThetaPhi -> SetGuidance
    ("/vis/viewer/set/lightsThetaPhi  [<theta>] [<phi>] [deg|rad]");
  fpCommandLightsThetaPhi -> SetGuidance
    ("  default: 60 45 deg - becomes \"current as default\"");
  fpCommandLightsThetaPhi -> SetGuidance
    ("Set direction from target to lights.");
  parameter = new G4UIparameter("theta", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandLightsThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter("phi", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandLightsThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("deg");
  fpCommandLightsThetaPhi -> SetParameter (parameter);

  fpCommandLightsVector = new G4UIcommand
    ("/vis/viewer/set/lightsVector", this);
  fpCommandLightsVector -> SetGuidance
    ("/vis/viewer/set/lightsVector  [<x>] [<y>] [<z>]");
  fpCommandLightsVector -> SetGuidance
    ("  default: 1 1 1 - becomes \"current as default\"");
  fpCommandLightsVector -> SetGuidance
    ("Set direction from target to lights.");
  parameter = new G4UIparameter("x", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandLightsVector -> SetParameter (parameter);
  parameter = new G4UIparameter("y", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandLightsVector -> SetParameter (parameter);
  parameter = new G4UIparameter ("z", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandLightsVector -> SetParameter (parameter);

  fpCommandLineSegments = new G4UIcmdWithAnInteger
    ("/vis/viewer/set/lineSegmentsPerCircle",this);
  fpCommandLineSegments->SetGuidance
    ("/vis/viewer/set/lineSegmentsPerCircle  [<number-of-sides-per-circle>]");
  fpCommandLineSegments->SetGuidance
    ("  default: 24");
  fpCommandLineSegments->SetGuidance
    ("Number of sides per circle in polygon/polyhedron graphical"
     "\nrepresentation of objects with curved lines/surfaces.");
  fpCommandLineSegments->SetParameterName("line-segments",omitable = true);
  fpCommandLineSegments->SetDefaultValue(24);

  fpCommandProjection = new G4UIcommand("/vis/viewer/set/projection",this);
  fpCommandProjection->SetGuidance
    ("/vis/viewer/set/projection"
     " o[rthogonal]|p[erspective] [<field-half-angle>] [deg|rad]");
  fpCommandProjection->SetGuidance("  default: orthogonal 30 deg");
  parameter = new G4UIparameter("projection",'s',omitable = true);
  parameter->SetDefaultValue("orthogonal");
  fpCommandProjection->SetParameter(parameter);
  parameter = new G4UIparameter("field-half-angle",'d',omitable = true);
  parameter->SetDefaultValue(30.);
  fpCommandProjection->SetParameter(parameter);
  parameter = new G4UIparameter("unit",'s',omitable = true);
  parameter->SetDefaultValue("deg");
  fpCommandProjection->SetParameter(parameter);

  fpCommandSectionPlane = new G4UIcommand 
    ("/vis/viewer/set/sectionPlane on|off [x] [y] [z] [units] [nx] [ny] [nz]",this);
  fpCommandSectionPlane -> SetGuidance
    ("  default: none 0 0 0 cm 1 0 0");
  fpCommandSectionPlane -> SetGuidance
    (
     "Set plane for drawing section (DCUT).  Specify plane by"
     "\nx y z units nx ny nz, e.g., for a y-z plane at x = 1 cm:"
     "\n/vis/viewer/set/sectionPlane on 1 0 0 cm 1 0 0"
     "\nTo turn off: /vis/viewer/set/sectionPlane off"
     );
  parameter  =  new G4UIparameter("Selector",'c',true);
  parameter  -> SetDefaultValue  ("?");
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
  fpCommandStyle->SetGuidance ("/vis/viewer/set/style w[ireframe]|s[urface]");
  fpCommandStyle->SetParameterName ("style",omitable = false);

  fpCommandUpThetaPhi = new G4UIcommand
    ("/vis/viewer/set/upThetaPhi", this);
  fpCommandUpThetaPhi -> SetGuidance
    ("/vis/viewer/set/upThetaPhi  [<theta>] [<phi>] [deg|rad]");
  fpCommandUpThetaPhi -> SetGuidance
    ("  default: 90 90 deg - becomes \"current as default\"");
  fpCommandUpThetaPhi -> SetGuidance
    ("Set up vector.  Viewer will attempt always to show"
     " this direction upwards.");
  parameter = new G4UIparameter("theta", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandUpThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter("phi", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandUpThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("deg");
  fpCommandUpThetaPhi -> SetParameter (parameter);

  fpCommandUpVector = new G4UIcommand
    ("/vis/viewer/set/upVector", this);
  fpCommandUpVector -> SetGuidance
    ("/vis/viewer/set/upVector  [<x>] [<y>] [<z>]");
  fpCommandUpVector -> SetGuidance
    ("  default: 0 1 0 - becomes \"current as default\"");
  fpCommandUpVector -> SetGuidance
    ("Set up vector.  Viewer will attempt always to show"
     " this direction upwards.");
  parameter = new G4UIparameter("x", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandUpVector -> SetParameter (parameter);
  parameter = new G4UIparameter("y", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandUpVector -> SetParameter (parameter);
  parameter = new G4UIparameter ("z", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandUpVector -> SetParameter (parameter);

  fpCommandViewpointThetaPhi = new G4UIcommand
    ("/vis/viewer/set/viewpointThetaPhi", this);
  fpCommandViewpointThetaPhi -> SetGuidance
    ("/vis/viewer/set/viewpointThetaPhi  [<theta>] [<phi>] [deg|rad]");
  fpCommandViewpointThetaPhi -> SetGuidance
    ("  default: 0 0 deg - becomes \"current as default\"");
  fpCommandViewpointThetaPhi -> SetGuidance
    ("Set direction from target to camera.  Also changes lightpoint direction"
     "\nif lights are set to move with camera.");
  parameter = new G4UIparameter("theta", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandViewpointThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter("phi", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandViewpointThetaPhi -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("deg");
  fpCommandViewpointThetaPhi -> SetParameter (parameter);

  fpCommandViewpointVector = new G4UIcommand
    ("/vis/viewer/set/viewpointVector", this);
  fpCommandViewpointVector -> SetGuidance
    ("/vis/viewer/set/viewpointVector  [<x>] [<y>] [<z>]");
  fpCommandViewpointVector -> SetGuidance
    ("  default: 0 0 1 - becomes \"current as default\"");
  fpCommandViewpointVector -> SetGuidance
    ("Set direction from target to camera.  Also changes lightpoint direction"
     "\nif lights are set to move with camera.");
  parameter = new G4UIparameter("x", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandViewpointVector -> SetParameter (parameter);
  parameter = new G4UIparameter("y", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
  fpCommandViewpointVector -> SetParameter (parameter);
  parameter = new G4UIparameter ("z", 'd', omitable = true);
  parameter -> SetCurrentAsDefault (true);
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

G4String G4VisCommandsViewerSet::GetCurrentValue(G4UIcommand* command) {
  G4String currentValue;
  if (command == fpCommandLightsThetaPhi) {
    currentValue = ConvertToString(fLightsVector.theta(),
			   fLightsVector.phi(), "deg");
  }
  else if (command == fpCommandLightsVector) {
    currentValue = G4UIcommand::ConvertToString(fLightsVector);
  }
  else if (command == fpCommandViewpointThetaPhi) {
    currentValue = ConvertToString(fViewpointVector.theta(),
			   fViewpointVector.phi(), "deg");
  }
  else if (command == fpCommandViewpointVector) {
    currentValue = G4UIcommand::ConvertToString(fViewpointVector);
  }
  else {
    currentValue = "invalid";
  }
  return currentValue;
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
    std::istrstream is ((char*)newValue.data());
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
    vp.SetNoOfSides(nSides);
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
      std::istrstream is ((char*)newValue.data());
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
    const char* t = newValue;
    std::istrstream is ((char*)t);
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

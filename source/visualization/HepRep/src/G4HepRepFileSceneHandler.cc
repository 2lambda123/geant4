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
// $Id: G4HepRepFileSceneHandler.cc,v 1.17 2003-12-09 15:39:26 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Joseph Perl  27th January 2002
// A base class for a scene handler to export geometry and trajectories
// to the HepRep xml file format.

#include "G4HepRepFileSceneHandler.hh"
#include "G4HepRepFile.hh"

#include "G4VSolid.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4ModelingParameters.hh"
#include "G4Polymarker.hh"
#include "G4Polyline.hh"
#include "G4Text.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4Polyhedron.hh"
#include "G4NURBS.hh"
#include "G4VTrajectory.hh"
#include "G4VTrajectoryPoint.hh"
#include "G4VHit.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

//HepRep
#include "G4HepRepFileXMLWriter.hh"

G4int G4HepRepFileSceneHandler::fSceneIdCount = 0;
// Counter for HepRep scene handlers.

G4int G4HepRepFileSceneHandler::fSceneCount = 0;
// No. of extanct scene handlers.

G4HepRepFileSceneHandler::G4HepRepFileSceneHandler(G4VGraphicsSystem& system,
					 const G4String& name):
  G4VSceneHandler(system, fSceneIdCount++, name),
  fCurrentDepth                 (0),
  fpCurrentPV                   (0),
  fpCurrentLV                   (0)
{
  fSceneCount++;

  hepRepXMLWriter = ((G4HepRepFile*)(&system))->GetHepRepXMLWriter();
  fileCounter = 0;
}

G4HepRepFileSceneHandler::~G4HepRepFileSceneHandler() {}

void G4HepRepFileSceneHandler::EstablishSpecials
(G4PhysicalVolumeModel& pvModel) {
  pvModel.DefinePointersToWorkingSpace (&fCurrentDepth,
					&fpCurrentPV,
					&fpCurrentLV);
}

void G4HepRepFileSceneHandler::BeginModeling() {
  G4VSceneHandler::BeginModeling();  // Required: see G4VSceneHandler.hh.
}

void G4HepRepFileSceneHandler::EndModeling() {
  G4VSceneHandler::EndModeling();  // Required: see G4VSceneHandler.hh.
}

#ifdef G4HEPREPFILEDEBUG
void G4HepRepFileSceneHandler::PrintThings() {
  G4cout <<
    "  with transformation "
	 << (void*)fpObjectTransformation;
  if (fpCurrentPV) {
    G4cout <<
      "\n  current physical volume: "
	   << fpCurrentPV->GetName() <<
      "\n  current logical volume: "
	   << fpCurrentLV->GetName() <<
      "\n  current depth of geometry tree: "
	   << fCurrentDepth;
  }
  G4cout << G4endl;
}
#endif

void G4HepRepFileSceneHandler::AddThis(const G4Box& box) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Box& box) called for "
	 << box.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(box);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Cons& cons) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Cons& cons) called for "
	 << cons.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(cons);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Tubs& tubs) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Tubs& tubs) called for "
	 << tubs.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(tubs);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Trd& trd) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Trd& trd) called for "
	 << trd.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(trd);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Trap& trap) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Trap& trap) called for "
	 << trap.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(trap);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Sphere& sphere) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Sphere& sphere) called for "
	 << sphere.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(sphere);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Para& para) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Para& para) called for "
	 << para.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(para);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Torus& torus) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Torus& torus) called for "
	 << torus.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(torus);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Polycone& polycone) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Polycone& polycone) called for "
	 << polycone.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(polycone);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4Polyhedra& polyhedra) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Polyhedra& polyhedra) called for "
	 << polyhedra.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(polyhedra);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis(const G4VSolid& solid) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddThis(const G4Solid& solid) called for "
	 << solid.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddThis(solid);  // Invoke default action.
}

void G4HepRepFileSceneHandler::AddThis (const G4VTrajectory& traj) {
#ifdef G4HEPREPFILEDEBUG
    G4cout << "G4HepRepFileSceneHandler::AddThis(G4VTrajectory&) " << G4endl;
#endif

  std::vector<G4AttValue>* attValues = traj.CreateAttValues();
  std::vector<G4AttValue>::iterator iAttVal;
  const std::map<G4String,G4AttDef>* attDefs = traj.GetAttDefs();
  std::map<G4String,G4AttDef>::const_iterator iAttDef;
  G4int i;

  // Open the HepRep output file if it is not already open.
  CheckFileOpen();

  // Add the Event Data Type if it hasn't already been added.
  // If this is the first trajectory, add the Event Data type and define attributes.
  if (strcmp("Event Data",hepRepXMLWriter->prevTypeName[0])!=0) {
    hepRepXMLWriter->addType("Event Data",0);
    hepRepXMLWriter->addInstance();
  }

  // Add the Trajectories Type.
  G4String previousName = hepRepXMLWriter->prevTypeName[1];
  hepRepXMLWriter->addType("Trajectories",1);

  // If this is the first trajectory of this event...
  if (strcmp("Trajectories",previousName)!=0) {
    // Specify attribute values common to all trajectories.
    hepRepXMLWriter->addAttValue("DrawAs","Line");
    hepRepXMLWriter->addAttValue("Layer",100);

    // Specify additional attribute definitions for trajectories.
    // Take all Trajectory attDefs from first trajectory.
    // Would rather be able to get these attDefs without needing a reference from any
    // particular trajectory, but don't know how to do that.
    if (attValues && attDefs) {
      for (iAttVal = attValues->begin();
	   iAttVal != attValues->end(); ++iAttVal) {
	iAttDef = attDefs->find(iAttVal->GetName());
	if (iAttDef != attDefs->end()) {
	  // Protect against incorrect use of Category.  Anything value other than the
	  // standard ones will be considered to be in the physics category.
	  G4String category = iAttDef->second.GetCategory();
	  if (strcmp(category,"Draw")!=0 &&
	      strcmp(category,"Physics")!=0 &&
	      strcmp(category,"Association")!=0 &&
	      strcmp(category,"PickAction")!=0)
	    category = "Physics";
	  hepRepXMLWriter->addAttDef(iAttVal->GetName(), iAttDef->second.GetDesc(),
				     category, iAttDef->second.GetExtra());
	}
      }
    }

    // Specify additional attribute definitions for trajectory points.
    // Take all TrajectoryPoint attDefs from first point of first trajectory.
    // Would rather be able to get these attDefs without needing a reference from any
    // particular point, but don't know how to do that.
    // Note also that until we get the good separation of Types and Instances that comes
    // in HepRep2, the user must be careful not to use the same AttName for two
    // different Types.
    if (traj.GetPointEntries()>0) {
      G4VTrajectoryPoint* aTrajectoryPoint = traj.GetPoint(0);
      std::vector<G4AttValue>* pointAttValues
	= aTrajectoryPoint->CreateAttValues();
      const std::map<G4String,G4AttDef>* pointAttDefs
	= aTrajectoryPoint->GetAttDefs();
      if (pointAttValues && pointAttDefs) {
	for (iAttVal = pointAttValues->begin();
	     iAttVal != pointAttValues->end(); ++iAttVal) {
	  iAttDef =
	    pointAttDefs->find(iAttVal->GetName());
	  if (iAttDef != pointAttDefs->end()) {
	    // Protect against incorrect use of Category.  Anything value other than the
	    // standard ones will be considered to be in the physics category.
	    G4String category = iAttDef->second.GetCategory();
	    if (strcmp(category,"Draw")!=0 &&
		strcmp(category,"Physics")!=0 &&
		strcmp(category,"Association")!=0 &&
		strcmp(category,"PickAction")!=0)
	      category = "Physics";
	    hepRepXMLWriter->addAttDef(iAttVal->GetName(), iAttDef->second.GetDesc(),
				       category, iAttDef->second.GetExtra());
	  }
	}
      }
    }
  }

  // For every trajectory, add an instance of Type Trajectory.
  hepRepXMLWriter->addInstance();

  // Set the LineColor attribute according to the particle charge.
  float redness = 0.;
  float greenness = 0.;
  float blueness = 0.;
  const G4double charge = traj.GetCharge();
  if(charge>0.)      blueness =  1.; // Blue = positive.
  else if(charge<0.) redness  =  1.; // Red = negative.
  else               greenness = 1.; // Green = neutral.
  hepRepXMLWriter->addAttValue("LineColor",redness,greenness,blueness);

  // Copy the current trajectory's G4AttValues to HepRepAttValues.
  if (attValues && attDefs) {
    for (iAttVal = attValues->begin();
	 iAttVal != attValues->end(); ++iAttVal) {
      std::map<G4String,G4AttDef>::const_iterator iAttDef =
	attDefs->find(iAttVal->GetName());
      if (iAttDef == attDefs->end()) {
	G4cout << "G4HepRepFileSceneHandler::AddThis(traj):"
	  "\n  WARNING: no matching definition for attribute \""
	       << iAttVal->GetName() << "\", value: "
	       << iAttVal->GetValue();
      }
      else {
	// Use GetDesc rather than GetName once WIRED can handle names with spaces in them.
	//hepRepXMLWriter->addAttValue(iAttDef->second.GetDesc(), iAttVal->GetValue());
	hepRepXMLWriter->addAttValue(iAttVal->GetName(), iAttVal->GetValue());
      }
    }    
    delete attValues;  // AttValues must be deleted after use.
  }

  // Each trajectory is made of a single primitive, a polyline.
  hepRepXMLWriter->addPrimitive();

  // Specify the polyline by using the trajectory points.
  for (i = 0; i < traj.GetPointEntries(); i++) {
    G4VTrajectoryPoint* aTrajectoryPoint = traj.GetPoint(i);
    G4Point3D vertex = aTrajectoryPoint->GetPosition();
    hepRepXMLWriter->addPoint(vertex.x(), vertex.y(), vertex.z());
  }

  // Create Trajectory Points as a subType of Trajectories.
  previousName = hepRepXMLWriter->prevTypeName[2];
  hepRepXMLWriter->addType("Trajectory Points",2);

  // Specify attributes common to all trajectory points.
  if (strcmp("Trajectory Points",previousName)!=0) {
    hepRepXMLWriter->addAttValue("DrawAs","Point");
    hepRepXMLWriter->addAttValue("Layer",110);
    hepRepXMLWriter->addAttValue("Visibility","False");
  }

  for (i = 0; i < traj.GetPointEntries(); i++) {
    G4VTrajectoryPoint* aTrajectoryPoint = traj.GetPoint(i);

    // Each point is a separate instance of the type Trajectory Points.
    hepRepXMLWriter->addInstance();

  // Copy the current trajectory point's G4AttValues to HepRepAttValues.
    std::vector<G4AttValue>* pointAttValues
      = aTrajectoryPoint->CreateAttValues();
    const std::map<G4String,G4AttDef>* pointAttDefs
      = aTrajectoryPoint->GetAttDefs();

    if (pointAttValues && pointAttDefs) {
      std::vector<G4AttValue>::iterator iAttVal;
      for (iAttVal = pointAttValues->begin();
	   iAttVal != pointAttValues->end(); ++iAttVal) {
	std::map<G4String,G4AttDef>::const_iterator iAttDef =
	  pointAttDefs->find(iAttVal->GetName());
	if (iAttDef == pointAttDefs->end()) {
	  G4cout << "\nG4VTrajectory::ShowTrajectory:"
	    "\n  WARNING: no matching definition for trajectory"
	    " point attribute \""
		 << iAttVal->GetName() << "\", value: "
		 << iAttVal->GetValue();
	}
	else {
	  //hepRepXMLWriter->addAttValue(iAttDef->second.GetDesc(), iAttVal->GetValue());
	  hepRepXMLWriter->addAttValue(iAttVal->GetName(), iAttVal->GetValue());
	}
      }
      delete pointAttValues;  // AttValues must be deleted after use.
    }

  // Each trajectory point is made of a single primitive, a point.
    hepRepXMLWriter->addPrimitive();
    G4Point3D vertex = aTrajectoryPoint->GetPosition();
    hepRepXMLWriter->addPoint(vertex.x(), vertex.y(), vertex.z());
  }
}

void G4HepRepFileSceneHandler::AddThis (const G4VHit& hit) {
#ifdef G4HEPREPFILEDEBUG
    G4cout << "G4HepRepFileSceneHandler::AddThis(G4VHit&) " << G4endl;
#endif
    G4VSceneHandler::AddThis (hit);
}

void G4HepRepFileSceneHandler::AddPrimitive(const G4Polyline& polyline) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4Polyline& polyline) called:"
    "\n  polyline: " << polyline
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Line",polyline);

  hepRepXMLWriter->addPrimitive();

  for (size_t i=0; i < polyline.size(); i++) {
      G4Point3D vertex = (*fpObjectTransformation) * polyline[i];
      hepRepXMLWriter->addPoint(vertex.x(), vertex.y(), vertex.z());
  }
}


void G4HepRepFileSceneHandler::AddPrimitive (const G4Polymarker& line) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4Polymarker& line) called"
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Point",line);

  hepRepXMLWriter->addAttValue("MarkName", "Dot");
  hepRepXMLWriter->addAttValue("MarkSize", 4);

  hepRepXMLWriter->addPrimitive();

  for (size_t i=0; i < line.size(); i++) {
     G4Point3D vertex = (*fpObjectTransformation) * line[i];
     hepRepXMLWriter->addPoint(vertex.x(), vertex.y(), vertex.z());
  }
}

#ifdef G4HEPREPFILEDEBUG
void G4HepRepFileSceneHandler::AddPrimitive(const G4Text& text) {
#else
void G4HepRepFileSceneHandler::AddPrimitive(const G4Text&) {
#endif
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4Text& text) called:"
    "\n  text: " << text.GetText()
	 << G4endl;
  PrintThings();
#endif
  G4cout << "G4HepRepFileSceneHandler::AddPrimitive G4Text : not yet implemented. " << G4endl;
}

void G4HepRepFileSceneHandler::AddPrimitive(const G4Circle& circle) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4Circle& circle) called:"
    "\n  radius: " << circle.GetWorldRadius()
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Point",circle);

  hepRepXMLWriter->addAttValue("MarkName", "Dot");
  hepRepXMLWriter->addAttValue("MarkSize", 4);

  hepRepXMLWriter->addPrimitive();

  G4Point3D center = (*fpObjectTransformation) * circle.GetPosition();
  hepRepXMLWriter->addPoint(center.x(), center.y(), center.z());
}

void G4HepRepFileSceneHandler::AddPrimitive(const G4Square& square) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4Square& square) called:"
    "\n  side: " << square.GetWorldRadius()
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Point",square);

  hepRepXMLWriter->addAttValue("MarkName", "Square");
  hepRepXMLWriter->addAttValue("MarkSize", 4);

  hepRepXMLWriter->addPrimitive();

  G4Point3D center = (*fpObjectTransformation) * square.GetPosition();
  hepRepXMLWriter->addPoint(center.x(), center.y(), center.z());
}

void G4HepRepFileSceneHandler::AddPrimitive(const G4Polyhedron& polyhedron) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4Polyhedron& polyhedron) called."
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Polygon",polyhedron);

  if(polyhedron.GetNoFacets()==0)return;

  G4Normal3D surfaceNormal;
  G4Point3D vertex;

  G4bool notLastFace;
  do {
      hepRepXMLWriter->addPrimitive();
      notLastFace = polyhedron.GetNextNormal (surfaceNormal);

      G4int edgeFlag = 1;
      G4bool notLastEdge;
      do {
          notLastEdge = polyhedron.GetNextVertex (vertex, edgeFlag);
          vertex = (*fpObjectTransformation) * vertex;
	  hepRepXMLWriter->addPoint(vertex.x(), vertex.y(), vertex.z());
      } while (notLastEdge);
  } while (notLastFace);
}

void G4HepRepFileSceneHandler::AddPrimitive(const G4NURBS&) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4NURBS& nurbs) called."
	 << G4endl;
  PrintThings();
#endif
    G4cout << "G4HepRepFileSceneHandler::AddPrimitive G4NURBS : not implemented. " << G4endl;
}

G4HepRepFileXMLWriter *G4HepRepFileSceneHandler::GetHepRepXMLWriter() {
    return hepRepXMLWriter;
}

void G4HepRepFileSceneHandler::AddHepRepInstance(const char* primName,
						 const G4Visible visible) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddHepRepInstance called."
	 << G4endl;
#endif

  // Open the HepRep output file if it is not already open.
  CheckFileOpen();

  // Handle Type declaration for Event Data.
  // Should be able to just test on fReadyForTransients, but this seems to
  // be falsely false if no geometry has yet been drawn.
  // So I test on both !fpCurrentPV (means no geometry has yet been drawn)
  // and fReadyForTransients.
  if (!fpCurrentPV || fReadyForTransients) {
    if (strcmp("Event Data",hepRepXMLWriter->prevTypeName[0])!=0) {
      hepRepXMLWriter->addType("Event Data",0);
      hepRepXMLWriter->addInstance();
    }

    if (strcmp("Line",primName)==0)
      hepRepXMLWriter->addType("Trajectories",1);
    else {
      if (strcmp(hepRepXMLWriter->prevTypeName[1],"Trajectories")==0)
	hepRepXMLWriter->addType("Trajectory Points",2);
      else
	hepRepXMLWriter->addType("Trajectory Points",1);
    }

    hepRepXMLWriter->addInstance();

    // Handle Type declaration for Detector Geometry,
    // replacing G4's top geometry level name "worldPhysical" with the
    // name "Detector Geometry".
  } else {
    if (strcmp("Detector Geometry",hepRepXMLWriter->prevTypeName[0])!=0) {
      hepRepXMLWriter->addType("Detector Geometry",0);
      hepRepXMLWriter->addInstance();
    }

    // Re-insert any layers of the hierarchy that were removed by G4's culling process.
    while (hepRepXMLWriter->typeDepth < (fCurrentDepth-1)) {
      hepRepXMLWriter->addType("G4 Culled Layer", hepRepXMLWriter->typeDepth + 1);
      hepRepXMLWriter->addInstance();
    }

    if (fCurrentDepth!=0) {
    // Add the HepRepType for the current volume.
      hepRepXMLWriter->addType(fpCurrentPV->GetName(),fCurrentDepth);
      hepRepXMLWriter->addInstance();
    }

    // Additional attributes.
    hepRepXMLWriter->addAttValue("LVol", fpCurrentLV->GetName());
    hepRepXMLWriter->addAttValue("Solid", fpCurrentLV->GetSolid()->GetName());
    hepRepXMLWriter->addAttValue("EType", fpCurrentLV->GetSolid()->GetEntityType());
    hepRepXMLWriter->addAttValue("Material", fpCurrentLV->GetMaterial()->GetName());
    hepRepXMLWriter->addAttValue("Density", fpCurrentLV->GetMaterial()->GetDensity());
    hepRepXMLWriter->addAttValue("State", fpCurrentLV->GetMaterial()->GetState());
    hepRepXMLWriter->addAttValue("Radlen", fpCurrentLV->GetMaterial()->GetRadlen());
  }

  hepRepXMLWriter->addAttValue("DrawAs",primName);
  hepRepXMLWriter->addAttValue("Layer",hepRepXMLWriter->typeDepth);

  // Handle color attribute, avoiding drawing anything black on black.
  G4Colour colour = GetColour(visible);
  float redness = colour.GetRed();
  float greenness = colour.GetGreen();
  float blueness = colour.GetBlue();

  if (redness==0. && greenness==0. && blueness==0.) {
    redness = 1.;
    greenness = 1.;
    blueness = 1.;
  }

  if (strcmp(primName,"Point")==0)
    hepRepXMLWriter->addAttValue("MarkColor",redness,greenness,blueness);
  else
    hepRepXMLWriter->addAttValue("LineColor",redness,greenness,blueness);

  // Handle visibility attribute.
  const G4VisAttributes* visAtts = visible.GetVisAttributes();
  if (visAtts && (visAtts->IsVisible()==0))
    hepRepXMLWriter->addAttValue("Visibility",false);
  else
    hepRepXMLWriter->addAttValue("Visibility",true);
}

void G4HepRepFileSceneHandler::CheckFileOpen() {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::CheckFileOpen called."
	 << G4endl;
#endif

  if (!hepRepXMLWriter->isOpen) {
    char* newFileSpec;
    newFileSpec = new char [100];
    int length;
    length = sprintf (newFileSpec, "%s%d%s","G4Data",fileCounter,".heprep");
    hepRepXMLWriter->open(newFileSpec);
#ifdef G4HEPREPFILEDEBUG
    G4cout <<
      "G4HepRepFileSceneHandler::CheckFileOpen opened file " << fileCounter
	   << G4endl;
#endif
    fileCounter++;

    hepRepXMLWriter->addAttDef("LVol", "Logical Volume", "Physics","");
    hepRepXMLWriter->addAttDef("Solid", "Solid Name", "Physics","");
    hepRepXMLWriter->addAttDef("EType", "Entity Type", "Physics","");
    hepRepXMLWriter->addAttDef("Material", "Material Name", "Physics","");
    hepRepXMLWriter->addAttDef("Density", "Material Density", "Physics","");
    hepRepXMLWriter->addAttDef("State", "Material State", "Physics","");
    hepRepXMLWriter->addAttDef("Radlen", "Material Radiation Length", "Physics","");
  }
}

void G4HepRepFileSceneHandler::ClearTransientStore () {
  G4VSceneHandler::ClearTransientStore ();
  if (fpViewer) {
    fpViewer -> SetView ();
    fpViewer -> ClearView ();
    fpViewer -> DrawView ();
  }
}

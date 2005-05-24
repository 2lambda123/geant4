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
// $Id: G4HepRepFileSceneHandler.cc,v 1.23 2005-05-24 23:02:42 perl Exp $
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
  G4VSceneHandler(system, fSceneIdCount++, name)
{
  fSceneCount++;

  hepRepXMLWriter = ((G4HepRepFile*)(&system))->GetHepRepXMLWriter();
  fileCounter = 0;
}


G4HepRepFileSceneHandler::~G4HepRepFileSceneHandler() {}


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


void G4HepRepFileSceneHandler::AddSolid(const G4Box& box) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Box& box) called for "
	 << box.GetName()
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Prism");
  hepRepXMLWriter->addPrimitive();

  G4double dx = box.GetXHalfLength();
  G4double dy = box.GetYHalfLength();
  G4double dz = box.GetZHalfLength();

  G4Point3D vertex1(G4Point3D( dx, dy,-dz));
  G4Point3D vertex2(G4Point3D( dx,-dy,-dz));
  G4Point3D vertex3(G4Point3D(-dx,-dy,-dz));
  G4Point3D vertex4(G4Point3D(-dx, dy,-dz));
  G4Point3D vertex5(G4Point3D( dx, dy, dz));
  G4Point3D vertex6(G4Point3D( dx,-dy, dz));
  G4Point3D vertex7(G4Point3D(-dx,-dy, dz));
  G4Point3D vertex8(G4Point3D(-dx, dy, dz));

  vertex1 = (*fpObjectTransformation) * vertex1;
  vertex2 = (*fpObjectTransformation) * vertex2;
  vertex3 = (*fpObjectTransformation) * vertex3;
  vertex4 = (*fpObjectTransformation) * vertex4;
  vertex5 = (*fpObjectTransformation) * vertex5;
  vertex6 = (*fpObjectTransformation) * vertex6;
  vertex7 = (*fpObjectTransformation) * vertex7;
  vertex8 = (*fpObjectTransformation) * vertex8;

  hepRepXMLWriter->addPoint(vertex1.x(), vertex1.y(), vertex1.z());
  hepRepXMLWriter->addPoint(vertex2.x(), vertex2.y(), vertex2.z());
  hepRepXMLWriter->addPoint(vertex3.x(), vertex3.y(), vertex3.z());
  hepRepXMLWriter->addPoint(vertex4.x(), vertex4.y(), vertex4.z());
  hepRepXMLWriter->addPoint(vertex5.x(), vertex5.y(), vertex5.z());
  hepRepXMLWriter->addPoint(vertex6.x(), vertex6.y(), vertex6.z());
  hepRepXMLWriter->addPoint(vertex7.x(), vertex7.y(), vertex7.z());
  hepRepXMLWriter->addPoint(vertex8.x(), vertex8.y(), vertex8.z());
}


void G4HepRepFileSceneHandler::AddSolid(const G4Cons& cons) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Cons& cons) called for "
	 << cons.GetName()
	 << G4endl;
  PrintThings();
#endif

  // HepRep does not have a primitive for a cut cone,
  // so if this cone is cut, let the base class convert this
  // solid to polygons.
  if (cons.GetDeltaPhiAngle() < twopi) {
    G4VSceneHandler::AddSolid(cons);  // Invoke default action.
  } else {
    AddHepRepInstance("Cylinder");

    G4Point3D vertex1(G4Point3D( 0., 0., cons.GetZHalfLength()));
    G4Point3D vertex2(G4Point3D( 0., 0.,-cons.GetZHalfLength()));

    vertex1 = (*fpObjectTransformation) * vertex1;
    vertex2 = (*fpObjectTransformation) * vertex2;

    // Outer cylinder.
    hepRepXMLWriter->addPrimitive();
    hepRepXMLWriter->addAttValue("Radius1",cons.GetOuterRadiusMinusZ());
    hepRepXMLWriter->addAttValue("Radius2",cons.GetOuterRadiusPlusZ());
    hepRepXMLWriter->addPoint(vertex1.x(), vertex1.y(), vertex1.z());
    hepRepXMLWriter->addPoint(vertex2.x(), vertex2.y(), vertex2.z());

    // Inner cylinder.
    hepRepXMLWriter->addPrimitive();
    hepRepXMLWriter->addAttValue("Radius1",cons.GetInnerRadiusMinusZ());
    hepRepXMLWriter->addAttValue("Radius2",cons.GetInnerRadiusPlusZ());
    hepRepXMLWriter->addPoint(vertex1.x(), vertex1.y(), vertex1.z());
    hepRepXMLWriter->addPoint(vertex2.x(), vertex2.y(), vertex2.z());
  }
}


void G4HepRepFileSceneHandler::AddSolid(const G4Tubs& tubs) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Tubs& tubs) called for "
	 << tubs.GetName()
	 << G4endl;
  PrintThings();
#endif

  // HepRep does not have a primitive for a cut cylinder,
  // so if this cylinder is cut, let the base class convert this
  // solid to polygons.
  if (tubs.GetDeltaPhiAngle() < twopi) {
    G4VSceneHandler::AddSolid(tubs);  // Invoke default action.
  } else {
    AddHepRepInstance("Cylinder");

    G4Point3D vertex1(G4Point3D( 0., 0., tubs.GetZHalfLength()));
    G4Point3D vertex2(G4Point3D( 0., 0.,-tubs.GetZHalfLength()));

    vertex1 = (*fpObjectTransformation) * vertex1;
    vertex2 = (*fpObjectTransformation) * vertex2;

    // Outer cylinder.
    hepRepXMLWriter->addPrimitive();
    hepRepXMLWriter->addAttValue("Radius1", tubs.GetOuterRadius());
    hepRepXMLWriter->addAttValue("Radius2", tubs.GetOuterRadius());
    hepRepXMLWriter->addPoint(vertex1.x(), vertex1.y(), vertex1.z());
    hepRepXMLWriter->addPoint(vertex2.x(), vertex2.y(), vertex2.z());

    // Inner cylinder.
    if (tubs.GetInnerRadius() != 0.) {
      hepRepXMLWriter->addPrimitive();
      hepRepXMLWriter->addAttValue("Radius1", tubs.GetInnerRadius());
      hepRepXMLWriter->addAttValue("Radius2", tubs.GetInnerRadius());
      hepRepXMLWriter->addPoint(vertex1.x(), vertex1.y(), vertex1.z());
      hepRepXMLWriter->addPoint(vertex2.x(), vertex2.y(), vertex2.z());
    }
  }
}


void G4HepRepFileSceneHandler::AddSolid(const G4Trd& trd) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Trd& trd) called for "
	 << trd.GetName()
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Prism");
  hepRepXMLWriter->addPrimitive();

  G4double dx1 = trd.GetXHalfLength1();
  G4double dy1 = trd.GetYHalfLength1();
  G4double dx2 = trd.GetXHalfLength2();
  G4double dy2 = trd.GetYHalfLength2();
  G4double dz = trd.GetZHalfLength();

  G4Point3D vertex1(G4Point3D( dx1, dy1,-dz));
  G4Point3D vertex2(G4Point3D( dx1,-dy1,-dz));
  G4Point3D vertex3(G4Point3D(-dx1,-dy1,-dz));
  G4Point3D vertex4(G4Point3D(-dx1, dy1,-dz));
  G4Point3D vertex5(G4Point3D( dx2, dy2, dz));
  G4Point3D vertex6(G4Point3D( dx2,-dy2, dz));
  G4Point3D vertex7(G4Point3D(-dx2,-dy2, dz));
  G4Point3D vertex8(G4Point3D(-dx2, dy2, dz));

  vertex1 = (*fpObjectTransformation) * vertex1;
  vertex2 = (*fpObjectTransformation) * vertex2;
  vertex3 = (*fpObjectTransformation) * vertex3;
  vertex4 = (*fpObjectTransformation) * vertex4;
  vertex5 = (*fpObjectTransformation) * vertex5;
  vertex6 = (*fpObjectTransformation) * vertex6;
  vertex7 = (*fpObjectTransformation) * vertex7;
  vertex8 = (*fpObjectTransformation) * vertex8;

  hepRepXMLWriter->addPoint(vertex1.x(), vertex1.y(), vertex1.z());
  hepRepXMLWriter->addPoint(vertex2.x(), vertex2.y(), vertex2.z());
  hepRepXMLWriter->addPoint(vertex3.x(), vertex3.y(), vertex3.z());
  hepRepXMLWriter->addPoint(vertex4.x(), vertex4.y(), vertex4.z());
  hepRepXMLWriter->addPoint(vertex5.x(), vertex5.y(), vertex5.z());
  hepRepXMLWriter->addPoint(vertex6.x(), vertex6.y(), vertex6.z());
  hepRepXMLWriter->addPoint(vertex7.x(), vertex7.y(), vertex7.z());
  hepRepXMLWriter->addPoint(vertex8.x(), vertex8.y(), vertex8.z());
}


void G4HepRepFileSceneHandler::AddSolid(const G4Trap& trap) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Trap& trap) called for "
	 << trap.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddSolid(trap);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddSolid(const G4Sphere& sphere) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Sphere& sphere) called for "
	 << sphere.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddSolid(sphere);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddSolid(const G4Para& para) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Para& para) called for "
	 << para.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddSolid(para);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddSolid(const G4Torus& torus) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Torus& torus) called for "
	 << torus.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddSolid(torus);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddSolid(const G4Polycone& polycone) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Polycone& polycone) called for "
	 << polycone.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddSolid(polycone);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddSolid(const G4Polyhedra& polyhedra) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Polyhedra& polyhedra) called for "
	 << polyhedra.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddSolid(polyhedra);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddSolid(const G4VSolid& solid) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddSolid(const G4Solid& solid) called for "
	 << solid.GetName()
	 << G4endl;
  PrintThings();
#endif
  G4VSceneHandler::AddSolid(solid);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddCompound (const G4VTrajectory& traj) {
#ifdef G4HEPREPFILEDEBUG
    G4cout << "G4HepRepFileSceneHandler::AddCompound(G4VTrajectory&) " << G4endl;
#endif

  std::vector<G4AttValue>* attValues = traj.CreateAttValues();
  std::vector<G4AttValue>::iterator iAttVal;
  const std::map<G4String,G4AttDef>* attDefs = traj.GetAttDefs();
  std::map<G4String,G4AttDef>::const_iterator iAttDef;
  G4int i;

  // Open the HepRep output file if it is not already open.
  CheckFileOpen();

  // Add the Event Data Type if it hasn't already been added.
  // If this is the first trajectory or hit, add the Event Data type and define attributes.
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
	G4cout << "G4HepRepFileSceneHandler::AddCompound(traj):"
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


void G4HepRepFileSceneHandler::AddCompound (const G4VHit& hit) {
#ifdef G4HEPREPFILEDEBUG
    G4cout << "G4HepRepFileSceneHandler::AddCompound(G4VHit&) " << G4endl;
#endif
    G4VSceneHandler::AddCompound(hit);  // Invoke default action.
}


void G4HepRepFileSceneHandler::AddPrimitive(const G4Polyline& polyline) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddPrimitive(const G4Polyline& polyline) called:"
    "\n  polyline: " << polyline
	 << G4endl;
  PrintThings();
#endif

  AddHepRepInstance("Line");

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

  AddHepRepInstance("Point");

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

  AddHepRepInstance("Point");

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

  AddHepRepInstance("Point");

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

  AddHepRepInstance("Polygon");

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


// Space holder method that has to be here to avoid bizarre Microsoft link error.
void G4HepRepFileSceneHandler::UnusedMethod() {
}

void G4HepRepFileSceneHandler::AddHepRepInstance(const char* primName) {
#ifdef G4HEPREPFILEDEBUG
  G4cout <<
    "G4HepRepFileSceneHandler::AddHepRepInstance called."
	 << G4endl;
#endif

  // Open the HepRep output file if it is not already open.
  CheckFileOpen();

  // Should be able to just test on fReadyForTransients, but this seems to
  // be falsely false if no geometry has yet been drawn.
  // So I test on both !fpCurrentPV (means no geometry has yet been drawn)
  // and fReadyForTransients.
  if (!fpCurrentPV || fReadyForTransients) {
    if (strcmp("Event Data",hepRepXMLWriter->prevTypeName[0])!=0) {
      hepRepXMLWriter->addType("Event Data",0);
      hepRepXMLWriter->addInstance();
    }

    // Applications have the option of either calling AddSolid(G4VTrajectory&) and
    // AddSolid(G4VHits&), or of just decomposing these into simpler primitives.
    // In the former case, drawing will be handled above and will include setting of
    // physics attributes.
    // In the latter case, which is an older style of working, we end up drawing the
    // trajectories and hits here, where we have no access to physics attributes. 
    // We receive primitives here.  We can figure out that these are transients, but we
    // have to guess exactly what these transients represent.
    // We assume the primitives are being used as in G4VTrajectory, hence we assume:
    // Lines are Trajectories
    // Squares that come after we've seen trajectories are Auxiliary Points
    // Circles that come after we've seen trajectories are Step Points
    // Other primitives are Hits

    int layer;

    if (strcmp("Line",primName)==0) {
      hepRepXMLWriter->addType("Trajectories",1);
      layer = 100;
    } else {
      if (strcmp(hepRepXMLWriter->prevTypeName[1],"Trajectories")==0 &&
	  strcmp("Square",primName)==0)
      {
	hepRepXMLWriter->addType("AuxiliaryPoints",2);
	layer = 110;
      } else {
        if (strcmp(hepRepXMLWriter->prevTypeName[1],"Trajectories")==0 &&
	    strcmp("Circle",primName)==0)
	{
	  hepRepXMLWriter->addType("StepPoints",2);
	  layer = 120;
	} else {
	  hepRepXMLWriter->addType("Hits",1);
	  layer = 130;
	}
      }
    }

    hepRepXMLWriter->addAttValue("Layer",layer);

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
    hepRepXMLWriter->addAttValue("Layer",hepRepXMLWriter->typeDepth);
    hepRepXMLWriter->addAttValue("LVol", fpCurrentLV->GetName());
    hepRepXMLWriter->addAttValue("Region", fpCurrentLV->GetRegion()->GetName());
    hepRepXMLWriter->addAttValue("RootRegion", fpCurrentLV->IsRootRegion());
    hepRepXMLWriter->addAttValue("Solid", fpCurrentLV->GetSolid()->GetName());
    hepRepXMLWriter->addAttValue("EType", fpCurrentLV->GetSolid()->GetEntityType());
    hepRepXMLWriter->addAttValue("Material", fpCurrentLV->GetMaterial()->GetName());
    hepRepXMLWriter->addAttValue("Density", fpCurrentLV->GetMaterial()->GetDensity());
    hepRepXMLWriter->addAttValue("State", fpCurrentLV->GetMaterial()->GetState());
    hepRepXMLWriter->addAttValue("Radlen", fpCurrentLV->GetMaterial()->GetRadlen());
  }

  hepRepXMLWriter->addAttValue("DrawAs",primName);

  // Handle color attribute, avoiding drawing anything black on black.
  G4Colour colour = fpVisAttribs->GetColour();
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
  if (fpVisAttribs && (fpVisAttribs->IsVisible()==0))
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
    hepRepXMLWriter->addAttDef("Region", "Cuts Region", "Physics","");
    hepRepXMLWriter->addAttDef("RootRegion", "Root Region", "Physics","");
    hepRepXMLWriter->addAttDef("Solid", "Solid Name", "Physics","");
    hepRepXMLWriter->addAttDef("EType", "Entity Type", "Physics","");
    hepRepXMLWriter->addAttDef("Material", "Material Name", "Physics","");
    hepRepXMLWriter->addAttDef("Density", "Material Density", "Physics","");
    hepRepXMLWriter->addAttDef("State", "Material State", "Physics","");
    hepRepXMLWriter->addAttDef("Radlen", "Material Radiation Length", "Physics","");
  }
}


void G4HepRepFileSceneHandler::ClearTransientStore() {
  G4VSceneHandler::ClearTransientStore();
  /*
  ClearTransientStore should restrict itself to its job.  In other
  places, a draw command follows, so it is not needed here.  In fact
  it can cause a double recursive descent into DrawView, so the following
  has been commented out (JA - 23/Jan/05).
  if (fpViewer) {
    fpViewer -> SetView();
    fpViewer -> ClearView();
    fpViewer -> DrawView();
  }
  */
}

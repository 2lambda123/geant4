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
// $Id: G4VRML1FileSceneHandler.hh,v 1.8 2001-07-27 22:33:08 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4VRML1FileSceneHandler.hh
// Satoshi Tanaka & Yasuhide Sawada

#ifndef G4VRML1FILE_SCENE_HANDLER_HH
#define G4VRML1FILE_SCENE_HANDLER_HH

#include "g4std/fstream"

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4VSceneHandler.hh"


class G4VRML1File;
class G4VisAttributes;

class G4VRML1FileSceneHandler: public G4VSceneHandler {


// methods (public) 
public:
	G4VRML1FileSceneHandler(G4VRML1File& system, const G4String& name = "");
	virtual ~G4VRML1FileSceneHandler();
	void AddThis(const G4Box&);
	void AddThis(const G4Cons&);
	void AddThis(const G4Tubs&);
	void AddThis(const G4Trd&);
	void AddThis(const G4Trap&);
	void AddThis(const G4Sphere&);
        void AddThis(const G4Para&);
	void AddThis(const G4Torus&);
        void AddThis ( const G4Polycone& polycone ) {
          G4VSceneHandler::AddThis (polycone);
        }
        void AddThis ( const G4Polyhedra& polyhedra) {
          G4VSceneHandler::AddThis (polyhedra);
        }
        void AddThis(const G4VSolid&);

	void BeginPrimitives(const G4Transform3D& objectTransformation);
	void EndPrimitives();

	void AddPrimitive(const G4Polyline&);
	void AddPrimitive(const G4Polyhedron&);
	void AddPrimitive(const G4NURBS&); 
	void AddPrimitive(const G4Text&); 
	void AddPrimitive(const G4Circle&);
	void AddPrimitive(const G4Square&);
	void AddPrimitive (const G4Polymarker& polymarker)
		{ G4VSceneHandler::AddPrimitive (polymarker); }

	void ClearStore();

	void BeginModeling();
	void EndModeling();

	void VRMLBeginModeling();
	void VRMLEndModeling();

	static G4int GetSceneCount() { return fSceneCount; }

	void connectPort();
	void closePort();

// methods (private) 
private:

	void SendMaterialNode( const G4VisAttributes*  pAV ); // NEW NEW NEW
	void SendMaterialNode();
	void SendMatrixTransformNode(const G4Transform3D *);
	void SendCubeNode(G4double, G4double, G4double);
	void SendCylinderNode(G4double, G4double);
	void SendSphereNode(G4double);

	void      SendMarkerColor         ( const G4VMarker&  mark ) ;
	void      SendMarkerWorldPosition ( const G4VMarker&  mark ) ;
	G4double  GetMarkerHalfSize       ( const G4VMarker&  mark ) ;

	G4bool    isConnected      () { return fFlagDestOpen ; }

// data 
private:
	char fVRMLFileDestDir[256] ; 
	char fVRMLFileName[256]    ; 

	G4String fCurrentDEF;

	G4VRML1File& fSystem;	// Graphics system for this scene.
	G4std::ofstream     fDest ;
	G4bool       fFlagDestOpen ;

	G4int        fMaxFileNum   ;	

	static G4int fSceneIdCount;
	static G4int fSceneCount;    // No. of existing scenes.

};

#endif //G4VRML1File_SCENE_HH

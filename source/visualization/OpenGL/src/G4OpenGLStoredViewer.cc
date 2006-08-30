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
// $Id: G4OpenGLStoredViewer.cc,v 1.17 2006-08-30 11:56:15 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Andrew Walkden  7th February 1997
// Class G4OpenGLStoredViewer : Encapsulates the `storedness' of
//                            an OpenGL view, for inheritance by
//                            derived (X, Xm...) classes.

#ifdef G4VIS_BUILD_OPENGL_DRIVER

#include "G4OpenGLStoredViewer.hh"

#include "G4OpenGLStoredSceneHandler.hh"
#include "G4Text.hh"
#include "G4Circle.hh"
#include "G4UnitsTable.hh"

G4OpenGLStoredViewer::G4OpenGLStoredViewer
(G4OpenGLStoredSceneHandler& sceneHandler):
G4VViewer (sceneHandler, -1),  
G4OpenGLViewer (sceneHandler), 
fG4OpenGLStoredSceneHandler (sceneHandler)
{
  fLastVP = fDefaultVP; // Not sure if this gets executed before or
  // after G4VViewer::G4VViewer!!  Doesn't matter much.
}

G4OpenGLStoredViewer::~G4OpenGLStoredViewer () {}

void G4OpenGLStoredViewer::KernelVisitDecision () {
  
  // If there's a significant difference with the last view parameters
  // of either the scene handler or this viewer, trigger a rebuild.

  if (!fG4OpenGLStoredSceneHandler.fTopPODL ||
      CompareForKernelVisit(fLastVP)) {
    NeedKernelVisit ();
  }      
  fLastVP = fVP;
}

G4bool G4OpenGLStoredViewer::CompareForKernelVisit(G4ViewParameters& lastVP) {

  if (
      (lastVP.GetDrawingStyle ()    != fVP.GetDrawingStyle ())    ||
      (lastVP.IsAuxEdgeVisible ()   != fVP.IsAuxEdgeVisible ())   ||
      (lastVP.GetRepStyle ()        != fVP.GetRepStyle ())        ||
      (lastVP.IsCulling ()          != fVP.IsCulling ())          ||
      (lastVP.IsCullingInvisible () != fVP.IsCullingInvisible ()) ||
      (lastVP.IsDensityCulling ()   != fVP.IsDensityCulling ())   ||
      (lastVP.IsCullingCovered ()   != fVP.IsCullingCovered ())   ||
      // No need to visit kernel if section plane changes.
      /**************************************************************
      But...OpenGL no longer seems to reconstruct clipped edges, so
      if we abandon this and use generic clipping in
      G4OpenGLSceneHandler::CreateSectionPolyhedron we need to force
      kernel visit here and below...
      (lastVP.IsSection ()          != fVP.IsSection ())          ||
      ***************************************************************/
      // No need to visit kernel if cutaway planes change.
      /*************************************************************
      But as for section planes...
      (lastVP.IsCutaway ()          != fVP.IsCutaway ())          ||
      ***************************************************************/
      (lastVP.IsExplode ()          != fVP.IsExplode ())          ||
      (lastVP.GetNoOfSides ()       != fVP.GetNoOfSides ())       ||
      (lastVP.IsMarkerNotHidden ()  != fVP.IsMarkerNotHidden ())  ||
      (lastVP.GetBackgroundColour ()!= fVP.GetBackgroundColour ())
      ) {
    return true;
  }

  if (lastVP.IsDensityCulling () &&
      (lastVP.GetVisibleDensity () != fVP.GetVisibleDensity ()))
    return true;

  /**************************************************************
  if (lastVP.IsSection () &&
      (lastVP.GetSectionPlane () != fVP.GetSectionPlane ()))
    return true;
  ***************************************************************/

  /**************************************************************
  if (lastVP.IsCutaway ()) {
    if (lastVP.GetCutawayPlanes ().size () !=
	fVP.GetCutawayPlanes ().size ()) return true;
    for (size_t i = 0; i < lastVP.GetCutawayPlanes().size(); ++i)
      if (lastVP.GetCutawayPlanes()[i] != fVP.GetCutawayPlanes()[i])
	return true;
  }
  ***************************************************************/

  if (lastVP.IsExplode () &&
      (lastVP.GetExplodeFactor () != fVP.GetExplodeFactor ()))
    return true;

  return false;
}

void G4OpenGLStoredViewer::DrawDisplayLists () {
  
  if (fG4OpenGLStoredSceneHandler.fTopPODL)
    glCallList (fG4OpenGLStoredSceneHandler.fTopPODL);
  
  for (size_t i = 0; i < fG4OpenGLStoredSceneHandler.fTOList.size(); ++i) {
    G4OpenGLStoredSceneHandler::TO& to =
      fG4OpenGLStoredSceneHandler.fTOList[i];
    if (to.fEndTime >= fStartTime && to.fEndTime <= fEndTime) {
      glPushMatrix();
      G4OpenGLTransform3D oglt (to.fTransform);
      glMultMatrixd (oglt.GetGLMatrix ());
      G4Colour& c = to.fColour;
      G4double bsf = 1.;  // Brightness scaling factor.
      if (fFadeFactor > 0. && to.fEndTime < fEndTime)
	bsf = 1. - fFadeFactor *
	  ((fEndTime - to.fEndTime) / (fEndTime - fStartTime));
      glColor3d(bsf * c.GetRed (), bsf * c.GetGreen (), bsf * c.GetBlue ());
      glCallList (to.fDisplayListId);
      glPopMatrix();
    }
  }

  // Display time at "head" of time range, which is fEndTime...
  if (fDisplayHeadTime && fEndTime < DBL_MAX) {
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho (-1., 1., -1., 1., -DBL_MAX, DBL_MAX);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    G4Text headTimeText(G4BestUnit(fEndTime,"Time"),
			G4Point3D(fDisplayHeadTimeX, fDisplayHeadTimeY, 0.));
    headTimeText.SetScreenSize(fDisplayHeadTimeSize);
    G4VisAttributes visAtts (G4Colour
			     (fDisplayHeadTimeRed,
			      fDisplayHeadTimeGreen,
			      fDisplayHeadTimeBlue));
    headTimeText.SetVisAttributes(&visAtts);
    fG4OpenGLStoredSceneHandler.AddPrimitive(headTimeText);
    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix();
  }

  // Display light front...
  if (fDisplayLightFront && fEndTime < DBL_MAX) {
    G4double radius = 0.;
    if (fEndTime > 0.) {
      radius = fEndTime * c_light;
    }
    G4Circle lightFront(G4Point3D
			(fDisplayLightFrontX,
			 fDisplayLightFrontY,
			 fDisplayLightFrontZ));
    lightFront.SetWorldRadius(radius);
    glColor3d(fDisplayLightFrontRed,
	      fDisplayLightFrontGreen,
	      fDisplayLightFrontBlue);
    static_cast<G4OpenGLSceneHandler&>(fSceneHandler).
      G4OpenGLSceneHandler::AddPrimitive(lightFront);
  }
}

#endif

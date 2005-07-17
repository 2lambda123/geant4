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
// $Id: G4RayTracerX.cc,v 1.1 2005-07-17 13:59:24 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//

#include "G4RayTracerX.hh"

#include "G4RayTracerFeatures.hh"
#include "G4RTXScanner.hh"
#include "G4RayTracerSceneHandler.hh"
#include "G4RayTracerXViewer.hh"

G4RayTracerX::G4RayTracerX():
  G4RayTracer(0, new G4RTXScanner)
{
  backgroundColour = G4Colour(0.,0.,0.);
}

G4VSceneHandler* G4RayTracerX::CreateSceneHandler (const G4String& name) {
  G4VSceneHandler* pSceneHandler = new G4RayTracerSceneHandler (*this, name);
  return pSceneHandler;
}

G4VViewer* G4RayTracerX::CreateViewer (G4VSceneHandler& sceneHandler,
				       const G4String& name) {
  G4VViewer* pViewer = new G4RayTracerXViewer (sceneHandler, name);
  if (pViewer) {
    if (pViewer->GetViewId() < 0) {
      G4cerr << "G4RayTracerX::CreateViewer: error flagged by negative"
        "\n  view id in G4RayTracerXViewer creation."
        "\n Destroying view and returning null pointer."
           << G4endl;
      delete pViewer;
      pViewer = 0;
    }
  }
  return pViewer;
}

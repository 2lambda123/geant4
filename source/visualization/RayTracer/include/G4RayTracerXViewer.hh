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
// $Id: G4RayTracerXViewer.hh,v 1.1 2005-07-17 13:59:24 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

// John Allison  17th March 2000

#ifndef G4RAYTRACERXVIEWER_HH
#define G4RAYTRACERXVIEWER_HH

#include "G4RayTracerViewer.hh"

class G4RayTracerXViewer: public G4RayTracerViewer {
public:
  G4RayTracerXViewer(G4VSceneHandler&,const G4String& name);
  virtual ~G4RayTracerXViewer();
};

#endif

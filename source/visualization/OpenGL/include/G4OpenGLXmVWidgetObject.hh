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
// $Id: G4OpenGLXmVWidgetObject.hh,v 1.5 2001-07-11 10:08:52 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//Virtual base class for all Motif widgets.

#ifdef G4VIS_BUILD_OPENGLXM_DRIVER

#ifndef G4OPENGLXMVWIDGETOBJECT_HH
#define G4OPENGLXMVWIDGETOBJECT_HH

#include "globals.hh"
#include <Xm/Xm.h>
#include "X11/Intrinsic.h"
#include <X11/Xlib.h>
#include "G4OpenGLXmViewer.hh"

class G4OpenGLXmVWidgetObject {

public:

  G4OpenGLXmVWidgetObject ();          //constructor
  virtual ~G4OpenGLXmVWidgetObject (); //destructor
  
  G4OpenGLXmViewer* GetView ();  //access to the pView
  void ProcesspView ();

protected:
  G4OpenGLXmVWidgetObject (const G4OpenGLXmVWidgetObject&);
  G4OpenGLXmVWidgetObject& operator = (const G4OpenGLXmVWidgetObject&);
  G4OpenGLXmViewer* pView;
  Colormap cmap;
  Pixel borcol;
  Pixel bgnd;
  unsigned int depth;
  Visual* visual;
  Widget top;
};

#endif

#endif

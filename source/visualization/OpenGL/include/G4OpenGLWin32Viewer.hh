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
// $Id: G4OpenGLWin32Viewer.hh,v 1.12 2006-05-30 06:25:31 perl Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// G4OpenGLWin32Viewer : Class to provide WindowsNT specific
//                       functionality for OpenGL in GEANT4

#ifdef G4VIS_BUILD_OPENGLWIN32_DRIVER

#ifndef G4OPENGLWIN32VIEWER_HH
#define G4OPENGLWIN32VIEWER_HH

#include "globals.hh"

#include "G4VViewer.hh"
#include "G4OpenGLSceneHandler.hh"

class G4OpenGLSceneHandler;

class G4OpenGLWin32Viewer: virtual public G4OpenGLViewer {

public:
  G4OpenGLWin32Viewer (G4OpenGLSceneHandler& scene);
  virtual ~G4OpenGLWin32Viewer ();
  void SetView ();
  void ShowView ();
protected:
  void GetWin32Connection ();
  void CreateGLWin32Context ();
  virtual void CreateMainWindow ();
protected:
  G4int WinSize_x;
  G4int WinSize_y;
  HDC fHDC;
private:
  static LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
  static bool SetWindowPixelFormat(HDC);
private:
  HWND fWindow;
  HGLRC fHGLRC;
};

#endif

#endif

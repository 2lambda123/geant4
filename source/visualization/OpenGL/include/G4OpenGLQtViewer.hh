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
// $Id: G4OpenGLQtViewer.hh,v 1.6 2008-01-15 11:05:08 lgarnier Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// G4OpenGLQtViewer : Class to provide WindowsNT specific
//                       functionality for OpenGL in GEANT4

#ifdef G4VIS_BUILD_OPENGLQT_DRIVER

#ifndef G4OPENGLQTVIEWER_HH
#define G4OPENGLQTVIEWER_HH

#include "globals.hh"

#include "G4VViewer.hh"
#include "G4OpenGLSceneHandler.hh"

#include <qobject.h>
#include <qpoint.h>

class QGLWidget;
class QDialog;
class QContextMenuEvent;
#if QT_VERSION < 0x040000
class QPopupMenu;
#else
class QMenu;
#endif
class QImage;
class QAction;

class G4OpenGLSceneHandler;

class G4OpenGLQtViewer: public QObject, virtual public G4OpenGLViewer {

  Q_OBJECT

public:
  G4OpenGLQtViewer (G4OpenGLSceneHandler& scene);
  virtual ~G4OpenGLQtViewer ();
  void SetView ();
  void ShowView ();
  virtual void updateQWidget()=0;
  void setupViewport(int, int);

protected:
  void CreateGLQtContext ();
  virtual void CreateMainWindow (QGLWidget*);
  void manageContextMenuEvent(QContextMenuEvent *e);
  void G4MousePressEvent(QPoint);
  void G4MouseReleaseEvent(QPoint p);
  void G4MouseDoubleClickEvent(QPoint p);
#if QT_VERSION < 0x040000
  void G4MouseMoveEvent(int, int, Qt::ButtonState);
#else
  void G4MouseMoveEvent(int, int, Qt::MouseButtons);
#endif


protected:
  G4int WinSize_x;
  G4int WinSize_y;
  QGLWidget* fWindow;
  QDialog* GLWindow;

private:
  void createPopupMenu();
  void createRadioAction(QAction *,QAction *, const std::string&,unsigned int a=1);
  void rescaleImage(int, int);
  bool generateEPS(QString,int,QImage);  
  bool generateVectorEPS (QString,int,int,QImage);
  bool generatePS_PDF(QString,int,QImage);  

#if QT_VERSION < 0x040000
  QPopupMenu *fContextMenu;
#else
  QMenu *fContextMenu;
#endif
  enum mouseActions {ROTATE, MOVE, ZOOM, PICK}; 
  mouseActions fMouseAction; // 1: rotate 0:move
  QPoint lastPos;
#if QT_VERSION < 0x040000
  QPopupMenu *fMouseRotate;
  QPopupMenu *fMouseMove;
  QPopupMenu *fMouseZoom;
  QPopupMenu *fMousePick;
  QPopupMenu *fDrawingWireframe;
  QPopupMenu *fDrawingLineRemoval;
  QPopupMenu *fDrawingSurfaceRemoval;
  QPopupMenu *fDrawingLineSurfaceRemoval;
#else
  QAction *fMouseRotate;
  QAction *fMouseMove;
  QAction *fMouseZoom;
  QAction *fMousePick;
  QAction *fDrawingWireframe;
  QAction *fDrawingLineRemoval;
  QAction *fDrawingSurfaceRemoval;
  QAction *fDrawingLineSurfaceRemoval;
#endif

private slots :
  void actionMouseRotate();
  void actionMouseMove();
  void actionMouseZoom();
  void actionMousePick();
  void actionDrawingWireframe();
  void actionDrawingLineRemoval();
  void actionDrawingSurfaceRemoval();
  void actionDrawingLineSurfaceRemoval();
  void actionCreateEPS();

  void toggleDrawingAction(int);
  void toggleMouseAction(mouseActions);
  void toggleRepresentation(bool);
  void toggleBackground(bool);
  void toggleTransparency(bool);
  void toggleAntialiasing(bool);
  void toggleHaloing(bool);
  void toggleAux(bool);
  void toggleFullScreen(bool);
  // Only use for Qt>4.0
  void dialogClosed();
};

#endif

#endif

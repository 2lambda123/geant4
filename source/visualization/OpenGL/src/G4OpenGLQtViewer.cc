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
// $Id: G4OpenGLQtViewer.cc,v 1.22 2008-03-14 09:42:04 lgarnier Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// G4OpenGLQtViewer : Class to provide Qt specific
//                     functionality for OpenGL in GEANT4
//
// 27/06/2003 : G.Barrand : implementation (at last !).

#ifdef G4VIS_BUILD_OPENGLQT_DRIVER

//#define GEANT4_QT_DEBUG

#include "G4OpenGLQtViewer.hh"

#include "G4ios.hh"
#include "G4VisExtent.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Point3D.hh"
#include "G4Normal3D.hh"
#include "G4Scene.hh"
#include "G4OpenGLQtExportDialog.hh"
#include "G4OpenGLQtMovieDialog.hh"
#include "G4UnitsTable.hh"
#include "G4Qt.hh"
#include "G4UImanager.hh"
#include "G4UIcommandTree.hh"
#include <qapplication.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qprocess.h>

#if QT_VERSION >= 0x040000
#include <qmenu.h>
#include <qimagewriter.h>
#else
#include <qaction.h>
#include <qwidgetlist.h>
#include <qpopupmenu.h>
#include <qimage.h>
#endif

#include <qapplication.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qgl.h> // include <qglwidget.h>
#include <qdialog.h>
#include <qevent.h> //include <qcontextmenuevent.h>


//////////////////////////////////////////////////////////////////////////////
/**
   Implementation of virtual method of G4VViewer
*/
void G4OpenGLQtViewer::SetView (
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::SetView ++++++++++++++++++++\n");
#endif
  //   if(!fHDC) return;
  //   if(!fHGLRC) return;
  //   ::wglMakeCurrent(fHDC,fHGLRC);
  //  fWindow->makeCurrent();
  G4OpenGLViewer::SetView ();
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::SetView --------------------\n");
#endif
}

/**
 * Set the viewport of the scene
 */
void G4OpenGLQtViewer::setupViewport(int aWidth, int aHeight)
{
  int side = aWidth;
  if (aHeight < aWidth) side = aHeight;
  glViewport((aWidth - side) / 2, (aHeight - side) / 2, side, side);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
}


//////////////////////////////////////////////////////////////////////////////
/**
   Implementation of virtual method of G4VViewer
*/
void G4OpenGLQtViewer::ShowView (
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::ShowView  +++++++++++++++++++++\n");
#endif
  if (!GLWindow) {
    G4cerr << "Visualization window not defined, please choose one before\n" << G4endl;
  } else {
#if QT_VERSION < 0x040000
    GLWindow->setActiveWindow();
#else
    GLWindow->activateWindow();
#endif
#ifdef GEANT4_QT_DEBUG
    printf("G4OpenGLQtViewer::ShowView -----------------------\n");
#endif
  }
  glFlush ();
  //   // Empty the Windows message queue :
  //   MSG event;
  //   while ( ::PeekMessage(&event, NULL, 0, 0, PM_REMOVE) ) {
  //     ::TranslateMessage(&event);
  //     ::DispatchMessage (&event);
  //   }
}



//////////////////////////////////////////////////////////////////////////////
void G4OpenGLQtViewer::CreateGLQtContext (
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::CreateGLQtContext \n");
#endif
}


//////////////////////////////////////////////////////////////////////////////
void G4OpenGLQtViewer::CreateMainWindow (
 QGLWidget* glWidget
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  if(fWindow) return; //Done.
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::CreateMainWindow glWidget\n");
#endif

  // launch Qt if not
  G4Qt* interactorManager = G4Qt::getInstance ();
  //  G4UImanager* UI = G4UImanager::GetUIpointer();

  fWindow = glWidget ;
  //  fWindow->makeCurrent();

  // create window
  if (((QApplication*)interactorManager->GetMainInteractor())) {
    // look for the main window
    bool found = false;
#if QT_VERSION < 0x040000
    // theses lines does nothing exept this one "GLWindow = new QDialog(0..."
    // but if I comment them, it doesn't work...
    QWidgetList  *list = QApplication::allWidgets();
    QWidgetListIt it( *list );         // iterate over the widgets
    QWidget * widget;
    while ( (widget=it.current()) != 0 ) {  // for each widget...
      ++it;
      if ((found== false) && (widget->inherits("QMainWindow"))) {
        GLWindow = new QDialog(0,0,FALSE,Qt::WStyle_Title | Qt::WStyle_SysMenu | Qt::WStyle_MinMax );
        found = true;
      }
    }
    delete list;                      // delete the list, not the widgets
#else
    foreach (QWidget *widget, QApplication::allWidgets()) {
      if ((found== false) && (widget->inherits("QMainWindow"))) {
        GLWindow = new QDialog(0,Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
        found = true;
      }
    }
#endif

#if QT_VERSION < 0x040000
    glWidget->reparent(GLWindow,0,QPoint(0,0));  
#else
    glWidget->setParent(GLWindow);  
#endif

    if (found==false) {
#ifdef GEANT4_QT_DEBUG
      printf("G4OpenGLQtViewer::CreateMainWindow case Qapp exist, but not found\n");
#endif
      GLWindow = new QDialog();
    }
  } else {
#ifdef GEANT4_QT_DEBUG
    printf("G4OpenGLQtViewer::CreateMainWindow case Qapp exist\n");
#endif
    GLWindow = new QDialog();
  }

  QHBoxLayout *mainLayout = new QHBoxLayout(GLWindow);

  mainLayout->addWidget(fWindow);

#if QT_VERSION < 0x040000
  GLWindow->setCaption( tr( "QGl Viewer" ));
#else
  GLWindow->setLayout(mainLayout);
  GLWindow->setWindowTitle(tr("QGl Viewer"));
#endif
  GLWindow->resize(300, 300);
  GLWindow->move(900,300);
  GLWindow->show();
  
  // delete the pointer if close this
  //  GLWindow->setAttribute(Qt::WA_DeleteOnClose);

#if QT_VERSION >= 0x040000
//   QObject ::connect(GLWindow, 
//                     SIGNAL(rejected()),
//                     this, 
//                     SLOT(dialogClosed()));
#endif

  WinSize_x = 400;
  WinSize_y = 400;
  if (WinSize_x < fVP.GetWindowSizeHintX ())
    WinSize_x = fVP.GetWindowSizeHintX ();
  if (WinSize_y < fVP.GetWindowSizeHintY ())
    WinSize_y = fVP.GetWindowSizeHintY ();

  if(!fWindow) return;
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::CreateMainWindow glWidget END\n");
#endif

  if (!fContextMenu) 
    createPopupMenu();

}

#if QT_VERSION >= 0x040000
/**  Close the dialog and set the pointer to NULL
 */
// void G4OpenGLQtViewer::dialogClosed() {
// #ifdef GEANT4_QT_DEBUG
//   printf("G4OpenGLQtViewer::dialogClosed END\n");
// #endif
//   //  GLWindow = NULL;
// }
#endif

//////////////////////////////////////////////////////////////////////////////
G4OpenGLQtViewer::G4OpenGLQtViewer (
                                    G4OpenGLSceneHandler& scene
                                    )
  :G4VViewer (scene, -1)
  ,G4OpenGLViewer (scene)
  ,fWindow(0)
  ,fRecordFrameNumber(0)
  ,fContextMenu(0)
  ,fMouseAction(STYLE1)
  ,fDeltaRotation(1)
  ,fDeltaSceneTranslation(0.01)
  ,fDeltaDepth(0.01)
  ,fDeltaZoom(0.1)
  ,fDeltaMove(0.05)
  ,fHoldKeyEvent(false)
  ,fHoldMoveEvent(false)
  ,fHoldRotateEvent(false)
  ,fAutoMove(false)
  ,fEncoderPath("")
  ,fTempFolderPath("")
  ,fMovieTempFolderPath("")
  ,fSaveFileName("")
  ,fParameterFileName("mpeg_encode_parameter_file.par")
  ,fMovieParametersDialog(NULL)
  ,fRecordingStep(WAIT)
  ,fProcess(NULL)
{

  initMovieParameters();

#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::G4OpenGLQtViewer \n");
#endif
}

//////////////////////////////////////////////////////////////////////////////
G4OpenGLQtViewer::~G4OpenGLQtViewer (
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#if QT_VERSION < 0x040000
  G4cout <<removeTempFolder().ascii() <<G4endl;
#else
  G4cout <<removeTempFolder().toStdString().c_str() <<G4endl;
#endif

#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::~G4OpenGLQtViewer \n");
#endif
}


/**
   Create a popup menu for the widget. This menu is activated by right-mouse click
*/
void G4OpenGLQtViewer::createPopupMenu()    {

#if QT_VERSION < 0x040000
  fContextMenu = new QPopupMenu( GLWindow,"All" );
#else
  fContextMenu = new QMenu("All");
#endif

#if QT_VERSION < 0x040000
  QPopupMenu *mMouseAction = new QPopupMenu( fContextMenu );
  fContextMenu->insertItem("&Mouse actions",mMouseAction);
#if QT_VERSION < 0x030200
  fRotateAction =  new QAction("&Rotate","&Rotate",CTRL+Key_R,mMouseAction,0,true);
  fMoveAction =  new QAction("&Move","&Move",CTRL+Key_M,mMouseAction,0,true);
  fPickAction =  new QAction("&Pick","&Pick",CTRL+Key_P,mMouseAction,0,true);
  QAction * shortcutsAction =  new QAction("&Show shortcuts","&Show shortcuts",CTRL+Key_S,mMouseAction,0,true);
#else
  fRotateAction =  new QAction("&Rotate",CTRL+Key_R,mMouseAction);
  fMoveAction =  new QAction("&Move",CTRL+Key_M,mMouseAction);
  fPickAction =  new QAction("&Pick",CTRL+Key_P,mMouseAction);
  QAction *shortcutsAction =  new QAction("&Show shortcuts",CTRL+Key_S,mMouseAction);
#endif
  fRotateAction->addTo(mMouseAction);
  fMoveAction->addTo(mMouseAction);
  fPickAction->addTo(mMouseAction);
  shortcutsAction->addTo(mMouseAction);

  fRotateAction->setToggleAction(true);
  fMoveAction->setToggleAction(true);
  fPickAction->setToggleAction(true);
  shortcutsAction->setToggleAction(true);

  fRotateAction->setOn(true);
  fMoveAction->setOn(false);
  fPickAction->setOn(false);
  shortcutsAction->setOn(false);


  QObject ::connect(fRotateAction, 
                    SIGNAL(activated()),
                    this,
                    SLOT(actionMouseRotate()));

  QObject ::connect(fMoveAction, 
                    SIGNAL(activated()),
                    this,
                    SLOT(actionMouseMove()));

  QObject ::connect(fPickAction, 
                    SIGNAL(activated()),
                    this,
                    SLOT(actionMousePick()));

  QObject ::connect(shortcutsAction, 
                    SIGNAL(activated()),
                    this,
                    SLOT(showShortcuts()));

#else
  QMenu *mMouseAction = fContextMenu->addMenu("&Mouse actions");

  fRotateAction = mMouseAction->addAction("Rotate");
  fMoveAction = mMouseAction->addAction("Move");
  fPickAction = mMouseAction->addAction("Pick");
  QAction *shortcutsAction = mMouseAction->addAction("Show shortcuts");

  fRotateAction->setCheckable(true);
  fMoveAction->setCheckable(false);
  fPickAction->setCheckable(false);
  shortcutsAction->setCheckable(false);

  fRotateAction->setChecked(true);
  fMoveAction->setChecked(false);
  fPickAction->setChecked(false);
  shortcutsAction->setChecked(false);

  QObject ::connect(fRotateAction, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(actionMouseRotate()));

  QObject ::connect(fMoveAction, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(actionMouseMove()));

  QObject ::connect(fPickAction, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(actionMousePick()));

  QObject ::connect(shortcutsAction, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(showShortcuts()));
#endif

#if QT_VERSION < 0x040000
  // === Style Menu ===
  QPopupMenu *mStyle = new QPopupMenu(fContextMenu);

  QPopupMenu *mRepresentation = new QPopupMenu(fContextMenu);

  QPopupMenu *mProjection = new QPopupMenu(fContextMenu);

#if QT_VERSION < 0x030200
  QAction *polyhedron = new QAction("&Polyhedron","&Polyhedron",CTRL+Key_P,mRepresentation,0,true);
  QAction *nurbs = new QAction("&NURBS","&NURBS",CTRL+Key_N,mRepresentation,0,true);

  QAction *ortho = new QAction("&Orthographic","&Orthographic",CTRL+Key_O,mProjection,0,true);
  QAction *perspective = new QAction("&Perspective","&Perspective",CTRL+Key_P,mProjection,0,true);
#else
  QAction *polyhedron = new QAction("&Polyhedron",CTRL+Key_P,mRepresentation);
  QAction *nurbs = new QAction("&NURBS",CTRL+Key_N,mRepresentation);

  QAction *ortho = new QAction("&Orthographic",CTRL+Key_O,mProjection);
  QAction *perspective = new QAction("&Perspective",CTRL+Key_P,mProjection);
  polyhedron->setToggleAction(true);
  nurbs->setToggleAction(true);
  ortho->setToggleAction(true);
  perspective->setToggleAction(true);
#endif
  polyhedron->addTo(mRepresentation);
  nurbs->addTo(mRepresentation);

  ortho->addTo(mProjection);
  perspective->addTo(mProjection);

  mStyle->insertItem("&Representation",mRepresentation);
  mStyle->insertItem("&Projection",mProjection);
  fContextMenu->insertItem("&Style",mStyle);


#else
  // === Style Menu ===
  QMenu *mStyle = fContextMenu->addMenu("&Style");

  QMenu *mRepresentation = mStyle->addMenu("&Representation");
  QMenu *mProjection = mStyle->addMenu("&Projection");
  QAction *polyhedron = mRepresentation->addAction("Polyhedron");
  QAction *nurbs = mRepresentation->addAction("NURBS");

  QAction *ortho = mProjection->addAction("Orthographic");
  QAction *perspective = mProjection->addAction("Persepective");
#endif

  // INIT mRepresentation
  G4ViewParameters::RepStyle style;
  style = fVP.GetRepStyle();
  if (style == G4ViewParameters::polyhedron) {
    createRadioAction(polyhedron,nurbs,SLOT(toggleRepresentation(bool)),1);
  } else if (style == G4ViewParameters::nurbs) {
    createRadioAction(polyhedron,nurbs,SLOT(toggleRepresentation(bool)),2);
  } else {
    mRepresentation->clear();
  }

  // INIT mProjection
  if (fVP.GetFieldHalfAngle() == 0) {
    createRadioAction(ortho, perspective,SLOT(toggleProjection(bool)),1);
  } else {
    createRadioAction(ortho, perspective,SLOT(toggleProjection(bool)),2);
  }

#if QT_VERSION < 0x040000
  // === Drawing Menu ===
  QPopupMenu *mDrawing = new QPopupMenu(fContextMenu);
  fContextMenu->insertItem("&Drawing",mDrawing);

#if QT_VERSION < 0x030200
  fDrawingWireframe = new QAction("&Wireframe","&Wireframe",CTRL+Key_W,mDrawing,0,true);
  fDrawingLineRemoval = new QAction("&Hidden line removal","&Hidden line removal",CTRL+Key_L,mDrawing,0,true);
  fDrawingSurfaceRemoval = new QAction("&Hidden surface removal","&Hidden surface removal",CTRL+Key_S,mDrawing,0,true);
  fDrawingLineSurfaceRemoval = new QAction("&Hidden line and surface removal","&Hidden line and surface removal",CTRL+Key_R,mDrawing,0,true);
#else
  fDrawingWireframe = new QAction("&Wireframe",CTRL+Key_W,mDrawing);
  fDrawingLineRemoval = new QAction("&Hidden line removal",CTRL+Key_L,mDrawing);
  fDrawingSurfaceRemoval = new QAction("&Hidden surface removal",CTRL+Key_S,mDrawing);
  fDrawingLineSurfaceRemoval = new QAction("&Hidden line and surface removal",CTRL+Key_R,mDrawing);
#endif
  fDrawingWireframe->setToggleAction(true);
  fDrawingLineRemoval->setToggleAction(true);
  fDrawingSurfaceRemoval->setToggleAction(true);
  fDrawingLineSurfaceRemoval->setToggleAction(true);

  fDrawingWireframe->addTo(mDrawing);
  fDrawingLineRemoval->addTo(mDrawing);
  fDrawingSurfaceRemoval->addTo(mDrawing);
  fDrawingLineSurfaceRemoval->addTo(mDrawing);


#else
  // === Drawing Menu ===
  QMenu *mDrawing = mStyle->addMenu("&Drawing");

  fDrawingWireframe = mDrawing->addAction("Wireframe");
  fDrawingWireframe->setCheckable(true);

  fDrawingLineRemoval = mDrawing->addAction("Hidden line removal");
  fDrawingLineRemoval->setCheckable(true);

  fDrawingSurfaceRemoval = mDrawing->addAction("Hidden Surface removal");
  fDrawingSurfaceRemoval->setCheckable(true);

  fDrawingLineSurfaceRemoval = mDrawing->addAction("Hidden line and surface removal");
  fDrawingLineSurfaceRemoval->setCheckable(true);
#endif
  // INIT Drawing
  G4ViewParameters::DrawingStyle d_style;
  d_style = fVP.GetDrawingStyle();
  
#if QT_VERSION < 0x040000
  if (d_style == G4ViewParameters::wireframe) {
    fDrawingWireframe->setOn(true);
  } else if (d_style == G4ViewParameters::hlr) {
    fDrawingLineRemoval->setOn(true);
  } else if (d_style == G4ViewParameters::hsr) {
    fDrawingSurfaceRemoval->setOn(true);
  } else if (d_style == G4ViewParameters::hlhsr) {
    fDrawingLineSurfaceRemoval->setOn(true);
  } else {
    mDrawing->clear();
  }
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer:: fDrawingWireframe 1\n");
#endif
  QObject ::connect(fDrawingWireframe, 
                    SIGNAL(activated()),
                    this, 
                    SLOT(actionDrawingWireframe()));
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer:: fDrawingWireframe 2\n");
#endif
  QObject ::connect(fDrawingLineRemoval, 
                    SIGNAL(activated()),
                    this, 
                    SLOT(actionDrawingLineRemoval()));
  QObject ::connect(fDrawingSurfaceRemoval, 
                    SIGNAL(activated()),
                    this, 
                    SLOT(actionDrawingSurfaceRemoval()));
  QObject ::connect(fDrawingLineSurfaceRemoval, 
                    SIGNAL(activated()),
                    this, 
                    SLOT(actionDrawingLineSurfaceRemoval()));
#else
  if (d_style == G4ViewParameters::wireframe) {
    fDrawingWireframe->setChecked(true);
  } else if (d_style == G4ViewParameters::hlr) {
    fDrawingLineRemoval->setChecked(true);
  } else if (d_style == G4ViewParameters::hsr) {
    fDrawingSurfaceRemoval->setChecked(true);
  } else if (d_style == G4ViewParameters::hlhsr) {
    fDrawingLineSurfaceRemoval->setChecked(true);
  } else {
    mDrawing->clear();
  }
  QObject ::connect(fDrawingWireframe, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(actionDrawingWireframe()));
  QObject ::connect(fDrawingLineRemoval, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(actionDrawingLineRemoval()));
  QObject ::connect(fDrawingSurfaceRemoval, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(actionDrawingSurfaceRemoval()));
  QObject ::connect(fDrawingLineSurfaceRemoval, 
                    SIGNAL(triggered(bool)),
                    this, 
                    SLOT(actionDrawingLineSurfaceRemoval()));
#endif



#if QT_VERSION < 0x040000
  QPopupMenu *mBackground = new QPopupMenu(mStyle);
  mStyle->insertItem("&Background color",mBackground);

#if QT_VERSION < 0x030200
  QAction *white = new QAction("&White","&White",CTRL+Key_W,mBackground,0,true);
  QAction *black =  new QAction("&Black","&Black",CTRL+Key_B,mBackground,0,true);
#else
  QAction *white = new QAction("&White",CTRL+Key_W,mBackground);
  QAction *black =  new QAction("&Black",CTRL+Key_B,mBackground);
  white->setToggleAction(true);
  black->setToggleAction(true);
#endif
  white->addTo(mBackground);
  black->addTo(mBackground);

#else
  QMenu *mBackground = mStyle->addMenu("&Background color");
  QAction *white = mBackground->addAction("White");
  QAction *black = mBackground->addAction("Black");

#endif
  if (background.GetRed() == 1. &&
      background.GetGreen() == 1. &&
      background.GetBlue() == 1.) {
    createRadioAction(white,black,SLOT(toggleBackground(bool)),1);
  } else {
    createRadioAction(white,black,SLOT(toggleBackground(bool)),2);
  }


#if QT_VERSION < 0x040000
  // === Action Menu ===
  QPopupMenu *mActions = new QPopupMenu(fContextMenu);
  fContextMenu->insertItem("&Actions",mActions);

#if QT_VERSION < 0x030200
  QAction *createEPS =  new QAction("&Save as ...","&Save as ...",CTRL+Key_S,mActions,0,true);
#else
  QAction *createEPS =  new QAction("&Save as ...",CTRL+Key_S,mActions);
#endif
  createEPS->addTo(mActions);
  QObject ::connect(createEPS, 
                    SIGNAL(activated()),
                    this,
                    SLOT(actionSaveImage()));

#else
  // === Action Menu ===
  QMenu *mActions = fContextMenu->addMenu("&Actions");
  QAction *createEPS = mActions->addAction("Save as ...");
  QObject ::connect(createEPS, 
                    SIGNAL(triggered()),
                    this,
                    SLOT(actionSaveImage()));
#endif

#if QT_VERSION < 0x040000
#if QT_VERSION < 0x030200
  QAction *movieParameters =  new QAction("&Movie parameters...","&Make movie ...",CTRL+Key_M,mActions,0,true);
#else
  QAction *movieParameters =  new QAction("&Movie parameters...",CTRL+Key_M,mActions);
#endif
  movieParameters->addTo(mActions);
  QObject ::connect(movieParameters, 
                    SIGNAL(activated()),
                    this,
                    SLOT(actionMovieParameters()));

#else
  // === Action Menu ===
  QAction *movieParameters = mActions->addAction("Movie parameters...");
  QObject ::connect(movieParameters, 
                    SIGNAL(triggered()),
                    this,
                    SLOT(actionMovieParameters()));
#endif




#if QT_VERSION < 0x040000
  // === Special Menu ===
  QPopupMenu *mSpecial = new QPopupMenu(fContextMenu);
  fContextMenu->insertItem("S&pecial",mSpecial);

  QPopupMenu *mTransparency = new QPopupMenu(mSpecial);
  mSpecial->insertItem("Transparency",mTransparency);

#if QT_VERSION < 0x030200
  QAction *transparencyOn = new QAction("&On","&On",CTRL+Key_O,mTransparency,0,true);
  QAction *transparencyOff = new QAction("&Off","&Off",CTRL+Key_F,mTransparency,0,true);
#else
  QAction *transparencyOn = new QAction("&On",CTRL+Key_O,mTransparency);
  QAction *transparencyOff = new QAction("&Off",CTRL+Key_F,mTransparency);
  transparencyOn->setToggleAction(true);
  transparencyOff->setToggleAction(true);
#endif
  transparencyOn->addTo(mTransparency);
  transparencyOff->addTo(mTransparency);

#else
  // === Special Menu ===
  QMenu *mSpecial = fContextMenu->addMenu("S&pecial");
  QMenu *mTransparency = mSpecial->addMenu("Transparency");
  QAction *transparencyOn = mTransparency->addAction("On");
  QAction *transparencyOff = mTransparency->addAction("Off");
#endif

  if (transparency_enabled == false) {
    createRadioAction(transparencyOn,transparencyOff,SLOT(toggleTransparency(bool)),2);
  } else if (transparency_enabled == true) {
    createRadioAction(transparencyOn,transparencyOff,SLOT(toggleTransparency(bool)),1);
  } else {
    mSpecial->clear();
  }


#if QT_VERSION < 0x040000
  QPopupMenu *mAntialiasing = new QPopupMenu(mSpecial);
  mSpecial->insertItem("Antialiasing",mAntialiasing);

#if QT_VERSION < 0x030200
  QAction *antialiasingOn = new QAction("&On","&On",CTRL+Key_O,mAntialiasing,0,true);
  QAction *antialiasingOff = new QAction("&Off","&Off",CTRL+Key_F,mAntialiasing,0,true);
#else
  QAction *antialiasingOn = new QAction("&On",CTRL+Key_O,mAntialiasing);
  QAction *antialiasingOff = new QAction("&Off",CTRL+Key_F,mAntialiasing);
  antialiasingOn->setToggleAction(true);
  antialiasingOff->setToggleAction(true);
#endif
  antialiasingOn->addTo(mAntialiasing);
  antialiasingOff->addTo(mAntialiasing);

#else
  QMenu *mAntialiasing = mSpecial->addMenu("Antialiasing");
  QAction *antialiasingOn = mAntialiasing->addAction("On");
  QAction *antialiasingOff = mAntialiasing->addAction("Off");
#endif

  if (antialiasing_enabled == false) {
    createRadioAction(antialiasingOn,antialiasingOff,SLOT(toggleAntialiasing(bool)),2);
  } else if (antialiasing_enabled == true) {
    createRadioAction(antialiasingOn,antialiasingOff,SLOT(toggleAntialiasing(bool)),1);
  } else {
    mAntialiasing->clear();
  }

#if QT_VERSION < 0x040000
  QPopupMenu *mHaloing = new QPopupMenu(mSpecial);
  mSpecial->insertItem("Haloing",mHaloing);

#if QT_VERSION < 0x030200
  QAction *haloingOn = new QAction("&On","&On",CTRL+Key_O,mHaloing,0,true);
  QAction *haloingOff = new QAction("&Off","&Off",CTRL+Key_F,mHaloing,0,true);
#else
  QAction *haloingOn = new QAction("&On",CTRL+Key_O,mHaloing);
  QAction *haloingOff = new QAction("&Off",CTRL+Key_F,mHaloing);
  haloingOn->setToggleAction(true);
  haloingOff->setToggleAction(true);
#endif
  haloingOn->addTo(mHaloing);
  haloingOff->addTo(mHaloing);
#else
  QMenu *mHaloing = mSpecial->addMenu("Haloing");
  QAction *haloingOn = mHaloing->addAction("On");
  QAction *haloingOff = mHaloing->addAction("Off");
#endif
  if (haloing_enabled == false) {
    createRadioAction(haloingOn,haloingOff,SLOT(toggleHaloing(bool)),2);
  } else if (haloing_enabled == true) {
    createRadioAction(haloingOn,haloingOff,SLOT(toggleHaloing(bool)),1);
  } else {
    mHaloing->clear();
  }

#if QT_VERSION < 0x040000
  QPopupMenu *mAux = new QPopupMenu(mSpecial);
  mSpecial->insertItem("Auxiliairy edges",mAux);

#if QT_VERSION < 0x030200
  QAction *auxOn = new QAction("&On","&On",CTRL+Key_O,mAux,0,true);
  QAction *auxOff = new QAction("&Off","&Off",CTRL+Key_F,mAux,0,true);
#else
  QAction *auxOn = new QAction("&On",CTRL+Key_O,mAux);
  QAction *auxOff = new QAction("&Off",CTRL+Key_F,mAux);
  auxOn->setToggleAction(true);
  auxOff->setToggleAction(true);
#endif
  auxOn->addTo(mAux);
  auxOff->addTo(mAux);

#else
  QMenu *mAux = mSpecial->addMenu("Auxiliary edges");
  QAction *auxOn = mAux->addAction("On");
  QAction *auxOff = mAux->addAction("Off");
#endif
  if (!fVP.IsAuxEdgeVisible()) {
    createRadioAction(auxOn,auxOff,SLOT(toggleAux(bool)),1);
  } else {
    createRadioAction(auxOn,auxOff,SLOT(toggleAux(bool)),2);
  }



#if QT_VERSION < 0x040000
  QPopupMenu *mFullScreen = new QPopupMenu(mSpecial);
  mSpecial->insertItem("Full screen",mFullScreen);

#if QT_VERSION < 0x030200
  fFullScreenOn = new QAction("&On","&On",CTRL+Key_O,mFullScreen,0,true);
  fFullScreenOff = new QAction("&Off","&Off",CTRL+Key_F,mFullScreen,0,true);
#else
  fFullScreenOn = new QAction("&On",CTRL+Key_O,mFullScreen);
  fFullScreenOff = new QAction("&Off",CTRL+Key_F,mFullScreen);
  fFullScreenOn->setToggleAction(true);
  fFullScreenOff->setToggleAction(true);
#endif
  fFullScreenOn->addTo(mFullScreen);
  fFullScreenOff->addTo(mFullScreen);
#else
  QMenu *mFullScreen = mSpecial->addMenu("&Full screen");
  fFullScreenOn = mFullScreen->addAction("On");
  fFullScreenOff = mFullScreen->addAction("Off");
#endif
  createRadioAction(fFullScreenOn,fFullScreenOff,SLOT(toggleFullScreen(bool)),2);

}

void G4OpenGLQtViewer::manageContextMenuEvent(QContextMenuEvent *e)
{
  if (!GLWindow) {
    G4cerr << "Visualization window not defined, please choose one before" << G4endl;
  } else {
  
    if (!fContextMenu) 
      createPopupMenu();
    
    // launch menu
    if ( fContextMenu ) {
      fContextMenu->exec( e->globalPos() );
      //    delete fContextMenu;
    }
  }
  e->accept();
}


/**
   Create a radio button menu. The two menu will be connected. When click on one,
   eatch state will be invert and callback method will be called.
   @param action1 first action to connect
   @param action2 second action to connect
   @param method callback method
   @param nCheck: 1 : first action will be set true. 2 : second action will be set true
*/
#if QT_VERSION < 0x040000
void G4OpenGLQtViewer::createRadioAction(QAction *action1,QAction *action2, const std::string& method,unsigned int nCheck) {

  if (action1->parent()->inherits("QPopupMenu")){
    ((QPopupMenu*)action1->parent())->setCheckable(true);
    ((QPopupMenu*)action2->parent())->setCheckable(true);
  }
  action1->setOn(false);
   action2->setOn(false);

  if (nCheck ==1)
    action1->setOn(true);
  else
    action2->setOn(true);
   
  //FIXME : Should not work on Qt3
  QObject ::connect(action1, SIGNAL(activated()),action2, SLOT(toggle()));
  QObject ::connect(action2, SIGNAL(activated()),action1, SLOT(toggle()));

  QObject ::connect(action1, SIGNAL(toggled(bool)),this, method.c_str());
}

#else
void G4OpenGLQtViewer::createRadioAction(QAction *action1,QAction *action2, const std::string& method,unsigned int nCheck) {

  action1->setCheckable(true);
  action2->setCheckable(true);

  if (nCheck ==1)
    action1->setChecked (true);
  else
    action2->setChecked (true);
   
  QObject ::connect(action1, SIGNAL(triggered(bool)),action2, SLOT(toggle()));
  QObject ::connect(action2, SIGNAL(triggered(bool)),action1, SLOT(toggle()));

  QObject ::connect(action1, SIGNAL(toggled(bool)),this, method.c_str());

}
#endif

/**
   Slot activate when mouseAction->rotate menu is set 
 */
void G4OpenGLQtViewer::actionMouseRotate() {
  emit toggleMouseAction(STYLE1);
}


/**
   Slot activate when mouseAction->rotate menu is set 
 */
void G4OpenGLQtViewer::actionMouseMove() {
  emit toggleMouseAction(STYLE2);
}


/**
   Slot activate when mouseAction->zoom menu is set 
 */
void G4OpenGLQtViewer::actionMousePick() {
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::actionMousePick \n");
#endif
  emit toggleMouseAction(STYLE3);
}


/**
   Slot activate when drawing->wireframe menu is set 
 */
void G4OpenGLQtViewer::actionDrawingWireframe() {
  emit toggleDrawingAction(1);
}

/**
   Slot activate when drawing->line removal menu is set 
 */
void G4OpenGLQtViewer::actionDrawingLineRemoval() {
  emit toggleDrawingAction(2);
}

/**
   Slot activate when drawing->surface removal menu is set 
 */
void G4OpenGLQtViewer::actionDrawingSurfaceRemoval() {
  emit toggleDrawingAction(3);
}

/**
   Slot activate when drawing->wireframe menu is set 
 */
void G4OpenGLQtViewer::actionDrawingLineSurfaceRemoval() {
  emit toggleDrawingAction(4);
}


/**
   Slot activated when mouse action is toggle
   @param aAction : STYLE1, STYLE2, STYLE3
 */
void G4OpenGLQtViewer::toggleMouseAction(mouseActions aAction) {
  
  if ((aAction == STYLE1) || //initialize all
      (aAction == STYLE2) ||
      (aAction == STYLE3))  {
#if QT_VERSION < 0x040000
    fRotateAction->setOn (false);
    fMoveAction->setOn (false);
    fPickAction->setOn (false);
#else
    fRotateAction->setChecked (false);
    fMoveAction->setChecked (false);
    fPickAction->setChecked (false);
#endif
    fVP.SetPicking(false);
    fMouseAction = aAction;
  }
  // rotate
  if (aAction == STYLE1) {  // rotate
    showShortcuts();
#if QT_VERSION < 0x040000
    fRotateAction->setOn (true);
#else
    fRotateAction->setChecked (true);
#endif
  } else  if (aAction == STYLE2) { //move
#if QT_VERSION < 0x040000
    fMoveAction->setOn (true);
#else
    fMoveAction->setChecked (true);
#endif
  } else  if (aAction == STYLE3) { //pick
#if QT_VERSION < 0x040000
    fPickAction->setOn (true);
#else
    fPickAction->setChecked (true);
#endif
    fVP.SetPicking(true);
  }
}

/**
   Show shortcuts for this mouse action
 */
void G4OpenGLQtViewer::showShortcuts() {
  if (fMouseAction == STYLE1) {  // rotate
    G4cout << "Click and move mouse to rotate volume " << G4endl;
    G4cout << "Press left/right arrows to move volume left/right" << G4endl;
    G4cout << "Press up/down arrows to move volume up/down" << G4endl;
    G4cout << "Press ALT+up/down arrows to move volume toward/forward" << G4endl;
    G4cout << "Press SHIFT+left/right arrows to rotate volume left/right" << G4endl;
    G4cout << "Press SHIFT+up/down arrows to rotate volume up/down" << G4endl;
    G4cout << "Press ALT+/- to slow/speed auto rotation/move" << G4endl;
    G4cout << "In video mode : " << G4endl;
    G4cout << " Press SPACE to Start/Pause video recording " << G4endl;
    G4cout << " Press RETURN to Stop video recording " << G4endl;
  } else  if (fMouseAction == STYLE2) { //move
    G4cout << "Move camera point of view with mouse" << G4endl;
    G4cout << "Press left/right arrows to move volume left/right" << G4endl;
    G4cout << "Press up/down arrows to move volume up/down" << G4endl;
    G4cout << "Press ALT+up/down arrows to move volume toward/forward" << G4endl;
    G4cout << "Press SHIFT+left/right arrows to rotate volume left/right" << G4endl;
    G4cout << "Press SHIFT+up/down arrows to rotate volume up/down" << G4endl;
    G4cout << "Press +/- to zoom into volume" << G4endl;
    G4cout << "Press ALT+/- to slow/speed auto rotation/move" << G4endl;
    G4cout << "In video mode : " << G4endl;
    G4cout << " Press SPACE to Start/Pause video recording " << G4endl;
    G4cout << " Press RETURN to Stop video recording " << G4endl;
  } else  if (fMouseAction == STYLE3) { //pick
    G4cout << "Click and pick " << G4endl;
  }

}



/**
   Slot activated when drawing menu is toggle
   Warning : When G4OpenGLStoredQtViewer::DrawView() method call,
   KernelVisitDecision () will be call and will set the fNeedKernelVisit
   to 1. See G4XXXStoredViewer::CompareForKernelVisit for explanations.
   It will cause a redraw of the view
   @param aAction : 1 wireframe, 2 line removal, 3 surface removal, 4 line & surface removal
   @see G4OpenGLStoredQtViewer::DrawView
   @see G4XXXStoredViewer::CompareForKernelVisit
 */
void G4OpenGLQtViewer::toggleDrawingAction(int aAction) {

  G4ViewParameters::DrawingStyle d_style;
  

  // initialize
  if ((aAction >0) && (aAction <5)) {
#if QT_VERSION < 0x040000
    fDrawingWireframe->setOn(false);
    fDrawingLineRemoval->setOn(false);
    fDrawingSurfaceRemoval->setOn(false);
    fDrawingLineSurfaceRemoval->setOn(false);
#else
    fDrawingWireframe->setChecked (false);
    fDrawingLineRemoval->setChecked (false);
    fDrawingSurfaceRemoval->setChecked (false);
    fDrawingLineSurfaceRemoval->setChecked (false);
#endif
  }
  if (aAction ==1) {
#if QT_VERSION < 0x040000
    fDrawingWireframe->setOn(true);
#else
    fDrawingWireframe->setChecked (true);
#endif

    d_style = G4ViewParameters::wireframe;

  } else  if (aAction ==2) {
#if QT_VERSION < 0x040000
    fDrawingLineRemoval->setOn(true);
#else
    fDrawingLineRemoval->setChecked (true);
#endif

    d_style = G4ViewParameters::hlr;

  } else  if (aAction ==3) {
#if QT_VERSION < 0x040000
    fDrawingSurfaceRemoval->setOn(true);
#else
    fDrawingSurfaceRemoval->setChecked (true);
#endif

    d_style = G4ViewParameters::hsr;

  } else  if (aAction ==4) {
#if QT_VERSION < 0x040000
    fDrawingLineSurfaceRemoval->setOn(true);
#else
    fDrawingLineSurfaceRemoval->setChecked (true);
#endif
    d_style = G4ViewParameters::hlhsr;
  }
  fVP.SetDrawingStyle(d_style);

  updateQWidget();
}


/**
   SLOT Activate by a click on the representation menu
   Warning : When G4OpenGLStoredQtViewer::DrawView() method call,
   KernelVisitDecision () will be call and will set the fNeedKernelVisit
   to 1. See G4XXXStoredViewer::CompareForKernelVisit for explanations.
   It will cause a redraw of the view
   @param check : 1 polyhedron, 0 nurbs
   @see G4OpenGLStoredQtViewer::DrawView
   @see G4XXXStoredViewer::CompareForKernelVisit
*/
void G4OpenGLQtViewer::toggleRepresentation(bool check) {

  G4ViewParameters::RepStyle style;
  if (check == 1) {
    style = G4ViewParameters::polyhedron;
  } else {
    style = G4ViewParameters::nurbs;
  }
  fVP.SetRepStyle (style);

  updateQWidget();
}

/**
   SLOT Activate by a click on the projection menu
   Warning : When G4OpenGLStoredQtViewer::DrawView() method call,
   KernelVisitDecision () will be call and will set the fNeedKernelVisit
   to 1. See G4XXXStoredViewer::CompareForKernelVisit for explanations.
   It will cause a redraw of the view
   @param check : 1 orthographic, 2 perspective
   @see G4OpenGLStoredQtViewer::DrawView
   @see G4XXXStoredViewer::CompareForKernelVisit
*/
void G4OpenGLQtViewer::toggleProjection(bool check) {

  if (check == 1) {
    fVP.SetFieldHalfAngle (0);
  } else {

    // look for the default parameter hidden in G4UIcommand parameters
    G4UImanager* UI = G4UImanager::GetUIpointer();
    if(UI==NULL)
      return;
    G4UIcommandTree * treeTop = UI->GetTree();

    // find command
    G4UIcommand* command = treeTop->FindPath("/vis/viewer/set/projection");
    if (!command)
      return;

    // find param
    G4UIparameter * angleParam = NULL;
    for(G4int i=0;  i<command->GetParameterEntries(); i++)
    {
      if( command->GetParameter(i)->GetParameterName() == "field-half-angle" ) {
        angleParam = command->GetParameter(i);
      }
    }
    if (!angleParam)
      return;

    // find unit
    G4UIparameter * unitParam = NULL;
    for(G4int i=0;  i<command->GetParameterEntries(); i++)
    {
      if( command->GetParameter(i)->GetParameterName() == "unit" ) {
        unitParam = command->GetParameter(i);
      }
    }
    if (!unitParam)
      return;

    G4double defaultValue = command->ConvertToDouble(angleParam->GetDefaultValue())
                            * G4UnitDefinition::GetValueOf(unitParam->GetDefaultValue()); 
    if (defaultValue > 89.5 || defaultValue <= 0.0) {
      G4cerr << "Field half angle should be 0 < angle <= 89.5 degrees. Check your default Field half angle parameter";
    } else {
      G4cout << "Perspective view has been set to default value. Field half angle="<<angleParam->GetDefaultValue() <<" " << G4endl;
      fVP.SetFieldHalfAngle (defaultValue);
      SetView ();
    }
  }  
  updateQWidget();
}


/**
   SLOT Activate by a click on the background menu
@param check : 1 white, 0 black
*/
void G4OpenGLQtViewer::toggleBackground(bool check) {

  //   //I need to revisit the kernel if the background colour changes and
  //   //hidden line removal is enabled, because hlr drawing utilises the
  //   //background colour in its drawing...
  //   // (Note added by JA 13/9/2005) Background now handled in view
  //   // parameters.  A kernel visit is triggered on change of background.
  if (check == 1) {
    ((G4ViewParameters&)this->GetViewParameters()).
      SetBackgroundColour(G4Colour(1.,1.,1.));  // White
  } else {
    ((G4ViewParameters&)this->GetViewParameters()).
      SetBackgroundColour(G4Colour(0.,0.,0.));  // Black
  }
  updateQWidget();
}

/**
   SLOT Activate by a click on the transparency menu
@param check : 1 , 0
*/
void G4OpenGLQtViewer::toggleTransparency(bool check) {
  
  if (check) {
    transparency_enabled = false;
  } else {
    transparency_enabled = true;
  }
  SetNeedKernelVisit (true);
  updateQWidget();
}

/**
   SLOT Activate by a click on the antialiasing menu
@param check : 1 , 0
*/
void G4OpenGLQtViewer::toggleAntialiasing(bool check) {

  if (!check) {
    antialiasing_enabled = false;
    glDisable (GL_LINE_SMOOTH);
    glDisable (GL_POLYGON_SMOOTH);
  } else {
    antialiasing_enabled = true;
    glEnable (GL_LINE_SMOOTH);
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable (GL_POLYGON_SMOOTH);
    glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  }

  updateQWidget();
}

/**
   SLOT Activate by a click on the haloing menu
@param check : 1 , 0
*/
//FIXME : I SEE NOTHING...
void G4OpenGLQtViewer::toggleHaloing(bool check) {
  if (check) {
    haloing_enabled = false;
  } else {
    haloing_enabled = true;
  }

  updateQWidget();

}

/**
   SLOT Activate by a click on the auxiliaire edges menu
@param check : 1 , 0
*/
void G4OpenGLQtViewer::toggleAux(bool check) {
  if (check) {
    fVP.SetAuxEdgeVisible(false);
  } else {
    fVP.SetAuxEdgeVisible(true);
  }
  SetNeedKernelVisit (true);
  updateQWidget();

}

/**
   SLOT Activate by a click on the full screen menu
*/
void G4OpenGLQtViewer::toggleFullScreen(bool check) {
  if (check != GLWindow->isFullScreen()) { //toggle
#if QT_VERSION >= 0x030200
    GLWindow->setWindowState(GLWindow->windowState() ^ Qt::WindowFullScreen);
#else
    G4cerr << "This version of Qt could not do fullScreen. Resizing the widget is the only solution available." << G4endl;
#endif
  }
}


void G4OpenGLQtViewer::savePPMToTemp() {
  if (fMovieTempFolderPath == "") {
    return;
  }
  QString fileName ="Test"+QString::number(fRecordFrameNumber)+".ppm";
  QString filePath =fMovieTempFolderPath+fileName;

  QImage image;
  image = fWindow->grabFrameBuffer();
  bool res = false;
  
#if QT_VERSION < 0x040000
  res = image.save(filePath,"PPM");
#else
  res = image.save(filePath,0);
#endif
  if (res == false) { 
    resetRecording();
    setRecordingInfos("Can't save tmp file "+filePath);
    return;
  }
  
  setRecordingInfos("File "+fileName+" saved");
  fRecordFrameNumber++;
}



void G4OpenGLQtViewer::actionSaveImage() {
  QString filters;
#if QT_VERSION < 0x040000
  QStrList listFormat=QImageIO::outputFormats();
  char *tmp=listFormat.first();
  while (tmp!=0) {
    filters += QString(tmp) + ";;";
    tmp=listFormat.next();
  }
#else
  QList<QByteArray> formats =  QImageWriter::supportedImageFormats ();
  for (int i = 0; i < formats.size(); ++i) {
    filters +=formats.at(i) + ";;";
  }
#endif
  filters += "eps;;";
  filters += "ps;;";
  filters += "pdf";
  QString* selectedFormat = new QString();
#if QT_VERSION < 0x040000
  QString nomFich =  QFileDialog::getSaveFileName ( ".",
                                                    filters,
                                                    GLWindow,
                                                    "Save file dialog",
                                                    tr("Save as ..."),
                                                    selectedFormat ); 
#else
  QString nomFich =  QFileDialog::getSaveFileName ( GLWindow,
                                                    tr("Save as ..."),
                                                    ".",
                                                    filters,
                                                    selectedFormat ); 
#endif
  // bmp jpg jpeg png ppm xbm xpm
  if (nomFich == "") {
    return;
  }
#if QT_VERSION < 0x040000
  nomFich += "."+QString(selectedFormat->ascii());
  QString format = selectedFormat->lower();
#else
  nomFich += "."+QString(selectedFormat->toStdString().c_str());
  QString format = selectedFormat->toLower();
#endif
  G4OpenGLQtExportDialog* exportDialog= new G4OpenGLQtExportDialog(GLWindow,format,fWindow->height(),fWindow->width());
  if(  exportDialog->exec()) {

    QImage image;
    bool res = false;
    if ((exportDialog->getWidth() !=fWindow->width()) ||
        (exportDialog->getHeight() !=fWindow->height())) {
      if (format != QString("eps")) {
      G4cerr << "Export->Change Size : This function is not implemented, to export in another size, please resize your frame to what you need" << G4endl;
      
      //    rescaleImage(exportDialog->getWidth(),exportDialog->getHeight());// re-scale image
      //      QGLWidget* glResized = fWindow;

      // FIXME :
      // L.Garnier : I've try to implement change size function, but the problem is 
      // the renderPixmap function call the QGLWidget to resize and it doesn't draw
      // the content of this widget... It only draw the background.

      //      fWindow->renderPixmap (exportDialog->getWidth()*2,exportDialog->getHeight()*2,true );

      //      QPixmap pixmap = fWindow->renderPixmap ();
      
      //      image = pixmap->toImage();
      //      glResized->resize(exportDialog->getWidth()*2,exportDialog->getHeight()*2);
      //      image = glResized->grabFrameBuffer();
      }      
    } else {
      image = fWindow->grabFrameBuffer();
    }    
    if (format == QString("eps")) {
      if (exportDialog->getVectorEPS()) {
        res = generateVectorEPS(nomFich,exportDialog->getWidth(),exportDialog->getHeight(),image);
      } else {
        res = generateEPS(nomFich,exportDialog->getNbColor(),image);
      }
    } else if ((format == "ps") || (format == "pdf")) {
      res = generatePS_PDF(nomFich,exportDialog->getNbColor(),image);
    } else if ((format == "tif") ||
               (format == "tiff") ||
               (format == "jpg") ||
               (format == "jpeg") ||
               (format == "png") ||
               (format == "pbm") ||
               (format == "pgm") ||
               (format == "ppm") ||
               (format == "bmp") ||
               (format == "xbm") ||
               (format == "xpm")) {
#if QT_VERSION < 0x040000
      res = image.save(nomFich,selectedFormat->ascii(),exportDialog->getSliderValue());
#else
      res = image.save(nomFich,0,exportDialog->getSliderValue());
#endif
    } else {
      G4cerr << "This version of G4UI Could not generate the selected format" << G4endl;
    }
    if (res == false) {
#if QT_VERSION < 0x040000
      G4cerr << "Error while saving file... "<<nomFich.ascii()<<"" << G4endl;
#else
      G4cerr << "Error while saving file... "<<nomFich.toStdString().c_str()<< G4endl;
#endif
    } else {
#if QT_VERSION < 0x040000
      G4cout << "File "<<nomFich.ascii()<<" has been saved " << G4endl;
#else
      G4cout << "File "<<nomFich.toStdString().c_str()<<" has been saved " << G4endl;
#endif
    }
    
  } else { // cancel selected
    return;
  }
  
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::actionSaveImage() \n");
#endif
}


void G4OpenGLQtViewer::actionMovieParameters() {

  showMovieParametersDialog();
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLQtViewer::actionMovieParameters() \n");
#endif
}


void G4OpenGLQtViewer::showMovieParametersDialog() {
  if (!fMovieParametersDialog) {
    fMovieParametersDialog= new G4OpenGLQtMovieDialog(this,GLWindow);
    displayRecordingStatus();
    if (getEncoderPath() == "") {
      setRecordingInfos("mpeg_encode is needed to encode in video format. It is available here: http://bmrc.berkeley.edu/frame/research/mpeg/");
    }
  }
  fMovieParametersDialog->show();
}


/*
// http://www.google.com/codesearch?hl=en&q=+jpg+Qt+quality+QDialog+show:FZkUoth8oiw:TONpW2mR-_c:tyTfrKMO-xI&sa=N&cd=2&ct=rc&cs_p=http://soft.proindependent.com/src/qtiplot-0.8.9.zip&cs_f=qtiplot-0.8.9/qtiplot/src/application.cpp#a0

void Graph::exportToSVG(const QString& fname)
{
  // enable workaround for Qt3 misalignments
  QwtPainter::setSVGMode(true);
  QPicture picture;
  QPainter p(&picture);
  d_plot->print(&p, d_plot->rect());
  p.end();

  picture.save(fname, "svg");
}
*/




/**
   Save the current mouse press point
   @param p mouse click point
*/
#if QT_VERSION < 0x040000
void G4OpenGLQtViewer::G4MousePressEvent(QPoint p,Qt::ButtonState mButtons)
#else
void G4OpenGLQtViewer::G4MousePressEvent(QPoint p,Qt::MouseButtons mButtons)
#endif
{
  fAutoMove = false; // stop automove
  fLastPos = p;
  if (fMouseAction == STYLE2){  // pick
    Pick(p.x(),p.y());
  }
}


/**
   @param pos_x mouse x position
   @param pos_y mouse y position
   @param mButtons mouse button active
   @param mAutoMove true: apply this move till another evnt came, false :one time move
*/

#if QT_VERSION < 0x040000
void G4OpenGLQtViewer::G4MouseMoveEvent(int pos_x, int pos_y,Qt::ButtonState mButtons,bool mAutoMove)
#else
  void G4OpenGLQtViewer::G4MouseMoveEvent(int pos_x, int pos_y,Qt::MouseButtons mButtons,bool mAutoMove)
#endif
{
  fAutoMove = mAutoMove;

  if (!fAutoMove) {  // keep old delta if automove
    fDeltaPosX = fLastPos.x() - pos_x;
    fDeltaPosY = fLastPos.y() - pos_y;
  }

  if ((fDeltaPosX == 0) && (fDeltaPosY == 0)) {
    fAutoMove = false;
  }

  if (fMouseAction == STYLE1) {  // rotate
    if (mButtons & Qt::LeftButton) {
      rotateScene(fDeltaPosX,fDeltaPosY,fAutoMove);
    }
  } else if (fMouseAction == STYLE2) {  // move
    if (mButtons & Qt::LeftButton) {
      if (fAutoMove) {
        while (fAutoMove) {
          moveScene(-fDeltaPosX,-fDeltaPosY,0,true,true);
          ((QApplication*)G4Qt::getInstance ())->processEvents();
        }
      } else {
        moveScene(-fDeltaPosX,-fDeltaPosY,0,true,false);
      }
    }
  }
  fLastPos = QPoint(pos_x, pos_y);
}


/**
   Move the scene of dx, dy, dz values.
   @param dx delta mouse x position
   @param dy delta mouse y position
   @param mouseMove : true if even comes froma mouse move, false if even comes from key action
*/

void G4OpenGLQtViewer::moveScene(G4double dx,G4double dy, G4double dz,bool mouseMove,bool mAutoMove)
{
  if (fHoldMoveEvent)
    return;
  fHoldMoveEvent = true;

  if( mAutoMove == false) {
    fAutoMove = true;
  }
  G4double coefTrans = 0;
  GLdouble coefDepth = 0;
  while (fAutoMove) {
    if( mAutoMove == false) {
      fAutoMove = false;
    }
    if(mouseMove) {
      coefTrans = ((G4double)getSceneNearWidth())/((G4double)WinSize_x);
      if (WinSize_y <WinSize_x) {
        coefTrans = ((G4double)getSceneNearWidth())/((G4double)WinSize_y);
      }
    } else {
      coefTrans = getSceneNearWidth()*fDeltaSceneTranslation;
      coefDepth = getSceneDepth()*fDeltaDepth;
    }
    fVP.IncrementPan(-dx*coefTrans,dy*coefTrans,dz*coefDepth);
    
    updateQWidget();
    if (fAutoMove)
      ((QApplication*)G4Qt::getInstance ())->processEvents();
  }

  fHoldMoveEvent = false;
}


/**
   @param dx delta mouse x position
   @param dy delta mouse y position
*/

void G4OpenGLQtViewer::rotateScene(G4double dx, G4double dy,bool mAutoRotate)
{
  if (fHoldRotateEvent)
    return;
  fHoldRotateEvent = true;

  if( mAutoRotate == false) {
    fAutoMove = true;
  }
  G4Vector3D vp;
  G4Vector3D up;
  
  G4Vector3D xprime;
  G4Vector3D yprime;
  G4Vector3D zprime;
  
  G4double delta_alpha;
  G4double delta_theta;
  
  G4Vector3D new_vp;
  G4Vector3D new_up;
  
  G4double cosalpha;
  G4double sinalpha;
  
  G4Vector3D a1;
  G4Vector3D a2;
  G4Vector3D delta;
  G4Vector3D viewPoint;

  while (fAutoMove) {
    if( mAutoRotate == false) {
      fAutoMove = false;
    }
    
    //phi spin stuff here
  
    vp = fVP.GetViewpointDirection ().unit ();
    up = fVP.GetUpVector ().unit ();
  
    yprime = (up.cross(vp)).unit();
    zprime = (vp.cross(yprime)).unit();
  
    if (fVP.GetLightsMoveWithCamera()) {
      delta_alpha = dy * fDeltaRotation;
      delta_theta = -dx * fDeltaRotation;
    } else {
      delta_alpha = -dy * fDeltaRotation;
      delta_theta = dx * fDeltaRotation;
    }    
  
    delta_alpha *= deg;
    delta_theta *= deg;
  
    new_vp = std::cos(delta_alpha) * vp + std::sin(delta_alpha) * zprime;

    // to avoid z rotation flipping
    // to allow more than 360� rotation
    if (fVP.GetLightsMoveWithCamera()) {
      new_up = (new_vp.cross(yprime)).unit();
      if (new_vp.z()*vp.z() <0) {
        new_up.set(new_up.x(),-new_up.y(),new_up.z());
      }
    } else {
      new_up = up;
      if (new_vp.z()*vp.z() <0) {
        new_up.set(new_up.x(),-new_up.y(),new_up.z());
      }
    }
    fVP.SetUpVector(new_up);
    ////////////////
    // Rotates by fixed azimuthal angle delta_theta.
    
    cosalpha = new_up.dot (new_vp.unit());
    sinalpha = std::sqrt (1. - std::pow (cosalpha, 2));
    yprime = (new_up.cross (new_vp.unit())).unit ();
    xprime = yprime.cross (new_up);
    // Projection of vp on plane perpendicular to up...
    a1 = sinalpha * xprime;
    // Required new projection...
    a2 = sinalpha * (std::cos (delta_theta) * xprime + std::sin (delta_theta) * yprime);
    // Required Increment vector...
    delta = a2 - a1;
    // So new viewpoint is...
    viewPoint = new_vp.unit() + delta;
    
#ifdef GEANT4_QT_DEBUG
//      printf("vp Vector : %f %f %f delta_alpha:%f delta_theta:%f cosalpha:%f sinalpha:%f\n",viewPoint.x(),viewPoint.y(),viewPoint.z(),delta_alpha,delta_theta,cosalpha,sinalpha);
//      printf("up : %f %f %f\n",up.x(),up.y(),up.z());
//      printf("new up : %f %f %f\n",new_up.x(),new_up.y(),new_up.z());
//      printf("vp : %f %f %f\n",vp.x(),vp.y(),vp.z());
//      printf("new_vp : %f %f %f\n",new_vp.x(),new_vp.y(),new_vp.z());
#endif
    fVP.SetViewAndLights (viewPoint);
    updateQWidget();
    
    if (fAutoMove)
      ((QApplication*)G4Qt::getInstance ())->processEvents();
  }
  
  fHoldRotateEvent = false;
}

/** This is the benning of a rescale function. It does nothing for the moment
    @param aWidth : new width
    @param aHeight : new height
*/
void G4OpenGLQtViewer::rescaleImage(
 int aWidth
,int aHeight
){
#ifdef GEANT4_QT_DEBUG
  printf("should rescale \n");
#endif
  GLfloat* feedback_buffer;
  GLint returned;
  FILE* file;
  
//   feedback_buffer = new GLfloat[size];
//   glFeedbackBuffer (size, GL_3D_COLOR, feedback_buffer);
//   glRenderMode (GL_FEEDBACK);
  
//   glViewport (0, 0, aWidth, aHeight);
//   DrawView();
//   returned = glRenderMode (GL_RENDER);

}

/**
   Generate Vectorial Encapsulated Postscript form image
   @param aFilename : name of file
   @param aInColor : numbers of colors : 1->BW 2->RGB 3->RGB+Alpha
   @param aImage : Image to print
*/
bool G4OpenGLQtViewer::generateVectorEPS (
 QString aFilename
,int aWidth
,int aHeight
,QImage aImage
)
{
  // Print vectored PostScript
  
  G4int size = 5000000;

  GLfloat* feedback_buffer;
  GLint returned;
  FILE* file;
  
  feedback_buffer = new GLfloat[size];
  glFeedbackBuffer (size, GL_3D_COLOR, feedback_buffer);
  glRenderMode (GL_FEEDBACK);
  
  int side = aWidth;
  if (aHeight < aWidth) side = aHeight;
  glViewport((aWidth - side) / 2, (aHeight - side) / 2, side, side);
  DrawView();

  returned = glRenderMode (GL_RENDER);
  
  
#if QT_VERSION < 0x040000
  file = fopen (aFilename.ascii(), "w");
#else
  file = fopen (aFilename.toStdString().c_str(), "w");
#endif
  if (file) {
    spewWireframeEPS (file, returned, feedback_buffer, "rendereps");
  } else {
#if QT_VERSION < 0x040000
    G4cerr << "Could not open "<< aFilename.ascii() << G4endl;
#else
    G4cerr << "Could not open "<< aFilename.toStdString().c_str() << G4endl;
#endif
  }
  
  delete[] feedback_buffer;

  return true;
}

/**
   Generate Encapsulated Postscript form image
   @param aFilename : name of file
   @param aInColor : numbers of colors : 1->BW 2->RGB 3->RGB+Alpha
   @param aImage : Image to print
*/
bool G4OpenGLQtViewer::generateEPS (
 QString aFilename
,int aInColor
,QImage aImage
)
{
  // FIXME
#ifdef GEANT4_QT_DEBUG
  printf("saving EPS\n");
#endif

  FILE* fp;

  if (aImage.bits () == NULL)
    return false;
  
#if QT_VERSION < 0x040000
  fp = fopen (aFilename.ascii(), "w");
#else
  fp = fopen (aFilename.toStdString().c_str(), "w");
#endif
  if (fp == NULL) {
    return false;
  }
  
  fprintf (fp, "%%!PS-Adobe-2.0 EPSF-1.2\n");
#if QT_VERSION < 0x040000
  fprintf (fp, "%%%%Title: %s\n", aFilename.ascii());
#else
  fprintf (fp, "%%%%Title: %s\n", aFilename.toStdString().c_str());
#endif
  fprintf (fp, "%%%%Creator: OpenGL pixmap render output\n");
  fprintf (fp, "%%%%BoundingBox: 0 0 %d %d\n", aImage.width(), aImage.height());
  fprintf (fp, "%%%%EndComments\n");
  fprintf (fp, "gsave\n");
  fprintf (fp, "/bwproc {\n");
  fprintf (fp, "    rgbproc\n");
  fprintf (fp, "    dup length 3 idiv string 0 3 0 \n");
  fprintf (fp, "    5 -1 roll {\n");
  fprintf (fp, "    add 2 1 roll 1 sub dup 0 eq\n");
  fprintf (fp, "    { pop 3 idiv 3 -1 roll dup 4 -1 roll dup\n");
  fprintf (fp, "       3 1 roll 5 -1 roll } put 1 add 3 0 \n");
  fprintf (fp, "    { 2 1 roll } ifelse\n");
  fprintf (fp, "    }forall\n");
  fprintf (fp, "    pop pop pop\n");
  fprintf (fp, "} def\n");
  fprintf (fp, "systemdict /colorimage known not {\n");
  fprintf (fp, "   /colorimage {\n");
  fprintf (fp, "       pop\n");
  fprintf (fp, "       pop\n");
  fprintf (fp, "       /rgbproc exch def\n");
  fprintf (fp, "       { bwproc } image\n");
  fprintf (fp, "   }  def\n");
  fprintf (fp, "} if\n");
  fprintf (fp, "/picstr %d string def\n", aImage.width() * aInColor);
  fprintf (fp, "%d %d scale\n", aImage.width(), aImage.height());
  fprintf (fp, "%d %d %d\n", aImage.width(), aImage.height(), 8);
  fprintf (fp, "[%d 0 0 %d 0 0]\n", aImage.width(), aImage.height());
  fprintf (fp, "{currentfile picstr readhexstring pop}\n");
  fprintf (fp, "false %d\n", aInColor);
  fprintf (fp, "colorimage\n");
  

  int width = aImage.width();
  int height = aImage.height();
  int depth = aImage.depth();
  int size = width*height;
  
  if (depth == 1)
    size = (width+7)/8*height;
  else if (aInColor == 1)
    size = size*3;
  
  int i = 0;
  //  if ( aInColor ==1 ) {
  // FIXME : L. Garnier. For the moment 10 dec 2007, I could not find a way
  // to save correctly grayscale Image. I mean that color or grayscale image
  // have the same file save size !
  
  /* } else*/ if (depth == 8) {
    for(int y=height-1; y >=0 ; y--) {
      const uchar * s = aImage.scanLine(y);
      for(int x=0; x <width; x++) {
        QRgb rgb = aImage.color(s[x]);
        if (aInColor == 1) {
          fprintf (fp, " %02hx ",(unsigned char)qGray(rgb));
          i++;
        } else {
          fprintf (fp, " %02hx %02hx %02hx",
                   (unsigned char) qRed(rgb),
                   (unsigned char) qGreen(rgb),
                   (unsigned char) qBlue(rgb));
          i += 3;
        }
      }
      fprintf (fp, "\n");
    }
  } else {
#if QT_VERSION < 0x040000
    bool alpha = aImage.hasAlphaBuffer();
#else
    bool alpha = aImage.hasAlphaChannel();
    for(int y=height-1; y >=0 ; y--) {
      QRgb * s = (QRgb*)(aImage.scanLine(y));
      for(int x=0; x <width; x++) {
        QRgb rgb = (*s++);
        if (alpha && qAlpha(rgb) < 0x40) // 25% alpha, convert to white -
          rgb = qRgb(0xff, 0xff, 0xff);
        if (aInColor == 1) {
          fprintf (fp, " %02hx ",(unsigned char)qGray(rgb));
          i++;
        } else {
          fprintf (fp, " %02hx %02hx %02hx",
                   (unsigned char) qRed(rgb),
                   (unsigned char) qGreen(rgb),
                   (unsigned char) qBlue(rgb));
          i += 3;
        }
      }
      fprintf (fp, "\n");
    } 
#endif

  }

  fprintf (fp, "grestore\n");
  fprintf (fp, "showpage\n");
  fclose (fp);

  return true;
}
/**
   Generate Postscript or PDF form image
   @param aFilename : name of file
   @param aInColor : numbers of colors : 1->BW 2->RGB
   @param aImage : Image to print
*/
bool G4OpenGLQtViewer::generatePS_PDF (
 QString aFilename
,int aInColor
,QImage aImage
)
{

#if QT_VERSION < 0x040000
#ifdef Q_WS_MAC || Q_WS_X11
  QPrinter printer;
  //  printer.setPageSize(pageSize);
  if (aInColor == 1) {
    printer.setColorMode(QPrinter::GrayScale);
  } else {
    printer.setColorMode(QPrinter::Color);
  }

  /* FIXME : I don't know which format it will save...
     if (aFilename.endsWith(".ps")) {
     printer.setOutputFormat(QPrinter::PostScriptFormat);
     } else {
     printer.setOutputFormat(QPrinter::PdfFormat);
     }
  */
  printer.setOutputFileName(aFilename);
  //  printer.setFullPage ( true);
  QPainter paint(&printer);
  paint.drawImage (0,0,aImage );
  paint.end();
#else
  G4cerr << "This fonction is only supported on Mac OsX or X11 with Qt3. Full platform supported with Qt4" << G4endl;
#endif
#else
  QPrinter printer;
  //  printer.setPageSize(pageSize);

  // FIXME : L. Garnier 4/12/07
  // This is not working, it does nothing. Image is staying in color mode
  // So I have desactivate the B/W button in GUI
  if ((!aImage.isGrayscale ()) &&(aInColor ==1 )) {
#if QT_VERSION < 0x040000
    aImage = aImage.convertDepth(1,Qt::MonoOnly);
#else
    aImage = aImage.convertToFormat ( aImage.format(), Qt::MonoOnly);
#endif
  }


  if (aFilename.endsWith(".ps")) {
#if QT_VERSION > 0x040200
    printer.setOutputFormat(QPrinter::PostScriptFormat);
#endif
  } else {
#if QT_VERSION > 0x040100
    printer.setOutputFormat(QPrinter::PdfFormat);
#endif
  }
#if QT_VERSION > 0x040100
  printer.setOutputFileName(aFilename);
#endif
  //  printer.setFullPage ( true);
  QPainter paint(&printer);
  paint.drawImage (0,0,aImage);
  paint.end();
#endif
  return true;
}



void G4OpenGLQtViewer::G4keyPressEvent (QKeyEvent * event) 
{
  if (fHoldKeyEvent)
    return;

  fHoldKeyEvent = true;

#if QT_VERSION < 0x040000
  if ((event->key() == Qt::Key_Down) && (event->state() & Qt::AltButton )) { // go backward
#else
  if ((event->key() == Qt::Key_Down) && (event->modifiers() & Qt::AltModifier )) { // go backward
#endif
    
    moveScene(0,0,1,false);
  }
#if QT_VERSION < 0x040000
  else if ((event->key() == Qt::Key_Up) && (event->state() & Qt::AltButton)) { // go forward
#else
  else if ((event->key() == Qt::Key_Up) && (event->modifiers() & Qt::AltModifier)) { // go forward
#endif
    moveScene(0,0,-1,false);
  }
#if QT_VERSION < 0x040000
  if ((event->key() == Qt::Key_Down) && (event->state() & Qt::ShiftButton)) { // rotate phi
#else
  if ((event->key() == Qt::Key_Down) && (event->modifiers() & Qt::ShiftModifier)) { // rotate phi
#endif
    rotateScene(0,-1);
  }
#if QT_VERSION < 0x040000
  else if ((event->key() == Qt::Key_Up) && (event->state() & Qt::ShiftButton)) { // rotate phi
#else
  else if ((event->key() == Qt::Key_Up) && (event->modifiers() & Qt::ShiftModifier)) { // rotate phi
#endif
    rotateScene(0,1);
  }
#if QT_VERSION < 0x040000
  if ((event->key() == Qt::Key_Left) && (event->state() & Qt::ShiftButton)) { // rotate theta
#else
  if ((event->key() == Qt::Key_Left) && (event->modifiers() & Qt::ShiftModifier)) { // rotate theta
#endif
    rotateScene(1,0);
  }
#if QT_VERSION < 0x040000
  else if ((event->key() == Qt::Key_Right) && (event->state() & Qt::ShiftButton)) { // rotate theta
#else
  else if ((event->key() == Qt::Key_Right) && (event->modifiers() & Qt::ShiftModifier)) { // rotate theta
#endif
    rotateScene(-1,0);
  }

#if QT_VERSION < 0x040000
  if ((event->state() & Qt::AltButton)) {
#else
  if ((event->modifiers() & Qt::AltModifier)) {
#endif
    if (event->key() == Qt::Key_Plus) {
      fDeltaRotation = fDeltaRotation/0.7;
    }
    else if (event->key() == Qt::Key_Minus) {
      fDeltaRotation = fDeltaRotation*0.7;
    }
  } else {
    if (event->key() == Qt::Key_Plus) {
      fVP.SetZoomFactor(fVP.GetZoomFactor()*(1+fDeltaZoom)); 
      updateQWidget();
    }
    else if (event->key() == Qt::Key_Minus) {
      fVP.SetZoomFactor(fVP.GetZoomFactor()*(1-fDeltaZoom)); 
      updateQWidget();
    }
  }
  
  
  if (event->key() == Qt::Key_Escape) { // escaped from full screen
#if QT_VERSION >= 0x030200
      toggleFullScreen(false);
#endif
  }
  // several case here : If return is pressed, in every case -> display the movie parameters dialog
  // If one parameter is wrong -> put it in red (only save filenam could be wrong..)
  // If encoder not found-> does nothing.Only display a message in status box
  // If all ok-> generate parameter file
  // If ok -> put encoder button enabled
 
  if ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter)){ // end of video
   stopVideo();
  }
  if (event->key() == Qt::Key_Space){ // start/pause of video
   startPauseVideo();
  }

  // with no modifiers
#if QT_VERSION < 0x040000
  if (event->state() == Qt::NoButton) {
#else
  if (event->modifiers() == Qt::NoModifier) {
#endif
    if (event->key() == Qt::Key_Down) { // go down
      moveScene(0,1,0,false);
    }
    else if (event->key() == Qt::Key_Up) {  // go up
      moveScene(0,-1,0,false);
    }
    if (event->key() == Qt::Key_Left) { // go left
      moveScene(-1,0,0,false);
    }
    else if (event->key() == Qt::Key_Right) { // go right
      moveScene(1,0,0,false);
    }
  }
  fHoldKeyEvent = false;
}
  

/** Stop the video. Check all parameters and enable encoder button if all is ok.
*/
void G4OpenGLQtViewer::stopVideo() {

 // if encoder parameter is wrong, display parameters dialog and return
  if (!fMovieParametersDialog) {
    showMovieParametersDialog();
  }
  setRecordingStatus(STOP);

  if (fRecordFrameNumber >0) {
    // check parameters if they were modified (Re APPLY them...)
    // It will enable/disable encode button
    fMovieParametersDialog->checkAllParameters();
  } else {
    resetRecording();
    setRecordingInfos("No frame to encode.");
  }
}


/** Start/Pause the video..
*/
void G4OpenGLQtViewer::startPauseVideo() {
   
  // first time, if temp parameter is wrong, display parameters dialog and return
  if ( fRecordingStep == WAIT) {
    if ( fRecordFrameNumber == 0) {
      if (getTempFolderPath() == "") {
        showMovieParametersDialog();
        setRecordingInfos("You should specified the temp folder in order to make movie");
        return;
      } else  {
        // remove temp folder if it was create
        QString tmp = removeTempFolder();
        if (tmp !="") {
          setRecordingInfos(tmp);
          return;
        }
        tmp = createTempFolder();
        if (tmp != "") {
          setRecordingInfos("Can't create temp folder."+tmp);
          return;
        }
      }
    }
  }
  if (fRecordingStep == WAIT) {
    setRecordingStatus(START); 
  } else if (fRecordingStep == START) {
    setRecordingStatus(PAUSE);
  } else if (fRecordingStep == PAUSE) {
    setRecordingStatus(CONTINUE);
  } else if (fRecordingStep == CONTINUE) {
    setRecordingStatus(PAUSE);
  }
}


void G4OpenGLQtViewer::setRecordingStatus(RECORDING_STEP step) {

  fRecordingStep = step;
  displayRecordingStatus();
}


void G4OpenGLQtViewer::displayRecordingStatus() {
  
  QString txt = "";
  if (fRecordingStep == WAIT) {
    txt  = "Waiting to start...";
    fRecordFrameNumber = 0; // reset the frame number
  } else if (fRecordingStep == START) {
    txt  = "Start Recording...";
  } else if (fRecordingStep == PAUSE) {
    txt  = "Pause Recording...";
  } else if (fRecordingStep == CONTINUE) {
    txt  = "Continue Recording...";
  } else if (fRecordingStep == STOP) {
    txt  = "Stop Recording...";
  } else if (fRecordingStep == READY_TO_ENCODE) {
    txt  = "Ready to Encode...";
  } else if (fRecordingStep == ENCODING) {
    txt  = "Encoding...";
  } else if (fRecordingStep == FAILED) {
    txt  = "Failed to encode...";
  } else if (fRecordingStep == SUCCESS) {
    txt  = "File encoded successfully";
  } else {
  }

  if (fMovieParametersDialog) {
    fMovieParametersDialog->setRecordingStatus(txt);
  } else {
#if QT_VERSION < 0x040000
    G4cout << txt.ascii() << G4endl;
#else
    G4cout << txt.toStdString().c_str() << G4endl;
#endif
  }
  setRecordingInfos("");
}


void G4OpenGLQtViewer::setRecordingInfos(QString txt) {
  if (fMovieParametersDialog) {
    fMovieParametersDialog->setRecordingInfos(txt);
  } else {
#if QT_VERSION < 0x040000
    G4cout << txt.ascii() << G4endl;
#else
    G4cout << txt.toStdString().c_str() << G4endl;
#endif
  }
}

/** Init the movie parameters. Temp dir and encoder path
*/
void G4OpenGLQtViewer::initMovieParameters() {
  //init encoder
  
   //look for encoderPath
     fProcess = new QProcess();
     
#if QT_VERSION < 0x040000
     QObject ::connect(fProcess,SIGNAL(processExited ()),
		       this,SLOT(processLookForFinished()));
     fProcess->setCommunication(QProcess::DupStderr);
     fProcess->setArguments(QStringList("which mpeg_encode"));
     fProcess->start();
#else
     QObject ::connect(fProcess,SIGNAL(finished ( int)),
		       this,SLOT(processLookForFinished()));
     fProcess->setReadChannelMode(QProcess::MergedChannels);
     fProcess->start ("which mpeg_encode");
#endif
  
}

/** @return encoder path or "" if it does not exist
 */
QString G4OpenGLQtViewer::getEncoderPath() {
  return fEncoderPath;
}
 

/**
 * set the new encoder path
 * @return "" if correct. The error otherwise
*/
QString G4OpenGLQtViewer::setEncoderPath(QString path) {
  if (path == "") {
    return "File does not exist";
  }

#if QT_VERSION < 0x040000
  path =  QDir::cleanDirPath(path);
#else
  path =  QDir::cleanPath(path);
#endif
  QFileInfo *f = new QFileInfo(path);
  if (!f->exists()) {
    return "File does not exist";
  } else if (f->isDir()) {
    return "This is a directory";
  } else if (!f->isExecutable()) {
    return "File exist but is not executable";
  } else if (!f->isFile()) {
    return "This is not a file";
  }
  fEncoderPath = path;
  return "";
}


bool G4OpenGLQtViewer::isRecording(){
  if ((fRecordingStep == START) || (fRecordingStep == CONTINUE)) {
    return true;
  }
  return false;
}

bool G4OpenGLQtViewer::isStopped(){
  if (fRecordingStep == STOP) {
    return true;
  }
  return false;
}


bool G4OpenGLQtViewer::isReadyToEncode(){
  if (fRecordingStep == READY_TO_ENCODE) {
    return true;
  }
  return false;
}

void G4OpenGLQtViewer::resetRecording() {
    setRecordingStatus(WAIT);
}

/**
 * set the temp folder path
 * @return "" if correct. The error otherwise
*/
QString G4OpenGLQtViewer::setTempFolderPath(QString path) {

  if (path == "") {
    return "Path does not exist";
  }
#if QT_VERSION < 0x040000
  path =  QDir::cleanDirPath(path);
#else
  path =  QDir::cleanPath(path);
#endif
  QFileInfo *d = new QFileInfo(path);
  if (!d->exists()) {
    return "Path does not exist";
  } else if (!d->isDir()) {
    return "This is not a directory";
  } else if (!d->isReadable()) {
    return path +" is read protected";
  } else if (!d->isWritable()) {
    return path +" is write protected";
  }
  
  fTempFolderPath = path;
  return "";
}

/** @return the temp folder path or "" if it does not exist
 */
QString G4OpenGLQtViewer::getTempFolderPath() {
  return fTempFolderPath;
}
 
/**
 * set the save file name path
 * @return "" if correct. The error otherwise
*/
QString G4OpenGLQtViewer::setSaveFileName(QString path) {

  if (path == "") {
    return "Path does not exist";
  }
  
  QFileInfo *file = new QFileInfo(path);
  QDir dir = file->dir();
#if QT_VERSION < 0x040000
  path =  QDir::cleanDirPath(path);
#else
  path =  QDir::cleanPath(path);
#endif
  if (file->exists()) {
    return "File already exist, please choose a new one";
  } else if (!dir.exists()) {
    return "Dir does not exist";
  } else if (!dir.isReadable()) {
    return path +" is read protected";
  }
  
  fSaveFileName = path;
  return "";
}

/** @return the save file path
 */
QString G4OpenGLQtViewer::getSaveFileName() {
  return fSaveFileName ;
}

/** Create a Qt_temp folder in the temp folder given
* The temp folder will be like this /tmp/QtMovie_12-02-2008_12_12_58/
* @return "" if success. Error message if not.
*/
QString G4OpenGLQtViewer::createTempFolder() {
  fMovieTempFolderPath = "";

  //check
  QString tmp = setTempFolderPath(fTempFolderPath);
  if (tmp != "") {
    return tmp;
  }
#if QT_VERSION < 0x040000
  QString sep = QChar(QDir::separator());
#else
  QString sep = QString(QDir::separator());
#endif
  QString path = sep+"QtMovie_"+QDateTime::currentDateTime ().toString("dd-MM-yyyy_hh-mm-ss")+sep; 
#if QT_VERSION < 0x040000
  QDir *d = new QDir(QDir::cleanDirPath(fTempFolderPath));
#else
  QDir *d = new QDir(QDir::cleanPath(fTempFolderPath));
#endif
  // check if it is already present
  if (d->exists(path)) {
    return "Folder "+path+" already exists.Please remove it first";
  }
  if (d->mkdir(fTempFolderPath+path)) {
    fMovieTempFolderPath = fTempFolderPath+path;
    return "";
  } else {
    return "Can't create "+fTempFolderPath+path;
  }
  return "-";
}

/** Remove the Qt_temp folder in the temp folder
*/
QString G4OpenGLQtViewer::removeTempFolder() {
	// remove files in Qt_temp folder
  if (fMovieTempFolderPath == "") {
    return "";
  }
#if QT_VERSION < 0x040000
  QDir *d = new QDir(QDir::cleanDirPath(fMovieTempFolderPath));
#else
  QDir *d = new QDir(QDir::cleanPath(fMovieTempFolderPath));
#endif
  if (!d->exists()) {
    return "";  // already remove
  }

  d->setFilter( QDir::Files );
  QStringList subDirList = d->entryList();
  int res = true;
  QString error = "";
  for (QStringList::ConstIterator it = subDirList.begin() ;(it != subDirList.end()) ; it++) {
    const QString currentFile = *it;
      if (!d->remove(currentFile)) {
        res = false;
        QString file = fMovieTempFolderPath+currentFile;
        error +="Removing file failed : "+file;
      } else {
      }
  }
  if (res) {
    if (d->rmdir(fMovieTempFolderPath)) {
      fMovieTempFolderPath = "";
      return "";
    } else {
      return "Dir "+fMovieTempFolderPath+" should be empty, but could not remove it";
    }

  }
  return "Could not remove "+fMovieTempFolderPath+" because of the following errors :"+error;
}



bool G4OpenGLQtViewer::hasPendingEvents () {
  return ((QApplication*)G4Qt::getInstance ())->hasPendingEvents ();
}

bool G4OpenGLQtViewer::generateMpegEncoderParameters () {

		// save the parameter file
  FILE* fp;
#if QT_VERSION < 0x040000
  fp = fopen (QString(fMovieTempFolderPath+fParameterFileName).ascii(), "w");
#else
  fp = fopen (QString(fMovieTempFolderPath+fParameterFileName).toStdString().c_str(), "w");
#endif

  if (fp == NULL) {
    setRecordingInfos("Generation of parameter file failed");
    return false;
  }

  fprintf (fp,"# parameter file template with lots of comments to assist you\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# you can use this as a template, copying it to a separate file then modifying\n");
  fprintf (fp,"# the copy\n");
  fprintf (fp,"#\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# any line beginning with '#' is a comment\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# no line should be longer than 255 characters\n");
  fprintf (fp,"#\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# general format of each line is:\n");
  fprintf (fp,"#	  \n");
  fprintf (fp,"#\n");
  fprintf (fp,"# lines can generally be in any order\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# an exception is the option 'INPUT' which must be followed by input\n");
  fprintf (fp,"# files in the order in which they must appear, followed by 'END_INPUT'\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# Also, if you use the `command` method of generating input file names,\n");
  fprintf (fp,"# the command will only be executed in the INPUT_DIR if INPUT_DIR preceeds\n");
  fprintf (fp,"# the INPUT parameter.\n");
  fprintf (fp,"#\n");
  fprintf (fp,"#  MUST be in UPPER CASE\n");
  fprintf (fp,"#\n");
  fprintf (fp,"\n");
  fprintf (fp,"# Pattern affects speed, quality and compression. See the User's Guide\n");
  fprintf (fp,"# for more info.\n");
  fprintf (fp,"\n");
  fprintf (fp,"PATTERN		IBBPBBPBBPBBPBBP\n");
#if QT_VERSION < 0x040000
  fprintf (fp,"OUTPUT		%s\n",getSaveFileName().ascii());
#else
  fprintf (fp,"OUTPUT		%s\n",getSaveFileName().toStdString().c_str());
#endif
  fprintf (fp,"\n");
  fprintf (fp,"# mpeg_encode really only accepts 3 different file formats, but using a\n");
  fprintf (fp,"# conversion statement it can effectively handle ANY file format\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# You must specify the type of the input files.  The choices are:\n");
  fprintf (fp,"#    YUV, PPM, JMOVIE, Y, JPEG, PNM\n");
  fprintf (fp,"#	(must be upper case)\n");
  fprintf (fp,"#\n");
  fprintf (fp,"BASE_FILE_FORMAT	PPM\n");
  fprintf (fp,"\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# if YUV format (or using parallel version), must provide width and height\n");
  fprintf (fp,"# YUV_SIZE	widthxheight\n");
  fprintf (fp,"# this option is ignored if BASE_FILE_FORMAT is not YUV and you're running\n");
  fprintf (fp,"# on just one machine\n");
  fprintf (fp,"#\n");
  fprintf (fp,"YUV_SIZE	352x240\n");
  fprintf (fp,"\n");
  fprintf (fp,"# If you are using YUV, there are different supported file formats.\n");
  fprintf (fp,"# EYUV or UCB are the same as previous versions of this encoder.\n");
  fprintf (fp,"# (All the Y's, then U's then V's, in 4:2:0 subsampling.)\n");
  fprintf (fp,"# Other formats, such as Abekas, Phillips, or a general format are\n");
  fprintf (fp,"# permissible, the general format is a string of Y's, U's, and V's\n");
  fprintf (fp,"# to specify the file order.\n");
  fprintf (fp,"\n");
  fprintf (fp,"INPUT_FORMAT UCB\n");
  fprintf (fp,"\n");
  fprintf (fp,"# the conversion statement\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# Each occurrence of '*' will be replaced by the input file\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# e.g., if you have a bunch of GIF files, then this might be:\n");
  fprintf (fp,"#	INPUT_CONVERT	giftoppm *\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# e.g., if you have a bunch of files like a.Y a.U a.V, etc., then:\n");
  fprintf (fp,"#	INPUT_CONVERT	cat *.Y *.U *.V\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# e.g., if you are grabbing from laser disc you might have something like\n");
  fprintf (fp,"#	INPUT_CONVERT	goto frame *; grabppm\n");
  fprintf (fp,"# 'INPUT_CONVERT *' means the files are already in the base file format\n");
  fprintf (fp,"#\n");
  fprintf (fp,"INPUT_CONVERT	* \n");
  fprintf (fp,"\n");
  fprintf (fp,"# number of frames in a GOP.\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# since each GOP must have at least one I-frame, the encoder will find the\n");
  fprintf (fp,"# the first I-frame after GOP_SIZE frames to start the next GOP\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# later, will add more flexible GOP signalling\n");
  fprintf (fp,"#\n");
  fprintf (fp,"GOP_SIZE	16\n");
  fprintf (fp,"\n");
  fprintf (fp,"# number of slices in a frame\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# 1 is a good number.  another possibility is the number of macroblock rows\n");
  fprintf (fp,"# (which is the height divided by 16)\n");
  fprintf (fp,"#\n");
  fprintf (fp,"SLICES_PER_FRAME	1\n");
  fprintf (fp,"\n");
  fprintf (fp,"# directory to get all input files from (makes this file easier to read)\n");
#if QT_VERSION < 0x040000
  fprintf (fp,"INPUT_DIR	%s\n",fMovieTempFolderPath.ascii());
#else
  fprintf (fp,"INPUT_DIR	%s\n",fMovieTempFolderPath.toStdString().c_str());
#endif
  fprintf (fp,"\n");
  fprintf (fp,"# There are a bunch of ways to specify the input files.\n");
  fprintf (fp,"# from a simple one-per-line listing, to the following \n");
  fprintf (fp,"# way of numbering them.  See the manual for more information.\n");
  fprintf (fp,"INPUT\n");
  fprintf (fp,"# '*' is replaced by the numbers 01, 02, 03, 04\n");
  fprintf (fp,"# if I instead do [01-11], it would be 01, 02, ..., 09, 10, 11\n");
  fprintf (fp,"# if I instead do [1-11], it would be 1, 2, 3, ..., 9, 10, 11\n");
  fprintf (fp,"# if I instead do [1-11+3], it would be 1, 4, 7, 10\n");
  fprintf (fp,"# the program assumes none of your input files has a name ending in ']'\n");
  fprintf (fp,"# if you do, too bad!!!\n");
  fprintf (fp,"#\n");
  fprintf (fp,"#\n");
  fprintf (fp,"Test*.ppm	[0-%d]\n",fRecordFrameNumber-1);
  fprintf (fp,"# can have more files here if you want...there is no limit on the number\n");
  fprintf (fp,"# of files\n");
  fprintf (fp,"END_INPUT\n");
  fprintf (fp,"\n");
  fprintf (fp,"\n");
  fprintf (fp,"\n");
  fprintf (fp,"# Many of the remaining options have to do with the motion search and qscale\n");
  fprintf (fp,"\n");
  fprintf (fp,"# FULL or HALF -- must be upper case\n");
  fprintf (fp,"# Should be FULL for computer generated images\n");
  fprintf (fp,"PIXEL		FULL\n");
  fprintf (fp,"\n");
  fprintf (fp,"# means +/- this many pixels for both P and B frame searches\n");
  fprintf (fp,"# specify two numbers if you wish to serc different ranges in the two.\n");
  fprintf (fp,"RANGE		10\n");
  fprintf (fp,"\n");
  fprintf (fp,"# The two search algorithm parameters below mostly affect speed,\n");
  fprintf (fp,"# with some affect on compression and almost none on quality.\n");
  fprintf (fp,"\n");
  fprintf (fp,"# this must be one of {EXHAUSTIVE, SUBSAMPLE, LOGARITHMIC}\n");
  fprintf (fp,"PSEARCH_ALG	LOGARITHMIC\n");
  fprintf (fp,"\n");
  fprintf (fp,"# this must be one of {SIMPLE, CROSS2, EXHAUSTIVE}\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# note that EXHAUSTIVE is really, really, really slow\n");
  fprintf (fp,"#\n");
  fprintf (fp,"BSEARCH_ALG	SIMPLE\n");
  fprintf (fp,"\n");
  fprintf (fp,"#\n");
  fprintf (fp,"# these specify the q-scale for I, P, and B frames\n");
  fprintf (fp,"# (values must be between 1 and 31)\n");
  fprintf (fp,"# These are the Qscale values for the entire frame in variable bit-rate\n");
  fprintf (fp,"# mode, and starting points (but not important) for constant bit rate\n");
  fprintf (fp,"#\n");
  fprintf (fp,"\n");
  fprintf (fp,"# Qscale (Quantization scale) affects quality and compression,\n");
  fprintf (fp,"# but has very little effect on speed.\n");
  fprintf (fp,"\n");
  fprintf (fp,"IQSCALE		4\n");
  fprintf (fp,"PQSCALE		5\n");
  fprintf (fp,"BQSCALE		12\n");
  fprintf (fp,"\n");
  fprintf (fp,"# this must be ORIGINAL or DECODED\n");
  fprintf (fp,"REFERENCE_FRAME	ORIGINAL\n");
  fprintf (fp,"\n");
  fprintf (fp,"# for parallel parameters see parallel.param in the exmaples subdirectory\n");
  fprintf (fp,"\n");
  fprintf (fp,"# if you want constant bit-rate mode, specify it as follows (number is bits/sec):\n");
  fprintf (fp,"#BIT_RATE  1000000\n");
  fprintf (fp,"\n");
  fprintf (fp,"# To specify the buffer size (327680 is default, measused in bits, for 16bit words)\n");
  fprintf (fp,"BUFFER_SIZE 327680\n");
  fprintf (fp,"\n");
  fprintf (fp,"# The frame rate is the number of frames/second (legal values:\n");
  fprintf (fp,"# 23.976, 24, 25, 29.97, 30, 50 ,59.94, 60\n");
  fprintf (fp,"FRAME_RATE 30\n");
  fprintf (fp,"\n");
  fprintf (fp,"# There are many more options, see the users manual for examples....\n");
  fprintf (fp,"# ASPECT_RATIO, USER_DATA, GAMMA, IQTABLE, etc.\n");
  fprintf (fp,"\n");
  fprintf (fp,"\n");
  fclose (fp);

  setRecordingInfos("Parameter file "+fParameterFileName+" generated in "+fMovieTempFolderPath);
  setRecordingStatus(READY_TO_ENCODE);
  return true;
}

void G4OpenGLQtViewer::encodeVideo()
{
  if ((getEncoderPath() != "") && (getSaveFileName() != "")) {
    setRecordingStatus(ENCODING);
    
#if QT_VERSION < 0x040000
    QStringList args = QStringList(fEncoderPath);
    args.push_back(fMovieTempFolderPath+fParameterFileName);
    fProcess = new QProcess(args);
    QObject ::connect(fProcess,SIGNAL(processExited ()),
                      this,SLOT(processEncodeFinished()));
    QObject ::connect(fProcess,SIGNAL(readyReadStdout ()),
                      this,SLOT(processEncodeStdout()));
    fProcess->setCommunication(QProcess::DupStderr);
    fProcess->launch("");
#else
    fProcess = new QProcess();
#if QT_VERSION > 0x040100
    QObject ::connect(fProcess,SIGNAL(finished ( int,QProcess::ExitStatus)),
                      this,SLOT(processEncodeFinished()));
    QObject ::connect(fProcess,SIGNAL(readyReadStandardOutput ()),
                      this,SLOT(processEncodeStdout()));
#else
    QObject ::connect(fProcess,SIGNAL(finished ( int)),
                      this,SLOT(processEncodeFinished()));
    QObject ::connect(fProcess,SIGNAL(readyReadStandardOutput ()),
                      this,SLOT(processEncodeStdout()));
#endif
    fProcess->setReadChannelMode(QProcess::MergedChannels);
    fProcess->start (fEncoderPath, QStringList(fMovieTempFolderPath+fParameterFileName));
#endif
  }
}


// FIXME : does not work on Qt3
void G4OpenGLQtViewer::processEncodeStdout()
{
#if QT_VERSION > 0x040000
  QString tmp = fProcess->readAllStandardOutput ().data();
  int start = tmp.lastIndexOf("ESTIMATED TIME");
  tmp = tmp.mid(start,tmp.indexOf("\n",start)-start);
#else
  QString tmp = fProcess->readStdout ().data();
  int start = tmp.findRev("ESTIMATED TIME");
  tmp = tmp.mid(start,tmp.find("\n",start)-start);
#endif
  setRecordingInfos(tmp);
}


void G4OpenGLQtViewer::processEncodeFinished()
{

  QString txt = "";
  txt = getProcessErrorMsg();
  if (txt == "") {
    setRecordingStatus(SUCCESS);
  } else {
    setRecordingStatus(FAILED);
  }
  setRecordingInfos(txt+removeTempFolder());
}


void G4OpenGLQtViewer::processLookForFinished() 
 {

  QString txt = getProcessErrorMsg();
  if (txt != "") {
    fEncoderPath = "";
  } else {
#if QT_VERSION > 0x040000
    fEncoderPath = QString(fProcess->readAllStandardOutput ().data()).trimmed();
#else
    fEncoderPath = QString(fProcess->readStdout ().data()).simplifyWhiteSpace();
#endif
    // if not found, return "not found"
    if (fEncoderPath.contains(" ")) {
      fEncoderPath = "";
    } else if (!fEncoderPath.contains("mpeg_encode")) {
      fEncoderPath = "";
    }
    setEncoderPath(fEncoderPath);
  }
  // init temp folder
#if QT_VERSION > 0x040000
  setTempFolderPath(QDir::temp ().absolutePath ());
#else
  // Let's have a try
  setTempFolderPath("/tmp/");
#endif
}


QString G4OpenGLQtViewer::getProcessErrorMsg()
{
  QString txt = "";
#if QT_VERSION < 0x040000
  if (!fProcess->normalExit ()) {
    txt = "Exist status "+ fProcess->exitStatus ();
  }
#else
  if (fProcess->exitCode() != 0) {
    switch (fProcess->error()) {
    case QProcess::FailedToStart:
      txt = "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.\n";
      break;
    case QProcess::Crashed:
      txt = "The process crashed some time after starting successfully.\n";
      break;
    case QProcess::Timedout:
      txt = "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.\n";
      break;
    case QProcess::WriteError:
      txt = "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.\n";
      break;
    case QProcess::ReadError:
      txt = "An error occurred when attempting to read from the process. For example, the process may not be running.\n";
      break;
    case QProcess::UnknownError:
      txt = "An unknown error occurred. This is the default return value of error().\n";
      break;
    }
  }
#endif
   return txt;
}

/*
  
void MultiLayer::exportToSVG(const QString& fname)
{
  QPicture picture;
  QPainter p(&picture);
  for (int i=0;i<(int)graphsList->count();i++)
    {
      Graph *gr=(Graph *)graphsList->at(i);
      Plot *myPlot= (Plot *)gr->plotWidget();
      
      QPoint pos=gr->pos();
      
      int width=int(myPlot->frameGeometry().width());
      int height=int(myPlot->frameGeometry().height());
      
      myPlot->print(&p, QRect(pos,QSize(width,height)));
    }
  
  p.end();
  picture.save(fname, "svg");
}
*/
#endif

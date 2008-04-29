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
// $Id: G4OpenGLStoredQtViewer.cc,v 1.14 2008-04-29 16:58:04 lgarnier Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Class G4OpenGLStoredQtViewer : a class derived from G4OpenGLQtViewer and
//                                G4OpenGLStoredViewer.

#ifdef G4VIS_BUILD_OPENGLQT_DRIVER

//#define GEANT4_QT_DEBUG

#include "G4OpenGLStoredQtViewer.hh"

#include "G4ios.hh"

//#include <qmouseevent.h>
#include <qevent.h> // include <qcontextmenuevent.h>

G4OpenGLStoredQtViewer::G4OpenGLStoredQtViewer
(G4OpenGLStoredSceneHandler& sceneHandler,
 const G4String&  name):
  G4VViewer (sceneHandler, sceneHandler.IncrementViewCount (), name),
  G4OpenGLViewer (sceneHandler),
  G4OpenGLQtViewer (sceneHandler),
  G4OpenGLStoredViewer (sceneHandler)             // FIXME : gerer le pb du parent !
{
#if QT_VERSION < 0x040000
  setFocusPolicy(QWidget::StrongFocus); // enable keybord events
#else
  setFocusPolicy(Qt::StrongFocus); // enable keybord events
#endif
  nbPaint =0;
  hasToRepaint =false;
  if (fViewId < 0) return;  // In case error in base class instantiation.
}

G4OpenGLStoredQtViewer::~G4OpenGLStoredQtViewer() {
#ifdef GEANT4_QT_DEBUG
  printf("GLWidget::~GLWidget \n");
#endif
  makeCurrent();
  // this is connect to the Dialog for deleting it properly
  // when close event.
  //   ((QDialog*)window())->reject();
#ifdef GEANT4_QT_DEBUG
  printf("GLWidget::~GLWidget END\n");
#endif
}

void G4OpenGLStoredQtViewer::Initialise() {
#ifdef GEANT4_QT_DEBUG
  printf("GLWidget::Initialise \n");
#endif
  readyToPaint = false;
  CreateGLQtContext ();
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::Initialise () 2\n");
#endif
  CreateMainWindow (this);
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::Initialise () 3\n");
#endif
  CreateFontLists ();  // FIXME Does nothing!
  
#ifdef GEANT4_QT_DEBUG
  printf("readyToPaint = true \n");
#endif
  readyToPaint = true;
}

void G4OpenGLStoredQtViewer::initializeGL () {

  InitializeGLView ();

#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::InitialiseGL () 1\n");
#endif

  // clear the buffers and window.
  ClearView ();
  //   printf("G4OpenGLStoredQtViewer::InitialiseGL () 2\n");
  FinishView ();
   
  glDepthFunc (GL_LEQUAL);
  glDepthMask (GL_TRUE);

  hasToRepaint =true;

#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::InitialiseGL  -------------------------------------------------------------------------------------\n");
#endif
}


void G4OpenGLStoredQtViewer::DrawView () {

#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::DrawView %d %d   VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n",WinSize_x, WinSize_y);
#endif
  G4ViewParameters::DrawingStyle style = GetViewParameters().GetDrawingStyle();

  //Make sure current viewer is attached and clean...
  //Qt version needed
  //   glViewport (0, 0, WinSize_x, WinSize_y);

  //See if things have changed from last time and remake if necessary...
  // The fNeedKernelVisit flag might have been set by the user in
  // /vis/viewer/rebuild, but if not, make decision and set flag only
  // if necessary...
  if (!fNeedKernelVisit) 

    if (!fNeedKernelVisit) KernelVisitDecision ();
   
  G4bool kernelVisitWasNeeded = fNeedKernelVisit; // Keep (ProcessView resets).
  ProcessView ();
   

  if(style!=G4ViewParameters::hlr &&
     haloing_enabled) {
#ifdef GEANT4_QT_DEBUG
    printf("G4OpenGLStoredQtViewer::DrawView DANS LE IF\n");
#endif

    HaloingFirstPass ();
    DrawDisplayLists ();
    glFlush ();

    HaloingSecondPass ();

    DrawDisplayLists ();
    FinishView ();

  } else {
#ifdef GEANT4_QT_DEBUG
    printf("***************************  CASE 1 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n");
#endif
     
    // If kernel visit was needed, drawing and FinishView will already
    // have been done, so...
    if (!kernelVisitWasNeeded) {
#ifdef GEANT4_QT_DEBUG
      printf("***************************  CASE 2 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n");
#endif
      DrawDisplayLists ();
      FinishView ();
    } else {
#ifdef GEANT4_QT_DEBUG
      printf("***************************  CASE 3 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n");
#endif
      // However, union cutaways are implemented in DrawDisplayLists, so make
      // an extra pass...
      if (fVP.IsCutaway() &&
          fVP.GetCutawayMode() == G4ViewParameters::cutawayUnion) {
        ClearView();
        DrawDisplayLists ();
        FinishView ();
#ifdef GEANT4_QT_DEBUG
        printf("***************************  CASE 4 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n");
#endif
      } else { // ADD TO AVOID KernelVisit=1 and nothing to display
        DrawDisplayLists ();
        FinishView ();
      }
    }
  }

  if (isRecording()) {
    savePPMToTemp();
  }

#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::DrawView %d %d ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n",WinSize_x, WinSize_y);
#endif
  hasToRepaint =true;
}


//////////////////////////////////////////////////////////////////////////////
void G4OpenGLStoredQtViewer::FinishView (
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::FinishView VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n");
#endif

  glFlush ();
  swapBuffers ();
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::FinishView ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n");
#endif

}


/**
   - Lors du resize de la fenetre, on doit non pas redessiner le detecteur, mais aussi les evenements
*/
void G4OpenGLStoredQtViewer::resizeGL(
                                      int aWidth
                                      ,int aHeight)
{  
  setupViewport(aWidth,aHeight);

  if (((WinSize_x != (G4int)aWidth)) || (WinSize_y != (G4int) aHeight)) {
    hasToRepaint =true;
  }
  WinSize_x = (G4int) aWidth;
  WinSize_y = (G4int) aHeight;
  
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::resizeGL ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ %d %d=%d %d=%d\n",hasToRepaint,width(),aWidth,height(),aHeight);
#endif
}


/** 
    @see :
*/

void G4OpenGLStoredQtViewer::paintGL()
{
  if (!readyToPaint) {
#ifdef GEANT4_QT_DEBUG
    printf("G4OpenGLStoredQtViewer::paintGL ============  Not ready %d\n",readyToPaint);
#endif
    readyToPaint= true;
    return;
  }
  // DO NOT RESIZE IF SIZE HAS NOT CHANGE :
  //    WHEN CLICK ON THE FRAME FOR EXAMPLE
  //    EXECEPT WHEN MOUSE MOVE EVENT
  if ( !hasToRepaint) {
    if (((WinSize_x == (G4int)width())) &&(WinSize_y == (G4int) height())) {
#ifdef GEANT4_QT_DEBUG
      printf("G4OpenGLStoredQtViewer::paintGL ============  Dont repaint\n");
#endif
      return;
    }
  }
  nbPaint++;
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::paintGL VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV %d ready %d\n",nbPaint,readyToPaint);
#endif
  WinSize_x = (G4int) width();
  WinSize_y = (G4int) height();
     
  setupViewport(width(),height());
  //     glViewport (0, 0, width(), height());
  //   glLoadIdentity();
     
     
  SetView();
     
  //   //  printf("before ClearView\n");
#ifdef GEANT4_QT_DEBUG
  printf("    ClearView\n");
#endif
     
  ClearView (); //ok, put the background correct
  DrawView();
     
  hasToRepaint =false;
     
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::paintGL ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ %d ready %d\n",nbPaint,readyToPaint);
#endif
}

void G4OpenGLStoredQtViewer::mousePressEvent(QMouseEvent *event)
{
#if QT_VERSION < 0x040000
  if ((event->button() & Qt::LeftButton)
      && !((event->state() & Qt::ShiftButton)
           || (event->state() & Qt::ControlButton)
           || (event->state() & Qt::AltButton)
           || (event->state() & Qt::MetaButton))) {
#else
  if ((event->buttons() & Qt::LeftButton)
      && !((event->modifiers() & Qt::ShiftModifier)
           || (event->modifiers() & Qt::ControlModifier)
           || (event->modifiers() & Qt::AltModifier)
           || (event->modifiers() & Qt::MetaModifier))) {
#endif
#ifdef GEANT4_QT_DEBUG
    printf("G4OpenGLStoredQtViewer::mousePressEvent\n");
#endif
    setMouseTracking(true);
    G4MousePressEvent(event->pos(),Qt::LeftButton);
  }
}

void G4OpenGLStoredQtViewer::keyPressEvent (QKeyEvent * event) 
{
  G4keyPressEvent(event);
}

/**
 * This function was build in order to make a zoom on double clic event.
 * It was think to build a rubberband on the zoom area, but never work fine
 */
void G4OpenGLStoredQtViewer::mouseDoubleClickEvent(QMouseEvent *event)
{
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::mouseDoubleClickEvent\n");
#endif
  setMouseTracking(true);
  //   glBufferImage = grabFrameBuffer().convertToFormat(QImage::Format_ARGB32);//_Premultiplied);  
}

void G4OpenGLStoredQtViewer::mouseReleaseEvent(QMouseEvent *event)
{
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::mouseReleaseEvent\n");
#endif
  setMouseTracking(false);
}

void G4OpenGLStoredQtViewer::mouseMoveEvent(QMouseEvent *event)
{
  
#if QT_VERSION < 0x040000
  G4MouseMoveEvent(event->x(),event->y(),event->state());
#else
  G4MouseMoveEvent(event->x(),event->y(),event->buttons());
#endif
  if (hasPendingEvents ())
#if QT_VERSION < 0x040000
    G4MouseMoveEvent(event->x(),event->y(),event->state());
#else
    G4MouseMoveEvent(event->x(),event->y(),event->buttons());
#endif
}


void G4OpenGLStoredQtViewer::contextMenuEvent(QContextMenuEvent *e)
{
#ifdef GEANT4_QT_DEBUG
  printf("G4OpenGLStoredQtViewer::contextMenuEvent\n");
#endif
  manageContextMenuEvent(e);
}

void G4OpenGLStoredQtViewer::updateQWidget() {
  hasToRepaint= true;
  updateGL();
  hasToRepaint= false;
}


#endif

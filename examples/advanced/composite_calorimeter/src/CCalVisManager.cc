///////////////////////////////////////////////////////////////////////////////
// File: CCalVisManager.cc
// Description: CCalVisManager visualisation manager for the example
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#ifdef G4VIS_USE
#include "CCalVisManager.hh"

// Supported drivers...

#ifdef G4VIS_USE_DAWN
#include "G4FukuiRenderer.hh"
#endif

#ifdef G4VIS_USE_DAWNFILE
#include "G4DAWNFILE.hh"
#endif

#ifdef G4VIS_USE_OPACS
#include "G4Wo.hh"
#include "G4Xo.hh"
#endif

#ifdef G4VIS_USE_OPENGLX
#include "G4OpenGLImmediateX.hh"
#include "G4OpenGLStoredX.hh"
#endif

#ifdef G4VIS_USE_OPENGLXM
#include "G4OpenGLImmediateXm.hh"
#include "G4OpenGLStoredXm.hh"
#endif

#ifdef G4VIS_USE_OIX
//#include "G4OpenInventorX.hh"
#endif

#ifdef G4VIS_USE_OIWIN32
//#include "G4OpenInventorWin32.hh"
#endif

#ifdef G4VIS_USE_RAYX
#include "G4RayX.hh"
#endif

#ifdef G4VIS_USE_VRML
//#include "G4VRML1.hh"
#include "G4VRML2.hh"
#endif

#ifdef G4VIS_USE_VRMLFILE
//#include "G4VRML1File.hh"
#include "G4VRML2File.hh"
#endif

CCalVisManager::CCalVisManager (G4int verboseLevel) {
  SetVerboseLevel( verboseLevel );
}

void CCalVisManager::RegisterGraphicsSystems () {

#ifdef G4VIS_USE_DAWN
  RegisterGraphicsSystem (new G4FukuiRenderer);
#endif
 
#ifdef G4VIS_USE_DAWNFILE
  RegisterGraphicsSystem (new G4DAWNFILE);
#endif

#ifdef G4VIS_USE_OPACS
  RegisterGraphicsSystem (new G4Wo);
  RegisterGraphicsSystem (new G4Xo);
#endif

#ifdef G4VIS_USE_OPENGLX
  RegisterGraphicsSystem (new G4OpenGLImmediateX);
  RegisterGraphicsSystem (new G4OpenGLStoredX);
#endif

#ifdef G4VIS_USE_OPENGLXM
  RegisterGraphicsSystem (new G4OpenGLImmediateXm);
  RegisterGraphicsSystem (new G4OpenGLStoredXm);
#endif

#ifdef G4VIS_USE_OIX
  //  RegisterGraphicsSystem (new G4OpenInventorX);
#endif

#ifdef G4VIS_USE_OIWIN32
  //  RegisterGraphicsSystem (new G4OpenInventorWin32);
#endif

#ifdef G4VIS_USE_RAYX
  RegisterGraphicsSystem (new G4RayX);
#endif

#ifdef G4VIS_USE_VRML
  //  RegisterGraphicsSystem (new G4VRML1);
 RegisterGraphicsSystem (new G4VRML2);
#endif

#ifdef G4VIS_USE_VRMLFILE
 //  RegisterGraphicsSystem (new G4VRML1File);
 RegisterGraphicsSystem (new G4VRML2File);
#endif

  if (fVerbose > 0) {
    cout <<
      "\nYou have successfully chosen to use the following graphics systems."
	 << endl;
    PrintAvailableGraphicsSystems ();
  }
}

#endif

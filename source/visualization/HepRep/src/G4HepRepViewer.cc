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

/**
 * @author Mark Donszelmann
 */

//HepRep
#include "HEPREP/HepRep.h"

//G4
#include "G4Types.hh"
//#include "G4VInteractorManager.hh"
#include "G4Scene.hh"
#include "G4HepRep.hh"
#include "G4HepRepSceneHandler.hh"
//This
#include "G4HepRepViewer.hh"

using namespace HEPREP;
using namespace std;

G4HepRepViewer::G4HepRepViewer (G4VSceneHandler& sceneHandler, const G4String& name)
        : G4VViewer (sceneHandler, sceneHandler.IncrementViewCount(), name) {

#ifdef SDEBUG
    cout << "G4HepRepViewer::G4HepRepViewer " << name << endl;
#endif

    // Make changes to view parameters for HepRep...
    fVP.SetCulling(false);
    fDefaultVP.SetCulling(false);
}



G4HepRepViewer::~G4HepRepViewer () {
#ifdef SDEBUG
    cout << "G4HepRepViewer::~G4HepRepViewer" << endl;
#endif
    dynamic_cast<G4HepRep*>(GetSceneHandler()->GetGraphicsSystem())->removeViewer();
}


void G4HepRepViewer::ClearView () {
#ifdef SDEBUG
    cout << "G4HepRepViewer::ClearView" << endl;
#endif
}

void G4HepRepViewer::SetView () {
#ifdef SDEBUG
    cout << "G4HepRepViewer::SetView" << endl;
#endif
}


/* NOTE:
    /vis/viewer/flush calls /vis/viewer/refresh followed by /vis/viewer/update
    /vis/viewer/refrash calls SetView, ClearView, DrawView
    /vis/viewer/update calls ShowView
*/
void G4HepRepViewer::DrawView () {
#ifdef SDEBUG
    cout << "G4HepRepViewer::DrawView" << endl;
#endif
}

void G4HepRepViewer::ShowView () {
#ifdef SDEBUG
    cout << "G4HepRepViewer::ShowView" << endl;
#endif
    G4VViewer::ShowView();

    G4HepRepSceneHandler* sceneHandler = dynamic_cast<G4HepRepSceneHandler*>(GetSceneHandler());
    if (sceneHandler->closeHepRep()) {
        sceneHandler->openHepRep();
    }
}

void G4HepRepViewer::FinishView () {
#ifdef SDEBUG
    cout << "G4HepRepViewer::FinishView" << endl;
#endif
    G4VViewer::FinishView();
}

void G4HepRepViewer::ProcessScene() {
// draws the geometry
    NeedKernelVisit();
    ProcessView();
}

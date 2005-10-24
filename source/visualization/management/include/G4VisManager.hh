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
// $Id: G4VisManager.hh,v 1.39 2005-10-24 11:32:43 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

// Class Description:
//
// The GEANT4 Visualization Manager - John Allison 02/Jan/1996.
//
// G4VisManager is a "Singleton", i.e., only one instance of it or any
// derived class may exist.  A G4Exception is thrown if an attempt is
// made to instantiate more than one.
//
// It is also an abstract class, so the user must derive his/her own
// class from G4VisManager, implement the pure virtual function
// RegisterGraphicsSystems, and instantiate an object of the derived
// class - for an example see
// visualization/include/MyVisManager.hh/cc.
//
// The recommended way for users to obtain a pointer to the vis
// manager is with G4VVisManager::GetConcreteInstance (), being always
// careful to test for non-zero.  This pointer is non-zero only when
// (a) an object of the derived class exists and (b) when there is a
// valid viewer available.
//
// Graphics system registration is normally done through the protected
// pure virtual function RegisterGraphicsSystems called from
// Initialise ().  You can also use the public function
// RegisterGraphicsSystem (new MyGraphicsSystem) if you have your own
// graphics system. A graphics system is, in effect, a factory for
// scene handlers and viewers.
//
// The VisManager creates and manages graphics systems, scenes, scene
// handlers, viewers and some models and model makers.  You can have
// any number.  It has the concept of a "current viewer", and the
// "current scene handler", the "current scene" and the "current
// graphics system" which go with it.  You can select the current
// viewer.  Most of the the operations of the VisManager take place
// with the current viewer, in particular, the Draw operations.
//
// Each scene comprises drawable objects such as detector components
// and hits when appropriate.  A scene handler translates a scene into
// graphics-system-specific function calls and, possibly, a
// graphics-system-dependent database - display lists, scene graphs,
// etc.  Each viewer has its "view parameters" (see class description
// of G4ViewParameters for available parameters and also for a
// description of the concept of a "standard view" and all that).
//
// A friend class G4VisStateDependent is "state dependent", i.e., it
// is notified on change of state (G4ApplicationState).  This is used
// to message the G4VisManager to draw hits and trajectories in the
// current scene at the end of event, as required.

#ifndef G4VISMANAGER_HH
#define G4VISMANAGER_HH

#include "G4VVisManager.hh"

#include "globals.hh"
#include "G4GraphicsSystemList.hh"
#include "G4SceneHandlerList.hh"
#include "G4SceneList.hh"
#include "G4Transform3D.hh"
#include "G4NullModel.hh"
#include "G4TrajectoriesModel.hh"
#include "G4ModelingParameters.hh"

#include <iostream>
#include <vector>

class G4Scene;
class G4UIcommand;
class G4UImessenger;
class G4VisStateDependent;
class G4VUserVisAction;
class G4VTrajectoryModel;
class G4VTrajectoryModelMaker;

class G4VisManager: public G4VVisManager {

  // Friends - classes and functions which need access to private
  // members of G4VisManager.  This is mainly to obtain access to
  // GetInstance (), which is private.  The correct way for normal
  // users to obtain a pointer to the vis manager is with
  // G4VVisManager::GetConcreteInstance (), always testing for
  // non-zero.

  // Odd friends that need access to various methods of the G4VisManager...
  friend class G4RTSteppingAction;
  friend class G4RayTrajectory;

  // Management friends...
  friend class G4VSceneHandler;
  friend class G4VViewer;

  friend std::ostream & operator <<
  (std::ostream &, const G4VGraphicsSystem &);

  friend std::ostream & operator <<
  (std::ostream &, const G4VSceneHandler &);

  friend class G4VisStateDependent;

public: // With description

  enum Verbosity {
    quiet,         // Nothing is printed.
    startup,       // Startup and endup messages are printed...
    errors,        // ...and errors...
    warnings,      // ...and warnings...
    confirmations, // ...and confirming messages...
    parameters,    // ...and parameters of scenes and views...
    all            // ...and everything available.
  };
  // Simple graded message scheme.

protected: // With description

  G4VisManager ();
  // The constructor is protected so that an object of the derived
  // class may be constructed.

public: // With description

  virtual ~G4VisManager ();

private:

  // Private copy constructor and assigment operator - copying and
  // assignment not allowed.  Keeps CodeWizard happy.
  G4VisManager (const G4VisManager&);
  G4VisManager& operator = (const G4VisManager&);

  static G4VisManager* GetInstance ();
  // Returns pointer to itself.  Throws a G4Exception if called before
  // instantiation.  Private so that only friends can use; the normal
  // user should instead use G4VVisManager::GetConcreteInstance () to
  // get a "higher level" pointer for general use - but always test
  // for non-zero.

public: // With description

  void Initialise ();
  void Initialize ();  // Alias Initialise ().

  G4bool RegisterGraphicsSystem (G4VGraphicsSystem*);
  // Register an individual graphics system.  Normally this is done in
  // a sub-class implementation of the protected virtual function,
  // RegisterGraphicsSystems.  See, e.g., G4VisExecutive.icc.

  G4bool RegisterTrajectoryModelMaker (G4VTrajectoryModelMaker*);
  // Register an trajectory model maker.  Normally this is done in
  // a sub-class implementation of the protected virtual function,
  // RegisterTrajectoryModelMakers.  See, e.g., G4VisExecutive.icc.

  G4bool RegisterTrajectoryModel (G4VTrajectoryModel*);
  // Register an individual trajectory model.

  /////////////////////////////////////////////////////////////////
  // Now functions that implement the pure virtual functions of
  // G4VVisManager for drawing various visualization primitives, useful
  // for representing hits, digis, etc.

  void Draw (const G4Circle&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4NURBS&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4Polyhedron&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4Polyline&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4Polymarker&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4Scale&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4Square&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4Text&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  ////////////////////////////////////////////////////////////////////
  // Now functions that implement the pure virtual functions of
  // G4VVisManager for drawing a GEANT4 object.  Note that the
  // visualization attributes needed in some cases override any
  // visualization attributes that are associated with the object
  // itself - thus you can, for example, change the colour of a
  // physical volume.

  void Draw (const G4VHit&);

  void Draw (const G4VTrajectory&, G4int i_mode);
  // i_mode is a parameter that can be used to control the drawing of
  // the trajectory.  See, e.g., G4VTrajectory::DrawTrajectory.
  // i_mode defaults to 0 by inheritance from G4VVisManager.

  void Draw (const G4LogicalVolume&, const G4VisAttributes&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4VPhysicalVolume&, const G4VisAttributes&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  void Draw (const G4VSolid&, const G4VisAttributes&,
    const G4Transform3D& objectTransformation = G4Transform3D::Identity);

  ////////////////////////////////////////////////////////////////////////
  // Now other pure virtual functions of G4VVisManager...

  void GeometryHasChanged ();
  // Used by run manager to notify change.

  void DispatchToCurrentModel(const G4VTrajectory&, G4int i_mode);
  // Draw the trajectory.

  ////////////////////////////////////////////////////////////////////////
  // Administration routines.

  void CreateSceneHandler (G4String name = "");
  // Creates scene handler for the current system.

  void CreateViewer  (G4String name = "");
  // Creates viewer for the current scene handler.

private:

  void BeginOfRun ();

  void BeginOfEvent ();

  void EndOfEvent ();
  // This is called on change of state (G4ApplicationState).  It is
  // used to draw hits and trajectories if included in the current
  // scene at the end of event, as required.

  void EndOfRun ();

public: // With description

  /////////////////////////////////////////////////////////////////////
  // Access functions.

  void Enable();
  void Disable();
  // Global enable/disable functions.

  G4VUserVisAction*            GetUserAction               () const;
  G4VisExtent                  GetUserActionExtent         () const;
  G4VGraphicsSystem*           GetCurrentGraphicsSystem    () const;
  G4Scene*                     GetCurrentScene             () const;
  G4VSceneHandler*             GetCurrentSceneHandler      () const;
  G4VViewer*                   GetCurrentViewer            () const;
  const G4GraphicsSystemList&  GetAvailableGraphicsSystems ();
  // The above is non-const because it checks and updates the List by
  // calling RegisterGraphicsSystems() if no graphics systems are
  // already registered.
  const G4SceneHandlerList&    GetAvailableSceneHandlers   () const;
  const G4SceneList&           GetSceneList                () const;
  Verbosity                    GetVerbosity                () const;
  void  GetWindowSizeHint (G4int& xHint, G4int& yHint) const;
  // Note: GetWindowSizeHint information is returned via the G4int& arguments.
  const G4String&              GetXGeometryString          () const;

  void SetUserAction (G4VUserVisAction* pVisAction,
		      const G4VisExtent& = G4VisExtent::NullExtent);
  void SetUserActionExtent (const G4VisExtent&);
  void              SetCurrentGraphicsSystem    (G4VGraphicsSystem* pSystem);
  void              SetCurrentScene             (G4Scene*);
  void              SetCurrentSceneHandler      (G4VSceneHandler* pScene);
  void              SetCurrentViewer            (G4VViewer* pView);
  G4SceneHandlerList& SetAvailableSceneHandlers ();  // Returns lvalue.
  G4SceneList&      SetSceneList                ();  // Returns lvalue.
  void              SetVerboseLevel             (G4int);
  void              SetVerboseLevel             (const G4String&);
  void              SetVerboseLevel             (Verbosity);
  void              SetWindowSizeHint           (G4int xHint, G4int yHint);
  void              SetXGeometryString          (const G4String&);


  /////////////////////////////////////////////////////////////////////
  // Utility functions.

  G4String ViewerShortName (const G4String& viewerName) const;
  // Returns shortened version of viewer name, i.e., up to first space,
  // if any.

  G4VViewer* GetViewer (const G4String& viewerName) const;
  // Returns zero if not found.  Can use long or short name, but find
  // is done on short name.

  static Verbosity GetVerbosityValue(const G4String&);
  // Returns verbosity given a string.  (Uses first character only.)

  static Verbosity GetVerbosityValue(G4int);
  // Returns verbosity given an integer.  If integer is out of range,
  // selects verbosity at extreme of range.

  static G4String VerbosityString(Verbosity);
  // Converts the verbosity into a string for suitable for printing.
  
  static std::vector<G4String> VerbosityGuidanceStrings;
  // Guidance on the use of visualization verbosity.

protected:

  virtual void RegisterGraphicsSystems () = 0;
  // The sub-class must implement and make successive calls to
  // RegisterGraphicsSystem.

  virtual void RegisterTrajectoryModelMakers
  (const G4String& commandPrefix) = 0;
  // The sub-class must implement and make successive calls to
  // RegisterTrajectoryModelMaker and arrange to pass on the command
  // prefix to any messengers.  Those messengers then prefix their
  // command names with this string.

  void RegisterMessengers              ();   // Command messengers.
  void PrintAvailableGraphicsSystems   () const;
  void PrintInvalidPointers            () const;
  G4bool IsValidView ();
  // True if view is valid.  Prints messages and sanitises various data.
  void ClearTransientStoreIfMarked();
  // Clears transient store of current scene handler if it is marked
  // for clearing.  Assumes view is valid.
  void CheckModel ();
  // If a scene handler has no model, provides a null model.

  static G4VisManager*  fpInstance;         // Pointer to single instance.
  G4bool                fInitialised;
  G4VUserVisAction*     fpUserVisAction;    // User vis action callback.
  G4VisExtent           fUserVisActionExtent;
  G4VGraphicsSystem*    fpGraphicsSystem;   // Current graphics system.
  G4Scene*              fpScene;            // Current scene.
  G4VSceneHandler*      fpSceneHandler;     // Current scene handler.
  G4VViewer*            fpViewer;           // Current viewer.
  G4GraphicsSystemList  fAvailableGraphicsSystems;
  G4SceneList           fSceneList;
  G4SceneHandlerList    fAvailableSceneHandlers;
  Verbosity             fVerbosity;
  const G4int           fVerbose;
  // fVerbose is kept for backwards compatibility for some user
  // examples.  (It is used in the derived user vis managers to print
  // available graphics systems.)  It is initialised to 1 in the
  // constructor and cannot be changed.
  std::vector<G4UImessenger*> fMessengerList;
  std::vector<G4UIcommand*>   fDirectoryList;
  G4VisStateDependent*  fpStateDependent;   // Friend state dependent class.
  G4int fWindowSizeHintX, fWindowSizeHintY; // For viewer...
  G4String fXGeometryString;                // ...construction.
  G4NullModel fVisManagerNullModel;         // As a default.
  G4TrajectoriesModel dummyTrajectoriesModel;  // For passing drawing mode.
  G4ModelingParameters fVisManagerModelingParameters;  // Useful memory.
  G4VTrajectoryModel* fpCurrentTrajectoryModel;
};

#include "G4VisManager.icc"

#endif

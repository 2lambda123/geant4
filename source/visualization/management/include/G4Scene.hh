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
// $Id: G4Scene.hh,v 1.13 2003-06-16 17:14:03 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Scene  John Allison  19th July 1996.
//
// Class Description:
//
// Defines the viewable scene.

#ifndef G4SCENE_HH
#define G4SCENE_HH

#include "globals.hh"
#include "G4ios.hh"

class G4VPhysicalVolume;

#include "G4VisExtent.hh"
#include "G4Point3D.hh"
#include "G4VModel.hh"
#include <vector>

class G4Scene {

public: // With description

  friend std::ostream& operator << (std::ostream& os, const G4Scene& d);

  enum {UNLIMITED = -1};

  G4Scene (const G4String& name = "scene-with-unspecified-name");
  ~G4Scene ();

  // Makes use of default (compiler generated) copy constructor and
  // assignment operator.

  G4bool operator == (const G4Scene&) const;
  G4bool operator != (const G4Scene&) const;

  //////////////////////////////////////////////////////
  // Get functions...

  const G4String& GetName () const;

  G4bool IsEmpty () const;

  const std::vector<G4VModel*>& GetRunDurationModelList () const;
  // Contains models which are expected to last for the duration of
  // the run, for example geometry volumes.

  const std::vector<G4VModel*>& GetEndOfEventModelList () const;
  // Contains models which are described at the end of event when the
  // scene is current.

  const G4VisExtent& GetExtent () const;
  // Overall extent of all objects in the scene.

  const G4Point3D& GetStandardTargetPoint () const;
  // Usually centre of extent.  See G4ViewParameters for definition.

  G4bool GetRefreshAtEndOfEvent () const;
  // If true, the visualization manager will request viewer to refresh
  // "transient" objects, such as hits, at end of event.  Otherwise
  // they will be accumulated.

  //////////////////////////////////////////////
  // Add, Set, Clear functions...

  G4bool AddRunDurationModel (G4VModel*, G4bool warn = false);
  // Adds models of type which are expected to last for the duration
  // of the run, for example geometry volumes.
  // Returns false if model is already in the list.
  // Prints warnings if warn is true.

  G4bool AddWorldIfEmpty (G4bool warn = false);
  // In some situations, if the user asks for a drawing and has not
  // yet set any run duration models it makes sense to put the "world"
  // in there by default.
  // Returns false if model is already in the list.
  // Prints warnings if warn is true.

  G4bool AddEndOfEventModel (G4VModel*, G4bool warn = false);
  // Adds models of type which are described at the end of event when
  // the scene is current.
  // Returns false if model is already in the list.
  // Prints warnings if warn is true.

  std::vector<G4VModel*>& SetRunDurationModelList ();
  // Allows you to change the model list - do with care!

  std::vector<G4VModel*>& SetEndOfEventModelList ();
  // Allows you to change the model list - do with care!

  void Clear ();
  // Clears and destroys models in all lists.

  void SetRefreshAtEndOfEvent(G4bool);
  // If set true, the visualization manager will request viewer to
  // refresh "transient" objects, such as hits, at end of event.
  // Otherwise they will be accumulated.

  void CalculateExtent();
  // (Re-)calculates the extent from the extents of its models.

private:
  G4String fName;
  std::vector<G4VModel*> fRunDurationModelList;
  std::vector<G4VModel*> fEndOfEventModelList;
  G4VisExtent fExtent;
  G4Point3D   fStandardTargetPoint;
  G4bool      fRefreshAtEndOfEvent;
};

#include "G4Scene.icc"

#endif

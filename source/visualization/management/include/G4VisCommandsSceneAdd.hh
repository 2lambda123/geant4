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
// $Id: G4VisCommandsSceneAdd.hh,v 1.13 2005-03-09 16:25:25 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

// /vis/scene commands - John Allison  9th August 1998

#ifndef G4VISCOMMANDSSCENEADD_HH
#define G4VISCOMMANDSSCENEADD_HH

#include "G4VisCommandsScene.hh"

class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;

#include "G4Transform3D.hh"
#include "G4VisAttributes.hh"

class G4VisCommandSceneAddAxes: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddAxes ();
  virtual ~G4VisCommandSceneAddAxes ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddAxes (const G4VisCommandSceneAddAxes&);
  G4VisCommandSceneAddAxes& operator = (const G4VisCommandSceneAddAxes&);
  G4UIcommand* fpCommand;
};

class G4VisCommandSceneAddGhosts: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddGhosts ();
  virtual ~G4VisCommandSceneAddGhosts ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddGhosts (const G4VisCommandSceneAddGhosts&);
  G4VisCommandSceneAddGhosts& operator =
  (const G4VisCommandSceneAddGhosts&);
  G4UIcmdWithAString* fpCommand;
};

class G4VisCommandSceneAddHits: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddHits ();
  virtual ~G4VisCommandSceneAddHits ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddHits (const G4VisCommandSceneAddHits&);
  G4VisCommandSceneAddHits& operator = (const G4VisCommandSceneAddHits&);
  G4UIcmdWithoutParameter* fpCommand;
};

class G4VisCommandSceneAddLogicalVolume: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddLogicalVolume ();
  virtual ~G4VisCommandSceneAddLogicalVolume ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddLogicalVolume (const G4VisCommandSceneAddLogicalVolume&);
  G4VisCommandSceneAddLogicalVolume& operator =
  (const G4VisCommandSceneAddLogicalVolume&);
  G4UIcommand* fpCommand;
};

class G4VisCommandSceneAddLogo: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddLogo ();
  virtual ~G4VisCommandSceneAddLogo ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddLogo (const G4VisCommandSceneAddLogo&);
  G4VisCommandSceneAddLogo& operator = (const G4VisCommandSceneAddLogo&);
  class G4Logo {
  public:
    G4Logo(G4double height, const G4VisAttributes&);
    ~G4Logo();
    void operator()(const G4Transform3D&);
  private:
    G4double fHeight;
    G4VisAttributes fVisAtts;
    G4Polyhedron *fpG, *fp4;
  };
  G4UIcommand* fpCommand;
};

class G4VisCommandSceneAddScale: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddScale ();
  virtual ~G4VisCommandSceneAddScale ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddScale (const G4VisCommandSceneAddScale&);
  G4VisCommandSceneAddScale& operator = (const G4VisCommandSceneAddScale&);
  G4UIcommand* fpCommand;
};

class G4VisCommandSceneAddText: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddText ();
  virtual ~G4VisCommandSceneAddText ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddText (const G4VisCommandSceneAddText&);
  G4VisCommandSceneAddText& operator = (const G4VisCommandSceneAddText&);
  G4UIcommand* fpCommand;
};

class G4VisCommandSceneAddTrajectories: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddTrajectories ();
  virtual ~G4VisCommandSceneAddTrajectories ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddTrajectories (const G4VisCommandSceneAddTrajectories&);
  G4VisCommandSceneAddTrajectories& operator =
  (const G4VisCommandSceneAddTrajectories&);
  G4UIcmdWithAnInteger* fpCommand;
};

class G4VisCommandSceneAddUserAction: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddUserAction ();
  virtual ~G4VisCommandSceneAddUserAction ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddUserAction (const G4VisCommandSceneAddUserAction&);
  G4VisCommandSceneAddUserAction& operator = (const G4VisCommandSceneAddUserAction&);
  G4UIcommand* fpCommand;
};

class G4VisCommandSceneAddVolume: public G4VVisCommandScene {
public:
  G4VisCommandSceneAddVolume ();
  virtual ~G4VisCommandSceneAddVolume ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandSceneAddVolume (const G4VisCommandSceneAddVolume&);
  G4VisCommandSceneAddVolume& operator = (const G4VisCommandSceneAddVolume&);
  G4UIcommand* fpCommand;
};

#endif

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
// $Id: G4RTXScanner.hh,v 1.2 2005-07-20 20:39:02 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

#ifdef G4VIS_BUILD_RAYTRACERX_DRIVER

#ifndef G4RTXScanner_H
#define G4RTXScanner_H 1

// class description:
//
// G4RTXScanner
// Provides a sequence of window coordinates suitable for a visible
// window of ever increasing resolution.

#include "G4VRTScanner.hh"

class G4RayTracer;

class G4RTXScanner: public G4VRTScanner {

public: // with description

  G4RTXScanner();

  // Compiler defaults for destructor, copy constructor and
  // assignmemt.

  virtual const G4String& GetGSName() const;
  // Get name that acts as graphics system name.

  virtual const G4String& GetGSNickname() const;
  // Get name that acts as graphics system nickname.  It is this that
  // the user specifies on the /vis/open and /vis/sceneHandler/create
  // commands.

  virtual void Initialize(G4int nRow, G4int nColumn);
  // Intialises scanner for window with nRow rows and nColumn columns.

  virtual G4bool Coords(G4int& iRow, G4int& iColumn);
  // Supplies coordinate (iRow,iColumn) and returns false when the
  // sequence has finished, i.e., on the call *after* suplying the
  // last valid coordinate.

  virtual void Draw
  (unsigned char red, unsigned char green, unsigned char blue,
   G4RayTracer*);
  // Draw coloured square at current position.

protected:
  G4String theGSName, theGSNickname;
  G4int theNRow, theNColumn, theStep, theIRow, theIColumn;
};

#endif

#endif

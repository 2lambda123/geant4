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
// $Id: G4Polyline.hh,v 1.7 2001-07-11 10:01:03 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  July 1995

// Class Description:
// A set of line segments defined with a set of vertices.
// G4Polyline is used for visualizing trajectories, steps, coordinate axes,
// etc.
// Class Description - End:


#ifndef G4POLYLINE_HH
#define G4POLYLINE_HH

#include "G4VVisPrim.hh"
#include "G4Point3DList.hh"

class G4Polyline: public G4VVisPrim, public G4Point3DList {
  friend G4std::ostream& operator << (G4std::ostream& os, const G4Polyline& line);

public: // With description

  G4Polyline ();
  G4Polyline (const G4VVisPrim& prim);
  virtual ~G4Polyline ();
  virtual G4Visible&  operator = (const G4Visible& right);
  virtual G4VVisPrim& operator = (const G4VVisPrim& right);
  virtual G4Polyline& operator = (const G4Polyline& right);
};

#endif

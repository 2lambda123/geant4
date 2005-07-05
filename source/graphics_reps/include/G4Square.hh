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
// $Id: G4Square.hh,v 1.6 2005-07-05 14:04:02 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  17/11/96.

// Class Description:
// G4Square is a kind of 3D-position marker. 
// Its shape is 2-dimensional right square.
// It inherits G4VMarker.  See G4VMarker.hh for more details.
// Class Description - End:

#ifndef G4SQUARE_HH
#define G4SQUARE_HH

#include "G4VMarker.hh"

class G4Square: public G4VMarker {

public: // With description

  G4Square ();
  G4Square (const G4Point3D& pos);
  G4Square (const G4VMarker& marker);
  ~G4Square ();

};

#include "G4Square.icc"

#endif

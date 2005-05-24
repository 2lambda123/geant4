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
// $Id: G4Visible.cc,v 1.10 2005-05-24 08:44:25 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  30th October 1996
// Base class for all things visible, i.e., which have Vis Attributes.

#include "G4Visible.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"

G4Visible::~G4Visible () {}

G4Visible& G4Visible::operator = (const G4Visible& right) {
  if (&right == this) return *this;
  fpVisAttributes = right.fpVisAttributes;
  return *this;
}

G4bool G4Visible::operator == (const G4Visible& right) const{
  return fpVisAttributes == right.fpVisAttributes;
}

void G4Visible::SetVisAttributes (const G4VisAttributes& VA) {
  fpVisAttributes = new G4VisAttributes(VA);
  static G4bool firstCall = true;
  if (firstCall) {
    firstCall = false;
    G4cout <<
"\n*******************************************************************************" 
"\nWARNING: DEPRECATED method G4Visible::SetVisAttributes(const G4VisAttributes&)"
"\n       has been invoked.  Please use SetVisAttributes(const G4VisAttributes*)"
"\n       instead, i.e., provide a pointer to a G4VisAttributes object.  Only the"
"\n       pointer is stored, so the G4VisAttributes object to which it refers must"
"\n       have a life long enough to satisfy all uses of the G4Visible object."
"\n       E.g., if the G4Visible object is created on the heap (using `new')"
"\n       then the associated G4VisAttributes object should normally also be"
"\n       created on the heap and managed in the same way."
"\n       (This message is printed only once.  Please check all occurances of"
"\n       your use of SetVisAttributes.)"
"\n*******************************************************************************\n"
 	   << G4endl;
  }
}

std::ostream& operator << (std::ostream& os, const G4Visible& v) {
  if (v.fpVisAttributes) return os << *(v.fpVisAttributes);
  else return os << "No Visualization Attributes";
}

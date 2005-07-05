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
// $Id: G4Text.hh,v 1.8 2005-07-05 14:04:02 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  17/11/96.

// Class Description:
// Text, i.e., a character string, is used to visualize various kinds of 
// description, particle name, energy, coordinate names etc. Text is 
// described by the class G4Text. The following constructors are supported: 
// 
//      //----- Constructors of G4Text
//      G4Text (const G4String& text);
//      G4Text (const G4String& text, const G4Point3D& pos);
// 
// where the argument text is the text (string) to be visualized, and pos 
// is the 3D position at which the text is visualized.  Note that class 
// G4Text inherits G4VMarker.  Size of text is recognized as "font size", 
// i.e., height of the text. All the access functions defined for class 
// G4VMarker are available. In addition, the following access functions 
// are available, too: 
// 
//      //----- Set functions of G4Text
//      void G4Text::SetText ( const G4String& text ) ;
//      void G4Text::SetOffset ( double dx, double dy ) ;
// 
//      //----- Get functions of G4Text
//      G4String G4Text::GetText () const;
//      G4double G4Text::GetXOffset () const;
//      G4double G4Text::GetYOffset () const;
// 
// Method SetText() defines text to be visualized, and GetText() returns 
// the defined text. Method SetOffset() defines x (horizontal) and 
// y (vertical) offsets in the screen coordinates. By default, both offsets 
// are zero, and the text starts from the 3D position given to the 
// constructor or to the method G4VMarker:SetPosition(). Offsets should be 
// given with the same units as the one adopted for the size, i.e., 
// world-size or screen-size units. 
// Class Description - End:


#ifndef G4TEXT_HH
#define G4TEXT_HH

#include "G4VMarker.hh"
#include "globals.hh"

class G4Text: public G4VMarker {

public: // With description

  enum Layout {left, centre, right};
  G4Text (const G4String& text);
  G4Text (const G4String& text, const G4Point3D& pos);
  G4Text (const G4VMarker& marker);
  ~G4Text ();

  G4String GetText   () const;
  Layout   GetLayout () const;

  G4double GetXOffset () const ;
  G4double GetYOffset () const ;

  void SetText   (const G4String& text);
  void SetLayout (Layout layout);

  void SetOffset ( double dx,  double dy ) ;   

private:
  G4String fText;
  Layout   fLayout;
  G4double fXOffset, fYOffset;
};

#include "G4Text.icc"

#endif

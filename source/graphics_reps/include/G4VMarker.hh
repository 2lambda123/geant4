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
// $Id: G4VMarker.hh,v 1.7 2001-07-11 10:01:03 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// G4VMarker - base class for markers - circles, squares, etc.
// John Allison  17/11/96.

// Class Description:
// G4VMarkers are 2-dimensional G4VVisPrims with the special
// properties (a) of always facing the camera and (b) of having the
// possibility of a size defined in screen units.  The convention is
// that if a world size is not specified, then the marker will be
// drawn to the given screen size or paper size regardless of the
// viewing transformation in effect.
//
// "Size" means "overall size", e.g., diameter of circle, side of
// square, height of text (but diameter and radius access functions
// are defined to avoid ambiguity).
//
// So the user who constructs the marker decides whether it should be
// drawn to a given size in world coordinates by setting the world
// size.  Alternatively, the user can set the screen size (internally,
// the world size is set to zero) and the marker is drawn to its
// screen size.  Finally, the user may decide not to set any size; in
// that case, it is drawn according to the sizes specified in the
// default marker specified in G4ViewParameters.
//
// Also in G4ViewParameters is a "global marker scale" which is a
// factor by which all marker sizes are multiplied before drawing.
//
// Thus the graphics system driver scene handler code might look like:
//
// void G4XXXGraphicsSceneHandler::AddPrimitive (const G4Circle& circle) {
//   G4bool hidden = !(fpView -> GetViewParameters().IsMarkerNotHidden());
//   const G4Colour&     colour = GetColour (circle);  
//                                   // Base class GetColour.
//   G4VMarker::FillStyle style = circle.GetFillStyle();
//   const G4Point3D&    centre = circle.GetPosition();
//   MarkerSizeType sizeType;
//   G4double size = GetMarkerSize (circle, sizeType);
//   switch (sizeType) {
//   default:
//   case screen:
//     // Draw in screen coordinates.
//     // ...
//     break;
//   case world:
//     // Draw in world coordinates.
//     // ...
//     break;
//   }
// }
// Class Description - End:


#ifndef G4VMARKER_HH
#define G4VMARKER_HH

#include "globals.hh"
#include "G4VVisPrim.hh"
#include "G4Point3D.hh"
#include "G4Colour.hh"
#include "G4Color.hh"

class G4VMarker: public G4VVisPrim {

  friend G4std::ostream& operator << (G4std::ostream& os, const G4VMarker& marker);

public: // With description

  enum FillStyle {noFill, hashed, filled};

  //////////////////////////////////////////////////////
  // Constructors...
  G4VMarker ();
  G4VMarker (const G4VMarker& marker);
  G4VMarker (const G4Point3D& pos);

  //////////////////////////////////////////////////////
  // Destructor...
  virtual ~G4VMarker ();

  //////////////////////////////////////////////////////
  // Assignment...
  virtual G4Visible&  operator = (const G4Visible& right);
  virtual G4VVisPrim& operator = (const G4VVisPrim& right);
  virtual G4VMarker&  operator = (const G4VMarker& right);

  //////////////////////////////////////////////////////
  // Logical...
  G4bool operator != (const G4VMarker&) const;

  /////////////////////////////////////////////////////
  // Get functions...
  G4Point3D GetPosition       () const;
  G4double  GetWorldSize      () const;
  G4double  GetWorldDiameter  () const;
  G4double  GetWorldRadius    () const;
  G4double  GetScreenSize     () const;
  G4double  GetScreenDiameter () const;
  G4double  GetScreenRadius   () const;
  FillStyle GetFillStyle      () const;

  /////////////////////////////////////////////////////
  // Set functions...
  void SetPosition       (const G4Point3D&);
  void SetWorldSize      (G4double);
  void SetWorldDiameter  (G4double);
  void SetWorldRadius    (G4double);
  void SetScreenSize     (G4double);
  void SetScreenDiameter (G4double);
  void SetScreenRadius   (G4double);
  void SetFillStyle      (FillStyle);

  // Access functions to the string for user custimizable information
  virtual   const G4String&  GetInfo() const;
  virtual   void             SetInfo( const G4String& info );

private:
  G4Point3D fPosition;
  G4double  fWorldSize;   // Default 0. means use screen size.
  G4double  fScreenSize;  // Default 0. means use global default.
  FillStyle fFillStyle;

  // String for user customizable information
  G4String  fInfo ;

};

#include "G4VMarker.icc"

#endif

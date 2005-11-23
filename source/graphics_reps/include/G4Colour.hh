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
// $Id: G4Colour.hh,v 1.12 2005-11-23 22:10:34 tinslay Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison 20th October 1996

// Class Description:
// Class G4Colour has 4 fields, which represent the RGBA (red, green, blue, 
// and alpha) components of colour. Each component takes a value between 
// 0 and 1. If an irrelevant value, i.e., a value less than 0 or greater 
// than 1, is given as an argument of the constructor, such a value is 
// automatically clipped to 0 or 1. Alpha is opacity (1 = opaque).
// 
// A G4Colour object is instantiated by giving red, green, and blue 
// components to its constructor, i.e., 
// 
//      G4Colour::G4Colour ( G4double r = 1.0, 
//                           G4double g = 1.0, 
//                           G4double b = 1.0, 
//                           G4double a = 1.0);
//                                   // 0<=red, green, blue <= 1.0 
// 
// The default value of each component is 1.0. That is to say, the default 
// colour is "white".  For example, colours which are often used can be 
// instantiated as follows: 
// 
//      G4Colour  white   ()              ;  // white
//      G4Colour  white   (1.0, 1.0, 1.0) ;  // white
//      G4Colour  gray    (0.5, 0.5, 0.5) ;  // gray
//      G4Colour  black   (0.0, 0.0, 0.0) ;  // black
//      G4Colour  red     (1.0, 0.0, 0.0) ;  // red
//      G4Colour  green   (0.0, 1.0, 0.0) ;  // green
//      G4Colour  blue    (0.0, 0.0, 1.0) ;  // blue
//      G4Colour  cyan    (0.0, 1.0, 1.0) ;  // cyan
//      G4Colour  magenta (1.0, 0.0, 1.0) ;  // magenta 
//      G4Colour  yellow  (1.0, 1.0, 0.0) ;  // yellow
// 
// For convenience, static member functions are also defined for the above colours. 
//
// After instantiation of a G4Colour object, you can access to its components 
// with the following access functions: 
// 
//      G4double G4Colour::GetRed   () const ; // Get the red   component. 
//      G4double G4Colour::GetGreen () const ; // Get the green component.
//      G4double G4Colour::GetBlue  () const ; // Get the blue  component.
//
// Class Description - End:

#ifndef G4COLOUR_HH
#define G4COLOUR_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <iostream>
#include <map>

using std::map;

class G4Colour {
  friend std::ostream& operator << (std::ostream& os, const G4Colour& c);

public: // With description

  G4Colour (G4double r = 1., G4double g = 1., G4double b = 1.,
	    G4double a = 1.);
  G4Colour (G4ThreeVector);
  // Converts the components of the 3-vector into red, green, blue.
  // The opacity, alpha = 1.
  operator G4ThreeVector();
  // Converts red, green, blue into the components of a 3-vector.
  G4bool operator != (const G4Colour& c) const;
  G4double GetRed   () const;
  G4double GetGreen () const;
  G4double GetBlue  () const;
  G4double GetAlpha () const;  // alpha = opacity = 1. - transparency.

  static const G4Colour& White();
  static const G4Colour& Gray();
  static const G4Colour& Grey();
  static const G4Colour& Black();
  static const G4Colour& Red();
  static const G4Colour& Green();
  static const G4Colour& Blue(); 
  static const G4Colour& Cyan();
  static const G4Colour& Magenta();
  static const G4Colour& Yellow();

  static void AddToMap(const G4String& key, const G4Colour& colour);
  // Add user defined colour to colour map with given key. Standard
  // colours are added to map by default.

  static bool GetColour(const G4String& key, G4Colour& result);
  // Get colour for given key. Returns false if key doesn't exist 
  // in colour map, leaving result unchanged. Colour map
  // is not sensitive to key case.
  
private:
  G4double red, green, blue, alpha;

  static const G4Colour fWhite;
  static const G4Colour fGray; 
  static const G4Colour fGrey; 
  static const G4Colour fBlack;
  static const G4Colour fRed;  
  static const G4Colour fGreen;
  static const G4Colour fBlue; 
  static const G4Colour fCyan; 
  static const G4Colour fMagenta;
  static const G4Colour fYellow; 
  
  static map<G4String, G4Colour> fColourMap;
  static bool fInitColourMap;

  static void InitialiseColourMap();
    
};

inline G4double G4Colour::GetRed   () const {return red;}
inline G4double G4Colour::GetGreen () const {return green;}
inline G4double G4Colour::GetBlue  () const {return blue;}
inline G4double G4Colour::GetAlpha () const {return alpha;}

#endif

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// Class description:
//
// History:
// - Created.                                  Zoltan Torzsok, November 2007
// -------------------------------------------------------------------------

#ifndef _G4GDMLWRITESOLIDS_INCLUDED_
#define _G4GDMLWRITESOLIDS_INCLUDED_

#include "G4BooleanSolid.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Ellipsoid.hh"
#include "G4EllipticalTube.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Hype.hh"
#include "G4Orb.hh"
#include "G4Para.hh"
#include "G4IntersectionSolid.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4ReflectedSolid.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4TessellatedSolid.hh"
#include "G4Tet.hh"
#include "G4Torus.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4TwistedBox.hh"
#include "G4UnionSolid.hh"

#include "G4SolidStore.hh"

#include "G4GDMLWriteMaterials.hh"

class G4GDMLWriteSolids : public G4GDMLWriteMaterials {
private:
   void booleanWrite(xercesc::DOMElement*,const G4BooleanSolid* const);
   void boxWrite(xercesc::DOMElement*,const G4Box* const);
   void coneWrite(xercesc::DOMElement*,const G4Cons* const);
   void ellipsoidWrite(xercesc::DOMElement*,const G4Ellipsoid* const);
   void eltubeWrite(xercesc::DOMElement*,const G4EllipticalTube* const);
   void xtruWrite(xercesc::DOMElement*,const G4ExtrudedSolid* const);
   void hypeWrite(xercesc::DOMElement*,const G4Hype* const);
   void orbWrite(xercesc::DOMElement*,const G4Orb* const);
   void paraWrite(xercesc::DOMElement*,const G4Para* const);
   void polyconeWrite(xercesc::DOMElement*,const G4Polycone* const);
   void polyhedraWrite(xercesc::DOMElement*,const G4Polyhedra* const);
   void sphereWrite(xercesc::DOMElement*,const G4Sphere* const);
   void tessellatedWrite(xercesc::DOMElement*,const G4TessellatedSolid* const);
   void tetWrite(xercesc::DOMElement*,const G4Tet* const);
   void torusWrite(xercesc::DOMElement*,const G4Torus* const);
   void trapWrite(xercesc::DOMElement*,const G4Trap* const);
   void trdWrite(xercesc::DOMElement*,const G4Trd* const);
   void tubeWrite(xercesc::DOMElement*,const G4Tubs* const);
   void twistedboxWrite(xercesc::DOMElement*,const G4TwistedBox* const);
   void zplaneWrite(xercesc::DOMElement*,const G4double&,const G4double&,const G4double&);
   void solidsWrite(xercesc::DOMElement*);
};

#endif

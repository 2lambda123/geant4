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
// $Id: G4CircleCreator.cc,v 1.6 2003-03-28 15:47:42 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4CircleCreator
//
// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
//
// History:
//   18-Nov-1999: First step of re-engineering - G.Cosmo
// ----------------------------------------------------------------------

#include "G4CircleCreator.hh"
#include "G4GeometryTable.hh"
#include "G4CircularCurve.hh"


G4CircleCreator G4CircleCreator::csc;

G4CircleCreator::G4CircleCreator()
{
  G4GeometryTable::RegisterObject(this);
}

G4CircleCreator::~G4CircleCreator() {}

G4CircleCreator G4CircleCreator::GetInstance()
{
  return csc;
}

void G4CircleCreator::CreateG4Geometry(STEPentity& Ent)
{
  G4Axis2Placement3D place;

  G4double radius;
  
  Ent.ResetAttributes();
  STEPattribute* Attr = Ent.NextAttribute();
  while(Attr->NonRefType() == STRING_TYPE ||
	Attr->NonRefType() == sdaiSTRING )
    Attr = Ent.NextAttribute();
  
  // Get the placement
  // SelectNode* SelectN = (SelectNode*)Attr->ptr.sh;
  // SdaiSelect* Select = (SdaiSelect*)SelectN;
  SdaiSelect* Select = Attr->ptr.sh;
  SdaiAxis2_placement* Place = (SdaiAxis2_placement*)Select;
  STEPentity* TmpEnt = Place->operator SdaiAxis2_placement_3dH();
  void * tmp =G4GeometryTable::CreateObject(*TmpEnt);
 
  if (tmp)
    place = *(G4Axis2Placement3D*)tmp;
  else
    G4cerr << "WARNING - G4CircleCreator::CreateG4Geometry" << G4endl
           << "\tUnexpected NULL axis placement (G4Axis2Placement3D) detected." << G4endl;
  
  Attr = Ent.NextAttribute();	
  radius = *Attr->ptr.r;

  G4CircularCurve* circle = new G4CircularCurve();
  circle->Init(place , radius);
  createdObject = circle;
}

void G4CircleCreator::CreateSTEPGeometry(void*)
{
}

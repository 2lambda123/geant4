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
// $Id: G4CylindricalSurfaceCreator.cc,v 1.5 2002-11-21 16:49:48 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4CylindricalSurfaceCreator
//
// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
//
// History:
//   18-Nov-1999: First step of re-engineering - G.Cosmo
// ----------------------------------------------------------------------

#include "G4CylindricalSurfaceCreator.hh"
#include "G4GeometryTable.hh"
#include "G4FCylindricalSurface.hh"
#include "G4CylindricalSurface.hh"

G4CylindricalSurfaceCreator G4CylindricalSurfaceCreator::csc;

G4CylindricalSurfaceCreator::G4CylindricalSurfaceCreator()
{
  G4GeometryTable::RegisterObject(this);
}

G4CylindricalSurfaceCreator::~G4CylindricalSurfaceCreator() {}

G4CylindricalSurfaceCreator G4CylindricalSurfaceCreator::GetInstance()
{
  return csc;
}

void G4CylindricalSurfaceCreator::CreateG4Geometry(STEPentity& Ent)
{
  // Made by L. Broglia

  STEPattribute *Attr;
  G4Axis2Placement3D* place; 
  G4double radius;

  G4String attrName("position");
  Attr = GetNamedAttribute(attrName, Ent);

  // Get placement
  STEPentity* TmpEnt = *Attr->ptr.c; // ptr.c --> ENTITY_TYPE
  void *tmp = G4GeometryTable::CreateObject(*TmpEnt);
  place = (G4Axis2Placement3D*)tmp;
  
  // Get radius
  G4String attrNameR("radius");
  Attr = GetNamedAttribute(attrNameR, Ent);
  radius = *Attr->ptr.r;
   
  G4CylindricalSurface* aG4cylinder = 0;
  if (place)
    aG4cylinder = new G4CylindricalSurface( (*place).GetLocation() ,
			                    (*place).GetAxis()     ,
			                    radius                );
  else
    G4cerr << "WARNING - G4CylindricalSurfaceCreator::CreateG4Geometry" << G4endl
           << "\tUnexpected NULL axis placement (G4Axis2Placement3D) !" << G4endl
	   << "\tCylindrical Surface NOT created." << G4endl;

  createdObject = aG4cylinder;
}

void G4CylindricalSurfaceCreator::CreateSTEPGeometry(void* G4obj)
{
  G4FCylindricalSurface* fCyl = (G4FCylindricalSurface*)G4obj;
  SdaiCylindrical_surface* srf= new SdaiCylindrical_surface();
  
  // Get placement
  G4String placementName("Axis2Placement3d");
  void * tmp =G4GeometryTable::CreateSTEPObject(&fCyl, placementName);
  SdaiAxis2_placement_3d *place = (SdaiAxis2_placement_3d*)tmp;
  srf->position_(place);
  
  // radius
  srf->radius_(fCyl->GetRadius());

  // Set STEP info
  srf->SetFileId(GetNextId());
  srf->name_("");
  
  // Write out object 
  srf->STEPwrite(G4cout);

  createdObject = srf;
}

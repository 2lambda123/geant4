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
// $Id: G4GDMLWriteStructure.hh,v 1.32 2008-07-03 08:38:29 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4GDMLWriteStructure
//
// Class description:
//
// GDML class for export of structures.

// History:
// - Created.                                  Zoltan Torzsok, November 2007
// -------------------------------------------------------------------------

#ifndef _G4GDMLWRITESTRUCTURE_INCLUDED_
#define _G4GDMLWRITESTRUCTURE_INCLUDED_

#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4PVDivision.hh"
#include "G4PVReplica.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ReflectedSolid.hh"
#include "G4Transform3D.hh"

#include "G4GDMLWriteParamvol.hh"

class G4GDMLWriteStructure : public G4GDMLWriteParamvol {
private:
// GC - 2
// std::map<const G4LogicalVolume*,G4Transform3D> volumeMap;

   xercesc::DOMElement* structureElement;

   static const int maxReflections = 8; // Constant for limiting the number displacements/reflections applied to a single solid

   void divisionvolWrite(xercesc::DOMElement*,const G4PVDivision* const);
   void physvolWrite(xercesc::DOMElement*,const G4VPhysicalVolume* const topVol,
                                          const G4Transform3D& transform,
                                          const G4String& moduleName);
   void replicavolWrite(xercesc::DOMElement*,const G4VPhysicalVolume* const);
   void structureWrite(xercesc::DOMElement*);
   G4Transform3D TraverseVolumeTree(const G4LogicalVolume* const topVol,const G4int depth);
};

#endif

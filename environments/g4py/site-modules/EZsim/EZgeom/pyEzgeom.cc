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
// $Id: pyEzgeom.cc,v 1.2 2006-06-04 21:36:34 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   pyEZgeom.cc
//
//   [EZgeom]
//   a site-module of Geant4Py
//
//   An easy way to build geometry
//
//                                         2005 Q
// ====================================================================
#include <boost/python.hpp>
#include "EzDetectorConstruction.hh"
#include "G4EzWorld.hh"
#include "G4EzVolume.hh"
#include "G4RunManager.hh"
#include "G4VSensitiveDetector.hh"

using namespace boost::python;

// ====================================================================
// thin wrappers
// ====================================================================
namespace pyEZgeom {

// methods in global namespace
void Construct()
{
  G4RunManager* runMgr= G4RunManager::GetRunManager();
  runMgr-> SetUserInitialization(new EzDetectorConstruction);
}

void ResetWorld(G4double dx, G4double dy, G4double dz)
{
  G4EzWorld::Reset(dx, dy, dz);
}

void ResizeWorld(G4double dx, G4double dy, G4double dz)
{
  G4EzWorld::Resize(dx, dy, dz);
}


void SetWorldMaterial(G4Material* amaterial)
{
  G4EzWorld::SetMaterial(amaterial);
}


void SetWorldVisibility(G4bool qvis)
{
  G4EzWorld::SetVisibility(qvis);
}


// CreateTubeVolume
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_CreateTubeVolume, 
				       CreateTubeVolume, 4, 6);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_CreateConeVolume, 
				       CreateConeVolume, 6, 8);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_CreateSphereVolume, 
				       CreateSphereVolume, 3, 7);

// PlaceIt
G4VPhysicalVolume*(G4EzVolume::*f1_PlaceIt)
  (const G4ThreeVector&, G4int, G4EzVolume*) = &G4EzVolume::PlaceIt;

G4VPhysicalVolume*(G4EzVolume::*f2_PlaceIt)
  (const G4Transform3D&, G4int, G4EzVolume*) = &G4EzVolume::PlaceIt;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_PlaceIt, PlaceIt, 1, 3);

// ReplicateIt
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_ReplicateIt, ReplicateIt, 4, 5);

// SetColor
void (G4EzVolume::*f1_SetColor)(const G4Color&) = &G4EzVolume::SetColor;
void (G4EzVolume::*f2_SetColor)(G4double, G4double, G4double) 
  = &G4EzVolume::SetColor;

};

using namespace pyEZgeom;

// ====================================================================
//   Expose to Python
// ====================================================================

BOOST_PYTHON_MODULE(EZgeom) {

  class_<G4EzVolume>("G4EzVolume", "an easy way of geometry configuration")
    .def(init<const G4String&>())
    // ---
    .def("CreateBoxVolume",     &G4EzVolume::CreateBoxVolume)
    .def("CreateTubeVolume",    &G4EzVolume::CreateTubeVolume, 
	                        f_CreateTubeVolume())
    .def("CreateConeVolume",    &G4EzVolume::CreateConeVolume, 
	                        f_CreateConeVolume())
    .def("CreateShpereVolume",  &G4EzVolume::CreateSphereVolume, 
	                        f_CreateSphereVolume())
    .def("CreateOrbVolume",     &G4EzVolume::CreateOrbVolume)
    // ---
    .def("SetSold",             &G4EzVolume::SetSolid)
    .def("GetSold",             &G4EzVolume::GetSolid,
         return_value_policy<reference_existing_object>())
    .def("SetMaterial",         &G4EzVolume::SetMaterial)
    .def("GetMaterial",         &G4EzVolume::GetMaterial,
         return_value_policy<reference_existing_object>())
    // ---
    .def("PlaceIt", f1_PlaceIt, 
	 f_PlaceIt()[return_value_policy<reference_existing_object>()])
    .def("PlaceIt", f2_PlaceIt, 
	 f_PlaceIt()[return_value_policy<reference_existing_object>()])
    .def("ReplicateIt", &G4EzVolume::ReplicateIt, 
	 f_ReplicateIt()[return_value_policy<reference_existing_object>()])
    .def("VoxelizeIt",          &G4EzVolume::VoxelizeIt)
    // ---
    .def("SetSensitiveDetector", &G4EzVolume::SetSensitiveDetector)
    // ---
    .def("SetColor",             f1_SetColor)
    .def("SetColor",             f2_SetColor)
    .def("SetVisibility",        &G4EzVolume::SetVisibility)
    ;

  // -------------------------------------------------------------------
  def("Construct",               Construct);
  def("ResetWorld",              ResetWorld);
  def("ResizeWorld",             ResizeWorld);
  def("SetWorldMaterial",        SetWorldMaterial);
  def("SetWorldVisibility",      SetWorldVisibility);

}

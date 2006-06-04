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
// $Id: pyG4LogicalVolume.cc,v 1.3 2006-06-04 21:34:28 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   pyG4LogicalVolume.cc
//
//                                         2005 Q
// ====================================================================
#include <boost/python.hpp>
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4FieldManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4UserLimits.hh"
#include "G4SmartVoxelHeader.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4FastSimulationManager.hh"
#include "G4VisAttributes.hh"
#include "pyG4Version.hh"

using namespace boost::python;

// ====================================================================
// thin wrappers
// ====================================================================
namespace pyG4LogicalVolume {

void(G4LogicalVolume::*f1_SetVisAttributes)(const G4VisAttributes*) 
  = &G4LogicalVolume::SetVisAttributes;

void(G4LogicalVolume::*f2_SetVisAttributes)(const G4VisAttributes&) 
  = &G4LogicalVolume::SetVisAttributes;

#if G4VERSION_NUMBER <= 701
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_GetMass, GetMass, 0, 2);
#elif G4VERSION_NUMBER >=710
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_GetMass, GetMass, 0, 3);
#endif

};

using namespace pyG4LogicalVolume;

// ====================================================================
// module definition
// ====================================================================
void export_G4LogicalVolume()
{
  class_<G4LogicalVolume, G4LogicalVolume*, boost::noncopyable>
    ("G4LogicalVolume", "logical volume class", no_init)
    // constructors
    .def(init<G4VSolid*, G4Material*, const G4String& >())
    .def(init<G4VSolid*, G4Material*, const G4String&,
         G4FieldManager* >())
    .def(init<G4VSolid*, G4Material*, const G4String&,
         G4FieldManager*, G4VSensitiveDetector* >())
    .def(init<G4VSolid*, G4Material*, const G4String&,
         G4FieldManager*, G4VSensitiveDetector*,
         G4UserLimits* >())
    .def(init<G4VSolid*, G4Material*, const G4String&,
         G4FieldManager*, G4VSensitiveDetector*,
         G4UserLimits*, G4bool >())
    // ---
    .def("GetName",         &G4LogicalVolume::GetName)
    .def("SetName",         &G4LogicalVolume::SetName)
    // ---
    .def("GetNoDaughters",  &G4LogicalVolume::GetNoDaughters)
    .def("GetDaughter",     &G4LogicalVolume::GetDaughter,
	 return_internal_reference<>())
    .def("AddDaughter",     &G4LogicalVolume::AddDaughter)
    .def("IsDaughter",      &G4LogicalVolume::IsDaughter)
    .def("IsAncestor",      &G4LogicalVolume::IsAncestor)
    .def("RemoveDaughter",  &G4LogicalVolume::RemoveDaughter)
    .def("ClearDaughters",  &G4LogicalVolume::ClearDaughters)
    .def("TotalVolumeEntities", &G4LogicalVolume::TotalVolumeEntities)
    // ----
    .def("GetSolid",        &G4LogicalVolume::GetSolid,
	 return_internal_reference<>())
    .def("SetSolid",        &G4LogicalVolume::SetSolid)
    .def("GetMaterial",     &G4LogicalVolume::GetMaterial,
	 return_internal_reference<>())
    .def("SetMaterial",     &G4LogicalVolume::SetMaterial)
    .def("UpdateMaterial",  &G4LogicalVolume::UpdateMaterial)
    // ---
    .def("GetMass",         &G4LogicalVolume::GetMass, f_GetMass())
    .def("GetFieldManager", &G4LogicalVolume::GetFieldManager,
	 return_internal_reference<>())
    .def("SetFieldManager", &G4LogicalVolume::SetFieldManager)
    .def("GetSensitiveDetector", &G4LogicalVolume::GetSensitiveDetector,
	 return_internal_reference<>())
    .def("GetUserLimits",   &G4LogicalVolume::GetUserLimits,
	 return_internal_reference<>())
    .def("SetUserLimits",   &G4LogicalVolume::SetUserLimits)
    // ---
    .def("GetVoxelHeader",  &G4LogicalVolume::GetVoxelHeader,
	 return_internal_reference<>())
    .def("SetVoxelHeader",  &G4LogicalVolume::SetVoxelHeader)
    .def("GetSmartless",    &G4LogicalVolume::GetSmartless)
    .def("SetSmartless",    &G4LogicalVolume::SetSmartless)
    .def("IsToOptimise",    &G4LogicalVolume::IsToOptimise)
    .def("SetOptimisation", &G4LogicalVolume::SetOptimisation)
    // ---
    .def("IsRootRegion",    &G4LogicalVolume::IsRootRegion)
    .def("SetRegionRootFlag", &G4LogicalVolume::SetRegionRootFlag)
    .def("IsRegion",        &G4LogicalVolume::IsRegion)
    .def("SetRegion",       &G4LogicalVolume::SetRegion)
    .def("GetRegion",       &G4LogicalVolume::GetRegion,
	 return_internal_reference<>())
    .def("PropagateRegion", &G4LogicalVolume::PropagateRegion)
    .def("GetMaterialCutsCouple", &G4LogicalVolume::GetMaterialCutsCouple,
	 return_internal_reference<>())
    .def("SetMaterialCutsCouple", &G4LogicalVolume::SetMaterialCutsCouple)
    // ---
    .def("GetVisAttributes", &G4LogicalVolume::GetVisAttributes,
	 return_internal_reference<>())
    .def("SetVisAttributes", f1_SetVisAttributes)
    .def("SetVisAttributes", f2_SetVisAttributes)
    // ---
#if G4VERSION_NUMBER >= 700 && G4VERSION_NUMBER <= 711 
    .def("BecomeEnvelopeForFastSimulation", 
	 &G4LogicalVolume::BecomeEnvelopeForFastSimulation)
    .def("ClearEnvelopeForFastSimulation", 
	 &G4LogicalVolume::ClearEnvelopeForFastSimulation)
#endif
    .def("GetFastSimulationManager", 
	 &G4LogicalVolume::GetFastSimulationManager,
	 return_internal_reference<>())
    // ---
    .def("SetBiasWeight",  &G4LogicalVolume::SetBiasWeight)
    .def("GetBiasWeight",  &G4LogicalVolume::GetBiasWeight)
    ;
}

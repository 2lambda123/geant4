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
// $Id: pyG4Tubs.cc,v 1.4 2006-06-29 15:32:45 gunter Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   pyG4Tubs.cc
//
//                                         2005 Q
// ====================================================================
#include <boost/python.hpp>
#include "G4Tubs.hh"

using namespace boost::python;

// ====================================================================
// module definition
// ====================================================================
void export_G4Tubs()
{
  class_<G4Tubs, G4Tubs*, bases<G4VSolid> >
    ("G4Tubs", "Tube solid class", no_init)
    // constructors
    .def(init<const G4String&, G4double, G4double, G4double,
	 G4double, G4double>())
    // ---
    .def("GetInnerRadius",   &G4Tubs::GetInnerRadius)
    .def("GetOuterRadius",   &G4Tubs::GetOuterRadius)
    .def("GetZHalfLength",   &G4Tubs::GetZHalfLength)
    .def("GetStartPhiAngle", &G4Tubs::GetStartPhiAngle)
    .def("GetDeltaPhiAngle", &G4Tubs::GetDeltaPhiAngle)
    .def("SetInnerRadius",   &G4Tubs::SetInnerRadius)
    .def("SetOuterRadius",   &G4Tubs::SetOuterRadius)
    .def("SetZHalfLength",   &G4Tubs::SetZHalfLength)
    .def("SetStartPhiAngle", &G4Tubs::SetStartPhiAngle)
    .def("SetDeltaPhiAngle", &G4Tubs::SetDeltaPhiAngle)
    .def("GetRMin",          &G4Tubs::GetRMin)
    .def("GetRMax",          &G4Tubs::GetRMax)
    .def("GetDz",            &G4Tubs::GetDz)
    .def("GetSPhi",          &G4Tubs::GetSPhi)
    .def("GetDPhi",          &G4Tubs::GetDPhi)
    .def("GetCubicVolume",   &G4Tubs::GetCubicVolume)
    // operators
    .def(self_ns::str(self))
    ;
}


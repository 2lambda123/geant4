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
// $Id: pyG4GDMLParser.cc,v 1.2 2008-12-03 06:54:39 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   pyG4GDMLParser.cc
//
//                                         2007 Q
// ====================================================================
#ifdef ENABLE_GDML

#include <boost/python.hpp>
#include "G4GDMLParser.hh"

using namespace boost::python;

// ====================================================================
// thin wrappers
// ====================================================================
namespace pyG4GDMLParser {

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_Read, Read, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_ReadModule, ReadModule, 1, 2);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_Write, Write, 1, 4);

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(f_GetWorldVolume, 
                                       GetWorldVolume, 0, 1);
}

using namespace pyG4GDMLParser;

// ====================================================================
// module definition
// ====================================================================
void export_G4GDMLParser()
{
  class_<G4GDMLParser, boost::noncopyable>
    ("G4GDMLParser", "GDML parser")
    // ---
    .def("Read",             &G4GDMLParser::Read,       f_Read())
    .def("ReadModule",       &G4GDMLParser::ReadModule, f_ReadModule())
    .def("Write",            &G4GDMLParser::Write,      f_Write())
    .def("ParseST",          &G4GDMLParser::ParseST,
         return_value_policy<reference_existing_object>())
    .def("GetWorldVolume",   &G4GDMLParser::GetWorldVolume,
         f_GetWorldVolume()
         [return_value_policy<reference_existing_object>()])
    ;
}

#endif

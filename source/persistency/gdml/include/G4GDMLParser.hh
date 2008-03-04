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
// History:
// - Created.                                  Zoltan Torzsok, November 2007
// -------------------------------------------------------------------------

#ifndef _G4GDMLPARSER_INCLUDED_
#define _G4GDMLPARSER_INCLUDED_

#include "G4GDMLReadStructure.hh"
#include "G4GDMLWriteStructure.hh"

class G4GDMLParser {
   G4GDMLReadStructure reader;
   G4GDMLWriteStructure writer;
public:
   G4GDMLParser() { xercesc::XMLPlatformUtils::Initialize(); }
   ~G4GDMLParser() { xercesc::XMLPlatformUtils::Terminate(); }

   void Read(const G4String& fname) { reader.Read(fname,false); /* false: not an external file! */ }
   void Write(const G4String& fname,const G4LogicalVolume* const logvol) { writer.Write(fname,logvol); }

   G4double getConstant(const G4String& name) { return reader.getConstant(name); }
   G4double getVariable(const G4String& name) { return reader.getVariable(name); }
   G4double getQuantity(const G4String& name) { return reader.getQuantity(name); }
   G4ThreeVector getPosition(const G4String& name) { return *reader.getPosition(name); }
   G4ThreeVector getRotation(const G4String& name) { return *reader.getRotation(name); }
   G4ThreeVector getScale(const G4String& name) { return *reader.getScale(name); }
   G4GDMLMatrix getMatrix(const G4String& name) { return *reader.getMatrix(name); }
   G4GDMLAuxListType getVolumeAuxiliaryInformation(const G4String& name) { return reader.getVolumeAuxiliaryInformation(name); }

   G4VPhysicalVolume* getWorldVolume(const G4String& setupName="Default") { 
   
      G4LogicalVolume* volume = reader.getVolume(reader.getSetup(setupName));
      volume->SetVisAttributes(G4VisAttributes::Invisible);
      return new G4PVPlacement(0,G4ThreeVector(),volume,"",0,0,0);
   }
};

#endif

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
// ===========================================================================
// GEANT4 class source file
//
// Class:                G4ExternalDEDXTable
//
// Base class:           G4VIonDEDXTable 
// 
// Author:               Anton Lechner (Anton.Lechner@cern.ch)
//
// First implementation: 29. 02. 2009
//
// Modifications:
//
//
// Class description:
//    Utility class for users to add their own electronic stopping powers
//    for ions. This class is dedicated for use with G4IonParametrisedLossModel
//    of the low-energy electromagnetic package.
//
// Comments:
//
// =========================================================================== 
//

#include "G4ExternalDEDXTable.hh" 
#include "G4PhysicsVector.hh"
#include "G4PhysicsVectorType.hh"
#include "G4LPhysicsFreeVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsFreeVector.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4PhysicsLinearVector.hh"
#include "G4PhysicsLnVector.hh"
#include <fstream>
#include <sstream>
#include <iomanip>


// #########################################################################

G4ExternalDEDXTable::G4ExternalDEDXTable() {

}

// #########################################################################

G4ExternalDEDXTable::~G4ExternalDEDXTable() {

  ClearTable();
}

// #########################################################################

G4bool G4ExternalDEDXTable::IsApplicable(
         G4int atomicNumberIon,  // Atomic number of ion
         G4int atomicNumberElem  // Atomic number of elemental material
				    ) {
  G4bool isApplicable = true; 
  G4IonDEDXKeyElem key = std::make_pair(atomicNumberIon, atomicNumberElem);

  G4IonDEDXMapElem::iterator iter = dedxMapElements.find(key);

  if(iter == dedxMapElements.end()) isApplicable = false; 

  return isApplicable; 
}

// #########################################################################

G4bool G4ExternalDEDXTable::IsApplicable(
         G4int atomicNumberIon,         // Atomic number of ion
         const G4String& matIdentifier  // Name or chemical formula of material
				    ) {
  G4bool isApplicable = true; 
  G4IonDEDXKeyMat key = std::make_pair(atomicNumberIon, matIdentifier);

  G4IonDEDXMapMat::iterator iter = dedxMapMaterials.find(key);

  if(iter == dedxMapMaterials.end()) isApplicable = false; 

  return isApplicable; 
}

// #########################################################################

G4PhysicsVector* G4ExternalDEDXTable::GetPhysicsVector(
         G4int atomicNumberIon,        // Atomic number of ion
         G4int atomicNumberElem        // Atomic number of elemental material
				    ) {

  G4PhysicsVector* physVector = 0;

  G4IonDEDXKeyElem key = std::make_pair(atomicNumberIon, atomicNumberElem);

  G4IonDEDXMapElem::iterator iter = dedxMapElements.find(key);

  if(iter != dedxMapElements.end()) physVector = iter -> second; 

  return physVector; 
}

// #########################################################################

G4PhysicsVector*  G4ExternalDEDXTable::GetPhysicsVector(
         G4int atomicNumberIon,        // Atomic number of ion
         const G4String& matIdentifier // Name or chemical formula of material
				    ) {

  G4PhysicsVector* physVector = 0;

  G4IonDEDXKeyMat key = std::make_pair(atomicNumberIon, matIdentifier);

  G4IonDEDXMapMat::iterator iter = dedxMapMaterials.find(key);

  if(iter != dedxMapMaterials.end()) physVector = iter -> second; 

  return physVector; 
}

// #########################################################################

G4double G4ExternalDEDXTable::GetDEDX(
         G4double kinEnergyPerNucleon, // Kinetic energy per nucleon
         G4int atomicNumberIon,        // Atomic number of ion
         G4int atomicNumberElem        // Atomic number of elemental material
				  ) {
  G4double dedx = 0;

  G4IonDEDXKeyElem key = std::make_pair(atomicNumberIon, atomicNumberElem);

  G4IonDEDXMapElem::iterator iter = dedxMapElements.find(key);

  if( iter != dedxMapElements.end() ) {
     G4PhysicsVector* physicsVector = iter -> second; 

     G4bool b;
     dedx = physicsVector -> GetValue( kinEnergyPerNucleon, b );   
  }

  return dedx; 
}

// #########################################################################

G4double G4ExternalDEDXTable::GetDEDX(
         G4double kinEnergyPerNucleon, // Kinetic energy per nucleon
         G4int atomicNumberIon,        // Atomic number of ion
         const G4String& matIdentifier // Name or chemical formula of material
				  ) {
  G4double dedx = 0;

  G4IonDEDXKeyMat key = std::make_pair(atomicNumberIon, matIdentifier);

  G4IonDEDXMapMat::iterator iter = dedxMapMaterials.find(key);

  if(iter != dedxMapMaterials.end()) {
     G4PhysicsVector* physicsVector = iter -> second; 

     G4bool b;
     dedx = physicsVector -> GetValue( kinEnergyPerNucleon, b );   
  }

  return dedx; 
}

// #########################################################################

G4bool G4ExternalDEDXTable::AddPhysicsVector(
        G4PhysicsVector* physicsVector, // Physics vector
	G4int atomicNumberIon,          // Atomic number of ion
        const G4String& matIdentifier,  // Name of elemental material
        G4int atomicNumberElem          // Atomic number of elemental material
				      ) {

  if(physicsVector == 0) {

#ifdef G4VERBOSE
     G4cerr << "G4IonDEDXTable::AddPhysicsVector() Error: Pointer to vector"
            << " is null-pointer."
            << G4endl;
#endif

     return false;
  }

  if(matIdentifier.empty()) {

#ifdef G4VERBOSE
     G4cerr << "G4IonDEDXTable::AddPhysicsVector() Error: "
            << "Cannot add physics vector. Invalid name."
            << G4endl;
#endif

     return false;
  }

  if(atomicNumberIon <= 2) {

#ifdef G4VERBOSE
     G4cerr << "G4IonDEDXTable::AddPhysicsVector() Error: "
            << "Cannot add physics vector. Illegal atomic number."
            << G4endl;
#endif

     return false;
  }

  if(atomicNumberElem > 0) {

     G4IonDEDXKeyElem key = std::make_pair(atomicNumberIon, atomicNumberElem);

     if(dedxMapElements.count(key) == 1) {

#ifdef G4VERBOSE
        G4cerr << "G4IonDEDXTable::AddPhysicsVector() Error: "
               << "Vector already exists. Remove first before replacing."
               << G4endl;
#endif
        return false;
     }

     dedxMapElements[key] = physicsVector;
  }

  G4IonDEDXKeyMat mkey = std::make_pair(atomicNumberIon, matIdentifier);

  if(dedxMapMaterials.count(mkey) == 1) {

#ifdef G4VERBOSE
     G4cerr << "G4IonDEDXTable::AddPhysicsVector() Error: "
            << "Vector already exists. Remove first before replacing."
            << G4endl;
#endif

     return false;
  }

  dedxMapMaterials[mkey] = physicsVector;

  return true;
}

// #########################################################################

G4bool G4ExternalDEDXTable::RemovePhysicsVector(
	G4int atomicNumberIon,         // Atomic number of ion
        const G4String& matIdentifier  // Name or chemical formula of material
				      ) {

  G4PhysicsVector* physicsVector = 0;

  // Deleting key of physics vector from material map
  G4IonDEDXKeyMat key = std::make_pair(atomicNumberIon, matIdentifier);

  G4IonDEDXMapMat::iterator iter = dedxMapMaterials.find(key);

  if(iter == dedxMapMaterials.end()) {

#ifdef G4VERBOSE
    G4cerr << "G4IonDEDXTable::RemovePhysicsVector() Warning: "
            << "Cannot remove physics vector. Vector not found."
             << G4endl;
#endif

     return false;
  }

  physicsVector = (*iter).second;
  dedxMapMaterials.erase(key);

  // Deleting key of physics vector from elemental material map (if it exists)
  G4IonDEDXMapElem::iterator iter_beg = dedxMapElements.begin();
  G4IonDEDXMapElem::iterator iter_end = dedxMapElements.end();
  
  for(;iter_beg != iter_end; iter_beg++) {

     if( (*iter_beg).second == physicsVector ) {

        dedxMapElements.erase(iter_beg);
     }
  }

  // Deleting physics vector
  delete physicsVector;

  return true;
}

// #########################################################################

G4bool G4ExternalDEDXTable::StorePhysicsTable(
         const G4String& fileName // File name
				    ) {
  G4bool success = true;

  std::ofstream ofilestream;

  ofilestream.open( fileName, std::ios::out );

  if( !ofilestream ) {

#ifdef G4VERBOSE
     G4cerr << "G4ExternalDEDXTable::StorePhysicsVector() " 
            << " Cannot open file "<< fileName 
            << G4endl;
#endif
      
     success = false;
  }   
  else {

     size_t nmbMatTables = dedxMapMaterials.size();

     ofilestream << nmbMatTables << G4endl << G4endl; 

     G4IonDEDXMapMat::iterator iterMat = dedxMapMaterials.begin();
     G4IonDEDXMapMat::iterator iterMat_end = dedxMapMaterials.end();

     for(;iterMat != iterMat_end; iterMat++) {
         G4IonDEDXKeyMat key = iterMat -> first;
         G4PhysicsVector* physicsVector = iterMat -> second; 

         G4int atomicNumberIon = key.first;
         G4String matIdentifier = key.second;

         G4int atomicNumberElem = FindAtomicNumberElement(physicsVector);

         if(physicsVector != 0) {
  	    ofilestream << atomicNumberIon << "  " << matIdentifier;

            if(atomicNumberElem > 0) ofilestream << "  " << atomicNumberElem;

            ofilestream << "  # <Atomic number ion>  <Material name>  ";

            if(atomicNumberElem > 0) ofilestream << "<Atomic number element>";

            ofilestream << G4endl << physicsVector -> GetType() << G4endl;

            physicsVector -> Store(ofilestream, true);

            ofilestream << G4endl;
         }
         else {

#ifdef G4VERBOSE
              G4cerr << "G4ExternalDEDXTable::StorePhysicsVector() " 
                     << " Cannot store physics vector." 
                     << G4endl;
#endif

         }
     }
  }

  ofilestream.close();

  return success; 
}

// #########################################################################

G4bool G4ExternalDEDXTable::RetrievePhysicsTable(
              const G4String& fileName  // File name
				             ) { 

  std::ifstream ifilestream;

  ifilestream.open( fileName, std::ios::in );

  if( !ifilestream ) {

#ifdef G4VERBOSE
     G4cerr << "G4ExternalDEDXTable::RetrievePhysicsVector() " 
            << " Cannot open file "<< fileName 
            << G4endl;
#endif
      
     return false;
  }   

  G4int nmbVectors;

  ifilestream >> nmbVectors;
  
  for(G4int i = 0; i < nmbVectors; i++) {

      G4String line = "";

      while( line.empty() ) {

         getline( ifilestream, line );

         if( ifilestream.fail() ) { 

#ifdef G4VERBOSE  
            G4cerr << "G4ExternalDEDXTable::RetrievePhysicsTable() " 
                   << " File content of " << fileName << " ill-formated." 
                   << G4endl;
#endif          
            ifilestream.close(); 
            return false; 
         }

         size_t pos = line.find_first_of("#");
         line = line.substr(0, pos);
      }

      std::istringstream headerstream( line );     

      G4int atomicNumberIon;
      headerstream >> atomicNumberIon;

      G4String materialName;
      headerstream >> materialName;

      if( headerstream.fail() ) {
 
#ifdef G4VERBOSE  
         G4cerr << "G4ExternalDEDXTable::RetrievePhysicsTable() " 
                << " File content of " << fileName << " ill-formated "
                << " (vector header)." 
                << G4endl;
#endif          
         ifilestream.close();
         return false;
      } 

      G4int atomicNumberMat;
      headerstream >> atomicNumberMat;

      if( headerstream.eof() ) atomicNumberMat = 0; 

      G4int vectorType;

      ifilestream >>  vectorType;
      
      G4PhysicsVector* physicsVector = CreatePhysicsVector(vectorType);

      if(physicsVector == 0) {

#ifdef G4VERBOSE  
         G4cerr << "G4ExternalDEDXTable::RetrievePhysicsTable  "
                << " illegal physics Vector type " << vectorType
                << " in  " << fileName 
                << G4endl;
#endif          
         ifilestream.close();
         return false;
      }

      if( !physicsVector -> Retrieve(ifilestream, true) ) {
 
#ifdef G4VERBOSE  
         G4cerr << "G4ExternalDEDXTable::RetrievePhysicsTable() " 
                << " File content of " << fileName << " ill-formated." 
                << G4endl;
#endif          
         ifilestream.close();
         return false;
      } 

      physicsVector -> SetSpline(true);

      // Retrieved vector is added to material store
      if( !AddPhysicsVector(physicsVector, atomicNumberIon, 
			    materialName, atomicNumberMat) ) {

  	 delete physicsVector;
         ifilestream.close();
         return false;
      }
  }

  ifilestream.close();

  return true;
}

// #########################################################################

G4PhysicsVector* G4ExternalDEDXTable::CreatePhysicsVector(G4int vectorType) {

  G4PhysicsVector* physicsVector = 0;

  switch (vectorType) {

  case T_G4PhysicsLinearVector: 
    physicsVector = new G4PhysicsLinearVector();
    break;

  case T_G4PhysicsLogVector: 
    physicsVector = new G4PhysicsLogVector();
    break;

  case T_G4PhysicsLnVector: 
    physicsVector = new G4PhysicsLnVector();
    break;

  case T_G4PhysicsFreeVector: 
    physicsVector = new G4PhysicsFreeVector();
    break;

  case T_G4PhysicsOrderedFreeVector: 
    physicsVector = new G4PhysicsOrderedFreeVector();
    break;

  case T_G4LPhysicsFreeVector: 
    physicsVector = new G4LPhysicsFreeVector();
    break;
  
  default:
    break;
  }
  return physicsVector;
}

// #########################################################################

G4int G4ExternalDEDXTable::FindAtomicNumberElement(
                                   G4PhysicsVector* physicsVector
                                                   ) {

  G4int atomicNumber = 0;

  G4IonDEDXMapElem::iterator iter = dedxMapElements.begin();
  G4IonDEDXMapElem::iterator iter_end = dedxMapElements.end();
  
  for(;iter != iter_end; iter++) {

     if( (*iter).second == physicsVector ) {

        G4IonDEDXKeyElem key = (*iter).first;
        atomicNumber = key.second;
     }
  }

  return atomicNumber;
}

// #########################################################################

void G4ExternalDEDXTable::ClearTable() {

  G4IonDEDXMapMat::iterator iterMat = dedxMapMaterials.begin();
  G4IonDEDXMapMat::iterator iterMat_end = dedxMapMaterials.end();

  for(;iterMat != iterMat_end; iterMat++) { 

    G4PhysicsVector* vec = iterMat -> second;

    if(vec != 0) delete vec;
  }

  dedxMapElements.clear();
  dedxMapMaterials.clear();
}

// #########################################################################

void G4ExternalDEDXTable::DumpMap() {

  G4IonDEDXMapMat::iterator iterMat = dedxMapMaterials.begin();
  G4IonDEDXMapMat::iterator iterMat_end = dedxMapMaterials.end();

  G4cout << std::setw(15) << std::right
         << "Atomic nmb ion"
         << std::setw(25) << std::right
         << "Material name"
         << std::setw(25) << std::right
         << "Atomic nmb material"
         << G4endl;

  for(;iterMat != iterMat_end; iterMat++) {
      G4IonDEDXKeyMat key = iterMat -> first;
      G4PhysicsVector* physicsVector = iterMat -> second; 

      G4int atomicNumberIon = key.first;
      G4String matIdentifier = key.second;

      G4int atomicNumberElem = FindAtomicNumberElement(physicsVector);

      if(physicsVector != 0) {
         G4cout << std::setw(15) << std::right
                << atomicNumberIon
                << std::setw(25) << std::right
                << matIdentifier
                << std::setw(25) << std::right;

         if(atomicNumberElem > 0) G4cout << atomicNumberElem;
         else G4cout << "N/A";

         G4cout << G4endl;
      }
  }

}

// #########################################################################


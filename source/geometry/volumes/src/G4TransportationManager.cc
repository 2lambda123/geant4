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
// $Id: G4TransportationManager.cc,v 1.9 2001-10-24 15:33:49 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//  G4TransportationManager 
//
//  
#include "G4TransportationManager.hh"

//  The following inclusions should be left here, as only
//    the constructor and destructor require them.
#include "G4GeometryMessenger.hh"
#include "G4PropagatorInField.hh"
#include "G4FieldManager.hh"

// This will ensure correct order of construction and destruption of 
//  static objects.
#include "G4NavigationLevel.hh"

G4Allocator<G4NavigationLevel>     aNavigationLevelAllocator;
G4Allocator<G4NavigationLevelRep>  aNavigLevelRepAllocator;

// Ditto: correct order initialisation of class (class) data members
const G4double G4FieldManager::fDefault_Delta_Intersection_Val= 0.1 * mm;
const G4double G4FieldManager::fDefault_Delta_One_Step_Value = 0.25 * mm;

// const G4double G4PropagatorInField::fDefault_Delta_Intersection_Val= 0.1 * mm;
// const G4double G4PropagatorInField::fDefault_Delta_One_Step_Value = 0.25 * mm;
// const G4double G4PropagatorInField::fEpsilonMin = 1.0e-10 ;

G4TransportationManager* G4TransportationManager::fTransportationManager=0;

G4TransportationManager::G4TransportationManager() 
{ 
  if (!fTransportationManager)
  {
    fGeomMessenger=        new G4GeometryMessenger(this);
    fNavigatorForTracking= new G4Navigator() ;
    fFieldManager=         new G4FieldManager() ;
    fPropagatorInField=    new G4PropagatorInField( fNavigatorForTracking,
                                                    fFieldManager);
  }
  else
  {
    G4Exception("Only ONE instance of G4TransportationManager is allowed!");
  }
} 


G4TransportationManager::~G4TransportationManager()
{
  delete fGeomMessenger;
  delete fNavigatorForTracking; 
  delete fPropagatorInField;
  delete fFieldManager; 
}


G4TransportationManager* G4TransportationManager::GetTransportationManager()
{
   static G4TransportationManager theInstance;
   if (!fTransportationManager)
     fTransportationManager = &theInstance;
   
   return fTransportationManager;
}

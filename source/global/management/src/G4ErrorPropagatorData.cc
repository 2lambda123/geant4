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
// $Id$
//
//
// --------------------------------------------------------------------
//      GEANT 4 class implementation file 
// --------------------------------------------------------------------

#include "G4ErrorPropagatorData.hh"

//-------------------------------------------------------------------

__thread G4ErrorPropagatorData* G4ErrorPropagatorData::theErrorPropagatorData = 0;
__thread G4int G4ErrorPropagatorData::theVerbosity = 0;

//-------------------------------------------------------------------

G4ErrorPropagatorData::G4ErrorPropagatorData()
  : theMode(G4ErrorMode_PropTest), theState(G4ErrorState_PreInit),
    theStage(G4ErrorStage_Inflation), theTarget(0)
{
}

G4ErrorPropagatorData::~G4ErrorPropagatorData()
{
}

G4ErrorPropagatorData* G4ErrorPropagatorData::GetErrorPropagatorData()
{
  static __thread G4ErrorPropagatorData *errorPropagatorData_G4MT_TLS_ = 0 ; if (!errorPropagatorData_G4MT_TLS_) errorPropagatorData_G4MT_TLS_ = new  G4ErrorPropagatorData  ;  G4ErrorPropagatorData &errorPropagatorData = *errorPropagatorData_G4MT_TLS_;
  if( !theErrorPropagatorData )
  {
    theErrorPropagatorData = &errorPropagatorData;
  }
  return theErrorPropagatorData;
}

G4int G4ErrorPropagatorData::verbose() 
{
  return theVerbosity;
}

void G4ErrorPropagatorData::SetVerbose( G4int ver )
{
  theVerbosity = ver;
}

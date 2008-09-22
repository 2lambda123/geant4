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
// $Id: G4PhysicsFreeVector.cc,v 1.11 2008-09-22 11:37:09 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//--------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//  G4PhysicsFreeVector.cc
//
//  History:
//    02 Dec. 1995, G.Cosmo : Structure created based on object model
//    06 June 1996, K.Amako : The 1st version of implemented
//    01 Jul. 1996, K.Amako : Cache mechanism and hidden bin from the 
//                            user introduced
//    26 Sep. 1996, K.Amako : Constructor with only 'bin size' added
//    11 Nov. 2000, H.Kurashige : use STL vector for dataVector and binVector
//
//--------------------------------------------------------------------

#include "G4PhysicsFreeVector.hh"


G4PhysicsFreeVector::G4PhysicsFreeVector() 
  : G4PhysicsVector()
{
  type = T_G4PhysicsFreeVector;
}

G4PhysicsFreeVector::G4PhysicsFreeVector(size_t theNbin)
  : G4PhysicsVector()
{
  type = T_G4PhysicsFreeVector;
  numberOfBin = theNbin;

  // Add extra one bin (hidden to user) to handle correctly when 
  // Energy=theEmax in getValue.
  dataVector.reserve(numberOfBin+1);
  binVector.reserve(numberOfBin+1);

  for (size_t i=0; i<=numberOfBin; i++)
  {
     binVector.push_back(0.0);
     dataVector.push_back(0.0);
  }
}  

G4PhysicsFreeVector::G4PhysicsFreeVector(const G4DataVector& theBinVector, 
                                         const G4DataVector& theDataVector)
{
  type = T_G4PhysicsFreeVector;
  numberOfBin = theBinVector.size();

  // Add extra one bin (hidden to user) to handle correctly when 
  // Energy=theEmax in getValue.
  dataVector.reserve(numberOfBin+1);
  binVector.reserve(numberOfBin+1);

  for (size_t i=0; i<numberOfBin; i++)
  {
     binVector.push_back(theBinVector[i]);
     dataVector.push_back(theDataVector[i]);
  }

  // Put values to extra hidden bin. For 'binVector', the 'edgeMin' of the
  // extra hidden bin is assumed to have the following value. For binary
  // search, this value is completely arbitrary if it is greater than
  // the 'edgeMin' at 'numberOfBin-1'. 
  binVector.push_back ( theBinVector[numberOfBin-1] + 1.0 );


  // Put values to extra hidden bin. For 'dataVector', the 'value' of the
  // extra hidden bin is assumed to have the same as the one at 'numberBin-1'. 
  dataVector.push_back( theDataVector[numberOfBin-1] );

  edgeMin = binVector[0];
  edgeMax = binVector[numberOfBin-1];
}  

G4PhysicsFreeVector::~G4PhysicsFreeVector(){}

void G4PhysicsFreeVector::PutValue( size_t theBinNumber, G4double theBinValue, 
                                    G4double theDataValue )
{
  binVector[theBinNumber]  = theBinValue;
  dataVector[theBinNumber] = theDataValue;

  if( theBinNumber == numberOfBin-1 )
  {
     edgeMax = binVector[numberOfBin-1];

     // Put values to extra hidden bin. For 'binVector', the 'edgeMin' 
     // of the extra hidden bin is assumed to have the following value. 
     // For binary search, this value is completely arbitrary if it is 
     // greater than the 'edgeMin' at 'numberOfBin-1'. 
     binVector[numberOfBin] = theBinValue + 1.0;

     // Put values to extra hidden bin. For 'dataVector', the 'value' 
     // of the extra hidden bin is assumed to have the same as the one 
     // at 'numberBin-1'. 
     dataVector[numberOfBin] = theDataValue;
  }

  if( theBinNumber == 0 )
  {
     edgeMin = binVector[0];
  }
}

G4PhysicsFreeVector::G4PhysicsFreeVector(const G4PhysicsFreeVector& right)
  : G4PhysicsVector(right)
{}

G4PhysicsFreeVector& 
G4PhysicsFreeVector::operator=(const G4PhysicsFreeVector& right)
{
  // Check assignment to self
  //
  if(this == &right) { return *this; }

  DeleteData();
  CopyData(right);

  return *this;
}


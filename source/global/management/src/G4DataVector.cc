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
// $Id: G4DataVector.cc,v 1.7 2005-03-15 19:11:35 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 class implementation file
//
//  G4DataVector.cc
//
//  History:
//    18 Sep. 2001, H.Kurashige : Structure created based on object model
// --------------------------------------------------------------

#include "G4DataVector.hh"
#include <iomanip>

G4DataVector::G4DataVector()
  : std::vector<G4double>()
{
}

G4DataVector::G4DataVector(size_t capacity)
  : std::vector<G4double>(capacity, 0.0)
{
}

G4DataVector::G4DataVector(size_t capacity, G4double value)
  : std::vector<G4double>(capacity, value)
{
}

G4DataVector::~G4DataVector()
{
}

G4bool G4DataVector::Store(std::ofstream& fOut, G4bool ascii)
{
  // Ascii mode
  if (ascii)
  {
    fOut << *this;
    return true;
  } 

  // Binary Mode
  size_t sizeV = size(); 
  fOut.write((char*)(&sizeV), sizeof sizeV);

  G4double* value = new G4double[sizeV];
  size_t i=0;
  for (const_iterator itr=begin(); itr!=end(); itr++, i++)
  {
    value[i]  =  *itr;
  }
  fOut.write((char*)(value), sizeV*(sizeof (G4double)) );
  delete [] value;
  
  return true;
}

G4bool G4DataVector::Retrieve(std::ifstream& fIn, G4bool ascii)
{
  clear();
  size_t sizeV=0;
  
  // retrieve in ascii mode
  if (ascii)
  {
    // contents
    fIn >> sizeV;
    if (fIn.fail())  { return false; }
    
    reserve(sizeV);
    for(size_t i = 0; i < sizeV ; i++)
    {
      G4double vData=0.0;
      fIn >> vData;
      if (fIn.fail())  { return false; }
      push_back(vData);
    }
    return true ;
  }
  
  // retrieve in binary mode
  fIn.read((char*)(&sizeV), sizeof sizeV); 
 
  G4double* value = new G4double[sizeV];
  fIn.read((char*)(value),  sizeV*(sizeof(G4double)) );
  if (G4int(fIn.gcount()) != G4int(sizeV*(sizeof(G4double))) )
  {
    delete [] value;
    return false;
  }

  reserve(sizeV);
  for(size_t i = 0; i < sizeV; i++) 
  {
    push_back(value[i]);
  }
  delete [] value;
  return true;
}
    
std::ostream& operator<<(std::ostream& out, const G4DataVector& pv)
{
  out << pv.size() << G4endl; 
  for(size_t i = 0; i < pv.size(); i++)
  {
    out << std::setprecision(12) << pv[i] << G4endl;
  }

  return out;
}


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
// $Id: G4PhysicsVector.cc,v 1.16 2005-03-15 19:11:35 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 class implementation file
//
//  G4PhysicsVector.cc
//
//  History:
//    02 Dec. 1995, G.Cosmo : Structure created based on object model
//    03 Mar. 1996, K.Amako : Implemented the 1st version
//    01 Jul. 1996, K.Amako : Hidden bin from the user introduced
//    12 Nov. 1998, K.Amako : A bug in GetVectorLength() fixed
//    11 Nov. 2000, H.Kurashige : use STL vector for dataVector and binVector
//    18 Jan. 2001, H.Kurashige : removed ptrNextTable
//    09 Mar. 2001, H.Kurashige : added G4PhysicsVector type 
// --------------------------------------------------------------

#include "G4PhysicsVector.hh"
#include <iomanip>

G4PhysicsVector::G4PhysicsVector()
 : type(T_G4PhysicsVector),
   edgeMin(0.), edgeMax(0.), numberOfBin(0),
   lastEnergy(0.), lastValue(0.), lastBin(0)
{
}

G4PhysicsVector::~G4PhysicsVector() 
{
  dataVector.clear();
  binVector.clear();
  type = T_G4PhysicsVector;
}

G4PhysicsVector::G4PhysicsVector(const G4PhysicsVector& right)
{
  *this=right;
}

G4PhysicsVector& G4PhysicsVector::operator=(const G4PhysicsVector& right)
{
  if (&right==this)  { return *this; }
  if (type != right.type)  { return *this; }

  type = right.type;
  edgeMin = right.edgeMin;
  edgeMax = right.edgeMax;
  numberOfBin = right.numberOfBin;
  lastEnergy = right.lastEnergy;
  lastValue = right.lastValue;
  lastBin = right.lastBin;
  dataVector = right.dataVector;
  binVector = right.binVector;
  comment = right.comment;
  return *this;
}

G4int G4PhysicsVector::operator==(const G4PhysicsVector &right) const
{
  return (this == &right);
}

G4int G4PhysicsVector::operator!=(const G4PhysicsVector &right) const
{
  return (this != &right);
}

G4double G4PhysicsVector::GetLowEdgeEnergy(size_t binNumber) const
{
  return binVector[binNumber];
}

G4bool G4PhysicsVector::Store(std::ofstream& fOut, G4bool ascii)
{
  // Ascii mode
  if (ascii)
  {
    fOut << *this;
    return true;
  } 
  // Binary Mode

  // binning
  fOut.write((char*)(&edgeMin), sizeof edgeMin);
  fOut.write((char*)(&edgeMax), sizeof edgeMax);
  fOut.write((char*)(&numberOfBin), sizeof numberOfBin);

  // contents
  size_t size = dataVector.size(); 
  fOut.write((char*)(&size), sizeof size);

  G4double* value = new G4double[2*size];
  for(size_t i = 0; i < size; i++)
  {
    value[2*i]  =  binVector[i];
    value[2*i+1]=  dataVector[i];
  }
  fOut.write((char*)(value), 2*size*(sizeof (G4double)));
  delete [] value;

  return true;
}

G4bool G4PhysicsVector::Retrieve(std::ifstream& fIn, G4bool ascii)
{
  // clear properties;
  lastEnergy=0.;
  lastValue =0.;
  lastBin   =0;
  dataVector.clear();
  binVector.clear();
  comment = "";

  // retrieve in ascii mode
  if (ascii)
  {
    // binning
    fIn >> edgeMin >> edgeMax >> numberOfBin; 
    if (fIn.fail())  { return false; }
    // contents
    size_t size=0;
    fIn >> size;
    if (fIn.fail())  { return false; }

    binVector.reserve(size);
    dataVector.reserve(size);
    for(size_t i = 0; i < size ; i++)
    {
      G4double vBin=0., vData=0.;
      fIn >> vBin >> vData;
      if (fIn.fail())  { return false; }
      binVector.push_back(vBin);
      dataVector.push_back(vData);
    }
    return true ;
  }

  // retrieve in binary mode
  // binning
  fIn.read((char*)(&edgeMin), sizeof edgeMin);
  fIn.read((char*)(&edgeMax), sizeof edgeMax);
  fIn.read((char*)(&numberOfBin), sizeof numberOfBin ); 
 
  // contents
  size_t size;
  fIn.read((char*)(&size), sizeof size); 
 
  G4double* value = new G4double[2*size];
  fIn.read((char*)(value),  2*size*(sizeof(G4double)) );
  if (G4int(fIn.gcount()) != G4int(2*size*(sizeof(G4double))) ){
    delete [] value;
    return false;
  }

  binVector.reserve(size);
  dataVector.reserve(size);
  for(size_t i = 0; i < size; i++) {
    binVector.push_back(value[2*i]);
    dataVector.push_back(value[2*i+1]);
  }
  delete [] value;
  return true;
}
    
std::ostream& operator<<(std::ostream& out, const G4PhysicsVector& pv)
{
  // binning
  out << std::setprecision(12) << pv.edgeMin;
  out <<" " << pv.edgeMax <<" "  << pv.numberOfBin << G4endl; 

  // contents
  out << pv.dataVector.size() << G4endl; 
  for(size_t i = 0; i < pv.dataVector.size(); i++)
  {
    out << std::setprecision(12) << pv.binVector[i] << "  "
        << pv.dataVector[i] << G4endl;
  }
  return out;
}

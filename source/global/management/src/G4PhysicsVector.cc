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
// $Id: G4PhysicsVector.cc,v 1.29 2009-05-12 08:53:35 vnivanch Exp $
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
//    05 Sep. 2008, V.Ivanchenko : added protections for zero-length vector
//    11 may  2009, A.Bagulya : added new implementation of methods 
//            ComputeSecondDerivatives - first derivatives at edge points 
//                                       should be provided by a user
//            FillSecondDerivatives - default computation base on "not-a-knot"
//                                    algorithm
// --------------------------------------------------------------

#include "G4PhysicsVector.hh"
#include <iomanip>

// --------------------------------------------------------------

G4PhysicsVector::G4PhysicsVector(G4bool spline)
 : type(T_G4PhysicsVector),
   edgeMin(0.), edgeMax(0.), numberOfBin(0),
   lastEnergy(0.), lastValue(0.), lastBin(0), 
   secDerivative(0), useSpline(spline)
{}

// --------------------------------------------------------------

G4PhysicsVector::~G4PhysicsVector() 
{
  DeleteData();
}

// --------------------------------------------------------------

G4PhysicsVector::G4PhysicsVector(const G4PhysicsVector& right)
{
  CopyData(right);
}

// --------------------------------------------------------------

G4PhysicsVector& G4PhysicsVector::operator=(const G4PhysicsVector& right)
{
  if (&right==this)  { return *this; }
  if (type != right.type)  { return *this; }

  DeleteData();
  CopyData(right);

  return *this;
}

// --------------------------------------------------------------

G4int G4PhysicsVector::operator==(const G4PhysicsVector &right) const
{
  return (this == &right);
}

// --------------------------------------------------------------

G4int G4PhysicsVector::operator!=(const G4PhysicsVector &right) const
{
  return (this != &right);
}

// --------------------------------------------------------------

void G4PhysicsVector::DeleteData()
{
  delete [] secDerivative;
  secDerivative = 0;
}

// --------------------------------------------------------------

void G4PhysicsVector::CopyData(const G4PhysicsVector& vec)
{
  type = vec.type;
  edgeMin = vec.edgeMin;
  edgeMax = vec.edgeMax;
  numberOfBin = vec.numberOfBin;
  lastEnergy = vec.lastEnergy;
  lastValue = vec.lastValue;
  lastBin = vec.lastBin;
  dataVector = vec.dataVector;
  binVector = vec.binVector;
  useSpline = vec.useSpline;
  comment = vec.comment;
  if (vec.secDerivative)
  {
    secDerivative = new G4double [numberOfBin];
    for (size_t i=0; i<numberOfBin; i++)
    {
       secDerivative[i] = vec.secDerivative[i];
    }
  }
  else
  {
    secDerivative = 0;
  }
}

// --------------------------------------------------------------

G4double G4PhysicsVector::GetLowEdgeEnergy(size_t binNumber) const
{
  return binVector[binNumber];
}

// --------------------------------------------------------------

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

// --------------------------------------------------------------

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
    G4double vBin, vData;

    for(size_t i = 0; i < size ; i++)
    {
      vBin = 0.;
      vData= 0.;
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
  if (G4int(fIn.gcount()) != G4int(2*size*(sizeof(G4double))) )
  {
    delete [] value;
    return false;
  }

  binVector.reserve(size);
  dataVector.reserve(size);
  for(size_t i = 0; i < size; i++)
  {
    binVector.push_back(value[2*i]);
    dataVector.push_back(value[2*i+1]);
  }
  delete [] value;
  return true;
}

// --------------------------------------------------------------

void 
G4PhysicsVector::ComputeSecondDerivatives(G4double firstPointDerivative, 
					  G4double endPointDerivative)
  //  A standard method of computation of second derivatives 
  //  First derivatives at the first and the last point should be provided
  //  See for example W.H. Press et al. "Numerical reciptes and C"
  //  Cambridge University Press, 1997.
{
  secDerivative = new G4double [numberOfBin]; 

  G4int n = numberOfBin-1;

  // cannot compute derivatives for less than 3 points
  if(3 > numberOfBin) {
    secDerivative[0] = 0.0;
    secDerivative[n] = 0.0;
    return;
  }

  G4double* u = new G4double [n];
  
  G4double p, sig, un;

  u[0] = (6.0/(binVector[1]-binVector[0]))
    * ((dataVector[1]-dataVector[0])/(binVector[1]-binVector[0])
       - firstPointDerivative);
 
  secDerivative[0] = - 0.5;

  // Decomposition loop for tridiagonal algorithm. secDerivative[i]
  // and u[i] are used for temporary storage of the decomposed factors.

  for(G4int i=1; i<n; i++)
  {
    sig = (binVector[i]-binVector[i-1]) / (binVector[i+1]-binVector[i-1]);
    p = sig*secDerivative[i-1] + 2.0;
    secDerivative[i] = (sig - 1.0)/p;
    u[i] = (dataVector[i+1]-dataVector[i])/(binVector[i+1]-binVector[i])
         - (dataVector[i]-dataVector[i-1])/(binVector[i]-binVector[i-1]);
    u[i] = 6.0*u[i]/(binVector[i+1]-binVector[i-1]) - sig*u[i-1]/p;
  }

  sig = (binVector[n-1]-binVector[n-2]) / (binVector[n]-binVector[n-2]);
  p = sig*secDerivative[n-2] + 2.0;
  un = (6.0/(binVector[n]-binVector[n-1]))
    *(endPointDerivative - 
      (dataVector[n]-dataVector[n-1])/(binVector[n]-binVector[n-1])) - u[n-1]/p;  
  secDerivative[n] = un/(secDerivative[n-1] + 2.0);

  // The back-substitution loop for the triagonal algorithm of solving
  // a linear system of equations.
   
  for(G4int k=n-1; k>0; k--)
  {
    secDerivative[k] *= 
      (secDerivative[k+1] - 
       u[k]*(binVector[k+1]-binVector[k-1])/(binVector[k+1]-binVector[k]));
  }
  secDerivative[0] = 0.5*(u[0] - secDerivative[1]);

  delete [] u;
}

// --------------------------------------------------------------

void G4PhysicsVector::FillSecondDerivatives()
  // Computation of second derivatives using "Not-a-knot" endpoint conditions
  // B.I. Kvasov "Methods of shape-preserving spline approximation"
  // World Scientific, 2000
{  
  secDerivative = new G4double [numberOfBin]; 

  G4int n = numberOfBin-1;

  // cannot compute derivatives for less than 3 points
  if(3 > numberOfBin) {
    secDerivative[0] = 0.0;
    secDerivative[n] = 0.0;
    return;
  }

  G4double* u = new G4double [n];
  
  G4double p, sig;

  u[1] = ((dataVector[2]-dataVector[1])/(binVector[2]-binVector[1]) -
	  (dataVector[1]-dataVector[0])/(binVector[1]-binVector[0]));
  u[1] = 6.0*u[1]*(binVector[2]-binVector[1])
    / ((binVector[2]-binVector[0])*(binVector[2]-binVector[0]));
 
  // Decomposition loop for tridiagonal algorithm. secDerivative[i]
  // and u[i] are used for temporary storage of the decomposed factors.

  secDerivative[1] = (2.0*binVector[1]-binVector[0]-binVector[2])
    / (2.0*binVector[2]-binVector[0]-binVector[1]);

  for(G4int i=2; i<n-1; i++)
  {
    sig = (binVector[i]-binVector[i-1]) / (binVector[i+1]-binVector[i-1]);
    p = sig*secDerivative[i-1] + 2.0;
    secDerivative[i] = (sig - 1.0)/p;
    u[i] = (dataVector[i+1]-dataVector[i])/(binVector[i+1]-binVector[i])
         - (dataVector[i]-dataVector[i-1])/(binVector[i]-binVector[i-1]);
    u[i] = (6.0*u[i]/(binVector[i+1]-binVector[i-1])) - sig*u[i-1]/p;
  }

  sig = (binVector[n-1]-binVector[n-2]) / (binVector[n]-binVector[n-2]);
  p = sig*secDerivative[n-3] + 2.0;
  u[n-1] = (dataVector[n]-dataVector[n-1])/(binVector[n]-binVector[n-1])
    - (dataVector[n-1]-dataVector[n-2])/(binVector[n-1]-binVector[n-2]);
  u[n-1] = (6.0*sig*u[n-1]/(binVector[n]-binVector[n-2]))
    - (2.0*sig - 1.0)*u[n-2]/p;  

  p = (1.0+sig) + (2.0*sig-1.0)*secDerivative[n-2];
  secDerivative[n-1] = u[n-1]/p;

  // The back-substitution loop for the triagonal algorithm of solving
  // a linear system of equations.
   
  for(G4int k=n-2; k>1; k--)
  {
    secDerivative[k] *= 
      (secDerivative[k+1] - 
       u[k]*(binVector[k+1]-binVector[k-1])/(binVector[k+1]-binVector[k]));
  }
  secDerivative[n] = (secDerivative[n-1] - (1.0-sig)*secDerivative[n-2])/sig;
  sig = 1.0 - ((binVector[2]-binVector[1])/(binVector[2]-binVector[0]));
  secDerivative[1] *= (secDerivative[2] - u[1]/(1.0-sig));
  secDerivative[0]  = (secDerivative[1] - sig*secDerivative[2])/(1.0-sig);

  delete [] u;
}
   
// --------------------------------------------------------------

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

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
// $Id: G4NeutronHPPartial.hh,v 1.9 2003-06-16 17:10:59 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4NeutronHPPartial_h
#define G4NeutronHPPartial_h 1

#include "globals.hh"
#include "G4NeutronHPVector.hh"
#include "G4InterpolationManager.hh"
#include "G4NeutronHPInterpolator.hh"

class G4NeutronHPPartial
{
  public:
  
  G4NeutronHPPartial(G4int n)
  { 
    X = new G4double[n];
    data = new G4NeutronHPVector[n];
    nData = n;
    T=NULL;
  }
  
  G4NeutronHPPartial(G4int n1, G4int n2)
  {
    T = new G4double[n2];
    X = new G4double[n1];
    data = new G4NeutronHPVector[n1];
    nData = std::max(n1,n2);
  }  
  
  void InitInterpolation(G4int i, std::ifstream & aDataFile)
  {
    data[i].InitInterpolation(aDataFile);
  }
  
  void InitInterpolation(std::ifstream & aDataFile)
  {
    theManager.Init(aDataFile);
  }
  
  void Init(std::ifstream & aDataFile)
  {
    G4int i;
    G4double e;
    for( i=0; i<nData; i++)
    {
      aDataFile >> e;
      e *= eV;
      SetX(i,e);
      InitData(i, aDataFile, eV); // energy and probability for gammas
    }  
  }
  
  void InitData(G4int i, std::ifstream & aDataFile, G4double unit=1.)
  {
    G4int ii;
    G4double eg, pg;
    G4int neg;
    aDataFile >> neg;
    data[i].InitInterpolation(aDataFile);
    for (ii=0; ii<neg; ii++)
    { 
      aDataFile >> eg >> pg;
      eg *= unit;
      SetX(i,ii,eg);
      SetY(i,ii,pg);
    }          
  }
  
  ~G4NeutronHPPartial()
  {
    delete [] X;
    if(T!=NULL) delete [] T;
    delete [] data;
  }
  inline G4int GetNumberOfEnergies() {return nData;}
  
  inline void SetX(G4int i, G4double x) {X[i]=x;}
  inline void SetT(G4int i, G4double x) {T[i]=x;}
  inline void SetX(G4int i, G4int j, G4double x) {data[i].SetX(j,x);}
  inline void SetY(G4int i, G4int j, G4double y) {data[i].SetY(j,y);}
  
  inline G4double GetX(G4int i) {return X[i];}
  inline G4double GetT(G4int i) {return T[i];}
  inline G4double GetX(G4int i, G4int j) {return data[i].GetX(j);}
  inline G4double GetY(G4int i, G4int j) {return data[i].GetY(j);}
  inline G4double GetY(G4int i, G4double e) {return data[i].GetY(e);}
  inline G4int GetNEntries(G4int i) {return data[i].GetVectorLength();}
  G4NeutronHPVector * GetY(G4double e1);
  G4double Sample(G4double x);
  
  private:
  
  G4double * X;
  G4double * T;
  G4NeutronHPVector * data;
  G4NeutronHPVector * theBuffer;
  G4int nData;
  G4InterpolationManager theManager; // interpolate between different data[i]
  G4NeutronHPInterpolator theInt;
};

#endif

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
#ifndef G4CrossSectionBuffer_h
#define G4CrossSectionBuffer_h

#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include <utility>
#include <vector>

class G4CrossSectionBuffer
{
  public:
  
    G4CrossSectionBuffer(const G4ParticleDefinition * aA, const G4ParticleDefinition * aB)
    : theA(aA), theB(aB) {}
    
    G4bool InCharge(const G4ParticleDefinition * aA, const G4ParticleDefinition * aB) const
    {
      G4bool result = false;
      if(aA == theA)
      {
        if(aB == theB) result = true;
      }
      else if(aA == theB)
      {
        if(aB == theA) result = true;
      }
      return result;
    }
    
    void push_back(G4double s, G4double x)
    {
      std::pair<G4double, G4double> aNew;
      aNew.first = s;
      aNew.second = x;
      theData.push_back(aNew);
    }
    G4double CrossSection(const G4KineticTrack& trk1, const G4KineticTrack& trk2) const
    {
      G4double sqrts = (trk1.Get4Momentum()+trk2.Get4Momentum()).mag();
      G4double x1(1), y1(0);
      G4double x2(2), y2(0);
      
      if(theData.size()==1) return theData[theData.size()-1].second;
      
      for(size_t i=0; i<theData.size(); i++)
      {
        if(theData[i].first>sqrts)
	{
	  if(0==i)
	  {
	    x1 = theData[i].first;
	    y1 = theData[i].second;
	    x2 = theData[i+1].first;
	    y2 = theData[i+1].second;
	  }
	  else if(theData.size()-1==i)
	  {
	    x1 = theData[theData.size()-2].first;
	    y1 = theData[theData.size()-2].second;
	    x2 = theData[theData.size()-1].first;
	    y2 = theData[theData.size()-1].second;
	  }
	  else
	  {
	    x1 = theData[i-1].first;
	    y1 = theData[i-1].second;
	    x2 = theData[i].first;
	    y2 = theData[i].second;
	  }
	  break;
	}
      }
      // LINLIN interpolation
      // G4cerr << "!!!!!! "<<sqrts<<" "<<x1<<" "<<x2<<" "<<y1<<" "<<y2<<" "<<i<<G4endl;
      G4double result = y1 + (sqrts-x1) * (y2-y1)/(x2-x1);
      if(result<0) result = 0;
      if(y1<0.01*millibarn) result = 0;
      return result;
    }
    
    void Print()
    {
      for(size_t i=0;i<theData.size(); i++)
      {
        G4cerr << "sqrts = "<<theData[i].first<<", X = "<<theData[i].second/millibarn<<G4endl;
      }
    }
  
  private:
  std::vector<std::pair<G4double, G4double> > theData;
  
  const G4ParticleDefinition * theA;
  const G4ParticleDefinition * theB;
};

#endif

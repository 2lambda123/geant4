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

#include "globals.hh"
#include "G4ios.hh"
#include "G4XNNTotalLowE.hh"
#include "G4KineticTrack.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleDefinition.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"

const G4int G4XNNTotalLowE::tableSize = 29;

const G4double G4XNNTotalLowE::ss[29]=
{
 1877.05, 1879.58, 1882.69, 1884.8, 1886.95, 1891.89, 1895.27, 1899.9, 1904.67, 1913.36, 1921.18, 1933.56, 1949.82, 1990.11, 2025.14, 2059.03, 2101.89, 2166.47, 2201.01, 2236.36, 2289.27, 2377.17, 2426.86, 2500.18, 2602.91, 2733.62, 2925.49, 3002.71 
};

const G4double G4XNNTotalLowE::ppTot[29] = 
{ 
  2000, 600, 250, 180, 138, 92, 75, 57, 46, 35, 29.5, 25.5, 25, 24, 23.7, 25, 29, 39, 44, 47, 48, 48, 47.5, 47, 45.6, 45, 43.3, 42.9, 42.9
};

const G4double G4XNNTotalLowE::npTot[29] = 
{
4250, 1380, 770, 585, 465, 300, 232, 175, 140, 100, 80, 63, 50, 40, 35, 34, 34, 36.5, 37., 37.7, 38, 39, 39.8, 40.5, 40.7, 41, 41.2, 41.5, 41.5
};

G4XNNTotalLowE::G4XNNTotalLowE() 
{ 
    
  G4LowEXsection* pp = new G4LowEXsection();
  G4LowEXsection* np = new G4LowEXsection();

  G4int i;
  for (i=0; i<tableSize; i++)
    {
      std::pair<double,double> it;
      it.first=ss[i];
      it.second=ppTot[i]; pp->push_back(it);
      it.second=npTot[i]; np->push_back(it);
    }
  theCrossSections[G4Proton::ProtonDefinition()->GetParticleName()] = pp;
  theCrossSections[G4Neutron::NeutronDefinition()->GetParticleName()] = np;
}


G4XNNTotalLowE::~G4XNNTotalLowE()
{
  delete theCrossSections[G4Proton::ProtonDefinition()->GetParticleName()];
  delete theCrossSections[G4Neutron::NeutronDefinition()->GetParticleName()];
}

G4double G4XNNTotalLowE::CrossSection(const G4KineticTrack& trk1, const G4KineticTrack& trk2) const
{
  G4double result = 0.;
  G4double sqrtS = (trk1.Get4Momentum() + trk2.Get4Momentum()).mag();
  G4String key = FindKeyParticle(trk1,trk2);

  if (theCrossSections.find(key)!= theCrossSections.end())
  {
    LowEMap::const_iterator iter;
    for (iter = theCrossSections.begin(); iter != theCrossSections.end(); ++iter)
    {
      if ((*iter).first == key)
      {
	result = (*iter).second->CrossSection(sqrtS);
      }
    }
  }
  else
  {
    throw G4HadronicException(__FILE__, __LINE__, "G4XNNTotalLowE: particle key out of range");
  }

  return result;
}

G4String G4XNNTotalLowE::Name() const
{
  G4String name("NNTotalLowE");
  return name;
}


G4bool G4XNNTotalLowE::IsValid(G4double e) const
{
  G4bool result = e>0&&e<3*GeV;
  return result;
}



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
// $Id: G4ShellData.cc,v 1.8 2006-06-29 19:41:21 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// History:
// -----------
// 31 Jul 2001   MGP        Created
//
// -------------------------------------------------------------------

#include "G4ShellData.hh"
#include "G4DataVector.hh"
#include <fstream>
#include <sstream>

// Constructor

G4ShellData::G4ShellData(G4int minZ, G4int maxZ)
  : zMin(minZ), zMax(maxZ)
{ }

// Destructor
G4ShellData::~G4ShellData()
{
  std::map<G4int,G4DataVector*,std::less<G4int> >::iterator pos;

  for (pos = idMap.begin(); pos != idMap.end(); ++pos)
    {
      G4DataVector* dataSet = (*pos).second;
      delete dataSet;
    }
  for (pos = bindingMap.begin(); pos != bindingMap.end(); ++pos)
    {
      G4DataVector* dataSet = (*pos).second;
      delete dataSet;
    }
}


size_t G4ShellData::NumberOfShells(G4int Z) const
{
  G4int z = Z - 1;
  G4int n = 0;

  if (Z>= zMin && Z <= zMax)
    {
      n = nShells[z];
    }
  return n;
}


const G4DataVector& G4ShellData::ShellIdVector(G4int Z) const
{
  std::map<G4int,G4DataVector*,std::less<G4int> >::const_iterator pos;
  if (Z < zMin || Z > zMax)
    G4Exception("G4ShellData::ShellIdVector - Z outside boundaries");
  pos = idMap.find(Z);
  G4DataVector* dataSet = (*pos).second;
  return *dataSet;
}

G4int G4ShellData::ShellId(G4int Z, G4int shellIndex) const
{
  G4int n = -1;

  if (Z >= zMin && Z <= zMax)
    {
      std::map<G4int,G4DataVector*,std::less<G4int> >::const_iterator pos;
      pos = idMap.find(Z);
      if (pos!= idMap.end())
	{
	  G4DataVector dataSet = *((*pos).second);
	  G4int nData = dataSet.size();
	  if (shellIndex >= 0 && shellIndex < nData)
	    {
	      n = (G4int) dataSet[shellIndex];
	    }
	}
    }
  return n;
}


G4double G4ShellData::BindingEnergy(G4int Z, G4int shellIndex)  const
{
  G4double value = 0.;

  if (Z >= zMin && Z <= zMax)
    {
      std::map<G4int,G4DataVector*,std::less<G4int> >::const_iterator pos;
      pos = bindingMap.find(Z);
      if (pos!= bindingMap.end())
	{
	  G4DataVector dataSet = *((*pos).second);
	  G4int nData = dataSet.size();
	  if (shellIndex >= 0 && shellIndex < nData)
	    {
	      value = dataSet[shellIndex];
	    }
	}
    }
  return value;
}

void G4ShellData::PrintData() const
{
  for (G4int Z = zMin; Z <= zMax; Z++)
    {
      G4cout << "---- Shell data for Z = "
	     << Z
	     << " ---- "
	     << G4endl;
      G4int nSh = nShells[Z-1];
      std::map<G4int,G4DataVector*,std::less<G4int> >::const_iterator posId;
      posId = idMap.find(Z);
      G4DataVector* ids = (*posId).second;
      std::map<G4int,G4DataVector*,std::less<G4int> >::const_iterator posE;
      posE = bindingMap.find(Z);
      G4DataVector* energies = (*posE).second;
      for (G4int i=0; i<nSh; i++)
	{
	  G4int id = (G4int) (*ids)[i];
	  G4double e = (*energies)[i] / MeV;
	  G4cout << i <<") Shell id: " << id 
		 << " - Binding energy = "
		 << e << " MeV " << G4endl;
	}
      G4cout << "-------------------------------------------------" 
	     << G4endl;
    }
}


void G4ShellData::LoadData(const G4String& fileName)
{ 
  // Build the complete string identifying the file with the data set
  
  std::ostringstream ost;
  
  ost << fileName << ".dat";
  
  G4String name(ost.str());
  
  char* path = getenv("G4LEDATA");
  if (!path)
    { 
      G4String excep("G4EMDataSet - G4LEDATA environment variable not set");
      G4Exception(excep);
    }
  
  G4String pathString(path);
  G4String dirFile = pathString + name;
  std::ifstream file(dirFile);
  std::filebuf* lsdp = file.rdbuf();

  if (! (lsdp->is_open()) )
    {
      G4String s1("G4ShellData - data file: ");
      G4String s2(" not found");
      G4String excep = s1 + dirFile + s2;
      G4Exception(excep);
    }

  G4double a = 0;
  G4int k = 1;
  G4int s = 0;
  
  G4int Z = 1;
  G4DataVector* energies = new G4DataVector;
  G4DataVector* ids = new G4DataVector;

  do {
    file >> a;
    G4int nColumns = 2;
    if (a == -1)
      {
	if (s == 0)
	  {
	    // End of a shell data set
	    idMap[Z] = ids;
            bindingMap[Z] = energies;
            G4int n = ids->size();
	    nShells.push_back(n);
	    // Start of new shell data set
	    ids = new G4DataVector;
            energies = new G4DataVector;
            Z++;	    
	  }      
	s++;
	if (s == nColumns)
	{
	  s = 0;
	}
      }
    else if (a == -2)
      {
	// End of file; delete the empty vectors created when encountering the last -1 -1 row
	delete energies;
	delete ids;
	//nComponents = components.size();
      }
    else
      {
	// 1st column is shell id
	if(k%nColumns != 0)
	  {	    
	    ids->push_back(a);
	    k++;
	  }
	else if (k%nColumns == 0)
	  {
	    // 2nd column is binding energy
	    G4double e = a * MeV;
	    energies->push_back(e);
	    k = 1;
	  }
      }
  } while (a != -2); // end of file
  file.close();    
}


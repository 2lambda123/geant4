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
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4VhShellCrossSection
//
// Author:        V.Ivanchenko (Vladimir.Ivanchenko@cern.ch)
// 
// History:
// -----------
// 20 Oct 2001 V.Ivanchenko   1st implementation
// 24 Oct 2001 MGP            Minor clean-up
// 29 Oct 2001 VI             Add delta energy
//
// -------------------------------------------------------------------

#include "G4VhShellCrossSection.hh"
#include "Randomize.hh"

G4VhShellCrossSection::G4VhShellCrossSection()
{ }


G4VhShellCrossSection::~G4VhShellCrossSection() 
{ }

void G4VhShellCrossSection :: SetTotalCS(G4double)
{
}


G4int G4VhShellCrossSection::SelectRandomShell(G4int Z, 
                                               G4double incidentEnergy,
					       G4double mass, 
					       G4double deltaEnergy) const 
{
  std::vector<G4double> p = Probabilities(Z,incidentEnergy,mass,deltaEnergy);
  G4int shell = 0;
  size_t nShells = p.size();
  G4double q = G4UniformRand();
  for (size_t i=0; i<nShells; i++) {
    
    if (p[i] >= q) {
      shell = i;
      break;
    }
    q -= p[i];
  }
  return shell;
}



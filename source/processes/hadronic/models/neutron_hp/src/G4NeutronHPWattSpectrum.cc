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
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
#include "G4NeutronHPWattSpectrum.hh"

  G4double G4NeutronHPWattSpectrum::Sample(G4double anEnergy) 
  {
    G4double a = theApar.GetY(anEnergy)*eV;
    G4double b = theBpar.GetY(anEnergy)/eV;
    G4double result;
    G4double random, cut, max;
    max = std::sinh(std::sqrt(b*15.*a));
    do
    {
      random = G4UniformRand();
      result = -a*std::log(random);
      cut = G4UniformRand();
    }
    while(cut>std::sinh(std::sqrt(b*result))/max);
    return result;
  }

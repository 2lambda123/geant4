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
#ifndef G4CASCAD_SPECIAL_FUNC_HH
#define G4CASCAD_SPECIAL_FUNC_HH

#include "globals.hh"

#include <math.h>
#include <algorithm>
#include <vector>

namespace G4CascadSpecialFunctions {

  std::pair<G4int, G4double> getPositionInEnergyScale2(G4double e); 

  std::pair<G4int, G4double> getPositionInEnergyScale1(G4double e);
 
  G4double absorptionCrosSection(G4double e, 
				 G4int type);

  G4double crossSection(G4double e, 
			G4int is);

  std::pair<G4int, G4double> getPositionInEnergyScaleEMS(G4double e); 

}
#endif

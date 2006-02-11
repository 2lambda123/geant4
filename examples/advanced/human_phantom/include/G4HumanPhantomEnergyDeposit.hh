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
// ----------------------------------------------------------------------------

#ifndef G4HumanPhantomEnergyDeposit_H
#define G4HumanPhantomEnergyDeposit_H 1

#include "globals.hh"
#include <map>

class G4HumanPhantomEnergyDeposit 
{
public:
  G4HumanPhantomEnergyDeposit();
  ~G4HumanPhantomEnergyDeposit();
 
  std::map<std::string,G4double> energyTotal;

  void Fill(G4String bodypartName, G4double energyDeposit);
  void TotalEnergyDeposit();

private:
  G4String bodypartName;
  G4double totalBody;
};
#endif

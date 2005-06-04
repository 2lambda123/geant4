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
// $Id: G4NeutronHPIsotropic.hh,v 1.9 2005-06-04 13:44:43 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4NeutronHPIsotropic_h
#define G4NeutronHPIsotropic_h 1

#include "G4ios.hh"
#include <fstream>
#include "globals.hh"
#include "G4ReactionProduct.hh"
#include "G4VNeutronHPEnergyAngular.hh"

class G4NeutronHPIsotropic : public G4VNeutronHPEnergyAngular
{
  public:
  
  G4NeutronHPIsotropic(){}
  
  ~G4NeutronHPIsotropic(){}
  
  public:
  
  void Init(std::ifstream & aDataFile);
  G4ReactionProduct * Sample(G4double anEnergy, G4double massCode, G4double mass);
  G4double MeanEnergyOfThisInteraction()
  {
    return -1.;
  }
  private:

};
#endif

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
// $Id: G4RToEConvForNeutron.hh,v 1.3 2003-06-16 16:58:19 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//
// Class Description
//  This class is a Range to Energy Converter for Neutron.
//
// ------------------------------------------------------------
//   First Implementation          5 Oct. 2002  H.Kurahige
// ------------------------------------------------------------

#ifndef G4RToEConvForNeutron_h
#define G4RToEConvForNeutron_h 1

#include "globals.hh"
#include "G4ios.hh"
#include <vector>

#include "G4VRangeToEnergyConverter.hh"


class G4RToEConvForNeutron : public G4VRangeToEnergyConverter
{
  public: 
  //  constructor
  G4RToEConvForNeutron();

  public:
  //  destructor
  virtual ~G4RToEConvForNeutron();

 // calculate energy cut from given range cut for the material
  virtual G4double Convert(G4double rangeCut, const G4Material* material);

};

inline
  G4double G4RToEConvForNeutron::Convert(G4double , const G4Material* )
{
  // reutrn lowest energy
  return LowestEnergy;
}
#endif










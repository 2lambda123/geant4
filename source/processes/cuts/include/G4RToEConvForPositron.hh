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
// $Id: G4RToEConvForPositron.hh,v 1.1 2003-09-19 14:43:48 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//
// Class Description
//  This class is a Range to Energy Converter for positron.
//
// ------------------------------------------------------------
//   First Implementation          5 Oct. 2002  H.Kurahige
// ------------------------------------------------------------

#ifndef G4RToEConvForPositron_h
#define G4RToEConvForPositron_h 1

#include "globals.hh"
#include "G4ios.hh"
#include <vector>

#include "G4VRangeToEnergyConverter.hh"


class G4RToEConvForPositron : public G4VRangeToEnergyConverter
{
  public:
  //  constructor
  G4RToEConvForPositron();

  public:
  //  destructor
  virtual ~G4RToEConvForPositron();

  protected:
    virtual G4double ComputeLoss(G4double AtomicNumber,
                                 G4double KineticEnergy
                                ) const;

  //-------------- Range Table ------------------------------------------
    virtual void BuildRangeVector(const G4Material* aMaterial,
                                  G4double       maxEnergy,
                                  G4double       aMass,
                                  G4RangeVector* rangeVector);


};


#endif










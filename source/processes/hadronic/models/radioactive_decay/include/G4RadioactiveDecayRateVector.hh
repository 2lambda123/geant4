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
#ifndef G4RadioactiveDecayRateVector_h
#define G4RadioactiveDecayRateVector_h 1
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:              RadioactiveDecayRateVector.hh
//
// Version:             0.b.4
// Date:                14/04/00
// Author:              F Lei & P R Truscott
// Organisation:        DERA UK
// Customer:            ESA/ESTEC, NOORDWIJK
// Contract:            12115/96/JG/NL Work Order No. 3
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 29 February 2000, P R Truscott, DERA UK
// 0.b.3 release.
//
// 13 April 2000, F Lei, DERA UK
// 0.b.4 release. No change to this file     
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//
#include "G4ios.hh"
#include "globals.hh"
#include "G4RadioactiveDecayRate.hh"

#include <vector>
////////////////////////////////////////////////////////////////////////////////
//
class G4RadioactiveDecayRateVector
{
  // class description
  //   This class contains the decay times and coefficients for calculating
  //   all the descendants of the named isotope. These data can be used to
  //   calculate their radioactivity at any given time
  // class description - end
public:
  typedef std::vector<G4RadioactiveDecayRate> G4RadioactiveDecayRates;
  
public:
  //Constructors
  G4RadioactiveDecayRateVector();
  
  //  Destructor
  virtual ~G4RadioactiveDecayRateVector();
  
public:
  //  copy constructor and assignment operatotr
  G4RadioactiveDecayRateVector(const G4RadioactiveDecayRateVector &);
  G4RadioactiveDecayRateVector 
    & operator=(const G4RadioactiveDecayRateVector &);
  
public:
  // equality operators
  G4int operator==(const G4RadioactiveDecayRateVector &right) const
    {return (this == &right);}
  G4int operator!=(const G4RadioactiveDecayRateVector &right) const
    {return (this != &right);}
  
public: // with description

  inline G4String  GetIonName() const {return ionName;}
  //
  // To retrieve the name of the isotope.
  inline G4RadioactiveDecayRates  GetItsRates() const {return itsRates; }
  //
  // To retrieve the coefficients and decays of all its descendants along the
  // decay chain.
  inline void SetIonName(G4String name) {ionName = name;}
  // 
  // To set the name of the isotope.
  inline void SetItsRates(G4RadioactiveDecayRates arate) {itsRates = arate;}
  //
  // To fill the coefficient and decay time table

protected:

  G4String                ionName;
  G4RadioactiveDecayRates itsRates;

};
#endif


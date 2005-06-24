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
// $Id: G4DNAElectronElasticBrenner.cc,v 1.2 2005-06-24 10:07:13 capra Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4DNAElectronElasticBrenner.hh"

const G4double     G4DNABrennerEnergyLimitsPolicy::lowEnergyLimit(200*eV);
const G4bool       G4DNABrennerEnergyLimitsPolicy::zeroBelowLowEnergyLimit(true);
const G4double     G4DNABrennerEnergyLimitsPolicy::highEnergyLimit(10*keV);
const G4bool       G4DNABrennerEnergyLimitsPolicy::zeroAboveHighEnergyLimit(true);

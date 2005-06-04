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
// $Id: G4FissionProbability.cc,v 1.6 2005-06-04 13:22:51 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//


#include "G4FissionProbability.hh"
#include "G4PairingCorrection.hh"



G4FissionProbability::G4FissionProbability(const G4FissionProbability &) : G4VEmissionProbability()
{
    throw G4HadronicException(__FILE__, __LINE__, "G4FissionProbability::copy_constructor meant to not be accessable");
}




const G4FissionProbability & G4FissionProbability::operator=(const G4FissionProbability &)
{
    throw G4HadronicException(__FILE__, __LINE__, "G4FissionProbability::operator= meant to not be accessable");
    return *this;
}


G4bool G4FissionProbability::operator==(const G4FissionProbability &) const
{
    return false;
}

G4bool G4FissionProbability::operator!=(const G4FissionProbability &) const
{
    return true;
}


G4double G4FissionProbability::EmissionProbability(const G4Fragment & fragment, const G4double MaximalKineticEnergy)
    // Compute integrated probability of fission channel
{
    if (MaximalKineticEnergy <= 0.0) return 0.0;
    G4double A = fragment.GetA();
    G4double Z = fragment.GetZ();
    G4double U = fragment.GetExcitationEnergy();
  
    G4double Ucompound = U - G4PairingCorrection::GetInstance()->GetPairingCorrection(static_cast<G4int>(A),
										      static_cast<G4int>(Z));
    G4double Ufission = U - G4PairingCorrection::GetInstance()->GetFissionPairingCorrection(static_cast<G4int>(A),
											    static_cast<G4int>(Z));
  
    G4double SystemEntropy = 2.0*std::sqrt(theEvapLDP.LevelDensityParameter(static_cast<G4int>(A),
								       static_cast<G4int>(Z),
								       Ucompound)*Ucompound);
	
    G4double afission = theFissLDP.LevelDensityParameter(static_cast<G4int>(A),
							 static_cast<G4int>(Z),
							 Ufission);

    G4double Cf = 2.0*std::sqrt(afission*MaximalKineticEnergy);

    //    G4double Q1 = 1.0 + (Cf - 1.0)*std::exp(Cf);
    //    G4double Q2 = 4.0*pi*afission*std::exp(SystemEntropy);
	
    //    G4double probability = Q1/Q2;
 
    
    G4double Exp1 = 0.0;
    if (SystemEntropy <= 160.0) Exp1 = std::exp(-SystemEntropy);
    // @@@@@@@@@@@@@@@@@ hpw changed max to min - cannot notify vicente now since cern mail gave up on me...
    G4double Exp2 = std::exp( std::min(700.0,Cf-SystemEntropy) ); 

    G4double probability = (Exp1 + (1.0-Cf)*Exp2) / 4.0*pi*afission;
    
    return probability;
}


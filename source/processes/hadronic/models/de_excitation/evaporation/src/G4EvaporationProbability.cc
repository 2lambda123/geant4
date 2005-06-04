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
// $Id: G4EvaporationProbability.cc,v 1.4 2005-06-04 13:21:21 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//


#include "G4EvaporationProbability.hh"
#include "G4PairingCorrection.hh"



G4EvaporationProbability::G4EvaporationProbability(const G4EvaporationProbability &) : G4VEmissionProbability()
{
    throw G4HadronicException(__FILE__, __LINE__, "G4EvaporationProbability::copy_constructor meant to not be accessable");
}




const G4EvaporationProbability & G4EvaporationProbability::
operator=(const G4EvaporationProbability &)
{
    throw G4HadronicException(__FILE__, __LINE__, "G4EvaporationProbability::operator= meant to not be accessable");
    return *this;
}


G4bool G4EvaporationProbability::operator==(const G4EvaporationProbability &) const
{
    return false;
}

G4bool G4EvaporationProbability::operator!=(const G4EvaporationProbability &) const
{
    return true;
}

G4double G4EvaporationProbability::EmissionProbability(const G4Fragment & fragment, const G4double anEnergy)
{
    G4double EmissionProbability = 0.0;
    G4double MaximalKineticEnergy = anEnergy;
    if (MaximalKineticEnergy > 0.0 && fragment.GetExcitationEnergy() > 0.0) {
	EmissionProbability = CalcProbability(fragment,MaximalKineticEnergy);
	// 		// Next there is a loop over excited states for this channel summing probabilities
	// 		G4double SavedGamma = Gamma;
	// 		for (G4int i = 0; i < ExcitationEnergies->length(); i++) {
	// 			if (ExcitationSpins->operator()(i) < 0.1) continue;
	// 			Gamma = ExcitationSpins->operator()(i)*A; // A is the channel mass number
	// 			// substract excitation energies
	// 			MaximalKineticEnergy -= ExcitationEnergies->operator()(i);
	// 			// update probability
	// 			EmissionProbability += CalcProbability(fragment,MaximalKineticEnergy);
	// 				EmissionProbability += tmp;
	// 			}
	// 		// restore Gamma and MaximalKineticEnergy
	// 		MaximalKineticEnergy = SavedMaximalKineticEnergy;
	// 		Gamma = SavedGamma;
	// 		}
    }
    return EmissionProbability;
}

G4double G4EvaporationProbability::CalcProbability(const G4Fragment & fragment, 
						   const G4double MaximalKineticEnergy)
    // Calculate integrated probability (width) for rvaporation channel
{	
    G4double ResidualA = static_cast<G4double>(fragment.GetA() - theA);
    G4double ResidualZ = static_cast<G4double>(fragment.GetZ() - theZ);
    G4double U = fragment.GetExcitationEnergy();
	
    G4double NuclearMass = G4ParticleTable::GetParticleTable()->GetIonTable()->GetNucleusMass(theZ,theA);


    G4double delta0 = G4PairingCorrection::GetInstance()->GetPairingCorrection(static_cast<G4int>(fragment.GetA()),
									       static_cast<G4int>(fragment.GetZ()));

    G4double SystemEntropy = 2.0*std::sqrt(theEvapLDPptr->LevelDensityParameter(static_cast<G4int>(fragment.GetA()),
									   static_cast<G4int>(fragment.GetZ()),U)*
				      (U-delta0));
								  
    // compute the integrated probability of evaporation channel
    G4double RN = 1.5*fermi;

    G4double Alpha = CalcAlphaParam(fragment);
    G4double Beta = CalcBetaParam(fragment);
	
    G4double Rmax = MaximalKineticEnergy;
    G4double a = theEvapLDPptr->LevelDensityParameter(static_cast<G4int>(ResidualA),
						      static_cast<G4int>(ResidualZ),
						      Rmax);
    G4double GlobalFactor = static_cast<G4double>(Gamma) * (Alpha/(a*a)) *
	(NuclearMass*RN*RN*std::pow(ResidualA,2./3.))/
	(2.*pi* hbar_Planck*hbar_Planck);
    G4double Term1 = (2.0*Beta*a-3.0)/2.0 + Rmax*a;
    G4double Term2 = (2.0*Beta*a-3.0)*std::sqrt(Rmax*a) + 2.0*a*Rmax;
	
    G4double ExpTerm1 = 0.0;
    if (SystemEntropy <= 600.0) ExpTerm1 = std::exp(-SystemEntropy);
	
    G4double ExpTerm2 = 2.*std::sqrt(a*Rmax) - SystemEntropy;
    if (ExpTerm2 > 700.0) ExpTerm2 = 700.0;
    ExpTerm2 = std::exp(ExpTerm2);
	
    G4double Width = GlobalFactor*(Term1*ExpTerm1 + Term2*ExpTerm2);
	
    return Width;
}





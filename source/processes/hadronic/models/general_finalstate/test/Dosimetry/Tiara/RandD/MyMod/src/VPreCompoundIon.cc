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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: VPreCompoundIon.cc,v 1.1 2003-10-08 12:32:13 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// by V. Lara

#include "VPreCompoundIon.hh"
#include "PreCompoundParameters.hh"
//#include "G4EvaporationLevelDensityParameter.hh"


G4double VPreCompoundIon::
ProbabilityDistributionFunction(const G4double eKin, 
				const G4Fragment& aFragment,G4double dLevelDensity)
{
  if ( !IsItPossible(aFragment) ) return 0.0;

  const G4double r0 = PreCompoundParameters::GetAddress()->Getr0();

  G4double U = aFragment.GetExcitationEnergy();
  G4double P = aFragment.GetNumberOfParticles();
  G4double H = aFragment.GetNumberOfHoles();
  G4double N = P + H;

  G4double E0 = std::max(0.0,U - (P*P+H*H+P-H)*0.25 + H*0.5);
  if (E0 == 0.0) return 0.0;
  G4double E1 = std::max(0.0,U - eKin - GetBindingEnergy() - (P*P+H*H+P-H+GetA()*(GetA()-2.*P-1))*0.25+H*0.5);
  G4double Ej = std::max(0.0,U + GetBindingEnergy() - GetA()*(GetA()+1.)*0.25);

  // g = 0.595*a*A
//  G4EvaporationLevelDensityParameter theLDP;
  G4double a;
  G4double g0 = 0.595*aFragment.GetA()*(a=dLevelDensity);
//      theLDP.LevelDensityParameter(aFragment.GetA(),aFragment.GetZ(),U);
  G4double g1 = 0.595*GetRestA()*a;
//      theLDP.LevelDensityParameter(GetRestA(),GetRestZ(),U);
  G4double gj = 0.595*GetA()*a;
//      theLDP.LevelDensityParameter(GetA(),GetZ(),U);
  G4double Probability = ((3.0/4.0)*sqrt(2.0/(GetReducedMass()*(eKin+GetBindingEnergy())))*
      GetAlpha()*(eKin+GetBeta(aFragment))/(r0*pow(GetRestA(),1.0/3.0))*
      CoalescenceFactor(aFragment.GetA())*FactorialFactor(N,P))*(pow((g1*E1)/(g0*E0),N-GetA()-1.0)*(g1/g0))*(pow((gj*Ej)/(g0*E0),GetA()-1.0)*(gj/g0)/E0);

  return Probability;
}








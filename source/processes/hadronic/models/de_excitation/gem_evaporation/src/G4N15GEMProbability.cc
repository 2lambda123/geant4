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
// $Id: G4N15GEMProbability.cc,v 1.1 2003-08-26 18:44:26 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999)
//


#include "G4N15GEMProbability.hh"

G4N15GEMProbability::G4N15GEMProbability() :
  G4GEMProbability(15,7,1.0/2.0) // A,Z,Spin
{
  ExcitEnergies.push_back(5270.40*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(1.80e-12*s);

  ExcitEnergies.push_back(5298.87*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(1.7E-14*s);

  ExcitEnergies.push_back(6323.85*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(0.15E-15*s);

  ExcitEnergies.push_back(7155.36*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(0.019e-12*s);

  ExcitEnergies.push_back(7301.09*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(0.17E-15*s);

  ExcitEnergies.push_back(7567.1*keV);
  ExcitSpins.push_back(7.0/2.0);
  ExcitLifetimes.push_back(0.040e-12*s);
 
  ExcitEnergies.push_back(8312.79*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(0.014e-12*s);

  ExcitEnergies.push_back(8571.4*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(0.07e-12*s);
 
  ExcitEnergies.push_back(9050.0*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(0.07e-12*s);
 
  ExcitEnergies.push_back(9152.24*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(0.028e-12*s);

  ExcitEnergies.push_back(9155.27*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(0.007e-12*s);

  ExcitEnergies.push_back(9225*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(0.09e-12*s);

  ExcitEnergies.push_back(9829*keV);
  ExcitSpins.push_back(7.0/2.0);
  ExcitLifetimes.push_back(0.13e-12*s);

  ExcitEnergies.push_back(9928*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(0.07e-12*s);

  ExcitEnergies.push_back(10449.7*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(0.5*keV));

  ExcitEnergies.push_back(10701.9*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(0.2*keV));

  ExcitEnergies.push_back(10804*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(1E-3*keV));

  ExcitEnergies.push_back(11235*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(3.3*keV)); 

  ExcitEnergies.push_back(11292.9*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(8*keV));   

  ExcitEnergies.push_back(11437.5*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(41.4*keV));

  ExcitEnergies.push_back(11615*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(405*keV));

  ExcitEnergies.push_back(11778*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(40*keV));   

  ExcitEnergies.push_back(11876*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(25*keV));

  ExcitEnergies.push_back(11942*keV);
  ExcitSpins.push_back(11.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(3.0*keV));

  ExcitEnergies.push_back(11965*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(17*keV)); 

  ExcitEnergies.push_back(12095*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(14*keV)); 

  ExcitEnergies.push_back(12145*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(47*keV)); 

  ExcitEnergies.push_back(12327*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(22*keV)); 

  ExcitEnergies.push_back(12493*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(40*keV)); 

  ExcitEnergies.push_back(12522*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(58*keV)); 

  ExcitEnergies.push_back(12920*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(56*keV)); 

  ExcitEnergies.push_back(12940*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(81*keV)); 

  ExcitEnergies.push_back(13173*keV);
  ExcitSpins.push_back(9.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(7*keV));  

  ExcitEnergies.push_back(13362*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(16*keV)); 

  ExcitEnergies.push_back(13390*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(56*keV)); 

  ExcitEnergies.push_back(13537*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(85*keV)); 

  ExcitEnergies.push_back(13608*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(18*keV)); 

  ExcitEnergies.push_back(13612*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(90*keV)); 

  ExcitEnergies.push_back(13840*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(75*keV)); 

  ExcitEnergies.push_back(13900*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(930*keV));

  ExcitEnergies.push_back(14100*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(100*keV));

  ExcitEnergies.push_back(14162*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(27*keV)); 

  ExcitEnergies.push_back(14240*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(150*keV));

  ExcitEnergies.push_back(14380*keV);
  ExcitSpins.push_back(7.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(100*keV));

  ExcitEnergies.push_back(16667*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(90*keV)); 

  ExcitEnergies.push_back(16850*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(110*keV));

  ExcitEnergies.push_back(19160*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(130*keV));

  ExcitEnergies.push_back(19500*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(400*keV));

  ExcitEnergies.push_back(20500*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(hbar_Planck*log(2.0)/(400*keV));

  SetExcitationEnergiesPtr(&ExcitEnergies);
  SetExcitationSpinsPtr(&ExcitSpins);
  SetExcitationLifetimesPtr(&ExcitLifetimes);
}


G4N15GEMProbability::G4N15GEMProbability(const G4N15GEMProbability &) : G4GEMProbability()
{
  G4Exception("G4N15GEMProbability::copy_constructor meant to not be accessable");
}




const G4N15GEMProbability & G4N15GEMProbability::
operator=(const G4N15GEMProbability &)
{
  G4Exception("G4N15GEMProbability::operator= meant to not be accessable");
  return *this;
}


G4bool G4N15GEMProbability::operator==(const G4N15GEMProbability &) const
{
  return false;
}

G4bool G4N15GEMProbability::operator!=(const G4N15GEMProbability &) const
{
  return true;
}




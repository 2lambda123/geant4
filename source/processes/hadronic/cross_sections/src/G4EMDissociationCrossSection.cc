// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:		G4EMDissociationCrossSection.cc
//
// Version:		B.1
// Date:		15/04/04
// Author:		P R Truscott
// Organisation:	QinetiQ Ltd, UK
// Customer:		ESA/ESTEC, NOORDWIJK
// Contract:		17191/03/NL/LvH
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// CHANGE HISTORY
// --------------
//
// 17 October 2003, P R Truscott, QinetiQ Ltd, UK
// Created.
//
// 15 March 2004, P R Truscott, QinetiQ Ltd, UK
// Beta release
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
// * This  code  implementation  is  the  intellectual   property  of *
// * QinetiQ.   Rights  to use  this  software  are  granted  to  the *
// * European Space Agency  under  the  provisions  of  Clause 39  of *
// * the    Standard    Conditions   for  Contract.   ESA    contract *
// * 17191/03/NL/LvH  provides  the  rights to distribute source code *
// * through  the  Geant4 Collaboration Agreement for general  public *
// * use.  Some elements of this source code may be derived from code *
// * developed   by   other   member   institutes   of   the   Geant4 *
// * Collaboration, and the provision of  their code under the Geant4 *
// * Collaboration is acknowledged.                                   *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
////////////////////////////////////////////////////////////////////////////////
//
#include "G4EMDissociationCrossSection.hh"
#include "G4PhysicsFreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "globals.hh"
////////////////////////////////////////////////////////////////////////////////
//
G4EMDissociationCrossSection::G4EMDissociationCrossSection ()
{
//
//
// This function makes use of the class which can sample the virtual photon
// spectrum, G4EMDissociationSpectrum.
//
  thePhotonSpectrum = new G4EMDissociationSpectrum();
//
//
// Define other constants.
//
  r0      = 1.18 * fermi;
  J       = 36.8 * MeV;
  Qprime  = 17.0 * MeV;
  epsilon = 0.0768;
  xd      = 0.25;
}
////////////////////////////////////////////////////////////////////////////////
//
G4EMDissociationCrossSection::~G4EMDissociationCrossSection()
{
  delete thePhotonSpectrum;
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool G4EMDissociationCrossSection::IsApplicable
  (const G4DynamicParticle *theDynamicParticle, const G4Element* theElement)
{
//
//
// The condition for the applicability of this class is that the projectile
// must be an ion and the target must have more than one nucleon.  In reality
// the value of A for either the projectile or target could be much higher,
// since for cases where both he projectile and target are medium to small
// Z, the probability of the EMD process is, I think, VERY small.
//
  if (G4ParticleTable::GetParticleTable()->GetIonTable()->
    IsIon(theDynamicParticle->GetDefinition()) && theElement->GetN() > 1.0)
    return true;
  else
    return false;
}
////////////////////////////////////////////////////////////////////////////////
//
G4double G4EMDissociationCrossSection::GetCrossSection
  (const G4DynamicParticle *theDynamicParticle, const G4Element* theElement,
  G4double temperature)
{
//
//
// Get relevant information about the projectile and target (A, Z) and
// velocity of the projectile.
//
  G4ParticleDefinition *definitionP = theDynamicParticle->GetDefinition();
  G4double AP   = definitionP->GetBaryonNumber();
  G4double ZP   = definitionP->GetPDGCharge();
  G4double b    = theDynamicParticle->Get4Momentum().beta();
//  G4double bsq  = b * b;
  
  G4double AT   = theElement->GetN();
  G4double ZT   = theElement->GetZ();
  G4double bmin = thePhotonSpectrum->GetClosestApproach(AP, ZP, AT, ZT, b);
//
//
// Calculate the cross-section for the projectile and then the target.  The
// information is returned in a G4PhysicsFreeVector, which separates out the
// cross-sections for the E1 and E2 moments of the virtual photon field, and
// the energies (GDR and GQR).
//
  G4PhysicsFreeVector *theProjectileCrossSections = 
    GetCrossSectionForProjectile (AP, ZP, AT, ZT, b, bmin);
  G4double crossSection =
    (*theProjectileCrossSections)[0]+(*theProjectileCrossSections)[1];
  delete theProjectileCrossSections;
  G4PhysicsFreeVector *theTargetCrossSections = 
    GetCrossSectionForTarget (AP, ZP, AT, ZT, b, bmin);
  crossSection += 
    (*theTargetCrossSections)[0]+(*theTargetCrossSections)[1];
  delete theTargetCrossSections;

  return crossSection;
}
////////////////////////////////////////////////////////////////////////////////
//
G4PhysicsFreeVector *
  G4EMDissociationCrossSection::GetCrossSectionForProjectile (G4double AP,
  G4double ZP, G4double AT, G4double ZT, G4double b, G4double bmin)
{
//
//
// Use Wilson et al's approach to calculate the cross-sections due to the E1
// and E2 moments of the field at the giant dipole and quadrupole resonances
// respectively,  Note that the algorithm is traditionally applied to the
// EMD break-up of the projectile in the field of the target, as is implemented
// here.
//
// Initialise variables and calculate the energies for the GDR and GQR.
//
  G4double AProot3 = pow(AP,1.0/3.0);
  G4double u       = 3.0 * J / Qprime / AProot3;
  G4double R0      = r0 * AProot3;
  G4double E_GDR  = hbarc / sqrt(0.7*amu_c2*R0*R0/8.0/J*
    (1.0 + u - (1.0 + epsilon + 3.0*u)/(1.0 + epsilon + u)*epsilon));
  G4double E_GQR  = 63.0 * MeV / AProot3;
//
//
// Determine the virtual photon spectra at these energies.
//
  G4double ZTsq = ZT * ZT;
  G4double nE1 = ZTsq *
    thePhotonSpectrum->GetGeneralE1Spectrum(E_GDR, b, bmin);
  G4double nE2 = ZTsq *
    thePhotonSpectrum->GetGeneralE2Spectrum(E_GQR, b, bmin);
//
//
// Now calculate the cross-section of the projectile for interaction with the
// E1 and E2 fields.
//
  G4double sE1 = 60.0 * millibarn * MeV * (AP-ZP)*ZP/AP;
  G4double sE2 = 0.22 * microbarn / MeV * ZP * AProot3 * AProot3;
  if (AP > 100.0)     sE2 *= 0.9;
  else if (AP > 40.0) sE2 *= 0.6;
  else                sE2 *= 0.3;
//
//
// ... and multiply with the intensity of the virtual photon spectra to get
// the probability of interaction.
//
  G4PhysicsFreeVector *theCrossSectionVector = new G4PhysicsFreeVector(2);
  theCrossSectionVector->PutValue(0, E_GDR, sE1*nE1);
  theCrossSectionVector->PutValue(1, E_GQR, sE2*nE2*E_GQR*E_GQR);
  
  return theCrossSectionVector;
}
////////////////////////////////////////////////////////////////////////////////
//
G4PhysicsFreeVector *
  G4EMDissociationCrossSection::GetCrossSectionForTarget (G4double AP,
  G4double ZP, G4double AT, G4double ZT, G4double b, G4double bmin)
{
//
//
// This is a cheaky little member function to calculate the probability of
// EMD for the target in the field of the projectile ... just by reversing the
// A and Z's for the participants.
//
  return GetCrossSectionForProjectile (AT, ZT, AP, ZP, b, bmin);
}
////////////////////////////////////////////////////////////////////////////////
//
G4double
  G4EMDissociationCrossSection::GetWilsonProbabilityForProtonDissociation
  (G4double A, G4double Z)
{
//
//
// This is a simple algorithm to choose whether a proton or neutron is ejected
// from the nucleus in the EMD interaction.
//
  G4double p = 0.0;
  if (Z < 6.0)
    p = 0.5;
  else if (Z < 8.0)
    p = 0.6;
  else if (Z < 14.0)
    p = 0.7;
  else
  {
    G4double p1 = (G4double) Z / (G4double) A;
    G4double p2 = 1.95*exp(-0.075*Z);
    if (p1 < p2) p = p1;
    else         p = p2;
  }

  return p;
}
////////////////////////////////////////////////////////////////////////////////
//

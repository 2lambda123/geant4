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
// $Id: G4CoulombBarrier.cc,v 1.3 2004-12-07 13:48:10 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Dec 1999)


#include "G4CoulombBarrier.hh"
#include "G4HadronicException.hh"
#include <strstream>

G4CoulombBarrier::G4CoulombBarrier(const G4CoulombBarrier & ) : G4VCoulombBarrier()
{
  throw G4HadronicException(__FILE__, __LINE__, "G4CoulombBarrier::copy_constructor meant to not be accessable.");
}


const G4CoulombBarrier & G4CoulombBarrier::operator=(const G4CoulombBarrier & )
{
  throw G4HadronicException(__FILE__, __LINE__, "G4CoulombBarrier::operator= meant to not be accessable.");
  return *this;
}

G4bool G4CoulombBarrier::operator==(const G4CoulombBarrier & ) const 
{
  return false;
}

G4bool G4CoulombBarrier::operator!=(const G4CoulombBarrier & ) const 
{
  return true;
}



G4double G4CoulombBarrier::GetCoulombBarrier(const G4int ARes, const G4int ZRes, const G4double U) const 
  // Calculation of Coulomb potential energy (barrier) for outgoing fragment
{
  G4double Barrier = 0.0;
  if (ZRes > ARes || ARes < 1) {
    char errMessage[1024];
    std::ostrstream errOs(errMessage,1024);
    errOs << "G4CoulombBarrier::GetCoulombBarrier: ";
    errOs << "Wrong values for ";
    errOs << "residual nucleus A = " << ARes << " ";
    errOs << "and residual nucleus Z = " << ZRes << G4endl;
    throw G4HadronicException(__FILE__, __LINE__, errMessage);
  }
  if (GetA() == 1 && GetZ() == 0) {
    Barrier = 0.0;   // Neutron Coulomb Barrier is 0
  } else {
    G4double CompoundRadius = CalcCompoundRadius(static_cast<G4double>(ZRes));
    Barrier = elm_coupling/CompoundRadius * static_cast<G4double>(GetZ())*static_cast<G4double>(ZRes)/
      (std::pow(static_cast<G4double>(GetA()),1./3.) + std::pow(static_cast<G4double>(ARes),1./3.));

    // Barrier penetration coeficient
    G4double K = BarrierPenetrationFactor(ZRes);
		
    Barrier *= K;
		
    Barrier /= (1.0 + std::sqrt(U/(2.0*static_cast<G4double>(ARes))));
  }
  return Barrier;
}




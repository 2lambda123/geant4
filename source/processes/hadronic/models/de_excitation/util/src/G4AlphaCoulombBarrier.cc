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
// $Id: G4AlphaCoulombBarrier.cc,v 1.1 2003-08-26 18:50:38 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Dec 1999)


#include "G4AlphaCoulombBarrier.hh"

G4AlphaCoulombBarrier::G4AlphaCoulombBarrier(const G4AlphaCoulombBarrier & ) : G4CoulombBarrier()
{
    G4Exception("G4AlphaCoulombBarrier::copy_constructor meant to not be accessable.");
}


const G4AlphaCoulombBarrier & G4AlphaCoulombBarrier::operator=(const G4AlphaCoulombBarrier & )
{
    G4Exception("G4AlphaCoulombBarrier::operator= meant to not be accessable.");
    return *this;
}

G4bool G4AlphaCoulombBarrier::operator==(const G4AlphaCoulombBarrier & ) const 
{
    return false;
}

G4bool G4AlphaCoulombBarrier::operator!=(const G4AlphaCoulombBarrier & ) const 
{
    return true;
}


G4double G4AlphaCoulombBarrier::BarrierPenetrationFactor(const G4double aZ) const
{
    // Data comes from 
    // Dostrovsky, Fraenkel and Friedlander
    // Physical Review, vol 116, num. 3 1959
    // 
    // const G4int size = 5;
    // const G4double Zlist[size]  = {10.0, 20.0, 30.0, 50.0, 70.0};
    // const G4double Kalpha[size] = {0.68, 0.82, 0.91, 0.97, 0.98};
    G4double K = 1.0;
    if (aZ>=70.0) {
	K = 0.98;
    } else {
	K = (((0.23684e-5*aZ) - 0.42143e-3)*aZ + 0.25222e-1)*aZ + 0.46699;
    }
    return K;
}

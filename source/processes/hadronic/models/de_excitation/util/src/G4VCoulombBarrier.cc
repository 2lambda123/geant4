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
// $Id: G4VCoulombBarrier.cc,v 1.1 2003-08-26 18:50:47 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Dec 1999)


#include "G4VCoulombBarrier.hh"
#include <strstream>

G4VCoulombBarrier::G4VCoulombBarrier(const G4int anA, const G4int aZ)
{
    if (anA >= aZ && anA > 0) {
	theA = anA;
	theZ = aZ;
    } else {
	char errMessage[1024];
	std::ostrstream errOs(errMessage,1024);
	errOs << "G4VCoulombBarrier::G4VCoulombBarrier: ";
	errOs << "Wrong values for ";
	errOs << "A = " << anA << " ";
	errOs << "and Z = " << aZ << G4endl;
	G4Exception(errMessage);
    }
}


G4VCoulombBarrier::G4VCoulombBarrier(const G4VCoulombBarrier & )
{
    G4Exception("G4VCoulombBarrier::copy_constructor meant to not be accessable.");
}


const G4VCoulombBarrier & G4VCoulombBarrier::operator=(const G4VCoulombBarrier & )
{
    G4Exception("G4VCoulombBarrier::operator= meant to not be accessable.");
    return *this;
}

G4bool G4VCoulombBarrier::operator==(const G4VCoulombBarrier & ) const 
{
    return false;
}

G4bool G4VCoulombBarrier::operator!=(const G4VCoulombBarrier & ) const 
{
    return true;
}


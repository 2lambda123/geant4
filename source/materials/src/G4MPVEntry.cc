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
// $Id: G4MPVEntry.cc,v 1.6 2001-07-11 10:01:28 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
////////////////////////////////////////////////////////////////////////
// G4MPVEntry Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File:        G4MPVEntry.cc
// Version:     1.0
// Created:     1996-02-08
// Author:      Juliet Armstrong
// Updated:     1997-03-25 by Peter Gumplinger
//              > cosmetics (only)
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#include "G4MPVEntry.hh"

/////////////////////////
// Class Implementation
/////////////////////////

        //////////////
        // Operators
        //////////////

// Overload the == operator
// ------------------------
// Well defined == semantics required by G4RWTPtrSortedVector
//
G4bool G4MPVEntry::operator ==(const G4MPVEntry &right) const  
{
	if (thePhotonMomentum == right.thePhotonMomentum) 
		return true;
	else
		return false; 
}

// Overload the < operator
// -----------------------
// Well defined < semantics required by G4RWTPtrSortedVector
//
G4bool G4MPVEntry::operator <(const G4MPVEntry &right) const  
{
	if (thePhotonMomentum < right.thePhotonMomentum) 
		return true;
	else
		return false;
}

// Overload the = operator
// -----------------------
// Well defined = semantics required by G4RWTPtrSortedVector
//
G4MPVEntry& G4MPVEntry::operator =(const G4MPVEntry& right)
{
        if (this == &right) return *this;
	
        thePhotonMomentum = right.thePhotonMomentum;
        theProperty = right.theProperty;
        return *this;
}

        /////////////////
        // Constructors
        /////////////////

G4MPVEntry::G4MPVEntry(G4double aPhotonMomentum, G4double aProperty)
{
        thePhotonMomentum = aPhotonMomentum;
        theProperty = aProperty;
}

G4MPVEntry::G4MPVEntry(const G4MPVEntry &right)
{
        thePhotonMomentum = right.thePhotonMomentum;
        theProperty = right.theProperty;
}


        ////////////////
        // Destructors
        ////////////////

G4MPVEntry::~G4MPVEntry(){}

        ////////////
        // Methods
        ////////////

void G4MPVEntry::DumpEntry()
{
	G4cout << "(" 
	     << thePhotonMomentum 
	     << ", " 
	     << theProperty
 	     << ")" 
	     << G4endl;	
}

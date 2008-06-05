//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4MaterialPropertyVector.hh,v 1.11 2008-06-05 23:37:37 gum Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
////////////////////////////////////////////////////////////////////////
// G4MaterialPropertyVector Class Definition
////////////////////////////////////////////////////////////////////////
//
// File:        G4MaterialPropertyVector.hh
//
// Description: A one-to-one mapping from Photon Energy to some
//              optical property 
// Version:     1.0
// Created:     1996-02-08
// Author:      Juliet Armstrong
// Updated:     1999-10-29 add method and class descriptors
//              1997-03-25 by Peter Gumplinger
//              > value.h -> templates.hh
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#ifndef G4MaterialPropertyVector_h   
#define G4MaterialPropertyVector_h 1

/////////////
// Includes
/////////////

#include "G4MPVEntry.hh"
#include <vector>
#include <functional>

// Class Description:
// A one-to-one mapping from Photon Energy to some optical property.
// Class Description - End:

/////////////////////
// Class Definition
/////////////////////

class G4MaterialPropertyVector {

	struct MPVEntry_less
	  : public std::binary_function<G4MPVEntry*, G4MPVEntry*, G4bool>
	{
	  G4bool operator()(G4MPVEntry* x, G4MPVEntry* y) { return *x < *y; }
	};

public: // Without description

	//////////////
	// Operators
	//////////////

	G4bool operator ++();
        G4MaterialPropertyVector&
                   operator =(const G4MaterialPropertyVector &right);

	/////////////////
	// Constructors
	/////////////////

	G4MaterialPropertyVector() : MPV(0) 
	{
		CurrentEntry = -1;
		NumEntries   = 0;
	};

public: // With description
	
	G4MaterialPropertyVector(G4double *PhotonEnergies, 
		  	   	 G4double *PropertyValues,
				 G4int     NumElements);
        // Constructor of G4MaterialPropertyVector object.

public: // Without description

	G4MaterialPropertyVector(const G4MaterialPropertyVector &right);

        ///////////////
        // Destructor
        ///////////////

	~G4MaterialPropertyVector();

        ////////////
        // Methods
        ////////////

public: // With description

	void ResetIterator();

        void AddElement(G4double aPhotonEnergy, 
			G4double aPropertyValue);
        // Add a new element (pair of numbers) to the G4MaterialPropertyVector.
	void RemoveElement(G4double aPhotonEnergy);
        // Remove the element with given x-value.

        G4double GetProperty(G4double aPhotonEnergy) const;
        // Returns the y-value for given x-value (with interpolation).
	G4double GetPhotonEnergy(G4double aProperty) const;
        // Returns the x-value for given y-value (with interpolation).
        // NOTE: Assumes that the y-value is an increasing function of 
        //       the x-value. Returns the x-value corresponding to the 
        //       y-value passed in. If several x-values correspond to 
        //       the y-value passed in, the method returns the first 
        //       x-value in the vector that corresponds to that value.
	// For use with G4MaterialPropertyVector iterator: return
        // property (or Photon Energy) at current point of iterator.

	G4double GetProperty() const;
	G4double GetPhotonEnergy() const;

	G4double GetMaxProperty() const;
	G4double GetMinProperty() const;
	G4double GetMaxPhotonEnergy() const;
	G4double GetMinPhotonEnergy() const;
		
	//////////
	// Tests
	//////////

	void DumpVector();	

private:

	/////////////////////
	// Helper Functions
	/////////////////////

	G4MPVEntry GetEntry(G4int i) const;

	void GetAdjacentBins(G4double aPhotonEnergy,
                             G4int *left,G4int *right) const;

	/////////////////////////
        // Private Data Members
	/////////////////////////

        std::vector<G4MPVEntry*> MPV;
	G4int NumEntries;
	G4int CurrentEntry;
};

///////////////////
// Inline methods
///////////////////

inline 
G4double G4MaterialPropertyVector::GetMaxProperty() const
{
  return MPV.back()->GetProperty();
}

inline
G4double G4MaterialPropertyVector::GetMinProperty() const
{
  return MPV.front()->GetProperty();
}

inline
G4double G4MaterialPropertyVector::GetMaxPhotonEnergy() const
{
  return MPV.back()->GetPhotonEnergy();
}

inline 
G4double G4MaterialPropertyVector::GetMinPhotonEnergy() const
{
  return MPV.front()->GetPhotonEnergy();
}

#endif /* G4MaterialPropertyVector_h */

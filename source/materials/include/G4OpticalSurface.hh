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
// $Id: G4OpticalSurface.hh,v 1.7 2003-12-01 15:13:41 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
////////////////////////////////////////////////////////////////////////
// G4OpticalSurface Definition
////////////////////////////////////////////////////////////////////////
//
// File:        G4OpticalSurface.hh
// Description: A optical surface class for use in G4OpBoundaryProcess
// Version:     2.0
// Created:     1997-06-26
// Author:      Peter Gumplinger
// Updated:     1999-10-29 add method and class descriptors
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#ifndef G4OpticalSurface_h
#define G4OpticalSurface_h 1

/////////////
// Includes
/////////////

#include "G4Types.hh"
#include "G4SurfaceProperty.hh"

// Class Description:
// A optical surface class for use in the G4OpBoundaryProcess class.
// Contains the enumerations: G4OpticalSurfaceFinish, G4OpticalSurfaceType,
// and G4OpticalSurfaceModel.
// Class Description - End:

enum G4OpticalSurfaceFinish
{
   polished,                    // smooth perfectly polished surface
   polishedfrontpainted,        // smooth top-layer (front) paint
   polishedbackpainted,         // same is 'polished' but with a back-paint
   ground,                      // rough surface
   groundfrontpainted,          // rough top-layer (front) paint
   groundbackpainted            // same as 'ground' but with a back-paint
};

enum G4OpticalSurfaceModel
{
   glisur,                      // original GEANT3 model
   unified                      // UNIFIED model
};

class G4MaterialPropertiesTable;

/////////////////////
// Class Definition
/////////////////////

class G4OpticalSurface : public G4SurfaceProperty
{

public: // Without description
  
        //////////////
        // Operators
        //////////////
  
	G4OpticalSurface(const G4OpticalSurface &right);
	const G4OpticalSurface & operator=(const G4OpticalSurface &right);
  
	G4int operator==(const G4OpticalSurface &right) const;
	G4int operator!=(const G4OpticalSurface &right) const;

public: // With description

        ////////////////////////////////
        // Constructors and Destructor
        ////////////////////////////////

	G4OpticalSurface(const G4String& name,
			 G4OpticalSurfaceModel model = glisur,
			 G4OpticalSurfaceFinish finish = polished,
			 G4SurfaceType type = dielectric_dielectric,
			 G4double value = 1.0);
        // Constructor of an optical surface object.

public: // Without description

	~G4OpticalSurface();

	////////////
	// Methods
        ////////////

	// public methods

public: // With description

        G4OpticalSurfaceFinish GetFinish() const {return theFinish;};
        // Returns the optical surface finish.
        void         SetFinish(const G4OpticalSurfaceFinish finish)
						 {theFinish = finish;};
        // Sets the optical surface finish.

        G4OpticalSurfaceModel GetModel() const {return theModel;};
        // Returns the optical surface model used.
        void           SetModel(const G4OpticalSurfaceModel model)
						   {theModel = model;};
        // Sets the optical surface model to be followed.

	G4double GetSigmaAlpha() const {return sigma_alpha;};
        // Returns an unified model surface parameter.
	void     SetSigmaAlpha(const G4double s_a)
				        {sigma_alpha = s_a;};
        // Sets an unified model surface parameter.

	G4double GetPolish() const {return polish;};
        // Returns the optical surface polish type.
	void     SetPolish(const G4double plsh) {polish=plsh;};
        // Sets the optical surface polish type.

	G4MaterialPropertiesTable* GetMaterialPropertiesTable() const
				       { return theMaterialPropertiesTable;};
        // Retrieves the pointer of the G4MaterialPropertiesTable 
        // attached to optical surface.

	void SetMaterialPropertiesTable(G4MaterialPropertiesTable *anMPT)
				    { theMaterialPropertiesTable = anMPT;};
        // Attaches a G4MaterialPropertiesTable to the optical surface.

	void DumpInfo() const;
        // Prints information about the optical surface.

private:

// ------------------
// Basic data members ( To define an optical surface)
// ------------------

        G4OpticalSurfaceModel theModel;		// Surface model
        G4OpticalSurfaceFinish theFinish;	// Surface finish

	G4double sigma_alpha;		// The sigma of micro-facet polar angle
	G4double polish;		// Polish parameter in glisur model

	G4MaterialPropertiesTable* theMaterialPropertiesTable;

};

////////////////////
// Inline methods
////////////////////

#endif /* G4OpticalSurface_h */

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
// $Id: G4UniformMagField.hh,v 1.7 2001-12-04 17:35:52 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// class G4UniformMagField
//
// Class description:
//
// Class for creation of Uniform Magnetic Field.

// History:
// - 30.01.97 V.Grichine, Created.
// - 01.08.97 J.Apostolakis, cleanup, new 3-vector constructor, 
//            and removal of helix-stepper (to separate file).
// - 05.11.97 G.Cosmo, added copy constructor and assignment operator.

#ifndef G4UNIFORMMAGFIELD_HH
#define G4UNIFORMMAGFIELD_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4Mag_EqRhs.hh"

class G4UniformMagField : public G4MagneticField
{
  public:  // with description
  
    G4UniformMagField(const G4ThreeVector& FieldVector );
      // A field with value equal to FieldVector.

    G4UniformMagField(G4double vField,
	              G4double vTheta,
		      G4double vPhi     ) ;
		       
    virtual ~G4UniformMagField() ;

    G4UniformMagField(const G4UniformMagField &p);
    G4UniformMagField& operator = (const G4UniformMagField &p);
      // Copy constructor and assignment operator.

    virtual void GetFieldValue(const G4double yTrack[4],
	                             G4double *MagField) const ;

    void SetFieldValue(const G4ThreeVector& newFieldValue);

    G4ThreeVector GetConstantFieldValue() const;
      // Return the field value  

  private:

    G4double fFieldComponents[3] ;
};

#endif



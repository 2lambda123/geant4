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
// $Id: F01ElectroMagneticField.hh,v 1.5 2003-11-12 11:06:12 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//    A class for control of the Magnetic Field of the detector.
//  The field is assumed to be uniform.
// 
//  $ Id:  $

// Should this be a:
//    i) messenger
//   ii) user class that creates the field       ? 
//  iii) simply a derived class of Uniform field ?  <== I have chosen this now.
//   iv) a field manager that creates/updates field    (Prefered?)
//


#ifndef F01ElectroMagneticField_H
#define F01ElectroMagneticField_H

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"

class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;
class F01FieldMessenger;

class F01ElectroMagneticField: public G4MagneticField
{
public:
  F01ElectroMagneticField(G4ThreeVector) ;  //  The value of the field
  F01ElectroMagneticField() ;               //  A zero field

 ~F01ElectroMagneticField() ;  
      
  void  GetFieldValue( const  G4double*,
			      G4double* ) const {}
  
  void SetStepperType( G4int i) { fStepperType = i ; }

  void SetStepper();

  void SetMinStep(G4double s) { fMinStep = s ; }

  void UpdateField();

  void SetFieldValue(G4ThreeVector fieldVector) ;
  void SetFieldValue(G4double      fieldValue) ;
  G4ThreeVector GetConstantFieldValue();

protected:

      // Find the global Field Manager

  G4FieldManager*         GetGlobalFieldManager() ;   // static 

  G4FieldManager*         fFieldManager ;
  G4ChordFinder*          fChordFinder ;
  G4Mag_UsualEqRhs*       fEquation ; 
  G4MagneticField*        fMagneticField ; 

  G4MagIntegratorStepper* fStepper ;
  G4int                   fStepperType ;

  G4double                fMinStep ;
 
  F01FieldMessenger*      fFieldMessenger;

};

#endif

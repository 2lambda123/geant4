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
// $Id: G4EquationOfMotion.hh,v 1.8 2003-10-31 14:35:51 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4EquationOfMotion
//
// Class description:
//
// Abstract Base Class for the right hand size of the equation of
// motion of a particle in a field.

// History:
// - Created. J.Apostolakis
// -------------------------------------------------------------------

#ifndef G4_EquationOfMotion_DEF
#define G4_EquationOfMotion_DEF

#include "globals.hh"
#include "G4Field.hh"   // required in inline method implementations

class G4EquationOfMotion 
{
  public:  // with description

     G4EquationOfMotion( G4Field *Field );
     virtual ~G4EquationOfMotion();
       // Constructor and virtual destructor. No operations.

     virtual void EvaluateRhsGivenB( const  G4double y[],
                                     const  G4double B[3],
                                     G4double dydx[] ) const = 0;
       // Given the value of the  field "B", this function 
       // calculates the value of the derivative dydx.
       // --------------------------------------------------------
       // This is the _only_ function a subclass must define.
       // The other two functions use Rhs_givenB.

     virtual void SetChargeMomentumMass(G4double particleCharge, // in e+ units
                                        G4double MomentumXc,
                                        G4double MassXc2) = 0;
       // Set the charge, momentum and mass of the current particle
       // --> used to set the equation's coefficients ...

     inline
     void RightHandSide( const  G4double y[],
                                G4double dydx[] ) const;
       // This calculates the value of the derivative dydx at y.
       // It is the usual enquiry function.
       // ---------------------------
       // (It is not virtual, but calls the virtual function above.)

     void EvaluateRhsReturnB( const  G4double y[],
                              G4double dydx[],
                              G4double Field[]  ) const;
       // Same as RHS above, but also returns the value of B.
       // Should be made the new default ? after putting dydx & B in a class.

     void GetFieldValue( const  G4double Point[4],
                                G4double Field[] )  const;
       // Obtain only the field - the stepper assumes it is pure Magnetic.
       // Not protected, because G4RKG3_Stepper uses it directly.

     const G4Field* GetFieldObj() const;
     void           SetFieldObj(G4Field* pField);

  private:
     // static const int G4maximum_number_of_field_components = 16;
     enum { G4maximum_number_of_field_components = 16 } ;

     G4Field *itsField;

};

#include "G4EquationOfMotion.icc"

#endif /* G4_EquationOfMotion_DEF */

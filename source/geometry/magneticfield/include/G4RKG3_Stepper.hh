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
// $Id: G4RKG3_Stepper.hh,v 1.10 2003-10-31 14:35:52 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4RKG3_Stepper
//
// Class description:
//
// Integrator Runga-Kutta Stepper from Geant3.

// History:
// - Created. J.Apostolakis, V.Grichine - 30.01.97
// -------------------------------------------------------------------

#ifndef G4RKG3_Stepper_hh
#define G4RKG3_Stepper_hh

#include "G4Types.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ThreeVector.hh"

class G4Mag_EqRhs;

class G4RKG3_Stepper : public G4MagIntegratorStepper
{
  public:  // with description

    G4RKG3_Stepper(G4Mag_EqRhs *EqRhs);
      // Integrate over 6 variables only:  position & velocity.
      // Not implemented yet !

    ~G4RKG3_Stepper();

    void Stepper( const G4double yIn[],
                  const G4double dydx[],
                        G4double h,
                        G4double yOut[],
                        G4double yErr[]  );
      // The method which must be provided, even if less efficient.

    G4double  DistChord() const ;
 
    void StepNoErr( const G4double tIn[7],
                    const G4double dydx[7],
                          G4double Step,
                          G4double tOut[7],
                          G4double B[3] );
      // Integrator RK Stepper from G3 with only two field evaluation per 
      // Step. It is used in propagation initial Step by small substeps
      // after solution error and delta geometry considerations. 
      // B[3] is magnetic field which is passed from substep to substep.

    void StepWithEst( const G4double  tIn[7],
                      const G4double dydx[7],
                            G4double Step,
                            G4double tOut[7],
                            G4double& alpha2,
                            G4double& beta2,
                      const G4double B1[3],
                            G4double B2[3] );
      // Integrator for RK from G3 with evaluation of error in solution and delta
      // geometry based on naive similarity with the case of uniform magnetic field.
      // B1[3] is input  and is the first magnetic field values
      // B2[3] is output and is the final magnetic field values.

  public:  // without description

    G4int IntegratorOrder() const { return 4; }

  private:

    G4ThreeVector fyInitial,
                  fyMidPoint,
                  fyFinal;
};

#endif

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
// $Id: G4CashKarpRKF45.hh,v 1.8 2003-04-02 08:52:32 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4CashKarpRKF45
//
// Class description:
//
// The Cash-Karp Runge-Kutta-Fehlberg 4/5 method is an embedded fourth
// order method (giving fifth-order accuracy) for the solution of an ODE.
// Two different fourth order estimates are calculated; their difference
// gives an error estimate. [ref. Numerical Recipes in C, 2nd Edition]
// It is used to integrate the equations of the motion of a particle 
// in a magnetic field.

// History:
// - Created. J.Apostolakis, V.Grichine - 30.1.97

#ifndef G4CashKARP_RKF45
#define G4CashKARP_RKF45

#include "G4MagIntegratorStepper.hh"

class G4CashKarpRKF45 : public G4MagIntegratorStepper
{

  public:  // with description

    G4CashKarpRKF45( G4EquationOfMotion *EqRhs,
                     G4int numberOfVariables = 6,
                     G4bool primary= true ) ;
   ~G4CashKarpRKF45() ;

    void Stepper( const G4double y[],
                  const G4double dydx[],
                        G4double h,
                        G4double yout[],
                        G4double yerr[] ) ;

  public:  // without description

    G4double  DistChord()   const; 
    G4int IntegratorOrder() const { return 4; }

  private:

    void StepWithEst( const G4double yIn[],
                      const G4double dydx[],
                            G4double Step,
                            G4double yOut[],
                            G4double& alpha2,
                            G4double& beta2,
                      const G4double B1[],
                            G4double B2[]    );  
      // No longer used. Obsolete.

    G4CashKarpRKF45(const G4CashKarpRKF45&);
    G4CashKarpRKF45& operator=(const G4CashKarpRKF45&);
      // Private copy constructor and assignment operator.

  private:

   G4int fNumberOfVariables ;
   G4double *ak2, *ak3, *ak4, *ak5, *ak6, *ak7, *yTemp, *yIn;  // scratch space

  // for DistChord calculations

  G4double fLastStepLength;
  G4double *fLastInitialVector, *fLastFinalVector,
           *fLastDyDx, *fMidVector, *fMidError;
  G4CashKarpRKF45* fAuxStepper; 
};

#endif /* G4CashKARP_RKF45 */

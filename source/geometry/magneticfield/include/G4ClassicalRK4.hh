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
// $Id: G4ClassicalRK4.hh,v 1.7 2003-04-02 08:52:54 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4ClassicalRK4
//
// Class description:
//
// Integrate the equations of the motion of a particle in a magnetic field
// using the classical 4th Runge-Kutta method.

// History:
// - Created: J.Apostolakis, V.Grichine - 30.1.97
// - Moved into G4MagErrorStepper: W.Wander <wwc@mit.edu> - 12/09/97

#include "G4MagErrorStepper.hh"
#include "G4ThreeVector.hh"

class G4ClassicalRK4 : public G4MagErrorStepper 
{
  public:  // with description

    G4ClassicalRK4(G4Mag_EqRhs *EqRhs, G4int numberOfVariables = 6) ;

    ~G4ClassicalRK4() ;

    // A stepper that does not know about errors.
    // It is used by the MagErrorStepper stepper.
   
    void DumbStepper( const G4double  yIn[],
                      const G4double  dydx[],
                            G4double  h,
                            G4double  yOut[] ) ;
      // Given values for the variables y[0,..,n-1] and their derivatives
      // dydx[0,...,n-1] known at x, use the classical 4th Runge-Kutta
      // method to advance the solution over an interval h and return the
      // incremented variables as yout[0,...,n-1], which not be a distinct
      // array from y. The user supplies the routine RightHandSide(x,y,dydx),
      // which returns derivatives dydx at x. The source is routine rk4 from
      // NRC p. 712-713 .

  public:  // without description

    G4int IntegratorOrder() const { return 4; }

  private:

    void StepWithEst( const G4double  yIn[],
                      const G4double  dydx[],
                            G4double  h,
                            G4double  yOut[],
                            G4double& alpha2,
                            G4double& beta2,
                      const G4double B1[],
                            G4double B2[] );
      // No longer used. Obsolete.

    G4ClassicalRK4(const G4ClassicalRK4&);
    G4ClassicalRK4& operator=(const G4ClassicalRK4&);
      // Private copy constructor and assignment operator.

  private:

    // G4int fNumberOfVariables ; // is set default to 6 in constructor

    G4double *dydxm, *dydxt, *yt; // scratch space - not state 
};

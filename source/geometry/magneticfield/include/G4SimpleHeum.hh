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
// $Id: G4SimpleHeum.hh,v 1.6 2003-10-31 14:35:52 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// class G4SimpleHeum
//
// Class description:
//
// Simple Heum stepper for magnetic field:
//        x_1 = x_0  +
//              h * 1/4 * dx(t0,x0)  +
//                  3/4 * dx(t0+2/3*h, x0+2/3*h*(dx(t0+h/3,x0+h/3*dx(t0,x0)))) 
//
// Third order solver.

// History:
// - Created. W. Wander <wwc@mit.edu>, 12/09/97
// -------------------------------------------------------------------

#ifndef G4SIMPLEHEUM_HH
#define G4SIMPLEHEUM_HH

#include "G4MagErrorStepper.hh"

class G4SimpleHeum : public G4MagErrorStepper
{

  public:  // with description

    G4SimpleHeum(G4Mag_EqRhs *EqRhs, G4int num_variables=6);
   ~G4SimpleHeum();
      // Constructor and destructor.

    void DumbStepper( const G4double y[],
                      const G4double dydx[],
                            G4double h,
                            G4double yout[]);

  public:  // without description
  
    G4int IntegratorOrder() const { return 3; }

  private:

    G4int fNumberOfVariables;

    G4double* dydxTemp  ;
    G4double* dydxTemp2 ;
    G4double* yTemp     ;
    G4double* yTemp2    ;
      // scratch space    
};

#endif /* G4SIMPLEHEUM_HH */

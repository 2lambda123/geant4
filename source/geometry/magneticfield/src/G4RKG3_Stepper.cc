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
// $Id: G4RKG3_Stepper.cc,v 1.8 2003-10-24 13:34:17 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "G4RKG3_Stepper.hh"
#include "G4ThreeVector.hh"
#include "G4LineSection.hh"

G4RKG3_Stepper::G4RKG3_Stepper(G4Mag_EqRhs *EqRhs)
  : G4MagIntegratorStepper(EqRhs,6)
{
  G4Exception("G4RKG3_Stepper::G4RKG3_Stepper()", "NotImplemented",
              FatalException, "Stepper not yet available.");
}

G4RKG3_Stepper::~G4RKG3_Stepper()
{
}

void G4RKG3_Stepper::Stepper(  const G4double  yInput[7],
                               const G4double dydx[7],
                                     G4double Step,
                                     G4double yOut[7],
                                     G4double yErr[])
{
   G4double  B[3];
   //   G4double  yderiv[6];
   //   G4double  alpha2, beta2;
   G4int nvar = 6 ;
   //   G4double beTemp2, beta2=0;

   G4int i;
   G4double  by15 = 1. / 15. ; // was  0.066666666 ;
   G4double yTemp[7], dydxTemp[6], yIn[7] ;
   //  Saving yInput because yInput and yOut can be aliases for same array
   for(i=0;i<nvar;i++) yIn[i]=yInput[i];

   G4double h = Step * 0.5; 

   // Do two half steps


   // To obtain B1 ...
   // GetEquationOfMotion()->GetFieldValue(yIn,B);
   // G4RKG3_Stepper::StepWithEst(yIn, dydx, Step, yOut,alpha2, beta2, B1, B2 );

   StepNoErr(yIn, dydx,h, yTemp,B) ;
                                     //   RightHandSide(yTemp,dydxTemp) ;
   GetEquationOfMotion()->EvaluateRhsGivenB(yTemp,B,dydxTemp) ;  
   StepNoErr(yTemp,dydxTemp,h,yOut,B);              // ,beTemp2) ;
                                                     //   beta2 += beTemp2;
                                                     //   beta2 *= 0.5;

   // Store midpoint, chord calculation
                                 
   fyMidPoint = G4ThreeVector( yTemp[0],  yTemp[1],  yTemp[2]); 

   // Do a full Step

   h *= 2 ;
   StepNoErr(yIn,dydx,h,yTemp,B); // ,beTemp2) ;
   for(i=0;i<nvar;i++)
   {
      yErr[i] = yOut[i] - yTemp[i] ;
      yOut[i] += yErr[i]*by15 ;          // Provides 5th order of accuracy
   }

//    for(i=0;i<ncomp;i++)
//    {
//       fyInitial[i]  = yIn[i]; 
//       fyFinal[i]    = yOut[i]; 
//    }

   fyInitial = G4ThreeVector( yIn[0],   yIn[1],   yIn[2]); 
   fyFinal   = G4ThreeVector( yOut[0],  yOut[1],  yOut[2]); 
   //   beta2 += beTemp2 ;
   //   beta2 *= 0.5 ;   
   // NormaliseTangentVector( yOut );  // Deleted
}

// ---------------------------------------------------------------------------

// Integrator for RK from G3 with evaluation of error in solution and delta
// geometry based on naive similarity with the case of uniform magnetic field.
// B1[3] is input  and is the first magnetic field values
// B2[3] is output and is the final magnetic field values.

void G4RKG3_Stepper::StepWithEst( const G4double*,
                                  const G4double*,
                                        G4double,
                                        G4double*,
                                        G4double&,
                                        G4double&,
                                  const G4double*,
                                        G4double* )
   
{
  G4Exception("G4RKG3_Stepper::StepWithEst()", "ObsoleteMethod",
              FatalException, "Method no longer used.");
}

// -----------------------------------------------------------------

// Integrator RK Stepper from G3 with only two field evaluation per Step. 
// It is used in propagation initial Step by small substeps after solution 
// error and delta geometry considerations. B[3] is magnetic field which 
// is passed from substep to substep.

void G4RKG3_Stepper::StepNoErr(const G4double tIn[7],
                               const G4double dydx[7],
                                     G4double Step,
                                     G4double tOut[7],
                                     G4double B[3]      )     // const
   
{
  //  Copy and edit the routine above, to delete alpha2, beta2, ...
   G4double K1[7],K2[7],K3[7],K4[7] ;
   G4double tTemp[7], yderiv[6] ;
   G4int i ;

#ifdef END_CODE_G3STEPPER
   G4Exception(" G4RKG3_Stepper::StepNoErr(): method to be no longer used.");
#else
   // GetEquationOfMotion()->EvaluateRhsReturnB(tIn,dydx,B1) ;
   
   for(i=0;i<3;i++)
   {
      K1[i] = Step * dydx[i+3];
      tTemp[i] = tIn[i] + Step*(0.5*tIn[i+3] + 0.125*K1[i]) ;
      tTemp[i+3] = tIn[i+3] + 0.5*K1[i] ;
   }
   GetEquationOfMotion()->EvaluateRhsReturnB(tTemp,yderiv,B) ;
     //  Calculates yderiv & returns B too!

   for(i=0;i<3;i++)
   {
      K2[i] = Step * yderiv[i+3];
      tTemp[i+3] = tIn[i+3] + 0.5*K2[i] ;
   }

   //  Given B, calculate yderiv !
   GetEquationOfMotion()->EvaluateRhsGivenB(tTemp,B,yderiv) ;  
   
   for(i=0;i<3;i++)
   {
      K3[i] = Step * yderiv[i+3];
      tTemp[i] = tIn[i] + Step*(tIn[i+3] + 0.5*K3[i]) ;
      tTemp[i+3] = tIn[i+3] + K3[i] ;
   }

   //  Calculates y-deriv(atives) & returns B too!
   GetEquationOfMotion()->EvaluateRhsReturnB(tTemp,yderiv,B) ;  

   for(i=0;i<3;i++)        // Output trajectory vector
   {
      K4[i] = Step * yderiv[i+3];
      tOut[i] = tIn[i] + Step*(tIn[i+3] + (K1[i] + K2[i] + K3[i])/6.0) ;
      tOut[i+3] = tIn[i+3] + (K1[i] + 2*K2[i] + 2*K3[i] +K4[i])/6.0 ;
   }
   // NormaliseTangentVector( tOut );
#endif
}

// ---------------------------------------------------------------------------

G4double G4RKG3_Stepper::DistChord()   const 
{
  // Soon: must check whether h/R > 2 pi  !!
  //  Method below is good only for < 2 pi

  return G4LineSection::Distline( fyMidPoint, fyInitial, fyFinal );
  // This is a class method that gives distance of Mid 
  //  from the Chord between the Initial and Final points.
}

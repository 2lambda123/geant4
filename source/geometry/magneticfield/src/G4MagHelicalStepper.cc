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
// $Id: G4MagHelicalStepper.cc,v 1.15 2005-02-15 17:37:41 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// --------------------------------------------------------------------

#include "G4MagHelicalStepper.hh"
#include "G4LineSection.hh"
#include "G4Mag_EqRhs.hh"

// given a purely magnetic field a better approach than adding a straight line
// (as in the normal runge-kutta-methods) is to add helix segments to the
// current position

G4MagHelicalStepper::G4MagHelicalStepper(G4Mag_EqRhs *EqRhs)
   : G4MagIntegratorStepper(EqRhs, 6)  // integrate over 6 variables only !!
                                       // position & velocity
{
  fPtrMagEqOfMot = EqRhs;
}

G4MagHelicalStepper::~G4MagHelicalStepper()
{
}

// Constant for determining unit conversion when using normal as integrand.
const G4double G4MagHelicalStepper::fUnitConstant = 0.299792458 * (GeV/(tesla*m));

void
G4MagHelicalStepper::AdvanceHelix( const G4double  yIn[],
				   G4ThreeVector   Bfld,    
				   G4double  h,
				   G4double  yHelix[]) const
{
  // const G4int    nvar = 6;
  const G4double approc_limit = 0.05;

  G4ThreeVector  Bnorm, B_x_P, vperp, vpar;
  // G4double norm;
  G4double B_d_P;  // B_perp;
  G4double Theta;  // , Theta_1;
  G4double R_1;
  G4double CosT2, SinT2, CosT, SinT;
  G4ThreeVector positionMove, endTangent;

  G4double Bmag = Bfld.mag();
  const G4double *pIn = yIn+3;
  G4ThreeVector initVelocity= G4ThreeVector( pIn[0], pIn[1], pIn[2]);
  G4double      velocityVal = initVelocity.mag();
  G4ThreeVector initTangent = (1.0/velocityVal) * initVelocity;  // .unit();  

  // fCof = fUnitConstant*particleCharge/MomentumXc;
  G4double particleCharge = fPtrMagEqOfMot->FCof() / (eplus*c_light); 
  G4double fCoefficient = (fUnitConstant / velocityVal) * particleCharge;

  // for too small magnetic fields there is no curvature
  // (include momentum here) FIXME

  if( Bmag < 1e-12 ) {
    LinearStep( yIn, h, yHelix );
  } else {
    // Bnorm = Bfld.unit();
    Bnorm = (1.0/Bmag)*Bfld;

    // calculate the direction of the force
    
    B_x_P = Bnorm.cross(initTangent);

    // parallel and perp vectors

    B_d_P = Bnorm.dot(initTangent); // this is the fraction of P parallel to B

    vpar = B_d_P * Bnorm;       // the component parallel      to B
    vperp= initTangent - vpar;  // the component perpendicular to B
    
    // B_v_P  = std::sqrt( 1 - B_d_P * B_d_P); // Fraction of P perp to B

    // calculate the radius^-1 of the helix and the stepping angle

    // R_1  = - fPtrMagEqOfMot->FCof() * Bmag;  // / B_v_P - but this cancels
    R_1  = - fCoefficient * Bmag;  // / B_v_P - but this cancels

    // again in Theta - so we don't need it.
    if( std::fabs(R_1) < 1e-10 ) {
      LinearStep( yIn, h, yHelix );
    } else {
      
      Theta   = R_1 * h; // * B_v_P;

      // Trigonometrix
      
      if( std::fabs(Theta) > approc_limit ) {
	SinT2    = std::sin(0.5 * Theta);
	CosT2    = std::cos(0.5 * Theta);
	// SinT     = std::sin(Theta);
	// CosT     = std::cos(Theta);
	SinT     = 2.0 * SinT2 * CosT2;
	CosT     = 1.0 - 2.0 * SinT2 * SinT2;
      } else {
	G4double Theta2 = Theta*Theta;
	G4double Theta3 = Theta2 * Theta;
	G4double Theta4 = Theta2 * Theta2;
	SinT     = Theta - 1.0/6.0 * Theta3;
	CosT     = 1 - 0.5 * Theta2 + 1.0/24.0 * Theta4;
	SinT2    = 0.5 * Theta - 1.0/48.0 * Theta3;
	CosT2    = 1 - 0.125 * Theta2 + 1.0/384 * Theta4;
      }

      // the actual "rotation"

      G4double R = 1.0 / R_1; 
      // positionMove  = h * ( CosT2 * vperp + SinT2 * B_x_P + vpar );
      positionMove  = R * ( SinT * vperp + (1-CosT) * B_x_P) + h * vpar;
      endTangent    = (CosT * vperp + SinT * B_x_P + vpar);

      // Store the resulting position and tangent
      yHelix[0]   = yIn[0] + positionMove.x(); 
      yHelix[1]   = yIn[1] + positionMove.y(); 
      yHelix[2]   = yIn[2] + positionMove.z(); 
				
      yHelix[3] = velocityVal * endTangent.x();
      yHelix[4] = velocityVal * endTangent.y();
      yHelix[5] = velocityVal * endTangent.z();

      // Store and/or calculate parameters for chord distance.
    }
  }
}

//
//  Use the midpoint method to get an error estimate and correction
//  modified from G4ClassicalRK4: W.Wander <wwc@mit.edu> 12/09/97
//

void
G4MagHelicalStepper::Stepper( const G4double yInput[],
		              const G4double*,
		                    G4double hstep,
		                    G4double yOut[],
		                    G4double yErr[]      )
{  
   const G4int nvar = 6 ;

   G4int i;
   // correction for Richardson Extrapolation.
   // G4double  correction = 1. / ( (1 << IntegratorOrder()) -1 );
   
   G4double      yTemp[7], yIn[7] ;
   G4ThreeVector Bfld_initial, Bfld_midpoint;

   //  Saving yInput because yInput and yOut can be aliases for same array

   for(i=0;i<nvar;i++) yIn[i]=yInput[i];

   G4double h = hstep * 0.5; 

   MagFieldEvaluate(yIn, Bfld_initial) ;        

   // Do two half steps
   DumbStepper(yIn,   Bfld_initial,  h, yTemp);
   MagFieldEvaluate(yTemp, Bfld_midpoint) ;     
   DumbStepper(yTemp, Bfld_midpoint, h, yOut); 

   // Store midpoint, to aid distance-from-chord calculation
   yMidPoint = G4ThreeVector( yTemp[0],  yTemp[1],  yTemp[2]); 

   // Do a full Step
   h = hstep ;
   DumbStepper(yIn, Bfld_initial, h, yTemp); 


   for(i=0;i<nvar;i++) {
     yErr[i] = yOut[i] - yTemp[i] ;
   }

#if G4HELICAL_USE_RICHARDSON_EXTRAPOLATION
   if( IntegratorOrder() > 1 ) {
       // It is unclear whether it is possible to 
       //  use the Richardson Extrapolation to increase accuracey by 1 order
     for(i=0;i<nvar;i++) {
       yOut[i] += yErr[i]*correction ;    
     }
   }
#endif

   yInitial = G4ThreeVector( yIn[0],   yIn[1],   yIn[2]); 
   yFinal   = G4ThreeVector( yOut[0],  yOut[1],  yOut[2]); 

   return ;
}


G4double
G4MagHelicalStepper::DistChord() const 
{
  // Soon: must check whether h/R > 2 pi  !!
  //  Method below is good only for < 2 pi

  return G4LineSection::Distline( yMidPoint, yInitial, yFinal );
  // This is a class method that gives distance of Mid 
  //  from the Chord between the Initial and Final points.
}

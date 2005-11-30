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
//
//
//---------------------------------------------------------------
//  GEANT 4 class header file
//
//  GFlashSamplingShowerTuning
//
//  Class description:
//
//  Tuning class for GFlash homogeneous shower parameterisation.
//  Definitions:
//    <t>: shower center of gravity
//      T: Depth at shower maximum
//     Ec: Critical energy
//     X0: Radiation length
//     y = E/Ec
//
//
// please see hep-ex/0001020 for details
//
// Author: Joanna Weng - November  2005
//---------------------------------------------------------------
#ifndef GFlashSamplingShowerTuning_hh
#define GFlashSamplingShowerTuning_hh
#include "GVFlashHomoShowerTuning.hh"

class GFlashSamplingShowerTuning : public GVFlashHomoShowerTuning
{
	public: // with description
	
	//	T_sam =  log(exp( log T_hom) + t1*Fs-1 + t2*(1-ehat));
	G4double ParsAveT1(){ return -0.55;} // t1
	G4double ParsAveT2(){ return -0.69;} // t2
	// alpha_sam = log(exp(log alphah_hom) +(a1*Fs-1)) 
	G4double ParsAveA1(){ return -0.476;  } // a1
	
	// std::sqrt(var(ln(T_sam))) = 1/(t+t2*ln(y))
	G4double ParsSigLogT1(){ return -2.5;} // t1
	G4double ParsSigLogT2(){ return 1.25;} // t2
	
	// std::sqrt(var(ln(alpha_sam))) = 1/(a1+a2*ln(y))
	G4double ParsSigLogA1(){ return -0.82;} // a1
	G4double ParsSigLogA2(){ return 0.79; } // a2
	
	// Correlation(ln(T),ln(alpha))=r1+r2*ln(y)
	G4double ParsRho1(){ return 0.784; } // r1
	G4double ParsRho2(){ return -0.023;} // r2
	
	// Radial profiles
	// f(r) := (1/dE(t))(dE(t,r)/dr)
	// Ansatz:
	// f(r) = p(2*r*Rc**2)/(r**2+Rc**2)**2+(1-p)*(2*r*Rt**2)/(r**2+Rt**2)**2,
	//        0<p<1
	
	// Rc_sam = Rc_hom + c1 * (1-ehat) + c2 *Fs-1*exp (-tau)
	
	G4double ParsRC1(){ return -0.0203;   } // c1
	G4double ParsRC2(){ return 0.0397;  }   // c2
	
	// Rt_sam = Rc_hom + t1 * (1-ehat) + t2 *Fs-1*exp (-tau)
	G4double ParsRT1(){ return -0.14;   } // t1
	G4double ParsRT2(){ return -0.495;}   // t2
	
	// W_sam = W_hom + (1-ehat)*(c1 + c2 *Fs-1 * exp (- (tau -1 )**2))
	G4double ParsWC1(){ return 0.348;   } // c1
	G4double ParsWC2(){ return -0.642;} // c2
	
	
	// Fluctuations on radial profiles through number of spots
	// The total number of spots needed for a shower is
	// Ns = n1*ln(Z)(E/GeV)**n2
	G4double ParsSpotN1(){ return 10.3;  } // n1
	G4double ParsSpotN2(){ return 0.959;} // n2
	
	// The number of spots per longitudinal interval is:
	// (1/Ns)(dNs(t)/dt) = f(t)
	//  = (beta*t)**(alpha-1)*beta*std::exp(-beta*t)/Gamma(alpha)
	// <t> = alpha_s/beta_s
	// Ts = (alpha_s-1)/beta_s
	// and
	// Ts = T*(t1+t2*Z)
	// alpha_s = alpha*(a1+a2*Z)
	G4double ParsSpotT1(){ return 0.813;  } // t1
	G4double ParsSpotT2(){ return 0.0019;} // t2
	
	G4double ParsSpotA1(){ return 0.844;  } //a1
	G4double ParsSpotA2(){ return 0.0026;} //a2
	
	//Resolution
	G4double ConstantResolution(){ return 0.00;  }  
	G4double NoiseResolution(){ return 0.00;  }  // not used    
	G4double SamplingResolution(){ return 0.11;  } // not used 
	
	
	
};

#endif

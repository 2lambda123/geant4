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
// $Id: G4GamDistrXTRdEdx.cc,v 1.4 2003-06-16 16:56:35 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include <complex>

#include "G4GamDistrXTRdEdx.hh"
#include "Randomize.hh"

#include "G4Gamma.hh"

////////////////////////////////////////////////////////////////////////////
//
// Constructor, destructor

G4GamDistrXTRdEdx::G4GamDistrXTRdEdx(G4Envelope *anEnvelope, 
					       G4double a, G4double alphaPlate,
                                               G4double b, G4double alphaGas) :
  G4VXTRdEdx(anEnvelope,a,b)
{
  G4cout<<"Gammma distributed X-ray TR radiator model is called"<<G4endl ;

  // Build energy and angular integral spectra of X-ray TR photons from
  // a radiator

  fAlphaPlate = alphaPlate ;
  fAlphaGas   = alphaGas   ;
  G4cout<<"fAlphaPlate = "<<fAlphaPlate<<" ; fAlphaGas = "<<fAlphaGas<<G4endl ;

  BuildTable() ;
}

///////////////////////////////////////////////////////////////////////////

G4GamDistrXTRdEdx::~G4GamDistrXTRdEdx()
{
  ;
}



///////////////////////////////////////////////////////////////////////////
//
// Rough approximation for radiator interference factor for the case of
// fully GamDistr radiator. The plate and gas gap thicknesses are distributed 
// according to exponent. The mean values of the plate and gas gap thicknesses 
// are supposed to be about XTR formation zones but much less than 
// mean absorption length of XTR photons in coresponding material.

G4double 
G4GamDistrXTRdEdx::GetStackFactor( G4double energy, 
                                         G4double gamma, G4double varAngle )
{
  G4double result, Za, Zb, Ma, Mb ;
  
  Za = GetPlateFormationZone(energy,gamma,varAngle) ;
  Zb = GetGasFormationZone(energy,gamma,varAngle) ;

  Ma = GetPlateLinearPhotoAbs(energy) ;
  Mb = GetGasLinearPhotoAbs(energy) ;


  G4complex Ca(1.0+0.5*fPlateThick*Ma/fAlphaPlate,fPlateThick/Za/fAlphaPlate) ; 
  G4complex Cb(1.0+0.5*fGasThick*Mb/fAlphaGas,fGasThick/Zb/fAlphaGas) ; 

  G4complex Ha = std::pow(Ca,-fAlphaPlate) ;  
  G4complex Hb = std::pow(Cb,-fAlphaGas) ;
  G4complex H  = Ha*Hb ;

  G4complex F1 =   (1.0 - Ha)*(1.0 - Hb )/(1.0 - H)
                 * G4double(fPlateNumber) ;

  G4complex F2 =   (1.0-Ha)*(1.0-Ha)*Hb/(1.0-H)/(1.0-H)
                 * (1.0 - std::pow(H,fPlateNumber)) ;

  G4complex R  = (F1 + F2)*OneInterfaceXTRdEdx(energy,gamma,varAngle) ;

  result       = 2.0*std::real(R) ;
  
  return      result ;
}


//
//
////////////////////////////////////////////////////////////////////////////









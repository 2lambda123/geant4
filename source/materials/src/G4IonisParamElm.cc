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
// $Id: G4IonisParamElm.cc,v 1.11 2004-12-07 08:50:03 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....
//
// 06-09-04, Update calculated values after any change of ionisation 
//           potential change. V.Ivanchenko
// 17-10-02, Fix excitation energy interpolation. V.Ivanchenko
// 08-03-01, correct handling of fShellCorrectionVector. M.Maire
// 22-11-00, tabulation of ionisation potential from 
//           the ICRU Report N#37. V.Ivanchenko
// 09-07-98, data moved from G4Element. M.Maire
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

#include "G4IonisParamElm.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4IonisParamElm::G4IonisParamElm(G4double Z)
{
    if (Z < 1.) G4Exception
      (" ERROR! It is not allowed to create an Element with Z < 1" );
    
    // some basic functions of the atomic number
    fZ = Z;
    fZ3  = std::pow(fZ, 1./3.);
    fZZ3 = std::pow(fZ*(fZ+1.), 1./3.);
    flogZ3 = std::log(fZ)/3.;
   
     // Parameters for energy loss by ionisation   

     // Mean excitation energy
     // from "Stopping Powers for Electrons and Positrons"
     // ICRU Report N#37, 1984  (energy in eV)
    static double exc[100] = { 
    21.8, 20.9,	13.3, 15.9, 15.2, 13.0,	11.7, 11.9, 11.5, 13.7,
    13.6, 13.0,	12.8, 12.4, 11.5, 11.3,	10.2, 10.4, 10.0,  9.6,
    10.3, 10.6,	10.7, 10.7, 10.9, 11.0,	11.0, 11.1, 11.1, 11.0,
    10.8, 10.4,	10.5, 10.2,  9.8,  9.8,  9.8,  9.6,  9.7,  9.8,
    10.2, 10.1, 10.0, 10.0, 10.0, 10.2, 10.0,  9.8, 10.0,  9.8,
     9.5,  9.3,  9.3,  8.9,  8.9,  8.8,  8.8,  8.8,  9.1,  9.1,
     9.2,  9.3,  9.2,  9.2,  9.4,  9.5,  9.7,  9.7,  9.8,  9.8,
     9.8,  9.8,  9.8,  9.8,  9.8,  9.8,  9.8, 10.1, 10.0, 10.0,
    10.0, 10.0,  9.9,  9.9,  9.7,  9.2,  9.5,  9.4,  9.4,  9.4,
     9.6,  9.7,  9.7,  9.8,  9.8,  9.8,  9.8,  9.9,  9.9,  9.9 };

    G4int iz = (G4int)Z - 1 ;
    if(0  > iz) iz = 0;
    else if(99 < iz) iz = 99 ;
    fMeanExcitationEnergy = fZ * exc[iz] * eV ;


    fTau0 = 0.1*fZ3*MeV/proton_mass_c2;
    fTaul = 2.*MeV/proton_mass_c2;

    // compute the Bethe-Bloch formula for energy = fTaul*particle mass
    G4double rate = fMeanExcitationEnergy/electron_mass_c2 ;
    G4double w = fTaul*(fTaul+2.) ;
    fBetheBlochLow = (fTaul+1.)*(fTaul+1.)*std::log(2.*w/rate)/w - 1. ;
    fBetheBlochLow = 2.*fZ*twopi_mc2_rcl2*fBetheBlochLow ; 
  
    fClow = std::sqrt(fTaul)*fBetheBlochLow;
    fAlow = 6.458040 * fClow/fTau0;
    G4double Taum = 0.035*fZ3*MeV/proton_mass_c2;
    fBlow =-3.229020*fClow/(fTau0*std::sqrt(Taum));

    // Shell correction factors
    fShellCorrectionVector = new G4double[3];
    rate = 0.001*fMeanExcitationEnergy/eV;
    G4double rate2 = rate*rate;
    /*    
    fShellCorrectionVector[0] = ( 1.10289e5 + 5.14781e8*rate)*rate2 ;
    fShellCorrectionVector[1] = ( 7.93805e3 - 2.22565e7*rate)*rate2 ;
    fShellCorrectionVector[2] = (-9.92256e1 + 2.10823e5*rate)*rate2 ;
    */
    fShellCorrectionVector[0] = ( 0.422377   + 3.858019*rate)*rate2 ;
    fShellCorrectionVector[1] = ( 0.0304043  - 0.1667989*rate)*rate2 ;
    fShellCorrectionVector[2] = (-0.00038106 + 0.00157955*rate)*rate2 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4IonisParamElm::~G4IonisParamElm()
{
  if (fShellCorrectionVector) delete [] fShellCorrectionVector; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4IonisParamElm::G4IonisParamElm(G4IonisParamElm& right)
{
  *this = right;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

const G4IonisParamElm& G4IonisParamElm::operator=(const G4IonisParamElm& right)
{
  if (this != &right)
    {
      fZ                     = right.fZ;
      fZ3                    = right.fZ3;
      fZZ3                   = right.fZZ3;
      flogZ3                 = right.flogZ3;
      fTau0                  = right.fTau0;
      fTaul                  = right.fTaul;
      fBetheBlochLow         = right.fBetheBlochLow;
      fAlow                  = right.fAlow;
      fBlow                  = right.fBlow;
      fClow                  = right.fClow;
      fMeanExcitationEnergy  = right.fMeanExcitationEnergy;
      if (fShellCorrectionVector) delete [] fShellCorrectionVector;             
      fShellCorrectionVector = new G4double[3];            
      fShellCorrectionVector[0] = right.fShellCorrectionVector[0];
      fShellCorrectionVector[1] = right.fShellCorrectionVector[1];
      fShellCorrectionVector[2] = right.fShellCorrectionVector[2];      
     } 
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4int G4IonisParamElm::operator==(const G4IonisParamElm& right) const
{
  return (this == (G4IonisParamElm *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

G4int G4IonisParamElm::operator!=(const G4IonisParamElm& right) const
{
  return (this != (G4IonisParamElm *) &right);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

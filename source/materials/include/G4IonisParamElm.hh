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
// $Id: G4IonisParamElm.hh,v 1.7 2004-12-07 08:50:01 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

// class description
//
// The class contains few (physical) quantities related to the Ionisation
// process, for the Element defined by its atomic number Z
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// 09.03.01: copy constructor and assignement operator in public (mma)
// 09.07.98: data moved from G4Element (mma) 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

#ifndef G4IonisParamElm_HH
#define G4IonisParamElm_HH

#include "G4ios.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

class G4IonisParamElm
{
public:  // with description

    G4IonisParamElm(G4double Z);
    virtual ~G4IonisParamElm();

    // retrieval methods
    
    G4double  GetZ()        const {return fZ;};       // atomic number Z
    G4double  GetZ3()       const {return fZ3;};      // std::pow (Z,1/3)
    G4double  GetZZ3()      const {return fZZ3;};     // std::pow (Z(Z+1),1/3)
    G4double  GetlogZ3()    const {return flogZ3;};   // std::log(Z)/3

    G4double  GetTau0() const {return fTau0;};
                       // 0.1*std::pow(Z,1/3)*MeV/proton_mass_c2
    
    G4double  GetTaul() const {return fTaul;};        // 2*MeV/proton mass
    
    G4double  GetAlow() const {return fAlow;};
    G4double  GetBlow() const {return fBlow;};
    G4double  GetClow() const {return fClow;};
                       // parameters for the low energy ion.loss
    
    G4double  GetMeanExcitationEnergy()  const {return fMeanExcitationEnergy;};
                                        // 16*std::pow(Z,0.9)*eV 
      
    G4double* GetShellCorrectionVector() const {return fShellCorrectionVector;};
                                       // shell correction coefficients
   
public:  // without description

    G4IonisParamElm(G4IonisParamElm&);
    const G4IonisParamElm& operator=(const G4IonisParamElm&);   
    G4int operator==(const G4IonisParamElm&) const;
    G4int operator!=(const G4IonisParamElm&) const;
     

private:

  //
  //  data members
  //
    G4double fZ;                 // effective Z
    G4double fZ3;                // std::pow (Z,1/3)
    G4double fZZ3;               // std::pow (Z(Z+1),1/3)
    G4double flogZ3;             // std::log(Z)/3

    //  ------ ionisation loss ---------------------------------
    
    G4double  fTau0 ;                 // 0.1*std::pow(Z,1/3)*MeV/proton_mass_c2
    G4double  fTaul ;                 // 2*MeV/proton mass
    G4double  fBetheBlochLow;         // Bethe-Bloch at fTaul*particle mass   
    G4double  fAlow,fBlow,fClow;      // parameters for the low energy ion.loss
    G4double  fMeanExcitationEnergy;  //     
    G4double* fShellCorrectionVector; // shell correction coefficients
};

#endif

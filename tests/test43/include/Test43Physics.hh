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
// -------------------------------------------------------------
//      GEANT 4 class 
//
//      ---------- Test43Physics -------
//           created from test30 files originally by Vladimir Ivanchenko
// 
//    Modified:
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Test43Physics_h
#define Test43Physics_h 1

#include "globals.hh"
#include "Test43HadronProduction.hh"
#include "G4ExcitationHandler.hh"
#include "G4PreCompoundModel.hh"
#include "G4BinaryCascade.hh"

class G4VProcess;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Test43Physics
{
  public:

    Test43Physics();
   ~Test43Physics();

    G4VProcess* GetProcess(const G4String&, const G4String&, G4Material*);
    G4double GetNucleusMass() {return theProcess->GetMass();};
    G4ExcitationHandler* GetDeExcitation() {return theDeExcitation;};
    G4PreCompoundModel* GetPreCompound() {return thePreCompound;};
//    void setCutOnP(G4double val) {if(hkmod) hkmod->setCutOnP(val);};
//    void setCutOnPPP(G4double val) {if(hkmod) hkmod->setCutOnPPP(val);};

  private:

    void Initialise();

    Test43HadronProduction* theProcess;
    G4ExcitationHandler*    theDeExcitation;
    G4PreCompoundModel*     thePreCompound;
    G4BinaryCascade*        hkmod;
};

#endif

 



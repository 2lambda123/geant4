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
// Short description of the G4 Class made by M.K. (asFarAsHeNnderstands)
// This is a substitute for the Physics List in Test49
// =====================================================================
//
//      ---------- Test49Physics -------
// 
//    Converted from Test29 to Test49 by Mikhail Kossov, 29 Jan 2005 
//
//---------------------------------------------------------------------------------------

#ifndef Test49Physics_h
#define Test49Physics_h 1

#include "globals.hh"
#include "Test49HadronProduction.hh"
#include "G4VRestProcess.hh"
#include "G4VDiscreteProcess.hh"
#include "G4QCaptureAtRest.hh"
#include "G4StringChipsInterface.hh"
#include "G4ExcitationHandler.hh"
#include "G4PreCompoundModel.hh"
#include "G4BinaryCascade.hh"

class G4VProcess;
class G4Material;

class Test49Physics
{
public:
  Test49Physics();
  ~Test49Physics();

  G4VProcess* GetProcess(const G4String&, const G4String&, G4Material*, G4bool, G4bool);
  G4double GetNucleusMass() {return theHDProcess->GetMass();}; //Only for HadronicProcesses
  G4ExcitationHandler* GetDeExcitation() {return theDeExcitation;};//Only for HadrProcesses
  G4PreCompoundModel* GetPreCompound() {return thePreCompound;};// Only for HadronProcesses
  //void setCutOnP(G4double val) {if(hkmod) hkmod->setCutOnP(val);};
  //void setCutOnPPP(G4double val) {if(hkmod) hkmod->setCutOnPPP(val);};

private:
  // Body
  G4VDiscreteProcess*     theWDProcess; // ElectroWeAk(W) Discrete(D) Process (onFlight)
  G4VRestProcess*         theWRProcess; // ElectroWeak(W) Rest(R) Process (atRest)
  Test49HadronProduction* theHDProcess; // Hadronic(H) Discrete(D) Process (onFlight)
  G4ExcitationHandler*    theDeExcitation;
  G4PreCompoundModel*     thePreCompound;
  G4BinaryCascade*        hkmod;
};

#endif

 



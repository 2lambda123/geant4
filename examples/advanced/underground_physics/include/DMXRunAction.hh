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
// --------------------------------------------------------------
//   GEANT 4 - Underground Dark Matter Detector Advanced Example
//
//      For information related to this code contact: Alex Howard
//      e-mail: alexander.howard@cern.ch
// --------------------------------------------------------------
// Comments
//
//                  Underground Advanced
//               by A. Howard and H. Araujo 
//                    (27th November 2001)
//
// RunAction header
// --------------------------------------------------------------

#ifndef DMXRunAction_h
#define DMXRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class DMXRunActionMessenger;
class G4Run;
class DMXAnalysisManager;

class DMXRunAction : public G4UserRunAction
{
  public:
    DMXRunAction();
   ~DMXRunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);

  public:
    void SetsavehitsFile   (G4String val)        { savehitsFile   = val;};
    void SetsavepmtFile    (G4String val)        { savepmtFile    = val;};
    void SetsavehistFile   (G4String val)        { savehistFile   = val;};
    void Setplotevent      (G4bool valb)         { plotevent      = valb;};
    void Setinteractplot   (G4bool valb)         { interactplot   = valb;};

    G4String GetsavehitsFile()                   {return savehitsFile;};
    G4String GetsavepmtFile()                    {return savepmtFile;};
    G4bool   Getplotevent()                      {return plotevent;};

  private:
  
  //messenger
    G4String savehitsFile;
    G4String savepmtFile;
    G4String savehistFile;
    G4bool   plotevent;
    G4bool   interactplot;

  DMXRunActionMessenger* runMessenger;

};

#endif


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
// ********************************************************************
// Code developed by:
//  S.Larsson
//
//    ******************************
//    *                            *
//    *    PurgMagRunAction.hh     *
//    *                            *
//    ******************************
//
// $Id: PurgMagRunAction.hh,v 1.1 2003-12-09 14:13:23 sussi Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef PurgMagRunAction_h
#define PurgMagRunAction_h 1

#include "PurgMagDetectorConstruction.hh"

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Run;

class PurgMagRunAction : public G4UserRunAction
{
public:
  
  PurgMagRunAction(PurgMagDetectorConstruction*);
  ~PurgMagRunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
    
  void  SetRndmFreq(G4int   val)  {saveRndm = val;}
  G4int GetRndmFreq()             {return saveRndm;}


private:

  PurgMagDetectorConstruction* Detector;    
  G4int saveRndm;

};

#endif














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
//    *****************************************
//    *                                       *
//    *      RemSimDetectrorMessenger.hh      *
//    *                                       *
//    *****************************************
//
// $Id: RemSimRunMessenger.hh,v 1.2 2004-05-22 12:57:05 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Code developed by: S.Guatelli, guatelli@ge.infn.it
// 
#ifndef RemSimRunMessenger_h
#define RemSimRunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RemSimRunAction;
class RemSimRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class RemSimRunMessenger: public G4UImessenger
{
public:
  RemSimRunMessenger(RemSimRunAction* );
  ~RemSimRunMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  RemSimRunAction*  run;
  G4UIdirectory*    runDir; 
  G4UIcmdWithAString* readFileCmd; //change data.txt 
};
#endif


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
// $Id: RemSimSteppingAction.hh,v 1.2 2004-03-12 10:55:54 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// Author: Susanna Guatelli (guatelli@ge.infn.it)
//
// History:
// -----------
// 27 May  2003   S.Guatelli    first code review 
// 17 May  2003   S. Guatelli   1st implementation
//
// -------------------------------------------------------------------

#ifndef RemSimSteppingAction_h
#define RemSimSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "globals.hh"

class G4Step;
class RemSimPrimaryGeneratorAction;
class RemSimDetectorConstruction;
class RemSimEventAction;
class RemSimSteppingAction : public G4UserSteppingAction
{
public:

  RemSimSteppingAction(RemSimPrimaryGeneratorAction*,
		       RemSimEventAction*, 
		       RemSimDetectorConstruction*);
  ~RemSimSteppingAction();

  void UserSteppingAction(const G4Step* aStep);
private:
  RemSimPrimaryGeneratorAction* primaryAction;
  RemSimEventAction* eventAction; 
  G4int IDnow;
  G4int IDold;
  RemSimDetectorConstruction* detector;  
};
#endif

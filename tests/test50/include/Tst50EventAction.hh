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
// $Id: Tst50EventAction.hh,v 1.13 2003-07-03 13:43:09 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Susanna Guatelli (guatelli@ge.infn.it)
//
// History:
// -----------
// 17 May  2003   S. Guatelli   1st implementation
//
// -------------------------------------------------------------------
 
#ifndef Tst50EventAction_h
#define Tst50EventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"

class G4Event;
class Tst50TrackerHit;

class Tst50EventAction : public G4UserEventAction
{
public:
  Tst50EventAction();
  ~Tst50EventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  G4int    GetEventNo();

private:
  G4int    collisionID;
  G4double  totalEnergy; 
};
#endif

    

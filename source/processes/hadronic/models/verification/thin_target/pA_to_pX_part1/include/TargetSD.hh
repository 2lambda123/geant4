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
// $Id: TargetSD.hh,v 1.1 2003-05-27 13:44:45 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
#ifndef TargetSD_h
#define TargetSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class TargetConstruction;
class G4HCofThisEvent;
class G4Step;
#include "TargetHit.hh"


class TargetSD : public G4VSensitiveDetector
{
  public:
  
    TargetSD(G4String, TargetConstruction* );
    ~TargetSD();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();

  private:
  
    TargetHitsCollection* TgtCollection;      
    TargetConstruction* Target;
    G4int HitID;
};

#endif




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
// $Id: HadrontherapyPhantomSD.hh,v 3.0, September 2004
// --------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// --------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone, F. Di Rosa, G. Russo
// Laboratori Nazionali del Sud - INFN, Catania, Italy
//
// --------------------------------------------------------------

#ifndef HadrontherapyPhantomSD_h
#define HadrontherapyPhantomSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HadrontherapyPhantomHit.hh"
#include "HadrontherapyRunAction.hh"
#include "globals.hh"
#include "G4ios.hh"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class HadrontherapyPhantomSD : public G4VSensitiveDetector
{
public:
  HadrontherapyPhantomSD(G4String name);
  ~HadrontherapyPhantomSD();

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll(); 

private:
  HadrontherapyPhantomHitsCollection *HitsCollection;
  G4String sensitiveDetectorName;	
};
#endif



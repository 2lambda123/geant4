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
// $Id: Tst14CalorimeterSD.hh,v 1.6 2003-02-23 09:49:14 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#ifndef Tst14CalorimeterSD_h
#define Tst14CalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class Tst14DetectorConstruction;
class G4HCofThisEvent;
class G4Step;
#include "Tst14CalorHit.hh"


class Tst14CalorimeterSD : public G4VSensitiveDetector
{
  public:
  
      Tst14CalorimeterSD(G4String, Tst14DetectorConstruction* );
     ~Tst14CalorimeterSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void PrintAll();

  private:
  
      Tst14CalorHitsCollection*  CalCollection;      
      Tst14DetectorConstruction* Detector;
      G4int*                   HitID;
};

#endif


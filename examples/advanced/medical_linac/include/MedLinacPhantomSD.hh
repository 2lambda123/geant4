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
// *                      
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//// $Id: MedLinacPhantomSD.hh,v 1.5 2005-07-03 23:27:37 mpiergen Exp $
//
//

// Code developed by: M. Piergentili
//
#ifndef MedLinacPhantomSD_h
#define MedLinacPhantomSD_h 1

#include "G4VSensitiveDetector.hh"
#include "MedLinacPhantomHit.hh"
class MedLinacPhantomMessenger;

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class MedLinacPhantomSD : public G4VSensitiveDetector
{
public:
  MedLinacPhantomSD(G4String name);
  ~MedLinacPhantomSD();


  void SetPhantomDimension (G4double);
  void SetNumberOfPhantomVoxels (G4int);

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();

public:
  G4double GetPhantomDimension()  {return phantomDimension;};
  G4int GetNumberOfPhantomVoxels()  {return numberOfPhantomVoxels;};

  private:
    G4double phantomDimension;
  G4int numberOfPhantomVoxels;
  MedLinacPhantomMessenger* phantomMessenger;
};
#endif



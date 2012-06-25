//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file analysis/A01/include/A01Hodoscope.hh
/// \brief Definition of the A01Hodoscope class
//
// $Id: A01Hodoscope.hh,v 1.4 2006-06-29 16:31:17 gunter Exp $
// --------------------------------------------------------------
//
#ifndef A01Hodoscope_h
#define A01Hodoscope_h 1

#include "G4VSensitiveDetector.hh"
#include "A01HodoscopeHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class A01Hodoscope : public G4VSensitiveDetector
{
  public:
      A01Hodoscope(G4String name);
      virtual ~A01Hodoscope();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      A01HodoscopeHitsCollection* fHitsCollection;
      G4int fHCID;
};

#endif


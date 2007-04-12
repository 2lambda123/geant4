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
//
// $Id: Tst52ElectronEEDL.hh,v 1.1 2007-04-12 12:00:17 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Authors: Susanna Guatelli (guatelli@ge.infn.it)
//         Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// History:
// -----------
// 22 Feb 2003 MGP          Created
//
// -------------------------------------------------------------------

// Class description:
// System test for e/gamma, electron processes based on EEDL for PhysicsList
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef TST52ELECTRONEEDL_HH
#define TST52ELECTRONEEDL_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class G4MultipleScattering;
class G4LowEnergyIonisation;
class G4LowEnergyBremsstrahlung;

class Tst52ElectronEEDL : public G4VPhysicsConstructor {

public: 

  Tst52ElectronEEDL(const G4String& name = "electron-eedl");
  
  virtual ~Tst52ElectronEEDL();
  
  // This method is dummy for physics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();

  void SetFacRange(G4double value);
  // Change the value of the FacRange

  void SetLowECut(G4double value);

private:
  G4MultipleScattering* scattering; 
  G4LowEnergyIonisation* ionization;
  G4LowEnergyBremsstrahlung* brem;
  G4double facValue;
  G4double cut;
};

#endif









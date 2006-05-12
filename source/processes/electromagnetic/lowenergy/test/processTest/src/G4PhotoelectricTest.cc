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
// $Id: G4PhotoelectricTest.cc,v 1.3 2006-05-12 17:47:58 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// History:
// -----------
// 07 Oct 2001   MGP        Created
//
// -------------------------------------------------------------------
// Class description:
// Test DoIt method of physics processes
// Further documentation available from http://www.ge.infn.it/geant4/lowE/index.html

// -------------------------------------------------------------------

#include "globals.hh"
#include "G4PhotoelectricTest.hh"
#include "G4VProcess.hh"

#include "G4LowEnergyPhotoElectric.hh"

#include "G4PhotoElectricEffect.hh"

#include "G4LowEnergyBremsstrahlung.hh"
#include "G4eBremsstrahlung.hh"

#include "G4LowEnergyIonisation.hh"
#include "G4eIonisation.hh"

G4PhotoelectricTest::G4PhotoelectricTest(const G4String& category)
  :type(category)
{ }

G4PhotoelectricTest:: ~G4PhotoelectricTest()
{ }

G4VProcess* G4PhotoelectricTest::createElectronIonisation()
{
  G4VProcess* testProcess = 0;
  if (type == "lowE")
    {
      testProcess = new G4LowEnergyIonisation;
    }
  else if (type == "standard")
    {
      testProcess = new G4eIonisation;
    }
  return testProcess;
}

G4VProcess* G4PhotoelectricTest::createBremsstrahlung()
{
  G4VProcess* testProcess = 0;
  if (type == "lowE")
    {
      testProcess = new G4LowEnergyBremsstrahlung;
    }
  else if (type == "standard")
    {
      testProcess = new G4eBremsstrahlung;
    }
  return testProcess;  
}

G4VProcess* G4PhotoelectricTest::createProcess()
{
  G4VProcess* testProcess = 0;
  if (type == "lowE")
    {
      testProcess = new G4LowEnergyPhotoElectric;
    }
  else if (type == "standard")
    {
      testProcess = new G4PhotoElectricEffect;
    }
  return testProcess;  
}



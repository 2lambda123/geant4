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
// $Id: G4eIonisationTest.hh,v 1.2 2006-05-12 17:47:58 pia Exp $
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
// Test of electromagnetic physics processes
// Further documentation available from http://www.ge.infn.it/geant4/lowE/index.html

// -------------------------------------------------------------------

#ifndef G4EIONISATIONTEST_HH
#define G4EIONISATIONTEST_HH 1

#include "globals.hh"
#include "G4ProcessTest.hh"

class G4VProcess;

class G4eIonisationTest : public G4ProcessTest 
{
public:

  G4eIonisationTest(const G4String& category);
  virtual ~G4eIonisationTest(); 

  protected:

  virtual G4VProcess* createProcess();
  virtual G4VProcess* createBremsstrahlung();
  virtual G4VProcess* createElectronIonisation();
  G4ParticleDefinition* createIncidentParticle();

private:
  
  // Hide copy constructor and assignment operator
  G4eIonisationTest(const G4eIonisationTest&);
  G4eIonisationTest & operator=(const G4eIonisationTest &right);

  const G4String& type;

};
 
#endif




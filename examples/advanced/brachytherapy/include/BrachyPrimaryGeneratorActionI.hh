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
// $Id: BrachyPrimaryGeneratorActionI.hh,v 1.3 2003-05-09 16:52:06 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//
//    ********************************************
//    *                                          *
//    *      BrachyPrimaryGeneratorActionI.hh     *
//    *                                          *
//    ********************************************
//Primary particles of the Iodium source. They are delivered from a random
//point of the radionuclide  with random direction. The energy spectrum
// of the gamma delivered is activated

#ifndef BrachyPrimaryGeneratorActionI_h
#define BrachyPrimaryGeneratorActionI_h 1

#include "globals.hh"
#include "g4std/vector"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "BrachyPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Run;
class G4Event;
class BrachyAnalysisManager;
class BrachyPrimaryGeneratorAction;

class BrachyPrimaryGeneratorActionI : public  G4VUserPrimaryGeneratorAction
{
 public:
      BrachyPrimaryGeneratorActionI();
      ~BrachyPrimaryGeneratorActionI();

 public:
      void GeneratePrimaries(G4Event* anEvent);
      G4double GetEnergy();

 private:
      G4ParticleGun* m_pParticleGun;
      G4double Energy;
      G4std::vector<G4double> vettore;
};

#endif


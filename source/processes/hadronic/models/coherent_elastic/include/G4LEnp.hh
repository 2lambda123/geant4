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
 // G4 Low energy model: n-p scattering
 // F.W. Jones, L.G. Greeniaus, H.P. Wellisch
 //  
 // For further comments see G4LEnpData.hh and G4LEnp.cc
 //

#ifndef G4LEnp_h
#define G4LEnp_h 1
 
#include "globals.hh"
#include "Randomize.hh"
#include "G4Element.hh"
#include "G4ElementVector.hh"
#include "G4ElementTable.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsVector.hh"
#include "G4LPhysicsFreeVector.hh"
#include "G4Gamma.hh"
#include "G4Step.hh"
#include "G4TrackStatus.hh"
#include "G4HadronicInteraction.hh"

#ifdef NPDEBUG
#include <iostream>
#include <fstream>
#endif


class G4LEnp : public G4HadronicInteraction
{
 private:

  //enum { NENERGY=21, NANGLE=180 };
    enum { NENERGY=39, NANGLE=180 };

 public:

   G4LEnp();

   ~G4LEnp();
 
   G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack,
                                  G4Nucleus& targetNucleus);


 private:

   //   std::ofstream* outFile;
   //   std::ofstream* outFile1;

 // The following arrays are declared static to allow the use of initializers.
 // They are initialized in G4LEnpData.hh

   static __thread G4float sig[NENERGY][NANGLE];
   static __thread G4float pcm[NENERGY], elab[NENERGY], 
     dsigmax[NENERGY], sigtot[NENERGY];

};

#endif

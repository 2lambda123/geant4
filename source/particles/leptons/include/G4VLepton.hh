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
// $Id: G4VLepton.hh,v 1.4 2003-03-11 05:49:40 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//      History: first implementation, based on object model of
//      2nd December 1995, G.Cosmo
//  Revised, Hisaya Kurashige, 15 Dec 1996
//  Revised, Hisaya Kurashige, 24 Frb 1997
// ------------------------------------------------------------

#ifndef G4VLepton_h
#define G4VLepton_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4Material.hh"

#include "G4PhysicsLogVector.hh"
#include "G4ParticleDefinition.hh"

class G4VLepton : public G4ParticleDefinition
{
  //  A virtual class for Leptons particles. It defines
  //  public methods which describe the behavior of a
  //  lepton.

  private:

      const G4VLepton & operator=(const G4VLepton &right);

  public:

      G4VLepton(const G4String&  aName,  
                G4double         mass,     
                G4double         width,
                G4double         charge,   
                G4int            iSpin,
                G4int            iParity,
                G4int            iConjugation,
                G4int            iIsospin,   
                G4int            iIsospinZ, 
                G4int            gParity,
                const G4String&  pType,
                G4int            lepton,
                G4int            baryon,
                G4int            encoding,
                G4bool           stable,
                G4double         lifetime,
                G4DecayTable     *decaytable)
	: G4ParticleDefinition(aName, mass, width, charge, iSpin, iParity,
                               iConjugation, iIsospin, iIsospinZ, gParity,
                               pType, lepton, baryon, encoding, stable,
                               lifetime, decaytable) {};

      virtual ~G4VLepton() {};

      G4int operator==(const G4VLepton &right) const;
      G4int operator!=(const G4VLepton &right) const;

};

#endif


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
// $Id: G4eBremsstrahlungSpectrum.hh,v 1.5 2003-02-28 08:41:47 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4EBremsstrahlungSpectrum
//
// Author:        V.Ivanchenko (Vladimir.Ivantchenko@cern.ch)
//
// Creation date: 27 September 2001
//
// Modifications:
// 10.10.01  MGP  Revision to improve code quality and consistency with design
// 29.11.01  V.Ivanchenko    Parametrisation is updated
// 21.02.03  V.Ivanchenko    Energy bins are defined in the constructor
// 28.02.03  V.Ivanchenko    Filename is defined in the constructor
//
// -------------------------------------------------------------------

// Class Description:
// Provides various integration over gamma spectrum of e- Bremsstrahlung.
// Parametrisation is described in Physics Reference Manual based on
// data from EEDL database.
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef G4EBREMSSTRAHLUNGSPECTRUM_HH
#define G4EBREMSSTRAHLUNGSPECTRUM_HH 1

#include "globals.hh"
#include "G4DataVector.hh"
#include "G4VEnergySpectrum.hh"

class G4BremsstrahlungParameters;

class G4eBremsstrahlungSpectrum : public G4VEnergySpectrum
{
public:

  G4eBremsstrahlungSpectrum(const G4DataVector& bins,const G4String& name);

  ~G4eBremsstrahlungSpectrum();

  G4double Probability(G4int Z,
                       G4double tMin,
                       G4double tMax,
                       G4double kineticEnergy,
                       G4int shell=0,
		       const G4ParticleDefinition* pd=0) const;

  G4double AverageEnergy(G4int Z,
                         G4double tMin,
                         G4double tMax,
                         G4double kineticEnergy,
                         G4int shell=0,
			 const G4ParticleDefinition* pd=0) const;

  G4double SampleEnergy(G4int Z,
                        G4double tMin,
                        G4double tMax,
                        G4double kineticEnergy,
                        G4int shell=0,
			const G4ParticleDefinition* pd=0) const;

  G4double MaxEnergyOfSecondaries(G4double kineticEnergy,
                                  G4int Z = 0,
				  const G4ParticleDefinition* pd=0) const;

  G4double Excitation(G4int Z, G4double kineticEnergy) const;

  void PrintData() const;

private:

  G4double IntSpectrum(G4double xMin, G4double xMax,
                         const G4DataVector& p) const;

  G4double AverageValue(G4double xMin, G4double xMax,
			const G4DataVector& p) const;

  G4double Function(G4double x, const G4DataVector& p) const;


  // Hide copy constructor and assignment operator
  G4eBremsstrahlungSpectrum(const  G4eBremsstrahlungSpectrum&);
  G4eBremsstrahlungSpectrum & operator = (const G4eBremsstrahlungSpectrum &right);

  G4BremsstrahlungParameters* theBRparam;
  G4double                    lowestE;
  size_t                      length;
  G4int                       verbose;
  const G4DataVector&         xp;
};

#endif

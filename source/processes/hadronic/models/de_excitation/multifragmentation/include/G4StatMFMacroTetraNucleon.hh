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
// $Id: G4StatMFMacroTetraNucleon.hh,v 1.1 2003-08-26 18:47:21 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara

#ifndef G4StatMFMacroTetraNucleon_h
#define G4StatMFMacroTetraNucleon_h 1

#include "G4VStatMFMacroCluster.hh"
#include "G4NucleiPropertiesTable.hh"

class G4StatMFMacroTetraNucleon : public G4VStatMFMacroCluster {

public:

    // Default constructor
    G4StatMFMacroTetraNucleon() : G4VStatMFMacroCluster(4) {};

    // Destructor
    ~G4StatMFMacroTetraNucleon() {};
	

private:

    // Copy constructor
    G4StatMFMacroTetraNucleon(const G4StatMFMacroTetraNucleon & right);

    // operators
    G4StatMFMacroTetraNucleon & operator=(const G4StatMFMacroTetraNucleon & right);
    G4bool operator==(const G4StatMFMacroTetraNucleon & right) const;
    G4bool operator!=(const G4StatMFMacroTetraNucleon & right) const;

public:

    G4double CalcMeanMultiplicity(const G4double FreeVol, const G4double mu, 
				  const G4double nu, const G4double T);
								
    G4double CalcZARatio(const G4double ) {return theZARatio = 0.5;}								

    G4double CalcEnergy(const G4double T);

    G4double CalcEntropy(const G4double T, const G4double FreeVol);

};

#endif

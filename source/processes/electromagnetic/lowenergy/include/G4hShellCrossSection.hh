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
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:  G4hShellCrossSection   
//
// Author:     S. Dussoni and A. Mantero (Alfonso.Mantero@ge.infn.it)
// 
// History:
// -----------
// 23 Oct 2001 A. Mantero   1st implementation
// 24 Oct 2001 MGP          Cleaned up
// 29 Oct 2001 VI           Add delta energy
// 22 Apr 2004 S.Saliceti   Add GetCrossSection method
// -------------------------------------------------------------------

// Class Description: 
// Model for shell cross sections in proton ionisation

// -------------------------------------------------------------------

#ifndef G4HSHELLCROSSSECTION_HH
#define G4HSHELLCROSSSECTION_HH 1

#include "globals.hh"
#include "G4VhShellCrossSection.hh" 

class G4hShellCrossSection : public G4VhShellCrossSection
{
public:

  G4hShellCrossSection();

  virtual ~G4hShellCrossSection();

  virtual std::vector<G4double> GetCrossSection(G4int Z,
						G4double incidentEnergy,
						G4double mass,
						G4double deltaEnergy,
						G4bool testFlag = false) const;

protected:

  virtual std::vector<G4double> Probabilities(G4int Z,
					      G4double incidentEnergy,
					      G4double mass,
					      G4double deltaEnergy) const;

private:

  // Hide copy constructor and assignment operator 
  G4hShellCrossSection(const G4hShellCrossSection&);
  G4hShellCrossSection & operator = (const G4hShellCrossSection &right);

};

#endif

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
// File name:  G4hShellCrossSectionDoubleExp   
//
// Author:     Simona Saliceti (simona.saliceti@ge.infn.it)
// 
// History:
// -----------
// 31/05/2004 Simona Saliceti 1st implementation
// -------------------------------------------------------------------
// Class Description: 
// Empiric Model for shell cross sections in proton ionisation
// -------------------------------------------------------------------
// $Id: G4hShellCrossSectionDoubleExp.hh,v 1.1 2004-09-29 08:34:35 saliceti Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#ifndef G4HSHELLCROSSSECTIONDOUBLEEXP_HH
#define G4HSHELLCROSSSECTIONDOUBLEEXP_HH 1

#include "globals.hh"
#include "G4VhShellCrossSection.hh" 
#include "G4hShellCrossSectionDoubleExpData.hh"

class G4hShellCrossSectionDoubleExp : public G4VhShellCrossSection
{
public:

  G4hShellCrossSectionDoubleExp();

  virtual ~G4hShellCrossSectionDoubleExp();

  virtual std::vector<G4double> GetCrossSection(G4int Z,
						G4double incidentEnergy,
						G4double mass,
						G4double deltaEnergy,
						G4bool testFlag = false) const;
 
  void SetTotalCS(G4double);
 
protected:

  virtual std::vector<G4double> Probabilities(G4int Z,
					      G4double incidentEnergy,
					      G4double mass,
					      G4double deltaEnergy) const;


private:

  G4double GetCrossSectionDoubleExp(G4int Z,
			            G4double incidentEnergy) const;

  // Hide copy constructor and assignment operator 
  G4hShellCrossSectionDoubleExp(const G4hShellCrossSectionDoubleExp&);
  G4hShellCrossSectionDoubleExp & operator = (const G4hShellCrossSectionDoubleExp &right);
  G4hShellCrossSectionDoubleExpData* kShellData;

  G4double atomTotalCrossSection;
};

#endif

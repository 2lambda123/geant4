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
// $Id: G4teoCrossSection.hh,v 1.3 2009-06-17 16:39:55 mantero Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//         
//
// History:
// -----------
//  21 Apr 2008   ALF  1st implementation
//  29 Apr 2009   ALF Updated Desing for Integration
//
// -------------------------------------------------------------------

// Class description:
// Low Energy Electromagnetic Physics, Cross section, p ionisation, K shell
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------


#ifndef G4TEOCROSSSECTION_HH
#define G4ETEOCROSSSECTION_HH 1

#include "globals.hh"
#include "G4VhShellCrossSection.hh"

#include "G4ecpssrKCrossSection.hh"
#include "G4ecpssrLiCrossSection.hh"

class G4teoCrossSection : public G4VhShellCrossSection 
{
public:

  G4teoCrossSection(G4String);

  ~G4teoCrossSection();
			     

  std::vector<G4double> GetCrossSection(G4int Z,
					G4double incidentEnergy,
					G4double mass,
					G4double deltaEnergy,
					G4bool testFlag = false) const;

  std::vector<G4double> Probabilities(G4int Z,
				      G4double incidentEnergy,
				      G4double mass,
				      G4double deltaEnergy) const;
  
  
  void SetTotalCS(G4double);
  
  
  
private:
  
  G4double totalCS;
              
  G4ecpssrKCrossSection*  ecpssrShellK;
  G4ecpssrLiCrossSection*  ecpssrShellLi;
			
			
  G4teoCrossSection(const G4teoCrossSection&);
  G4teoCrossSection & operator = (const G4teoCrossSection &right);
  
};

#endif

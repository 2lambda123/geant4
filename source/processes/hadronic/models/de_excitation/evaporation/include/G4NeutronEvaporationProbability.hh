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
// $Id: G4NeutronEvaporationProbability.hh,v 1.2 2004-12-07 13:46:36 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999) 
//



#ifndef G4NeutronEvaporationProbability_h
#define G4NeutronEvaporationProbability_h 1


#include "G4EvaporationProbability.hh"


class G4NeutronEvaporationProbability : public G4EvaporationProbability
{
public:
  // Only available constructor
  G4NeutronEvaporationProbability();
		
  ~G4NeutronEvaporationProbability() {}
private:  
  // Copy constructor
  G4NeutronEvaporationProbability(const G4NeutronEvaporationProbability &right);

  const G4NeutronEvaporationProbability & operator=(const G4NeutronEvaporationProbability &right);
  G4bool operator==(const G4NeutronEvaporationProbability &right) const;
  G4bool operator!=(const G4NeutronEvaporationProbability &right) const;
  

private:

  virtual G4double CalcAlphaParam(const G4Fragment & fragment) const 
  { return 0.76+2.2/std::pow(static_cast<G4double>(fragment.GetA()-GetA()),1.0/3.0);}
	
  virtual G4double CalcBetaParam(const G4Fragment & fragment) const 
  { return (2.12/std::pow(static_cast<G4double>(fragment.GetA()-GetA()),2.0/3.0) - 0.05)*MeV/
      CalcAlphaParam(fragment); }

  // Excitation energy levels 
  std::vector<G4double> ExcitEnergies;
  // Spin of excitation energy levels 
  std::vector<G4int> ExcitSpins;

};


#endif

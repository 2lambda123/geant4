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
// $Id: G4EvaporationLevelDensityParameter.hh,v 1.2 2005-06-04 13:29:20 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998) 
//



#ifndef G4EvaporationLevelDensityParameter_h
#define G4EvaporationLevelDensityParameter_h 1


#include "G4VLevelDensityParameter.hh"
#include "G4CameronTruranHilfShellCorrections.hh"

class G4EvaporationLevelDensityParameter : public G4VLevelDensityParameter
{
public:

  G4EvaporationLevelDensityParameter()  {};

  virtual ~G4EvaporationLevelDensityParameter() {};

private:  
	
  G4EvaporationLevelDensityParameter(const G4EvaporationLevelDensityParameter &right);

  const G4EvaporationLevelDensityParameter & operator=(const G4EvaporationLevelDensityParameter &right);
  G4bool operator==(const G4EvaporationLevelDensityParameter &right) const;
  G4bool operator!=(const G4EvaporationLevelDensityParameter &right) const;
  
public:
  G4double LevelDensityParameter(const G4int A,const G4int Z,const G4double U) const;

private:

  G4double ShellCorrection(const G4int Z, const G4int N) const
  { 
    G4CameronTruranHilfShellCorrections* SPtr = G4CameronTruranHilfShellCorrections::GetInstance();
    return SPtr->GetShellZ(Z) + SPtr->GetShellN(N);
  }
		  		
private:

  static const G4double ConstEvapLevelDensityParameter;
  static const G4double alpha;
  static const G4double beta;
  static const G4double gamma;
  static const G4double Bs;
};


#endif

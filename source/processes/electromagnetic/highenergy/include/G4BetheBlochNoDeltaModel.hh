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
// $Id: G4BetheBlochNoDeltaModel.hh,v 1.2 2005-05-18 11:02:10 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4BetheBlochNoDeltaModel
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 18.05.2005
//
// Modifications:
//
//
// Class Description:
//
// Implementation of Bethe-Bloch model of energy loss without delta-ray

// -------------------------------------------------------------------
//

#ifndef G4BetheBlochNoDeltaModel_h
#define G4BetheBlochNoDeltaModel_h 1

#include "G4BetheBlochModel.hh"

class G4BetheBlochNoDeltaModel : public G4BetheBlochModel
{

public:

  G4BetheBlochNoDeltaModel(G4double ch = 1.0, const G4ParticleDefinition* p = 0,
    const G4String& nam = "BetheBlochNoD");

  virtual ~G4BetheBlochNoDeltaModel();

  virtual G4double ComputeDEDXPerVolume(const G4Material*,
					const G4ParticleDefinition*,
					G4double kineticEnergy,
					G4double cutEnergy);

  virtual G4double CrossSectionPerVolume(const G4Material*,
					 const G4ParticleDefinition*,
					 G4double kineticEnergy,
					 G4double cutEnergy,
					 G4double maxEnergy);

private:

  // hide assignment operator
  G4BetheBlochNoDeltaModel & operator=(const  G4BetheBlochNoDeltaModel &right);
  G4BetheBlochNoDeltaModel(const  G4BetheBlochNoDeltaModel&);

  G4double mcharge2;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4BetheBlochNoDeltaModel::G4BetheBlochNoDeltaModel(G4double ch,
         const G4ParticleDefinition*p, const G4String& nam) :
	 G4BetheBlochModel(p, nam), mcharge2(ch*ch)
{}

inline G4BetheBlochNoDeltaModel::~G4BetheBlochNoDeltaModel()
{}

inline G4double G4BetheBlochNoDeltaModel::ComputeDEDXPerVolume(
                            const G4Material* material,
			    const G4ParticleDefinition* pd,
                            G4double kinEnergy, G4double)
{
  G4double dedx = G4BraggIonModel::ComputeDEDXPerVolume(material, pd, kinEnergy, DBL_MAX);
  if(mcharge2 > 2.0) {
    G4double m = pd->GetPDGMass();
    dedx *= mcharge2*kinEnergy*(kinEnergy + 2.0*m)/((kinEnergy + m)*(kinEnergy + m));
  }
  return dedx;
}

inline G4double G4BetheBlochNoDeltaModel::CrossSectionPerVolume(
                            const G4Material*,
			    const G4ParticleDefinition*,
			    G4double, G4double, G4double)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif

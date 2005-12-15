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
// $Id: TiaraPrimaryGeneratorAction.cc,v 1.4 2005-12-15 14:25:33 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "globals.hh"

#include "TiaraPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Neutron.hh"
#include "G4Geantino.hh"
#include "G4ThreeVector.hh"
#include "TiaraDimensions.hh"
#include "TiaraVSourceEnergyGenerator.hh"
#include "TiaraVDirectionGenerator.hh"

TiaraPrimaryGeneratorAction::
TiaraPrimaryGeneratorAction(const TiaraVSourceEnergyGenerator& eG,
			    const TiaraVDirectionGenerator& dG,
			    const TiaraTally &tally,
			    const TiaraDimensions &tiaraDimensions) :
  fEnergyGenerator(eG.Clone()),
  fDirectionGenerator(dG.Clone()), 
  particleGun(new G4ParticleGun(1)),
  fTally(tally)
{
  particleGun->
    SetParticlePosition(G4ThreeVector(0.0, 0.0, 
				      tiaraDimensions.targetPosZ));
}

TiaraPrimaryGeneratorAction::~TiaraPrimaryGeneratorAction()
{
  delete particleGun;
  delete fDirectionGenerator;
  delete fEnergyGenerator;
}

TiaraPrimaryGeneratorAction::
TiaraPrimaryGeneratorAction(const TiaraPrimaryGeneratorAction &rhs)
  : G4VUserPrimaryGeneratorAction()
{  
  *this = rhs;
}

TiaraPrimaryGeneratorAction &
TiaraPrimaryGeneratorAction::
operator=(const TiaraPrimaryGeneratorAction &rhs)
{  
  if (this != &rhs) {
    fEnergyGenerator = rhs.fEnergyGenerator->Clone();
    fDirectionGenerator = rhs.fDirectionGenerator->Clone(); 
    particleGun = new G4ParticleGun(1);
    fTally = rhs.fTally;
  }
  return *this;
}

void TiaraPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->SetParticleDefinition(G4Neutron::NeutronDefinition());
  particleGun->SetParticleMomentumDirection(fDirectionGenerator->
					    GetDirection());  


  G4double e(fEnergyGenerator->GetEnergy());
  fTally.fill(e, 1);
  fTally.EndOfEventAction();
  particleGun->SetParticleEnergy(e);

  particleGun->GeneratePrimaryVertex(anEvent);
}

const TiaraVSourceEnergyGenerator *TiaraPrimaryGeneratorAction::
GetEnergyGenerator() const {
  return fEnergyGenerator;
}

const TiaraTally &
TiaraPrimaryGeneratorAction::GetTally() const {
  return fTally;
}

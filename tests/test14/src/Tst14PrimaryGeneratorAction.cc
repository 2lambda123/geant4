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
// $Id: Tst14PrimaryGeneratorAction.cc,v 1.7 2004-12-02 15:39:24 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "Tst14PrimaryGeneratorAction.hh"

#include "Tst14DetectorConstruction.hh"
//#include "Tst14PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
 G4String Tst14PrimaryGeneratorAction::thePrimaryParticleName="e-" ; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Tst14PrimaryGeneratorAction::Tst14PrimaryGeneratorAction(
                                            Tst14DetectorConstruction* Tst14DC)
:Tst14Detector(Tst14DC),rndmFlag("off"),xvertex(0.),yvertex(0.),zvertex(0.),
 vertexdefined(false)
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  //  gunMessenger = new Tst14PrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="e-");
  particleGun->SetParticleDefinition(particle);
  
  thePrimaryParticleName = particle->GetParticleName() ;

  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(30.*MeV);

  zvertex = -0.5*(Tst14Detector->GetAbsorberThickness());
  particleGun->SetParticlePosition(G4ThreeVector(xvertex,yvertex,zvertex));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Tst14PrimaryGeneratorAction::~Tst14PrimaryGeneratorAction()
{
  delete particleGun;
  //  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Tst14PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 
  thePrimaryParticleName = particleGun->GetParticleDefinition()->
                                                GetParticleName() ;
  G4double x0,y0,z0 ;
  if(vertexdefined)
  {
    x0 = xvertex ;
    y0 = yvertex ;
    z0 = zvertex ;
  }
  else
  {
    x0 = 0. ;
    y0 = 0. ;
    //z0 = -0.5*(Tst14Detector->GetWorldSizeZ()) ;
    z0 = Tst14Detector->GetzstartAbs() ;
  }
  G4double r0,phi0 ;
  if (rndmFlag == "on")
     {r0 = (Tst14Detector->GetAbsorberRadius())*std::sqrt(G4UniformRand());
      phi0 = twopi*G4UniformRand();
      x0 = r0*std::cos(phi0);
      y0 = r0*std::sin(phi0);
     } 
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4String Tst14PrimaryGeneratorAction::GetPrimaryName()
{
   return thePrimaryParticleName ;
}

void Tst14PrimaryGeneratorAction::Setzvertex(G4double z)
{
  vertexdefined = true ;
  zvertex = z ;
  G4cout << " Z coordinate of the primary vertex = " << zvertex/mm <<
            " mm." << G4endl;
}
void Tst14PrimaryGeneratorAction::Setxvertex(G4double x)
{
  vertexdefined = true ;
  xvertex = x ;
  G4cout << " X coordinate of the primary vertex = " << xvertex/mm <<
            " mm." << G4endl;
}

void Tst14PrimaryGeneratorAction::Setyvertex(G4double y)
{
  vertexdefined = true ;
  yvertex = y ;
  G4cout << " Y coordinate of the primary vertex = " << yvertex/mm <<
            " mm." << G4endl;
}

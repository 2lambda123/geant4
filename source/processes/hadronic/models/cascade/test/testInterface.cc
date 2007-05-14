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

#include "globals.hh"
#include "G4IonTable.hh"

#include "G4InuclCollider.hh"
#include "G4IntraNucleiCascader.hh"
#include "G4ElementaryParticleCollider.hh"
#include "G4NonEquilibriumEvaporator.hh"
#include "G4EquilibriumEvaporator.hh"
#include "G4Fissioner.hh"
#include "G4BigBanger.hh"
#include "G4InuclElementaryParticle.hh"
#include "G4InuclNuclei.hh"
#include "G4InuclParticle.hh"
#include "G4CollisionOutput.hh"
#include "G4Nucleus.hh"
#include "G4NucleiModel.hh"

typedef std::vector<G4InuclElementaryParticle>::iterator particleIterator;
typedef std::vector<G4InuclNuclei>::iterator nucleiIterator;

int main(int argc, char **argv ) {

  G4int verboseLevel = 3;

  enum particleType { nuclei = 0, proton = 1, neutron = 2, pionPlus = 3, pionMinus = 5, pionZero = 7, photon = 10 };

  G4int bulletType = 0;

  std::vector<G4double>  momentumBullet(4, 0.0);
  momentumBullet[0] = 1.37126;
  momentumBullet[3] = 1.5;

  G4InuclParticle *  bullet = new G4InuclElementaryParticle(momentumBullet, 1); 

  G4InuclNuclei*   target  = NULL;
  G4InuclParticle* targetH = NULL;
  G4NucleiModel*   model = NULL;

  G4double theNucleusA = 1;
  std::vector<G4double> targetMomentum(4, 0.0);

  G4CollisionOutput output;

  G4ElementaryParticleCollider*   colep = new G4ElementaryParticleCollider;
  G4IntraNucleiCascader*            inc = new G4IntraNucleiCascader; // the actual cascade
  inc->setInteractionCase(1); // Interaction type is particle with nuclei.

  G4NonEquilibriumEvaporator*     noneq = new G4NonEquilibriumEvaporator;
  G4EquilibriumEvaporator*         eqil = new G4EquilibriumEvaporator;
  G4Fissioner*                     fiss = new G4Fissioner;
  G4BigBanger*                     bigb = new G4BigBanger;

  G4InuclCollider*             collider = new G4InuclCollider(colep, inc, noneq, eqil, fiss, bigb);

  for (G4int i = 1; i< 100 ; i++) {
    if ( theNucleusA < 1.5 ) 
      {
	model = new G4NucleiModel(new G4InuclNuclei(targetMomentum, 1, 1));
	targetH = new G4InuclElementaryParticle((model->generateNucleon(1, 1)).getMomentum(), 1); 
   
	//		do
	  {
	    G4cout << "+";
	    output = collider->collide(bullet, targetH); 
	  } 
	  //		while(output.getOutgoingParticles().size()<2.5);
      } 
    else 
      {
	output = collider->collide(bullet, target ); 
      }

    if (verboseLevel > 1) 
      {
	G4cout << " Cascade output: " << G4endl;
	output.printCollisionOutput();
      }
  
    // Convert cascade data to use hadronics interface

    std::vector<G4InuclNuclei>             nucleiFragments = output.getNucleiFragments();
    std::vector<G4InuclElementaryParticle> particles =       output.getOutgoingParticles();

    G4int numSecondaries = nucleiFragments.size()+particles.size();
    G4cout << "num secondaries: " << numSecondaries << G4endl;
    if(!particles.empty()) { 
      particleIterator ipart;
      G4int outgoingParticle;

      for(ipart = particles.begin(); ipart != particles.end(); ipart++) {
	outgoingParticle = ipart->type();
	std::vector<G4double> mom = ipart->getMomentum();
	G4double ekin = ipart->getKineticEnergy() * GeV;
	G4ThreeVector aMom(mom[1], mom[2], mom[3]);
	aMom = aMom.unit();

      
      }

      nucleiIterator ifrag;

      for(ifrag = nucleiFragments.begin(); ifrag != nucleiFragments.end(); ifrag++) 
	{
	  G4double eKin = ifrag->getKineticEnergy() * GeV;
	  std::vector<G4double> mom = ifrag->getMomentum();
	  G4ThreeVector aMom(mom[1], mom[2], mom[3]);
	  aMom = aMom.unit();

	  // hpw @@@ ==> Should be zero: G4double fragmentExitation = ifrag->getExitationEnergyInGeV();

	  if (verboseLevel > 2) {
	    G4cout << " Nuclei fragment: " << G4endl;
	    ifrag->printParticle();
	  }
	  G4int A = G4int(ifrag->getA());
	  G4int Z = G4int(ifrag->getZ());

	}
    }
  }
}





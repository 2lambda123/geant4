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
#include "G4ElasticCascadeInterface.hh"
#include "globals.hh"
#include "G4DynamicParticleVector.hh"
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
#include "G4V3DNucleus.hh"
#include "G4Track.hh"
#include "G4Nucleus.hh"
#include "G4NucleiModel.hh"
#include "G4LorentzRotation.hh"


typedef std::vector<G4InuclElementaryParticle>::iterator particleIterator;
typedef std::vector<G4InuclNuclei>::iterator nucleiIterator;

G4ElasticCascadeInterface::G4ElasticCascadeInterface()
  :verboseLevel(0)
{
  if (verboseLevel > 3) {
    G4cout << " >>> G4ElasticCascadeInterface::G4ElasticCascadeInterface" << G4endl;
  }
}
   
G4ReactionProductVector* G4ElasticCascadeInterface::Propagate(G4KineticTrackVector*, G4V3DNucleus* ) {
  return NULL;
}

// #define debug_G4ElasticCascadeInterface

G4HadFinalState* G4ElasticCascadeInterface::ApplyYourself(const G4HadProjectile& aTrack, 
						     G4Nucleus& theNucleus) {
#ifdef debug_G4ElasticCascadeInterface
  static G4int counter(0);
  counter++;
  G4cerr << "Reaction number "<< counter << " "<<aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName()<<" "<< aTrack.GetDynamicParticle()->GetKineticEnergy()<<G4endl;
#endif

  theResult.Clear();

  if (verboseLevel > 3) {
    G4cout << " >>> G4ElasticCascadeInterface::ApplyYourself" << G4endl;
  };

  G4double eInit     = 0.0;
  G4double eTot      = 0.0;
  G4double sumBaryon = 0.0;
  G4double sumEnergy = 0.0;

  // Make conversion between native Geant4 and Bertini cascade classes.
  // NOTE: Geant4 units are MeV = 1 and GeV = 1000. Cascade code by default use GeV = 1.

  enum particleType { nuclei = 0, proton = 1, neutron = 2, pionPlus = 3, pionMinus = 5, pionZero = 7, photon = 10 };

  G4int bulletType = 0;

  // Coding particles 
  if (aTrack.GetDefinition() ==    G4Proton::Proton()    ) bulletType = proton;
  if (aTrack.GetDefinition() ==   G4Neutron::Neutron()   ) bulletType = neutron;
  if (aTrack.GetDefinition() ==  G4PionPlus::PionPlus()  ) bulletType = pionPlus;
  if (aTrack.GetDefinition() == G4PionMinus::PionMinus() ) bulletType = pionMinus;
  if (aTrack.GetDefinition() ==  G4PionZero::PionZero()  ) bulletType = pionZero;
  if (aTrack.GetDefinition() ==     G4Gamma::Gamma()     ) bulletType = photon;

  // Code momentum and energy.
  G4double px,py,pz;
  px=aTrack.Get4Momentum().px() / GeV;
  py=aTrack.Get4Momentum().py() / GeV;
  pz=aTrack.Get4Momentum().pz() / GeV;

  G4LorentzVector projectileMomentum = aTrack.Get4Momentum();
  G4LorentzRotation toZ;
  toZ.rotateZ(-projectileMomentum.phi());
  toZ.rotateY(-projectileMomentum.theta());
  G4LorentzRotation toLabFrame = toZ.inverse();

  std::vector<G4double> momentumBullet(4);
  momentumBullet[0] =0.;
  momentumBullet[1] =0;
  momentumBullet[2] =0;
  momentumBullet[3] =std::sqrt(px*px+py*py+pz*pz);

  G4InuclElementaryParticle *  bullet = new G4InuclElementaryParticle(momentumBullet, bulletType); 

  sumEnergy = bullet->getKineticEnergy(); // In GeV 
  if (bulletType == proton || bulletType == neutron) {
    sumBaryon += 1;
  } 

  // Set target
  G4InuclNuclei*   target  = NULL;
  G4InuclParticle* targetH = NULL;
  // and outcoming particles
  G4DynamicParticle* cascadeParticle = NULL;

  std::vector<G4double> targetMomentum(4, 0.0);

  G4double theNucleusA = theNucleus.GetN();

  if ( !(G4int(theNucleusA) == 1) ) {
    target  = new G4InuclNuclei(targetMomentum, 
				theNucleusA, 
				theNucleus.GetZ());
    target->setEnergy();

    std::vector<G4double>  bmom = bullet->getMomentum();
    eInit = std::sqrt(bmom[0] * bmom[0]);
    std::vector<G4double> tmom = target->getMomentum();
    eInit += std::sqrt(tmom[0] * tmom[0]);

    sumBaryon += theNucleusA;

    if (verboseLevel > 2) {
      G4cout << "Bullet:  " << G4endl;  
      bullet->printParticle();
    }
    if (verboseLevel > 2) {
      G4cout << "Target:  " << G4endl;  
      target->printParticle();
    }
  }

  G4CollisionOutput output;

  // Colliders initialisation
  G4ElementaryParticleCollider*   colep = new G4ElementaryParticleCollider;
  G4IntraNucleiCascader*            inc = new G4IntraNucleiCascader; // the actual cascade
  inc->setInteractionCase(1); // Interaction type is particle with nuclei.

  G4NonEquilibriumEvaporator*     noneq = new G4NonEquilibriumEvaporator;
  G4EquilibriumEvaporator*         eqil = new G4EquilibriumEvaporator;
  G4Fissioner*                     fiss = new G4Fissioner;
  G4BigBanger*                     bigb = new G4BigBanger;
  G4InuclCollider*             collider = new G4InuclCollider(colep, inc, noneq, eqil, fiss, bigb);

      G4int  maxTries = 10; // maximum tries for inelastic collision to avoid infinite loop
      G4int  nTries   = 0;  // try counter

      if (G4int(theNucleusA) == 1) { // special treatment for target H(1,1) (proton)

	targetH = new G4InuclElementaryParticle(targetMomentum, 1);

	G4float cutElastic[8];
	cutElastic[proton   ] = 1.0; // GeV
	cutElastic[neutron  ] = 1.0;
	cutElastic[pionPlus ] = 0.6;
	cutElastic[pionMinus] = 0.2;
	cutElastic[pionZero ] = 0.2;

	//	Was >
	if (momentumBullet[3] < cutElastic[bulletType]) { // elastic collision possible

	  do {   // we try to create elastic interaction
	    output = collider->collide(bullet, targetH);
	    nTries++;
	  } while(
		  (nTries < maxTries)                                           &&
		  (output.getOutgoingParticles().size() != 2                    && // elastic: bullet + p = H(1,1) coming out
		   (output.getOutgoingParticles().begin()->type() != bulletType ||
		    output.getOutgoingParticles().begin()->type() != proton) //changed all != -> ==
		   )
		  );

	} else { // only elastic collision is energetically possible
	  output = collider->collide(bullet, targetH);
	}

	sumBaryon += 1;

	std::vector<G4double> bmom = bullet->getMomentum();
	eInit = std::sqrt(bmom[0] * bmom[0]);
	std::vector<G4double> tmom = targetH->getMomentum();
	eInit += std::sqrt(tmom[0] * tmom[0]);

	if (verboseLevel > 2) {
	  G4cout << "Target:  " << G4endl;
	  targetH->printParticle();
	}

      } else {  // treat all other targets excepet H(1,1)

	do  // we try to create inelastic interaction ELASTIC COLLISION
	  {
	    output = collider->collide(bullet, target );
	    nTries++;
	  } while(
		   (nTries < maxTries)                                                               &&
		   //changed < to >
		   //(output.getOutgoingParticles().size() + output.getNucleiFragments().size() < 2.5) &&
		   (output.getOutgoingParticles().size() + output.getNucleiFragments().size() != 2) &&
		   (output.getOutgoingParticles().size()==0) && // != -> ==
                   (output.getOutgoingParticles().begin()->type() != bullet->type()) //changed == -> !=
		  );
      }

  if (verboseLevel > 1) 
    {
      G4cout << " Cascade output: " << G4endl;
      output.printCollisionOutput();
    }
  
  // Convert cascade data to use hadronics interface
  std::vector<G4InuclNuclei>             nucleiFragments = output.getNucleiFragments();
  std::vector<G4InuclElementaryParticle> particles =       output.getOutgoingParticles();

  theResult.SetStatusChange(stopAndKill);

  if (!particles.empty()) { 
    particleIterator ipart;
    G4int outgoingParticle;

    for (ipart = particles.begin(); ipart != particles.end(); ipart++) {
      outgoingParticle = ipart->type();
      std::vector<G4double> mom = ipart->getMomentum();
      eTot   += std::sqrt(mom[0] * mom[0]);

      G4double ekin = ipart->getKineticEnergy() * GeV;
      G4ThreeVector aMom(mom[1], mom[2], mom[3]);
      aMom = aMom.unit();

      if (outgoingParticle == proton ||  outgoingParticle == neutron) {
	sumBaryon -= 1;
      } 

      sumEnergy -= ekin / GeV;

      switch(outgoingParticle) {

      case proton: 
#ifdef debug_G4ElasticCascadeInterface
	G4cerr << "proton " << counter << " " << aMom << " " << ekin << G4endl;
#endif
	cascadeParticle = 
	  new G4DynamicParticle(G4Proton::ProtonDefinition(), aMom, ekin);
	break; 

      case neutron: 

#ifdef debug_G4ElasticCascadeInterface
	G4cerr << "neutron "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	cascadeParticle = 
	  new G4DynamicParticle(G4Neutron::NeutronDefinition(), aMom, ekin);
	break;

      case pionPlus: 
	cascadeParticle = 
	  new G4DynamicParticle(G4PionPlus::PionPlusDefinition(), aMom, ekin);

#ifdef debug_G4ElasticCascadeInterface
	G4cerr << "pionPlus "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case pionMinus:
	cascadeParticle = 
	  new G4DynamicParticle(G4PionMinus::PionMinusDefinition(), aMom, ekin);

#ifdef debug_G4ElasticCascadeInterface
	G4cerr << "pionMinus "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case pionZero: 
	cascadeParticle = 
	  new G4DynamicParticle(G4PionZero::PionZeroDefinition(), aMom, ekin);

#ifdef debug_G4ElasticCascadeInterface
	G4cerr << "pionZero "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case photon: 
	cascadeParticle = 
	  new G4DynamicParticle(G4Gamma::Gamma(), aMom, ekin);

#ifdef debug_G4ElasticCascadeInterface
	G4cerr << "photon "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      default:
        G4cout << " ERROR: G4ElasticCascadeInterface::Propagate undefined particle type"
	       << G4endl;
      }

      cascadeParticle->Set4Momentum(cascadeParticle->Get4Momentum()*=toLabFrame);
      theResult.AddSecondary(cascadeParticle); 
    }
  }

  // get nuclei fragments
  G4DynamicParticle * aFragment = NULL;
  G4ParticleDefinition * aIonDef = 0;
  G4ParticleTable *theTableOfParticles = G4ParticleTable::GetParticleTable();

  if (!nucleiFragments.empty()) { 
    nucleiIterator ifrag;

    for (ifrag = nucleiFragments.begin(); ifrag != nucleiFragments.end(); ifrag++) 
      {
	G4double eKin = ifrag->getKineticEnergy() * GeV;
	std::vector<G4double> mom = ifrag->getMomentum();
        eTot   += std::sqrt(mom[0] * mom[0]);

	G4ThreeVector aMom(mom[1], mom[2], mom[3]);
	aMom = aMom.unit();

	// hpw @@@ ==> Should be zero: G4double fragmentExitation = ifrag->getExitationEnergyInGeV();

	if (verboseLevel > 2) {
	  G4cout << " Nuclei fragment: " << G4endl;
	  ifrag->printParticle();
	}

	G4int A = G4int(ifrag->getA());
	G4int Z = G4int(ifrag->getZ());
	aIonDef = theTableOfParticles->FindIon(Z, A, 0, Z);
      
	aFragment =  new G4DynamicParticle(aIonDef, aMom, eKin);

	sumBaryon -= A;
	sumEnergy -= eKin / GeV;

        aFragment->Set4Momentum(aFragment->Get4Momentum()*=toLabFrame);
	theResult.AddSecondary(aFragment); 
      }
  }

  if (verboseLevel > 2) {
    if (sumBaryon != 0) {
      G4cout << "ERROR: no baryon number conservation, sum of baryons = "
             << sumBaryon << G4endl;
    }

    if (sumEnergy > 0.01 ) {
      G4cout << "Kinetic energy conservation violated by "
	     << sumEnergy << " GeV" << G4endl;
    }
     
    G4cout << "Total energy conservation at level ~"
	   << (eInit - eTot) * GeV << " MeV" << G4endl;
    
    if (sumEnergy < -5.0e-5 ) { // 0.05 MeV
      G4cout << "FATAL ERROR: energy created  "
             << sumEnergy * GeV << " MeV" << G4endl;
    }
  }

  delete bullet;
  delete colep;
  delete inc;
  delete noneq; 
  delete fiss;
  delete eqil;
  delete bigb;
  delete collider;

  if(target != NULL) delete target;
  if(targetH != NULL) delete targetH;
 // if(cascadeParticle != NULL) delete cascadeParticle;
 // if(aFragment != NULL) delete aFragment;

  return &theResult;
}

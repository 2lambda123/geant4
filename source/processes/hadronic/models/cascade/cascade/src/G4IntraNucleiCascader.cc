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
// $Id: G4IntraNucleiCascader.cc,v 1.31 2010-03-16 23:54:21 mkelsey Exp $
// Geant4 tag: $Name: not supported by cvs2svn $
//
// 20100114  M. Kelsey -- Remove G4CascadeMomentum, use G4LorentzVector directly
// 20100307  M. Kelsey -- Bug fix: momentum_out[0] should be momentum_out.e()

#define RUN

#include "G4IntraNucleiCascader.hh"
#include "G4InuclElementaryParticle.hh"
#include "G4InuclNuclei.hh"
#include "G4LorentzConvertor.hh"
#include "G4ParticleLargerEkin.hh"
#include "G4NucleiModel.hh"
#include "G4CascadParticle.hh"
#include "G4NucleiProperties.hh"
#include "G4HadTmpUtil.hh"

#include "Randomize.hh"
#include <algorithm>

typedef std::vector<G4InuclElementaryParticle>::iterator particleIterator;

G4IntraNucleiCascader::G4IntraNucleiCascader()
  : verboseLevel(0) {

  if (verboseLevel > 3) {
    G4cout << " >>> G4IntraNucleiCascader::G4IntraNucleiCascader" << G4endl;
  }

}

G4CollisionOutput G4IntraNucleiCascader::collide(G4InuclParticle* bullet,
						 G4InuclParticle* target) {

  if (verboseLevel > 3) {
    G4cout << " >>> G4IntraNucleiCascader::collide" << G4endl;
  }

  const G4int itry_max = 1000;
  const G4int reflection_cut = 500;
  //  const G4double eexs_cut = 0.0001;

  if (verboseLevel > 3) {
    bullet->printParticle();
    target->printParticle();
  }

  G4CollisionOutput output;

#ifdef RUN
  G4InuclNuclei* tnuclei = dynamic_cast<G4InuclNuclei*>(target);
  G4InuclNuclei* bnuclei = dynamic_cast<G4InuclNuclei*>(bullet);
  G4InuclElementaryParticle* bparticle = 
                          dynamic_cast<G4InuclElementaryParticle*>(bullet);
  G4NucleiModel model(tnuclei);
  G4double coulombBarrier = 0.00126*tnuclei->getZ()/
                                      (1.+std::pow(tnuclei->getA(),0.333));

  G4LorentzVector momentum_in = bullet->getMomentum();

  momentum_in.setE(momentum_in.e()+tnuclei->getMass());

  G4double ekin_in; 

  if (verboseLevel > 3) {
    model.printModel();
    G4cout << " intitial momentum  E " << momentum_in.e() << " Px "
	   << momentum_in.x() << " Py " << momentum_in.y() << " Pz "
	   << momentum_in.z() << G4endl;
  }

  G4int itry = 0;

  while (itry < itry_max) {
    itry++;
    model.reset();

    std::vector<G4CascadParticle> cascad_particles;
    G4ExitonConfiguration theExitonConfiguration;
    std::vector<G4InuclElementaryParticle> output_particles;
    G4double afin = tnuclei->getA();
    G4double zfin = tnuclei->getZ();
   
    if (inter_case == 1) { // particle with nuclei
      ekin_in = bparticle->getKineticEnergy();
      zfin += bparticle->getCharge();

      if (bparticle->baryon()) afin += 1.0;

      cascad_particles.push_back(model.initializeCascad(bparticle));

    } else { // nuclei with nuclei

      ekin_in = bnuclei->getKineticEnergy();

      G4double ab = bnuclei->getA();
      G4double zb = bnuclei->getZ();

      afin += ab;
      zfin += zb;

      std::pair<std::vector<G4CascadParticle>, std::vector<G4InuclElementaryParticle> > 
	all_particles = model.initializeCascad(bnuclei, tnuclei);

      cascad_particles = all_particles.first;

      for (G4int ip = 0; ip < G4int(all_particles.second.size()); ip++) 
	output_particles.push_back(all_particles.second[ip]);

      if (cascad_particles.size() == 0) { // compound nuclei
	G4int ia = G4int(ab + 0.5);
	G4int iz = G4int(zb + 0.5);
	G4int i;

	for (i = 0; i < ia; i++) {
	  G4int knd = i < iz ? 1 : 2;
	  theExitonConfiguration.incrementQP(knd);
	};

	G4int ihn = G4int(2.0 * (ab - zb) * inuclRndm() + 0.5);
	G4int ihz = G4int(2.0 * zb * inuclRndm() + 0.5);

	for (i = 0; i < ihn; i++) theExitonConfiguration.incrementHoles(2);

	for (i = 0; i < ihz; i++) theExitonConfiguration.incrementHoles(1);
      };
    }; 

    std::vector<G4CascadParticle> new_cascad_particles;
    G4int iloop = 0;

    while (!cascad_particles.empty() && !model.empty()) {
      iloop++;

      if (verboseLevel > 3) {
	G4cout << " Number of cparticles " << cascad_particles.size() << G4endl;
	cascad_particles.back().print();
      }

      new_cascad_particles = model.generateParticleFate(cascad_particles.back(),
							theElementaryParticleCollider);
      if (verboseLevel > 2) {
	G4cout << " ew particles " << new_cascad_particles.size() << G4endl;
      }

      // handle the result of a new step

      if (new_cascad_particles.size() == 1) { // last particle goes without interaction
	cascad_particles.pop_back();

	if (model.stillInside(new_cascad_particles[0])) { // particle survives 

	  if (verboseLevel > 3) {
	    G4cout << " still inside " << G4endl;
	  }

	  if (new_cascad_particles[0].getNumberOfReflections() < reflection_cut &&
	     model.worthToPropagate(new_cascad_particles[0])) { // it's ok

	    if (verboseLevel > 3) {
	      G4cout << " survives " << G4endl;
	    }

	    cascad_particles.push_back(new_cascad_particles[0]);

	  } else { // it becomes an exiton 

	    if (verboseLevel > 3) {
	      G4cout << " becomes an exiton " << G4endl;
	    }

	    theExitonConfiguration.incrementQP(new_cascad_particles[0].getParticle().type());
	  };  

        } else { // particle about to leave nucleus - check for Coulomb barrier

	  if (verboseLevel > 3) {
	    G4cout << " Goes out " << G4endl;
	    new_cascad_particles[0].print();
	  }
          G4InuclElementaryParticle currentParticle = new_cascad_particles[0].getParticle();
          G4double KE = currentParticle.getKineticEnergy();
          G4double mass = currentParticle.getMass();
          G4double Q = currentParticle.getCharge();
          if (KE < Q*coulombBarrier) {
     	    // Calculate barrier penetration
            G4double CBP = 0.0; 
	    // if (KE > 0.0001) CBP = std::exp(-0.00126*tnuclei->getZ()*0.25*
	    //   (1./KE - 1./coulombBarrier));
            if (KE > 0.0001) CBP = std::exp(-0.0181*0.5*tnuclei->getZ()*
                                            (1./KE - 1./coulombBarrier)*
                                         std::sqrt(mass*(coulombBarrier-KE)) );
            if (G4UniformRand() < CBP) {
	      output_particles.push_back(currentParticle);
            } else {
              theExitonConfiguration.incrementQP(currentParticle.type());
            }
          } else {
	    output_particles.push_back(currentParticle);
          }
        } 

      } else { // interaction 

	cascad_particles.pop_back();

	for (G4int i = 0; i < G4int(new_cascad_particles.size()); i++) 
	  cascad_particles.push_back(new_cascad_particles[i]);

	std::pair<G4int, G4int> holes = model.getTypesOfNucleonsInvolved();

	theExitonConfiguration.incrementHoles(holes.first);

	if (holes.second > 0) theExitonConfiguration.incrementHoles(holes.second);

      };
    };

    // Cascade is finished. Check if it's OK.

    if (verboseLevel > 3) {
      G4cout << " Cascade finished  " << G4endl
	     << " output_particles  " << output_particles.size() <<  G4endl;
    }

    G4LorentzVector momentum_out;
    particleIterator ipart;

    for (ipart = output_particles.begin(); ipart != output_particles.end(); ipart++) {
      momentum_out += ipart->getMomentum();

      zfin -= ipart->getCharge();
      if (ipart->baryon()) afin -= 1.0;
    };

    if (verboseLevel > 3) {
      G4cout << "  afin " << afin << " zfin " << zfin <<  G4endl;
    }

    if (afin > 1.0) {
      G4InuclNuclei outgoing_nuclei(afin, zfin);
      G4double mass = outgoing_nuclei.getMass();
      momentum_out.setE(momentum_out.e()+mass);

      momentum_out = momentum_in - momentum_out;

      if (verboseLevel > 3) {
	G4cout << "  Eex + Ekin " << momentum_out.e()  <<  G4endl;
      }

      if (momentum_out.e() > 0.0) { // Eex + Ekin > 0.0
	G4double pnuc = momentum_out.vect().mag2(); 
	G4double ekin = std::sqrt(mass * mass + pnuc) - mass;
	G4double Eex = 1000.0 * (momentum_out.e() - ekin);

	if (verboseLevel > 3) {
	  G4cout << "  Eex  " << Eex  <<  G4endl;
	}

	if (goodCase(afin, zfin, Eex, ekin_in)) { // ok, exitation energy > cut
	  std::sort(output_particles.begin(), output_particles.end(), G4ParticleLargerEkin());
	  output.addOutgoingParticles(output_particles);
	  outgoing_nuclei.setMomentum(momentum_out);
	  outgoing_nuclei.setExitationEnergy(Eex);
	  outgoing_nuclei.setExitonConfiguration(theExitonConfiguration);	                           	  
          output.addTargetFragment(outgoing_nuclei);

	  return output;
	};
      };

    } else { // special case, when one has no nuclei after the cascad

      if (afin == 1.0) { // recoiling nucleon

	momentum_out = momentum_in - momentum_out;

	G4InuclElementaryParticle  last_particle;

	if (zfin == 1.0) { // recoiling proton
	  last_particle.setType(1);

	} else { // neutron
	  last_particle.setType(2);
	}; 

	last_particle.setMomentum(momentum_out);
	output_particles.push_back(last_particle);
      }; 

      std::sort(output_particles.begin(), output_particles.end(), G4ParticleLargerEkin());
      output.addOutgoingParticles(output_particles);

      return output;
    }; 
  };

#else

  // special branch to avoid the cascad generation but to get the input for evaporation etc

  G4LorentzVector momentum_out;
  G4InuclNuclei outgoing_nuclei(169, 69);

  outgoing_nuclei.setMomentum(momentum_out);
  outgoing_nuclei.setExitationEnergy(150.0);

  G4ExitonConfiguration theExitonConfiguration(3.0, 3.0, 5.0, 6.0);

  outgoing_nuclei.setExitonConfiguration(theExitonConfiguration);	                           
  output.addTargetFragment(outgoing_nuclei);

  return output;

  /*
    G4InuclElementaryParticle* bparticle = dynamic_cast<G4InuclElementaryParticle*>
    (bullet);
    G4InuclNuclei* tnuclei = dynamic_cast<G4InuclNuclei*>(target);
    output.addOutgoingParticle(*bparticle);
    output.addTargetFragment(*tnuclei);
    return output;
  */

#endif

  if (verboseLevel > 3) {
    G4cout << " IntraNucleiCascader-> no inelastic interaction after " << itry_max << " attempts "
	   << G4endl;
  }

  output.trivialise(bullet, target);

  return output;
}

G4bool G4IntraNucleiCascader::goodCase(G4double a, 
				       G4double z, 
				       G4double eexs, 
				       G4double ein) const {

  if (verboseLevel > 3) {
    G4cout << " >>> G4IntraNucleiCascader::goodCase" << G4endl;
  }

  const G4double eexs_cut = 0.0001;
  const G4double reason_cut = 7.0;
  const G4double ediv_cut = 5.0;

  G4bool good = false;

  if (eexs > eexs_cut) {
    G4double eexs_max0z = 1000.0 * ein / ediv_cut;
    //    G4double dm = bindingEnergy(a, z);
    G4double dm = G4NucleiProperties::GetBindingEnergy(G4lrint(a), G4lrint(z));
    G4double eexs_max = eexs_max0z > reason_cut*dm ? eexs_max0z : reason_cut * dm;

    if(eexs < eexs_max) good = true;

    if (verboseLevel > 3) {
      G4cout << " eexs " << eexs << " max " << eexs_max << " dm " << dm << G4endl;
    }

  };

  return good; 
}

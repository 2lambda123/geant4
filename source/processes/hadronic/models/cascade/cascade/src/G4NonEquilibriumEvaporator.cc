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
// $Id: G4NonEquilibriumEvaporator.cc,v 1.27 2010-04-12 23:39:41 mkelsey Exp $
// Geant4 tag: $Name: not supported by cvs2svn $
//
// 20100114  M. Kelsey -- Remove G4CascadeMomentum, use G4LorentzVector directly
// 20100309  M. Kelsey -- Use new generateWithRandomAngles for theta,phi stuff;
//		eliminate some unnecessary std::pow()
// 20100413  M. Kelsey -- Pass G4CollisionOutput by ref to ::collide()

#define RUN

#include <cmath>
#include "G4NonEquilibriumEvaporator.hh"
#include "G4InuclElementaryParticle.hh"
#include "G4InuclNuclei.hh"
#include "G4InuclSpecialFunctions.hh"
#include "G4LorentzConvertor.hh"
#include "G4NucleiProperties.hh"
#include "G4HadTmpUtil.hh"

using namespace G4InuclSpecialFunctions;


G4NonEquilibriumEvaporator::G4NonEquilibriumEvaporator()
  : verboseLevel(0) {

  if (verboseLevel > 3) {
    G4cout << " >>> G4NonEquilibriumEvaporator::G4NonEquilibriumEvaporator" << G4endl;
  }
}

void G4NonEquilibriumEvaporator::collide(G4InuclParticle* /*bullet*/,
					 G4InuclParticle* target,
					 G4CollisionOutput& output) {

  if (verboseLevel > 3) {
    G4cout << " >>> G4NonEquilibriumEvaporator::collide" << G4endl;
  }

  const G4double a_cut = 5.0;
  const G4double z_cut = 3.0;

#ifdef RUN 
  const G4double eexs_cut = 0.1;
#else
  const G4double eexs_cut = 100000.0;
#endif

  const G4double coul_coeff = 1.4;
  const G4int itry_max = 1000;
  const G4double small_ekin = 1.0e-6;
  const G4double width_cut = 0.005;

  if (G4InuclNuclei* nuclei_target = dynamic_cast<G4InuclNuclei*>(target)) {
    //  initialization
    G4double A = nuclei_target->getA();
    G4double Z = nuclei_target->getZ();
    G4LorentzVector PEX = nuclei_target->getMomentum();
    G4LorentzVector pin = PEX;
    G4double EEXS = nuclei_target->getExitationEnergy();
    pin.setE(pin.e() + 0.001 * EEXS);
    G4InuclNuclei dummy_nuc;
    G4ExitonConfiguration config = nuclei_target->getExitonConfiguration();  

    G4double QPP = config.protonQuasiParticles;

    G4double QNP = config.neutronQuasiParticles; 

    G4double QPH = config.protonHoles;

    G4double QNH = config.neutronHoles; 

    G4double QP = QPP + QNP;
    G4double QH = QPH + QNH;
    G4double QEX = QP + QH;
    G4InuclElementaryParticle dummy(small_ekin, 1);
    G4LorentzConvertor toTheExitonSystemRestFrame;

    toTheExitonSystemRestFrame.setBullet(dummy.getMomentum(), dummy.getMass());

    G4double EFN = FermiEnergy(A, Z, 0);
    G4double EFP = FermiEnergy(A, Z, 1);

    G4double AR = A - QP;
    G4double ZR = Z - QPP;  
    G4int NEX = G4int(QEX + 0.5);
    G4LorentzVector ppout;
    G4bool try_again = NEX > 0 ? true : false;
  
    while (try_again) {

      if (A >= a_cut && Z >= z_cut && EEXS > eexs_cut) { // ok

	if (verboseLevel > 2) {
	  G4cout << " A " << A << " Z " << Z << " EEXS " << EEXS << G4endl; 
	}

	// update exiton system
	G4double nuc_mass = dummy_nuc.getNucleiMass(A, Z); 
	PEX.setVectM(PEX.vect(), nuc_mass);
	toTheExitonSystemRestFrame.setTarget(PEX, nuc_mass);
	toTheExitonSystemRestFrame.toTheTargetRestFrame();
	G4double MEL = getMatrixElement(A);
	G4double E0 = getE0(A);
	G4double PL = getParLev(A, Z);
	G4double parlev = PL / A;
	G4double EG = PL * EEXS;

	if (QEX < std::sqrt(2.0 * EG)) { // ok

	  std::pair<G4double, G4double> parms = paraMakerTruncated(Z);

	  G4double AK1 = parms.first;
	  G4double CPA1 = parms.second;
	  G4double VP = coul_coeff * Z * AK1 / (G4cbrt(A - 1.0) + 1.0) /
	    (1.0 + EEXS / E0);
	  //	  G4double DM1 = bindingEnergy(A, Z);
	  //	  G4double BN = DM1 - bindingEnergy(A - 1.0, Z);
	  //	  G4double BP = DM1 - bindingEnergy(A - 1.0, Z - 1.0);
          G4double DM1 = G4NucleiProperties::GetBindingEnergy(G4lrint(A), G4lrint(Z));
	  G4double BN = DM1 - G4NucleiProperties::GetBindingEnergy(G4lrint(A-1.0), G4lrint(Z));
	  G4double BP = DM1 - G4NucleiProperties::GetBindingEnergy(G4lrint(A-1.0), G4lrint(Z-1.0));
	  G4double EMN = EEXS - BN;
	  G4double EMP = EEXS - BP - VP * A / (A - 1.0);
	  G4double ESP = 0.0;
	
	  if (EMN > eexs_cut) { // ok
	    G4int icase = 0;
	  
	    if (NEX > 1) {
	      G4double APH = 0.25 * (QP * QP + QH * QH + QP - 3.0 * QH);
	      G4double APH1 = APH + 0.5 * (QP + QH);
	      ESP = EEXS / QEX;
	      G4double MELE = MEL / ESP / (A*A*A);

	      if (ESP > 15.0) {
		MELE *= std::sqrt(15.0 / ESP);

	      } else if(ESP < 7.0) {
		MELE *= std::sqrt(ESP / 7.0);

		if (ESP < 2.0) MELE *= std::sqrt(ESP / 2.0);
	      };    
            
	      G4double F1 = EG - APH;
	      G4double F2 = EG - APH1;
	    
	      if (F1 > 0.0 && F2 > 0.0) {
		G4double F = F2 / F1;
		G4double M1 = 2.77 * MELE * PL;
		std::vector<G4double> D(3, 0.0);
		D[0] = M1 * F2 * F2 * std::pow(F, NEX - 1) / (QEX + 1.0);

		if (D[0] > 0.0) {

		  if (NEX >= 2) {
		    D[1] = 0.0462 / parlev / G4cbrt(A) * QP * EEXS / QEX;

		    if (EMP > eexs_cut) 
		      D[2] = D[1] * std::pow(EMP / EEXS, NEX) * (1.0 + CPA1);
		    D[1] *= std::pow(EMN / EEXS, NEX) * getAL(A);   

		    if (QNP < 1.0) D[1] = 0.0;

		    if (QPP < 1.0) D[2] = 0.0;

		    try_again = NEX > 1 && (D[1] > width_cut * D[0] || 
					    D[2] > width_cut * D[0]);

		    if (try_again) {
		      G4double D5 = D[0] + D[1] + D[2];
		      G4double SL = D5 * inuclRndm();
		      G4double S1 = 0.;

		      for (G4int i = 0; i < 3; i++) {
			S1 += D[i]; 	

			if (SL <= S1) {
			  icase = i;

			  break;
			};
		      };
		    }; 
		  };

		} else {
		  try_again = false;
		}; 

	      } else {
		try_again = false;
	      }; 
	    };  	  

	    if (try_again) {

	      if (icase > 0) { // N -> N - 1 with particle escape	     
		G4double V = 0.0;
		G4int ptype = 0;
		G4double B = 0.0;

		if (A < 3.0) try_again = false;

		if (try_again) { 

		  if (icase == 1) { // neutron escape

		    if (QNP < 1.0) { 
		      icase = 0;

		    } else {
		      B = BN;
		      V = 0.0;
		      ptype = 2;		  
		    };    

		  } else { // proton esape
		    if (QPP < 1.0) { 
		      icase = 0;

		    } else {
		      B = BP;
		      V = VP;
		      ptype = 1;

		      if (Z - 1.0 < 1.0) try_again = false;
		    };   
		  };
	        
		  if (try_again && icase != 0) {
		    G4double EB = EEXS - B;
		    G4double E = EB - V * A / (A - 1.0);

		    if (E < 0.0) {
		      icase = 0;

		    } else {
		      G4double E1 = EB - V;
		      G4double EEXS_new = -1.;
		      G4double EPART = 0.0;
		      G4int itry1 = 0;
		      G4bool bad = true;

		      while (itry1 < itry_max && icase > 0 && bad) {
			itry1++;
			G4int itry = 0;		    

			while (EEXS_new < 0.0 && itry < itry_max) {
			  itry++;
			  G4double R = inuclRndm();
			  G4double X;

			  if (NEX == 2) {
			    X = 1.0 - std::sqrt(R);

			  } else {		         
			    G4double QEX2 = 1.0 / QEX;
			    G4double QEX1 = 1.0 / (QEX - 1.0);
			    X = std::pow(0.5 * R, QEX2);

			    for (G4int i = 0; i < 1000; i++) {
			      G4double DX = X * QEX1 * 
				(1.0 + QEX2 * X * (1.0 - R / std::pow(X, NEX)) / (1.0 - X));
			      X -= DX;

			      if (std::fabs(DX / X) < 0.01) break;  

			    };
			  }; 
			  EPART = EB - X * E1;
			  EEXS_new = EB - EPART * A / (A - 1.0);
			};
                    
			if (itry == itry_max || EEXS_new < 0.0) {
			  icase = 0;

			} else { // real escape		        
			  G4InuclElementaryParticle particle(ptype);

                          particle.setModel(5);
			  G4double mass = particle.getMass();
			  EPART *= 0.001; // to the GeV
			  // generate particle momentum
			  G4double pmod = std::sqrt(EPART * (2.0 * mass + EPART));
			  G4LorentzVector mom = generateWithRandomAngles(pmod,mass);
			  G4LorentzVector mom_at_rest;

			  G4double QPP_new = QPP;
			  G4double Z_new = Z;
		        
			  if (ptype == 1) {
			    QPP_new -= 1.;
			    Z_new -= 1.0;
			  };

			  G4double QNP_new = QNP;

			  if (ptype == 2) QNP_new -= 1.0;

			  G4double A_new = A - 1.0;
			  G4double new_exiton_mass =
			    dummy_nuc.getNucleiMass(A_new, Z_new);
			  mom_at_rest.setVectM(-mom.vect(), new_exiton_mass); 

			  G4LorentzVector part_mom = 
		            toTheExitonSystemRestFrame.backToTheLab(mom);
			  part_mom.setVectM(part_mom.vect(), mass);

			  G4LorentzVector ex_mom = 
			    toTheExitonSystemRestFrame.backToTheLab(mom_at_rest);
			  ex_mom.setVectM(ex_mom.vect(), new_exiton_mass);   

			  //             check energy conservation and set new exitation energy
			  EEXS_new = 1000.0 * (PEX.e() + 0.001 * EEXS - 
					       part_mom.e() - ex_mom.e());

			  if (EEXS_new > 0.0) { // everything ok
			    particle.setMomentum(part_mom);
			    output.addOutgoingParticle(particle);
			    ppout += part_mom;

			    A = A_new;
			    Z = Z_new;
			    PEX = ex_mom;
			    EEXS = EEXS_new;
			    NEX -= 1;
			    QEX -= 1;
			    QP -= 1.0;
			    QPP = QPP_new;
			    QNP = QNP_new;
			    bad = false;
			  };
			};  	
		      };   

		      if (itry1 == itry_max) icase = 0;
		    };   
		  };
		}; 
	      };

	      if (icase == 0 && try_again) { // N -> N + 2 
		G4double TNN = 1.6 * EFN + ESP;
		G4double TNP = 1.6 * EFP + ESP;
		G4double XNUN = 1.0 / (1.6 + ESP / EFN);
		G4double XNUP = 1.0 / (1.6 + ESP / EFP);
		G4double SNN1 = csNN(TNP) * XNUP;
		G4double SNN2 = csNN(TNN) * XNUN;
		G4double SPN1 = csPN(TNP) * XNUP;
		G4double SPN2 = csPN(TNN) * XNUN;
		G4double PP = (QPP * SNN1 + QNP * SPN1) * ZR;
		G4double PN = (QPP * SPN2 + QNP * SNN2) * (AR - ZR);
		G4double PW = PP + PN;
		NEX += 2;
		QEX += 2.0; 
		QP += 1.0;
		QH += 1.0;
		AR -= 1.0;

		if (AR > 1.0) {
		  G4double SL = PW * inuclRndm();

		  if (SL > PP) {
		    QNP += 1.0;
		    QNH += 1.0;

		  } else {
		    QPP += 1.0;
		    QPH += 1.0;
		    ZR -= 1.0;

		    if (ZR < 2.0) try_again = false;

		  };    

		} else {
		  try_again = false;
		};
	      };
	    };

	  } else {
	    try_again = false;
	  };

	} else {
	  try_again = false;
	}; 

      } else {
	try_again = false;
      };  
    };
    // everything finished, set output nuclei
    // the exitation energy has to be re-set properly for the energy
    // conservation

    G4LorentzVector pnuc = pin - ppout;
    G4InuclNuclei nuclei(pnuc, A, Z);

    nuclei.setModel(5);
    nuclei.setEnergy();

    pnuc = nuclei.getMomentum(); 
    G4double eout = pnuc.e() + ppout.e();  
    G4double eex_real = 1000.0 * (pin.e() - eout);        

    nuclei.setExitationEnergy(eex_real);
    output.addTargetFragment(nuclei);

  } else {
    G4cout << " NonEquilibriumEvaporator -> target is not nuclei " << G4endl;    

  }; 

  return;
}

G4double G4NonEquilibriumEvaporator::getMatrixElement(G4double A) const {

  if (verboseLevel > 3) {
    G4cout << " >>> G4NonEquilibriumEvaporator::getMatrixElement" << G4endl;
  }

  G4double me;

  if (A > 150.0) {
    me = 100.0;

  } else if(A > 20.0) {
    me = 140.0;

  } else {
    me = 70.0;
  }; 
 
  return me;
}

G4double G4NonEquilibriumEvaporator::getE0(G4double ) const {

  if (verboseLevel > 3) {
    G4cout << " >>> G4NonEquilibriumEvaporator::getEO" << G4endl;
  }

  const G4double e0 = 200.0;

  return e0;   
}

G4double G4NonEquilibriumEvaporator::getParLev(G4double A, 
					       G4double ) const {

  if (verboseLevel > 3) {
    G4cout << " >>> G4NonEquilibriumEvaporator::getParLev" << G4endl;
  }

  //  const G4double par = 0.125;
  G4double pl = 0.125 * A;

  return pl; 
}

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
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     G4CollisionTest.cc
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it), 
// 
//      Creation date: 15 April 1999
//
//      Modifications: 
//
// -------------------------------------------------------------------

#include "globals.hh"

#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <assert.h>

#include "CLHEP/Hist/TupleManager.h"
#include "CLHEP/Hist/HBookFile.h"
#include "CLHEP/Hist/Histogram.h"
#include "CLHEP/Hist/Tuple.h"

#include "Randomize.hh"

#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"

#include "G4AntiProton.hh"
#include "G4Proton.hh"
#include "G4AntiNeutron.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4KaonMinus.hh"
#include "G4KaonPlus.hh"
#include "G4Gamma.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4Lambda.hh"

#include "G4VShortLivedParticle.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4ParticleTable.hh"
#include "G4ShortLivedTable.hh"

#include "G4KineticTrack.hh"
#include "G4KineticTrackVector.hh"
#include "G4KineticTrackVector.hh"

#include "G4ParticleDefinition.hh"

#include "G4VCollision.hh"
#include "G4CollisionNN.hh"
#include "G4CollisionNNElastic.hh"

int main()
{
  // MGP ---- HBOOK initialization
  HepTupleManager* hbookManager;
  hbookManager = new HBookFile("collision.hbook", 58);
  assert (hbookManager != 0);

  // MGP ---- Book histograms

  // MGP ---- Book a ntuple
  HepTuple* ntuple;
  ntuple = hbookManager->ntuple("Collision ntuple");
  assert (ntuple != 0);


  G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();

  G4ParticleDefinition* proton = G4Proton::ProtonDefinition();
  G4ParticleDefinition* antiProton = G4AntiProton::AntiProtonDefinition();
  G4ParticleDefinition* neutron = G4Neutron::NeutronDefinition();   
  G4ParticleDefinition* antiNeutron = G4AntiNeutron::AntiNeutronDefinition();   

  G4ParticleDefinition* pionPlus = G4PionPlus::PionPlusDefinition();
  G4ParticleDefinition* pionMinus = G4PionMinus::PionMinusDefinition();
  G4ParticleDefinition* pionZero = G4PionZero::PionZeroDefinition();

  G4ParticleDefinition* kaonPlus = G4KaonPlus::KaonPlusDefinition();
  G4ParticleDefinition* kaonMinus = G4KaonMinus::KaonMinusDefinition();
   
  G4ParticleDefinition* lambda = G4Lambda::LambdaDefinition();

  G4ParticleDefinition* theMuonPlus = G4MuonPlus::MuonPlusDefinition();
  G4ParticleDefinition* theMuonMinus = G4MuonMinus::MuonMinusDefinition();

  G4ParticleDefinition* theNeutrinoMu = G4NeutrinoMu::NeutrinoMuDefinition();
  G4ParticleDefinition* theAntiNeutrinoMu = G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

  G4ShortLivedConstructor ShortLived;
  ShortLived.ConstructParticle();

  // Select track 1
  
  G4cout << "---- Set KineticTrack 1 ----" << G4endl;
  G4int id1 = 0;
  G4cout << "1 p   2 ap   3 n   4 an   5 pi+   6 pi-   7 K+   8 K- " << G4endl;
  G4cin >> id1;

  G4ParticleDefinition* definition1;
  switch (id1)
    {
    case 1:
      definition1 = proton;
      break;
    case 2:
      definition1 = antiProton;
      break;
    case 3:
      definition1 = neutron;
      break;
    case 4:
      definition1 = antiNeutron;
      break;
    case 5:
      definition1 = pionPlus;
      break;
    case 6:
      definition1 = pionMinus;
      break;
    case 7:
      definition1 = kaonPlus;
      break;
    case 8:
      definition1 = kaonMinus;
      break;
    default:
      definition1 = proton;
      break;
    }
  G4double mass1 = definition1->GetPDGMass();

  // Formation time
  G4double time1 = 0.;
  //  G4cout << "Formation time (ns)" << G4endl;
  //  G4cin >> time1;
  //  if (time1 < 0.) time1 = 0.;
  //   time1 = time1 * ns;
  
  // Initial position
  G4double x1 = 0.;
  G4double y1 = 0.;
  G4double z1 = 0.;
  //  G4cout <<"Initial position (cm) " << G4endl;
  //  G4cin >> x1 >> y1 >> z1;
  G4ThreeVector position1(x1*cm, y1*cm, z1*cm);

  // Momentum
  G4double pzMin1;
  G4double pzMax1;
  G4cout << "Pz min and max (GeV)" << G4endl;
  G4cin >> pzMin1 >> pzMax1;
  pzMin1 = pzMin1 * GeV;
  pzMax1 = pzMax1 * GeV;

  // Select track 2
 
  G4cout << "---- Set KineticTrack 2 ----" << G4endl;
  G4int id2 = 0;
  G4cout << "1 p   2 ap   3 n   4 an   5 pi+   6 pi-   7 K+   8 K- " << G4endl;
  G4cin >> id2;

  G4ParticleDefinition* definition2;
  G4KineticTrack trk2;
  switch (id2)
    {
    case 1:
      definition2 = proton;
      break;
    case 2:
      definition2 = antiProton;
      break;
    case 3:
      definition2 = neutron;
      break;
    case 4:
      definition2 = antiNeutron;
      break;
    case 5:
      definition2 = pionPlus;
      break;
    case 6:
      definition2 = pionMinus;
      break;
    case 7:
      definition2 = kaonPlus;
      break;
    case 8:
      definition2 = kaonMinus;
      break;
    default:
      definition2 = proton;
      break;
    }
  G4double mass2 = definition2->GetPDGMass();

  // Formation time
  G4double time2 = 0.;
  //  G4cout << "Formation time (ns)" << G4endl;
  //  G4cin >> time2;
  //  if (time2 < 0.) time2 = 0.;
  //  time2 = time2 * ns;
  
  // Initial position
  G4double x2 = 0.;
  G4double y2 = 0.;
  G4double z2 = 0.;
  //  G4cout <<"Initial position (cm) " << G4endl;
  //  G4cin >> x2 >> y2 >> z2;
  G4ThreeVector position2(x2*cm, y2*cm, z2*cm);

  // Momentum
  G4double pzMin2;
  G4double pzMax2;
  G4cout << "Pz min and max (GeV)" << G4endl;
  G4cin >> pzMin2 >> pzMax2;
  pzMin2 = pzMin2 * GeV;
  pzMax2 = pzMax2 * GeV;

  G4VCollision* collision = new G4CollisionNNElastic;
  //    collision->Print();

  // Number of iterations
  G4int n = 0;
  G4cout << "---- Number of iterations ----" << G4endl;
  G4cin >> n;
  if (n < 1) n = 1;

  G4double step1 = (pzMax1 - pzMin1) / n;
  G4double step2 = (pzMax2 - pzMin2) / n;

  G4cout << "pz min max step " 
	 << pzMin1 / GeV << ", " 
	 << pzMax1 / GeV << ", " 
	 << step1  / GeV << ", " << G4endl 
	 << "pz min max step " 
	 << pzMin2 / GeV << ", " 
	 << pzMax2 / GeV << ", " 
	 << step2  / GeV << G4endl; 

  G4int i;
  for (i=0; i<n; i++)
    {
      G4double pStep1 = pzMin1 + step1 * i;
      G4ThreeVector p1(0.,0.,pStep1);
      G4double e1 = sqrt(mass1*mass1 + p1.mag()*p1.mag());
      G4LorentzVector p41(p1,e1);
      G4KineticTrack trk1(definition1,time1,position1,p41);

      G4double pStep2 = -(pzMin2 + step2 * i);
      G4ThreeVector p2(0.,0.,pStep2);
      G4double e2 = sqrt(mass2*mass2 + p2.mag()*p2.mag());
      G4LorentzVector p42(p2,e2);
      G4KineticTrack trk2(definition2,time2,position2,p42);

      G4double sqrts = (trk1.Get4Momentum() + trk2.Get4Momentum()).mag();

      G4KineticTrackVector* finalState = collision->FinalState(trk1,trk2);
      G4int nFinal = finalState->size();
      for (G4int it=0; it<nFinal; it++)
	{
	  G4KineticTrack* trk = (*finalState)[it];
	  G4double mass = trk->GetDefinition()->GetPDGMass();
	  G4ThreeVector p3 = trk->Get4Momentum().vect();
	  G4double p = p3.mag() / GeV;
	  G4double px = p3.x() / GeV;
	  G4double py = p3.y() / GeV;
	  G4double pz = p3.z() / GeV;
	  G4cout << mass
		 << " - p = "
		 << p
		 << ", (px,py,pz) =  "		
		 << px
		 << ","
		 << py
		 << ","			 
		 << pz
		 << G4endl;
	}
    }


  hbookManager->write();
  
  return EXIT_SUCCESS;
}






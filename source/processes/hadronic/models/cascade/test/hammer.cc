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
#include "Ghad_hp.hh"
#include "G4HadronKineticModel.hh"
#include "G4FieldManager.hh"
#include "G4ExcitationHandler.hh"
#include "G4PreCompoundModel.hh"
#include "G4FieldManager.hh"

const G4double G4FieldManager::fDefault_Delta_Intersection_Val= 0.1 * mm;
const G4double G4FieldManager::fDefault_Delta_One_Step_Value = 0.25 * mm;


main()
{
  G4int nev;
  G4double ekin;
  G4cout << "nev = ?"<<endl;
  cin >> nev;
  G4cout << "ekin = ?"<<endl;
  cin >> ekin;
  G4ExcitationHandler aHandler;
  G4PreCompoundModel aPhase(&aHandler);
  G4HadronKineticModel* theModel = new G4HadronKineticModel;
  for(G4int i=0; i<nev; i++) 
  {
    Ghad_hp* theModel = new Ghad_hp;
    theModel->SetDeExcitation(&aPhase);
    G4cerr << "Event number "<<i<<G4endl;

    G4V3DNucleus * theN = new G4Fancy3DNucleus;
    theN->Init(27, 13);

    G4ParticleDefinition * aP = G4Proton::ProtonDefinition();
    G4KineticTrackVector * pro = new G4KineticTrackVector;
    G4double m = aP->GetPDGMass();
    G4double ek = ekin*MeV;
    G4double e = m+ek;
    G4double pz = std::sqrt(e*e-m*m);
    G4LorentzVector theM (0,0,pz, e);
    G4ThreeVector position(0,0,-1.4*theN->GetOuterRadius());
    G4DynamicParticle * it1 = new G4DynamicParticle(aP, theM);
    G4KineticTrack * aPro = 
           new G4KineticTrack(aP, 0, 
	                      position,
			      theM );
    pro->push_back(aPro);

    G4ReactionProductVector * result = new G4ReactionProductVector;
    result = theModel->Propagate(pro, theN);
    
    for(G4int it=0; it<result->size(); it++)
    {
      if(result->operator[](it))
      {
      G4cout << "DATA "
             << result->operator[](it)->GetMomentum().x()<<" "
             << result->operator[](it)->GetMomentum().y()<<" "
             << result->operator[](it)->GetMomentum().z()<<" "
	     << result->operator[](it)->GetKineticEnergy()<<" "
	     << result->operator[](it)->GetDefinition()->GetPDGEncoding()<<" "
	     << result->operator[](it)->GetDefinition()->GetParticleName()<<" "
	     << G4endl;
      delete result->operator[](it);
      }
    }
    delete result;
    delete theModel;
  }
}


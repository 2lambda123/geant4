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
// $Id: G4ExcitationHandler.cc,v 1.36 2010-05-09 17:31:45 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (May 1998)
//
//
// Modified:
// (30 June 1998) by V. Lara:
//      -Modified the Transform method for use G4ParticleTable and 
//       therefore G4IonTable. It makes possible to convert all kind 
//       of fragments (G4Fragment) produced in deexcitation to 
//       G4DynamicParticle
//      -It uses default algorithms for:
//              Evaporation: G4Evaporation
//              MultiFragmentation: G4StatMF 
//              Fermi Breakup model: G4FermiBreakUp
// (24 Jul 2008) by M. A. Cortes Giraldo:
//      -Max Z,A for Fermi Break-Up turns to 9,17 by default
//      -BreakItUp() reorganised and bug in Evaporation loop fixed
//      -Transform() optimised
// (September 2008) by J. M. Quesada. External choices have been added for :
//      -inverse cross section option (default OPTxs=3)
//      -superimposed Coulomb barrier (if useSICB is set true, by default it is false) 
// (September 2009) by J. M. Quesada: 
//      -according to Igor Pshenichnov, SMM will be applied (just in case) only once.
// (27 Nov 2009) by V.Ivanchenko: 
//      -cleanup the logic, reduce number internal vectors, fixed memory leak.
//

#include "G4ExcitationHandler.hh"
#include "globals.hh"
#include "G4LorentzVector.hh"
#include "G4NistManager.hh"
#include <list>

G4ExcitationHandler::G4ExcitationHandler():
  // JMQ 160909 Fermi BreakUp & MultiFrag are on by default 
  // This is needed for activation of such models when G4BinaryLightIonReaction is used
  // since no interface (for external activation via macro input file) is still available.
  maxZForFermiBreakUp(9),maxAForFermiBreakUp(17),minEForMultiFrag(4.0*GeV),
  //  maxZForFermiBreakUp(9),maxAForFermiBreakUp(17),minEForMultiFrag(3.0*MeV),
  //maxZForFermiBreakUp(1),maxAForFermiBreakUp(1),minEForMultiFrag(4.0*GeV),
  minExcitation(CLHEP::keV),
  MyOwnEvaporationClass(true), MyOwnMultiFragmentationClass(true),MyOwnFermiBreakUpClass(true),
  MyOwnPhotonEvaporationClass(true),OPTxs(3),useSICB(false)
{                                                                          
  theTableOfParticles = G4ParticleTable::GetParticleTable();
  
  theEvaporation = new G4Evaporation;
  theMultiFragmentation = new G4StatMF;
  theFermiModel = new G4FermiBreakUp;
  thePhotonEvaporation = new G4PhotonEvaporation;
  SetParameters();
}

G4ExcitationHandler::~G4ExcitationHandler()
{
  if (MyOwnEvaporationClass) delete theEvaporation;
  if (MyOwnMultiFragmentationClass) delete theMultiFragmentation;
  if (MyOwnFermiBreakUpClass) delete theFermiModel;
  if (MyOwnPhotonEvaporationClass) delete thePhotonEvaporation;
}

void G4ExcitationHandler::SetParameters()
{
  //for inverse cross section choice
  theEvaporation->SetOPTxs(OPTxs);
  //for the choice of superimposed Coulomb Barrier for inverse cross sections
  theEvaporation->UseSICB(useSICB);
  theEvaporation->Initialise();
}

G4ReactionProductVector * G4ExcitationHandler::BreakItUp(const G4Fragment & theInitialState) const
{	
  
  // Variables existing until end of method
  G4Fragment * theInitialStatePtr = new G4Fragment(theInitialState);
  G4FragmentVector * theTempResult = 0;      // pointer which receives temporal results
  std::list<G4Fragment*> theEvapList;        // list to apply Evaporation, SMF or Fermi Break-Up
  std::list<G4Fragment*> theEvapStableList;  // list to apply PhotonEvaporation
  std::list<G4Fragment*> theResults;         // list to store final result
  std::list<G4Fragment*>::iterator iList;
  //
  //G4cout << "@@@@@@@@@@ Start G4Excitation Handler @@@@@@@@@@@@@" << G4endl;  
  //G4cout << theInitialState << G4endl;  
  
  // Variables to describe the excited configuration
  G4double exEnergy = theInitialState.GetExcitationEnergy();
  G4int A = static_cast<G4int>( theInitialState.GetA() +0.5 );
  G4int Z = static_cast<G4int>( theInitialState.GetZ() +0.5 );

  G4NistManager* nist = G4NistManager::Instance();
  
  // JMQ 150909:  first step in de-excitation chain (SMM will be used only here)
  
  // In case A <= 1 the fragment will not perform any nucleon emission
  if (A <= 1)
    {
      theResults.push_back( theInitialStatePtr );
    }
  // check if a fragment is stable
  else if(exEnergy < minExcitation && nist->GetIsotopeAbundance(Z, A) > 0.0)
    {
      theResults.push_back( theInitialStatePtr );
    }  
  else  // In all cases apply once theFermiModel, theMultiFragmentation or theEvaporation
    {      
      // JMQ 150909: first step in de-excitation is treated separately 
      // Fragments after the first step are stored in theEvapList 
      // Statistical Multifragmentation will take place (just in case) only here
      //
      // Test applicability
      // Initial State De-Excitation 
      if( A<GetMaxA() && Z<GetMaxZ()) 
        {
          theTempResult = theFermiModel->BreakItUp(theInitialState);
	  // fragment was not decaied try to evaporate
	  if(1 == theTempResult->size()) {
            if((*theTempResult)[0] !=  theInitialStatePtr) { delete (*theTempResult)[0]; }
            delete theTempResult;
	    theTempResult = theEvaporation->BreakItUp(theInitialState);
	  }
        }
      else   if (exEnergy>GetMinE()*A) 
        {
          theTempResult = theMultiFragmentation->BreakItUp(theInitialState);
	  // fragment was not decaied try to evaporate
	  if(1 == theTempResult->size()) {
            if((*theTempResult)[0] !=  theInitialStatePtr) { delete (*theTempResult)[0]; }
            delete theTempResult;
	    theTempResult = theEvaporation->BreakItUp(theInitialState);
	  }
        }
      else 
        {
          theTempResult = theEvaporation->BreakItUp(theInitialState);
        }
      
      G4bool deletePrimary = true;
      G4int nsec=theTempResult->size();
      if(nsec > 0) 
	{      
	  // Sort out secondary fragments
	  G4FragmentVector::iterator j;
	  for (j = theTempResult->begin(); j != theTempResult->end(); ++j)
	    {  
	      if((*j) == theInitialStatePtr) { deletePrimary = false; }
	      A = static_cast<G4int>((*j)->GetA()+0.5);  // +0.5 to avoid bad truncation

	      if(A <= 1) { theResults.push_back(*j); }   // gamma, p, n
	      else if(1 == nsec) { theEvapStableList.push_back(*j); } // evaporation is not possible  
	      else  // Analyse fragment
		{
		  G4double exEnergy = (*j)->GetExcitationEnergy();
		  if(exEnergy < minExcitation) {
		    Z = static_cast<G4int>((*j)->GetZ()+0.5);  // +0.5 to avoid bad truncation
                    if(nist->GetIsotopeAbundance(Z, A) > 0.0) { 
		      theResults.push_back(*j); // stable fragment 
		    } else {   
		      theEvapList.push_back(*j); // unstable cold fragment 
		    }
		  } else { theEvapList.push_back(*j); } // hot fragment
		}
	    }
	}
      if( deletePrimary ) { delete theInitialStatePtr; }
      delete theTempResult;
    }
  //
  // JMQ 150909: Further steps in de-excitation chain follow ..
      
  //G4cout << "## After first step " << theEvapList.size() << " for evap;  "
  //	 << theEvapStableList.size() << " for photo-evap; " 
  //	 << theResults.size() << " results. " << G4endl; 

  // ------------------------------
  // De-excitation loop
  // ------------------------------
      
  for (iList = theEvapList.begin(); iList != theEvapList.end(); ++iList)
    {
      A = static_cast<G4int>((*iList)->GetA()+0.5);  // +0.5 to avoid bad truncation
      Z = static_cast<G4int>((*iList)->GetZ()+0.5);
	  
      if ( A < GetMaxA() && Z < GetMaxZ() ) // if satisfied apply Fermi Break-Up
	{
	  theTempResult = theFermiModel->BreakItUp(*(*iList));
	  // fragment was not decaied try to evaporate
	  if(1 == theTempResult->size()) {
            if((*theTempResult)[0] !=  theInitialStatePtr) { delete (*theTempResult)[0]; }
            delete theTempResult;
	    theTempResult = theEvaporation->BreakItUp(*(*iList));
	  }
	}
      else // apply Evaporation in another case
	{
	  theTempResult = theEvaporation->BreakItUp(*(*iList));
	}
      
      G4bool deletePrimary = true;
      G4int nsec = theTempResult->size();
		  
      // Sort out secondary fragments
      if ( nsec > 0 )
	{
	  G4FragmentVector::iterator j;
	  for (j = theTempResult->begin(); j != theTempResult->end(); ++j)
	    {
	      if((*j) == (*iList)) { deletePrimary = false; }
	      A = static_cast<G4int>((*j)->GetA()+0.5);  // +0.5 to avoid bad truncation

	      if(A <= 1) { theResults.push_back(*j); }        // gamma, p, n
	      else if(1 == nsec) { theEvapStableList.push_back(*j); } // evaporation is not possible  
	      else  // Analyse fragment
		{
		  G4double exEnergy = (*j)->GetExcitationEnergy();
		  if(exEnergy < minExcitation) {
		    Z = static_cast<G4int>((*j)->GetZ()+0.5);  // +0.5 to avoid bad truncation
                    if(nist->GetIsotopeAbundance(Z, A) > 0.0) { 
		      theResults.push_back(*j); // stable fragment 
		    } else {   
		      theEvapList.push_back(*j); // unstable cold fragment 
		    }
		  } else { theEvapList.push_back(*j); } // hot fragment
		}
	    }
	}
      if( deletePrimary ) { delete (*iList); }
      delete theTempResult;
    } // end of the loop over theEvapList

  //G4cout << "## After 2nd step " << theEvapList.size() << " was evap;  "
  //	 << theEvapStableList.size() << " for photo-evap; " 
  //	 << theResults.size() << " results. " << G4endl; 
      
  // -----------------------
  // Photon-Evaporation loop
  // -----------------------
  
  for (iList = theEvapStableList.begin(); iList != theEvapStableList.end(); ++iList)
    {
      // photon-evaporation is applied
      //theTempResult = thePhotonEvaporation->BreakItUp(*(*iList));
      theTempResult = thePhotonEvaporation->BreakUpFragment(*iList);
	  
      G4bool deletePrimary = true;
      G4int nsec = theTempResult->size();
	  
      // if there is a gamma emission then
      if (nsec > 1)
	{
	  G4FragmentVector::iterator j;
	  for (j = theTempResult->begin(); j != theTempResult->end(); ++j)
	    {
	      if((*j) == (*iList)) { deletePrimary = false; }
	      A = static_cast<G4int>((*j)->GetA()+0.5);  // +0.5 to avoid bad truncation

	      if(A <= 1) { theResults.push_back(*j); }        // gamma
	      else  // Analyse fragment
		{
		  G4double exEnergy = (*j)->GetExcitationEnergy();
		  if(exEnergy < minExcitation) {
		    theResults.push_back(*j); // cold fragment 
		  } else {   
		    theEvapStableList.push_back(*j); // hot fragment 
		  }
		}
	    }
	}
      // normally should not reach this point - it is kind of work around	      
      else if(1 == nsec) 
	{
	  G4Fragment* nuc = (*theTempResult)[0]; 
	  if(nuc == (*iList)) { deletePrimary = false; }
	  // Let's create a G4Fragment pointer representing the gamma emmited
	  G4LorentzVector lv = nuc->GetMomentum();
	  G4double Mass = nuc->GetGroundStateMass();
	  G4double Ecm = lv.m();
	  if(Ecm - Mass > minExcitation) 
	    {
	      G4ThreeVector bst = lv.boostVector();
	      G4double GammaEnergy = 0.5*(Ecm - Mass)*(Ecm + Mass)/Ecm;
	      G4double cosTheta = 1. - 2. * G4UniformRand(); 
	      G4double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
	      G4double phi = twopi * G4UniformRand();
	      G4LorentzVector Gamma4P(GammaEnergy * sinTheta * std::cos(phi),
				      GammaEnergy * sinTheta * std::sin(phi),
				      GammaEnergy * cosTheta,
				      GammaEnergy);
	      Gamma4P.boost(bst);  
	      G4Fragment * thePhoton = new G4Fragment(Gamma4P,G4Gamma::GammaDefinition());
	      theResults.push_back(thePhoton); 
	      
	      // And now we update momentum and energy for the nucleus
	      lv -= Gamma4P;
	      thePhoton->SetCreationTime(nuc->GetCreationTime());
	      nuc->SetMomentum(lv); // Now this fragment has been deexcited!
	    }
	  // we store the deexcited fragment 
	  theResults.push_back(nuc);
	}
      if( deletePrimary ) { delete (*iList); }
      delete theTempResult;
    } // end of photon-evaporation loop

  //G4cout << "## After 3d step " << theEvapList.size() << " was evap;  "
  //	 << theEvapStableList.size() << " was photo-evap; " 
  //	 << theResults.size() << " results. " << G4endl; 
    
#ifdef debug
  CheckConservation(theInitialState,*theResults);
#endif

  G4ReactionProductVector * theReactionProductVector = new G4ReactionProductVector;

  // MAC (24/07/08)
  // To optimise the storing speed, we reserve space in memory for the vector
  theReactionProductVector->reserve( theResults.size() );

  G4int theFragmentA, theFragmentZ;
  G4LorentzVector theFragmentMomentum;

  std::list<G4Fragment*>::iterator i;
  for (i = theResults.begin(); i != theResults.end(); ++i) 
    {
      theFragmentA = static_cast<G4int>((*i)->GetA());
      theFragmentZ = static_cast<G4int>((*i)->GetZ());
      theFragmentMomentum = (*i)->GetMomentum();
      G4ParticleDefinition* theKindOfFragment = 0;
      if (theFragmentA == 0) {       // photon or e-
	theKindOfFragment = (*i)->GetParticleDefinition();   
      } else if (theFragmentA == 1 && theFragmentZ == 0) { // neutron
	theKindOfFragment = G4Neutron::NeutronDefinition();
      } else if (theFragmentA == 1 && theFragmentZ == 1) { // proton
	theKindOfFragment = G4Proton::ProtonDefinition();
      } else if (theFragmentA == 2 && theFragmentZ == 1) { // deuteron
	theKindOfFragment = G4Deuteron::DeuteronDefinition();
      } else if (theFragmentA == 3 && theFragmentZ == 1) { // triton
	theKindOfFragment = G4Triton::TritonDefinition();
      } else if (theFragmentA == 3 && theFragmentZ == 2) { // helium3
	theKindOfFragment = G4He3::He3Definition();
      } else if (theFragmentA == 4 && theFragmentZ == 2) { // alpha
	theKindOfFragment = G4Alpha::AlphaDefinition();;
      } else {
	theKindOfFragment = theTableOfParticles->FindIon(theFragmentZ,theFragmentA,0,theFragmentZ);
      }
      if (theKindOfFragment != 0) 
	{
	  G4ReactionProduct * theNew = new G4ReactionProduct(theKindOfFragment);
	  theNew->SetMomentum(theFragmentMomentum.vect());
	  theNew->SetTotalEnergy(theFragmentMomentum.e());
	  theNew->SetFormationTime((*i)->GetCreationTime());
	  theReactionProductVector->push_back(theNew);
	}
      delete (*i);
    }

  return theReactionProductVector;
}



G4ReactionProductVector * 
G4ExcitationHandler::Transform(G4FragmentVector * theFragmentVector) const
{
  if (theFragmentVector == 0) return 0;
  
  // Conversion from G4FragmentVector to G4ReactionProductVector
  G4ParticleDefinition *theGamma = G4Gamma::GammaDefinition();
  G4ParticleDefinition *theNeutron = G4Neutron::NeutronDefinition();
  G4ParticleDefinition *theProton = G4Proton::ProtonDefinition();   
  G4ParticleDefinition *theDeuteron = G4Deuteron::DeuteronDefinition();
  G4ParticleDefinition *theTriton = G4Triton::TritonDefinition();
  G4ParticleDefinition *theHelium3 = G4He3::He3Definition();
  G4ParticleDefinition *theAlpha = G4Alpha::AlphaDefinition();
  G4ParticleDefinition *theKindOfFragment = 0;
  theNeutron->SetVerboseLevel(2);
  G4ReactionProductVector * theReactionProductVector = new G4ReactionProductVector;

  // MAC (24/07/08)
  // To optimise the storing speed, we reserve space in memory for the vector
  theReactionProductVector->reserve( theFragmentVector->size() * sizeof(G4ReactionProduct*) );

  G4int theFragmentA, theFragmentZ;
  G4LorentzVector theFragmentMomentum;

  G4FragmentVector::iterator i;
  for (i = theFragmentVector->begin(); i != theFragmentVector->end(); i++) {
    //    std::cout << (*i) <<'\n';
    theFragmentA = static_cast<G4int>((*i)->GetA());
    theFragmentZ = static_cast<G4int>((*i)->GetZ());
    theFragmentMomentum = (*i)->GetMomentum();
    theKindOfFragment = 0;
    if (theFragmentA == 0 && theFragmentZ == 0) {       // photon
      theKindOfFragment = theGamma;      
    } else if (theFragmentA == 1 && theFragmentZ == 0) { // neutron
      theKindOfFragment = theNeutron;
    } else if (theFragmentA == 1 && theFragmentZ == 1) { // proton
      theKindOfFragment = theProton;
    } else if (theFragmentA == 2 && theFragmentZ == 1) { // deuteron
      theKindOfFragment = theDeuteron;
    } else if (theFragmentA == 3 && theFragmentZ == 1) { // triton
      theKindOfFragment = theTriton;
    } else if (theFragmentA == 3 && theFragmentZ == 2) { // helium3
      theKindOfFragment = theHelium3;
    } else if (theFragmentA == 4 && theFragmentZ == 2) { // alpha
      theKindOfFragment = theAlpha;
    } else {
      theKindOfFragment = theTableOfParticles->FindIon(theFragmentZ,theFragmentA,0,theFragmentZ);
    }
    if (theKindOfFragment != 0) 
      {
	G4ReactionProduct * theNew = new G4ReactionProduct(theKindOfFragment);
	theNew->SetMomentum(theFragmentMomentum.vect());
	theNew->SetTotalEnergy(theFragmentMomentum.e());
	theNew->SetFormationTime((*i)->GetCreationTime());
#ifdef PRECOMPOUND_TEST
	theNew->SetCreatorModel((*i)->GetCreatorModel());
#endif
	theReactionProductVector->push_back(theNew);
      }
  }
  if (theFragmentVector != 0)
    { 
      std::for_each(theFragmentVector->begin(), theFragmentVector->end(), DeleteFragment());
      delete theFragmentVector;
    }
  G4ReactionProductVector::iterator debugit;
  for(debugit=theReactionProductVector->begin(); 
      debugit!=theReactionProductVector->end(); debugit++)
    {
    if((*debugit)->GetTotalEnergy()<1.*eV)
      {
	if(getenv("G4DebugPhotonevaporationData"))
	  {
	    G4cerr << "G4ExcitationHandler: Warning: Photonevaporation data not exact."<<G4endl;
	    G4cerr << "G4ExcitationHandler: Warning: Found gamma with energy = "
		   << (*debugit)->GetTotalEnergy()/MeV << "MeV"
		   << G4endl;
	  }
	delete (*debugit);
	*debugit = 0;
      }
  }
  G4ReactionProduct* tmpPtr=0;
  theReactionProductVector->erase(std::remove_if(theReactionProductVector->begin(),
                                                 theReactionProductVector->end(),
                                                 std::bind2nd(std::equal_to<G4ReactionProduct*>(),
                                                              tmpPtr)),
				  theReactionProductVector->end());
  return theReactionProductVector;
}


#ifdef debug
void G4ExcitationHandler::CheckConservation(const G4Fragment & theInitialState,
					    G4FragmentVector * Result) const
{
  G4double ProductsEnergy =0;
  G4ThreeVector ProductsMomentum;
  G4int ProductsA = 0;
  G4int ProductsZ = 0;
  G4FragmentVector::iterator h;
  for (h = Result->begin(); h != Result->end(); h++) {
    G4LorentzVector tmp = (*h)->GetMomentum();
    ProductsEnergy += tmp.e();
    ProductsMomentum += tmp.vect();
    ProductsA += static_cast<G4int>((*h)->GetA());
    ProductsZ += static_cast<G4int>((*h)->GetZ());
  }
  
  if (ProductsA != theInitialState.GetA()) {
    G4cout << "!!!!!!!!!! Baryonic Number Conservation Violation !!!!!!!!!!" << G4endl;
    G4cout << "G4ExcitationHandler.cc: Barionic Number Conservation test for deexcitation fragments" 
	   << G4endl; 
    G4cout << "Initial A = " << theInitialState.GetA() 
	   << "   Fragments A = " << ProductsA << "   Diference --> " 
	   << theInitialState.GetA() - ProductsA << G4endl;
  }
  if (ProductsZ != theInitialState.GetZ()) {
    G4cout << "!!!!!!!!!! Charge Conservation Violation !!!!!!!!!!" << G4endl;
    G4cout << "G4ExcitationHandler.cc: Charge Conservation test for deexcitation fragments" 
	   << G4endl; 
    G4cout << "Initial Z = " << theInitialState.GetZ() 
	   << "   Fragments Z = " << ProductsZ << "   Diference --> " 
	   << theInitialState.GetZ() - ProductsZ << G4endl;
  }
  if (std::abs(ProductsEnergy-theInitialState.GetMomentum().e()) > 1.0*keV) {
    G4cout << "!!!!!!!!!! Energy Conservation Violation !!!!!!!!!!" << G4endl;
    G4cout << "G4ExcitationHandler.cc: Energy Conservation test for deexcitation fragments" 
	   << G4endl; 
    G4cout << "Initial E = " << theInitialState.GetMomentum().e()/MeV << " MeV"
	   << "   Fragments E = " << ProductsEnergy/MeV  << " MeV   Diference --> " 
	   << (theInitialState.GetMomentum().e() - ProductsEnergy)/MeV << " MeV" << G4endl;
  } 
  if (std::abs(ProductsMomentum.x()-theInitialState.GetMomentum().x()) > 1.0*keV || 
      std::abs(ProductsMomentum.y()-theInitialState.GetMomentum().y()) > 1.0*keV ||
      std::abs(ProductsMomentum.z()-theInitialState.GetMomentum().z()) > 1.0*keV) {
    G4cout << "!!!!!!!!!! Momentum Conservation Violation !!!!!!!!!!" << G4endl;
    G4cout << "G4ExcitationHandler.cc: Momentum Conservation test for deexcitation fragments" 
	   << G4endl; 
    G4cout << "Initial P = " << theInitialState.GetMomentum().vect() << " MeV"
	   << "   Fragments P = " << ProductsMomentum  << " MeV   Diference --> " 
	   << theInitialState.GetMomentum().vect() - ProductsMomentum << " MeV" << G4endl;
  }
  return;
}
#endif





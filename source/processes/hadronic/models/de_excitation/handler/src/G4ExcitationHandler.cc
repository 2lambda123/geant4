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
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (May 1998)
// Modif (30 June 1998) by V. Lara:
//      -Modified the Transform method for use G4ParticleTable and 
//       therefore G4IonTable. It makes possible to convert all kind 
//       of fragments (G4Fragment) produced in deexcitation to 
//       G4DynamicParticle
//      -It uses default algorithms for:
//              Evaporation: G4StatEvaporation
//              MultiFragmentation: G4DummyMF (a dummy one)
//              Fermi Breakup model: G4StatFermiBreakUp


#include "G4ExcitationHandler.hh"
#include <list>

//#define debugphoton


G4ExcitationHandler::G4ExcitationHandler():
  maxZForFermiBreakUp(1),maxAForFermiBreakUp(1),minEForMultiFrag(4.0*GeV),
  MyOwnEvaporationClass(true), MyOwnMultiFragmentationClass(true),MyOwnFermiBreakUpClass(true),
  MyOwnPhotonEvaporationClass(true)
{                                                                          
  theTableOfParticles = G4ParticleTable::GetParticleTable();
  
  theEvaporation = new G4Evaporation;
  theMultiFragmentation = new G4StatMF;
  theFermiModel = new G4FermiBreakUp;
  thePhotonEvaporation = new G4PhotonEvaporation;
}

G4ExcitationHandler::G4ExcitationHandler(const G4ExcitationHandler &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4ExcitationHandler::copy_constructor: is meant to not be accessable! ");
}


G4ExcitationHandler::~G4ExcitationHandler()
{
  if (MyOwnEvaporationClass) delete theEvaporation;
  if (MyOwnMultiFragmentationClass) delete theMultiFragmentation;
  if (MyOwnFermiBreakUpClass) delete theFermiModel;
  if (MyOwnPhotonEvaporationClass) delete thePhotonEvaporation;
}


const G4ExcitationHandler & G4ExcitationHandler::operator=(const G4ExcitationHandler &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4ExcitationHandler::operator=: is meant to not be accessable! ");

  return *this;
}


G4bool G4ExcitationHandler::operator==(const G4ExcitationHandler &) const
{
  throw G4HadronicException(__FILE__, __LINE__, "G4ExcitationHandler::operator==: is meant to not be accessable! ");
  return false;
} 

G4bool G4ExcitationHandler::operator!=(const G4ExcitationHandler &) const
{
  throw G4HadronicException(__FILE__, __LINE__, "G4ExcitationHandler::operator!=: is meant to not be accessable! ");
  return true;
}


G4ReactionProductVector * G4ExcitationHandler::BreakItUp(const G4Fragment &theInitialState) const
{

  G4FragmentVector * theResult = 0; 
  G4double exEnergy = theInitialState.GetExcitationEnergy();
  //  G4cout << " first exEnergy in MeV: " << exEnergy/MeV << G4endl;
  G4double A = theInitialState.GetA();
  G4int Z = static_cast<G4int>(theInitialState.GetZ());
  G4FragmentVector* theTempResult = 0; 
  G4Fragment theExcitedNucleus;

  // Test applicability
  if (A > 4) 
    {
      // Initial State De-Excitation 
      if(A<GetMaxA()&&Z<GetMaxZ()) 
        //     && exEnergy>G4NucleiPropertiesTable::GetBindingEnergy(Z,A)) {
        {
          theResult = theFermiModel->BreakItUp(theInitialState);
        }
      else   if (exEnergy>GetMinE()*A) 
        {
          theResult = theMultiFragmentation->BreakItUp(theInitialState);
        }
      else 
        {
          theResult = theEvaporation->BreakItUp(theInitialState);
        }
    



      // De-Excitation loop
      // ------------------
      // Check if there are excited fragments
      std::list<G4Fragment*> theResultList;
      G4FragmentVector::iterator j;
      std::list<G4Fragment*>::iterator i;
      for (j = theResult->begin(); j != theResult->end();j++) 
        {
          theResultList.push_back(*j);
        }
      theResult->clear();
      for (i = theResultList.begin(); i != theResultList.end(); i++) 
        {
          exEnergy = (*i)->GetExcitationEnergy();
	  //	  G4cout << " exEnergy in MeV: " << exEnergy/MeV << G4endl;
          if (exEnergy > 0.0) 
            {
              A = (*i)->GetA();
              Z = static_cast<G4int>((*i)->GetZ());
              theExcitedNucleus = *(*i);
              // try to de-excite this fragment
              if( A < GetMaxA() && Z < GetMaxZ() )
                // && exEnergy>G4NucleiPropertiesTable::GetBindingEnergy(Z,A)) 
                {
                  // Fermi Breakup not now called for for exotic fragments for good reasons...
                  // theTempResult = theFermiModel->BreakItUp(theExcitedNucleus);
                  //if (theTempResult->size() == 1)
                  //  {
                  //    std::for_each(theTempResult->begin(),theTempResult->end(), G4Delete());
                  //    delete theTempResult;
                  //  }
                  theTempResult = theEvaporation->BreakItUp(theExcitedNucleus);
                } 
              else 
                {
                  // Evaporation
                  theTempResult = theEvaporation->BreakItUp(theExcitedNucleus);
                }
              // The Nucleus has been fragmented?
              if (theTempResult->size() > 1) 
                // If so :
                {
                  // Remove excited fragment from the result 
                  //	delete theResult->removeAt(i--);
                  delete (*i);
                  i = theResultList.erase(i);
                  // and add theTempResult elements to theResult
                  for (G4FragmentVector::reverse_iterator ri = theTempResult->rbegin();
                       ri != theTempResult->rend(); ++ri)
                    {
                      theResultList.push_back(*ri);
                    }
                  delete theTempResult;
                } 
              else 
                // If not :
                {
                  // it doesn't matter, we Follow with the next fragment but
                  // I have to clean up
                  std::for_each(theTempResult->begin(),theTempResult->end(), G4Delete());
                  delete theTempResult;
                }
            }
        }
      for (i = theResultList.begin(); i != theResultList.end(); i++)
        {
          theResult->push_back(*i);
        }
      theResultList.clear();
    }
  else   // if A > 4
    {
      theResult = new G4FragmentVector();
      theResult->push_back(new G4Fragment(theInitialState));
    }

  // Now we try to deexcite by means of PhotonEvaporation those fragments
  // which are excited.
  
  theTempResult = 0;
  std::list<G4Fragment*> theFinalResultList;
  //AHtest  std::list<G4Fragment*> theFinalPhotonResultList;
  std::list<G4Fragment*> theResultList;
  std::list<G4Fragment*>::iterator j;
  G4FragmentVector::iterator i;
  for (i = theResult->begin(); i != theResult->end();i++) 
    {
      theResultList.push_back(*i);
      //      G4cout << " Before loop list energy in MeV: " << ((*i)->GetExcitationEnergy())/MeV << G4endl;
    }
  theResult->clear();

  for (j = theResultList.begin(); j != theResultList.end(); j++) {
    //    G4cout << " Test loop list: " << (*j)->GetExcitationEnergy() << " size: " << theResultList.size() << G4endl;
  }

  //  for (j = theResultList.begin(); j != theResultList.end(); j++) 
  j = theResultList.begin();  //AH
  while (j != theResultList.end()) //AH
    {
      if ((*j)->GetA() > 1 && (*j)->GetExcitationEnergy() > 0.1*eV) 
        {
          theExcitedNucleus = *(*j);
          theTempResult = thePhotonEvaporation->BreakItUp(theExcitedNucleus);
          // If Gamma Evaporation has succeed then
          if (theTempResult->size() > 1) 
            {
              // Remove excited fragment from the result 
	      //	      delete (*j);
	      //              theResultList.erase(j--);
	      //	      theResultList.erase(j); don't delete as there's no push back...
              // and add theTempResult elements to theResult
	      for (G4FragmentVector::reverse_iterator ri = theTempResult->rbegin();
		   ri != theTempResult->rend(); ++ri)
                {
#ifdef PRECOMPOUND_TEST
                  if ((*ri)->GetA() == 0)
                    (*ri)->SetCreatorModel(G4String("G4PhotonEvaporation"));
                  else
                    (*ri)->SetCreatorModel(G4String("ResidualNucleus"));
#endif
		  theResultList.push_back(*ri);
		  //AHtest		  theFinalPhotonResultList.push_back(*ri);
		  //		  theFinalResultList.push_back(*ri); don't add to final result as they'll go through the loop
                }
              delete theTempResult;
            }
          // In other case, just clean theTempResult and continue
          else 
            {
              std::for_each(theTempResult->begin(), theTempResult->end(), DeleteFragment());
              delete theTempResult;
#ifdef debugphoton
              G4cout << "G4ExcitationHandler: Gamma Evaporation could not deexcite the nucleus: \n"
                     << "-----------------------------------------------------------------------\n"
                     << theExcitedNucleus << '\n'
                     << "-----------------------------------------------------------------------\n";
#endif
              G4double GammaEnergy = (*j)->GetExcitationEnergy();
              G4double cosTheta = 1. - 2. * G4UniformRand();
              G4double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
              G4double phi = twopi * G4UniformRand();
              G4ThreeVector GammaP(GammaEnergy * sinTheta * std::cos(phi),
                                   GammaEnergy * sinTheta * std::sin(phi),
                                   GammaEnergy * cosTheta );
              G4LorentzVector Gamma4P(GammaP,GammaEnergy);
              G4Fragment * theHandlerPhoton = new G4Fragment(Gamma4P,G4Gamma::GammaDefinition());
              
              
              
              G4double Mass = (*j)->GetGroundStateMass();
              G4ThreeVector ResidualP((*j)->GetMomentum().vect() - GammaP);
              G4double ResidualE = std::sqrt(ResidualP*ResidualP + Mass*Mass);
              G4LorentzVector Residual4P(ResidualP,ResidualE);
              (*j)->SetMomentum(Residual4P);
              
              
	
#ifdef PRECOMPOUND_TEST
              theHandlerPhoton->SetCreatorModel("G4ExcitationHandler");
#endif
//	      theFinalPhotonResultList.push_back( theHandlerPhoton );
//	      G4cout << " adding photon fragment " << G4endl;
	      theResultList.push_back( theHandlerPhoton );
	      //	      theFinalResultList.push_back( theHandlerPhoton );
	      theFinalResultList.push_back(*j);
	      number_other_fragments++;
#ifdef debugphoton
              G4cout << "Emmited photon:\n"
                     << theResultList.back() << '\n'
                     << "Residual nucleus after photon emission:\n"
                     << *(*j) << '\n'
                     << "-----------------------------------------------------------------------\n";
#endif
	      //test	      j++; // AH only increment if not erased:
            }	
        } else {
	  //test	  j++; // AH increment iterator if a proton or excitation energy small
	  theFinalResultList.push_back(*j);
	  number_other_fragments++;
	}
//      G4cout << " Inside loop list: " << (*j)->GetExcitationEnergy() << " size: " << theFinalResultList.size() << G4endl;
      j++;
    }
  //  for (j = theResultList.begin(); j != theResultList.end(); j++)
  for (j = theFinalResultList.begin(); j != theFinalResultList.end(); j++)
    {
      theResult->push_back(*j);
    }

//AHtest   for (j = theFinalPhotonResultList.begin(); j != theFinalPhotonResultList.end(); j++)
//AHtest     {
//AHtest       theResult->push_back(*j);
//AHtest       number_results++;
//AHtest     }


  theResultList.clear();
  theFinalResultList.clear();
  //AHtest  theFinalPhotonResultList.clear();
  
  
#ifdef debug
  CheckConservation(theInitialState,theResult);
#endif
  // Change G4FragmentVector by G4DynamicParticle
  return Transform(theResult);
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





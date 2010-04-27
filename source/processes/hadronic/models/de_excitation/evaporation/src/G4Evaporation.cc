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
// $Id: G4Evaporation.cc,v 1.17 2010-04-27 11:43:16 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//
// Alex Howard - added protection for negative probabilities in the sum, 14/2/07
//
// Modif (03 September 2008) by J. M. Quesada for external choice of inverse 
// cross section option
// JMQ (06 September 2008) Also external choices have been added for 
// superimposed Coulomb barrier (if useSICBis set true, by default is false) 
//
// V.Ivanchenko added G4EvaporationDefaultGEMFactory option, 27/07/09

#include "G4Evaporation.hh"
#include "G4EvaporationFactory.hh"
#include "G4EvaporationGEMFactory.hh"
#include "G4EvaporationDefaultGEMFactory.hh"
#include "G4HadronicException.hh"
#include "G4NistManager.hh"
#include <numeric>

G4Evaporation::G4Evaporation() 
{
  //theChannelFactory = new G4EvaporationFactory();
  theChannelFactory = new G4EvaporationDefaultGEMFactory();
  Initialise();
}

G4Evaporation::G4Evaporation(std::vector<G4VEvaporationChannel*> * aChannelsVector) 
  : theChannels(aChannelsVector), theChannelFactory(0), nChannels(0)
{
  Initialise();
}

G4Evaporation::~G4Evaporation()
{
  if (theChannels != 0) { theChannels = 0; }
  if (theChannelFactory != 0) { delete theChannelFactory; }
}

void G4Evaporation::Initialise()
{
  nist = G4NistManager::Instance();
  minExcitation = CLHEP::keV;
  if(theChannelFactory) { theChannels = theChannelFactory->GetChannel(); }
  nChannels = theChannels->size();
  probabilities.resize(nChannels, 0.0);
  // loop over evaporation channels
  std::vector<G4VEvaporationChannel*>::iterator i;
  for (i=theChannels->begin(); i != theChannels->end(); i++) 
    {
      // for inverse cross section choice
      (*i)->SetOPTxs(OPTxs);
      // for superimposed Coulomb Barrier for inverse cross sections
      (*i)->UseSICB(useSICB);
    }
}

void G4Evaporation::SetDefaultChannel()
{
  if (theChannelFactory != 0) delete theChannelFactory;
  theChannelFactory = new G4EvaporationFactory();
  Initialise();
}

void G4Evaporation::SetGEMChannel()
{
  if (theChannelFactory != 0) delete theChannelFactory;
  theChannelFactory = new G4EvaporationGEMFactory();
  Initialise();
}

void G4Evaporation::SetCombinedChannel()
{
  if (theChannelFactory != 0) delete theChannelFactory;
  theChannelFactory = new G4EvaporationDefaultGEMFactory();
  Initialise();
}

G4FragmentVector * G4Evaporation::BreakItUp(const G4Fragment &theNucleus)
{
  G4FragmentVector * theResult = new G4FragmentVector;
  G4FragmentVector * theTempResult;

  // The residual nucleus (after evaporation of each fragment)
  G4Fragment* theResidualNucleus = new G4Fragment(theNucleus);

  G4double totprob, prob;
  G4int maxchannel, i;

  G4int A = static_cast<G4int>( theResidualNucleus->GetA() + 0.5 );

  // Starts loop over evaporated particles, loop is limited by number
  // of nucleons
  for(G4int ia=0; ia<A; ++ia) {
 
    totprob = 0.0;
    maxchannel = nChannels;

    // loop over evaporation channels
    for(i=0; i<nChannels; ++i) {
      (*theChannels)[i]->Initialize(*theResidualNucleus);
      prob = (*theChannels)[i]->GetEmissionProbability();
      totprob += prob;
      probabilities[i] = totprob;
      if(prob <= 0.0 && i>=8) {
        maxchannel = i+1; 
	break;
      }
    }

    // stable fragnent - evaporation is complited
    if(0.0 == totprob) {
      theResult->push_back(theResidualNucleus);
      return theResult;
    }

    // select channel
    totprob*G4UniformRand();
    // loop over evaporation channels
    for(i=0; i<maxchannel; ++i) { if(probabilities[i] >= totprob) { break; } }

    // this should not happen
    if(i >= nChannels) { i = nChannels - 1; }

    // photon evaporation in the case of no other channels available
    // do evaporation chain and return results
    if(0 == i && probabilities[0] == totprob) {
      theTempResult = (*theChannels)[0]->BreakUpFragment(theResidualNucleus);
      if(theTempResult) {
	size_t nsec = theTempResult->size();
	for(size_t j=0; j<nsec; ++j) {
	  theResult->push_back((*theTempResult)[j]);
	}
        delete theTempResult;
      }
      return theResult;

    // single photon evaporation, primary pointer is kept
    } else if(0 == i) {
      G4Fragment* gamma = (*theChannels)[0]->EmittedFragment(theResidualNucleus);
      if(gamma) { theResult->push_back(gamma); }

      // fission, return results to the main loop if fission is succesful
    } else if(1 == i) {
      theTempResult = (*theChannels)[1]->BreakUp(*theResidualNucleus);
      if(theTempResult) {
	size_t nsec = theTempResult->size();
        G4bool deletePrimary = true;
	for(size_t j=0; j<nsec; ++j) {
          if(theResidualNucleus == (*theTempResult)[j]) { deletePrimary = false; }
	  theResult->push_back((*theTempResult)[j]);
	}
        if(deletePrimary) { delete theResidualNucleus; }
        delete theTempResult;
	return theResult;
      }

      // other channels
    } else {
      theTempResult = (*theChannels)[i]->BreakUp(*theResidualNucleus);
      if(theTempResult) {
	size_t nsec = theTempResult->size();
        if(nsec > 0) {
          --nsec;
	  for(size_t j=0; j<nsec; ++j) {
	    theResult->push_back((*theTempResult)[j]);
	  }
	  // if the residual change its pointer 
	  // then delete previous residual fragment and update to the new
          if(theResidualNucleus != (*theTempResult)[nsec] ) { 
	    delete theResidualNucleus; 
	    theResidualNucleus = (*theTempResult)[nsec];

	    // check if it is stable, then finish evaporation
            if(theResidualNucleus->GetExcitationEnergy() > minExcitation) {
	      G4int iz = static_cast<G4int>(theResidualNucleus->GetZ()+0.5);  
	      G4int ia = static_cast<G4int>(theResidualNucleus->GetA()+0.5);  
              if( nist->GetIsotopeAbundance(iz, ia) > 0.0 ) {
		theResult->push_back(theResidualNucleus);
                return theResult;
	      }
	    }
	  }
	}
        delete theTempResult;
      }
    }
  }
  // loop is stopped, save residual
  theResult->push_back(theResidualNucleus);
  
#ifdef debug
  G4cout << "======== Evaporation Conservation Test ===========\n"
	 << "==================================================\n";
  CheckConservation(theNucleus,theResult);
  G4cout << "==================================================\n";
#endif
  return theResult;
}

/*
G4FragmentVector * G4Evaporation::BreakItUp(const G4Fragment &theNucleus)
{
  G4FragmentVector * theResult = new G4FragmentVector;

    // CHECK that Excitation Energy != 0
    if (theNucleus.GetExcitationEnergy() <= 0.0) {
	theResult->push_back(new G4Fragment(theNucleus));
	return theResult;
    }

    // The residual nucleus (after evaporation of each fragment)
    G4Fragment theResidualNucleus = theNucleus;

    // Number of channels
    G4int TotNumberOfChannels = theChannels->size();  

    // Starts loop over evaporated particles
    for (;;) 
 
   {    
	// loop over evaporation channels
	std::vector<G4VEvaporationChannel*>::iterator i;
	for (i=theChannels->begin(); i != theChannels->end(); i++) 
	  {
  // for inverse cross section choice
            (*i)->SetOPTxs(OPTxs);
  // for superimposed Coulomb Barrier for inverse cross sections
            (*i)->UseSICB(useSICB);

	    (*i)->Initialize(theResidualNucleus);
	  }
	// Can't use this form beacuse Initialize is a non const member function
	//  	for_each(theChannels->begin(),theChannels->end(),
	//  		 bind2nd(mem_fun(&G4VEvaporationChannel::Initialize),theResidualNucleus));
	// Work out total decay probability by summing over channels 
  	G4double TotalProbability =  std::accumulate(theChannels->begin(),
						       theChannels->end(),
						       0.0,SumProbabilities());
	
	if (TotalProbability <= 0.0) 
	  {
	    // Will be no evaporation more
	    // write information about residual nucleus
	    theResult->push_back(new G4Fragment(theResidualNucleus));
	    break; 
	  } 
	else 
	  {
	    // Selection of evaporation channel, fission or gamma
	    // G4double * EmissionProbChannel = new G4double(TotNumberOfChannels);
	    std::vector<G4double> EmissionProbChannel;
	    EmissionProbChannel.reserve(theChannels->size());
	    

	    // EmissionProbChannel[0] = theChannels->at(0)->GetEmissionProbability();


	    G4double first = theChannels->front()->GetEmissionProbability();

	    EmissionProbChannel.push_back(first >0 ? first : 0); // index 0


	    //	    EmissionProbChannel.push_back(theChannels->front()->GetEmissionProbability()); // index 0
	    
	    for (i= (theChannels->begin()+1); i != theChannels->end(); i++) 
	      {
		// EmissionProbChannel[i] = EmissionProbChannel[i-1] + 
		// theChannels->at(i)->GetEmissionProbability();
		//		EmissionProbChannel.push_back(EmissionProbChannel.back() + (*i)->GetEmissionProbability());
		first = (*i)->GetEmissionProbability();
		EmissionProbChannel.push_back(first> 0? EmissionProbChannel.back() + first : EmissionProbChannel.back());
	      }

	    G4double shoot = G4UniformRand() * TotalProbability;
	    G4int j;
	    for (j=0; j < TotNumberOfChannels; j++) 
	      {
		// if (shoot < EmissionProbChannel[i]) 
		if (shoot < EmissionProbChannel[j]) 
		  break;
	      }
	    
	    // delete [] EmissionProbChannel;
	    EmissionProbChannel.clear();
			
	    if( j >= TotNumberOfChannels ) 
	      {
		G4cerr << " Residual A: " << theResidualNucleus.GetA() << " Residual Z: " << theResidualNucleus.GetZ() << " Excitation Energy: " << theResidualNucleus.GetExcitationEnergy() << G4endl;
		G4cerr << " j has not chosen a channel, j = " << j << " TotNumberOfChannels " << TotNumberOfChannels << " Total Probability: " << TotalProbability << G4endl;
		for (j=0; j < TotNumberOfChannels; j++) 
		  {
		    G4cerr << " j: " << j << " EmissionProbChannel: " << EmissionProbChannel[j] << " and shoot: " << shoot << " (<ProbChannel?) " << G4endl;
		  }		
		throw G4HadronicException(__FILE__, __LINE__,  "G4Evaporation::BreakItUp: Can't define emission probability of the channels!" );
	      } 
	    else 
	      {
		// Perform break-up
		G4FragmentVector * theEvaporationResult = (*theChannels)[j]->BreakUp(theResidualNucleus);

#ifdef debug
		G4cout <<           "-----------------------------------------------------------\n"; 
		G4cout << G4endl << " After the evaporation of a particle, testing conservation \n";
		CheckConservation(theResidualNucleus,theEvaporationResult);
		G4cout << G4endl 
		       <<           "------------------------------------------------------------\n"; 
#endif  

		// Check if chosen channel is fission (there are only two EXCITED fragments)
		// or the channel could not evaporate anything
		if ( theEvaporationResult->size() == 1 || 
		     ((*(theEvaporationResult->begin()))->GetExcitationEnergy() > 0.0 && 
		      (*(theEvaporationResult->end()-1))->GetExcitationEnergy() > 0.0) ) {
		  // FISSION 
		  for (G4FragmentVector::iterator i = theEvaporationResult->begin();
		       i != theEvaporationResult->end(); ++i) 
		    {
		      theResult->push_back(*(i));
		    }
		  delete theEvaporationResult;
		  break;
		} else {
		  // EVAPORATION
		  for (G4FragmentVector::iterator i = theEvaporationResult->begin();
		       i != theEvaporationResult->end()-1; ++i) 
		    {
#ifdef PRECOMPOUND_TEST
		      if ((*i)->GetA() == 0) (*i)->SetCreatorModel(G4String("G4PhotonEvaporation"));
#endif
		      theResult->push_back(*(i));
		    }
		  theResidualNucleus = *(theEvaporationResult->back());
		  delete theEvaporationResult->back();
		  delete theEvaporationResult;
#ifdef PRECOMPOUND_TEST
		  theResidualNucleus.SetCreatorModel(G4String("ResidualNucleus"));
#endif

		}
	      }
	  }
      }
    
#ifdef debug
    G4cout << "======== Evaporation Conservation Test ===========\n"
	   << "==================================================\n";
    CheckConservation(theNucleus,theResult);
    G4cout << "==================================================\n";
#endif
    return theResult;
}
*/


#ifdef debug
void G4Evaporation::CheckConservation(const G4Fragment & theInitialState,
				      G4FragmentVector * Result) const
{
    G4double ProductsEnergy =0;
    G4ThreeVector ProductsMomentum;
    G4int ProductsA = 0;
    G4int ProductsZ = 0;
    for (G4FragmentVector::iterator h = Result->begin(); h != Result->end(); h++) {
	G4LorentzVector tmp = (*h)->GetMomentum();
	ProductsEnergy += tmp.e();
	ProductsMomentum += tmp.vect();
	ProductsA += static_cast<G4int>((*h)->GetA());
	ProductsZ += static_cast<G4int>((*h)->GetZ());
    }

    if (ProductsA != theInitialState.GetA()) {
	G4cout << "!!!!!!!!!! Baryonic Number Conservation Violation !!!!!!!!!!" << G4endl;
	G4cout << "G4Evaporation.cc: Barionic Number Conservation test for evaporation fragments" 
	       << G4endl; 
	G4cout << "Initial A = " << theInitialState.GetA() 
	       << "   Fragments A = " << ProductsA << "   Diference --> " 
	       << theInitialState.GetA() - ProductsA << G4endl;
    }
    if (ProductsZ != theInitialState.GetZ()) {
	G4cout << "!!!!!!!!!! Charge Conservation Violation !!!!!!!!!!" << G4endl;
	G4cout << "G4Evaporation.cc: Charge Conservation test for evaporation fragments" 
	       << G4endl; 
	G4cout << "Initial Z = " << theInitialState.GetZ() 
	       << "   Fragments Z = " << ProductsZ << "   Diference --> " 
	       << theInitialState.GetZ() - ProductsZ << G4endl;
    }
    if (std::abs(ProductsEnergy-theInitialState.GetMomentum().e()) > 1.0*keV) {
	G4cout << "!!!!!!!!!! Energy Conservation Violation !!!!!!!!!!" << G4endl;
	G4cout << "G4Evaporation.cc: Energy Conservation test for evaporation fragments" 
	       << G4endl; 
	G4cout << "Initial E = " << theInitialState.GetMomentum().e()/MeV << " MeV"
	       << "   Fragments E = " << ProductsEnergy/MeV  << " MeV   Diference --> " 
	       << (theInitialState.GetMomentum().e() - ProductsEnergy)/MeV << " MeV" << G4endl;
    } 
    if (std::abs(ProductsMomentum.x()-theInitialState.GetMomentum().x()) > 1.0*keV || 
	std::abs(ProductsMomentum.y()-theInitialState.GetMomentum().y()) > 1.0*keV ||
	std::abs(ProductsMomentum.z()-theInitialState.GetMomentum().z()) > 1.0*keV) {
	G4cout << "!!!!!!!!!! Momentum Conservation Violation !!!!!!!!!!" << G4endl;
	G4cout << "G4Evaporation.cc: Momentum Conservation test for evaporation fragments" 
	       << G4endl; 
	G4cout << "Initial P = " << theInitialState.GetMomentum().vect() << " MeV"
	       << "   Fragments P = " << ProductsMomentum  << " MeV   Diference --> " 
	       << theInitialState.GetMomentum().vect() - ProductsMomentum << " MeV" << G4endl;
    }
    return;
}
#endif





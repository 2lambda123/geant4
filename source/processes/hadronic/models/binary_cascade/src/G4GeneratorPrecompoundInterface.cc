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
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4DynamicParticleVector.hh"
#include "G4IonTable.hh"

//
// HPW, 10DEC 98, the decay part originally written by Gunter Folger in his FTF-test-program.
//
      
   G4HadFinalState* G4GeneratorPrecompoundInterface::
   ApplyYourself(const G4HadProjectile &, G4Nucleus & )
   {
     std::cout << "G4GeneratorPrecompoundInterface: ApplyYourself interface called stand-allone."<< G4endl;
     std::cout << "This class is only a mediator between generator and precompound"<<G4endl;
     std::cout << "Please remove from your physics list."<<G4endl;
     throw G4HadronicException(__FILE__, __LINE__, "SEVERE: G4GeneratorPrecompoundInterface model interface called stand-allone.");
     return new G4HadFinalState;
   }
   
   G4ReactionProductVector* G4GeneratorPrecompoundInterface::
   Propagate(G4KineticTrackVector* theSecondaries, G4V3DNucleus* theNucleus)
   {
     G4ReactionProductVector * theTotalResult = new G4ReactionProductVector;

     // decay the strong resonances
     G4KineticTrackVector *result1, *secondaries, *result;
     result1=theSecondaries;
     result=new G4KineticTrackVector();
     
     for (unsigned int aResult=0; aResult < result1->size(); aResult++)
     {
       G4ParticleDefinition * pdef;
       pdef=result1->operator[](aResult)->GetDefinition();
       secondaries=NULL;
       if ( pdef->IsShortLived() )
       {
	  secondaries = result1->operator[](aResult)->Decay();
       }
       if ( secondaries == NULL )
       {
	  result->push_back(result1->operator[](aResult));
	  result1->operator[](aResult)=NULL;	//protect for clearAndDestroy 
       } 
       else
       {
	 for (unsigned int aSecondary=0; aSecondary<secondaries->size(); aSecondary++)
	 {
	     result1->push_back(secondaries->operator[](aSecondary));
	 }
	 delete secondaries;
       }
     }
     std::for_each(result1->begin(), result1->end(), DeleteKineticTrack());
     delete result1;
     
     
     
     // prepare the fragment
     G4Fragment anInitialState;
     G4int anA=theNucleus->GetMassNumber();
     G4int aZ=theNucleus->GetCharge();
     G4int numberOfEx = 0;
     G4int numberOfCh = 0;
     G4int numberOfHoles = 0;
     G4double exEnergy = 0;
     G4ThreeVector exciton3Momentum(0,0,0);
     // loop over secondaries
     for(unsigned int list=0; list < result->size(); list++)
     {
       G4KineticTrack *aTrack = result->operator[](list);
       if(aTrack->GetDefinition() != G4Proton::Proton() && 
          aTrack->GetDefinition() != G4Neutron::Neutron())
       {
	 G4ReactionProduct * theNew = new G4ReactionProduct(aTrack->GetDefinition());
	 theNew->SetMomentum(aTrack->Get4Momentum().vect());
	 theNew->SetTotalEnergy(aTrack->Get4Momentum().e());
         theTotalResult->push_back(theNew);            
       }
       else if(aTrack->Get4Momentum().t() - aTrack->Get4Momentum().mag()>80*MeV)
       {
	 G4ReactionProduct * theNew = new G4ReactionProduct(aTrack->GetDefinition());
	 theNew->SetMomentum(aTrack->Get4Momentum().vect());
	 theNew->SetTotalEnergy(aTrack->Get4Momentum().e());
         theTotalResult->push_back(theNew);            
       }
       else if(aTrack->GetPosition().mag() > theNucleus->GetNuclearRadius())
       {
	 G4ReactionProduct * theNew = new G4ReactionProduct(aTrack->GetDefinition());
	 theNew->SetMomentum(aTrack->Get4Momentum().vect());
	 theNew->SetTotalEnergy(aTrack->Get4Momentum().e());
         theTotalResult->push_back(theNew);            
       }
       else
       {
         // within the nucleus, neutron or proton
         // now calculate  A, Z of the fragment, momentum, number of exciton states
         anA++;;
         numberOfEx++;
         aZ += G4int(aTrack->GetDefinition()->GetPDGCharge());
         numberOfCh += G4int(aTrack->GetDefinition()->GetPDGCharge());
         exciton3Momentum += aTrack->Get4Momentum().vect();
         exEnergy += (aTrack->Get4Momentum().t()-aTrack->Get4Momentum().m());
       }
     }
     
     // loop over wounded nucleus
     G4Nucleon * theCurrentNucleon = theNucleus->StartLoop() ? theNucleus->GetNextNucleon() : NULL;
     while(theCurrentNucleon != NULL)
     {
       if(theCurrentNucleon->AreYouHit()) 
       {
         numberOfHoles++;
         numberOfEx++;
         anA--;
         aZ -= G4int(theCurrentNucleon->GetDefinition()->GetPDGCharge());
         exciton3Momentum -= theCurrentNucleon->Get4Momentum().vect();
         exEnergy+=theCurrentNucleon->GetBindingEnergy();
       }
       theCurrentNucleon = theNucleus->GetNextNucleon();
     }   
     
     if(!theDeExcitation)
     {
       // throw G4HadronicException(__FILE__, __LINE__, "Please register an evaporation phase with G4GeneratorPrecompoundInterface.");
     }
     else if(0!=anA && 0!=aZ)
     {
       G4double residualMass =  
                G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(aZ ,anA);
       residualMass += exEnergy;
       G4LorentzVector exciton4Momentum(exciton3Momentum, 
                                        std::sqrt(exciton3Momentum.mag2()+residualMass*residualMass));
     
       anInitialState.SetA(anA);
       anInitialState.SetZ(aZ);
       anInitialState.SetNumberOfParticles(numberOfEx-numberOfHoles);
       anInitialState.SetNumberOfCharged(numberOfCh);
       anInitialState.SetNumberOfHoles(numberOfHoles);
       anInitialState.SetMomentum(exciton4Momentum);
//      anInitialState.SetExcitationEnergy(exEnergy); // now a redundant call.

     // call pre-compound
       const G4Fragment aFragment(anInitialState);
       G4ReactionProductVector * aPreResult = theDeExcitation->DeExcite(aFragment);
//       G4ReactionProductVector * aPreResult = new G4ReactionProductVector;
       // fill pre-compound part into the result, and return
       for(unsigned int ll=0; ll<aPreResult->size(); ll++)
       {
         theTotalResult->push_back(aPreResult->operator[](ll));
       }
       delete aPreResult;
     }
     else
     {
       // throw G4HadronicException(__FILE__, __LINE__, "Please register an evaporation phase with G4GeneratorPrecompoundInterface.");
     }
     // now return
     
     std::for_each(result->begin(), result->end(), DeleteKineticTrack());
     delete result;
     return theTotalResult;
   }
  

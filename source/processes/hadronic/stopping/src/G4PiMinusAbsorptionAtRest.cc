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
// $Id: G4PiMinusAbsorptionAtRest.cc,v 1.5 2001-07-11 10:08:09 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     G4PiMinusAbsorptionAtRest.hh 
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it)
// 
//      Creation date: 8 May 1998
//
//      Modifications: 
//      MGP            4 Jul 1998 Changed excitation energy calculation
//      MGP           14 Sep 1998 Fixed excitation energy calculation
//
// -------------------------------------------------------------------

#include "G4ios.hh"

#include "G4PiMinusAbsorptionAtRest.hh"

#include "G4PiMinusStopLi.hh"
#include "G4PiMinusStopC.hh"
#include "G4PiMinusStopN.hh"
#include "G4PiMinusStopO.hh"
#include "G4PiMinusStopAl.hh"
#include "G4PiMinusStopCu.hh"
#include "G4PiMinusStopCo.hh"
#include "G4PiMinusStopTa.hh"
#include "G4PiMinusStopPb.hh"
#include "G4StopTheoDeexcitation.hh"
#include "G4StopDummyDeexcitation.hh"
#include "G4DynamicParticle.hh"
#include "G4DynamicParticleVector.hh"
#include "G4NucleiPropertiesTable.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4LorentzVector.hh"

// Constructor

G4PiMinusAbsorptionAtRest::G4PiMinusAbsorptionAtRest(const G4String& processName)
  : G4VRestProcess (processName)
{
  //  _stopAbsorption = 0;
  //  _stopDeexcitation = 0;

  _indexDeexcitation = 0;

  if (verboseLevel>0) 
    { G4cout << GetProcessName() << " is created "<< G4endl; }
}


// Destructor

G4PiMinusAbsorptionAtRest::~G4PiMinusAbsorptionAtRest()
{}


G4VParticleChange* G4PiMinusAbsorptionAtRest::AtRestDoIt(const G4Track& track, const G4Step& Step)
{
  const G4DynamicParticle* stoppedHadron = track.GetDynamicParticle();
      
  // Check applicability
  if (! IsApplicable(*(stoppedHadron->GetDefinition())))
    {
      G4cerr  
      << "G4PiMinusAbsorptionAtRest: ERROR, particle must be a pion minus!" 
      << G4endl;
      return NULL;
    }

  // Get the current material
  const G4Material* material = track.GetMaterial();

  G4double A;
  G4double Z;
  G4double random = G4UniformRand();
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int i;
  G4double sum = 0;
  G4double totalsum=0;
  for(i=0; i<material->GetNumberOfElements(); ++i)
    {
      if((*theElementVector)(i)->GetZ()!=1) totalsum+=material->GetFractionVector()[i];
    }
  for (i = 0; i<material->GetNumberOfElements(); ++i)
    {
      if((*theElementVector)(i)->GetZ()!=1) sum += material->GetFractionVector()[i];
      if ( sum/totalsum > random )  
	{ 
	  A = (*theElementVector)(i)->GetA()*mole/g;
	  Z = (*theElementVector)(i)->GetZ();
          break;
	}
    }

  // Do the interaction with the nucleon cluster
  
  G4PiMinusStopMaterial* algorithm = LoadAlgorithm(Z);
  G4PiMinusStopAbsorption stopAbsorption(algorithm,Z,A);
  stopAbsorption.SetVerboseLevel(verboseLevel);

  G4DynamicParticleVector* absorptionProducts = stopAbsorption.DoAbsorption();

  // Deal with the leftover nucleus

  G4double pionEnergy = stoppedHadron->GetTotalEnergy();
  G4double excitation = pionEnergy - stopAbsorption.Energy();
  if (excitation < 0.) G4Exception("G4PiMinusAbsorptionAtRest::AtRestDoIt -- excitation energy < 0");
  if (verboseLevel>0) { G4cout << " excitation " << excitation << G4endl; }

  G4StopDeexcitationAlgorithm* nucleusAlgorithm = LoadNucleusAlgorithm();
  G4StopDeexcitation stopDeexcitation(nucleusAlgorithm);

  G4double newZ = Z - stopAbsorption.NProtons();
  G4double newN = A - Z - stopAbsorption.NNeutrons();
  G4double newA = newZ + newN;
  G4double pNucleus = (stopAbsorption.RecoilMomentum()).mag();
  G4ReactionProductVector* fragmentationProducts = stopDeexcitation.DoBreakUp(newA,newZ,excitation,pNucleus);

  G4int nAbsorptionProducts = 0;
  if (absorptionProducts != 0)     
    { nAbsorptionProducts  =  absorptionProducts->entries(); }

  G4int nFragmentationProducts = 0;
  if (fragmentationProducts != 0) 
    { nFragmentationProducts = fragmentationProducts->entries(); }
  
  if (verboseLevel>0) 
    {
      G4cout << "nAbsorptionProducts = " << nAbsorptionProducts
	     << "  nFragmentationProducts = " << nFragmentationProducts
	     << G4endl;
    }

  // Deal with ParticleChange final state

  aParticleChange.Initialize(track);
  aParticleChange.SetNumberOfSecondaries(G4int(nAbsorptionProducts + nFragmentationProducts)); 
     
  for (i = 0; i<nAbsorptionProducts; i++)
    { aParticleChange.AddSecondary(absorptionProducts->at(i)); }

//  for (i = 0; i<nFragmentationProducts; i++)
//    { aParticleChange.AddSecondary(fragmentationProducts->at(i)); }
  for(i=0; i<nFragmentationProducts; i++)
  {
    G4DynamicParticle * aNew = 
       new G4DynamicParticle(fragmentationProducts->at(i)->GetDefinition(),
                             fragmentationProducts->at(i)->GetMomentum());
    G4double newTime = aParticleChange.GetGlobalTime(fragmentationProducts->at(i)->GetFormationTime());
    aParticleChange.AddSecondary(aNew, newTime);
    delete fragmentationProducts->at(i);
  }

  if (fragmentationProducts != 0)   delete fragmentationProducts;   

  if (_indexDeexcitation == 1) aParticleChange.SetLocalEnergyDeposit(excitation);

  // Kill the absorbed pion
  aParticleChange.SetStatusChange(fStopAndKill); 

  return &aParticleChange;

}

G4PiMinusStopMaterial* G4PiMinusAbsorptionAtRest::LoadAlgorithm(int Z)
{  
  if (verboseLevel>0) 
    {
      G4cout << "Load material algorithm " << Z << G4endl; 
    }

  G4int index = 3;
  if (Z <= 3) { index = 3;}
  if (Z > 3 && Z<= 6) {index = 6;}
  if (Z == 7) {index = 7;}
  if (Z >= 8 && Z<= 11) {index = 8;}
  if (Z >= 12 && Z<= 18) {index = 13;}
  if (Z >=19 && Z<= 27) {index = 27;}
  if (Z >= 28 && Z<= 51) {index = 29;}
  if (Z >=52 ) {index = 73;}

  switch (index) 
    {
    case 3:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load Li algorithm " << G4endl; }
      return new G4PiMinusStopLi();
    case 6:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load C algorithm " << G4endl; }
      return new G4PiMinusStopC();
    case 7:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load N algorithm " << G4endl; }
      return new G4PiMinusStopN();
    case 8:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load O algorithm " << G4endl; }
      return new G4PiMinusStopO();
    case 13:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load Al algorithm " << G4endl; }
      return new G4PiMinusStopAl();
    case 27:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load Cu algorithm " << G4endl; }
      return new G4PiMinusStopCu();
    case 29:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load Co algorithm " << G4endl; }
      return new G4PiMinusStopCo();
    case 73:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load Ta algorithm " << G4endl; }
      return new G4PiMinusStopTa();
    default: 
      if (verboseLevel>0) 
	{ G4cout << " =================== Load default material algorithm " << G4endl; }
      return new G4PiMinusStopC();
    }
}

G4StopDeexcitationAlgorithm* G4PiMinusAbsorptionAtRest::LoadNucleusAlgorithm()
{  

  switch (_indexDeexcitation) 
    {
    case 0:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load Theo deexcitation " << G4endl; }
      return new G4StopTheoDeexcitation();
    case 1:
      if (verboseLevel>0) 
	{ G4cout << " =================== Load Dummy deexcitation " << G4endl; }
      return new G4StopDummyDeexcitation();
    default:  
      if (verboseLevel>0) 
	{ G4cout << " =================== Load default deexcitation " << G4endl; }
      return new G4StopTheoDeexcitation();
    }
}

void G4PiMinusAbsorptionAtRest::SetDeexcitationAlgorithm(G4int index)
{  
  _indexDeexcitation = index;
}

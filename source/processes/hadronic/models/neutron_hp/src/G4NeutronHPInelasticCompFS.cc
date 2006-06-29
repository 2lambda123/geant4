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
// neutron_hp -- source file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
#include "G4NeutronHPInelasticCompFS.hh"
#include "G4Nucleus.hh"
#include "G4NucleiPropertiesTable.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4Electron.hh"
#include "G4NeutronHPDataUsed.hh"
#include "G4ParticleTable.hh"

void G4NeutronHPInelasticCompFS::InitGammas(G4double AR, G4double ZR)
{
  //   char the[100] = {""};
  //   std::ostrstream ost(the, 100, std::ios::out);
  //   ost <<gammaPath<<"z"<<ZR<<".a"<<AR;
  //   G4String * aName = new G4String(the);
  //   std::ifstream from(*aName, std::ios::in);

   std::ostringstream ost;
   ost <<gammaPath<<"z"<<ZR<<".a"<<AR;
   G4String aName = ost.str();
   std::ifstream from(aName, std::ios::in);

   if(!from) return; // no data found for this isotope
   //   std::ifstream theGammaData(*aName, std::ios::in);
   std::ifstream theGammaData(aName, std::ios::in);
    
   theGammas.Init(theGammaData);
   //   delete aName;
}

void G4NeutronHPInelasticCompFS::Init (G4double A, G4double Z, G4String & dirName, G4String & aFSType)
{
  gammaPath = "/Inelastic/Gammas/";
    if(!getenv("NeutronHPCrossSections")) 
       throw G4HadronicException(__FILE__, __LINE__, "Please setenv NeutronHPCrossSections to point to the neutron cross-section files.");
  G4String tBase = getenv("NeutronHPCrossSections");
  gammaPath = tBase+gammaPath;
  G4String tString = dirName;
  G4bool dbool;
  G4NeutronHPDataUsed aFile = theNames.GetName(static_cast<G4int>(A), static_cast<G4int>(Z), tString, aFSType, dbool);
  G4String filename = aFile.GetName();
  theBaseA = aFile.GetA();
  theBaseZ = aFile.GetZ();
  if(!dbool || ( Z<2.5 && ( std::abs(theBaseZ - Z)>0.0001 || std::abs(theBaseA - A)>0.0001)))
  {
    if(getenv("NeutronHPNamesLogging")) G4cout << "Skipped = "<< filename <<" "<<A<<" "<<Z<<G4endl;
    hasAnyData = false;
    hasFSData = false; 
    hasXsec = false;
    return;
  }
  theBaseA = A;
  theBaseZ = G4int(Z+.5);
  std::ifstream theData(filename, std::ios::in);
  if(!theData)
  {
    hasAnyData = false;
    hasFSData = false; 
    hasXsec = false;
    theData.close();
    return;
  }
  // here we go
  G4int infoType, dataType, dummy;
  G4int sfType, it;
  hasFSData = false; 
  while (theData >> infoType)
  {
    hasFSData = true; 
    theData >> dataType;
    theData >> sfType >> dummy;
    it = 50;
    if(sfType>=600||(sfType<100&&sfType>=50)) it = sfType%50;
    if(dataType==3) 
    {
      theData >> dummy >> dummy;
      theXsection[it] = new G4NeutronHPVector;
      G4int total;
      theData >> total;
      theXsection[it]->Init(theData, total, eV);
    }
    else if(dataType==4)
    {
      theAngularDistribution[it] = new G4NeutronHPAngular;
      theAngularDistribution[it]->Init(theData);
    }
    else if(dataType==5)
    {
      theEnergyDistribution[it] = new G4NeutronHPEnergyDistribution;
      theEnergyDistribution[it]->Init(theData); 
    }
    else if(dataType==6)
    {
      theEnergyAngData[it] = new G4NeutronHPEnAngCorrelation;
      theEnergyAngData[it]->Init(theData);
    }
    else if(dataType==12)
    {
      theFinalStatePhotons[it] = new G4NeutronHPPhotonDist;
      theFinalStatePhotons[it]->InitMean(theData);
    }
    else if(dataType==13)
    {
      theFinalStatePhotons[it] = new G4NeutronHPPhotonDist;
      theFinalStatePhotons[it]->InitPartials(theData);
    }
    else if(dataType==14)
    {
      theFinalStatePhotons[it]->InitAngular(theData);
    }
    else if(dataType==15)
    {
      theFinalStatePhotons[it]->InitEnergies(theData);
    }
    else
    {
      throw G4HadronicException(__FILE__, __LINE__, "Data-type unknown to G4NeutronHPInelasticCompFS");
    }
  }
  theData.close();
}

G4int G4NeutronHPInelasticCompFS::SelectExitChannel(G4double eKinetic)
{
  G4double running[50];
  running[0] = 0;
  unsigned int i;
  for(i=0; i<50; i++)
  {
    if(i!=0) running[i]=running[i-1];
    if(theXsection[i] != NULL) 
    {
      running[i] += std::max(0., theXsection[i]->GetXsec(eKinetic));
    }
  }
  G4double random = G4UniformRand();
  G4double sum = running[49];
  G4int it = 50;
  if(0!=sum)
  {
    G4int i0;
    for(i0=0; i0<50; i0++)
    {
      it = i0;
      if(random < running[i0]/sum) break;
    }
  }
//debug:  it = 1;
  return it;
}

void G4NeutronHPInelasticCompFS::CompositeApply(const G4HadProjectile & theTrack, G4ParticleDefinition * aDefinition)
{

// prepare neutron
    theResult.Clear();
    G4double eKinetic = theTrack.GetKineticEnergy();
    const G4HadProjectile *incidentParticle = &theTrack;
    G4ReactionProduct theNeutron( const_cast<G4ParticleDefinition *>(incidentParticle->GetDefinition()) );
    theNeutron.SetMomentum( incidentParticle->Get4Momentum().vect() );
    theNeutron.SetKineticEnergy( eKinetic );

// prepare target
    G4int i;
    for(i=0; i<50; i++) if(theXsection[i] != NULL) break; 
    G4double targetMass=0;
    G4double eps = 0.0001;
    targetMass = ( G4NucleiPropertiesTable::GetNuclearMass(static_cast<G4int>(theBaseZ+eps), static_cast<G4int>(theBaseA+eps))) /
                   G4Neutron::Neutron()->GetPDGMass();
//    if(theEnergyAngData[i]!=NULL)
//        targetMass = theEnergyAngData[i]->GetTargetMass();
//    else if(theAngularDistribution[i]!=NULL)
//        targetMass = theAngularDistribution[i]->GetTargetMass();
//    else if(theFinalStatePhotons[50]!=NULL)
//        targetMass = theFinalStatePhotons[50]->GetTargetMass();
    G4Nucleus aNucleus;
    G4ReactionProduct theTarget; 
    G4ThreeVector neuVelo = (1./incidentParticle->GetDefinition()->GetPDGMass())*theNeutron.GetMomentum();
    theTarget = aNucleus.GetBiasedThermalNucleus( targetMass, neuVelo, theTrack.GetMaterial()->GetTemperature());

// prepare the residual mass
    G4double residualMass=0;
    G4double residualZ = theBaseZ - aDefinition->GetPDGCharge();
    G4double residualA = theBaseA - aDefinition->GetBaryonNumber()+1;
    residualMass = ( G4NucleiPropertiesTable::GetNuclearMass(static_cast<G4int>(residualZ+eps), static_cast<G4int>(residualA+eps)) ) /
                     G4Neutron::Neutron()->GetPDGMass();

// prepare energy in target rest frame
    G4ReactionProduct boosted;
    boosted.Lorentz(theNeutron, theTarget);
    eKinetic = boosted.GetKineticEnergy();
//    G4double momentumInCMS = boosted.GetTotalMomentum();
  
// select exit channel for composite FS class.
    G4int it = SelectExitChannel(eKinetic);
   
// set target and neutron in the relevant exit channel
    InitDistributionInitialState(theNeutron, theTarget, it);    

    G4ReactionProductVector * thePhotons = NULL;
    G4ReactionProductVector * theParticles = NULL;
    G4ReactionProduct aHadron;
    aHadron.SetDefinition(aDefinition); // what if only cross-sections exist ==> Na 23 11 @@@@    
    G4double availableEnergy = theNeutron.GetKineticEnergy() + theNeutron.GetMass() - aHadron.GetMass() +
                             (targetMass - residualMass)*G4Neutron::Neutron()->GetPDGMass();
    G4int nothingWasKnownOnHadron = 0;
    G4int dummy;
    G4double eGamm = 0;
    G4int iLevel=it-1;
    if(50==it) 
    {
      iLevel=-1;
      aHadron.SetKineticEnergy(availableEnergy*residualMass*G4Neutron::Neutron()->GetPDGMass()/
                               (aHadron.GetMass()+residualMass*G4Neutron::Neutron()->GetPDGMass()));
      aHadron.SetMomentum(theNeutron.GetMomentum()*(1./theNeutron.GetTotalMomentum())*
                           std::sqrt(aHadron.GetTotalEnergy()*aHadron.GetTotalEnergy()-
                                aHadron.GetMass()*aHadron.GetMass()));
    }
    else
    {
      while( iLevel!=-1 && theGammas.GetLevel(iLevel)==NULL ) iLevel--;
    }
    if(theAngularDistribution[it]!= NULL)
    {
      if(theEnergyDistribution[it]!=NULL)
      {
        aHadron.SetKineticEnergy(theEnergyDistribution[it]->Sample(eKinetic, dummy));
        G4double eSecN = aHadron.GetKineticEnergy();
        eGamm = eKinetic-eSecN;
        for(iLevel=theGammas.GetNumberOfLevels()-1; iLevel>=0; iLevel--)
        {
          if(theGammas.GetLevelEnergy(iLevel)<eGamm) break;
        }
        G4double random = 2*G4UniformRand();
        iLevel+=G4int(random);
        if(iLevel>theGammas.GetNumberOfLevels()-1)iLevel = theGammas.GetNumberOfLevels()-1;
      }
      else
      {
        G4double eExcitation = 0;
        if(iLevel>=0) eExcitation = theGammas.GetLevel(iLevel)->GetLevelEnergy();    
        while (eKinetic-eExcitation < 0 && iLevel>0)
	{
	  iLevel--;
	  eExcitation = theGammas.GetLevel(iLevel)->GetLevelEnergy();    
	}
	
	if(getenv("InelasticCompFSLogging") && eKinetic-eExcitation < 0) 
	{
	  throw G4HadronicException(__FILE__, __LINE__, "SEVERE: InelasticCompFS: Consistency of data not good enough, please file report");
	}
	if(eKinetic-eExcitation < 0) eExcitation = 0;
	if(iLevel!= -1) aHadron.SetKineticEnergy(eKinetic - eExcitation);
	
      }
      theAngularDistribution[it]->SampleAndUpdate(aHadron);
      if(theFinalStatePhotons[it] == NULL)
      {
	thePhotons = theGammas.GetDecayGammas(iLevel);
	eGamm -= theGammas.GetLevelEnergy(iLevel);
	if(eGamm>0) // @ ok for now, but really needs an efficient way of correllated sampling @
	{
          G4ReactionProduct * theRestEnergy = new G4ReactionProduct;
          theRestEnergy->SetDefinition(G4Gamma::Gamma());
          theRestEnergy->SetKineticEnergy(eGamm);
          G4double costh = 2.*G4UniformRand()-1.;
          G4double phi = twopi*G4UniformRand();
          theRestEnergy->SetMomentum(eGamm*std::sin(std::acos(costh))*std::cos(phi), 
                                     eGamm*std::sin(std::acos(costh))*std::sin(phi),
                                     eGamm*costh);
          if(thePhotons == NULL) thePhotons = new G4ReactionProductVector;
          thePhotons->push_back(theRestEnergy);
	}
      }
    }
    else if(theEnergyAngData[it] != NULL)  
    {
      theParticles = theEnergyAngData[it]->Sample(eKinetic);
    }
    else
    {
      // @@@ what to do, if we have photon data, but no info on the hadron itself
      nothingWasKnownOnHadron = 1;
    }
    if(theFinalStatePhotons[it]!=NULL) 
    {
      // the photon distributions are in the Nucleus rest frame.
      G4ReactionProduct boosted;
      boosted.Lorentz(theNeutron, theTarget);
      G4double anEnergy = boosted.GetKineticEnergy();
      thePhotons = theFinalStatePhotons[it]->GetPhotons(anEnergy);
      G4double aBaseEnergy = theFinalStatePhotons[it]->GetLevelEnergy();
      G4double testEnergy = 0;
      if(thePhotons!=NULL && thePhotons->size()!=0) aBaseEnergy-=thePhotons->operator[](0)->GetTotalEnergy();
      if(theFinalStatePhotons[it]->NeedsCascade())
      {
	while(aBaseEnergy>0.01*keV)
        {
          // cascade down the levels
	  G4bool foundMatchingLevel = false;
          G4int closest = 2;
	  G4double deltaEold = -1;
	  for(G4int i=1; i<it; i++)
          {
            if(theFinalStatePhotons[i]!=NULL) 
            {
              testEnergy = theFinalStatePhotons[i]->GetLevelEnergy();
            }
            else
            {
              testEnergy = 0;
            }
	    G4double deltaE = std::abs(testEnergy-aBaseEnergy);
            if(deltaE<0.1*keV)
            {
              G4ReactionProductVector * theNext = 
        	theFinalStatePhotons[i]->GetPhotons(anEnergy);
              thePhotons->push_back(theNext->operator[](0));
              aBaseEnergy = testEnergy-theNext->operator[](0)->GetTotalEnergy();
              delete theNext;
	      foundMatchingLevel = true;
              break; // ===>
            }
	    if(theFinalStatePhotons[i]!=NULL && ( deltaE<deltaEold||deltaEold<0.) )
	    {
	      closest = i;
	      deltaEold = deltaE;     
	    }
          } // <=== the break goes here.
	  if(!foundMatchingLevel)
	  {
            G4ReactionProductVector * theNext = 
               theFinalStatePhotons[closest]->GetPhotons(anEnergy);
            thePhotons->push_back(theNext->operator[](0));
            aBaseEnergy = aBaseEnergy-theNext->operator[](0)->GetTotalEnergy();
            delete theNext;
	  }
        } 
      }
    }
    unsigned int i0;
    if(thePhotons!=NULL)
    {
      for(i0=0; i0<thePhotons->size(); i0++)
      {
	// back to lab
	thePhotons->operator[](i0)->Lorentz(*(thePhotons->operator[](i0)), -1.*theTarget);
      }
    }
    if(nothingWasKnownOnHadron)
    {
      G4double totalPhotonEnergy = 0;
      if(thePhotons!=NULL)
      {
        unsigned int nPhotons = thePhotons->size();
	unsigned int i0;
	for(i0=0; i0<nPhotons; i0++)
        {
          totalPhotonEnergy += thePhotons->operator[](i0)->GetTotalEnergy();
        }
      }
      availableEnergy -= totalPhotonEnergy;
      residualMass += totalPhotonEnergy/G4Neutron::Neutron()->GetPDGMass();
      aHadron.SetKineticEnergy(availableEnergy*residualMass*G4Neutron::Neutron()->GetPDGMass()/
                               (aHadron.GetMass()+residualMass*G4Neutron::Neutron()->GetPDGMass()));
      G4double CosTheta = 1.0 - 2.0*G4UniformRand();
      G4double SinTheta = std::sqrt(1.0 - CosTheta*CosTheta);
      G4double Phi = twopi*G4UniformRand();
      G4ThreeVector Vector(std::cos(Phi)*SinTheta, std::sin(Phi)*SinTheta, CosTheta);
      aHadron.SetMomentum(Vector* std::sqrt(aHadron.GetTotalEnergy()*aHadron.GetTotalEnergy()-
                                       aHadron.GetMass()*aHadron.GetMass()));
    }

// fill the result
// Beware - the recoil is not necessarily in the particles...
// Can be calculated from momentum conservation?
// The idea is that the particles ar emitted forst, and the gammas only once the
// recoil is on the residual; assumption is that gammas do not contribute to 
// the recoil.
// This needs more design @@@

    G4int nSecondaries = 2; // the hadron and the recoil
    G4bool needsSeparateRecoil = false;
    G4int totalBaryonNumber = 0;
    G4int totalCharge = 0;
    G4ThreeVector totalMomentum(0);
    if(theParticles != NULL) 
    {
      nSecondaries = theParticles->size();
      G4ParticleDefinition * aDef;
      unsigned int i0;
      for(i0=0; i0<theParticles->size(); i0++)
      {
        aDef = theParticles->operator[](i0)->GetDefinition();
	totalBaryonNumber+=aDef->GetBaryonNumber();
	totalCharge+=G4int(aDef->GetPDGCharge()+eps);
        totalMomentum += theParticles->operator[](i0)->GetMomentum();
      } 
      if(totalBaryonNumber!=G4int(theBaseA+eps+incidentParticle->GetDefinition()->GetBaryonNumber())) 
      {
        needsSeparateRecoil = true;
	nSecondaries++;
	residualA = G4int(theBaseA+eps+incidentParticle->GetDefinition()->GetBaryonNumber()
	                  -totalBaryonNumber);
	residualZ = G4int(theBaseZ+eps+incidentParticle->GetDefinition()->GetPDGCharge()
	                  -totalCharge);
      }
    }
    
    G4int nPhotons = 0;
    if(thePhotons!=NULL) nPhotons = thePhotons->size();
    nSecondaries += nPhotons;
        
    G4DynamicParticle * theSec;
    
    if( theParticles==NULL )
    {
      theSec = new G4DynamicParticle;   
      theSec->SetDefinition(aHadron.GetDefinition());
      theSec->SetMomentum(aHadron.GetMomentum());
      theResult.AddSecondary(theSec);    
 
 	aHadron.Lorentz(aHadron, theTarget);
        G4ReactionProduct theResidual;   
        theResidual.SetDefinition(G4ParticleTable::GetParticleTable()
	                          ->GetIon(static_cast<G4int>(residualZ), static_cast<G4int>(residualA), 0));  
        theResidual.SetKineticEnergy(aHadron.GetKineticEnergy()*aHadron.GetMass()/theResidual.GetMass());
        theResidual.SetMomentum(-1.*aHadron.GetMomentum());
	theResidual.Lorentz(theResidual, -1.*theTarget);
	G4ThreeVector totalPhotonMomentum(0,0,0);
	if(thePhotons!=NULL)
	{
          for(i=0; i<nPhotons; i++)
          {
            totalPhotonMomentum += thePhotons->operator[](i)->GetMomentum();
          }
	}
        theSec = new G4DynamicParticle;   
        theSec->SetDefinition(theResidual.GetDefinition());
        theSec->SetMomentum(theResidual.GetMomentum()-totalPhotonMomentum);
        theResult.AddSecondary(theSec);    
    }
    else
    {
      for(i0=0; i0<theParticles->size(); i0++)
      {
        theSec = new G4DynamicParticle; 
        theSec->SetDefinition(theParticles->operator[](i0)->GetDefinition());
        theSec->SetMomentum(theParticles->operator[](i0)->GetMomentum());
        theResult.AddSecondary(theSec); 
        delete theParticles->operator[](i0); 
      } 
      delete theParticles;
      if(needsSeparateRecoil && residualZ!=0)
      {
        G4ReactionProduct theResidual;   
        theResidual.SetDefinition(G4ParticleTable::GetParticleTable()
	                          ->GetIon(static_cast<G4int>(residualZ), static_cast<G4int>(residualA), 0));  
        G4double resiualKineticEnergy  = theResidual.GetMass()*theResidual.GetMass();
                 resiualKineticEnergy += totalMomentum*totalMomentum;
  	         resiualKineticEnergy  = std::sqrt(resiualKineticEnergy) - theResidual.GetMass();
//        cout << "Kinetic energy of the residual = "<<resiualKineticEnergy<<endl;
	theResidual.SetKineticEnergy(resiualKineticEnergy);
        theResidual.SetMomentum(-1.*totalMomentum);
        theSec = new G4DynamicParticle;   
        theSec->SetDefinition(theResidual.GetDefinition());
        theSec->SetMomentum(theResidual.GetMomentum());
        theResult.AddSecondary(theSec);  
      }  
    }
    if(thePhotons!=NULL)
    {
      for(i=0; i<nPhotons; i++)
      {
        theSec = new G4DynamicParticle;    
        theSec->SetDefinition(G4Gamma::Gamma());
        theSec->SetMomentum(thePhotons->operator[](i)->GetMomentum());
        theResult.AddSecondary(theSec); 
        delete thePhotons->operator[](i);
      }
// some garbage collection
      delete thePhotons;
    }
// clean up the primary neutron
    theResult.SetStatusChange(stopAndKill);
}

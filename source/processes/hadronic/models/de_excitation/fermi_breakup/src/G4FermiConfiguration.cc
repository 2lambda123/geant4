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
// $Id: G4FermiConfiguration.cc,v 1.1 2003-08-26 18:34:45 lara Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1998)
//
// 


#include "G4FermiConfiguration.hh"
#include "G4FermiPhaseSpaceDecay.hh"
#include <set>

// Kappa = V/V_0 it is used in calculation of Coulomb energy
// Kappa is adimensional
const G4double G4FermiConfiguration::Kappa = 1.0;

// r0 is the nuclear radius
const G4double G4FermiConfiguration::r0 = 1.3*fermi;


G4double G4FermiConfiguration::CoulombBarrier(void)
{
  //  Calculates Coulomb Barrier (MeV) for given channel with K fragments.
  const G4double Coef = (3./5.)*(elm_coupling/r0)*pow(1./(1.+Kappa), 1./3.);

  G4double SumA = 0;
  G4double SumZ = 0;
  G4double CoulombEnergy = 0.;
  for (std::vector<const G4VFermiFragment*>::iterator i = Configuration.begin(); 
       i != Configuration.end(); i++) 
    {
      G4double z = static_cast<G4double>((*i)->GetZ());
      G4double a = static_cast<G4double>((*i)->GetA());
      CoulombEnergy += (z*z) / pow(a, 1./3.);
      SumA += a;
      SumZ += z;
    }
  CoulombEnergy -= SumZ*SumZ/pow(SumA, 1./3.);
  return -Coef * CoulombEnergy;
}




G4double G4FermiConfiguration::DecayProbability(const G4int A, const G4double TotalE)
  // Decay probability  for a given channel with K fragments
{
  // A: Atomic Weight
  // TotalE: Total energy of nucleus


  G4double KineticEnergy = TotalE; // MeV
  G4double ProdMass = 1.0;
  G4double SumMass = 0.0;
  G4double S_n = 1.0;
  std::set<G4int>   combSet;
  std::multiset<G4int> combmSet;
  
  for (std::vector<const G4VFermiFragment*>::iterator i = Configuration.begin(); 
       i != Configuration.end(); i++) 
    {
      G4int a = (*i)->GetA();
      combSet.insert(a);
      combmSet.insert(a);
      G4double m = (*i)->GetFragmentMass();
      ProdMass *= m;
      SumMass += m;
      // Spin factor S_n
      S_n *= (*i)->GetPolarization();
      KineticEnergy -= m + (*i)->GetExcitationEnergy();
    }

  G4double MassFactor = ProdMass/SumMass;
  MassFactor *= sqrt(MassFactor);
  
  // Check that there is enough energy to produce K fragments
  if ((KineticEnergy -= this->CoulombBarrier()) <= 0.0) return 0.0;
  
  
  // Number of fragments
  G4int K = Configuration.size();

  // This is the constant (doesn't depend on nucleus) part
  const G4double ConstCoeff = pow(r0/hbarc,3.0)*Kappa*sqrt(2.0/pi)/3.0;
  G4double Coeff = pow(ConstCoeff*A,K-1);


  // Calculation of 1/Gamma(3(k-1)/2)
  G4double Gamma = 1.0;
  G4double arg = 3.0*(K-1)/2.0 - 1.0;
  while (arg > 1.1) 
    {
      Gamma *= arg; 
      arg--;
    }
  if ((K-1)%2 == 1) Gamma *= sqrt(pi);

  
  
  // Permutation Factor G_n
  G4double G_n = 1.0;
  for (std::set<G4int>::iterator s = combSet.begin(); s != combSet.end(); ++s)
    {
      for (G4int ni = combmSet.count(*s); ni > 1; ni--) G_n *= ni;
    }

  G4double Weight = Coeff * MassFactor * (S_n / G_n) / Gamma;
  Weight *= pow(KineticEnergy,3.0*(K-1)/2.0)/KineticEnergy;

  return Weight; 
}


G4FragmentVector * G4FermiConfiguration::GetFragments(const G4Fragment & theNucleus)
{
 
  G4FermiPhaseSpaceDecay thePhaseSpace;

  // Calculate Momenta of K fragments
  G4double M = theNucleus.GetMomentum().m();
  std::vector<G4double> m;
  m.reserve(Configuration.size());
  for (std::vector<const G4VFermiFragment*>::iterator i = Configuration.begin(); i != Configuration.end(); ++i)
    {
      m.push_back( (*i)->GetFragmentMass() );
    }
  std::vector<G4LorentzVector*>* MomentumComponents = thePhaseSpace.Decay(M,m);
  
  
  G4FragmentVector * theResult = new G4FragmentVector;
  
  G4ThreeVector boostVector = theNucleus.GetMomentum().boostVector();

  // Go back to the Lab Frame
  for (std::vector<const G4VFermiFragment*>::iterator i = Configuration.begin(); i != Configuration.end(); ++i) 
    {
      G4LorentzVector FourMomentum(*(MomentumComponents->
				     operator[](std::distance(Configuration.begin(),i))));
      
    
      // Lorentz boost
      FourMomentum.boost(boostVector);
      
      G4FragmentVector * fragment = (*i)->GetFragment(FourMomentum);
      
      for (G4FragmentVector::reverse_iterator ri = fragment->rbegin();
	   ri != fragment->rend(); ++ri)
	{
	  theResult->push_back(*ri);
	}
      delete fragment;
    }
  
  if (!MomentumComponents->empty())
    {
      std::for_each(MomentumComponents->begin(),MomentumComponents->end(),
		      DeleteFragment());
    }
  
  delete MomentumComponents;
  
  return theResult;
}



std::deque<G4LorentzVector*>* 
G4FermiConfiguration::FragmentsMomentum(G4double KineticEnergy)
{
  // Calculates momentum for K fragments (Kopylov's method of sampling is used)
  // KinetEnergy is the available kinetic energy
  
  G4int K = Configuration.size();
  
  std::deque<G4LorentzVector*>* MomentumList = new std::deque<G4LorentzVector*>(K);
  
  G4double AvalaibleMass = 0; 

  std::vector<const G4VFermiFragment*>::iterator i;
  for (i=Configuration.begin(); i != Configuration.end(); i++)
    {
      AvalaibleMass += (*i)->GetTotalEnergy(); // Mass + Excitation Energy
    }
  
  G4double PFragMagCM = 0.0;
  G4double Mass = AvalaibleMass+KineticEnergy;
  G4LorentzVector PFragCM(0.0,0.0,0.0,0.0);
  G4LorentzVector PFragLab(0.0,0.0,0.0,0.0);
  G4LorentzVector PRestCM(0.0,0.0,0.0,0.0);
  G4LorentzVector PRestLab(0.0,0.0,0.0,Mass);
  
  std::vector<const G4VFermiFragment*>::iterator l;
  for (l = Configuration.begin(); l != Configuration.begin()+K-1; l++) 
    {
      std::vector<const G4VFermiFragment*>::iterator LK = Configuration.begin()+K-1 - 
	std::distance(l,Configuration.begin());
      
      G4double FragMass = (*(LK-1))->GetFragmentMass() + (*(LK-1))->GetExcitationEnergy();;
      AvalaibleMass -= FragMass;
      
      G4int lk = std::distance(LK,Configuration.begin());
      if (lk > 2) KineticEnergy *= RNKSI(lk-1); 
      else KineticEnergy = 0.0;
      
      G4double RestMass = AvalaibleMass + KineticEnergy;
      
      PFragMagCM = sqrt(
			abs((Mass*Mass - (FragMass + RestMass)*(FragMass + RestMass))*
			    (Mass*Mass - (FragMass - RestMass)*(FragMass - RestMass)))
			)/ (2.0*Mass);
      
      
      // Create a unit vector with a random direction isotropically distributed
      G4ParticleMomentum RandVector(IsotropicVector(PFragMagCM)); 
      
      PFragCM.setVect(RandVector);
      //    PFragCM.setE((Mass*Mass + FragMass*FragMass - RestMass*RestMass)/(2.0*Mass));
      PFragCM.setE(sqrt(RandVector.mag2()+FragMass*FragMass));
      
      PRestCM.setVect(-RandVector);
      //    PRestCM.setE((Mass*Mass + RestMass*RestMass - FragMass*FragMass)/(2.0*Mass));
      PRestCM.setE(sqrt(RandVector.mag2()+RestMass*RestMass));
      
      
      G4ThreeVector BoostV = PRestLab.boostVector();
      
      PFragLab = PFragCM;
      PFragLab.boost(BoostV);
      PRestLab = PRestCM;
      PRestLab.boost(BoostV);
      
      //    MomentumList->prepend(new G4LorentzVector(PFragLab));
      MomentumList->push_front(new G4LorentzVector(PFragLab));
      
      Mass = RestMass;
    }
  
  //  MomentumList->prepend(new G4LorentzVector(PRestLab));
  MomentumList->push_front(new G4LorentzVector(PRestLab));
  return MomentumList;
}


G4double G4FermiConfiguration::RNKSI(const G4int K)
{
  G4double csim = (3.0*K-5.0)/(3.0*K-4.0);
  G4double pex = (3.0*K-5.0)/2.0;
  G4double fcsim = sqrt(1.0-csim)*pow(csim,pex);

  G4double csi = 0.0;
  G4double fcsi= 0.0;
  G4double rf = 0.0;
  do 
    {
      csi = G4UniformRand();
      fcsi = sqrt(1.0-csi)*pow(csi,pex);
      rf = fcsim*G4UniformRand();
    }
  while (rf > fcsi);
  return csi;
}
    
G4ParticleMomentum G4FermiConfiguration::IsotropicVector(const G4double Magnitude)
  // Samples a isotropic random vectorwith a magnitud given by Magnitude.
  // By default Magnitude = 1.0
{
  G4double CosTheta = 1.0 - 2.0*G4UniformRand();
  G4double SinTheta = sqrt(1.0 - CosTheta*CosTheta);
  G4double Phi = twopi*G4UniformRand();
  G4ParticleMomentum Vector(Magnitude*cos(Phi)*SinTheta,
			    Magnitude*sin(Phi)*SinTheta,
			    Magnitude*CosTheta);
  return Vector;
}

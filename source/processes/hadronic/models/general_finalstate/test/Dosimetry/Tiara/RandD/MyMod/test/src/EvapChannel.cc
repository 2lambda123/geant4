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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: EvapChannel.cc,v 1.1 2003-10-08 12:32:19 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Oct 1998)
//

#include "EvapChannel.hh"
#include "G4PairingCorrection.hh"


EvaporationChannel::EvaporationChannel(const G4int theA, const G4int theZ,
				   G4VEmissionProbability * aEmissionStrategy,
				   G4VCoulombBarrier * aCoulombBarrier):
    A(theA),
    Z(theZ),
    theEvaporationProbabilityPtr(aEmissionStrategy),
    theCoulombBarrierPtr(aCoulombBarrier),
    EmissionProbability(0.0),
    MaximalKineticEnergy(-1000.0)
{ 
    theLevelDensityPtr = new MyLDP;
    MyOwnLevelDensity = true;
}

EvaporationChannel::EvaporationChannel(const G4int theA, const G4int theZ, const G4String & aName,
					   G4VEmissionProbability * aEmissionStrategy,
					   G4VCoulombBarrier * aCoulombBarrier):
    G4VEvaporationChannel(aName),
    A(theA),
    Z(theZ),
    theEvaporationProbabilityPtr(aEmissionStrategy),
    theCoulombBarrierPtr(aCoulombBarrier),
    EmissionProbability(0.0),
    MaximalKineticEnergy(-1000.0)
{ 
    theLevelDensityPtr = new MyLDP;
    MyOwnLevelDensity = true;
}

EvaporationChannel::EvaporationChannel(const G4int theA, const G4int theZ, const G4String * aName,
					   G4VEmissionProbability * aEmissionStrategy,
					   G4VCoulombBarrier * aCoulombBarrier):
    G4VEvaporationChannel(aName),
    A(theA),
    Z(theZ),
    theEvaporationProbabilityPtr(aEmissionStrategy),
    theCoulombBarrierPtr(aCoulombBarrier),
    EmissionProbability(0.0),
    MaximalKineticEnergy(-1000.0)
{ 
    theLevelDensityPtr = new MyLDP;
    MyOwnLevelDensity = true;
}


EvaporationChannel::~EvaporationChannel()
{
    if (MyOwnLevelDensity) delete theLevelDensityPtr;
}




EvaporationChannel::EvaporationChannel(const EvaporationChannel & right)
{
    G4Exception("G4EvaporationChannel::copy_costructor meant to not be accessable");
}

const EvaporationChannel & EvaporationChannel::operator=(const EvaporationChannel & right)
{
    G4Exception("G4EvaporationChannel::operator= meant to not be accessable");
    return *this;
}

G4bool EvaporationChannel::operator==(const EvaporationChannel & right) const 
{
    return (this == (EvaporationChannel *) &right);
    //  return false;
}

G4bool EvaporationChannel::operator!=(const EvaporationChannel & right) const 
{
    return (this != (EvaporationChannel *) &right);
    //  return true;
}



void EvaporationChannel::Initialize(const G4Fragment & fragment)
{

    G4int anA = G4int(fragment.GetA());
    G4int aZ = G4int(fragment.GetZ());
    AResidual = anA - A;
    ZResidual = aZ - Z;

    // Effective excitation energy
    G4double ExEnergy = fragment.GetExcitationEnergy() -
#ifdef __USE_BIASING__
G4PairingCorrection::GetInstance()->GetPairingCorrection(anA,aZ);
#else
G4PairingCorrection::GetPairingCorrection(anA,aZ);
#endif
    // We only take into account channels which are physically allowed
    if (AResidual <= 0 || ZResidual <= 0 || AResidual < ZResidual ||
	(AResidual == ZResidual && AResidual > 1) || ExEnergy <= 0.0) {
	// 		LevelDensityParameter = 0.0;
	CoulombBarrier = 0.0;
	// 		BindingEnergy = 0.0;
	MaximalKineticEnergy = -1000.0*MeV;
	EmissionProbability = 0.0;
    } else {
	// // Get Level Density
	// LevelDensityParameter = theLevelDensityPtr->LevelDensityParameter(anA,aZ,ExEnergy);

	// Coulomb Barrier calculation
	CoulombBarrier = theCoulombBarrierPtr->GetCoulombBarrier(AResidual,ZResidual,ExEnergy);
	
	// // Binding Enegy (for separate fragment from nucleus)
	// BindingEnergy = CalcBindingEnergy(anA,aZ);

	// Maximal Kinetic Energy
	MaximalKineticEnergy = CalcMaximalKineticEnergy(G4ParticleTable::GetParticleTable()->
							GetIonTable()->GetNucleusMass(aZ,anA)+ExEnergy);
		
	// Emission probability
	if (MaximalKineticEnergy <= 0.0) EmissionProbability = 0.0;
	else { 
	    // Total emission probability for this channel
	    EmissionProbability = theEvaporationProbabilityPtr->EmissionProbability(fragment,MaximalKineticEnergy);

	}
    }
	
    return;
}


G4FragmentVector * EvaporationChannel::BreakUp(const G4Fragment & theNucleus)
{

    G4double EvaporatedKineticEnergy = CalcKineticEnergy((G4Fragment&)theNucleus);
    G4double EvaporatedMass = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(Z,A);
    G4double EvaporatedEnergy = EvaporatedKineticEnergy + EvaporatedMass;

    G4ThreeVector momentum(IsotropicVector(sqrt(EvaporatedKineticEnergy*
						(EvaporatedKineticEnergy+2.0*EvaporatedMass))));
  
    G4LorentzVector EvaporatedMomentum(momentum,EvaporatedEnergy);
    //EvaporatedMomentum.boost(theNucleus.GetMomentum().boostVector());

    G4Fragment * EvaporatedFragment = new G4Fragment(A,Z,EvaporatedMomentum);

    // ** And now the residual nucleus ** 
    G4double theExEnergy = theNucleus.GetExcitationEnergy();
    G4double theMass = G4ParticleTable::GetParticleTable()->GetIonTable()->
	GetNucleusMass(theNucleus.GetZ(),theNucleus.GetA());
    G4double ResidualEnergy = theMass + (theExEnergy - EvaporatedKineticEnergy) - EvaporatedMass;
	
    G4LorentzVector ResidualMomentum(-momentum+theNucleus.GetMomentum().vect(),ResidualEnergy);
    //ResidualMomentum.boost(theNucleus.GetMomentum().boostVector());
	
    G4Fragment * ResidualFragment = new G4Fragment( AResidual, ZResidual, ResidualMomentum );


    G4FragmentVector * theResult = new G4FragmentVector;

#ifdef debug
    G4double Efinal = ResidualMomentum.e() + EvaporatedMomentum.e();
    G4ThreeVector Pfinal = ResidualMomentum.vect() + EvaporatedMomentum.vect();
    if (abs(Efinal-theNucleus.GetMomentum().e()) > 1.0*keV) {
	G4cout << "@@@@@@@@@@@@@@@@@@@@@ G4Evaporation Chanel: ENERGY @@@@@@@@@@@@@@@@" << G4endl;
	G4cout << "Initial : " << theNucleus.GetMomentum().e()/MeV << " MeV    Final :" 
	       <<Efinal/MeV << " MeV    Delta: " <<  (Efinal-theNucleus.GetMomentum().e())/MeV 
	       << " MeV" << G4endl;
    }
    if (abs(Pfinal.x()-theNucleus.GetMomentum().x()) > 1.0*keV ||
	abs(Pfinal.y()-theNucleus.GetMomentum().y()) > 1.0*keV ||
	abs(Pfinal.z()-theNucleus.GetMomentum().z()) > 1.0*keV ) {
	G4cout << "@@@@@@@@@@@@@@@@@@@@@ G4Evaporation Chanel: MOMENTUM @@@@@@@@@@@@@@@@" << G4endl;
	G4cout << "Initial : " << theNucleus.GetMomentum().vect() << " MeV    Final :" 
	       <<Pfinal/MeV << " MeV    Delta: " <<  Pfinal-theNucleus.GetMomentum().vect()
	       << " MeV" << G4endl;   

    }
#endif
    theResult->push_back(EvaporatedFragment);
    theResult->push_back(ResidualFragment);
    return theResult; 
} 



// G4double G4EvaporationChannel::CalcBindingEnergy(const G4int anA, const G4int aZ)
// // Calculate Binding Energy for separate fragment from nucleus
// {
// 	// Mass Excess for residual nucleus
// 	G4double ResNucMassExcess = G4NucleiProperties::GetNuclearMass(AResidual,ZResidual);
// 	// Mass Excess for fragment
// 	G4double FragmentMassExcess = G4NucleiProperties::GetNuclearMass(A,Z);
// 	// Mass Excess for Compound Nucleus
// 	G4double NucleusMassExcess = G4NucleiProperties::GetNuclearMass(anA,aZ);
// 
// 	return ResNucMassExcess + FragmentMassExcess - NucleusMassExcess;
// }


G4double EvaporationChannel::CalcMaximalKineticEnergy(const G4double NucleusTotalE)
    // Calculate maximal kinetic energy that can be carried by fragment.
{
    G4double ResidualMass = G4ParticleTable::GetParticleTable()->GetIonTable()->GetNucleusMass( ZResidual, AResidual );
    G4double EvaporatedMass = G4ParticleTable::GetParticleTable()->GetIonTable()->GetNucleusMass( Z, A );
	
    G4double T = (NucleusTotalE*NucleusTotalE + EvaporatedMass*EvaporatedMass - ResidualMass*ResidualMass)/
	(2.0*NucleusTotalE) -
	EvaporatedMass - CoulombBarrier;
	
    return T;
}




G4double EvaporationChannel::CalcKineticEnergy(G4Fragment& frag)
    // Samples fragment kinetic energy.
    // It uses Dostrovsky's approximation for the inverse reaction cross
    // in the probability for fragment emisson
{
    if (MaximalKineticEnergy < 0.0) 
	G4Exception("G4EvaporationChannel::CalcKineticEnergy: maximal kinetic energy is less than 0");
    G4double res = ((MyEvapProb*)theEvaporationProbabilityPtr)->SampleEnergy(frag,MaximalKineticEnergy);
    res = MaximalKineticEnergy*res+CoulombBarrier;
    return res;
    /*
    G4double Rb = 4.0*theLevelDensityPtr->LevelDensityParameter(AResidual+A,ZResidual+Z,MaximalKineticEnergy)*
	MaximalKineticEnergy;
    G4double RbSqrt = sqrt(Rb);
    G4double PEX1 = 0.0;
    if (RbSqrt < 160.0) PEX1 = exp(-RbSqrt);
    G4double Rk = 0.0;
    G4double FRk = 0.0;
    do {
	G4double RandNumber = G4UniformRand();
	Rk = 1.0 + (1./RbSqrt)*log(RandNumber + (1.0-RandNumber)*PEX1);
	G4double Q1 = 1.0;
	G4double Q2 = 1.0;
	if (Z == 0) { // for emitted neutron
	    G4double Beta = (2.12/pow(AResidual,2./3.) - 0.05)*MeV/
		(0.76 + 2.2/pow(AResidual,1./3.));
	    Q1 = 1.0 + Beta/(MaximalKineticEnergy);
	    Q2 = Q1*sqrt(Q1);
	} 
    
	FRk = (3.0*sqrt(3.0)/2.0)/Q2 * Rk * (Q1 - Rk*Rk);
    
    } while (FRk < G4UniformRand());

    G4double result =  MaximalKineticEnergy * (1.0-Rk*Rk) + CoulombBarrier;
    return result;*/
} 
 

G4ThreeVector EvaporationChannel::IsotropicVector(const G4double Magnitude)
    // Samples a isotropic random vectorwith a magnitud given by Magnitude.
    // By default Magnitude = 1.0
{
    G4double CosTheta = 1.0 - 2.0*G4UniformRand();
    G4double SinTheta = sqrt(1.0 - CosTheta*CosTheta);
    G4double Phi = twopi*G4UniformRand();
    G4ThreeVector Vector(Magnitude*cos(Phi)*SinTheta,
			 Magnitude*sin(Phi)*SinTheta,
			 Magnitude*CosTheta);
    return Vector;
}




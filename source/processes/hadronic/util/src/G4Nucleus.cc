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
//
 // original by H.P. Wellisch
 // modified by J.L. Chuma, TRIUMF, 19-Nov-1996
 // last modified: 27-Mar-1997
 // J.P.Wellisch: 23-Apr-97: minor simplifications
 // modified by J.L.Chuma 24-Jul-97  to set the total momentum in Cinema and
 //                                  EvaporationEffects
 // modified by J.L.Chuma 21-Oct-97  put abs() around the totalE^2-mass^2
 //                                  in calculation of total momentum in
 //                                  Cinema and EvaporationEffects
 // Chr. Volcker, 10-Nov-1997: new methods and class variables.
 // HPW added utilities for low energy neutron transport. (12.04.1998)
 // M.G. Pia, 2 Oct 1998: modified GetFermiMomentum to avoid memory leaks
 
#include "G4Nucleus.hh"
#include "Randomize.hh"
 
G4ReactionProduct G4Nucleus::
GetBiasedThermalNucleus(G4double aMass, G4ThreeVector aVelocity, G4double temp) const
{
  G4double velMag = aVelocity.mag();
  G4ReactionProduct result;
  G4double value = 0;
  G4double random = 1;
  G4double norm = 3.*sqrt(k_Boltzmann*temp*aMass*G4Neutron::Neutron()->GetPDGMass());
  norm /= G4Neutron::Neutron()->GetPDGMass();
  norm *= 5.;
  norm += velMag;
  norm /= velMag;
  while(value/norm<random)
  {
     result = GetThermalNucleus(aMass, temp);
     G4ThreeVector targetVelocity = 1./result.GetMass()*result.GetMomentum();
     value = (targetVelocity+aVelocity).mag()/velMag;
     random = G4UniformRand();
  }
  return result;
}

G4ReactionProduct G4Nucleus::GetThermalNucleus(G4double targetMass, G4double temp) const
  {
    G4double currentTemp = temp;
    if(currentTemp < 0) currentTemp = theTemp;
    G4ReactionProduct theTarget;    
    theTarget.SetMass(targetMass*G4Neutron::Neutron()->GetPDGMass());
    G4double px, py, pz;
    px = GetThermalPz(theTarget.GetMass(), currentTemp);
    py = GetThermalPz(theTarget.GetMass(), currentTemp);
    pz = GetThermalPz(theTarget.GetMass(), currentTemp);
    theTarget.SetMomentum(px, py, pz);
    G4double tMom = sqrt(px*px+py*py+pz*pz);
    G4double tEtot = sqrt((tMom+theTarget.GetMass())*
                          (tMom+theTarget.GetMass())-
                          2.*tMom*theTarget.GetMass());
    if(1-tEtot/theTarget.GetMass()>0.001)
    {
      theTarget.SetTotalEnergy(tEtot);
    }
    else
    {
      theTarget.SetKineticEnergy(tMom*tMom/(2.*theTarget.GetMass()));
    }    
    return theTarget;
  }
 
 void
  G4Nucleus::ChooseParameters( const G4Material *aMaterial )
  {
    G4double random = G4UniformRand();
    G4double sum = 0;
    const G4ElementVector *theElementVector = aMaterial->GetElementVector();
    unsigned int i;
    for(i=0; i<aMaterial->GetNumberOfElements(); ++i )
    {
      sum += aMaterial->GetAtomicNumDensityVector()[i];
    }
    G4double running = 0;
    for(i=0; i<aMaterial->GetNumberOfElements(); ++i )
    {
      running += aMaterial->GetAtomicNumDensityVector()[i];
      if( running/sum > random ) {
        aEff = (*theElementVector)[i]->GetA()*mole/g;
        zEff = (*theElementVector)[i]->GetZ();
        break;
      }
    }
  }
 
 void
  G4Nucleus::SetParameters( const G4double A, const G4double Z )
  {
    G4int myZ = G4int(Z + 0.5);
    G4int myA = G4int(A + 0.5);   
    if( myA<1 || myZ<0 || myZ>myA )
      G4Exception("G4Nucleus::SetParameters called with non-physical parameters");
    aEff = A;  // atomic weight
    zEff = Z;  // atomic number
  }

 G4DynamicParticle *
  G4Nucleus::ReturnTargetParticle() const
  {
    // choose a proton or a neutron as the target particle
    
    G4DynamicParticle *targetParticle = new G4DynamicParticle;
    if( G4UniformRand() < zEff/aEff )
      targetParticle->SetDefinition( G4Proton::Proton() );
    else
      targetParticle->SetDefinition( G4Neutron::Neutron() );
    return targetParticle;
  }
 
 G4double
  G4Nucleus::AtomicMass( const G4double A, const G4double Z ) const
  {
    // derived from original FORTRAN code ATOMAS by H. Fesefeldt (2-Dec-1986)
    //
    // Computes atomic mass in MeV
    // units for A example:  A = material->GetA()/(g/mole);
    //
    // Note:  can't just use aEff and zEff since the Nuclear Reaction
    //        function needs to calculate atomic mass for various values of A and Z

    const G4double electron_mass = G4Electron::Electron()->GetPDGMass()/MeV;
    const G4double proton_mass = G4Proton::Proton()->GetPDGMass()/MeV;
    const G4double neutron_mass = G4Neutron::Neutron()->GetPDGMass()/MeV;
    const G4double deuteron_mass = G4Deuteron::Deuteron()->GetPDGMass()/MeV;
    const G4double alpha_mass = G4Alpha::Alpha()->GetPDGMass()/MeV;
    
    G4int myZ = G4int(Z + 0.5);
    G4int myA = G4int(A + 0.5);
    if( myA <= 0 )return DBL_MAX;
    if( myZ > myA)return DBL_MAX;
    if( myA == 1 )
    {
      if( myZ == 0 )return neutron_mass*MeV;
      if( myZ == 1 )return proton_mass*MeV + electron_mass*MeV;   // hydrogen
    }
    else if( myA == 2 && myZ == 1 )
    {
      return deuteron_mass*MeV;
    }
    else if( myA == 4 && myZ == 2 )
    {
      return alpha_mass*MeV;
    }
    //
    // Weitzsaecker's Mass formula
    //
    G4double mass =
      (A-Z)*neutron_mass + Z*proton_mass + Z*electron_mass
      - 15.67*A                                          // nuclear volume
      + 17.23*pow(A,2./3.)                               // surface energy
      + 93.15*pow(A/2.-Z,2.)/A                           // asymmetry
      + 0.6984523*pow(Z,2.)*pow(A,-1./3.);               // coulomb
    G4int ipp = (myA - myZ)%2;            // pairing
    G4int izz = myZ%2;
    if( ipp == izz )mass += (ipp+izz-1) * 12.0 * pow(A,-0.5);
    return mass*MeV;
  }
 
 G4double
  G4Nucleus::GetThermalPz( const G4double mass, const G4double temp ) const
  {
    G4double result = G4RandGauss::shoot();
    result *= sqrt(k_Boltzmann*temp*mass); // Das ist impuls (Pz),
                                           // nichtrelativistische rechnung
                                           // Maxwell verteilung angenommen
    return result;
  }
 
 G4double 
  G4Nucleus::EvaporationEffects( G4double kineticEnergy )
  {
    // derived from original FORTRAN code EXNU by H. Fesefeldt (10-Dec-1986)
    //
    // Nuclear evaporation as function of atomic number
    // and kinetic energy (MeV) of primary particle
    //
    // returns kinetic energy (MeV)
    //
    if( aEff < 1.5 )
    {
      pnBlackTrackEnergy = dtaBlackTrackEnergy = 0.0;
      return 0.0;
    }
    G4double ek = kineticEnergy/GeV;
    G4float ekin = G4std::min( 4.0, G4std::max( 0.1, ek ) );
    const G4float atno = G4std::min( 120., aEff ); 
    const G4float gfa = 2.0*((aEff-1.0)/70.)*exp(-(aEff-1.0)/70.);
    //
    // 0.35 value at 1 GeV
    // 0.05 value at 0.1 GeV
    //
    G4float cfa = G4std::max( 0.15, 0.35 + ((0.35-0.05)/2.3)*log(ekin) );
    G4float exnu = 7.716 * cfa * exp(-cfa)
      * ((atno-1.0)/120.)*exp(-(atno-1.0)/120.);
    G4float fpdiv = G4std::max( 0.5, 1.0-0.25*ekin*ekin );
    //
    // pnBlackTrackEnergy  is the kinetic energy (in GeV) available for
    //                     proton/neutron black track particles
    // dtaBlackTrackEnergy is the kinetic energy (in GeV) available for
    //                     deuteron/triton/alpha black track particles
    //
    pnBlackTrackEnergy = exnu*fpdiv;
    dtaBlackTrackEnergy = exnu*(1.0-fpdiv);
    
    if( G4int(zEff+0.1) != 82 )
    { 
      //G4double ran1 = G4RandGauss::shoot();
      //G4double ran2 = G4RandGauss::shoot();
      G4double ran1 = -6.0;
      G4double ran2 = -6.0;
      for( G4int i=0; i<12; ++i )
      {
        ran1 += G4UniformRand();
        ran2 += G4UniformRand();
      }
      pnBlackTrackEnergy *= 1.0 + ran1*gfa;
      dtaBlackTrackEnergy *= 1.0 + ran2*gfa;
    }
    pnBlackTrackEnergy = G4std::max( 0.0, pnBlackTrackEnergy );
    dtaBlackTrackEnergy = G4std::max( 0.0, dtaBlackTrackEnergy );
    while( pnBlackTrackEnergy+dtaBlackTrackEnergy >= ek )
    {
      pnBlackTrackEnergy *= 1.0 - 0.5*G4UniformRand();
      dtaBlackTrackEnergy *= 1.0 - 0.5*G4UniformRand();
    }
    return (pnBlackTrackEnergy+dtaBlackTrackEnergy)*GeV;
  }
 
 G4double 
  G4Nucleus::Cinema( G4double kineticEnergy )
  {
    // derived from original FORTRAN code CINEMA by H. Fesefeldt (14-Oct-1987)
    //
    // input: kineticEnergy (MeV)
    // returns modified kinetic energy (MeV)
    //
    static const G4double expxu =  82.;           // upper bound for arg. of exp
    static const G4double expxl = -expxu;         // lower bound for arg. of exp
    
    G4double ek = kineticEnergy/GeV;
    G4double ekLog = log( ek );
    G4double aLog = log( aEff );
    G4double em = G4std::min( 1.0, 0.2390 + 0.0408*aLog*aLog );
    G4double temp1 = -ek * G4std::min( 0.15, 0.0019*aLog*aLog*aLog );
    G4double temp2 = exp( G4std::max( expxl, G4std::min( expxu, -(ekLog-em)*(ekLog-em)*2.0 ) ) );
    G4double result = 0.0;
    if( abs( temp1 ) < 1.0 )
    {
      if( temp2 > 1.0e-10 )result = temp1*temp2;
    }
    else result = temp1*temp2;
    if( result < -ek )result = -ek;
    return result*GeV;
  }

 //
 // methods for class G4Nucleus  ... by Christian Volcker
 //

 G4ThreeVector G4Nucleus::GetFermiMomentum()
  {
    // chv: .. we assume zero temperature!
    
    // momentum is equally distributed in each phasespace volume dpx, dpy, dpz.
    G4double ranflat1=RandFlat::shoot((HepDouble)0.,(HepDouble)fermiMomentum);   
    G4double ranflat2=RandFlat::shoot((HepDouble)0.,(HepDouble)fermiMomentum);   
    G4double ranflat3=RandFlat::shoot((HepDouble)0.,(HepDouble)fermiMomentum);   
    G4double ranmax = (ranflat1>ranflat2? ranflat1: ranflat2);
    ranmax = (ranmax>ranflat3? ranmax : ranflat3);
    
    // - random decay angle
    G4double theta=RandFlat::shoot((HepDouble)0.,(HepDouble)pi);  // isotropic decay angle theta
    G4double phi  =RandFlat::shoot((HepDouble)0.,(HepDouble)2*pi);  // isotropic decay angle phi
    
    // - setup ThreeVector
    G4double pz=cos(theta)*ranmax;
    G4double px=sin(theta)*cos(phi)*ranmax;
    G4double py=sin(theta)*sin(phi)*ranmax;
    G4ThreeVector p(px,py,pz);
    return p;
  }
 
 G4ReactionProductVector* G4Nucleus::Fragmentate()
  {
    // needs implementation!
    return NULL;
  }
 
 void G4Nucleus::AddMomentum(const G4ThreeVector aMomentum)
  {
    momentum+=(aMomentum);
  }
 
 void G4Nucleus::AddExcitationEnergy( G4double anEnergy )
  {
    excitationEnergy+=anEnergy;
  }

 /* end of file */


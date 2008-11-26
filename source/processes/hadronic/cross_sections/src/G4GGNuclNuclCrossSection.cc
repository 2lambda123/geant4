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
// 24.11.08 V. Grichine - first implementation
//

#include "G4GGNuclNuclCrossSection.hh"

#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"



////////////////////////////////////////////////////////////////////////////////
//
//

G4GGNuclNuclCrossSection::G4GGNuclNuclCrossSection() 
: fUpperLimit( 100000 * GeV ),
  fLowerLimit( 0.1 * GeV ),
  fRadiusConst( 1.08*fermi )  // 1.1, 1.3 ?
{
  theProton   = G4Proton::Proton();
  theNeutron  = G4Neutron::Neutron();
}

///////////////////////////////////////////////////////////////////////////////////////
//
//

G4GGNuclNuclCrossSection::~G4GGNuclNuclCrossSection()
{
}


////////////////////////////////////////////////////////////////////////////////////////
//
//


G4bool 
G4GGNuclNuclCrossSection::IsApplicable(const G4DynamicParticle* aDP, 
					  const G4Element*  anElement)
{
  return IsZAApplicable(aDP, anElement->GetZ(), anElement->GetN());
} 

////////////////////////////////////////////////////////////////////////////////////////
//
//

G4bool 
G4GGNuclNuclCrossSection::IsZAApplicable(const G4DynamicParticle* aDP, 
					    G4double Z, G4double)
{
  G4bool applicable      = false;
  // G4int baryonNumber     = aDP->GetDefinition()->GetBaryonNumber();
  G4double kineticEnergy = aDP->GetKineticEnergy();

  //  const G4ParticleDefinition* theParticle = aDP->GetDefinition();
 
  if ( kineticEnergy  >= fLowerLimit && Z > 1.5 ) applicable = true;

  return applicable;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// Calculates total and inelastic Xsc, derives elastic as total - inelastic accordong to
// Glauber model with Gribov correction calculated in the dipole approximation on
// light cone. Gaussian density helps to calculate rest integrals of the model.
// [1] B.Z. Kopeliovich, nucl-th/0306044 


G4double G4GGNuclNuclCrossSection::
GetCrossSection(const G4DynamicParticle* aParticle, const G4Element* anElement, G4double T)
{
  return GetIsoZACrossSection(aParticle, anElement->GetZ(), anElement->GetN(), T);
}

////////////////////////////////////////////////////////////////////////////////////////
//
// Calculates total and inelastic Xsc, derives elastic as total - inelastic accordong to
// Glauber model with Gribov correction calculated in the dipole approximation on
// light cone. Gaussian density of point-like nucleons helps to calculate rest integrals of the model.
// [1] B.Z. Kopeliovich, nucl-th/0306044 + simplification above



G4double G4GGNuclNuclCrossSection::
GetIsoZACrossSection(const G4DynamicParticle* aParticle, G4double tZ, G4double tA, G4double)
{
  G4double xsection, sigma, cofInelastic = 2.4, cofTotal = 2.0, nucleusSquare, ratio;

  G4double pZ = aParticle->GetDefinition()->GetPDGCharge();
  G4double pA = aParticle->GetDefinition()->GetBaryonNumber();

  G4double pTkin = aParticle->GetKineticEnergy();  
  pTkin /= pA;

  G4double pN = pA - pZ;
  if( pN < 0. ) pN = 0.;

  G4double tN = tA - tZ;
  if( tN < 0. ) tN = 0.;

  G4double tR = GetNucleusRadius(tA);  
  G4double pR = GetNucleusRadius(pA); 

  sigma = (pZ*tZ+pN*tN)*GetHadronNucleonXscNS(theProton, pTkin, theProton) +
          (pZ*tN+pN*tZ)*GetHadronNucleonXscNS(theProton, pTkin, theNeutron);

  nucleusSquare = cofTotal*pi*( pR*pR + tR*tR );   // basically 2piRR

  ratio = sigma/nucleusSquare;

  xsection =  nucleusSquare*std::log( 1. + ratio );

  fTotalXsc = xsection;

  fInelasticXsc = nucleusSquare*std::log( 1. + cofInelastic*ratio )/cofInelastic;

  fElasticXsc   = fTotalXsc - fInelasticXsc;
    
  G4double difratio = ratio/(1.+ratio);

  fDiffractionXsc = 0.5*nucleusSquare*( difratio - std::log( 1. + difratio ) );

  // production to be checked !!! edit MK xsc

  sigma = (pZ*tZ+pN*tN)*GetHadronNucleonXscMK(theProton, pTkin, theProton) +
          (pZ*tN+pN*tZ)*GetHadronNucleonXscMK(theProton, pTkin, theNeutron);
 
  ratio = sigma/nucleusSquare;

  fProductionXsc = nucleusSquare*std::log( 1. + cofInelastic*ratio )/cofInelastic;

  if (fElasticXsc < 0.) fElasticXsc = 0.;

  return xsection; 
}

//////////////////////////////////////////////////////////////////////////
//
// Return single-diffraction/inelastic cross-section ratio

G4double G4GGNuclNuclCrossSection::
GetRatioSD(const G4DynamicParticle* aParticle, G4double tA, G4double tZ)
{
  G4double sigma, cofInelastic = 2.4, cofTotal = 2.0, nucleusSquare, ratio;

  G4double pZ = aParticle->GetDefinition()->GetPDGCharge();
  G4double pA = aParticle->GetDefinition()->GetBaryonNumber();

  G4double pTkin = aParticle->GetKineticEnergy();  
  pTkin /= pA;

  G4double pN = pA - pZ;
  if( pN < 0. ) pN = 0.;

  G4double tN = tA - tZ;
  if( tN < 0. ) tN = 0.;

  G4double tR = GetNucleusRadius(tA);  
  G4double pR = GetNucleusRadius(pA); 

  sigma = (pZ*tZ+pN*tN)*GetHadronNucleonXscNS(theProton, pTkin, theProton) +
          (pZ*tN+pN*tZ)*GetHadronNucleonXscNS(theProton, pTkin, theNeutron);

  nucleusSquare = cofTotal*pi*( pR*pR + tR*tR );   // basically 2piRR

  ratio = sigma/nucleusSquare;


  fInelasticXsc = nucleusSquare*std::log( 1. + cofInelastic*ratio )/cofInelastic;
   
  G4double difratio = ratio/(1.+ratio);

  fDiffractionXsc = 0.5*nucleusSquare*( difratio - std::log( 1. + difratio ) );

  if (fInelasticXsc > 0.) ratio = fDiffractionXsc/fInelasticXsc;
  else                    ratio = 0.;

  return ratio; 
}

//////////////////////////////////////////////////////////////////////////
//
// Return suasi-elastic/inelastic cross-section ratio

G4double G4GGNuclNuclCrossSection::
GetRatioQE(const G4DynamicParticle* aParticle, G4double tA, G4double tZ)
{
  G4double sigma, cofInelastic = 2.4, cofTotal = 2.0, nucleusSquare, ratio;

  G4double pZ = aParticle->GetDefinition()->GetPDGCharge();
  G4double pA = aParticle->GetDefinition()->GetBaryonNumber();

  G4double pTkin = aParticle->GetKineticEnergy();  
  pTkin /= pA;

  G4double pN = pA - pZ;
  if( pN < 0. ) pN = 0.;

  G4double tN = tA - tZ;
  if( tN < 0. ) tN = 0.;

  G4double tR = GetNucleusRadius(tA);  
  G4double pR = GetNucleusRadius(pA); 

  sigma = (pZ*tZ+pN*tN)*GetHadronNucleonXscNS(theProton, pTkin, theProton) +
          (pZ*tN+pN*tZ)*GetHadronNucleonXscNS(theProton, pTkin, theNeutron);

  nucleusSquare = cofTotal*pi*( pR*pR + tR*tR );   // basically 2piRR

  ratio = sigma/nucleusSquare;

  fInelasticXsc = nucleusSquare*std::log( 1. + cofInelastic*ratio )/cofInelastic;

  //  sigma = GetHNinelasticXsc(aParticle, tA, tZ);
  ratio = sigma/nucleusSquare;

  fProductionXsc = nucleusSquare*std::log( 1. + cofInelastic*ratio )/cofInelastic;

  if (fInelasticXsc > fProductionXsc) ratio = (fInelasticXsc-fProductionXsc)/fInelasticXsc;
  else                                ratio = 0.;
  if ( ratio < 0. )                   ratio = 0.;

  return ratio; 
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon Xsc according to differnt parametrisations:
// [2] E. Levin, hep-ph/9710546
// [3] U. Dersch, et al, hep-ex/9910052
// [4] M.J. Longo, et al, Phys.Rev.Lett. 33 (1974) 725 

G4double 
G4GGNuclNuclCrossSection::GetHadronNucleonXsc(const G4DynamicParticle* aParticle, 
                                                  const G4Element* anElement          )
{
  G4double At = anElement->GetN();  // number of nucleons 
  G4double Zt = anElement->GetZ();  // number of protons


  return GetHadronNucleonXsc( aParticle, At, Zt );
}




/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon Xsc according to differnt parametrisations:
// [2] E. Levin, hep-ph/9710546
// [3] U. Dersch, et al, hep-ex/9910052
// [4] M.J. Longo, et al, Phys.Rev.Lett. 33 (1974) 725 

G4double 
G4GGNuclNuclCrossSection::GetHadronNucleonXsc(const G4DynamicParticle* aParticle, 
                                                   G4double At,  G4double Zt       )
{
  G4double xsection = 0.;


  G4double targ_mass = G4ParticleTable::GetParticleTable()->
  GetIonTable()->GetIonMass( G4int(Zt+0.5) , G4int(At+0.5) );

  targ_mass = 0.939*GeV;  // ~mean neutron and proton ???

  G4double proj_mass     = aParticle->GetMass();
  G4double proj_momentum = aParticle->GetMomentum().mag();
  G4double sMand = CalcMandelstamS ( proj_mass , targ_mass , proj_momentum );

  sMand /= GeV*GeV;  // in GeV for parametrisation
  proj_momentum /= GeV;

  const G4ParticleDefinition* pParticle = aParticle->GetDefinition();
  

  if(pParticle == theNeutron) // as proton ??? 
  {
    xsection = At*(21.70*std::pow(sMand,0.0808) + 56.08*std::pow(sMand,-0.4525));
  } 
  else if(pParticle == theProton) 
  {
    xsection = At*(21.70*std::pow(sMand,0.0808) + 56.08*std::pow(sMand,-0.4525));
    // xsection = At*( 49.51*std::pow(sMand,-0.097) + 0.314*std::log(sMand)*std::log(sMand) );
    // xsection = At*( 38.4 + 0.85*std::abs(std::pow(log(sMand),1.47)) );
  } 
 
  xsection *= millibarn;
  return xsection;
}


/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon Xsc according to PDG parametrisation (2005):
// http://pdg.lbl.gov/2006/reviews/hadronicrpp.pdf

G4double 
G4GGNuclNuclCrossSection::GetHadronNucleonXscPDG(const G4DynamicParticle* aParticle, 
                                                  const G4Element* anElement          )
{
  G4double At = anElement->GetN();  // number of nucleons 
  G4double Zt = anElement->GetZ();  // number of protons


  return GetHadronNucleonXscPDG( aParticle, At, Zt );
}




/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon Xsc according to PDG parametrisation (2005):
// http://pdg.lbl.gov/2006/reviews/hadronicrpp.pdf
//  At = number of nucleons,  Zt = number of protons 

G4double 
G4GGNuclNuclCrossSection::GetHadronNucleonXscPDG(const G4DynamicParticle* aParticle, 
                                                     G4double At,  G4double Zt )
{
  G4double xsection = 0.;

  G4double Nt = At-Zt;              // number of neutrons
  if (Nt < 0.) Nt = 0.;  


  G4double targ_mass = G4ParticleTable::GetParticleTable()->
  GetIonTable()->GetIonMass( G4int(Zt+0.5) , G4int(At+0.5) );

  targ_mass = 0.939*GeV;  // ~mean neutron and proton ???

  G4double proj_mass     = aParticle->GetMass(); 
  G4double proj_momentum = aParticle->GetMomentum().mag();

  G4double sMand = CalcMandelstamS ( proj_mass , targ_mass , proj_momentum );

  sMand         /= GeV*GeV;  // in GeV for parametrisation

  // General PDG fit constants

  G4double s0   = 5.38*5.38; // in Gev^2
  G4double eta1 = 0.458;
  G4double eta2 = 0.458;
  G4double B    = 0.308;


  const G4ParticleDefinition* pParticle = aParticle->GetDefinition();
  

  if(pParticle == theNeutron) // proton-neutron fit 
  {
    xsection = Zt*( 35.80 + B*std::pow(std::log(sMand/s0),2.) 
                          + 40.15*std::pow(sMand,-eta1) - 30.*std::pow(sMand,-eta2));
    xsection  += Nt*( 35.45 + B*std::pow(std::log(sMand/s0),2.) 
		      + 42.53*std::pow(sMand,-eta1) - 33.34*std::pow(sMand,-eta2)); // pp for nn
  } 
  else if(pParticle == theProton) 
  {
      
      xsection  = Zt*( 35.45 + B*std::pow(std::log(sMand/s0),2.) 
                          + 42.53*std::pow(sMand,-eta1) - 33.34*std::pow(sMand,-eta2));

      xsection += Nt*( 35.80 + B*std::pow(std::log(sMand/s0),2.) 
                          + 40.15*std::pow(sMand,-eta1) - 30.*std::pow(sMand,-eta2));
  } 
  xsection *= millibarn; // parametrised in mb
  return xsection;
}




/////////////////////////////////////////////////////////////////////////////////////
//
// Returns nucleon-nucleon cross-section based on N. Starkov parametrisation of
// data from mainly http://wwwppds.ihep.su:8001/c5-6A.html database
// projectile nucleon is pParticle with pTkin shooting target nucleon tParticle

G4double 
G4GGNuclNuclCrossSection::GetHadronNucleonXscNS( G4ParticleDefinition* pParticle, 
                                                 G4double pTkin, 
                                                 G4ParticleDefinition* tParticle)
{
  G4double xsection(0), Delta, A0, B0;
  G4double hpXsc(0);
  G4double hnXsc(0);


  G4double targ_mass     = tParticle->GetPDGMass();
  G4double proj_mass     = pParticle->GetPDGMass(); 

  G4double proj_energy   = proj_mass + pTkin; 
  G4double proj_momentum = std::sqrt(pTkin*(pTkin+2*proj_mass));

  G4double sMand = CalcMandelstamS ( proj_mass , targ_mass , proj_momentum );

  sMand         /= GeV*GeV;  // in GeV for parametrisation
  proj_momentum /= GeV;
  proj_energy   /= GeV;
  proj_mass     /= GeV;

  // General PDG fit constants

  //  G4double s0   = 5.38*5.38; // in Gev^2
  //  G4double eta1 = 0.458;
  //  G4double eta2 = 0.458;
  //  G4double B    = 0.308;

  


  
  if( proj_momentum >= 10. ) // high energy: pp = nn = np
    // if( proj_momentum >= 2.)
  {
    Delta = 1.;

    if( proj_energy < 40. ) Delta = 0.916+0.0021*proj_energy;

    if( proj_momentum >= 10.)
    {
        B0 = 7.5;
        A0 = 100. - B0*std::log(3.0e7);

        xsection = A0 + B0*std::log(proj_energy) - 11
                  + 103*std::pow(2*0.93827*proj_energy + proj_mass*proj_mass+
                     0.93827*0.93827,-0.165);        //  mb
    }
  }
  else // low energy pp = nn != np
  {
      if(pParticle == tParticle) // pp or nn      // nn to be pp
      {
        if( proj_momentum < 0.73 )
        {
          hnXsc = 23 + 50*( std::pow( std::log(0.73/proj_momentum), 3.5 ) );
        }
        else if( proj_momentum < 1.05  )
        {
          hnXsc = 23 + 40*(std::log(proj_momentum/0.73))*
                         (std::log(proj_momentum/0.73));
        }
        else  // if( proj_momentum < 10.  )
        {
          hnXsc = 39.0 + 
              75*(proj_momentum - 1.2)/(std::pow(proj_momentum,3.0) + 0.15);
        }
        xsection = hnXsc;
      }
      else  // pn to be np
      {
        if( proj_momentum < 0.8 )
        {
          hpXsc = 33+30*std::pow(std::log(proj_momentum/1.3),4.0);
        }      
        else if( proj_momentum < 1.4 )
        {
          hpXsc = 33+30*std::pow(std::log(proj_momentum/0.95),2.0);
        }
        else    // if( proj_momentum < 10.  )
        {
          hpXsc = 33.3+
              20.8*(std::pow(proj_momentum,2.0)-1.35)/
                 (std::pow(proj_momentum,2.50)+0.95);
        }
        xsection = hpXsc;
      }
  }
  xsection *= millibarn; // parametrised in mb
  return xsection;
}

/*
/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon inelastic cross-section based on proper parametrisation 

G4double 
G4GGNuclNuclCrossSection::GetHNinelasticXsc(const G4DynamicParticle* aParticle, 
                                                  const G4Element* anElement          )
{
  G4double At = anElement->GetN();  // number of nucleons 
  G4double Zt = anElement->GetZ();  // number of protons


  return GetHNinelasticXsc( aParticle, At, Zt );
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon inelastic cross-section based on FTF-parametrisation 

G4double 
G4GGNuclNuclCrossSection::GetHNinelasticXsc(const G4DynamicParticle* aParticle, 
                                                     G4double At,  G4double Zt )
{
  //  G4ParticleDefinition* hadron = aParticle->GetDefinition();
  G4double sumInelastic, Nt = At - Zt;

  if(Nt < 0.) Nt = 0.;
  
  sumInelastic  = Zt*GetHadronNucleonXscMK(aParticle, theProton);
  sumInelastic += Nt*GetHadronNucleonXscMK(aParticle, theNeutron);    
 
  return sumInelastic;
}
*/

/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon inelastic cross-section based on FTF-parametrisation 

G4double 
G4GGNuclNuclCrossSection::GetHNinelasticXscVU(const G4DynamicParticle* aParticle, 
                                                     G4double At,  G4double Zt )
{
  G4int PDGcode    = aParticle->GetDefinition()->GetPDGEncoding();
  G4int absPDGcode = std::abs(PDGcode);

  G4double Elab = aParticle->GetTotalEnergy();              
                          // (s - 2*0.88*GeV*GeV)/(2*0.939*GeV)/GeV;
  G4double Plab = aParticle->GetMomentum().mag();            
                          // std::sqrt(Elab * Elab - 0.88);

  Elab /= GeV;
  Plab /= GeV;

  G4double LogPlab    = std::log( Plab );
  G4double sqrLogPlab = LogPlab * LogPlab;

  //G4cout<<"Plab = "<<Plab<<G4endl;

  G4double NumberOfTargetProtons  = Zt; 
  G4double NumberOfTargetNucleons = At;
  G4double NumberOfTargetNeutrons = NumberOfTargetNucleons - NumberOfTargetProtons;

  if(NumberOfTargetNeutrons < 0.) NumberOfTargetNeutrons = 0.;

  G4double Xtotal = 0., Xelastic = 0., Xinelastic =0.;

  if( absPDGcode > 1000 )  //------Projectile is baryon --------
  {
       G4double XtotPP = 48.0 +  0. *std::pow(Plab, 0.  ) +
                         0.522*sqrLogPlab - 4.51*LogPlab;

       G4double XtotPN = 47.3 +  0. *std::pow(Plab, 0.  ) +
                         0.513*sqrLogPlab - 4.27*LogPlab;

       G4double XelPP  = 11.9 + 26.9*std::pow(Plab,-1.21) +
                         0.169*sqrLogPlab - 1.85*LogPlab;

       G4double XelPN  = 11.9 + 26.9*std::pow(Plab,-1.21) +
                         0.169*sqrLogPlab - 1.85*LogPlab;

       Xtotal          = ( NumberOfTargetProtons  * XtotPP +
                           NumberOfTargetNeutrons * XtotPN  );

       Xelastic        = ( NumberOfTargetProtons  * XelPP  +
                           NumberOfTargetNeutrons * XelPN   );
  }
  Xinelastic = Xtotal - Xelastic;

  if(Xinelastic < 0.) Xinelastic = 0.;

  return Xinelastic*= millibarn;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Returns hadron-nucleon cross-section based on Mikhail Kossov CHIPS parametrisation of
// data from G4QuasiFreeRatios class

G4double 
G4GGNuclNuclCrossSection::GetHadronNucleonXscMK(G4ParticleDefinition* pParticle, G4double pTkin,
                                                G4ParticleDefinition* nucleon  )
{
  G4int I = -1;
  G4int PDG = pParticle->GetPDGEncoding();
  G4double totalXsc = 0;
  G4double elasticXsc = 0;
  G4double inelasticXsc;
  // G4int absPDG = std::abs(PDG);

  G4double pM = pParticle->GetPDGMass();
  G4double p  = std::sqrt(pTkin*(pTkin+2*pM))/GeV;

  G4bool F = false;            
  if(nucleon == theProton)       F = true;
  else if(nucleon == theNeutron) F = false;
  else
  {
    G4cout << "nucleon is not proton or neutron, return xsc for proton" << G4endl;
    F = true;
  }

  G4bool kfl = true;                             // Flag of K0/aK0 oscillation
  G4bool kf  = false;

  if( PDG == 130 || PDG == 310 )
  {
    kf = true;
    if( G4UniformRand() > .5 ) kfl = false;
  }
  if     ( (PDG == 2212 && F) || (PDG == 2112 && !F) ) I = 0; // pp/nn
  else if( (PDG == 2112 && F) || (PDG == 2212 && !F) ) I = 1; // np/pn
  else
  {
    G4cout<<"MK PDG = "<<PDG
          <<", while it is defined only for p,n,hyperons,anti-baryons,pi,K/antiK"<<G4endl;
    G4Exception("G4QuasiFreeRatio::FetchElTot:","22",FatalException,"CHIPScrash");
  }

  // Each parameter set can have not more than nPoints = 128 parameters

  static const G4double lmi = 3.5;       // min of (lnP-lmi)^2 parabola
  static const G4double pbe = .0557;     // elastic (lnP-lmi)^2 parabola coefficient
  static const G4double pbt = .3;        // total (lnP-lmi)^2 parabola coefficient
  static const G4double pmi = .1;        // Below that fast LE calculation is made
  static const G4double pma = 1000.;     // Above that fast HE calculation is made
                  
  if( p <= 0.)
  {
    G4cout<<" p = "<<p<<" is zero or negative"<<G4endl;

    elasticXsc   = 0.;
    inelasticXsc = 0.;
    totalXsc     = 0.;

    return totalXsc;
  }
  if (!I)                          // pp/nn
  {
    if( p < pmi )
    {
      G4double p2 = p*p;
      elasticXsc          = 1./(.00012 + p2*.2);
      totalXsc          = elasticXsc;
    }
    else if(p>pma)
    {
      G4double lp  = std::log(p)-lmi;
      G4double lp2 = lp*lp;
      elasticXsc  = pbe*lp2 + 6.72;
      totalXsc    = pbt*lp2 + 38.2;
    }
    else
    {
      G4double p2  = p*p;
      G4double LE  = 1./( .00012 + p2*.2);
      G4double lp  = std::log(p) - lmi;
      G4double lp2 = lp*lp;
      G4double rp2 = 1./p2;
      elasticXsc  = LE + ( pbe*lp2 + 6.72+32.6/p)/( 1. + rp2/p);
      totalXsc    = LE + ( pbt*lp2 + 38.2+52.7*rp2)/( 1. + 2.72*rp2*rp2);
    }
  }
  else if( I==1 )                        // np/pn
  {
    if( p < pmi )
    {
      G4double p2 = p*p;
      elasticXsc = 1./( .00012 + p2*( .051 + .1*p2));
      totalXsc   = elasticXsc;
    }
    else if( p > pma )
    {
      G4double lp  = std::log(p) - lmi;
      G4double lp2 = lp*lp;
      elasticXsc  = pbe*lp2 + 6.72;
      totalXsc    = pbt*lp2 + 38.2;
    }
    else
    {
      G4double p2  = p*p;
      G4double LE  = 1./( .00012 + p2*( .051 + .1*p2 ) );
      G4double lp  = std::log(p) - lmi;
      G4double lp2 = lp*lp;
      G4double rp2 = 1./p2;
      elasticXsc  = LE + (pbe*lp2 + 6.72 + 30./p)/( 1. + .49*rp2/p);
      totalXsc    = LE + (pbt*lp2 + 38.2)/( 1. + .54*rp2*rp2);
    }
  }
  else
  {
    G4cout<<"PDG incoding = "<<I<<" is not defined (0-1)"<<G4endl;
  
  }
  if( elasticXsc > totalXsc ) elasticXsc = totalXsc;

  totalXsc   *= millibarn;
  elasticXsc *= millibarn;
  inelasticXsc   = totalXsc - elasticXsc;
  if (inelasticXsc < 0.) inelasticXsc = 0.;

  return inelasticXsc;
}

////////////////////////////////////////////////////////////////////////////////////
//
//

G4double 
G4GGNuclNuclCrossSection::GetNucleusRadius( const G4DynamicParticle* , 
                                               const G4Element* anElement)
{
  G4double At       = anElement->GetN();
  G4double oneThird = 1.0/3.0;
  G4double cubicrAt = std::pow (At, oneThird); 


  G4double R;  // = fRadiusConst*cubicrAt;
  /*  
  G4double tmp = std::pow( cubicrAt-1., 3.);
  tmp         += At;
  tmp         *= 0.5;

  if (At > 20.)   // 20.
  {
    R = fRadiusConst*std::pow (tmp, oneThird); 
  }
  else
  {
    R = fRadiusConst*cubicrAt; 
  }
  */
  
  R = fRadiusConst*cubicrAt;

  // return R;  // !!!!


  
  G4double meanA  = 21.;

  G4double tauA1  = 40.; 
  G4double tauA2  = 10.; 
  G4double tauA3  = 5.; 

  G4double a1 = 0.85;
  G4double b1 = 1. - a1;

  G4double b2 = 0.3;
  G4double b3 = 4.;

  if (At > 20.)   // 20.
  {
    R *= ( a1 + b1*std::exp( -(At - meanA)/tauA1) ); 
  }
  else if (At > 3.5)
  {
    R *= ( 1.0 + b2*( 1. - std::exp( (At - meanA)/tauA2) ) ); 
  }
  else 
  {
    R *= ( 1.0 + b3*( 1. - std::exp( (At - meanA)/tauA3) ) ); 
  }  
  return R;
 
}

////////////////////////////////////////////////////////////////////////////////////
//
//

G4double 
G4GGNuclNuclCrossSection::GetNucleusRadius(G4double At)
{
  G4double R;

  // R = GetNucleusRadiusGG(At);

  R = GetNucleusRadiusDE(At);

  return R;
}

///////////////////////////////////////////////////////////////////

G4double 
G4GGNuclNuclCrossSection::GetNucleusRadiusGG(G4double At)
{
  
  G4double oneThird = 1.0/3.0;
  G4double cubicrAt = std::pow (At, oneThird); 


  G4double R;  // = fRadiusConst*cubicrAt;
  
  /*
  G4double tmp = std::pow( cubicrAt-1., 3.);
  tmp         += At;
  tmp         *= 0.5;

  if (At > 20.)
  {
    R = fRadiusConst*std::pow (tmp, oneThird); 
  }
  else
  {
    R = fRadiusConst*cubicrAt; 
  }
  */
  
  R = fRadiusConst*cubicrAt;

  G4double meanA = 20.;
  G4double tauA  = 20.; 

  if ( At > 20.)   // 20.
  {
    R *= ( 0.8 + 0.2*std::exp( -(At - meanA)/tauA) ); 
  }
  else
  {
    R *= ( 1.0 + 0.1*( 1. - std::exp( (At - meanA)/tauA) ) ); 
  }

  return R;

}


G4double 
G4GGNuclNuclCrossSection::GetNucleusRadiusDE(G4double A)
{

  // algorithm from diffuse-elastic

  G4double R, r0, a1, a2, a3;

  a1 = 1.26;  // 1.08, 1.16
  a2 = 1.1;
  a3 = 1.7;


  if( A < 50. )
  {
    if( A > 10. ) r0  = a1*( 1 - std::pow(A, -2./3.) )*fermi;   // 1.08*fermi;
    else          r0  = a2*fermi;

    R = r0*std::pow( A, 1./3. );
  }
  else
  {
    r0 = a3*fermi;

    R  = r0*std::pow(A, 0.27);
  }
  return R;



}






////////////////////////////////////////////////////////////////////////////////////
//
//

G4double G4GGNuclNuclCrossSection::CalculateEcmValue( const G4double mp , 
                                                         const G4double mt , 
                                                         const G4double Plab )
{
  G4double Elab = std::sqrt ( mp * mp + Plab * Plab );
  G4double Ecm  = std::sqrt ( mp * mp + mt * mt + 2 * Elab * mt );
  // G4double Pcm  = Plab * mt / Ecm;
  // G4double KEcm = std::sqrt ( Pcm * Pcm + mp * mp ) - mp;

  return Ecm ; // KEcm;
}


////////////////////////////////////////////////////////////////////////////////////
//
//

G4double G4GGNuclNuclCrossSection::CalcMandelstamS( const G4double mp , 
                                                       const G4double mt , 
                                                       const G4double Plab )
{
  G4double Elab = std::sqrt ( mp * mp + Plab * Plab );
  G4double sMand  = mp*mp + mt*mt + 2*Elab*mt ;

  return sMand;
}


//
//
///////////////////////////////////////////////////////////////////////////////////////

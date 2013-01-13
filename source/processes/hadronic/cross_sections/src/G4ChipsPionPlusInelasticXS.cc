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
// The lust update: M.V. Kossov, CERN/ITEP(Moscow) 17-June-02
//
//
// G4 Physics class: G4ChipsPionPlusInelasticXS for gamma+A cross sections
// Created: M.V. Kossov, CERN/ITEP(Moscow), 20-Dec-03
// The last update: M.V. Kossov, CERN/ITEP (Moscow) 15-Feb-04
//
// -------------------------------------------------------------------------------------
// Short description: Cross-sections extracted (by W.Pokorski) from the CHIPS package for 
// pion interactions. Original author: M. Kossov
// -------------------------------------------------------------------------------------
//

#include "G4ChipsPionPlusInelasticXS.hh"
#include "G4SystemOfUnits.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4PionPlus.hh"

// factory
#include "G4CrossSectionFactory.hh"
//
G4_DECLARE_XS_FACTORY(G4ChipsPionPlusInelasticXS);

G4ChipsPionPlusInelasticXS::G4ChipsPionPlusInelasticXS():G4VCrossSectionDataSet(Default_Name())
{
  // Initialization of the
  lastLEN=0; // Pointer to lastArray of LowEn CS
  lastHEN=0; // Pointer to lastArray of HighEn CS
  lastN=0;   // The last N of calculated nucleus
  lastZ=0;   // The last Z of calculated nucleus
  lastP=0.;  // Last used in cross section Momentum
  lastTH=0.; // Last threshold momentum
  lastCS=0.; // Last value of the Cross Section
  lastI=0;   // The last position in the DAMDB
  LEN = new std::vector<G4double*>;
  HEN = new std::vector<G4double*>;
}


G4ChipsPionPlusInelasticXS::~G4ChipsPionPlusInelasticXS()
{
  G4int lens=LEN->size();
  for(G4int i=0; i<lens; ++i) delete[] (*LEN)[i];
  delete LEN;
  G4int hens=HEN->size();
  for(G4int i=0; i<hens; ++i) delete[] (*HEN)[i];
  delete HEN;
}

G4bool G4ChipsPionPlusInelasticXS::IsIsoApplicable(const G4DynamicParticle* Pt, G4int, G4int,    
				 const G4Element*,
				 const G4Material*)
{
  G4ParticleDefinition* particle = Pt->GetDefinition();
  if (particle == G4PionPlus::PionPlus()      ) return true;
  return false;
}

// The main member function giving the collision cross section (P is in IU, CS is in mb)
// Make pMom in independent units ! (Now it is MeV)
G4double G4ChipsPionPlusInelasticXS::GetIsoCrossSection(const G4DynamicParticle* Pt, G4int tgZ, G4int A,  
							const G4Isotope*,
							const G4Element*,
							const G4Material*)
{
  G4double pMom=Pt->GetTotalMomentum();
  G4int tgN = A - tgZ;
  
  return GetChipsCrossSection(pMom, tgZ, tgN, 211);
}


G4double G4ChipsPionPlusInelasticXS::GetChipsCrossSection(G4double pMom, G4int tgZ, G4int tgN, G4int)
{
  static __thread G4int j;                      // A#0f Z/N-records already tested in AMDB
  static __thread std::vector <G4int>    *colN_G4MT_TLS_ = 0 ; if (!colN_G4MT_TLS_) colN_G4MT_TLS_ = new  std::vector <G4int>     ;  std::vector <G4int>    &colN = *colN_G4MT_TLS_;  // Vector of N for calculated nuclei (isotops)
  static __thread std::vector <G4int>    *colZ_G4MT_TLS_ = 0 ; if (!colZ_G4MT_TLS_) colZ_G4MT_TLS_ = new  std::vector <G4int>     ;  std::vector <G4int>    &colZ = *colZ_G4MT_TLS_;  // Vector of Z for calculated nuclei (isotops)
  static __thread std::vector <G4double> *colP_G4MT_TLS_ = 0 ; if (!colP_G4MT_TLS_) colP_G4MT_TLS_ = new  std::vector <G4double>  ;  std::vector <G4double> &colP = *colP_G4MT_TLS_;  // Vector of last momenta for the reaction
  static __thread std::vector <G4double> *colTH_G4MT_TLS_ = 0 ; if (!colTH_G4MT_TLS_) colTH_G4MT_TLS_ = new  std::vector <G4double>  ;  std::vector <G4double> &colTH = *colTH_G4MT_TLS_; // Vector of energy thresholds for the reaction
  static __thread std::vector <G4double> *colCS_G4MT_TLS_ = 0 ; if (!colCS_G4MT_TLS_) colCS_G4MT_TLS_ = new  std::vector <G4double>  ;  std::vector <G4double> &colCS = *colCS_G4MT_TLS_; // Vector of last cross sections for the reaction
  // ***---*** End of the mandatory Static Definitions of the Associative Memory ***---***

  G4bool in=false;                     // By default the isotope must be found in the AMDB
  if(tgN!=lastN || tgZ!=lastZ)         // The nucleus was not the last used isotope
  {
    in = false;                        // By default the isotope haven't be found in AMDB  
    lastP   = 0.;                      // New momentum history (nothing to compare with)
    lastN   = tgN;                     // The last N of the calculated nucleus
    lastZ   = tgZ;                     // The last Z of the calculated nucleus
    lastI   = colN.size();             // Size of the Associative Memory DB in the heap
    j  = 0;                            // A#0f records found in DB for this projectile
    if(lastI) for(G4int i=0; i<lastI; i++) // AMDB exists, try to find the (Z,N) isotope
    {
      if(colN[i]==tgN && colZ[i]==tgZ) // Try the record "i" in the AMDB
      {
        lastI=i;                       // Remember the index for future fast/last use
        lastTH =colTH[i];              // The last THreshold (A-dependent)
        if(pMom<=lastTH)
        {
          return 0.;                   // Energy is below the Threshold value
        }
        lastP  =colP [i];              // Last Momentum  (A-dependent)
        lastCS =colCS[i];              // Last CrossSect (A-dependent)
        in = true;                     // This is the case when the isotop is found in DB
        // Momentum pMom is in IU ! @@ Units
        lastCS=CalculateCrossSection(-1,j,211,lastZ,lastN,pMom); // read & update
        if(lastCS<=0. && pMom>lastTH)  // Correct the threshold (@@ No intermediate Zeros)
        {
          lastCS=0.;
          lastTH=pMom;
        }
        break;                         // Go out of the LOOP
      }
      j++;                             // Increment a#0f records found in DB
    }
    if(!in)                            // This isotope has not been calculated previously
    {
      //!!The slave functions must provide cross-sections in millibarns (mb) !! (not in IU)
      lastCS=CalculateCrossSection(0,j,211,lastZ,lastN,pMom); //calculate & create
      //if(lastCS>0.)                   // It means that the AMBD was initialized
      //{

      lastTH = 0; //ThresholdEnergy(tgZ, tgN); // The Threshold Energy which is now the last
        colN.push_back(tgN);
        colZ.push_back(tgZ);
        colP.push_back(pMom);
        colTH.push_back(lastTH);
        colCS.push_back(lastCS);
      //} // M.K. Presence of H1 with high threshold breaks the syncronization
      return lastCS*millibarn;
    } // End of creation of the new set of parameters
    else
    {
      colP[lastI]=pMom;
      colCS[lastI]=lastCS;
    }
  } // End of parameters udate
  else if(pMom<=lastTH)
  {
    return 0.;                         // Momentum is below the Threshold Value -> CS=0
  }
  else                                 // It is the last used -> use the current tables
  {
    lastCS=CalculateCrossSection(1,j,211,lastZ,lastN,pMom); // Only read and UpdateDB
    lastP=pMom;
  }
  return lastCS*millibarn;
}

// The main member function giving the gamma-A cross section (E in GeV, CS in mb)
G4double G4ChipsPionPlusInelasticXS::CalculateCrossSection(G4int F, G4int I,
                                        G4int, G4int targZ, G4int targN, G4double Momentum)
{
  static const G4double THmin=27.;     // default minimum Momentum (MeV/c) Threshold
  static const G4double THmiG=THmin*.001; // minimum Momentum (GeV/c) Threshold
  static const G4double dP=10.;        // step for the LEN (Low ENergy) table MeV/c
  static const G4double dPG=dP*.001;   // step for the LEN (Low ENergy) table GeV/c
  static const G4int    nL=105;        // A#of LEN points in E (step 10 MeV/c)
  static const G4double Pmin=THmin+(nL-1)*dP; // minP for the HighE part with safety
  static const G4double Pmax=227000.;  // maxP for the HEN (High ENergy) part 227 GeV
  static const G4int    nH=224;        // A#of HEN points in lnE
  static const G4double milP=std::log(Pmin);// Low logarithm energy for the HEN part
  static const G4double malP=std::log(Pmax);// High logarithm energy (each 2.75 percent)
  static const G4double dlP=(malP-milP)/(nH-1); // Step in log energy in the HEN part
  static const G4double milPG=std::log(.001*Pmin);// Low logarithmEnergy for HEN part GeV/c
  G4double sigma=0.;
  if(F&&I) sigma=0.;                   // @@ *!* Fake line *!* to use F & I !!!Temporary!!!
  //G4double A=targN+targZ;              // A of the target
  if(F<=0)                             // This isotope was not the last used isotop
  {
    if(F<0)                            // This isotope was found in DAMDB =-----=> RETRIEVE
    {
      G4int sync=LEN->size();
      if(sync<=I) G4cerr<<"*!*G4ChipsPiMinusNuclCS::CalcCrosSect:Sync="<<sync<<"<="<<I<<G4endl;
      lastLEN=(*LEN)[I];               // Pointer to prepared LowEnergy cross sections
      lastHEN=(*HEN)[I];               // Pointer to prepared High Energy cross sections
    }
    else                               // This isotope wasn't calculated before => CREATE
    {
      lastLEN = new G4double[nL];      // Allocate memory for the new LEN cross sections
      lastHEN = new G4double[nH];      // Allocate memory for the new HEN cross sections
      // --- Instead of making a separate function ---
      G4double P=THmiG;                // Table threshold in GeV/c
      for(G4int k=0; k<nL; k++)
      {
        lastLEN[k] = CrossSectionLin(targZ, targN, P);
        P+=dPG;
      }
      G4double lP=milPG;
      for(G4int n=0; n<nH; n++)
      {
        lastHEN[n] = CrossSectionLog(targZ, targN, lP);
        lP+=dlP;
      }
      // --- End of possible separate function
      // *** The synchronization check ***
      G4int sync=LEN->size();
      if(sync!=I)
      {
        G4cerr<<"***G4ChipsPiMinusNuclCS::CalcCrossSect: Sinc="<<sync<<"#"<<I<<", Z=" <<targZ
              <<", N="<<targN<<", F="<<F<<G4endl;
        //G4Exception("G4PiMinusNuclearCS::CalculateCS:","39",FatalException,"DBoverflow");
      }
      LEN->push_back(lastLEN);         // remember the Low Energy Table
      HEN->push_back(lastHEN);         // remember the High Energy Table
    } // End of creation of the new set of parameters
  } // End of parameters udate
  // =-----------------= NOW the Magic Formula =-------------------------=
  if (Momentum<lastTH) return 0.;      // It must be already checked in the interface class
  else if (Momentum<Pmin)              // High Energy region
  {
    sigma=EquLinearFit(Momentum,nL,THmin,dP,lastLEN);
  }
  else if (Momentum<Pmax)              // High Energy region
  {
    G4double lP=std::log(Momentum);
    sigma=EquLinearFit(lP,nH,milP,dlP,lastHEN);
  }
  else                                 // UHE region (calculation, not frequent)
  {
    G4double P=0.001*Momentum;         // Approximation formula is for P in GeV/c
    sigma=CrossSectionFormula(targZ, targN, P, std::log(P));
  }
  if(sigma<0.) return 0.;
  return sigma;
}

// Electromagnetic momentum-threshold (in MeV/c) 
G4double G4ChipsPionPlusInelasticXS::ThresholdMomentum(G4int tZ, G4int tN)
{
  static const G4double third=1./3.;
  static const G4double pM = G4PionPlus::PionPlus()->Definition()->GetPDGMass(); // Projectile mass in MeV
  static const G4double tpM= pM+pM;       // Doubled projectile mass (MeV)
  G4double tA=tZ+tN;
  if(tZ<.99 || tN<0.) return 0.;
  else if(tZ==1 && tN==0) return 300.;    // A threshold on the free proton
  //G4double dE=1.263*tZ/(1.+std::pow(tA,third));
  G4double dE=tZ/(1.+std::pow(tA,third)); // Safety for diffused edge of the nucleus (QE)
  G4double tM=931.5*tA;
  G4double T=dE+dE*(dE/2+pM)/tM;
  return std::sqrt(T*(tpM+T));
}

// Calculation formula for piMinus-nuclear inelastic cross-section (mb) (P in GeV/c)
G4double G4ChipsPionPlusInelasticXS::CrossSectionLin(G4int tZ, G4int tN, G4double P)
{
  G4double lP=std::log(P);
  return CrossSectionFormula(tZ, tN, P, lP);
}

// Calculation formula for piMinus-nuclear inelastic cross-section (mb) log(P in GeV/c)
G4double G4ChipsPionPlusInelasticXS::CrossSectionLog(G4int tZ, G4int tN, G4double lP)
{
  G4double P=std::exp(lP);
  return CrossSectionFormula(tZ, tN, P, lP);
}
// Calculation formula for piMinus-nuclear inelastic cross-section (mb) log(P in GeV/c)
G4double G4ChipsPionPlusInelasticXS::CrossSectionFormula(G4int tZ, G4int tN,
                                                              G4double P, G4double lP)
{
  G4double sigma=0.;
  if(tZ==1 && !tN)                        // PiPlus-Proton interaction from G4QuasiElRatios
  {
    G4double ld=lP-3.5;
    G4double ld2=ld*ld;
    G4double p2=P*P;
    G4double p4=p2*p2;
    G4double sp=std::sqrt(P);
    G4double lm=lP-.32;
    G4double md=lm*lm+.04;
    G4double El=(.0557*ld2+2.4+6./sp)/(1.+3./p4);
    G4double To=(.3*ld2+22.3+5./sp)/(1.+1./p4);
    sigma=(To-El)+.1/md;
  }
  else if(tZ==1 && tN==1)                 // pimp_tot
  {
    G4double p2=P*P;
    G4double d=lP-2.7;
    G4double f=lP+1.25;
    G4double gg=lP-.017;
    sigma=(.55*d*d+38.+23./std::sqrt(P))/(1.+.3/p2/p2)+18./(f*f+.1089)+.02/(gg*gg+.0025);
  }
  else if(tZ<97 && tN<152)                // General solution
  {
    G4double d=lP-4.2;
    G4double p2=P*P;
    G4double p4=p2*p2;
    G4double a=tN+tZ;                     // A of the target
    G4double al=std::log(a);
    G4double sa=std::sqrt(a);
    G4double ssa=std::sqrt(sa);
    G4double a2=a*a;
    G4double c=41.*std::exp(al*.68)*(1.+44./a2)/(1.+8./a)/(1.+200./a2/a2);
    G4double f=290.*ssa/(1.+34./a/ssa);
    G4double gg=-1.32-al*.043;
    G4double u=lP-gg;
    G4double h=al*(.4-.055*al);
    G4double r=.01+a2*5.E-8;
    sigma=(c+d*d)/(1.+(.2-.009*sa)/p4)+f/(u*u+h*h)/(1.+r/p2);
  }
  else
  {
    G4cerr<<"-Warning-G4ChipsPiPlusNuclearCroSect::CSForm:*Bad A* Z="<<tZ<<", N="<<tN<<G4endl;
    sigma=0.;
  }
  if(sigma<0.) return 0.;
  return sigma;  
}

G4double G4ChipsPionPlusInelasticXS::EquLinearFit(G4double X, G4int N, G4double X0, G4double DX, G4double* Y)
{
  if(DX<=0. || N<2)
    {
      G4cerr<<"***G4ChipsPionPlusInelasticXS::EquLinearFit: DX="<<DX<<", N="<<N<<G4endl;
      return Y[0];
    }
  
  G4int    N2=N-2;
  G4double d=(X-X0)/DX;
  G4int         j=static_cast<int>(d);
  if     (j<0)  j=0;
  else if(j>N2) j=N2;
  d-=j; // excess
  G4double yi=Y[j];
  G4double sigma=yi+(Y[j+1]-yi)*d;
  
  return sigma;
}

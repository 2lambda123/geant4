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
// $Id: G4QNucleus.cc,v 1.72 2007-05-07 07:47:54 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QNucleus ----------------
//             by Mikhail Kossov, Sept 1999.
//      class for Nuclei/Nuclear Environment used by CHIPS Model
// ------------------------------------------------------------------

//#define debug
//#define cldebug
//#define pdebug 
//#define pardeb
//#define ppdebug

#include "G4QNucleus.hh"
#include "Randomize.hh"
#include <algorithm>
#include <cmath>
#include <vector>
//#include <cstdlib>
using namespace std;

// Static parameters definition
G4double G4QNucleus::freeNuc=0.1;       // probability to find quasiFreeBaryon on Surface
G4double G4QNucleus::freeDib=.05;       // probability to find quasiFreeDiBaryon on Surface
G4double G4QNucleus::clustProb=4.;      // clusterization probability in dense region
G4double G4QNucleus::mediRatio=1.;      // relative vacuum hadronization probability
G4double G4QNucleus::nucleonDistance=.8*fermi; // Distance between nucleons (0.8 fm)

G4QNucleus::G4QNucleus() : G4QHadron(),Z(0),N(0),S(0),maxClust(0),currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
  probVect[0]=mediRatio;
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(1) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(G4int z, G4int n, G4int s) :
  G4QHadron(90000000+s*1000000+z*1000+n),Z(z),N(n),S(s),maxClust(0),currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
  probVect[0]=mediRatio;
  for(G4int i=1; i<256; i++) probVect[i] = 0.;
#ifdef debug
  G4cout<<"G4QNucleus::Construction By Z="<<z<<",N="<<n<<",S="<<s<<G4endl;
#endif
  SetZNSQC(z,n,s);
  G4QPDGCode nQPDG(90000000+S*1000000+Z*1000+N); // Not necessary (? look above)
#ifdef debug
  G4cout<<"G4QNucleus::ConstructionByZNS: nQPDG="<<nQPDG<<G4endl;
#endif
  G4double mass=nQPDG.GetNuclMass(Z,N,S);
#ifdef debug
  G4cout<<"G4QNucleus::ConstructionByZNS: mass="<<mass<<G4endl;
#endif
  SetQPDG(nQPDG);                                // Not necessary (? look above)
#ifdef debug
  G4cout<<"G4QNucleus::ConstructionByZNS: nQPDG set"<<G4endl;
#endif
  G4LorentzVector p(0.,0.,0.,mass);
  Set4Momentum(p);
  SetNFragments(0);
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(2) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(G4int nucPDG): G4QHadron(nucPDG), maxClust(0),currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
  InitByPDG(nucPDG);
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(3) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(G4LorentzVector p, G4int nucPDG) :
  G4QHadron(nucPDG,p),maxClust(0),currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
  InitByPDG(nucPDG);
  Set4Momentum(p);
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(4) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(G4int z, G4int n, G4int s, G4LorentzVector p) :
  G4QHadron(90000000+s*1000000+z*1000+n,p),Z(z),N(n),S(s),maxClust(0),currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
  probVect[0]=mediRatio;
  for(G4int i=1; i<256; i++) probVect[i] = 0.;
  Set4Momentum(p);
  SetNFragments(0);
  G4int ZNS=Z+N+S;
  G4QPDGCode nPDG(90000000+S*1000000+Z*1000+N);
  SetQPDG(nPDG);
  G4QContent nQC(N+ZNS,Z+ZNS,S,0,0,0);
  SetZNSQC(z,n,s);
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(5) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(G4QContent nucQC): G4QHadron(nucQC), maxClust(0), currentNucleon(-1)
{
  static const G4double mPi0 = G4QPDGCode(111).GetMass();
  Tb = new std::vector<G4double>;
#ifdef debug
  G4cout<<"G4QNucleus::Construction By QC="<<nucQC<<G4endl;
#endif
  probVect[0]=mediRatio;
  for(G4int i=1; i<256; i++) probVect[i] = 0.;
  G4int u=nucQC.GetU()-nucQC.GetAU();
  G4int d=nucQC.GetD()-nucQC.GetAD();
  S = nucQC.GetS()-nucQC.GetAS();     // a#of LAMBDA's in the nucleus
  G4int du= d-u;                      // isotopic shift
  G4int b =(d+u+S)/3;                 // baryon number
  Z = (b-S-du)/2;                     // protons
  N = Z+du;                           // neutrons
  SetQC(nucQC);
#ifdef debug
  G4cout<<"G4QNucleus::ConstructionByQC: N="<<N<<",Z="<<Z<<",S="<<S<<G4endl;
#endif
  G4int nucPDG=90000000+S*1000000+Z*1000+N;
  G4QPDGCode nQPDG(nucPDG);
#ifdef debug
  G4cout<<"G4QNucleus::ConstructionByQC: nQPDG="<<nQPDG<<G4endl;
#endif
  G4double mass=nQPDG.GetNuclMass(Z,N,S);
  if(nucPDG==90000000)
  {
    if(nucQC.GetTot()) mass=mPi0;
    else               mass=0.;
  }
#ifdef debug
  G4cout<<"G4QNucleus::ConstructionByQC: mass="<<mass<<G4endl;
#endif
  SetQPDG(nQPDG);
#ifdef debug
  G4cout<<"G4QNucleus::ConstructionByQC: nQPDG set"<<G4endl;
#endif
  G4LorentzVector p(0.,0.,0.,mass);
  Set4Momentum(p);
  SetNFragments(0);
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(6) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(G4QContent nucQC, G4LorentzVector p) :
  G4QHadron(nucQC,p), maxClust(0), currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
#ifdef debug
  G4cout<<"G4QNucleus::(LV)Construction By QC="<<nucQC<<G4endl;
#endif
  probVect[0]=mediRatio;
  for(G4int i=1; i<256; i++) probVect[i] = 0.;
  Set4Momentum(p);
  G4int u=nucQC.GetU()-nucQC.GetAU();
  G4int d=nucQC.GetD()-nucQC.GetAD();
  S = nucQC.GetS()-nucQC.GetAS();     // a#of LAMBDA's in the nucleus
  G4int du= d-u;                      // isotopic shift
  G4int b =(d+u+S)/3;                 // baryon number
  Z = (b-S-du)/2;                     // protons
  N = Z+du;                           // neutrons
  SetQC(nucQC);
#ifdef debug
  G4cout<<"G4QNucleus::(LV)ConstructionByQC: N="<<N<<",Z="<<Z<<",S="<<S<<G4endl;
#endif
  G4QPDGCode nPDG(90000000+S*1000000+Z*1000+N);
  SetQPDG(nPDG);
  SetNFragments(0);
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(7) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(const G4QNucleus& right) : G4QHadron(&right), currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
  G4int lTb=right.GetBThickness()->size();
  if(lTb) for(G4int j=0; j<=lTb; j++) Tb->push_back((*right.GetBThickness())[j]);
  Set4Momentum   (right.Get4Momentum());
  SetQPDG        (right.GetQPDG());
  SetQC          (right.GetQC());
  SetNFragments  (right.GetNFragments());
  Z             = right.Z;
  N             = right.N;
  S             = right.S;
  dZ            = right.dZ;
  dN            = right.dN;
  dS            = right.dS;
  maxClust      = right.maxClust;
  for(G4int i=0; i<=maxClust; i++) probVect[i] = right.probVect[i];
  probVect[254] = right.probVect[254];
  probVect[255] = right.probVect[255];
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(8) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

G4QNucleus::G4QNucleus(G4QNucleus* right) : currentNucleon(-1)
{
  Tb = new std::vector<G4double>;
  G4int lTb=right->GetBThickness()->size();
  if(lTb) for(G4int j=0; j<=lTb; j++) Tb->push_back((*right->GetBThickness())[j]);
  Set4Momentum   (right->Get4Momentum());
  SetQPDG        (right->GetQPDG());
  SetQC          (right->GetQC());
  SetNFragments  (right->GetNFragments());
  Z             = right->Z;
  N             = right->N;
  S             = right->S;
  dZ            = right->dZ;
  dN            = right->dN;
  dS            = right->dS;
  maxClust      = right->maxClust;
  for(G4int i=0; i<=maxClust; i++) probVect[i] = right->probVect[i];
  probVect[254] = right->probVect[254];
  probVect[255] = right->probVect[255];
#ifdef pardeb
  G4cout<<"G4QNucleus::Constructor:(9) N="<<freeNuc<<", D="<<freeDib<<", W="<<clustProb
        <<", R="<<mediRatio<<G4endl;
#endif
}

// Assignment operator
const G4QNucleus& G4QNucleus::operator=(const G4QNucleus& right)
{//               ==============================================
  if(this != &right)                          // Beware of self assignment
  {
    Tb->clear();
    G4int lTb=right.GetBThickness()->size();
    if(lTb) for(G4int j=0; j<=lTb; j++) Tb->push_back((*right.GetBThickness())[j]);
    Set4Momentum   (right.Get4Momentum());
    SetQPDG        (right.GetQPDG());
    SetQC          (right.GetQC());
    SetNFragments  (right.GetNFragments());
    Z             = right.Z;
    N             = right.N;
    S             = right.S;
    dZ            = right.dZ;
    dN            = right.dN;
    dS            = right.dS;
    maxClust      = right.maxClust;
    for(G4int i=0; i<=maxClust; i++) probVect[i] = right.probVect[i];
    probVect[254] = right.probVect[254];
    probVect[255] = right.probVect[255];
  }
  return *this;
}

G4QNucleus::~G4QNucleus()
{
  if(theNucleons.size()) for_each(theNucleons.begin(),theNucleons.end(),DeleteQHadron());
  Tb->clear();
  delete Tb;
}

// Fill the private parameters
void G4QNucleus::SetParameters(G4double a,G4double b, G4double c, G4double d, G4double e)
{//  ====================================================================================
  freeNuc=a; 
  freeDib=b; 
  clustProb=c;
  mediRatio=d;
		nucleonDistance=e;
}

// Standard output for QNucleus {Z - a#of protons, N - a#of neutrons, S - a#of lambdas}
ostream& operator<<(ostream& lhs, G4QNucleus& rhs)
{//      =========================================
 lhs<<"{Z="<<rhs.GetZ()<<",N="<<rhs.GetN()<<",S="<<rhs.GetS()<<",M="<<rhs.GetGSMass()<<"}";
 return lhs;
}

// Standard output for QNucleus {Z - a#of protons, N - a#of neutrons, S - a#of lambdas}
ostream& operator<<(ostream& lhs, const G4QNucleus& rhs)
{//      ===============================================
  lhs<<"{Z="<<rhs.GetZ()<<",N="<<rhs.GetN()<<",S="<<rhs.GetS()<< "}";
  return lhs;
}

// Init existing nucleus by new PDG Code
void G4QNucleus::InitByPDG(G4int nucPDG)
{//  ===================================
  static const G4int NUCPDG  = 90000000;
#ifdef debug
  G4cout<<"G4QNucleus::InitByPDG: >Called< PDG="<<nucPDG<<G4endl;
#endif
  probVect[0]=mediRatio;                        // init Vacuum/Medium probability
  if(nucPDG<80000000) nucPDG=HadrToNucPDG(nucPDG); // Convert HadrPDGCode to NucPDGCode
  G4int s=0;
  G4int z=0;
  G4int n=0;
  if(nucPDG>80000000&&nucPDG<100000000) // Try to convert the NUCCoding to PDGCoding
  {
    G4QPDGCode(22).ConvertPDGToZNS(nucPDG, z, n, s);
    Z  =z;
    N  =n;
    S  =s;
#ifdef debug
    G4cout<<"G4QNucleus::InitByPDG:Z="<<Z<<",N="<<N<<",S="<<S<<G4endl;
#endif
    SetZNSQC(Z,N,S);  // @@ ??
    G4QPDGCode nPDG(nucPDG);
    G4double PDGMass=0.;
    if(nucPDG!=NUCPDG) PDGMass=nPDG.GetMass();
    SetQPDG(nPDG);
    G4LorentzVector p(0.,0.,0.,PDGMass);
    Set4Momentum(p);
    SetNFragments(0);
#ifdef debug
	G4cout<<"G4QNucleus::InitByPDG:->QPDG="<<nPDG<<": 4M="<<p<<G4endl;
#endif
  }
  else
  {
    G4cerr<<"***G4QNucleus::InitByPDG:Initialized by not nuclear PDGCode="<<nucPDG<<G4endl;
    //throw G4QException("G4QNucleus::InitByPDG:PDGCode can't be converted to NucPDGCode");
  }
}
// End of "InitByPDG"

// Calculate probabilities of clusters and return the maximum baryon number of clusters
G4int G4QNucleus::UpdateClusters(G4bool din)
{//   ======================================
  //static const G4double r0 = 1.1;          // fm, for nuclear radius: r=r0*A^(1/3)
  //static const G4double del= .55;          // fm, for a difused surface of the nucleus
  //static const G4double rCl= 2.0;          // clusterization radius @@??
  //static const G4double freeibuc = 0.10;   // probab. of the quasi-free baryon on surface
  //static const G4double freeDib = 0.05;    // probab. of the quasi-free dibar. on surface
  //static const G4double clustProb = 4.0;   // clusterization probability in dense region
  //static const G4double prQ = 1.0;         // relative probability for a Quasmon
  //static const G4double prQ = 0.;          //@@for pi@@relative probability for Quasmon
  G4double probSInt[254];                    // integrated static probabilities
  for (G4int in=0; in<256; in++) probVect[in]=0.; // Make preinit to avoid the postinit
  probSInt[0]=0;                             // integrated static probabilities
  G4int a = Z + N + S;                       // atomic number
#ifdef pdebug
  G4cout<<"G4QN::UpdateCl:A="<<a<<"(Z="<<Z<<",N="<<N<<",S="<<S<<"),mR="<<mediRatio<<G4endl;
#endif
  G4double A=a;
  if(A<=0.)
  {
#ifdef debug
    G4cout<<"***G4QNucleus::UpdateClusters:No clusters can be calculated as A="<<A<<G4endl;
#endif
    return 0;
  }
  G4double surf=freeNuc+freeDib;             // surface relative population
  G4double surA=A*surf;                      // surface absolute population
  G4int sA=static_cast<G4int>(surA);
  if(surf>0.||surf<1.)sA=RandomizeBinom(surf,a); // randomize SurfaceNucleons by Binomial
#ifdef pdebug
  G4cout<<"G4QN::UpdateCl:surf="<<surf<<"= N="<<freeNuc<<"+D="<<freeDib<<",A="<<sA<<G4endl;
#endif
  G4int dA=a-sA;                             // a#of nucleons in dense part of the nucleus
  if (din && dA<2 && a>2)
  {
	dA=2;
    sA=a-2;
  }
#ifdef pdebug
  G4cout<<"G4QN::UpdtC:dA="<<dA<<",A="<<A<<",s="<<surf<<",S="<<sA<<",C="<<maxClust<<G4endl;
#endif
  G4int maxi=1;                              // A#of elements filled by the progran
  G4double pA=0.;
  G4double uA=0.;
  G4double dpA=0.;
  if(surf>0.)
  {
    pA=0.5*freeDib*sA/surf; //@@Randomize(?)// a#of quasi-free Nucleon Pairs on the surface
    dpA=pA+pA;
    uA=sA-pA-pA;                             // a#of quasi-free nucleons on Nuclear Surface
  }
  uA=uA/A;                                   // Normalization of probability
  pA=pA/A;
  G4double sum =0.;
  if(dA<2)                                   // There is no dense phase at all
  {
    //probVect[1]= dA/A;                       // a#of quasi-free nucleons (only dense)
    //probVect[1]= (uA+dA)/A;                  // a#of quasi-free nucleons (different norm)
    probVect[1]= uA+dA/A;                    // a#of quasi-free nucleons (correct)
    sum = probVect[1];
    probSInt[1]=sum;                         // integrated static probabilities
    maxi=2;
    probVect[254]= 0;                        // a#of dense nucleons (correct)
    if(A>1 && pA>0.)
    {
      //probVect[2]= (pA+pA)/A/(A-1);          // a#of quasi-free "dibaryons" (correct)
      probVect[2]= pA;                       // a#of quasi-free "dibaryons" (correct)
      //probVect[2]= 0;                        // a#of quasi-free "dibaryons" (only dense)
      sum+= probVect[2]+probVect[2];
      probSInt[2]=sum;                       // integrated static probabilities
      maxi=3;
      probVect[255]= 0;                      // a#of dense "dibaryons" (correct)
    }
#ifdef pdebug
    G4cout<<"G4QNucleus::UpdateClust:Only quasi-free nucleons pV[1]="<<probVect[1]<<G4endl;
#endif
    dZ=0;
    dN=0;
    dS=0;
  }
  else
  {
    G4double wrd=clustProb/dA;               // relative volume of clusterization (omega)
    G4double sud=pow(1.+wrd,dA-1);           // normalization factor for the dense region
    // dA=C*Sum_k=1-A[n*C^A_k*wrd^(k-1)]=C*dA*(1+wrd)^(dA-1) => C=1/sud, sud=(1+wrd)^(dA-1)
    // =1
    G4double rd= dA/sud/A;
    //G4double comb=A;
    //G4double prb=rd;                              // (only dense)
    G4double prb=rd+uA;
    sum =prb;
#ifdef pdebug
	G4cout<<"G4QNucl::UpdateCl:sud="<<sud<<",v[1]=s="<<sum<<",dA="<<dA<<",uA="<<uA<<G4endl;
#endif
    //probVect[1]= prb/comb;                   // a#of quasi-free nucleons (correct)
    //probVect[254]= rd/comb;                  // a#of dense nucleons (correct)
    probVect[1]= prb;                        // a#of quasi-free nucleons (correct)
    probVect[254]= rd;                       // a#of dense nucleons (correct)
    probSInt[1]=sum;                         // integrated static probabilities
    // =2
    rd*=wrd*(dA-1.)/2;
    //comb*=(A-1.)/2;
    //prb=rd;                                  // (only dense)
    prb=rd+pA;
    sum+=prb+prb;
#ifdef pdebug
	G4cout<<"G4QNucl::UpdateCl:sud="<<sud<<",v[2]="<<prb<<",s="<<sum<<",pA="<<pA<<G4endl;
#endif
    //probVect[2]= prb/comb;                   // a#of quasi-free "dibaryons" (correct)
    //probVect[255]= rd/comb;                  // a#of dense "dibaryons" (correct)
    probVect[2]= prb;                        // a#of quasi-free "dibaryons" (correct)
    probVect[255]= rd;                       // a#of dense "dibaryons" (correct)
    probSInt[2]=sum;                         // integrated static probabilities
    // >2
    maxi=3;
#ifdef debug
	G4cout<<"G4QNucleus::UpdateClusters:p1="<<probVect[1]<<", p2="<<probVect[2]<<",sA="<<sA
          <<",uA="<<uA<<",pA="<<pA<<",wrd="<<wrd<<",sud="<<sud<<G4endl;
#endif
    if(dA>2)
    {
      ///////////G4double itA=A+1.; 
      G4double idA=dA+1.; 
      G4int dLim=dA;
      if(maxClust<dA) dLim=maxClust;
      for (int i=3; i<=dLim; i++)
      {
        rd*=wrd*(idA-i)/i;
        sum+=rd*i;
#ifdef pdebug
	    G4cout<<"G4QNucleus::UpdateCl:sud="<<sud<<", v["<<i<<"]="<<rd<<", s="<<sum<<G4endl;
#endif
        //comb*=(itA-i)/i;
        //probVect[i]=rd/comb;                 // Divide by sum of combinations for N+Z+S
        probVect[i]=rd;                      // Comb's for N,Z,S are canceled later(G4QNuc)
        probSInt[i]=sum;                     // integrated static probabilities
        maxi=i+1;
#ifdef debug
        G4cout<<"G4QNucleus::UpdateCl:Cluster of "<<i<<" baryons,pV="<<probVect[i]<<G4endl;
#endif
      }
	}
    dS = S;                                  // @@ Lambdas are always in the dense region
    dZ = static_cast<int>(static_cast<double>((dA-dS)*Z)/(Z+N) + 0.5);
    dN = dA - dZ;
  }
#ifdef pdebug
  G4cout<<"G4QNucleus::UpdateClusters: Sum of weighted probabilities s="<<sum<<G4endl;
#endif
  maxClust=maxi-1;
  //for (G4int j=maxi; j<255; j++) probVect[j]=0.;//Make the rest to be 0 [preinited above]
  // ===================== From here probability randomization starts ===============
  //  G4int rA=a;                              // Residual number of nucleons
  //#ifdef pdebug
  //G4cout<<"G4QNuc::UpdateClust:A="<<A<<",M="<<k<<",P1="<<probVect[1]<<",P2="<<probVect[2]
  //      <<G4endl;
  //#endif
  //if (k>1) for (j=k; j>1; j--)               // nucleons are not randomized
  //{
  //  G4int jmax=rA/j;                         // Max number of this kind of clusters
  //  if (jmax)
  //  {
  //    G4double prob=probVect[j]/probSInt[j]; // Probab of the cluster in the dest nucleus
  //#ifdef pdebug
  //    G4cout<<"G4QNucl::UpdateClusters: j="<<j<<",sP="<<probVect[j]<<",iP="<<probSInt[j]
  //          <<G4endl;
  //#endif
  //	G4int m=RandomizeBinom(prob,jmax);     // A#of clusters of this type
  //    if(m)
  //    {
  //      probVect[j]=m;
  //      rA-=m*j;
  //    }
  //    else
  //    {
  //      probVect[j]=0.;
  //      if(j==maxClust) maxClust--;
  //    }
  //#ifdef pdebug
  //    G4cout<<"G4QNucl::UpdateClust:p="<<prob<<",r="<<rA<<",m="<<jmax<<",P="<<probVect[j]
  //          <<G4endl;
  //#endif
  //  }
  //  else
  //  {
  //    probVect[j]=0.;
  //    if(j==maxClust) maxClust--;
  //  }
  //}
  //probVect[1]=rA;
  // ===================== From here probability randomization starts ===============
  return maxClust;
}
// End of "UpdateClusters"

// Reduce nucleus by emitted cluster with PDG Code cPDG
void G4QNucleus::Reduce(G4int cPDG)
{//  ==============================
  static const G4int NUCPDG=90000000;
  if(cPDG>80000000&&cPDG!=NUCPDG)
  {
    G4int curPDG=GetPDG();
    G4int newPDG=curPDG-cPDG+NUCPDG;             // PDG Code of Residual Nucleus
    if(newPDG==NUCPDG) InitByPDG(NUCPDG);        // Empty
    else
    {
      //if(abs(newPDG)<NUCPDG)
	  //{
      //  G4cerr<<"***G4QNucleus::Reduce:iPDG="<<curPDG<<"=newPDG="<<newPDG<<"+cPDG="<<cPDG
      //        <<G4endl;
      //  throw G4QException("*E*:::G4QNucleus::Reduce: Abnormal Nuclear Reduction");
	  //}
      InitByPDG(newPDG);                         // Reinit the Nucleus
	}
  }
  else if(cPDG!=NUCPDG) G4cerr<<"***G4QN::Reduce:Subtract not nuclear PDGC="<<cPDG<<G4endl;
  // in case of cPDG=90000000 - subtract nothing
}

// Increase nucleus by cluster with PDG Code cPDG (4-mom is optional)
void G4QNucleus::Increase(G4int cPDG, G4LorentzVector c4M)
{//  =====================================================
  static const G4int NUCPDG=90000000;
  if(cPDG>80000000&&cPDG!=NUCPDG)
  {
    G4int newPDG=GetPDG()+cPDG-NUCPDG;        // PDG Code of the New Nucleus
    InitByPDG(newPDG);                        // Reinit the Nucleus
    if (c4M!=G4LorentzVector(0.,0.,0.,0.))
    {
      G4LorentzVector t4M = Get4Momentum();   // 4Mom of the nucleus
      t4M +=c4M;
      Set4Momentum(t4M);
    }
  }
  else G4cerr<<"***G4QNucleus::Increase: PDGCode="<<cPDG<<",4M="<<c4M<<G4endl;
}

// Increase nucleus by Quasmon with Quark Content qQC (4-mom is optional)
void G4QNucleus::Increase(G4QContent qQC, G4LorentzVector q4M)
{//  =========================================================
    G4LorentzVector t4M = Get4Momentum();     // 4Mom of the old nucleus
    G4QContent  newQC   = GetQC()+qQC;        // Quark Content of the New Nucleus
    InitByQC(newQC);                          // Reinit the Nucleus
    t4M +=q4M;
    Set4Momentum(t4M);                        // 4Mom of the new nucleus
}

// Set Quark Content, using Z,N,S of nucleus
void G4QNucleus::SetZNSQC(G4int z, G4int n, G4int s)
{//  ===============================================
  G4int zns=z+n+s;
  G4int Dq=n+zns;
  G4int Uq=z+zns;
  G4int Sq=s;
  if      (Dq<0&&Uq<0&&Sq<0)SetQC(G4QContent( 0, 0, 0,-Dq,-Uq,-Sq));
  else if (Uq<0&&Sq<0)      SetQC(G4QContent(Dq, 0, 0,  0,-Uq,-Sq));
  else if (Dq<0&&Sq<0)      SetQC(G4QContent( 0,Uq, 0,-Dq,  0,-Sq));
  else if (Dq<0&&Uq<0)      SetQC(G4QContent( 0, 0,Sq,-Dq,-Uq,  0));
  else if (Uq<0)            SetQC(G4QContent(Dq, 0,Sq,  0,-Uq,  0));
  else if (Sq<0)            SetQC(G4QContent(Dq,Uq, 0,  0,  0,-Sq));
  else if (Dq<0)            SetQC(G4QContent(0 ,Uq,Sq,-Dq,  0,  0));
  else                      SetQC(G4QContent(Dq,Uq,Sq,  0,  0,  0));
}
  
// Tests if it is possible to split one Baryon (n,p,Lambda) or alpha from the Nucleus
G4int G4QNucleus::SplitBaryon()
{//  ===========================
  static const G4QContent neutQC(2,1,0,0,0,0);
  static const G4QContent protQC(1,2,0,0,0,0);
  static const G4QContent lambQC(1,1,1,0,0,0);
  static const G4QContent deutQC(3,3,0,0,0,0);
  static const G4QContent alphQC(6,6,0,0,0,0);
  static const G4double mNeut= G4QPDGCode(2112).GetMass();
  static const G4double mProt= G4QPDGCode(2212).GetMass();
  static const G4double mLamb= G4QPDGCode(3122).GetMass();
  static const G4double mDeut= G4QPDGCode(2112).GetNuclMass(1,1,0);
  static const G4double mAlph= G4QPDGCode(2112).GetNuclMass(2,2,0);
  G4int     baryn=GetA();                         // Baryon Number of the Nucleus
  if(baryn<2) return 0;
  //G4double   totM=GetGSMass();                    // GS Mass value of the Nucleus
  G4double   totM=Get4Momentum().m();             // Real Mass value of the Nucleus
  G4QContent valQC=GetQCZNS();                    // Quark Content of the Nucleus
#ifdef pdebug
  G4cout<<"G4QNucleus::SplitBaryon: B="<<baryn<<", M="<<totM<<valQC<<G4endl;
#endif
  G4int NQ=valQC.GetN();
  if(NQ)                                          // ===> "Can try to split a neutron" case
  {
    G4QContent resQC=valQC-neutQC;                // QC of Residual for the Neutron
    G4int    resPDG=resQC.GetSPDGCode();          // PDG of Residual for the Neutron
    G4double resMas=G4QPDGCode(resPDG).GetMass(); // GS Mass of the Residual
    G4double sM=resMas+mNeut;
#ifdef pdebug
    G4cout<<"G4QNucleus::SplitBaryon: (neutron),sM="<<sM<<",d="<<totM-sM<<G4endl;
#endif
    if(sM<totM+.001) return 2112;
  }
  G4int PQ=valQC.GetP();
  if(PQ)                                          // ===> "Can try to split a proton" case
  {
    G4QContent resQC=valQC-protQC;                // QC of Residual for the Proton
    G4int    resPDG=resQC.GetSPDGCode();          // PDG of Residual for the Proton
    G4double resMas=G4QPDGCode(resPDG).GetMass(); // GS Mass of the Residual
    G4double CB=CoulombBarrier(1,1);              // Coulomb Barrier for the proton
    G4double sM=resMas+mProt+CB;
    /////////G4double sM=resMas+mProt;
#ifdef pdebug
    G4cout<<"G4QNucleus::SplitBaryon: (proton),sM="<<sM<<",d="<<totM-sM<<G4endl;
#endif
    if(sM<totM+.001) return 2212;
  }
  G4int LQ=valQC.GetL();
  if(LQ)                                          // ===> "Can try to split a lambda" case
  {
    G4QContent resQC=valQC-lambQC;                // QC of Residual for the Lambda
    G4int    resPDG=resQC.GetSPDGCode();          // PDG of Residual for the Lambda
    G4double resMas=G4QPDGCode(resPDG).GetMass(); // GS Mass of the Residual
    G4double sM=resMas+mLamb;
#ifdef pdebug
    G4cout<<"G4QNucleus::SplitBaryon: (lambda),sM="<<sM<<",d="<<totM-sM<<G4endl;
#endif
    if(sM<totM+.001) return 3122;
  }
  G4int AQ=NQ+PQ+LQ;
  if(NQ>0&&PQ>0&&AQ>2)                            // ===> "Can try to split deuteron" case
  {
    G4QContent resQC=valQC-deutQC;                // QC of Residual for the Deuteron
    G4int    resPDG=resQC.GetSPDGCode();          // PDG of Residual for the Deuteron
    G4double resMas=G4QPDGCode(resPDG).GetMass(); // GS Mass of the Residual
    G4double CB=CoulombBarrier(1,2);              // Coulomb Barrier for the Deuteron
    G4double sM=resMas+mDeut+CB;
    //G4double sM=resMas+mDeut;
#ifdef pdebug
    G4cout<<"G4QNucleus::SplitBaryon: (deuteron),sM="<<sM<<",d="<<totM-sM<<G4endl;
#endif
    if(sM<totM+.001) return 90001001;
  }
  if(NQ>1&&PQ>1&&AQ>4)                            // ===> "Can try to split an alpha" case
  {
    G4QContent resQC=valQC-alphQC;                // QC of Residual for the Alpha
    G4int    resPDG=resQC.GetSPDGCode();          // PDG of Residual for the Alpha
    G4double resMas=G4QPDGCode(resPDG).GetMass(); // GS Mass of the Residual
    G4double CB=CoulombBarrier(2,4);              // Coulomb Barrier for the Alpha
    G4double sM=resMas+mAlph;
    if(NQ!=4||PQ!=4) sM+=CB;
#ifdef pdebug
    G4cout<<"G4QNucleus::SplitBaryon: (alpha),sM="<<sM<<",d="<<totM-sM<<G4endl;
#endif
    if(sM<totM+.001) return 90002002;
  }
  return 0;
}
  
// Tests if it is possible to split two Baryons (nn,np,pp,Ln,Lp,LL) from the Nucleus
G4bool G4QNucleus::Split2Baryons()
{//  =============================
  static const G4QContent neutQC(2,1,0,0,0,0);
  static const G4QContent protQC(1,2,0,0,0,0);
  static const G4QContent lambQC(1,1,1,0,0,0);
  static const G4double mNeut= G4QPDGCode(2112).GetMass();
  static const G4double mProt= G4QPDGCode(2212).GetMass();
  static const G4double mLamb= G4QPDGCode(3122).GetMass();
  G4int     baryn=GetA();                        // Baryon Number of the Nucleus
  if(baryn<3) return false;
  G4double   totM=Get4Momentum().m();            // Real Mass value of the Nucleus
  G4QContent valQC=GetQCZNS();                   // Quark Content of the Nucleus
#ifdef pdebug
  G4cout<<"G4QNucleus::Split2Baryons: B="<<baryn<<", M="<<totM<<valQC<<G4endl;
#endif
  G4int NQ=valQC.GetN();
  if(NQ>1)                                       // ===> "Can try to split 2 neutrons" case
  {
    G4QContent resQC=valQC-neutQC-neutQC;        // QC of ResidNucleus for the Two Neutrons
    G4int    resPDG=resQC.GetSPDGCode();         // PDG of ResidNucleus for 2 Neutrons
    G4double resMas=G4QPDGCode(resPDG).GetMass();// GS Mass of the Residual Nucleus
    G4double sM=resMas+mNeut+mNeut;
#ifdef pdebug
    G4cout<<"G4QNucleus::Split2Baryons: (2 neutrons), sM="<<sM<<", d="<<totM-sM<<G4endl;
#endif
    if(sM<totM) return true;
  }
  G4int PQ=valQC.GetP();
  if(PQ>1)                                       // ===> "Can try to split 2 protons" case
  {
    G4QContent resQC=valQC-protQC-protQC;        // QC of ResidualNucleus for 2 Protons
    G4int    resPDG=resQC.GetSPDGCode();         // PDG of Residual Nucleus for 2 Proton
    G4double resMas=G4QPDGCode(resPDG).GetMass();// GS Mass of the Residual Nucleus
    G4double sM=resMas+mProt+mProt;
#ifdef pdebug
    G4cout<<"G4QNucleus::Split2Baryons: (2 protons), sM="<<sM<<", d="<<totM-sM<<G4endl;
#endif
    if(sM<totM) return true;
  }
  if(PQ&&NQ)                                     // ===> "Can try to split proton+neutron"
  {
    G4QContent resQC=valQC-protQC-neutQC;        // QC of ResidNucleus for Proton+Neutron
    G4int    resPDG=resQC.GetSPDGCode();         // PDG of ResidNucleus for Proton+Neutron
    G4double resMas=G4QPDGCode(resPDG).GetMass();// GS Mass of the Residual Nucleus
    G4double sM=resMas+mProt+mNeut;
#ifdef pdebug
    G4cout<<"G4QNucleus::Split2Baryons:(proton+neutron), sM="<<sM<<", d="<<totM-sM<<G4endl;
#endif
    if(sM<totM) return true;
  }
  G4int LQ=valQC.GetL();
  if(LQ&&NQ)                                     // ===> "Can try to split lambda+neutron"
  {
    G4QContent resQC=valQC-lambQC-neutQC;        // QC of ResidNucleus for Lambda+Neutron
    G4int    resPDG=resQC.GetSPDGCode();         // PDG of ResidNucleus for Lambda+Neutron
    G4double resMas=G4QPDGCode(resPDG).GetMass();// GS Mass of the Residual Nucleus
    G4double sM=resMas+mLamb+mNeut;
#ifdef pdebug
    G4cout<<"G4QNucleus::Split2Baryons:(lambda+neutron), sM="<<sM<<", d="<<totM-sM<<G4endl;
#endif
    if(sM<totM) return true;
  }
  if(LQ&&PQ)                                     // ===> "Can try to split lambda+proton"
  {
    G4QContent resQC=valQC-protQC-lambQC;        // QC of ResidNucleus for Proton+Lambda
    G4int    resPDG=resQC.GetSPDGCode();         // PDG of ResidNucleus for Proton+Lambda
    G4double resMas=G4QPDGCode(resPDG).GetMass();// GS Mass of the Residual Nucleus
    G4double sM=resMas+mProt+mLamb;
#ifdef pdebug
    G4cout<<"G4QNucleus::Split2Baryons: (proton+lambda), sM="<<sM<<", d="<<totM-sM<<G4endl;
#endif
    if(sM<totM) return true;
  }
  if(LQ>1)                                       // ===> "Can try to split 2 lambdas" case
  {
    G4QContent resQC=valQC-lambQC-lambQC;        // QC of ResidNucleus for the Two Lambdas
    G4int    resPDG=resQC.GetSPDGCode();         // PDG of ResidNucleus for the Two Lambdas
    G4double resMas=G4QPDGCode(resPDG).GetMass();// GS Mass of the Residual Nucleus
    G4double sM=resMas+mLamb+mLamb;
#ifdef pdebug
    G4cout<<"G4QNucleus::Split2Baryons: (two lambdas), sM="<<sM<<", d="<<totM-sM<<G4endl;
#endif
    if(sM<totM) return true;
  }
  return false;
}
  
// Evaporate one Baryon (n,p,Lambda) (h1) from the Nucleus & get Residual Nucleus (h2)
G4bool G4QNucleus::EvaporateBaryon(G4QHadron* h1, G4QHadron* h2)
{//  ===========================================================
  //static const G4double   uWell=2.7;              // EffectiveDepth of potential well B
  //static const G4double   uWell=7.;               // EffectiveDepth of potential well B
  static const G4double   uWell=1.7;              // EffectiveDepth of potential well B
  //static const G4double   uWell=0.0;              // EffectiveDepth of potential well B
  //static const G4double   alpha=7.2;              // Probability for alpha to evaporate
  //////////static const G4double   gunA=80.;       // Switch A-parameter for BaryonGun
  //static const G4double   gunB=exp(1)/gunA;
  ///////////////////static const G4double   gunB=exp(2)/4/gunA/gunA;
  //////////////static const G4double   gunP2=200000.; // Switch P2-parameter for BaryonGun
  //////////////static const G4double   maSht=1.2;  // shift for maximal x approximation
  ///////////static const G4double   coSht=.19;     // multiple for maximal x approximation
  //////////////static const G4double   third=1./3.;// power for maximal x approximation
  static const G4int      gPDG =   22;            // PDGCode of gamma
  static const G4QPDGCode gQPDG(gPDG);            // QPDGCode of gamma
  static const G4int      nPDG = 2112;            // PDGCode of neutron
  static const G4QPDGCode nQPDG(nPDG);            // QPDGCode of neutron
  static const G4int      pPDG = 2212;            // PDGCode of proton
  static const G4QPDGCode pQPDG(pPDG);            // QPDGCode of proton
  static const G4int      lPDG = 3122;            // PDGCode of Lambda
  static const G4QPDGCode lQPDG(lPDG);            // QPDGCode of Lambda
  static const G4int      dPDG = 90001001;        // PDGCode of deutron
  static const G4int      aPDG = 90002002;        // PDGCode of ALPHA
  static const G4QPDGCode aQPDG(aPDG);            // QPDGCode of ALPHA
  static const G4QPDGCode NPQPDG(dPDG);           // QPDGCode of deutron
  static const G4QPDGCode NNQPDG(90000002);       // QPDGCode of n+n
  static const G4QPDGCode PPQPDG(90002000);       // QPDGCode of p+p
  static const G4QPDGCode NLQPDG(91000001);       // QPDGCode of n+L
  static const G4QPDGCode PLQPDG(91001000);       // QPDGCode of p+L
  static const G4QPDGCode LLQPDG(92000000);       // QPDGCode of L+L
  static const G4QPDGCode NAQPDG(90002003);       // QPDGCode of N+ALPHA
  static const G4QPDGCode PAQPDG(90003002);       // QPDGCode of L+ALPHA
  static const G4QPDGCode LAQPDG(91002002);       // QPDGCode of L+ALPHA
  static const G4QPDGCode AAQPDG(90004004);       // QPDGCode of ALPHA+ALPHA
  static const G4QPDGCode PIPQPDG(211);           // QPDGCode of PI+
  static const G4QPDGCode PIMQPDG(-211);          // QPDGCode of PI+
  static const G4double   mNeut= G4QPDGCode(nPDG).GetMass(); // Mass of neutron
  static const G4double   mProt= G4QPDGCode(pPDG).GetMass(); // Mass of proton
  static const G4double   mLamb= G4QPDGCode(lPDG).GetMass(); // Mass of Lambda
  static const G4double   mDeut= G4QPDGCode(nPDG).GetNuclMass(1,1,0);// Mass of deutr
  static const G4double   mAlph= G4QPDGCode(nPDG).GetNuclMass(2,2,0);// Mass of alpha
  static const G4double   mPi  = G4QPDGCode(211).GetMass();  // Mass of charged pion
  static const G4double   mN2  = mNeut*mNeut;     // Mass^2 of neutron
  static const G4double   mP2  = mProt*mProt;     // Mass^2 of proton
  static const G4double   mL2  = mLamb*mLamb;     // Mass^2 of Lambda
  static const G4double   mA2  = mAlph*mAlph;     // Mass^2 of Alpha
  static const G4double   mNP  = mNeut+mProt;     // proton and neutron mass
  //static const G4double   mNN  = mNeut+mNeut;     // 2 neutrons mass
  //static const G4double   mPP  = mProt+mProt;     // 2 protons mass
  //static const G4double   mNL  = mNeut+mLamb;     // neutron and Lambda mass
  //static const G4double   mPL  = mProt+mLamb;     // proton and Lambda mass
  //static const G4double   mLL  = mLamb+mLamb;     // 2 Lambdas mass
  G4bool barf=true;                               // Take into account CB in limits
  G4double uW=uWell;
  G4int    a = GetA();
  G4double alpha=0.;                                // NO alpha evaporation
  //G4double alpha=1.;
  //G4double alpha=clustProb*clustProb*clustProb;
#ifdef ppdebug
  G4cout<<"G4QNucleus::EvaporBaryon:Called with a="<<a<<GetThis()<<",alph="<<alpha<<G4endl;
#endif
  G4double a1= a-1;
  //////////G4double z = Z;
  //////////G4double zn= Z+N;
  G4double PBarr= CoulombBarrier(1,1);            // CoulombBarrier for proton
  G4double PPBarr= CoulombBarrier(1,1,1,1);       // CoulombBarrier for proton (after prot)
  G4double PABarr= CoulombBarrier(1,1,2,4);       // CoulombBarrier for proton (after alph)
  G4double APBarr= CoulombBarrier(2,4,1,1);       // CoulombBarrier for alpha (after prot)
  G4double ABarr= CoulombBarrier(2,4);            // CoulombBarrier for alpha
  G4double AABarr= CoulombBarrier(2,4,2,4);       // CoulombBarrier for alpha (after alpha)
  G4double PPPBarr= CoulombBarrier(1,1,2,2);      // CoulombBarrier for proton (after 2 pr)
  G4double AAABarr= CoulombBarrier(2,4,4,8);      // CoulombBarrier for alpha (after 2alph)
  //////G4double APABarr= CoulombBarrier(2,4,3,5);// CoulombBarrier for alpha (after p+al)
  G4double PPABarr= CoulombBarrier(1,1,3,5);      // CoulombBarrier for proton (after p+al)
  G4double SPPBarr=PBarr+PPBarr;                  // SummedCoulombBarrier for p+p pair
  G4double SAABarr=ABarr+AABarr;                  // SummedCoulombBarrier for 2 alpha pair
  G4double SPPPBarr=SPPBarr+PPPBarr;              // SummedCoulombBarrier for 3 protons
  G4double SAAABarr=SAABarr+AAABarr;              // SummedCoulombBarrier for 3 alphas
  G4double SAPBarr=PABarr+ABarr;                  // SummedCoulombBarrier for alpha+p pair
  G4double DAPBarr=APBarr+PBarr;                  // Other SummedCoulombBarrier for alph+2p
  if(DAPBarr>SAPBarr)SAPBarr=DAPBarr;             // Get max to make possible BothSequences
  ///////G4double SAPABarr=APABarr+SAPBarr;       // Summed Coulomb Barrier for alph+p+alph
  G4double SPPABarr=PPABarr+SAPBarr;              // Summed Coulomb Barrier for p+p+alpha
  G4LorentzVector h1mom;
  G4LorentzVector h2mom;
  G4LorentzVector h3mom;
  G4double totMass= GetMass();                    // Total mass of the Nucleus
  //if(totMass-GetGSMass()>100.)throw G4QException("****Big Mass");
#ifdef ppdebug
  G4cout<<"G4QN::EB:pB="<<PBarr<<",aB="<<ABarr<<",ppB="<<PPBarr<<",paB="<<PABarr<<G4endl;
#endif
  if(a==2)
  {
    if(Z<0||N<0)
	{
      G4int  nucPDG = 2112;
      G4double nucM = mNeut;
      G4int   piPDG = -211;
      G4QPDGCode db = NNQPDG;
      G4QPDGCode pi = PIMQPDG;
      if(N<0)
	  {
        nucPDG = 2212;
        nucM   = mProt;
        piPDG  = 211;
        db     = PPQPDG;
        pi     = PIPQPDG;
	  }
      if(totMass>mPi+nucM+nucM)
	  {
        G4LorentzVector n14M(0.,0.,0.,nucM);
        G4LorentzVector n24M(0.,0.,0.,nucM);
        G4LorentzVector pi4M(0.,0.,0.,mPi);
	    if(!DecayIn3(n14M,n24M,pi4M))
		{
          G4cerr<<"***G4QNucl::EvapBary: tM="<<totMass<<"-> 2N="<<nucPDG<<"(M="
		        <<nucM<<") + pi="<<piPDG<<"(M="<<mPi<<")"<<G4endl;
		  //throw G4QException("G4QNucl::EvapBary:ISO-dibaryon DecayIn3 did not succeed");
		  return false;
		}
        n14M+=n24M;
        h1->SetQPDG(db);
        h2->SetQPDG(pi);
        h1->Set4Momentum(n14M);
        h2->Set4Momentum(pi4M);
        return true;
	  }
	  else
	  {
        G4cerr<<"***G4QNucleus::EvaporateBaryon: M="<<totMass
              <<", M="<<totMass<<" < M_2N+Pi, d="<<totMass-2*nucM-mPi<<G4endl;
        //throw G4QException("***G4QNucl::EvaporateBaryon: ISO-dibaryon under Mass Shell");
        return false;
	  }      
	}
    else if(Z==2)
    {
      h1mom=G4LorentzVector(0.,0.,0.,mProt);
      h2mom=h1mom;
      h1->SetQPDG(pQPDG);
      h2->SetQPDG(pQPDG);
	  if(!DecayIn2(h1mom,h2mom)) return false;
    }
    else if(N==2)
    {
      h1mom=G4LorentzVector(0.,0.,0.,mNeut);
      h2mom=h1mom;
      h1->SetQPDG(nQPDG);
      h2->SetQPDG(nQPDG);
	  if(!DecayIn2(h1mom,h2mom)) return false;
    }
    else if(N==1&&Z==1)
    {
      if(totMass<=mNP)
	  {
#ifdef ppdebug
        G4cout<<"G4QNucl::EvaporateBaryon: Photon ### d+g ###, dM="<<totMass-mNP<<G4endl;
#endif
        h1mom=G4LorentzVector(0.,0.,0.,0.);
        h2mom=G4LorentzVector(0.,0.,0.,mDeut);
        h1->SetQPDG(gQPDG);
        h2->SetQPDG(NPQPDG);
	  }
      else
	  {
        h1mom=G4LorentzVector(0.,0.,0.,mProt);
        h2mom=G4LorentzVector(0.,0.,0.,mNeut);
        h1->SetQPDG(pQPDG);
        h2->SetQPDG(nQPDG);
      }
	  if(!DecayIn2(h1mom,h2mom)) return false;
    }
    else if(Z==1&&S==1)
    {
      h1mom=G4LorentzVector(0.,0.,0.,mProt);
      h2mom=G4LorentzVector(0.,0.,0.,mLamb);
      h1->SetQPDG(pQPDG);
      h2->SetQPDG(lQPDG);
	  if(!DecayIn2(h1mom,h2mom)) return false;
    }
    else
    {
      h1mom=G4LorentzVector(0.,0.,0.,mNeut);
      h2mom=G4LorentzVector(0.,0.,0.,mLamb);
      h1->SetQPDG(nQPDG);
      h2->SetQPDG(lQPDG);
	  if(!DecayIn2(h1mom,h2mom)) return false;
    }
    h1->Set4Momentum(h1mom);
    h2->Set4Momentum(h2mom);
	return true;
  }
  else if(a>2)
  {
    G4bool nFlag    = false;               // Flag of possibility to radiate neutron
    G4bool pFlag    = false;               // Flag of possibility to radiate proton
    G4bool lFlag    = false;               // Flag of possibility to radiate lambda
    G4bool aFlag    = false;               // Flag of possibility to radiate alpha
    G4bool nnFlag   = false;               // Flag of possibility to radiate 2 neutrons
    G4bool npFlag   = false;               // Flag of possibility to radiate neutron+proton
    G4bool nlFlag   = false;               // Flag of possibility to radiate neutron+lambda
    G4bool ppFlag   = false;               // Flag of possibility to radiate 2 protons
    G4bool plFlag   = false;               // Flag of possibility to radiate proton+lambda
    G4bool llFlag   = false;               // Flag of possibility to radiate 2 lambdas
    G4bool paFlag   = false;               // Flag of possibility to radiate proton+alpha
    G4bool naFlag   = false;               // Flag of possibility to radiate neutron+alpha
    G4bool laFlag   = false;               // Flag of possibility to radiate lambda+alpha
    G4bool aaFlag   = false;               // Flag of possibility to radiate alpha+alpha
    G4bool nnnF     = false;
    G4bool nnpF     = false;
    G4bool nppF     = false;
    G4bool pppF     = false;
    G4bool nnlF     = false;
    G4bool nplF     = false;
    G4bool pplF     = false;
    G4bool nllF     = false;
    G4bool pllF     = false;
    G4bool lllF     = false;
    G4bool nnaF     = false;
    G4bool npaF     = false;
    G4bool ppaF     = false;
    G4bool nlaF     = false;
    G4bool plaF     = false;
    G4bool llaF     = false;
    G4bool paaF     = false;
    G4bool naaF     = false;
    G4bool laaF     = false;
    G4bool aaaF     = false;
    G4double GSMass = GetGSMass(); // Ground State mass of the Nucleus
    G4double GSResNN= GSMass;      // Prototype of Residual Nuclear Mass for n+n
    G4double GSResNP= GSMass;      // Prototype of Residual Nuclear Mass for n+p
    G4double GSResNL= GSMass;      // Prototype of Residual Nuclear Mass for n+l
    G4double GSResPP= GSMass;      // Prototype of Residual Nuclear Mass for p+p
    G4double GSResPL= GSMass;      // Prototype of Residual Nuclear Mass for p+l
    G4double GSResLL= GSMass;      // Prototype of Residual Nuclear Mass for l+l
    G4double GSResNA= GSMass;      // Prototype of Residual Nuclear Mass for n+alpha
    G4double GSResPA= GSMass;      // Prototype of Residual Nuclear Mass for p+alpha
    G4double GSResLA= GSMass;      // Prototype of Residual Nuclear Mass for l+alpha
    G4double GSResAA= GSMass;      // Prototype of Residual Nuclear Mass for alpha+alpha
    G4double GSResNa= GSMass;      // Prototype of Residual Nuclear Mass for alpha
    G4double GSReNNN= GSMass;      // Prototype of Residual Nuclear Mass for n+n+n
    G4double GSReNNP= GSMass;      // Prototype of Residual Nuclear Mass for n+n+p
    G4double GSReNPP= GSMass;      // Prototype of Residual Nuclear Mass for n+p+p
    G4double GSRePPP= GSMass;      // Prototype of Residual Nuclear Mass for p+p+p
    G4double GSReNNL= GSMass;      // Prototype of Residual Nuclear Mass for n+n+l
    G4double GSReNPL= GSMass;      // Prototype of Residual Nuclear Mass for n+p+l
    G4double GSRePPL= GSMass;      // Prototype of Residual Nuclear Mass for p+p+l
    G4double GSReNLL= GSMass;      // Prototype of Residual Nuclear Mass for n+l+l
    G4double GSRePLL= GSMass;      // Prototype of Residual Nuclear Mass for p+l+l
    G4double GSReLLL= GSMass;      // Prototype of Residual Nuclear Mass for l+l+l
    G4double GSReNNA= GSMass;      // Prototype of Residual Nuclear Mass for n+n+a
    G4double GSReNPA= GSMass;      // Prototype of Residual Nuclear Mass for n+p+a
    G4double GSRePPA= GSMass;      // Prototype of Residual Nuclear Mass for p+p+a
    G4double GSReNLA= GSMass;      // Prototype of Residual Nuclear Mass for n+l+a
    G4double GSRePLA= GSMass;      // Prototype of Residual Nuclear Mass for p+l+a
    G4double GSReLLA= GSMass;      // Prototype of Residual Nuclear Mass for l+l+a
    G4double GSRePAA= GSMass;      // Prototype of Residual Nuclear Mass for p+a+a
    G4double GSReNAA= GSMass;      // Prototype of Residual Nuclear Mass for n+a+a
    G4double GSReLAA= GSMass;      // Prototype of Residual Nuclear Mass for l+a+a
    G4double GSReAAA= GSMass;      // Prototype of Residual Nuclear Mass for a+a+a
    G4QPDGCode PQPDG(22);          // Prototype of QPDG for ResidualNucleus to proton
    G4QPDGCode NQPDG(22);          // Prototype of QPDG for ResidualNucleus to neutron
    G4QPDGCode LQPDG(22);          // Prototype of QPDG for ResidualNucleus to lambda
    G4QPDGCode AQPDG(22);          // Prototype of QPDG for ResidualNucleus to alpha
    G4QPDGCode nnQPDG(22);         // Prototype of QPDG for ResidualNucleus to nn-dibar.
    G4QPDGCode npQPDG(22);         // Prototype of QPDG for ResidualNucleus to np-dibar.
    G4QPDGCode nlQPDG(22);         // Prototype of QPDG for ResidualNucleus to nl-dibar.
    G4QPDGCode ppQPDG(22);         // Prototype of QPDG for ResidualNucleus to pp-dibar.
    G4QPDGCode plQPDG(22);         // Prototype of QPDG for ResidualNucleus to pl-dibar.
    G4QPDGCode llQPDG(22);         // Prototype of QPDG for ResidualNucleus to ll-dibar.
    G4QPDGCode naQPDG(22);         // Prototype of QPDG for ResidualNucleus to n+alpha
    G4QPDGCode paQPDG(22);         // Prototype of QPDG for ResidualNucleus to p+alpha
    G4QPDGCode laQPDG(22);         // Prototype of QPDG for ResidualNucleus to l+alpha
    G4QPDGCode aaQPDG(22);         // Prototype of QPDG for ResidualNucleus to alph+alph
    G4QPDGCode dbQPDG(22);         // Prototype of chosen dibaryon QPDG
    G4QPDGCode fQPDG(22);          // Prototype of QPDG of the Second Baryon
    G4double rMass  = 0.;          // Prototype of mass of Residual Nucleus
    G4double eMass  = 0.;          // Prototype of mass of Evaporated Baryon
    G4double fMass  = 0.;          // Prototype of mass of the Second Baryon
#ifdef ppdebug
    G4cout<<"G4QNuc::EvaB:a>2, totM="<<totMass<<" > GSMass="<<GSMass<<",d="<<totMass-GSMass
          <<G4endl;
#endif
    G4double tM2    = totMass*totMass;
    G4double qtM2   = 4*tM2;
    G4double GSResNp= GSMass;     // Prototype of Residual Nuclear Mass for proton
    G4double pExcess= 0.;         // Prototype of excess energy for proton
    G4double aExcess= 0.;         // Prototype of excess energy for alpha
    G4double pp2m   = 0.;         // Prototype of max square momentum for proton
    G4double ap2m   = 0.;         // Prototype of max square momentum for proton
    G4double pBnd   = 0.;         // Binding energy for proton
    G4double aBnd   = 0.;         // Binding energy for proton
    G4bool three=false;           // Prototype of the Flag of b+b+ResNuc decay
    if(Z>0)
    {
      PQPDG=G4QPDGCode(90000000+1000*(1000*S+Z-1)+N);
      GSResNp=PQPDG.GetMass();
      G4double mpls=GSResNp+mProt;
      G4double mmin=GSResNp-mProt;
      pp2m=(tM2-mpls*mpls)*(tM2-mmin*mmin)/qtM2;
      if(pp2m>=0.000001)
	     {
        pFlag=true;
        pBnd=mProt-GSMass+GSResNp;             // Binding energy for proton
        G4double eMax=sqrt(mP2+pp2m);
#ifdef ppdebug
	       G4cout<<"G4QNuc::EvapBaryon:pm="<<eMax+sqrt(pp2m+GSResNp*GSResNp)<<" = M="<<totMass
              <<", sm="<<GSResNp+mProt+PBarr<<",pp2="<<pp2m<<",pB="<<pBnd<<G4endl;
#endif
        pExcess=eMax-mProt+pBnd;               // Max Kin Energy from bottom
	     }
      else pExcess=pBnd;
      if(Z>1)
	     {
        ppQPDG=G4QPDGCode(90000000+1000*(1000*S+Z-2)+N);
        GSResPP=ppQPDG.GetMass();
#ifdef ppdebug
        G4double sm=GSResPP+mProt+mProt+SPPBarr;
	       G4cout<<"G4QNucl::EvapBaryon: ppM="<<GSResPP<<",T="<<sm-GSMass<<",E="<<totMass-sm
              <<",C="<<PBarr<<G4endl;
#endif
        if(GSResPP+mProt+mProt+SPPBarr<totMass) ppFlag=true;
        if(Z>2&&a>3)
        {
          GSRePPP=G4QPDGCode().GetNuclMass(Z-3,N,S);
          if(GSRePPP+mProt+mProt+mProt+SPPPBarr<totMass) pppF=true;
          if(N>1&&a>5)
          {
            paQPDG =G4QPDGCode(90000000+1000*(1000*S+Z-3)+N-2);
            GSResPA=paQPDG.GetMass();
#ifdef ppdebug
            G4double s=GSResPA+mAlph+mProt+SAPBarr;
	           G4cout<<"G4QN::EB:paM="<<GSResPA<<",T="<<s-GSMass<<",E="<<totMass-s<<G4endl;
#endif
            if(GSResPA+mProt+SAPBarr+mAlph<totMass) paFlag=true;
		        }
        }
        if(N>0&&a>3)
        {
          GSReNPP=G4QPDGCode().GetNuclMass(Z-2,N-1,S);
          if(GSReNPP+mProt+mProt+SPPBarr+mNeut<totMass) nppF=true;
		      }
        if(S>0&&a>3)
        {
          GSRePPL=G4QPDGCode().GetNuclMass(Z-2,N,S-1);
          if(GSRePPL+mProt+mProt+SPPBarr+mLamb<totMass) pplF=true;
		      }
        if(N>1&&a>4)
        {
          if(a>6)
          {
            if(S>1)
            {
              GSReLLA=G4QPDGCode().GetNuclMass(Z-2,N-2,S-2);
              if(GSReLLA+mAlph+ABarr+mLamb+mLamb<totMass) llaF=true;
		          }
            if(N>2&&S>0)
            {
              GSReNLA=G4QPDGCode().GetNuclMass(Z-2,N-3,S-1);
              if(GSReNLA+mAlph+ABarr+mNeut+mLamb<totMass) nlaF=true;
		          }
            if(Z>2&&S>0)
            {
              GSRePLA=G4QPDGCode().GetNuclMass(Z-3,N-2,S-1);
              if(GSRePLA+mAlph+SAPBarr+mProt+mLamb<totMass) plaF=true;
		          }
            if(N>3)
            {
              GSReNNA=G4QPDGCode().GetNuclMass(Z-2,N-4,S);
              if(GSReNNA+mAlph+ABarr+mNeut+mNeut<totMass) nnaF=true;
		          }
            if(Z>2&&N>2)
            {
              GSReNPA=G4QPDGCode().GetNuclMass(Z-3,N-3,S);
              if(GSReNPA+mAlph+SAPBarr+mProt+mNeut<totMass) npaF=true;
		          }
            if(N>3)
            {
              GSRePPA=G4QPDGCode().GetNuclMass(Z-4,N-2,S);
              if(GSRePPA+mAlph+SPPABarr+mProt+mProt<totMass) ppaF=true;
		          }
            if(a>9)
            {
              if(Z>3&&N>3&&S>0)
              {
                GSReLAA=G4QPDGCode().GetNuclMass(Z-4,N-4,S-1);
                if(GSReLAA+mLamb+mAlph+mAlph+SAABarr<totMass) laaF=true;
              }
              if(Z>3&&N>4)
              {
                GSReNAA=G4QPDGCode().GetNuclMass(Z-4,N-5,S);
                if(GSReNAA+mNeut+mAlph+mAlph+SAABarr<totMass) naaF=true;
              }
              if(Z>4&&N>3)
              {
                GSRePAA=G4QPDGCode().GetNuclMass(Z-5,N-4,S);
                if(GSRePAA+mProt+mAlph+mAlph+SAABarr<totMass) paaF=true;
              }
              if(a>12&&N>5&&Z>5)
              {
                GSReAAA=G4QPDGCode().GetNuclMass(Z-6,N-6,S);
                if(GSReAAA+mAlph+mAlph+mAlph+SAAABarr<totMass) aaaF=true;
              }
			         }
		        }
          if(N>3&&Z>3&&a>8)
          {
            aaQPDG =G4QPDGCode(90000000+1000*(1000*S+Z-4)+N-4);
            GSResAA=aaQPDG.GetMass();
#ifdef ppdebug
            G4double s=GSResAA+mAlph+mAlph+SAABarr;
	           G4cout<<"G4QNucl::EvapBaryon: a="<<GSResNP<<",T="<<s-GSMass<<",E="<<totMass-s
                  <<",A="<<SAABarr<<G4endl;
#endif
            if(GSResAA+mAlph+mAlph+SAABarr<totMass) aaFlag=true;
		        }
          if(N>2&&a>5)
          {
            naQPDG =G4QPDGCode(90000000+1000*(1000*S+Z-2)+N-3);
            GSResNA=naQPDG.GetMass();
#ifdef ppdebug
            G4double s=GSResNA+mAlph+mNeut;
	           G4cout<<"G4QNucl::EvapBary: M="<<GSResNA<<",T="<<s-GSMass<<",E="<<totMass-s
                  <<",C="<<ABarr<<G4endl;
#endif
            if(GSResNA+mNeut+mAlph+ABarr<totMass) naFlag=true;
		        }
          if(S>0&&a>5)
          {
            laQPDG =G4QPDGCode(90000000+1000*(1000*S+Z-1002)+N-2);
            GSResLA=laQPDG.GetMass();
            if(GSResLA+mLamb+mAlph+ABarr<totMass) laFlag=true;
		        }
          AQPDG =G4QPDGCode(90000000+1000*(1000*S+Z-2)+N-2);
          GSResNa=AQPDG.GetMass();
          mpls=GSResNa+mAlph;
          mmin=GSResNa-mAlph;
          ap2m=(tM2-mpls*mpls)*(tM2-mmin*mmin)/qtM2;
          if(ap2m>=0.000001)
	         {
            aFlag=true;
            aBnd=mAlph-GSMass+GSResNa;           // Binding energy for ALPHA
            G4double eMax=sqrt(mA2+ap2m);
#ifdef ppdebug
	           G4cout<<"G4QNuc::EvapBar:m="<<eMax+sqrt(ap2m+GSResNa*GSResNa)<<" = M="<<totMass
                  <<", sm="<<GSResNp+mProt+PBarr<<",pp2="<<pp2m<<",pB="<<pBnd<<G4endl;
#endif
            aExcess=eMax-mAlph+aBnd;             // Max Kin Energy from bottom
	         }
          else aExcess=pBnd;
		      }
	     }
      if(N>0)
	     {
        if(Z>0)
        {
          npQPDG=G4QPDGCode(90000000+1000*(1000*S+Z-1)+N-1);
          GSResNP=npQPDG.GetMass();
#ifdef ppdebug
          G4double s=GSResNP+mNeut+mProt;
	         G4cout<<"G4QNucl::EvapBaryon: npM="<<GSResNP<<",T="<<s-GSMass<<",E="<<totMass-s
                <<",C="<<PBarr<<G4endl;
#endif
          if(GSResNP+mNeut+mProt+PBarr<totMass) npFlag=true;
		      }
        if(N>1)
        {
          GSReNNP=G4QPDGCode().GetNuclMass(Z-1,N-2,S);
          if(GSReNNP+mProt+PBarr+mNeut+mNeut<totMass) nnpF=true;
		      }
        if(S>0)
        {
          GSReNPL=G4QPDGCode().GetNuclMass(Z-1,N-1,S-1);
          if(GSReNPL+mProt+PBarr+mNeut+mLamb<totMass) nplF=true;
		      }
	     }
      if(S>0)
	     {
        if(Z>0)
        {
          plQPDG=G4QPDGCode(90000000+1000*(1000*(S-1)+Z-1)+N);
          GSResPL=plQPDG.GetMass();
          if(GSResPL+mProt+PBarr+mLamb<totMass) plFlag=true;
		      }
        if(S>1)
        {
          GSRePLL=G4QPDGCode().GetNuclMass(Z-1,N,S-2);
          if(GSRePLL+mProt+PBarr+mLamb+mLamb<totMass) pllF=true;
		      }
	     }
	   }
    G4double GSResNn= GSMass;         // Prototype of Residual Nuclear Mass for neutron
    G4double nExcess= 0.;             // Prototype of excess energy for neutron
    G4double np2m   = 0.;             // Prototype of max square momentum for neutron
    G4double nBnd   = 0.;             // Binding energy for neutron
    if(N>0)
    {
      NQPDG=G4QPDGCode(90000000+1000*(1000*S+Z)+N-1);
      GSResNn=NQPDG.GetMass();
#ifdef ppdebug
	     G4cout<<"G4QNucleus::EvapBaryon: M(A-N)="<<GSResNn<<",Z="<<Z
            <<",N="<<N<<",S="<<S<<G4endl;
#endif
      G4double mpls=GSResNn+mNeut;
      G4double mmin=GSResNn-mNeut;
      np2m=(tM2-mpls*mpls)*(tM2-mmin*mmin)/qtM2;
      if(np2m>=0.000001)
      {
        nFlag=true;
        nBnd=mNeut-GSMass+GSResNn;    // Binding energy for neutron
        G4double eMax=sqrt(mN2+np2m);
#ifdef ppdebug
	    G4cout<<"G4QNucl::EvapBaryon: nm="<<eMax+sqrt(np2m+GSResNn*GSResNn)<<" = M="<<totMass
              <<", sm="<<GSResNn+mNeut<<",np2="<<np2m<<",nB="<<nBnd<<G4endl;
#endif
        nExcess=eMax-mNeut+nBnd;
	     }
      else nExcess=nBnd;
      if(N>1)
	     {
        nnQPDG=G4QPDGCode(90000000+1000*(1000*S+Z)+N-2);
        GSResNN=nnQPDG.GetMass();
        if(GSResNN+mNeut+mNeut<totMass) nnFlag=true;
        if(N>2)
        {
          GSReNNN=G4QPDGCode().GetNuclMass(Z,N-3,S);
          if(GSReNNN+mNeut*3<totMass) nnnF=true;
		      }
        if(S>0)
        {
          GSReNNL=G4QPDGCode().GetNuclMass(Z,N-2,S-1);
          if(GSReNNL+mNeut+mNeut+mLamb<totMass) nnlF=true;
		      }
	     }
      if(S>0)
	     {
        nlQPDG=G4QPDGCode(90000000+1000*(1000*(S-1)+Z)+N-1);
        GSResNL=nlQPDG.GetMass();
        if(GSResNL+mNeut+mLamb<totMass) nlFlag=true;
        if(S>1)
        {
          GSReNLL=G4QPDGCode().GetNuclMass(Z,N-1,S-2);
          if(GSReNLL+mNeut+mLamb+mLamb<totMass) nllF=true;
		      }
	     }
	   }
    G4double GSResNl= GSMass;         // Prototype of Residual Nuclear Mass for Lambda
    G4double lExcess= 0.;             // Prototype of excess energy for Lambda
    G4double lp2m   = 0.;             // Prototype of max square momentum for lambda
    G4double lBnd   = 0.;             // Binding energy for lambda
    if(S>0)
    {
      LQPDG=G4QPDGCode(90000000+1000*(1000*(S-1)+Z)+N);
      GSResNl=LQPDG.GetMass();
      G4double mpls=GSResNl+mLamb;
      G4double mmin=GSResNl-mLamb;
      lp2m=(tM2-mpls*mpls)*(tM2-mmin*mmin)/qtM2;
      if(lp2m>=0.000001)
      {
        lFlag=true;
        lBnd=mLamb-GSMass+GSResNl;    // Binding energy for lambda
        G4double eMax=sqrt(mL2+lp2m);
#ifdef ppdebug
	    G4cout<<"G4QNucl::EvapBaryon:lm="<<eMax+sqrt(lp2m+GSResNl*GSResNl)<<" = M="<<totMass
              <<", sm="<<GSResNl+mLamb<<",lp2="<<lp2m<<",lB="<<lBnd<<G4endl;
#endif
        lExcess=eMax-mLamb+lBnd;
	     }
      else lExcess=lBnd;
      if(S>1)
	     {
        llQPDG=G4QPDGCode(90000000+1000*(1000*(S-2)+Z)+N);
        GSResLL=llQPDG.GetMass();
        if(GSResLL+mLamb+mLamb<totMass) llFlag=true;
        if(S>2)
        {
          GSReLLL=G4QPDGCode().GetNuclMass(Z,N,S-3);
          if(GSReLLL+mLamb*3<totMass) lllF=true;
		      }
	     }
	   }
    G4bool nSecF = nnFlag||npFlag||nlFlag||naFlag; // Pos of second radiation after neutron
    G4bool pSecF = npFlag||ppFlag||plFlag||paFlag; // Pos of second radiation after proton
    G4bool lSecF = nlFlag||plFlag||llFlag||laFlag; // Pos of second radiation after lambda
    G4bool aSecF = naFlag||paFlag||laFlag||aaFlag; // Pos of second radiation after alpha
    //G4bool nTrF=nnnF||nnpF||nppF||nnlF||nplF||nllF; //Pos of 3-d baryon radiation after n
    //G4bool pTrF=nnpF||nppF||pppF||nplF||pplF||pllF; //Pos of 3-d baryon radiation after p
    //G4bool lTrF=nnlF||nplF||pplF||nllF||pllF||lllF; //Pos of 3-d baryon radiation after l
    //G4bool aTrF=nnaF||npaF||ppaF||nlaF||plaF||llaF; //Pos of 3-d baryon radiation after a
    G4bool secB  = nSecF||pSecF||lSecF||aSecF; // Possibili to decay in TwoBaryons (Alphas)
    //G4bool thdB  = nTrF||pTrF||lTrF||aTrF||naaF||paaF||laaF||aaaF;// Pos to radiate three
#ifdef ppdebug
	   G4cout<<"G4QNucl::EvapBary:n="<<nSecF<<",p="<<pSecF<<",l="<<lSecF<<",a="<<aSecF<<",nn="
          <<nnFlag<<", np="<<npFlag<<",pp="<<ppFlag<<",pa="<<paFlag<<",na="<<naFlag<<",aa="
          <<aaFlag<<G4endl;
#endif
    G4QPDGCode bQPDG;
    G4QPDGCode rQPDG;
	   if(secB)                            // Decay in two baryons is possible
	   //if(thdB)                            //@@CHECK@@ Decay in three baryons is possible
    {
      if(!nSecF) nFlag=false;
      if(!pSecF) pFlag=false;
      if(!lSecF) lFlag=false;
      if(!aSecF) aFlag=false;
#ifdef ppdebug
	     G4cout<<"G4QNuc::EB:nF="<<nFlag<<",pF="<<pFlag<<",lF="<<lFlag<<",aF="<<aFlag<<G4endl;
#endif
      G4double maxE=0.;                          // Prototype for maximum energy
      if(nFlag&&nExcess>maxE) maxE=nExcess;
      if(pFlag&&pExcess>maxE) maxE=pExcess;
      if(lFlag&&lExcess>maxE) maxE=lExcess;
      if(lFlag&&aExcess>maxE) maxE=aExcess;
      G4double pMin=pBnd;                        // Binding energy for proton
      if(pFlag)pMin+= PBarr;                     // Add Coulomb Barrier for protons
      G4double nMin=nBnd;                        // Binding energy for neutron
      G4double lMin=lBnd;                        // Binding energy for Lambda
      G4double aMin=aBnd;                        // Binding energy for alpha
      if(aFlag)aMin+= ABarr;                     // Add Coulomb Barrier for alpha
      G4double minE=GSMass;                      // Prototype for mimimum energy
      if(nFlag&&nMin<minE) minE=nMin;
      if(pFlag&&pMin<minE) minE=pMin;
      if(lFlag&&lMin<minE) minE=lMin;
      if(alpha&&aFlag&&aMin<minE) minE=aMin;

#ifdef ppdebug
      G4cout<<"G4QNucleus::EvapBaryon: nE="<<nExcess<<">"<<nMin<<",pE="<<pExcess<<">"<<pMin
            <<",sE="<<lExcess<<">"<<lMin<<",E="<<aExcess<<">"<<aMin<<",miE="<<minE<<"<maE="
            <<maxE<<G4endl;
#endif
      // @@ Here one can put a condition for the Baryon Gun
      G4int    cntr= 0;
      //G4int    cntm= 27;
      //G4int    cntm= 72;                 // Important difference !!DOn't change
      //G4int    cntm= 80;                 // Important difference !!DOn'tChange"IsoNuclei"
      G4int    cntm= 90;                 // Important difference !!DOn'tChange "Lept/Hyper"
      if( (pFlag&&pExcess>pMin || nFlag&&nExcess>nMin || lFlag&&lExcess>lMin ||
           aFlag&&aExcess>aMin) && minE<maxE)
      {
        G4double mi=uWell+minE;          // Minimum Kinetic Energy for minimal nucleon
        G4double mm=uWell+maxE;          // Personal maximum for Kinetic Energy
        G4double ma=uWell*a+maxE;        // Total Kinetic Energy of baryons (@@alphas?)
        if(mi<0.)
	       {
          uW-=mi;
          mm-=mi;
          mi=0.;
	       }
        G4bool good=true;
        if(ma<mm)
        {
          ma=mm;
          good=false;
        }
#ifdef ppdebug
	       G4cout<<"G4QNuc::EvapBary:iE="<<minE<<",aE="<<maxE<<",mi="<<mi<<",mm="<<mm<<",ma="
              <<ma<<G4endl;
#endif
        G4double xMi=mi/ma;                     // Minimal value of x
        G4double xMm=mm/ma;                     // Personal maximum x
        //G4double xCa=maSht-coSht*log(a);        // Maximal value of x (approximation)
        //G4double xMa=xCa;                       // Maximal value of x
        //if(xMm<xMa) xMa=xMm;
        G4double xMa=xMm;
        if(xMa>1.)xMa=1.;
        if(xMi<0.)xMi=0.;
        if(xMi>xMa)
        {
	         G4cerr<<"***G4QNucleus::EvapBaryon: M="<<mm/ma<<",xi="<<xMi<<",xa="<<xMa<<G4endl;
          return false;
        }
        xMi=sqrt(xMi);                          // @@ ?
        xMa=sqrt(xMa);                          // @@ ?
#ifdef ppdebug
	       G4cout<<"G4QNuc:EvapBaryon:mi="<<mi<<",ma="<<ma<<", xi="<<xMi<<",xa="<<xMa<<G4endl;
#endif
        G4double powr=1.5*a1;                   // Power for low & up limits
        G4double revP=1./powr;                  // Reversed power for randomization
#ifdef ppdebug
        G4cout<<"G4QNucleus::EvaporateBaryon: Power="<<powr<<",RevPower="<<revP<<G4endl;
#endif
        G4double minR=pow(1.-xMa*xMa,powr);    // Look on @@ ? (up)
        G4double maxR=pow(1.-xMi*xMi,powr);
#ifdef ppdebug
        G4cout<<"G4QNucleus::EvaporateBaryon: miR="<<minR<<", maR="<<maxR<<G4endl;
#endif
        G4bool   cond=true;
        G4int    PDG = 0;
        G4double tk  = 0.;                      // Kinetic energy over the well
        while(cond&&cntr<cntm)
        {
          G4double R = minR+(maxR-minR)*G4UniformRand();
          //if(!good)R = maxR;
          G4double x2= 1.-pow(R,revP);
          G4double x = sqrt(x2);
          if(x<xMi||x>xMa)
		        {
#ifdef ppdebug
            G4cerr<<"**G4QNucl::EvapB:R="<<R<<",xi="<<xMi<<" < "<<x<<" < xa="<<xMa<<G4endl;
#endif
            if(x<xMi) x=xMi;
            else      x=xMa;
            x2 = x*x;
		        }
          G4double rn=G4UniformRand();
          //if(rn<x/xMa||!good)
          if(rn<x/xMa)                                 // Randomization cut
          {
            tk= ma*x2-uW;                              // Kinetic energy of the fragment
            G4double psum =0.;
            G4double zCBPP=0.;                         // Probabylity for a proton
#ifdef ppdebug
			         G4cout<<"G4QNuc::EvapB:t="<<tk<<",pM="<<pMin<<",pB="<<pBnd<<",n="<<nMin<<",a="
                  <<aMin<<G4endl;
#endif
            if(pFlag&&tk>pMin)
            {
              G4double kin=tk-pBnd;
              //if(barf) kin-=PBarr; //@@ This is a mistake
#ifdef ppdebug
			           G4cout<<"G4QN::EB:Proton="<<kin<<",CB="<<PBarr<<",B="<<pBnd<<",M="<<pMin
                    <<",p="<<CoulBarPenProb(PBarr,kin,1,1)<<G4endl;
#endif
              zCBPP=Z*CoulBarPenProb(PBarr,kin,1,1)*sqrt(kin);
			         }
            psum+=zCBPP;
            G4double nCBPP=0.;                       // Probability for a neutron (=> p+n)
            if(nFlag&&tk>nMin)
            {
              G4double kin=tk-nBnd;
#ifdef ppdebug
			           G4cout<<"G4QN::EB:Neutron="<<kin<<",p="<<CoulBarPenProb(0.,kin,0,1)<<G4endl;
#endif
              nCBPP=N*CoulBarPenProb(0.,kin,0,1)*sqrt(kin);
            }
            psum+=nCBPP;
            nCBPP+=zCBPP;
            G4double lCBPP=0.;                       // Probability for a lambda (=> p+n+l)
            if(lFlag&&tk>lMin)
            {
              G4double kin=tk-lBnd;
#ifdef ppdebug
			           G4cout<<"G4QN::EB:Lambda="<<kin<<",p="<<CoulBarPenProb(0,kin,0,1)<<G4endl;
#endif
              lCBPP=S*CoulBarPenProb(0.,kin,0,1)*sqrt(kin);
			         }
            psum+=lCBPP;
            lCBPP+=nCBPP;
			         if(alpha&&aFlag&&tk>aMin)
            {
              G4double kin=tk-aBnd;
              //if(barf) kin-=ABarr; //@@ This is a mistake
#ifdef ppdebug
			           G4cout<<"G4QN::EB:Alpha="<<kin<<",CB="<<ABarr<<",p="
                    <<CoulBarPenProb(ABarr,kin,2,4)<<G4endl;
#endif
              psum+=CoulBarPenProb(ABarr,kin,2,4)*sqrt(kin)*alpha*Z*(Z-1)*N*(N-1)
                                                 *6/a1/(a-2)/(a-3);
			         }
            G4double r = psum*G4UniformRand();
#ifdef ppdebug
			         G4cout<<"G4QNuc::EvapB:"<<r<<",p="<<zCBPP<<",pn="<<nCBPP<<",pnl="<<lCBPP<<",t="
                  <<psum<<G4endl;
#endif
            cond = false;
            if     (r&&r>lCBPP)
		          {
#ifdef ppdebug
			           G4cout<<"G4QNuc::EvaB:ALPHA is selected for evap, r="<<r<<">"<<lCBPP<<G4endl;
#endif
              PDG=aPDG;
		          }
            else if(r&&r>nCBPP&&r<=lCBPP)
            {
#ifdef ppdebug
			           G4cout<<"G4QNuc::EvaB:LAMBDA is selected for evap,r="<<r<<"<"<<lCBPP<<G4endl;
#endif
              PDG=lPDG;
			         }
            else if(r&&r>zCBPP&&r<=nCBPP)
            {
#ifdef ppdebug
			           G4cout<<"G4QNuc::EvaBar: N is selected for evapor,r="<<r<<"<"<<nCBPP<<G4endl;
#endif
              PDG=nPDG;
			         }
            else if(r&&r<=zCBPP)
            {
#ifdef ppdebug
			           G4cout<<"G4QNuc::EvaBar: P is selected for evapor,r="<<r<<"<"<<zCBPP<<G4endl;
#endif
              PDG=pPDG;
			         }
            else cond=true;
		        }
#ifdef ppdebug
          G4cout<<"G4QNuc::EvapBar:c="<<cond<<",x="<<x<<",cnt="<<cntr<<",R="<<R<<",ma="<<ma
                <<",rn="<<rn<<"<r="<<x/xMa<<",tk="<<tk<<",ni="<<nMin<<",pi="<<pMin<<G4endl;
#endif
          cntr++;
	       }
        if(cntr<cntm)                       // => Succeeded to find the evaporation channel
	       {
          G4double p2=0.;
          if     (PDG==aPDG)
          {
            tk-=aBnd-mAlph;                 // Pays for binding and convert to total energy
            p2=tk*tk-mA2;
            if(p2>ap2m)
            {
              p2=ap2m;
              tk=sqrt(p2+mA2);
		          }
            eMass=mAlph;
            bQPDG=aQPDG;
            rQPDG=AQPDG;
	         }
          else if(PDG==pPDG)
          {
            tk-=pBnd-mProt;                 // Pays for binding and convert to total energy
            p2=tk*tk-mP2;
            if(p2>pp2m)
            {
              p2=pp2m;
              tk=sqrt(p2+mP2);
		          }
            eMass=mProt;
            bQPDG=pQPDG;
            rQPDG=PQPDG;
	         }
          else if(PDG==nPDG)
          {
            tk-=nBnd-mNeut;                 // Pays for binding and convert to total energy
            p2=tk*tk-mN2;
#ifdef ppdebug
            G4cout<<"G4QNucleus::EvaporateBaryon:np2="<<p2<<",np2m="<<np2m<<G4endl;
#endif
            if(p2>np2m)
            {
              p2=np2m;
              tk=sqrt(p2+mN2);
		          }
            eMass=mNeut;
            bQPDG=nQPDG;
            rQPDG=NQPDG;
	         }
          else if(PDG==lPDG)
          {
            tk-=lBnd-mLamb;                 // Pays for binding and convert to total energy
            p2=tk*tk-mL2;
            if(p2>lp2m)
            {
              p2=lp2m;
              tk=sqrt(p2+mL2);
		          }
            eMass=mLamb;
            bQPDG=lQPDG;
            rQPDG=LQPDG;
	         }
          else G4cerr<<"***G4QNucleus::EvaporateBaryon: PDG="<<PDG<<G4endl;
          G4double rEn=totMass-tk;
          rMass=sqrt(rEn*rEn-p2);                  // Mass of Residual Nucleus
          // Find out if the ResidualNucleus is below of the SecondBaryonDecayLimit
          //@@ Calculate it depending on PDG !!!!!!!
          G4bool nnCond=!nnFlag||nnFlag&&GSResNN+mNeut>rMass;
          G4bool npCond=!npFlag||npFlag&&GSResNP+mProt+PBarr>rMass;
          G4bool nlCond=!nlFlag||nlFlag&&GSResNL+mLamb>rMass;
          G4bool naCond=!naFlag||naFlag&&GSResNA+mAlph+ABarr>rMass;
          G4bool pnCond=!npFlag||npFlag&&GSResNP+mNeut>rMass;
          if(barf) pnCond=!npFlag||npFlag&&GSResNP+mNeut+PBarr>rMass;
          G4bool ppCond=!ppFlag||ppFlag&&GSResPP+mProt+PPBarr>rMass;
          if(barf) ppCond=!ppFlag||ppFlag&&GSResPP+mProt+SPPBarr>rMass;
          G4bool plCond=!plFlag||plFlag&&GSResPL+mLamb>rMass;
          if(barf) plCond=!plFlag||plFlag&&GSResPL+mLamb+PBarr>rMass;
          G4bool paCond=!paFlag||paFlag&&GSResPA+mAlph+APBarr>rMass;
          if(barf) paCond=!paFlag||paFlag&&GSResPA+mAlph+SAPBarr>rMass;
          G4bool lnCond=!nlFlag||nlFlag&&GSResNL+mNeut>rMass;
          G4bool lpCond=!plFlag||plFlag&&GSResPL+mProt+PBarr>rMass;
          G4bool llCond=!llFlag||llFlag&&GSResLL+mLamb>rMass;
          G4bool laCond=!laFlag||laFlag&&GSResLA+mAlph+ABarr>rMass;
          G4bool anCond=!naFlag||naFlag&&GSResNA+mNeut>rMass;
          if(barf) anCond=!naFlag||naFlag&&GSResNA+mNeut+ABarr>rMass;
          G4bool apCond=!paFlag||paFlag&&GSResPA+mProt+PABarr>rMass;
          if(barf) apCond=!paFlag||paFlag&&GSResPA+mProt+SAPBarr>rMass;
          G4bool alCond=!laFlag||laFlag&&GSResLA+mLamb>rMass;
          if(barf) alCond=!laFlag||laFlag&&GSResLA+mLamb+ABarr>rMass;
          G4bool aaCond=!aaFlag||aaFlag&&GSResAA+mAlph+AABarr>rMass;
          if(barf) aaCond=!aaFlag||aaFlag&&GSResAA+mAlph+SAABarr>rMass;
#ifdef ppdebug
		        G4cout<<"G4QNucl::EvaB:"<<PDG<<", E="<<tk<<", rM="<<rMass<<", ";
          if(PDG==pPDG)      G4cout<<"PN="<<GSResNP+mNeut<<"("<<pnCond<<"),PP="
                                   <<GSResPP+mProt+PPBarr<<"("<<ppCond<<"),PL="
                                   <<GSResPL+mLamb<<"("<<plCond<<"),PA="
                                   <<GSResPA+mAlph+APBarr<<"("<<paCond;
          else if(PDG==nPDG) G4cout<<"NN="<<GSResNN+mNeut<<"("<<nnCond<<"),NP="
                                   <<GSResNP+mProt+PBarr<<"("<<npCond<<"),NL="
                                   <<GSResNL+mLamb<<"("<<nlCond<<"),NA="
                                   <<GSResNA+mAlph+ABarr<<"("<<naCond;
          else if(PDG==lPDG) G4cout<<"LN="<<GSResNL+mNeut<<"("<<lnCond<<"),LP="
                                   <<GSResPL+mProt+PBarr<<"("<<lpCond<<"),LL="
                                   <<GSResLL+mLamb<<"("<<llCond<<"),LA="
                                   <<GSResLA+mAlph+ABarr<<"("<<laCond;
          else if(PDG==aPDG) G4cout<<"AN="<<GSResNA+mNeut<<"("<<anCond<<"),AP="
                                   <<GSResPA+mProt+PABarr<<"("<<apCond<<"),AL="
                                   <<GSResLA+mLamb<<"("<<alCond<<"),AA="
                                   <<GSResAA+mAlph+AABarr<<"("<<aaCond;
          G4cout<<")"<<G4endl;
#endif
          three=false;                               // Flag of b+b+ResNuc decay
          //if(3>2)three=false;                        // @@@@@@@@@@@@@@@@@@
          //else if(PDG==pPDG&&(pnCond&&ppCond&&plCond&&paCond)) // @@@@@@@@@@@@@@@@@@@
		        if(PDG==pPDG&&(pnCond&&ppCond&&plCond&&paCond))//p+RN decay, p+b+RN dec is closed
		        {
#ifdef ppdebug
			         G4cout<<"G4QN::EB:*p*: n="<<pnCond<<",p="<<ppCond<<",l="<<plCond<<",a="<<paCond
                  <<G4endl;
#endif
            fMass=mProt;
            fQPDG=pQPDG;
            G4double nLim=0.;
            if(N&&GSResNP!=GSMass&&fMass+PBarr+mNeut+GSResNP<totMass)
			         {
			           if(barf) nLim+=(N+N)*pow(totMass-mNeut-mProt-PBarr-GSResNP,2);
              else     nLim+=(N+N)*pow(totMass-mNeut-mProt-GSResNP,2);
			         }
            G4double zLim=nLim;
            if(Z>1&&GSResPP!=GSMass&&fMass+mProt+SPPBarr+GSResPP<totMass)
			         {
			           if(barf) zLim+=(Z-1)*pow(totMass-mProt-mProt-SPPBarr-GSResPP,2);
              else     zLim+=(Z-1)*pow(totMass-mProt-mProt-GSResPP,2);
			         }
            G4double sLim=zLim;
            if(S&&GSResPL!=GSMass&&fMass+PBarr+mLamb+GSResPL<totMass)
			         {
			           if(barf) sLim+=(S+S)*pow(totMass-mProt-mLamb-PBarr-GSResPL,2);
              else     sLim+=(S+S)*pow(totMass-mProt-mLamb-GSResPL,2);
			         }
            G4double aLim=sLim;
            if(alpha&&Z>2&&N>1&&a>4&&GSResPL!=GSMass&&fMass+SAPBarr+mAlph+GSResPA<totMass)
			         {
			           if(barf) aLim+=pow(totMass-mProt-mAlph-SAPBarr-GSResPA,2)*alpha*(Z-1)*(Z-2)*N
                             *(N-1)*12/(a-2)/(a-3)/(a-4);
              else     aLim+=pow(totMass-mProt-mAlph-GSResPA,2)*alpha*(Z-1)*(Z-2)*N*(N-1)
                             *12/(a-2)/(a-3)/(a-4);
			         }
            G4double r = aLim*G4UniformRand();
#ifdef ppdebug
			         G4cout<<"G4QNuc::EvaB:p, r="<<r<<",n="<<nLim<<",z="<<zLim<<",s="<<sLim<<",a="
                  <<aLim<<G4endl;
#endif
            three=true;                               // Flag of b+b+ResNuc decay
            if(!aLim) three=false;
            else if(r>sLim)
		          {
              eMass = mAlph;
              dbQPDG= PAQPDG;
              rMass = GSResPA;
              rQPDG = paQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: P+A"<<G4endl;
#endif
		          }
            else if(zLim<sLim&&r>zLim&&r<=sLim)
		          {
              eMass = mLamb;
              dbQPDG= PLQPDG;
              rMass = GSResPL;
              rQPDG = plQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: P+L"<<G4endl;
#endif
		          }
            else if(nLim<zLim&&r>nLim&&r<=zLim)
	           {
              eMass = mProt;
              dbQPDG= PPQPDG;
              rMass = GSResPP;
              rQPDG = ppQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: P+P"<<G4endl;
#endif
		          }
            else if(r<=nLim)
	           {
              eMass = mNeut;
              dbQPDG= NPQPDG;
              rMass = GSResNP;
              rQPDG = npQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: P+N"<<G4endl;
#endif
   	        }
            else three=false;
		        }
          else if(PDG==nPDG&&(nnCond&&npCond&&nlCond&&naCond)) // n+b+RN decay can't happen
		        { //@@ Take into account Coulomb Barier Penetration Probability
#ifdef ppdebug
			         G4cout<<"G4QN::EB:*n*: n="<<nnCond<<",p="<<npCond<<",l="<<nlCond<<",a="<<naCond
                  <<G4endl;
#endif
            fMass=mNeut;
            fQPDG=nQPDG;
            G4double nLim=0.;
            if(N>1&&GSResNN!=GSMass&&fMass+mNeut+GSResNN<totMass)
              nLim+=(N-1)*pow(totMass-mNeut-mNeut-GSResNN,2);
            G4double zLim=nLim;
            if(Z&&GSResNP!=GSMass&&fMass+mProt+PBarr+GSResNP<totMass)
			         {
			           if(barf) zLim+=(Z+Z)*pow(totMass-mNeut-mProt-PBarr-GSResNP,2);
              else     zLim+=(Z+Z)*pow(totMass-mNeut-mProt-GSResNP,2);
			         }
            G4double sLim=zLim;
            if(S&&GSResNL!=GSMass&&fMass+mLamb+GSResNL<totMass)
              sLim+=(S+S)*pow(totMass-mNeut-mLamb-GSResNL,2);
            G4double aLim=sLim;
            if(alpha&&Z>1&&N>2&&a>4&&GSResNA!=GSMass&&fMass+mAlph+ABarr+GSResNA<totMass)
			         {
			           if(barf) aLim+=pow(totMass-mNeut-mAlph-ABarr-GSResNA,2)*
                             alpha*Z*(Z-1)*(N-1)*(N-2)*12/(a-2)/(a-3)/(a-4);
              else     aLim+=pow(totMass-mNeut-mAlph-GSResNA,2)*
                             alpha*Z*(Z-1)*(N-1)*(N-2)*12/(a-2)/(a-3)/(a-4);
			         }
            G4double r = aLim*G4UniformRand();
#ifdef ppdebug
			         G4cout<<"G4QN::EB:n, r="<<r<<",n="<<nLim<<",z="<<zLim<<",s="<<sLim<<",a="<<aLim
                  <<G4endl;
#endif
            three=true;                               // Flag of b+b+ResNuc decay
            if(!aLim) three=false;
            else if(r>sLim)
		          {
              eMass = mAlph;
              dbQPDG= NAQPDG;
              rMass = GSResNA;
              rQPDG = naQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: N+A"<<G4endl;
#endif
		          }
            else if(zLim<sLim&&r>zLim&&r<=sLim)
		          {
              eMass = mLamb;
              dbQPDG= NLQPDG;
              rMass = GSResNL;
              rQPDG = nlQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: N+L"<<G4endl;
#endif
		          }
            else if(nLim<zLim&&r>nLim&&r<=zLim)
	           {
              eMass = mProt;
              dbQPDG= NPQPDG;
              rMass = GSResNP;
              rQPDG = npQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: N+P"<<G4endl;
#endif
		          }
            else if(r<=nLim)
	           {
              eMass = mNeut;
              dbQPDG= NNQPDG;
              rMass = GSResNN;
              rQPDG = nnQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: N+N"<<G4endl;
#endif
   	        }     
            else three=false;
		        }
          else if(PDG==lPDG&&(lnCond&&lpCond&&llCond&&laCond)) // l+b+RN decay can't happen
		        { //@@ Take into account Coulomb Barier Penetration Probability
#ifdef ppdebug
			         G4cout<<"G4QN::EB:*l*: n="<<lnCond<<",p="<<lpCond<<",l="<<llCond<<",a="<<laCond
                  <<G4endl;
#endif
            fMass=mLamb;
            fQPDG=lQPDG;
            G4double nLim=0.;
            if(N&&GSResNL!=GSMass&&fMass+mNeut+GSResNL<totMass)
              nLim+=(N+N)*pow(totMass-mNeut-mLamb-GSResNL,2);
            G4double zLim=nLim;
            if(Z&&GSResPL!=GSMass&&fMass+mProt+PBarr+GSResPL<totMass)
			         {
			           if(barf) zLim+=(Z+Z)*pow(totMass-mProt-mLamb-PBarr-GSResPL,2);
              else zLim+=(Z+Z)*pow(totMass-mProt-mLamb-GSResPL,2);
			         }
            G4double sLim=zLim;
            if(S>1&&GSResLL!=GSMass&&fMass+mLamb+GSResLL<totMass)
              sLim+=(S-1)*pow(totMass-mLamb-mLamb-GSResLL,2);
            G4double aLim=sLim;
            if(alpha&&Z>1&&N>1&&a>4&&GSResLA!=GSMass&&fMass+mAlph+ABarr+GSResLA<totMass)
			         {
			           if(barf) aLim+=pow(totMass-mLamb-mAlph-ABarr-GSResLA,2)*
                             alpha*Z*(Z-1)*N*(N-1)*12/(a-2)/(a-3)/(a-4);
              else     aLim+=pow(totMass-mLamb-mAlph-GSResLA,2)*
                             alpha*Z*(Z-1)*N*(N-1)*12/(a-2)/(a-3)/(a-4);
			         }
            G4double r = aLim*G4UniformRand();
#ifdef ppdebug
			         G4cout<<"G4QN::EB:l, r="<<r<<",n="<<nLim<<",z="<<zLim<<",s="<<sLim<<",a="<<aLim
                  <<G4endl;
#endif
            three=true;                               // Flag of b+b+ResNuc decay
            if(!aLim) three=false;
            else if(r>sLim)
		          {
              eMass = mAlph;
              dbQPDG= LAQPDG;
              rMass = GSResLA;
              rQPDG = laQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: L+A"<<G4endl;
#endif
		          }
            else if(zLim<sLim&&r>zLim&&r<=sLim)
		          {
              eMass = mLamb;
              dbQPDG= LLQPDG;
              rMass = GSResLL;
              rQPDG = llQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: L+L"<<G4endl;
#endif
		          }
            else if(nLim<zLim&&r>nLim&&r<=zLim)
	           {
              eMass = mProt;
              dbQPDG= PLQPDG;
              rMass = GSResPL;
              rQPDG = plQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: L+P"<<G4endl;
#endif
		          }
            else if(r<=nLim)
	           {
              eMass = mNeut;
              dbQPDG= NLQPDG;
              rMass = GSResNL;
              rQPDG = nlQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: L+N"<<G4endl;
#endif
   	        }
            else three=false;
		        }
          else if(PDG==aPDG&&(anCond&&apCond&&alCond&&aaCond)) // a+b+RN decay can't happen
		        { //@@ Take into account Coulomb Barier Penetration Probability
#ifdef ppdebug
			         G4cout<<"G4QN::EB:*a*: n="<<anCond<<",p="<<apCond<<",l="<<alCond<<",a="<<aaCond
                  <<G4endl;
#endif
            fMass=mAlph;
            fQPDG=aQPDG;
            G4double nLim=0.;
            if(N>2&&GSResNA!=GSMass&&fMass+mNeut+ABarr+GSResNA<totMass)
			         {
			           if(barf) nLim+=(N-2)*pow(totMass-mNeut-mAlph-ABarr-GSResNA,2);
              else     nLim+=(N-2)*pow(totMass-mNeut-mAlph-GSResNA,2);
			         }
            G4double zLim=nLim;
            if(Z>2&&GSResPA!=GSMass&&fMass+mProt+SAPBarr+GSResPA<totMass)
			         {
			           if(barf) zLim+=(Z-2)*pow(totMass-mProt-mAlph-SAPBarr-GSResPA,2);
              else     zLim+=(Z-2)*pow(totMass-mProt-mAlph-GSResPA,2);
			         }
            G4double sLim=zLim;
            if(S&&GSResLA!=GSMass&&fMass+mLamb+ABarr+GSResLA<totMass)
			         {
			           if(barf) sLim+=S*pow(totMass-mLamb-mAlph-ABarr-GSResLA,2);
              else     sLim+=S*pow(totMass-mLamb-mAlph-GSResLA,2);
			         }
            G4double aLim=sLim;
            if(alpha&&Z>3&&N>3&&a>7&&GSResAA!=GSMass&&fMass+mAlph+SAABarr+GSResAA<totMass)
			         {
			           if(barf) aLim+=pow(totMass-mAlph-mAlph-SAABarr-GSResAA,2)*
                             alpha*(Z-2)*(Z-3)*(N-2)*(N-3)*12/(a-5)/(a-6)/(a-7);
              else     aLim+=pow(totMass-mAlph-mAlph-GSResAA,2)*
                             alpha*(Z-2)*(Z-3)*(N-2)*(N-3)*12/(a-5)/(a-6)/(a-7);
			         }
            G4double r = aLim*G4UniformRand();
#ifdef ppdebug
			         G4cout<<"G4QN::EB:a, r="<<r<<",n="<<nLim<<",z="<<zLim<<",s="<<sLim<<",a="<<aLim
                  <<G4endl;
#endif
            three=true;                               // Flag of b+b+ResNuc decay
            if(!aLim) three=false;
            else if(r>sLim)
		          {
              eMass = mAlph;
              dbQPDG= AAQPDG;
              rMass = GSResAA;
              rQPDG = aaQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: A+A"<<G4endl;
#endif
		          }
            else if(zLim<sLim&&r>zLim&&r<=sLim)
		          {
              eMass = mLamb;
              dbQPDG= LAQPDG;
              rMass = GSResLA;
              rQPDG = laQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: A+L"<<G4endl;
#endif
		          }
            else if(nLim<zLim&&r>nLim&&r<=zLim)
	           {
              eMass = mProt;
              dbQPDG= PAQPDG;
              rMass = GSResPA;
              rQPDG = paQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: A+P"<<G4endl;
#endif
		          }
            else if(r<=nLim)
	           {
              eMass = mNeut;
              dbQPDG= NAQPDG;
              rMass = GSResNA;
              rQPDG = naQPDG;
#ifdef ppdebug
			           G4cout<<"G4QNucleus::EvaporateBary: A+N"<<G4endl;
#endif
   	        }
            else three=false;
		        }
          else three=false;
          if(rMass<1600.)
		        {
            if     (rQPDG==pQPDG)rMass=mProt;
            else if(rQPDG==nQPDG)rMass=mNeut;
            else if(rQPDG==lQPDG)rMass=mLamb;
		        }
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBary:evaBar="<<eMass<<bQPDG<<",resN="<<rMass<<rQPDG
                <<",secB="<<fMass<<",three="<<three<<G4endl;
#endif
	       }
	     }
      else // ==============> Just decay in a baryon and a residual (to avoid gamma-decay)
	     { //@@ Take into account Coulomb Barier Penetration Probability (?? - Emergency)
        G4double nLim=0.;
        if(nFlag&&mNeut+GSResNn<totMass)
        {
          G4double ken=totMass-mNeut-GSResNn;
          nLim+=N*CoulBarPenProb(0.,ken,0,1)*sqrt(ken);
		      }
        G4double zLim=nLim;
        if(pFlag&&mProt+PBarr+GSResNp<totMass)
        {
          G4double ken=totMass-mProt-GSResNp;
          if(barf) ken-=PBarr;
          zLim+=Z*CoulBarPenProb(PBarr,ken,1,1)*sqrt(ken);
		      }
        G4double sLim=zLim;
        if(lFlag&&mLamb+GSResNl<totMass)
        {
          G4double ken=totMass-mLamb-GSResNl;
          sLim+=S*CoulBarPenProb(0.,ken,0,1)*sqrt(ken);
		      }
        G4double aLim=sLim;
        if(alpha&&aFlag&&mAlph+GSResNa<totMass)
        {
          G4double ken=totMass-mAlph-GSResNa;
          if(barf) ken-=ABarr;
          aLim+=CoulBarPenProb(ABarr,ken,2,4)*sqrt(ken)*alpha*Z*(Z-1)*N*(N-1)
                *6/a1/(a-2)/(a-3);
		      }
        G4double r = aLim*G4UniformRand();
#ifdef ppdebug
        G4cout<<"G4QNucl::EvapBar:2Decay r="<<r<<",nLim="<<nLim<<",zLim="<<zLim<<",sLim="
              <<sLim<<",nF="<<nFlag<<",pF="<<pFlag<<",lF="<<lFlag<<",aF="<<aFlag<<G4endl;
#endif
        if     (aFlag&&r>sLim)
		      {
          bQPDG=aQPDG;
          eMass=mAlph;
          rQPDG=AQPDG;
          rMass=GSResNa;
		      }
        else if(lFlag&&r>=zLim&&r<=sLim&&zLim<sLim)
		      {
          bQPDG=lQPDG;
          eMass=mLamb;
          rQPDG=LQPDG;
          rMass=GSResNl;
		      }
        else if(nFlag&&r>=nLim&&r<=zLim&&nLim<zLim)
	       {
          bQPDG=pQPDG;
          eMass=mProt;
          rQPDG=PQPDG;
          rMass=GSResNp;
		      }
        else if(pFlag&&r<nLim)
	       {
          bQPDG=nQPDG;
          eMass=mNeut;
          rQPDG=NQPDG;
          rMass=GSResNn;
   	    }
        else
	       {
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon: Photon #2-B#, dM="<<totMass-GSMass<<G4endl;
#endif
          bQPDG=gQPDG;
          rQPDG=GetQPDG();
          eMass=0.;
          rMass=GSMass;
	       }
#ifdef ppdebug
        G4cout<<"G4QNucl::EvaporateBaryon: b="<<eMass<<bQPDG<<",r="<<rMass<<rQPDG<<G4endl;
#endif
      }
      if(three)           // Decay in two baryons + Residual Nucleus
	     {
#ifdef ppdebug
          G4cout<<"G4QNucl::EvaporateBaryon:Decay in 3 particles"<<G4endl;
#endif
        h1mom=G4LorentzVector(0.,0.,0.,eMass);
        h2mom=G4LorentzVector(0.,0.,0.,rMass);
        h3mom=G4LorentzVector(0.,0.,0.,fMass);
	       if(!DecayIn3(h1mom,h2mom,h3mom))
        {
#ifdef ppdebug
          G4cout<<"*G4QNucl::EvaporateBaryon:Decay M="<<totMass<<",b="<<eMass<<bQPDG
		        <<",f="<<fMass<<fQPDG<<",r="<<rMass<<rQPDG<<G4endl;
#endif
          return false;
	       }
        h1mom+=h3mom;
        bQPDG=dbQPDG;
	     }
      else
	     {
        if(eMass+rMass<totMass&&cntr<cntm)
        {
#ifdef ppdebug
          G4cout<<"G4QN::EvaB:eM="<<eMass<<"+rM="<<rMass<<" ="<<eMass+rMass<<" < "<<totMass
                <<",c="<<cntr<<" < cm="<<cntm<<", bPDG="<<bQPDG<<", rPDG="<<rQPDG<<G4endl;
#endif
          if(rMass<1600.)
		        {
            if     (rQPDG==pQPDG)rMass=mProt;
            else if(rQPDG==nQPDG)rMass=mNeut;
            else if(rQPDG==lQPDG)rMass=mLamb;
		        }
          h1mom=G4LorentzVector(0.,0.,0.,eMass);
          h2mom=G4LorentzVector(0.,0.,0.,rMass);
        }
        else if(totMass>mNeut+GSResNn)               // Neutron if 2-Decay failed
	       {
#ifdef ppdebug
		        G4cout<<"G4QNucl::EvaporateBaryon: Neutron , dM="<<totMass-GSResNn-mNeut<<G4endl;
#endif
          bQPDG=nQPDG;
          rQPDG=NQPDG;
          h1mom=G4LorentzVector(0.,0.,0.,mNeut);
          h2mom=G4LorentzVector(0.,0.,0.,GSResNn);      
	       }
        else if(totMass>mProt+PBarr+GSResNp)               // Proton if 2-Decay failed
	       {
#ifdef ppdebug
		        G4cout<<"G4QNucl::EvaporateBaryon: Proton , dM="<<totMass-GSResNp-mProt<<G4endl;
#endif
          bQPDG=pQPDG;
          rQPDG=PQPDG;
          h1mom=G4LorentzVector(0.,0.,0.,mProt);
          h2mom=G4LorentzVector(0.,0.,0.,GSResNp);      
	       }
        else if(totMass>mAlph+ABarr+GSResNa)               // Alpha if 2-Decay failed
	       {
#ifdef ppdebug
		        G4cout<<"G4QNucl::EvaporateBaryon: Alpha , dM="<<totMass-GSResNa-mAlph<<G4endl;
#endif
          bQPDG=aQPDG;
          rQPDG=AQPDG;
          h1mom=G4LorentzVector(0.,0.,0.,mAlph);
          h2mom=G4LorentzVector(0.,0.,0.,GSResNa);      
	       }
        else if(totMass>GSMass)               // Photon if 2-Decay failed
	       {
#ifdef ppdebug
		        G4cout<<"G4QNucl::EvaporateBaryon:Photon ### 2 ###, dM="<<totMass-GSMass<<G4endl;
#endif
          bQPDG=gQPDG;
          rQPDG=GetQPDG();
          h1mom=G4LorentzVector(0.,0.,0.,0.);
          h2mom=G4LorentzVector(0.,0.,0.,GSMass);      
	       }
        else
        {
	         G4cerr<<"***G4QNucl::EvaporateBaryon: Cann't evaporate even gamma (1)"<<G4endl;
          return false;
        }
      }
	   }
    else // ==> Decay in 3 Baryons + Residual is impossible at this point
	   {
	     if(secB)                        // Decay in 2Baryons(2a,a+bary)+ResidN is possible
	     //if(2>3)
	     {
#ifdef ppdebug
        G4cout<<"G4QNucleus::EvaporateBaryon: Decay in 2 baryons"<<G4endl;
#endif
        G4bool tpd=true;
        //@@ Coulomb Barrier penetration can be added
        G4double alp=0.;
		      if(aSecF)alp=alpha*Z*(Z-1)*N*(N-1)*10/(a-2)/(a-3)/(a-4);
        G4double  nnLim=0.;
        if(nnFlag&&totMass>mNeut+mNeut+GSResNN)
          nnLim+=N*(N-1)*pow(totMass-mNeut-mNeut-GSResNN,2);
        G4double  nzLim=nnLim;
        if(npFlag&&totMass>mNeut+mProt+PBarr+GSResNP)
		      {
          if(barf) nzLim+=2*N*Z*pow(totMass-mNeut-mProt-PBarr-GSResNP,2);
          else     nzLim+=2*N*Z*pow(totMass-mNeut-mProt-GSResNP,2);
		      }
        G4double  zzLim=nzLim;
        if(ppFlag&&totMass>mProt+mProt+SPPBarr+GSResPP)
		      {
          if(barf) zzLim+=Z*(Z-1)*pow(totMass-mProt-mProt-SPPBarr-GSResPP,2);
          else     zzLim+=Z*(Z-1)*pow(totMass-mProt-mProt-GSResPP,2);
		      }
        G4double  nlLim=zzLim;
        if(nlFlag&&totMass>mNeut+mLamb+GSResNL)
          nlLim+=2*N*S*pow(totMass-mNeut-mLamb-GSResNL,2);
        G4double  zlLim=nlLim;
        if(plFlag&&totMass>mProt+PBarr+mLamb+GSResPL)
		      {
          if(barf) zlLim+=2*Z*S*pow(totMass-mProt-mLamb-PBarr-GSResPL,2);
          else     zlLim+=2*Z*S*pow(totMass-mProt-mLamb-GSResPL,2);
		      }
        G4double  llLim=zlLim;
        if(llFlag&&totMass>mLamb+mLamb+GSResLL)
          llLim+=S*(S-1)*pow(totMass-mLamb-mLamb-GSResLL,2);
        G4double  naLim=llLim;
        if(naFlag&&totMass>mNeut+mAlph+ABarr+GSResNA)
		      {
          if(barf) naLim+=(N-3)*alp*pow(totMass-mNeut-mAlph-ABarr-GSResNA,2);
          else     naLim+=(N-3)*alp*pow(totMass-mNeut-mAlph-GSResNA,2);
		      }
        G4double  zaLim=naLim;
        if(paFlag&&totMass>mProt+SAPBarr+mAlph+GSResPA)
		      {
          if(barf) zaLim+=(Z-3)*alp*pow(totMass-mProt-mAlph-SAPBarr-GSResPA,2);
          else     zaLim+=(Z-3)*alp*pow(totMass-mProt-mAlph-GSResPA,2);
		      }
        G4double  laLim=zaLim;
        if(laFlag&&totMass>mLamb+mAlph+ABarr+GSResLA)
		      {
          if(barf) laLim+=S*alp*pow(totMass-mLamb-mAlph-ABarr-GSResLA,2);
          else     laLim+=S*alp*pow(totMass-mLamb-mAlph-GSResLA,2);
		      }
        G4double  aaLim=laLim;
        if(alpha&&aaFlag&&totMass>mAlph+mAlph+SAABarr+GSResAA)
		      {
          if(barf) aaLim+=alp*pow(totMass-mAlph-mAlph-SAABarr-GSResAA,2)*
                          alpha*(Z-2)*(Z-3)*(N-2)*(N-3)*7/(a-5)/(a-6)/(a-7);
          else     aaLim+=alp*pow(totMass-mAlph-mAlph-GSResAA,2)*
                          alpha*(Z-2)*(Z-3)*(N-2)*(N-3)*7/(a-5)/(a-6)/(a-7);
		      }
        G4double r = aaLim*G4UniformRand();
        if     (aaLim&&laLim<r)
	       {
          dbQPDG= AAQPDG;
          eMass=mAlph;
          fMass=mAlph;
          rQPDG=aaQPDG;
          rMass=GSResAA;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: A+A, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&zaLim<r&&r<laLim)
	       {
          dbQPDG= LAQPDG;
          eMass=mAlph;
          fMass=mLamb;
          rQPDG=laQPDG;
          rMass=GSResLA;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: A+L, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&naLim<r&&r<zaLim)
	       {
          dbQPDG= PAQPDG;
          eMass=mAlph;
          fMass=mProt;
          rQPDG=paQPDG;
          rMass=GSResPA;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: A+P, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&llLim<r&&r<naLim)
	       {
          dbQPDG= NAQPDG;
          eMass=mAlph;
          fMass=mNeut;
          rQPDG=naQPDG;
          rMass=GSResNA;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: A+N, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&zlLim<r&&r<llLim)
	       {
          dbQPDG= LLQPDG;
          eMass=mLamb;
          fMass=mLamb;
          rQPDG=llQPDG;
          rMass=GSResLL;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: L+L, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&nlLim<r&&r<zlLim)
	       {
          dbQPDG= PLQPDG;
          eMass=mLamb;
          fMass=mProt;
          rQPDG=plQPDG;
          rMass=GSResPL;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: L+p, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&zzLim<r&&r<nlLim)
	       {
          dbQPDG= NLQPDG;
          eMass=mLamb;
          fMass=mNeut;
          rQPDG=nlQPDG;
          rMass=GSResNL;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: L+n, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif

	       }
        else if(aaLim&&nzLim<r&&r<zzLim)
	       {
          dbQPDG= PPQPDG;
          eMass=mProt;
          fMass=mProt;
          rQPDG=ppQPDG;
          rMass=GSResPP;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: p+p, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&nnLim<r&&r<nzLim)
	       {
          dbQPDG= NPQPDG;
          eMass=mNeut;
          fMass=mProt;
          rQPDG=npQPDG;
          rMass=GSResNP;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: n+p, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
	       }
        else if(aaLim&&r<nnLim)
	       {
          dbQPDG= NNQPDG;
          eMass=mNeut;
          fMass=mNeut;
          rQPDG=nnQPDG;
          rMass=GSResNN;
#ifdef ppdebug
		        G4cout<<"G4QNuc::EvapBaryon: n+n, e="<<eMass<<",f="<<fMass<<",r="<<rMass<<G4endl;
#endif
   	    }
        //Two particle decay only possible (not frequent event!)
        else if(nFlag)
		      {
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon:Photon ### Decay in neutron ###"<<G4endl;
#endif
          tpd=false;
          bQPDG=nQPDG;
          rQPDG=NQPDG;
          eMass=mNeut;
          rMass=GSResNn;
		      }
        else if(pFlag)
		      {
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon:Photon ### Decay in proton ###"<<G4endl;
#endif
          tpd=false;
          bQPDG=pQPDG;
          rQPDG=PQPDG;
          eMass=mProt;
          rMass=GSResNp;
		      }
        else if(aFlag)
		      {
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon:Photon ### Decay in alpha ###"<<G4endl;
#endif
          tpd=false;
          bQPDG=aQPDG;
          rQPDG=AQPDG;
          eMass=mAlph;
          rMass=GSResNa;
		      }
        else if(lFlag)
		      {
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon:Photon ### Decay in Lambda ###"<<G4endl;
#endif
          tpd=false;
          bQPDG=lQPDG;
          rQPDG=LQPDG;
          eMass=mLamb;
          rMass=GSResNl;
		      }
        else
		      {
#ifdef ppdebug
          G4cout<<"G4QNuc::EvaporBaryon: Photon ### 3-Big ###,dM="<<totMass-GSMass<<G4endl;
#endif
          tpd=false;
          bQPDG=gQPDG;
          rQPDG=GetQPDG();
          eMass=0.;
          rMass=GSMass;
		      }
        if(tpd)
        {
          h1mom=G4LorentzVector(0.,0.,0.,eMass);
          h2mom=G4LorentzVector(0.,0.,0.,rMass);
          h3mom=G4LorentzVector(0.,0.,0.,fMass);
	         if(!DecayIn3(h1mom,h2mom,h3mom))
          {
#ifdef ppdebug
            G4cout<<"*G4QNucl::EvaporateBaryon:Decay M="<<totMass<<",b="<<eMass<<bQPDG
		          <<",f="<<fMass<<fQPDG<<",r="<<rMass<<rQPDG<<G4endl;
#endif
            return false;
	         }
          h1mom+=h3mom;
          bQPDG=dbQPDG;
#ifdef ppdebug
          G4double sma=h1mom.m();
          G4double dma=sma-eMass-fMass;
		        G4cout<<"G4QNuc::EvapBar:s="<<sma<<",e="<<eMass<<",f="<<fMass<<",d="<<dma<<",rM="
                <<rMass<<G4endl;
#endif
		      }
        else
		      {
          if(rMass<1600.)
		        {
            if     (rQPDG==pQPDG)rMass=mProt;
            else if(rQPDG==nQPDG)rMass=mNeut;
            else if(rQPDG==lQPDG)rMass=mLamb;
		        }
          h1mom=G4LorentzVector(0.,0.,0.,eMass);
          h2mom=G4LorentzVector(0.,0.,0.,rMass);      
	         if(!DecayIn2(h1mom,h2mom))
          {
#ifdef ppdebug
            G4cout<<"***G4QNucleus::EvaporateBaryon: Emergency Decay M="<<totMass<<",b="
                  <<bQPDG<<h1->GetQC()<<eMass<<",r="<<rQPDG<<h2->GetQC()<<rMass<<G4endl;
#endif
            return false;
	         }
          h1->SetQPDG(bQPDG);
          h2->SetQPDG(rQPDG);
          h1->Set4Momentum(h1mom);
          h2->Set4Momentum(h2mom);
#ifdef ppdebug
          G4cout<<"G4QNuc::EvapBaryon: Emergency decay is done for b="<<bQPDG<<h1->GetQC()
	               <<h1mom<<h1mom.m()<<", r="<<rQPDG<<h2->GetQC()<<h2mom<<h2mom.m()<<G4endl;
#endif
          return true;
		      }
	     }
      else                                     // Only decay in Baryon+Residual is possible
      {
#ifdef ppdebug
        G4cout<<"G4QNucleus::EvaporateBaryon: Decay in Baryon+Resid"<<G4endl;
#endif
        //@@ Take into account Coulomb Barier Penetration Probability
        G4double nLim=0.;
        if(nFlag&&mNeut+GSResNn<totMass)
        {
          G4double ken=totMass-mNeut-GSResNn;
          nLim+=N*CoulBarPenProb(0.,ken,0,1)*sqrt(ken);
		      }
        G4double zLim=nLim;
        if(pFlag&&mProt+PBarr+GSResNp<totMass)
        {
          G4double ken=totMass-mProt-GSResNp;
          if(barf) ken-=PBarr;
          zLim+=Z*CoulBarPenProb(PBarr,ken,1,1)*sqrt(ken);
		      }
        G4double sLim=zLim;
        if(lFlag&&mLamb+GSResNl<totMass)
        {
          G4double ken=totMass-mLamb-GSResNl;
          sLim+=S*CoulBarPenProb(0.,ken,0,1)*sqrt(ken);
		      }
        G4double aLim=sLim;
		      if(aFlag&&mAlph+GSResNa+ABarr<totMass)
        {
          G4double ken=totMass-mAlph-GSResNa;
          if(barf) ken-=ABarr;
          aLim+=CoulBarPenProb(ABarr,ken,2,4)*sqrt(ken)*alpha*Z*(Z-1)*N*(N-1)
                *6/a1/(a-2)/(a-3);
#ifdef ppdebug
          G4cout<<"G4QN::EvaB:al="<<alpha<<",k="<<ken<<",P="<<CoulBarPenProb(ABarr,ken,2,4)
                <<G4endl;
#endif
		      }
        G4double r = aLim*G4UniformRand();
#ifdef ppdebug
        G4cout<<"G4QN::EB:DecIn2#2#r="<<r<<",nL="<<nLim<<",zL="<<zLim<<",sL="<<sLim<<",aL="
              <<aLim<<",nF="<<nFlag<<",pF="<<pFlag<<",lF="<<lFlag<<",aF="<<aFlag<<G4endl;
#endif
        if     (aFlag&&r>sLim)
	       {
          bQPDG=aQPDG;
          eMass=mAlph;
          rQPDG=AQPDG;
          rMass=GSResNa;
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon: Decay in A + rA="<<GSResNa+mAlph<<G4endl;
#endif
	       }
        else if(lFlag&&r>zLim&&r<sLim)
	       {
          bQPDG=lQPDG;
          eMass=mLamb;
          rQPDG=LQPDG;
          rMass=GSResNl;
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon: Decay in L + rA="<<GSResNl+mLamb<<G4endl;
#endif
	       }
        else if(pFlag&&r>nLim&&r<zLim)
	       {
          bQPDG=pQPDG;
          eMass=mProt;
          rQPDG=PQPDG;
          rMass=GSResNp;
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon: Decay in P + rA="<<GSResNp+mProt<<G4endl;
#endif
	       }
        else if(nFlag&&r<nLim)
	       {
          bQPDG=nQPDG;
          eMass=mNeut;
          rQPDG=NQPDG;
          rMass=GSResNn;
#ifdef ppdebug
          G4cout<<"G4QNucleus::EvaporateBaryon: Decay in N + rA="<<GSResNn+mNeut<<G4endl;
#endif
   	    }
        else if(mProt+GSResNp<totMass)
	       {
#ifdef ppdebug
          G4cout<<"G4QNucl::EvapBar: Emergency Proton, dM="<<totMass-GSResNp-mProt<<G4endl;
#endif
          bQPDG=pQPDG;
          rQPDG=PQPDG;
          eMass=mProt;
          rMass=GSResNp;
	       }
        else if(mAlph+GSResNa<totMass)
	       {
#ifdef ppdebug
          G4cout<<"G4QNucl::EvapBar: Emergency Alpha, dM="<<totMass-GSResNa-mAlph<<G4endl;
#endif
          bQPDG=aQPDG;
          rQPDG=AQPDG;
          eMass=mAlph;
          rMass=GSResNa;
	       }
        else
	       {
#ifdef ppdebug
          G4cout<<"G4QNuc::EvapBaryon: Photon ### 4-Big ###, dM="<<totMass-GSMass<<G4endl;
#endif
          bQPDG=gQPDG;
          rQPDG=GetQPDG();
          eMass=0.;
          rMass=GSMass;
	       }
        if(rMass<1600.)
		      {
          if     (rQPDG==pQPDG)rMass=mProt;
          else if(rQPDG==nQPDG)rMass=mNeut;
          else if(rQPDG==lQPDG)rMass=mLamb;
	       }
        h1mom=G4LorentzVector(0.,0.,0.,eMass);
        h2mom=G4LorentzVector(0.,0.,0.,rMass);      
      }
	   }
	   if(!DecayIn2(h1mom,h2mom))
    {
#ifdef ppdebug
      G4cout<<"*G4QNucleus::EvaporateBaryon: Decay M="<<totMass<<",b="<<bQPDG<<h1->GetQC()
		    <<eMass<<",r="<<rQPDG<<h2->GetQC()<<rMass<<G4endl;
#endif
      return false;
	   }
#ifdef ppdebug
	   G4cout<<"G4QN::EvaB: **RESULT** b="<<bQPDG<<h1mom<<", r="<<rQPDG<<h2mom<<G4endl;
#endif
    h1->SetQPDG(bQPDG);
    h2->SetQPDG(rQPDG);
    h1->Set4Momentum(h1mom);
    h2->Set4Momentum(h2mom);
#ifdef ppdebug
    G4cout<<"G4QNucleus::EvaporateBaryon: Evaporation is done for b="<<bQPDG<<h1->GetQC()
	      <<h1mom<<h1mom.m()<<", r="<<rQPDG<<h2->GetQC()<<h2mom<<h2mom.m()<<G4endl;
#endif
	   return true;
  }
  else if(a==1)
  {
#ifdef ppdebug
    G4cerr<<"***G4QNucleus::EvaporateBaryon: ??? A=1"<<G4endl;
#endif
    h1->SetQPDG(gQPDG);
    h1->Set4Momentum(G4LorentzVector(0.,0.,0.,0.));
    if     (Z>0)
    {
      h2->SetQPDG(pQPDG);
      h2->Set4Momentum(G4LorentzVector(0.,0.,0.,mProt));
    }
    else if(N>0)
    {
      h2->SetQPDG(nQPDG);
      h2->Set4Momentum(G4LorentzVector(0.,0.,0.,mNeut));
    }
    else if(S>0)
    {
      h2->SetQPDG(lQPDG);
      h2->Set4Momentum(G4LorentzVector(0.,0.,0.,mLamb));
    }
    else return false;
	return true;
  }
  if(a<1) G4cerr<<"***G4QNucleus::EvaporateBaryon: A="<<a<<G4endl;
  return false;
}
// End of "EvaporateBaryon"

// Randomize bimomial low 
G4int G4QNucleus::RandomizeBinom(G4double p,G4int aN)
{//  ===============================================
  G4double r = G4UniformRand();
  G4double d = 1.-p;
  G4double v = pow(d,aN);
  G4double s = v;
  if(r<s) return 0;
  G4int i=0;
  G4int j=aN+1;
  G4double f=p/d;
  while(r>s && i<aN)
  {
    j--;
	   i++;
	   v*=j*f/i;
    s+=v;
  }
  return i;
}

//Initialize a Candidate vector for the instance of a Quasmon
void G4QNucleus::InitCandidateVector(G4QCandidateVector& theQCandidates,
                                    G4int maxMes, G4int maxBar, G4int maxClst)
//   ==========================================================================
{
  static const G4int nOfMesons =45; //a#of S=0,1,2,3,4 Mesons, => candidates to hadrons
  static const G4int nOfBaryons=72; //a#of 1/2,3/2,5/2,7/2 Baryons => candidates to hadrons
  // Scalar resonances   (0):           Eta,Pi0,Pi+,APi-,Ka0,Ka+,AKa0,AKa-,Eta*
  static G4int mesonPDG[nOfMesons]  =  {221,111,211,-211,311,321,-311,-321,331,    //  0- 8
  // Vector resonances   (1):           omega,Rh0,Rh+,Rho-,K0*,K+*,AK0*,AK-*,Phi
																																								223,113,213,-213,313,323,-313,-323,333,    //  9-18
  // Tensor D-resonances (2):           f2 ,a20,a2+, a2-,K20,K2+,AK20,AK2-,f2'
																																								225,115,215,-215,315,325,-315,-325,335,    // 19-27
  // Tensor F-resonances (3):           om3,ro3,r3+,rh3-,K30,K3+,AK30,AK3-,Phi3
                                        227,117,217,-217,317,327,-317,-327,337,    // 28-35
  // Tensor G-resonances (4):           f4 ,a40,a4+, a4-,K40,K4+,AK40,AK4-,f4'
								                                229,119,219,-219,319,329,-319,-329,339};   // 36-44
  // Baryon octet      (1/2):          n  , an  , p  , ap  ,lamb,alamb, sig-,asig-
  static G4int baryonPDG[nOfBaryons]={2112,-2112,2212,-2212,3122,-3122,3112,-3112, // 45-52
		// Hyperon octet     (1/2):         sig0,asig0,sig+,asig+,ksi-,aksi-,ksi0,aksi0
																																						3212,-3212,3222,-3222,3312,-3312,3322,-3322, // 53-60
  // Baryon decuplet   (3/2):   del-,adel-,del0,adel0,del+,adel+,dl++,adl++,sis-,asis-
																																1114,-1114,2114,-2114,2214,-2214,2224,-2224,3114,-3114,//70
		//                            sis0,asis0,sis+,asis+,kss-,akss-,kss0,akss0,omeg,aomeg
																																3214,-3214,3224,-3224,3314,-3314,3324,-3324,3334,-3334,//80
  // Baryon octet      (5/2):         n5/2,an5/2,p5/2,ap5/2,l5/2,al5/2,si5-,asi5-
																																						2116,-2116,2216,-2216,3126,-3126,3116,-3116, // 81-88
		//                                  si50,asi50,si5+,asi5+,ks5-,aks5-,ks50,aks50
                                      3216,-3216,3226,-3226,3316,-3316,3326,-3326, // 89-96
  // Baryon decuplet   (7/2):  dl5-,adl5-,dl50,adl50,dl5+,adl5+,d5++,ad5++,si5-,asi5-
																														1118,-1118,2118,-2118,2218,-2218,2228,-2228,3118,-3118, //106
		//                          si50,asi50,si5+,asi5+,ks5-,aks5-,ks50,aks50,ome5,aome5
																														3218,-3218,3228,-3228,3318,-3318,3328,-3328,3338,-3338};//116
  G4int i=0;
#ifdef debug
  G4int ind=0;
#endif
  if(maxMes>nOfMesons) maxMes=nOfMesons;
  if(maxMes>=0) for (i=0; i<maxMes; i++) 
  {
    theQCandidates.push_back(new G4QCandidate(mesonPDG[i]));
#ifdef debug
    G4cout<<"G4QNucleus::InitCandidateVector: "<<ind++<<", Meson # "<<i<<" with code = "
          <<mesonPDG[i]<<", QC="<<theQCandidates[i]->GetQC()<<" is initialized"<<G4endl;
#endif
  }
  if(maxBar>nOfBaryons) maxBar=nOfBaryons;
  if(maxBar>=0) for (i=0; i<maxBar; i++) 
  {
#ifdef debug
    G4cout<<"G4QNucleus::InitCandidateVector: define PDG="<<baryonPDG[i]<<G4endl;
#endif
    G4QCandidate* curBar=new G4QCandidate(baryonPDG[i]);
#ifdef debug
    G4cout<<"G4QNucleus::InitCandidateVector: current baryon is defined"<<G4endl;
#endif
    theQCandidates.push_back(curBar); // delete equivalent
#ifdef debug
    G4cout<<"G4Nucleus::InitCandidateVector: "<<ind++<<", Baryon # "<<i<<" with code = "
          <<baryonPDG[i]<< ", QC="<<theQCandidates[i]->GetQC()<<" is initialized"<<G4endl;
#endif
  }
  if(maxClst>=0) for (i=0; i<maxClst; i++) 
  {
    G4int clustQCode = i+G4QPDGCode().GetNQHadr(); //Q-codes of cluster in the CHIPS world
    G4QPDGCode clustQPDG;
    clustQPDG.InitByQCode(clustQCode);
    G4int clusterPDG=clustQPDG.GetPDGCode();
    theQCandidates.push_back(new G4QCandidate(clusterPDG)); // delete equivalent
#ifdef debug
    G4cout<<"G4QNucleus::InitCandidateVector:"<<ind++<<", Cluster # "<<i<<" with code = "
          <<clusterPDG<<", QC="<<clustQPDG.GetQuarkContent()<<" is initialized"<<G4endl;
#endif
  }
} // End of "InitCandidateVector"

// Calculate a#of Z,N,L-clusters in the nucleus and fill candidate's probabilities
void G4QNucleus::PrepareCandidates(G4QCandidateVector& theQCandidates, G4bool piF,
                                   G4bool gaF, G4LorentzVector pLV)
//   ==============================================================================
{
  static const G4LorentzVector zeroLV(0.,0.,0.,0.);
  G4double ze  = Z;
  G4double ne  = N;
  G4double se  = S;
  G4double ae  = Z+N+S;
  G4double aea = ae*(ae-1);
  G4double ae2 = aea/2.;
  G4double ze1 = dZ + 1;
  G4double ne1 = dN + 1;
  G4double se1 = dS + 1;
  G4double ae0 = dZ + dN + dS;
  G4double ae1 = ae0 + 1;
  G4double pos = probVect[0];           // Value of Pre-Probability for VacuumHadronization
  G4int mac=6;                          // Maximum cluster # for fixed baryon number
  G4int cca=0;                          // Counter of clusters for the same baryon number
  G4int acm=0;                          // Threshold ac value
  G4int mCand=theQCandidates.size();    // Full set of candidates made in UpdateClusters
  G4double s=0.;                        // Prototype of summ for constant A (=ac>2)
  G4double comb=ae0*(ae0-1)/2;          // Product up to ac=2
  if(comb<=0.) comb=1;
#ifdef cldebug
  G4double sZ=0.;                       // Percent of protons
  G4double sN=0.;                       // Percent of neutrons
  G4cout<<"G4QN::PC:C#"<<mCand<<",dZ="<<dZ<<",dN="<<dN<<",ZNS="<<Z<<","<<N<<","<<S<<G4endl;
#endif
  for (G4int index=0; index<mCand; index++)
  {
    G4QCandidate* curCand=theQCandidates[index];
    G4int cPDG  = curCand->GetPDGCode();
    G4int cBN   = curCand->GetBaryonNumber();
    G4int cST   = curCand->GetStrangeness();
    // ***********************************************************************************
    // These are first 117 candidates which are defined in G4QNucleus::InitCandidateVector
    // ***!!!*** if they are changed there the corresponding change must be done here
    //static const G4int nOfMesons =45;//a#of S=0,1,2,3,4 Mesons, => candidates to hadrons
    //static const G4int nOfBaryons=72;//a#of 1/2,3/2,5/2,7/2 Baryons => cand's to hadrons
    // Scalar resonances   (0):           Eta,Pi0,Pi+,APi-,Ka0,Ka+,AKa0,AKa-,Eta*
    //static G4int mesonPDG[45]  =  {221,111,211,-211,311,321,-311,-321,331,       //  0- 8
    // Vector resonances   (1):    omega,Rh0,Rh+,Rho-,K0*,K+*,AK0*,AK-*,Phi
		  //																							 						223,113,213,-213,313,323,-313,-323,333,        //  9-18
    // Tensor D-resonances (2):     f2 ,a20,a2+, a2-,K20,K2+,AK20,AK2-,f2'
		  //																						 							225,115,215,-215,315,325,-315,-325,335,        // 19-27
    // Tensor F-resonances (3):     om3,ro3,r3+,rh3-,K30,K3+,AK30,AK3-,Phi3
    //                              227,117,217,-217,317,327,-317,-327,337,        // 28-35
    // Tensor G-resonances (4):     f4 ,a40,a4+, a4-,K40,K4+,AK40,AK4-,f4'
		  //						                        229,119,219,-219,319,329,-319,-329,339};       // 36-44
    // Baryon octet       (1/2):    n  , an  , p  , ap  ,lamb,alamb, sig-,asig-
    //static G4int baryonPDG[72]={2112,-2112,2212,-2212,3122,-3122,3112,-3112,     // 45-52
		  // Hyperon octet     (1/2): sig0,asig0,sig+,asig+,ksi-,aksi-,ksi0,aksi0
		  //																												3212,-3212,3222,-3222,3312,-3312,3322,-3322,     // 53-60
    // Baryon decuplet   (3/2): del-,adel-,del0,adel0,del+,adel+,dl++,adl++,sis-,asis-
		  //																										1114,-1114,2114,-2114,2214,-2214,2224,-2224,3114,-3114,//70
		  //                          sis0,asis0,sis+,asis+,kss-,akss-,kss0,akss0,omeg,aomeg
		  //																										3214,-3214,3224,-3224,3314,-3314,3324,-3324,3334,-3334,//80
    // Baryon octet      (5/2): n5/2,an5/2,p5/2,ap5/2,l5/2,al5/2,si5-,asi5-
		  //																										2116,-2116,2216,-2216,3126,-3126,3116,-3116,       // 81-88
		  //                          si50,asi50,si5+,asi5+,ks5-,aks5-,ks50,aks50
    //                          3216,-3216,3226,-3226,3316,-3316,3326,-3326,       // 89-96
    // Baryon decuplet  (7/2): dl5-,adl5-,dl50,adl50,dl5+,adl5+,d5++,ad5++,si5-,asi5-
				//																								1118,-1118,2118,-2118,2218,-2218,2228,-2228,3118,-3118, //106
		  //                        si50,asi50,si5+,asi5+,ks5-,aks5-,ks50,aks50,ome5,aome5
				//																						  3218,-3218,3228,-3228,3318,-3318,3328,-3328,3338,-3338};//116
    // One should take into account, that #of mesons & baryons can be cut in G4Quas::HadrQE
				//G4int nP= theWorld->GetQPEntries(); // A#of initialized particles in CHIPS World
				////@@ Make special parametyer to cut high resonances for nuclear fragmentation !!
				//G4int          nMesons  = 45;
				//if     (nP<34) nMesons  =  9;
				//else if(nP<51) nMesons  = 18;
				//else if(nP<65) nMesons  = 27;
				//else if(nP<82) nMesons  = 36;
				//G4int          nBaryons = 72;
				//if     (nP<45) nBaryons = 16;
				//else if(nP<59) nBaryons = 36;
				//else if(nP<76) nBaryons = 52;
    // **********************************************************************************
    //G4int cS    = curCand->GetStrangeness();
	   //if(piF&&gaF&&cPDG!=90000001&&cPDG!=90001000) // Both flags, in case of pi-first-int
	   //if(piF&&gaF&&cBN!=1&&cBN!=3) // Both flags, which is in case of pi-first-int
	   if(piF&&gaF&&cBN!=1)// @@ Should be both, which is in case of pi-first-interaction @@ ?
	   //if(piF&&gaF&&cBN!=1&&cBN!=4) // Should be both, in case of pi-first-interaction
    {
      curCand->SetPreProbability(0.);  
      curCand->SetDenseProbability(0.); 
	     curCand->SetPossibility(false);    
#ifdef cldebug
      if(cPDG==90001001) G4cout<<"G4QNuc::PrepCand: piF/gaF fragments are blocked"<<G4endl;
#endif
    }
    // @@ in case of the Ksi or Omega- capture it can disturb the simulation
	   else if(cPDG<80000000&&(abs(cPDG)%10>4||cST>2))// @@ PreClosed HighSpin/Strange hadrons
    {
      curCand->SetPreProbability(0.);  
      curCand->SetDenseProbability(0.); 
	     curCand->SetPossibility(false);    
    }
    else
	   {
      G4double tnM=GetQPDG().GetMass();          // Total mass of this nucleus
      if(cPDG>80000000&&cPDG!=90000000)          // ===> Cluster case
	     {
        G4int sc = cST;                          // "S" of the cluster
        G4int zc = curCand->GetCharge();         // "Z" of the cluster
        G4int ac = cBN;                          // "A" of the cluster
        G4int nc = ac-zc-sc;                     // "N" of the cluster
        G4double cM=tnM-G4QNucleus(Z-zc,N-nc,S-sc).GetGSMass(); // BoundMass of the cluster
        G4LorentzVector intLV=pLV+G4LorentzVector(0.,0.,0.,cM); // 4-mom of the proj+clust
        pos      = probVect[ac];                 // Cluster Probability NormalizationFactor
        if(ac<=maxClust&&pos>0.&&(pLV==zeroLV||intLV.m()>.00001+cM))
	       {

#ifdef cldebug
          G4cout<<"G4QNucleus::PrepareCand: ac="<<ac<<", pV="<<pos<<G4endl;
#endif
          G4int dac=ac+ac;
          if(ac && (piF || gaF))                 // zc>=0
										{
            if     (piF&&!gaF&&zc+ac) pos*=(zc+ac)/ac;  // piF interaction (#of u-quarks)
            else if(gaF&&!piF&&zc+dac) pos*=(zc+dac)/ac; // gaF interaction (sum of Q_q^2)
          }
          G4double dense=1.;
          if     (ac==1&&pos>0.)dense=probVect[254]/pos;
          else if(ac==2&&pos>0.)dense=probVect[255]/pos;
#ifdef cldebug
	         G4cout<<"G4QNucleus::PrepC: cPDG="<<cPDG<<",norm="<<pos<<",zc="<<zc<<",nc="<<nc
                <<",sc="<<sc<<",ac="<<ac<<",ze1="<<ze1<<",ne1="<<ne1<<",se1="<<se1<<G4endl;
          G4double mp=pos;
#endif
          if     (ac==1 && ae)                   // ae=0 protection (since here no /pos)
	         {
            if     (zc) pos*=ze/ae;
            else if(nc) pos*=ne/ae;
            else if(sc) pos*=se/ae;
            //if     (zc) pos*=ze;
            //else if(nc) pos*=ne;
            //else if(sc) pos*=se;
            acm=1;
#ifdef cldebug
            if(pos)
			         G4cout<<"G4QN::PrC:mp="<<mp<<",pos="<<pos<<",ae="<<ae
                  <<",Z="<<zc<<",N="<<nc<<",mac="<<mac<<G4endl;
            sZ+=pos*zc;
            sN+=pos*nc;
#endif
          }
          else if(ac==2)
	         {
            if(ze<zc||ne<nc||se<sc) pos=0.;
            else if(aea)                         // Protection against aea=0.
		          {
              if     (zc==2) pos*=ze*(ze-1)/aea;
              else if(nc==2) pos*=ne*(ne-1)/aea;
              else if(sc==2) pos*=se*(se-1)/aea;
              else if(zc==1&&nc==1) pos*=ze*ne/ae2;
              else if(zc==1&&sc==1) pos*=ze*se/ae2;
              else if(sc==1&&nc==1) pos*=se*ne/ae2;
              //if     (zc==2) pos*=ze*(ze-1)/2.;
              //else if(nc==2) pos*=ne*(ne-1)/2.;
              //else if(sc==2) pos*=se*(se-1)/2.;
              //else if(zc==1&&nc==1) pos*=ze*ne;
              //else if(zc==1&&sc==1) pos*=ze*se;
              //else if(sc==1&&nc==1) pos*=se*ne;
              else G4cout<<"***G4QNucl::PrepCand: z="<<zc<<", n="<<nc<<", s="<<sc<<G4endl;
		            // Normalization for only not strange matter
		          }
            acm=2;
#ifdef cldebug
            if(pos)
		          G4cout<<"G4QN::PrC:mp="<<mp<<",p="<<pos<<",A=2,(Z="<<zc<<",N="<<nc<<"),m="
                  <<mac<<G4endl;
            sZ+=pos*zc;
            sN+=pos*nc;
#endif
          }
          else                                     // ac>2
	         {
            if(acm<ac)                             // first time that big cluster
		          {
              if(ac<ae1 && ac>0) comb*=(ae1-ac)/ac;
              acm=ac;
              s=0.;
              cca=0;
              if(ac%2) mac=7;                      // @@Change it if cluster set is changed
		            else     mac=8;                      // @@ It is not yet automatic
#ifdef cldebug
              G4cout<<"G4QNuc::PrepCl:c="<<comb<<",ac="<<ac<<"("<<index<<"),m="<<mac<<",a="
                    <<ae0<<G4endl;
#endif
            }
            G4double prod=1.;
            if(ze1<=zc||ne1<=nc||se1<=sc) prod=0.;
            else
		          {
              if(zc>0) for(int iz=1; iz<=zc; iz++) prod*=(ze1-iz)/iz;
              if(nc>0) for(int in=1; in<=nc; in++) prod*=(ne1-in)/in;
              if(sc>0) for(int is=1; is<=sc; is++) prod*=(se1-is)/is;
		          }
            s+=prod;
            pos*=prod;
            pos/=comb;
#ifdef cldebug
		          if(pos) G4cout<<"G4QN::PreC:c="<<cPDG<<",p="<<pos<<",i="<<index<<",m="<<mac
                          <<",pr="<<prod<<",c="<<cca<<G4endl;
            sZ+=pos*zc;
            sN+=pos*nc;
#endif
            cca++;
	         }
          curCand->SetPreProbability(pos);
          curCand->SetDenseProbability(pos*dense);
#ifdef cldebug
	         G4cout<<"G4QN::PrepC: ClasterPDG="<<cPDG<<",preProb="<<pos<<",d="<<dense<<G4endl;
#endif
        }
	       else                                       // => "Cluster is too big" case
        {
          curCand->SetPreProbability(0.);     
          curCand->SetDenseProbability(0.);
	         curCand->SetPossibility(false);          // This candidate is not possible 
        }
	     }
	     else
      {
        curCand->SetPreProbability(pos);           // ===> Hadronic case in Vacuum     
        curCand->SetDenseProbability(0.);          // ===> Hadronic case in Vacuum
      }
	     curCand->SetPossibility(true);           // All candidates are possible at this point
    }
  } // End of the LOOP over Candidates
#ifdef cldebug
  G4cout<<"G4QNucl::PrepCand:covP="<<ae*sZ/ze<<",covN="<<ae*sN/ne<<",totP="<<sZ+sN<<G4endl;
  //throw G4QException("G4QNucleus::PrepareCandidate: Temporary stop");
#endif
}// End of PrepareCandidates

//Coulomb Barrier Calculation
G4double G4QNucleus::CoulombBarrier(const G4double& cZ, const G4double& cA, G4double delZ,
                                    G4double dA)
{//                  =====================================================================
  static const G4double third=1./3.;
  //return 0.;                                       //@@ Temporary for test
  if(cZ<=0.) return 0.;
  G4double rA=GetA()-cA;
  if (dA)  rA-=dA;
  G4double rZ=Z-cZ;
  if(delZ) rZ-=delZ;
  G4double zz=rZ*cZ;
  // Naitive CHIPS radius: CB={1.46=200(MeV)/137}*z*Z/{R=1.13}*((a*z)**1/3+A**1/3) (?)
  //G4double cb=1.29*zz/(pow(rA,third)+pow(cA,third));
  G4double ff=cZ;
  if(cA==1) ff+=3.;
  if(rA<0.) rA=0.;
#ifdef debug
  if(cA<0.) G4cout<<"-Warning-G4QNucl::CoulombBarrier: NucleusA="<<cA<<", Z="<<cZ<<G4endl;
#endif
  G4double ca=cA;

  // Alex Howard adding protection against NaNs 25/4/07
  if(cA<0.) ca=0.;
  G4double cb=0.; // default of 0 MeV coulomb barrier if meson? AH
  G4double denominator = pow(rA,third)+ff*pow(ca,third);
  if(denominator != 0) cb=zz/denominator; // Negative hadronic potential
  // AH

  // Geant4 solution for protons is practically the same:
  // G4double cb=1.263*Z/(1.0 + pow(rA,third));
  // @@ --- Temporary "Lambda/Delta barrier for mesons"
  //if(!cA) cb+=40.;
		// --- End of Temporary ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  return cb;
} // End of "CoulombBarier"

// Fission Coulomb Barrier Calculation
G4double G4QNucleus::FissionCoulombBarrier(const G4double& cZ, const G4double& cA,
                                           G4double delZ, G4double dA)
{//                  =====================================================================
  static const G4double third=1./3.;
  if(cZ<=0.) return 0.;
  G4double rA=GetA()-cA;
  if(dA) rA-=dA;                        // Reduce rA f CB is calculated for wounded nucleus
  G4double rZ=Z-cZ;
  if(delZ) rZ-=delZ;                    // Reduce rZ f CB is calculated for wounded nucleus
  G4double zz=rZ*cZ;                    // Product of charges
  G4double r=(pow(rA,third)+pow(cA,third))*(1.51+.00921*zz)/(1.+.009443*zz);
  return 1.44*zz/r;
} // End of "FissionCoulombBarier"

//Coulomb Binding Energy for the cluster
G4double G4QNucleus::BindingEnergy(const G4double& cZ, const G4double& cA, G4double delZ,
                                   G4double dA)
{//                  ====================================================================
  static const G4double mNeut= G4QPDGCode(2112).GetMass(); // Mass of neutron
  static const G4double mProt= G4QPDGCode(2212).GetMass(); // Mass of proton
  if(!cZ && !cA) return Z*mProt+N*mNeut-G4QNucleus(Z,N,0).GetGSMass(); // For QGSM
  G4double GSM=GetGSMass();
  G4int iZ=static_cast<int>(cZ);
  G4int cN=static_cast<int>(cA-cZ);
  G4int sZ=iZ;
  G4int sN=cN;
  if(delZ||dA)
  {
    G4int dz=static_cast<int>(delZ);
    G4int dn=static_cast<int>(dA-delZ);
    GSM=G4QNucleus(Z-dz,N-dn,S).GetGSMass();
    sZ-=dz;
    sN-=dn;
  }
  return G4QNucleus(Z-sZ,N-sN,S).GetGSMass()+G4QNucleus(iZ,cN,0).GetGSMass()-GSM;
} // End of "BindingEnergy"

//Coulomb Barrier Reflection Probability (CB - Coulomb Barrier, E - Kinetic Energy) 
G4double G4QNucleus::CoulBarPenProb(const G4double& CB, const G4double& E,
                                    const G4int& C, const G4int& B)
{//                 = A.Lepretre et al, Nucl.Phys., A390 (1982) 221-239 =
  static const G4double mNeut= G4QPDGCode(2112).GetMass();          // Mass of neutron
  static const G4double dNeut= mNeut+mNeut;                         // DiMass of neutron
  static const G4double mProt= G4QPDGCode(2212).GetMass();          // Mass of proton
  static const G4double dProt= mProt+mProt;                         // DiMass of proton
  static const G4double mDeut= G4QPDGCode(2112).GetNuclMass(1,1,0); // Mass of deuteron
  //static const G4double mTrit= G4QPDGCode(2112).GetNuclMass(1,2,0); // Mass of tritium
  //static const G4double mHel3= G4QPDGCode(2112).GetNuclMass(2,1,0); // Mass of Helium 3
  //static const G4double mAlph= G4QPDGCode(2112).GetNuclMass(2,2,0); // Mass of alpha
  static const G4double wellDebth=40.;          //@@ Should be jus binding energy @@ done
  // @@ --- Temporary 1 ---> close the OverBarrierReflection for all
  //return 1.;
  // ^^^^^^^---> End of Themporary 1
  // @@ --- Temporary 2 ---> close the OverBarrierReflection for fragments and mesons
  //if(E<CB) return 0.;
  //if(B!=1) return 1.;
  if(B<1 || B>2) return 1.;
  if(C>B+1)
  {
#ifdef debug
    G4cout<<"-Warning-G4QN::CBPP:SubtractedChrg="<<C<<" >SubtractedBaryonNmbr="<<B<<G4endl;
#endif
    return 1.;
  }
  // ^^^^^^^---> End of Themporary 2
  //G4double nA=GetA();
  //G4double nA=GetA()-B;
  //if(nA==40) G4cout<<"G4QN::CBPP:Z="<<GetZ()<<",C="<<C<<",B="<<B<<G4endl;
  //else     return 1.;           // @@@@@ Over barrier reflection is closed @@@ !!! @@@
  //      Li6      C12           Al27
  //else if(nA<7||nA>8&&nA<12||nA>16&&nA<40) return 1.;// "OverBarrierReflection is closed"
  //else if(nA>8&&nA<12||nA>16&&nA<40) return 1.; // "OverBarrierReflection is closed" Cond
  //else if(nA<12||nA>16&&nA<40) return 1.; // "OverBarrierReflection is closed" Condition
  //else if(nA<12||nA>16) return 1.; // "OverBarrierReflection is closed" Condition
  //else if(nA<12) return 1.;    // @@@@@ Over barrier reflection is closed @@@ !!! @@@
  //if(B+B>Z+N+S) return 1.;
  //G4double wD=wellDebth*B;
  G4double wD=wellDebth;
  //G4double wD=0.;
  // @@ --- Temporary 3 ---> close the OverBarrierReflection for mesons
  //if(!B) wD=0.;
  // ^^^^^^^---> End of Themporary 3
  G4double GSM=GetGSMass();
  if(2>3);
  // @@ Temporary "Mass Barrier for mesons" @@ __________________
  //else if(!B) wD=40.;
		// @@ End of Temporary^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  ////else if(nA<7&&B>0)  wD=0.;    // Only Coulomb Barrier can reflect !!!
  ////else if((nA<12||nA>16)&&B>0) wD=0.;// Only CoulombB can reflect !!! O16 E-dep of gamA
  ////else if((nA<12||nA>27)&&B>0) wD=0.;// Only CoulombB can reflect !!! O16 E-dep of gamA
  ////else if(nA<9&&B>0) return 1.;// Only CoulombBarrier can reflect !!! O16 E-dep of gamA
  ////else if(B>0)  wD=0.;    // Only Coulomb Barrier can reflect !!!
  ////else if(B==1)  wD=0.;
  else if(B==1&&C==1) wD=G4QNucleus(Z-1,N,S).GetGSMass()+mProt-GSM;
  else if(B==1&&C==0) wD=G4QNucleus(Z,N-1,S).GetGSMass()+mNeut-GSM;
  ////else if(B==1&&C==0) wD=0.;
  ////else if(B>1)  return 1.;
  ////else if(B>1)  wD=0.;
  ////else if(B==2)  wD=0.;
  else if(B==2)
  {
    if       (!C) wD=G4QNucleus(Z,N-2,S).GetGSMass()+dNeut-GSM;
    else if(C==1) wD=G4QNucleus(Z-1,N-1,S).GetGSMass()+mDeut-GSM;
    else if(C==2) wD=G4QNucleus(Z-2,N,S).GetGSMass()+dProt-GSM;
    // @@ Temporary "Local B=2 Anti Virial factor" @@ __________________
    wD=80.; // 40 MeV per each nucleon
    //wD=wD/2;
			 // @@ End of Temporary^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  }
  ////else if(B>2)  wD=0.;
  ////else if(B>2)  return 1.;
  ////else if(B==3)  wD=0.;
  //else if(B==3&&C==1) wD=G4QNucleus(Z-1,N-2,S).GetGSMass()+mTrit-GSM;
  ////else if(B==3&&C==1) wD=0.;
  //else if(B==3&&C==2) wD=G4QNucleus(Z-2,N-1,S).GetGSMass()+mHel3-GSM;
  ////else if(B>3)  wD=0.;
  ////else if(B==4)  wD=0.;
  //else if(B==4&&C==2) wD=G4QNucleus(Z-2,N-2,S).GetGSMass()+mAlph-GSM;
  ////else if(B>4)  wD=0.;
  ////else if(B>4)  return 1.;
  //else if(B>4)wD=G4QNucleus(Z-C,N-B+C,S).GetGSMass()+G4QNucleus(C,B-C,S).GetGSMass()-GSM;
  if(wD<0.) wD=0.;
  // @@ Temporary "Virial factor" @@ __________________
  wD=wD+wD;
		// @@ End of Temporary^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  G4double sR=0.;
  //if(nA<27) sR=sqrt(wD/(E+wD));
  //else      sR=sqrt((CB+wD)/(E+wD));
  sR=sqrt((CB+wD)/(E+wD));
  //sR=sqrt(wD/(E+wD));
#ifdef pdebug
  G4cout<<"G4QN::CBPP:s="<<sR<<",E="<<E<<",w="<<wD<<",CB="<<CB<<",B="<<B<<",C="<<C<<G4endl;
#endif
  if(sR>=1.) return 0.;
  return   1.-sR*sR*sR;
} // End of "CoulBarPenProb"

// Randomize 2D-vector b = 2D impact parameter
pair<G4double, G4double> G4QNucleus::ChooseImpactXandY(G4double maxImpact)
{
  G4double x=1.;
  G4double y=1.;
  do
  {
    x = G4UniformRand();
    x = x+x-1.;
    y = G4UniformRand();
    y = y+y-1.;
  }
  while(x*x+y*y > 1.);
  theImpactParameter.first  = x*maxImpact;
  theImpactParameter.second = y*maxImpact;
  return theImpactParameter;
} // End of "ChooseImpactXandY"

// Initializes 3D Nucleons in the nucleus using random sequence
void G4QNucleus::ChooseNucleons()
{
	 G4int protons =0;
	 G4int nucleons=0;
  G4int myA=GetA();
	 while (nucleons < myA)
	 {
	   if(protons<Z && G4UniformRand() < G4double(Z-protons)/G4double(myA-nucleons) )
	   {
	     protons++;
      nucleons++;
      G4QHadron* proton = new G4QHadron(2212);
	     theNucleons.push_back(proton);
	   }
	   else if ( (nucleons-protons) < (myA-Z) )
	   {
      nucleons++;
      G4QHadron* neutron = new G4QHadron(2112);
	     theNucleons.push_back(neutron);
	   }
	   else G4cout<<"G4QNucleus::ChooseNucleons not efficient"<<G4endl;
	 }
	 return;
} // End of ChooseNucleons

// Initializes positions of 3D nucleons (@@ in QGS only 2D impact par positions are needed)
void G4QNucleus::ChoosePositions()
{
  static const G4double mProt= G4QPDGCode(2212).GetMass();
  static const G4double mProt2= mProt*mProt;
	 G4int i=0;                                  // general index
  G4int myA=GetA();                           // cashed value of A
	 G4ThreeVector	aPos;                         // Prototype of nucleon position
	 G4ThreeVector	delta;                        // Prototype of distance between nucleons
  G4ThreeVector* places = new G4ThreeVector[myA]; // Vector of 3D positions
  G4double*      placeM = new G4double[myA];  // Vector of radii
	 G4bool		freeplace;                          // flag of free space available
	 G4double nucDist2 = nucleonDistance*nucleonDistance; // @@ can be a common static
	 G4double maxR=GetRadius(0.01);              // there are no nucleons at this density
	 while(i<myA)                                // LOOP over all nucleons
	 {
	   aPos = maxR*RandomUnitSphere();           // Get random position of nucleon iside maxR
	   G4double density=GetRelativeDensity(aPos);
	   if(G4UniformRand()<density)               // Try this position with frequency ~Density
	   {
	     freeplace = true;                       // Imply that there is a free space
	     for(G4int j=0; j<i && freeplace; j++)   // Check that there is no overlap with others
	     {
	       delta = places[j] - aPos;             // Distance to nucleon j
        freeplace= delta.mag2()>nucDist2;     // If false break the LOOP (M.K.)
        if(!freeplace) break;                 // M.K. acceleration
	     }
		    //  protons must at least have binding energy of CoulombBarrier (@@ ? M.K.), so
		    //  assuming Fermi Energy corresponds to Potential, we must place protons such
		    //  that the Fermi Energy > CoulombBarrier (?)
	     if(freeplace && theNucleons[i]->GetPDGCode() == 2212) // Free Space Protons
	     {
  		    G4double pFermi=GetFermiMomentum(GetDensity(aPos));
	       G4double eFermi= sqrt(pFermi*pFermi+mProt2)-mProt; // Kinetic energy
	       if (eFermi <= CoulombBarrier()) freeplace=false;
	     }
	     if(freeplace)
	     {
		      places[i]=aPos;
		      ++i;
	     }
	   }
	 }
	 ReduceSum(places,placeM);                    // Reduce center of mass shift (equalWeight)
  for(i=0; i<myA; i++) theNucleons[i]->SetPosition(places[i]);
  delete [] places;
  delete [] placeM;
#ifdef debug
  G4cout << "G4QNucleus::ChoosePositions: A="<<myA<<G4endl;
#endif
} // End of ChoosePositions

// Initializes density of 3D nucleons in the 3D nucleus
void G4QNucleus::InitDensity()
{
  static const G4double deld=0.545*fermi;           // Surface thickness
  static const G4double r0sq=0.8133*fermi*fermi;    // Base for A-dep of rel.mean.radius
  static const G4double third=1./3.;
  G4int    iA = GetA();
  G4double rA = iA;
  G4double At = pow(rA,third);
  G4double At2= At*At;
  if(iA<17)                                         // Gaussian density distribution
		{
    radius = r0sq*At2;                              // Mean Squared Radius (fm^2)
    rho0   = pow(pi*radius, -1.5);                  // Central Density
  }
  else                                              // Wood-Saxon density distribution
		{
    G4double r0=1.16*(1.-1.16/At2)*fermi;           // Base for A-dependent radius
    G4double rd=deld/r0;                            // Relative thickness of the surface
    radius = r0*At;                                 // Half Density Radius (fm)
    rho0=0.75/(pi*pow(r0,3.)*iA*(1.+rd*rd*pi2));    // Central Density
  }
} // End of InitDensity

// Calculates Derivity of the nuclear density (for Binary Cascade, can be absolete)
G4double G4QNucleus::GetDeriv(const G4ThreeVector& aPosition)
{
  static const G4double deld=0.545*fermi;           // Surface thickness
  if(radius==0.) InitDensity();
  G4double rPos=aPosition.mag();
  if(GetA()<17) return -GetDensity(aPosition)*(rPos+rPos)/radius; // Gaussian density
  // Wood-Saxon density distribution
  G4double dens=GetRelativeDensity(aPosition);
  return -exp((rPos-radius)/deld)*dens*dens*rho0/deld;
} // End of GetDeriv

// Radius of the deffinit % of nuclear density
G4double G4QNucleus::GetRadius(const G4double maxRelDens)
{
  static const G4double deld=0.545*fermi;           // Surface thickness
  if(radius==0.) InitDensity();
  if(GetA()<17)                                     // Gaussian density distribution
    return (maxRelDens>0 && maxRelDens <= 1. ) ? sqrt(-radius*log(maxRelDens)) : DBL_MAX;
  // Wood-Saxon density distribution
  return (maxRelDens>0 && maxRelDens <= 1. ) ?
         (radius + deld*log((1.-maxRelDens+exp(-radius/deld))/maxRelDens)) : DBL_MAX;
} // End of GetRadius

// Calculates Densyty/rho0
G4double G4QNucleus::GetRelativeDensity(const G4ThreeVector& aPosition)
{
  static const G4double deld=0.545*fermi;           // Surface thickness
  if(radius==0.) InitDensity();
  if(GetA()<17)return exp(-aPosition.mag2()/radius);// Gaussian distribution
  // Wood-Saxon density distribution
  return 1./(1.+exp((aPosition.mag()-radius)/deld));
} // End of GetRelativeDensity

// Calculates modul of Fermy Momentum  depending on density
G4double G4QNucleus::GetFermiMomentum(G4double density)
{
  static const G4double third=1./3.;
  static const G4double constofpmax=hbarc*pow(3.*pi2,third);
  return constofpmax * pow(density*GetA(),third);
} // End of GetFermiMomentum

// Calculates 3D Fermy Momentuma for 3D nucleons in 3D nucleus
void G4QNucleus::ChooseFermiMomenta()
{
  static const G4double mProt= G4QPDGCode(2212).GetMass(); // Mass of proton
  static const G4double mProt2= mProt*mProt;
  static const G4double mNeut= G4QPDGCode(2112).GetMass(); // Mass of neutron
  G4int i=0;
  G4double density=0;
  G4int myA=GetA();
  G4ThreeVector* momentum = new G4ThreeVector[myA];
  G4double*      fermiM   = new G4double[myA];
	 for(i=0; i<myA; i++) // momenta for all, including the last, in case we swap nucleons
	 {
	   density=GetDensity(theNucleons[i]->GetPosition());// density around nucleon i
	   G4double ferm = GetFermiMomentum(density);        // module of momentun for nucleon i
	   fermiM[i] = ferm;                                 // module of momentun for nucleon i
	   G4ThreeVector mom=ferm*RandomUnitSphere();        // 3-vector for nucleon mpomentum
	   if(theNucleons[i]->GetPDGCode() == 2212)           // the nucleon is proton
	   {
	     G4double eMax = sqrt(ferm*ferm+mProt2)-CoulombBarrier();
	     if(eMax>mProt)
	     {
	       G4double pmax2= eMax*eMax - mProt2;
		      fermiM[i] = sqrt(pmax2);                      // modify proton momentum
		      while(mom.mag2() > pmax2) mom=Get3DFermiMomentum(density, fermiM[i]);
	     }
      else
	     {
	       G4cerr<<"G4QNucleus: difficulty finding proton momentum -> mom=0"<<G4endl;
		      mom=0;
	     }
	   }
	   momentum[i]= mom;
	 }
	 ReduceSum(momentum,fermiM);                    // Reduse momentum nonconservation
  for(i=0; i< myA ; i++ )
  {
				G4double mN=mNeut;
    if(theNucleons[i]->GetPDGCode() == 2212) mN=mProt;
    G4double energy = mN - BindingEnergy()/myA;
    G4LorentzVector tempV(momentum[i],energy);
    theNucleons[i]->Set4Momentum(tempV);
  }
  delete [] momentum;
  delete [] fermiM;
} // End of ChooseFermiMomenta

// Reduce momentum nonconservation (reenterable attempts
G4bool G4QNucleus::ReduceSum(G4ThreeVector* momentum, G4double* pFermiM)
{
  G4int myA=GetA();
  if(myA<2)                                           // Can not reduce only one nucleon
  {
    G4cout<<"-W-G4QNucleus::ReduceSum: *Failed* A="<<myA<<" < 2"<<G4endl;
    return false;
  }
  G4int i=0;                                          // To avoid declaration in j-LOOP
	 G4ThreeVector sum(0.,0.,0.);                        // The sum which must be reduced
  G4double minV=DBL_MAX;                              // Min value of Fermi Momentum
  G4double maxV=0.;                                   // Max value of Fermi Momentum
  G4int    maxI=-1;                                   // Index of maximum Fermi Momentum
	 for(i=0; i<myA; i++)
  {
    sum+=momentum[i];                                 // Make sum of 3D Momenta
    G4double pfi=pFermiM[i];                          // To avoid multiple call by index
    if(pfi<minV) minV=pfi;                            // Find the minimum Fermi Momentum
    if(pfi>maxV)                                      // Better maximum is found
    {
      maxV=pfi;                                       // Find the maximum Fermi Momentum
      maxI=i;                                         // Index of maximum Fermi Momentum
    }
  }
  minV*=0.01;                                         // Value to estimate residual (@@par)
  if(sum.mag()<minV)                                  // Probably reduction is not needed
  {
    momentum[maxI]-=sum;                              // add residual to maximum momentum
    pFermiM[maxI]=momentum[maxI].mag();               // udate only one Fermi Momentum
    return true;
  }
  G4double* fm2   = new G4double[myA];                // collect temporary squared FermiMom
	 for(i=0; i<myA; i++) fm2[i]=pFermiM[i]*pFermiM[i];  // calculate squared Fermi Momenta
  G4int myA1=myA-1;                                   // to avoid calculation in the j-LOOP
  for(G4int j=0; j<myA; j++)                          // Not more than myA attempts
  {
    G4double minP=DBL_MAX;                            // Minimal progection value
    G4int    minI=-1;                                 // Minimal projection index
	   for(i=0; i<myA; i++)                              // Sirch for minimum projection
    {
      G4double proj=fabs(sum.dot(momentum[i])/fm2[i]);// abs value of |sum|*cos(theta)
      if(proj<minP)
				  {
        minP=proj;
        minI=i;
      }
    }
    G4ThreeVector cand=momentum[minI]-sum;
    G4double      canV=cand.mag();                    // what we've got
    if(canV<0.000001)                                 // Never should come in
    {
      G4cout<<"-W-G4QNucleus::ReduceSumm: *Failed* A="<<myA<<",C="<<canV<<",j="<<j<<G4endl;
      return false;
    }
    cand = cand*pFermiM[minI]/canV;                   // pFermiM is unchanged !
    sum += cand-momentum[minI];                       // sum is updated
    momentum[minI] = cand;                            // Update direction of the momentum
    if(sum.mag()<minV || j == myA1)                   // Two resons to finish reduction
    {
      momentum[maxI]-=sum;                            // add residual to maximum momentum
      pFermiM[maxI]=momentum[maxI].mag();             // update the biggest momentum
    }
  }
 	return true;
} // End of ReduceSum

// Initializes 3D nucleus with (Pos,4M)-Nucleons. It automatically starts the LOOP
void G4QNucleus::Init3D()
{
#ifdef debug
  G4cout<<"G4QNucleus::Init3D: is called currentNucleon="<<currentNucleon<<G4endl;
#endif
  if(currentNucleon>-1) return;                       // This is a global parameter in Body
  G4int myA = GetA();
#ifdef debug
  G4cout<<"G4QNucleus::Init3D: A="<<myA<<", Z="<<Z<<G4endl;
#endif
		InitDensity();
  ChooseNucleons();  
  ChoosePositions();
  ChooseFermiMomenta();
  G4double Ebind= BindingEnergy()/myA;
  for (G4int i=0; i<myA; i++) theNucleons[i]->SetBindingEnergy(Ebind); // @@ ? M.K.
 	currentNucleon=0;                                   // Automatically starts the LOOP
  return;  
} // End of Init3D

// Get radius of the most far nucleon (+ nucleon radius)
G4double G4QNucleus::GetOuterRadius()
{
	 G4double maxradius2=0;
  G4int myA=theNucleons.size();
	 if(myA) for(G4int i=0; i<myA; i++)
	 {
		 	G4double nucr2=theNucleons[i]->GetPosition().mag2();
	   if(nucr2 > maxradius2) maxradius2=nucr2;
	 }
	 return sqrt(maxradius2)+nucleonDistance;
} // End of GetOuterRadius

//
void G4QNucleus::DoLorentzBoost(const G4LorentzVector& theBoost)
{
  G4int myA=theNucleons.size();
 	if(myA) for(G4int i=0; i<myA; i++) theNucleons[i]->Boost(theBoost);
} // End of DoLorentzBoost(G4LorentzVector)

//
void G4QNucleus::DoLorentzBoost(const G4ThreeVector& theBoost)
{
  G4int myA=theNucleons.size();
 	if(myA) for(G4int i=0; i<myA; i++) theNucleons[i]->Boost(theBoost);
} // End of DoLorentzBoost(G4ThreeVector)

//
void G4QNucleus::DoLorentzContraction(const G4ThreeVector& theBeta)
{
  G4double bet2=theBeta.mag2();
	 G4double factor=(1.-sqrt(1.-bet2))/bet2;         // 1./(beta2*gamma2)
  G4int myA=theNucleons.size();
 	if(myA) for (G4int i=0; i< myA; i++)
	 {
	   G4ThreeVector pos=theNucleons[i]->GetPosition(); 
	   pos -= factor*(theBeta*pos)*theBeta;  
	   theNucleons[i]->SetPosition(pos);
	 }    
} // End of DoLorentzContraction(G4ThreeVector)

// Shift all nucleons of the 3D Nucleus (Used in GHAD-TFT)
void G4QNucleus::DoTranslation(const G4ThreeVector& theShift)
{
  G4int myA=theNucleons.size();
 	if(myA)	for(G4int i=0; i<myA; i++)
    theNucleons[i]->SetPosition(theNucleons[i]->GetPosition() + theShift);
} // End of DoTranslation

// Initializes currentNucleon=0 returns size of theNucleons vector
G4bool G4QNucleus::StartLoop()
{
  G4int myA=theNucleons.size();
  if(myA) currentNucleon=0;
		else G4cout<<"-Warning-G4QNucleus::StartLoop: LOOP starts for uninited nucleons"<<G4endl;
	 return myA;
} // End of StartLoop

//Calculate T(b) with step .1 fm
void G4QNucleus::ActivateBThickness()
{ //             ====================
  static const G4double aT= .0008;          // pred exponent parameter
  static const G4double sT= .42;            // slope parameter
  static const G4double pT=-.26;            // power parameter
  static const G4double db= .1;             // step in b (fm)
  // @@ make better approximation for light nuclei
  G4double A = GetA();                      // atomic weight
  G4double B = aT*A*A;                      // predexponent (no units)
  G4double D = sT*std::pow(A,pT);           // b^2 slope (fm^-2)
  G4double C = A*D/pi/std::log(1.+B);       // Norm for plane density (fm^-2)
  G4double mT= C*B/(1+B);                   // Max (b=0) b-thickness
  G4double T = mT;                          // Current b-thickness
		mT/=1000.;                                // Min b-thickness (@@ make 1000 a parameter)
  G4double b = 0.;
  while(T>mT)
  {
    Tb->push_back(T);                       // Fill the thickness vector starting with b=0
    b+=db;                                  // increment impact parameter
    G4double E=B*std::exp(-D*b*b);          // b-dependent factor
    T=C*E/(1.+E);                           // T(b) in fm^-2
  }
} // End of "ActivateBThickness"

// Randomize position inside 3D UnitRadius Sphere
G4ThreeVector G4QNucleus::RandomUnitSphere()
{ //                      ==================
  G4double x=G4UniformRand(), y=G4UniformRand(), z=G4UniformRand();
  G4double r2= x*x+y*y+z*z;
  while(r2>1.)
  {
    x = G4UniformRand(); y = G4UniformRand(); z = G4UniformRand();
    r2=x*x+y*y+z*z;
  }
  return G4ThreeVector(x, y, z);
} // End of RandomUnitSphere

// Add Cluster
G4QNucleus G4QNucleus::operator+=(const G4QNucleus& rhs)
//====================================================
{
  Z+=rhs.Z;
  N+=rhs.N;
  S+=rhs.S;
  dZ+=rhs.dZ;
  dN+=rhs.dN;
  dS+=rhs.dS;
  // Atributes of aHadron
  G4int           newPDG= GetPDGCode()   + rhs.GetPDGCode() - 90000000;
  SetQPDG        (newPDG);
  G4QContent      newQC = GetQC()        + rhs.GetQC();
  SetQC          (newQC);
  G4LorentzVector newLV = Get4Momentum() + rhs.Get4Momentum();
  Set4Momentum   (newLV);
  return *this;
} 

// Subtract Cluster
G4QNucleus G4QNucleus::operator-=(const G4QNucleus& rhs)
//======================================================
{
  Z-=rhs.Z;
  N-=rhs.N;
  S-=rhs.S;
  dZ-=rhs.dZ;
  dN-=rhs.dN;
  dS-=rhs.dS;
  // Atributes of aHadron
  G4int           newPDG= GetPDGCode()   - rhs.GetPDGCode() + 90000000;
  SetQPDG        (newPDG);
  G4QContent      newQC = GetQC()        - rhs.GetQC();
  SetQC          (newQC);
  G4LorentzVector newLV = Get4Momentum() - rhs.Get4Momentum();
  Set4Momentum   (newLV);
  return *this;
} 

// Multiply Nucleus by integer value
G4QNucleus G4QNucleus::operator*=(const G4int& rhs)
//=================================================
{
  Z*=rhs;
  N*=rhs;
  S*=rhs;
  dZ*=rhs;
  dN*=rhs;
  dS*=rhs;
  // Atributes of aHadron
  G4int           newPDG= rhs*(GetPDGCode() - 90000000) + 90000000;
  SetQPDG        (newPDG);
  G4QContent      newQC = rhs*GetQC();
  SetQC          (newQC);
  G4LorentzVector newLV = rhs*Get4Momentum();
  Set4Momentum   (newLV);
  return *this;
} 

// Converts hadronic PDG Code to nuclear PDG Code
G4int G4QNucleus::HadrToNucPDG(G4int hPDG)
//========================================
{
  G4int  nPDG=hPDG;
  if     (hPDG==2212) nPDG=90001000; // p
  else if(hPDG==2112) nPDG=90000001; // n
  else if(hPDG==3122||hPDG==3212) nPDG=91000000; // Lambda
  else if(hPDG== 211) nPDG=90000999; // pi+
  else if(hPDG==-211) nPDG=89999001; // pi-
  else if(hPDG== 213) nPDG=89001000; // K0 (anti-strange)
  else if(hPDG== 213) nPDG=89000001; // K+ (anti-strange)
  else if(hPDG==-213) nPDG=90999000; // anti-K0 (strange)
  else if(hPDG==-213) nPDG=90999999; // K-      (strange)
  else if(hPDG==1114) nPDG=90001999; // Delta-
  else if(hPDG==2224) nPDG=89999002; // Delta++
  else if(hPDG==3112) nPDG=91000999; // Sigma-
  else if(hPDG==3222) nPDG=90999001; // Sigma+
  else if(hPDG==3312) nPDG=91999999; // Ksi-
  else if(hPDG==3322) nPDG=91999000; // Ksi0
  else if(hPDG==3334) nPDG=92998999; // Omega-
  return nPDG;
} 

// Converts nuclear PDG Code to hadronic PDG Code
G4int G4QNucleus::NucToHadrPDG(G4int nPDG)
//========================================
{
  G4int  hPDG=nPDG;
  if     (nPDG==90001000) hPDG=2212; // p
  else if(nPDG==90000001) hPDG=2112; // n
  else if(nPDG==91000000) hPDG=3122; // Lambda
  else if(nPDG==90000999) hPDG= 211; // pi+
  else if(nPDG==89999001) hPDG=-211; // pi-
  else if(nPDG==89001000) hPDG= 213; // K0 (anti-strange)
  else if(nPDG==89000001) hPDG= 213; // K+ (anti-strange)
  else if(nPDG==90999000) hPDG=-213; // anti-K0 (strange)
  else if(nPDG==90999999) hPDG=-213; // K-      (strange)
  else if(nPDG==90001999) hPDG=1114; // Delta-
  else if(nPDG==89999002) hPDG=2224; // Delta++
  else if(nPDG==91000999) hPDG=3112; // Sigma-
  else if(nPDG==90999001) hPDG=3222; // Sigma+
  else if(nPDG==91999999) hPDG=3312; // Ksi-
  else if(nPDG==91999000) hPDG=3322; // Ksi0
  else if(nPDG==92998999) hPDG=3334; // Omega-
  return hPDG;
} 

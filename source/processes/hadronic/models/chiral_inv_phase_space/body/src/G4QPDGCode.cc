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
// $Id: G4QPDGCode.cc,v 1.31 2003-09-09 09:13:41 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QPDGCode ----------------
//             by Mikhail Kossov, Sept 1999.
//      class for Hadron definitions in CHIPS Model
// -------------------------------------------------------------------

//#define debug
//#define pdebug

#include "G4QPDGCodeVector.hh"

G4QPDGCode::G4QPDGCode(G4int PDGCode): thePDGCode(PDGCode)
{
  if(PDGCode) theQCode=MakeQCode(PDGCode);
  else        
  {
#ifdef pdebug
    G4cout<<"***G4QPDGCode: Constructed with PDGCode=0, QCode=-2"<<G4endl;  
#endif
    theQCode=-2;
  }
}

G4QPDGCode::G4QPDGCode(G4QContent QCont)
{
  InitByQCont(QCont);
}

G4QPDGCode::G4QPDGCode(const G4QPDGCode& rhs)
{
  thePDGCode =rhs.thePDGCode;
  theQCode   =rhs.theQCode;
}

G4QPDGCode::G4QPDGCode(G4QPDGCode* rhs)
{
  thePDGCode =rhs->thePDGCode;
  theQCode   =rhs->theQCode;
}

const G4QPDGCode& G4QPDGCode::operator=(const G4QPDGCode& rhs)
{
  thePDGCode =rhs.thePDGCode;
  theQCode   =rhs.theQCode;

  return *this;
}

G4QPDGCode::~G4QPDGCode() {}

// Standard output for QPDGCode
G4std::ostream& operator<<(G4std::ostream& lhs, G4QPDGCode& rhs)
//       =========================================
{
  lhs << "[ PDG=" << rhs.GetPDGCode() << ", Q=" << rhs.GetQCode() << "]";
  return lhs;
}

// Standard output for const QPDGCode
G4std::ostream& operator<<(G4std::ostream& lhs, const G4QPDGCode& rhs)
//       ===============================================
{
  lhs << "[ PDG=" << rhs.GetPDGCode() << ", Q=" << rhs.GetQCode() << "]";
  return lhs;
}

// Overloading of QPDGCode addition
G4int operator+(const G4QPDGCode& lhs, const G4QPDGCode& rhs)
//         =======================================================
{
  G4int  s  = lhs.GetPDGCode();
  return s += rhs.GetPDGCode();
}
G4int operator+(const G4QPDGCode& lhs, const G4int& rhs)
//         =======================================================
{
  G4int  s  = lhs.GetPDGCode();
  return s += rhs;
}
G4int operator+(const G4int& lhs, const G4QPDGCode& rhs)
//         =======================================================
{
  G4int  s  = lhs;
  return s += rhs.GetPDGCode();
}

// Overloading of QPDGCode subtraction
G4int operator-(const G4QPDGCode& lhs, const G4QPDGCode& rhs)
//         =======================================================
{
  G4int  s  = lhs.GetPDGCode();
  return s -= rhs.GetPDGCode();
}
G4int operator-(const G4QPDGCode& lhs, const G4int& rhs)
//         =======================================================
{
  G4int  s  = lhs.GetPDGCode();
  return s -= rhs;
}
G4int operator-(const G4int& lhs, const G4QPDGCode& rhs)
//         =======================================================
{
  G4int  s  = lhs;
  return s -= rhs.GetPDGCode();
}

// Overloading of QPDGCode multiplication
G4int operator*(const G4QPDGCode& lhs, const G4QPDGCode& rhs)
//         =======================================================
{
  G4int  s  = lhs.GetPDGCode();
  return s *= rhs.GetPDGCode();
}
G4int operator*(const G4QPDGCode& lhs, const G4int& rhs)
//         =======================================================
{
  G4int  s  = lhs.GetPDGCode();
  return s *= rhs;
}
G4int operator*(const G4int& lhs, const G4QPDGCode& rhs)
//         =======================================================
{
  G4int  s  = lhs;
  return s *= rhs.GetPDGCode();
}

// Overloading of QPDGCode division
G4int operator/(const G4QPDGCode& lhs, const G4QPDGCode& rhs)
{//   =======================================================
  G4int  s  = lhs.GetPDGCode();
  return s /= rhs.GetPDGCode();
}
G4int operator/(const G4QPDGCode& lhs, const G4int& rhs)
{//   ==================================================
  G4int  s  = lhs.GetPDGCode();
  return s /= rhs;
}
G4int operator/(const G4int& lhs, const G4QPDGCode& rhs)
{//   ==================================================
  G4int  s  = lhs;
  return s /= rhs.GetPDGCode();
}

// Overloading of QPDGCode residual
G4int operator%(const G4QPDGCode& lhs, const G4int& rhs)
{//   ==================================================
  G4int  s  = lhs.GetPDGCode();
  return s %= rhs;
}

// TRUE if it is not RealNeutral (111,221,331 etc), FALSE if it is.
G4bool G4QPDGCode::TestRealNeutral(const G4int& PDGCode)
{//    =================================================
  if(PDGCode>0 && PDGCode<999)    // RealNeutral are always positive && mesons
  {
    if(PDGCode==22) return false; // Photon
    G4int p=PDGCode/10;
    if(p/10==p%10) return false; // This is a RealNeutral
  }
  return true;
}

// Make a Q Code out of the PDG Code
G4int G4QPDGCode::MakePDGCode(const G4int& QCode)
{//   ===========================================
  //static const G4int modi = 81;      // Q Codes for more than di-baryon nuclei
  static const G4int modi = 89;      // Q Codes for more than di-baryon nuclei "IsoNuclei"
  static G4int qC[modi] = { 22,  110,  220,  330,  111,  211,  221,  311,  321,  331,
                          2112, 2212, 3122, 3112, 3212, 3222, 3312, 3322,  113,  213,
                           223,  313,  323,  333, 1114, 2114, 2214, 2224, 3124, 3114,
                          3214, 3224, 3314, 3324, 3334,  115,  215,  225,  315,  325,
                           335, 2116, 2216, 3126, 3116, 3216, 3226, 3316, 3326,  117,
                           217,  227,  317,  327,  337, 1118, 2118, 2218, 2228, 3128,
                          3118, 3218, 3228, 3318, 3328, 3338,  119,  219,  229,  319,
                           329,  339, 90002999 , 89999003 , 90003998 , 89998004 ,
                           90003999 , 89999004 , 90004998 , 89998005 ,
                           90000001 , 90001000 ,  91000000 ,  90000002 ,
                           90001001 , 90002000 ,  91000001 ,  91001000 ,  92000000};
  static G4int aC[15] = {1,1000,999001,1000000,1000999,1999000,1999999,        // sum 1
                         2,1001,2000,1000001,1001000,1999001,2000000,2000999}; // sum 2
  if      (QCode<0)
  {
    G4cerr<<"***G4QPDGCode::MakePDGCode: negative Q Code ="<<QCode<<G4endl;
    return 0;
  }
  else if (QCode>=modi)
  {
    G4int q=QCode-modi;              // Starting BarNum=3
    G4int a=q/15+1;                  // BarNum/2
    G4int b=q%15;
    return 90000000+a*1001+aC[b];
  }
  return qC[theQCode];
}

// Make a Q Code out of the PDG Code
G4int G4QPDGCode::MakeQCode(const G4int& PDGCode)
{//   ===========================================
  G4int PDGC=abs(PDGCode);        // Qcode is always not negative
  if (PDGC<80000000)                // Baryons & Mesons
  {
    if(PDGC>10000000)  return -2; // Impossible code
    if     (PDGC==10)  return -1; // Chipolino
    else if(PDGC==22)  return  0; // Photon
    else if(PDGC==110) return  1; // Low R-P: Sigma (pi,pi S-wave)
    else if(PDGC==220) return  2; // Midle Regeon-Pomeron
    else if(PDGC==330) return  3; // High Regeon-Pomeron
    G4int p=PDGC/10;              // Quark Content
    G4int r=PDGC%10;              // 2s+1
    G4int         Q= 0;
    if     (!r)
    {
#ifdef pdebug
      G4cout<<"***G4QPDGCode::MakeQCode: (0) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
      return -2;
    }
    else if(r==1) Q= 3;
    else if(r==2) Q= 9;
    else if(r==3) Q=17;
    else if(r==4) Q=23;
    else if(r==5) Q=34;
    else if(r==6) Q=40;
    else if(r==7) Q=48;
    else if(r==8) Q=54;
    else if(r==9) Q=65;     // @@ Not yet implemented family of mesons with spin 4
    if(r%2)                 // Mesons are all the same
	{
      if     (p==11) return Q+=1;
      else if(p==21) return Q+=2;
      else if(p==22) return Q+=3;
      else if(p==31) return Q+=4;
      else if(p==32) return Q+=5;
      else if(p==33) return Q+=6;
      else
      {
#ifdef pdebug
        G4cout<<"***G4QPDGCode::MakeQCode: (1) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
        return -2;
      }
	}
    else                    // Baryons
	{
      G4int s=r/2;
      if(s%2)               // N Family
	  {
        if     (p==211) return Q+=1;
        else if(p==221) return Q+=2;
        else if(p==312) return Q+=3;
        else if(p==311) return Q+=4;
        else if(p==321) return Q+=5;
        else if(p==322) return Q+=6;
        else if(p==331) return Q+=7;
        else if(p==332) return Q+=8;
        else
        {
#ifdef pdebug
          G4cout<<"***G4QPDGCode::MakeQCode: (2) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
          return -2;
        }
	  }
	  else                  // Delta Family
	  {
        if     (p==111) return Q+= 1;
        else if(p==211) return Q+= 2;
        else if(p==221) return Q+= 3;
        else if(p==222) return Q+= 4;
        else if(p==312) return Q+= 5;
        else if(p==311) return Q+= 6;
        else if(p==321) return Q+= 7;
        else if(p==322) return Q+= 8;
        else if(p==331) return Q+= 9;
        else if(p==332) return Q+=10;
        else if(p==333) return Q+=11;
        else
        {
#ifdef pdebug
          G4cout<<"***G4QPDGCode::MakeQCode: (3) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
          return -2;
        }
	  }
	}
  }
  else                        // Nuclear Fragments
  {
    if(PDGC==90002999) return 71;
    if(PDGC==89999003) return 72;
    if(PDGC==90003998) return 73;
    if(PDGC==89998004) return 74;
    if(PDGC==90003999) return 75;
    if(PDGC==89999004) return 76;
    if(PDGC==90004998) return 77;
    if(PDGC==89998005) return 78;
    G4int r=PDGC-90000000;      // cut the fake 90000000 for the Normal and Hypernuclei
    if(!r) return -2;
    G4int ds=0;
    G4int dz=0;
    G4int dn=0;
    if(r<-100000)
    {
      G4int ns=(-r)/1000000+1;
      r+=ns*1000000;
      ds+=ns;
    }
    else if(r<-100)
    {
      G4int nz=(-r)/1000+1;
      r+=nz*1000;
      dz+=nz;
    }
    else if(r<0)
    {
      G4int nn=-r;
      r=0;
      dn+=nn;
    }
    G4int sz =r/1000;
    G4int n=r%1000;
    if(n>700)
    {
      n-=1000;
      dz--;
    }
    G4int z=sz%1000-dz;
    if(z>700)
    {
      z-=1000;
      ds--;
    }
    G4int s=sz/1000-ds;
    G4int d=n+n+z+s;          // a#of d quarks
    G4int u=n+z+z+s;          // a#of u quarks
    G4int t=d+u+s;            // tot#of quarks
    if(t%3 || abs(t)<3)       // b=0 are in mesons
    {
#ifdef pdebug
      G4cout<<"***G4QPDGCode::MakeQCode: Unknown PDGCode="<<PDGCode<<", t="<<t<<G4endl;
#endif
      return -2;
	}
    else
	{
      G4int b=t/3;            // baryon number
      if(b==1)                // baryons
      {
        if     (s==0&&u==1&&d==2) return 80;
        else if(s==0&&u==2&&d==1) return 81;
        else if(s==1&&u==1&&d==1) return 82;
        else
        {
#ifdef pdebug
          G4cout<<"***G4QPDGCode::MakeQCode: (5) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
          return -2;
        }
      }
      else if(b==2)           // di-baryons
      {
        if     (s==0&&u==2&&d==4) return 83;
        else if(s==0&&u==3&&d==3) return 84;
        else if(s==0&&u==4&&d==2) return 85;
        else if(s==1&&u==2&&d==3) return 86;
        else if(s==1&&u==3&&d==2) return 87;
        else if(s==2&&u==2&&d==2) return 88;
        else
        {
#ifdef pdebug
          G4cout<<"***G4QPDGCode::MakeQCode: (6) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
          return -2;
        }
      }
      G4int c=b/2;
      G4int g=b%2;
      G4int h=c*3;
      //G4int Q=57+c*15;
      G4int Q=65+c*15;           // "IsoNuclei"
      u-=h;
      d-=h;
      if(g)
      {
        if     (s==0&&u==1&&d==2) return Q+= 9;
        else if(s==0&&u==2&&d==1) return Q+=10;
        else if(s==1&&u==0&&d==2) return Q+=11;
        else if(s==1&&u==1&&d==1) return Q+=12;
        else if(s==1&&u==2&&d==0) return Q+=13;
        else if(s==2&&u==0&&d==1) return Q+=14;
        else if(s==2&&u==1&&d==0) return Q+=15;
        else
        {
#ifdef debug
          G4cout<<"***G4QPDGCode::MakeQCode: (7) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
          return -2;
        }
      }
      else
	  {
        if     (s==0&&u==-1&&d== 1) return Q+=1;
        else if(s==0&&u== 0&&d== 0) return Q+=2;
        else if(s==0&&u== 1&&d==-1) return Q+=3;
        else if(s==1&&u==-1&&d== 0) return Q+=4;
        else if(s==1&&u== 0&&d==-1) return Q+=5;
        else if(s==2&&u==-2&&d== 0) return Q+=6;
        else if(s==2&&u==-1&&d==-1) return Q+=7;
        else if(s==2&&u== 0&&d==-2) return Q+=8;
        else
        {
#ifdef debug
          G4cout<<"***G4QPDGCode::MakeQCode: (8) Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
          return -2;
        }
	  }
	}
  }
#ifdef pdebug
  G4cout<<"***G4QPDGCode::MakeQCode: () Unknown in Q-System code: "<<PDGCode<<G4endl;
#endif
// return -2; not reachable statement  
}

// Get the mean mass value for the PDG
G4double G4QPDGCode::GetMass()
{//      =====================
  //static const int nM = 72;
  static const int nM = 80;
  static G4double m[nM] = {0., 980., 780., 1500., 134.98, 139.57, 547.3, 497.67, 493.68, 957.78
    , 939.5656, 938.2723, 1115.684,  1197.44, 1192.55, 1189.37, 1321.3, 1314.9,   770.,   770.
    ,   781.94,    896.1,   891.66, 1019.413,   1232.,   1232.,  1232.,  1232., 1519.5, 1387.2
    ,   1383.7,   1382.8,    1535.,   1531.8, 1672.45,  1318.1, 1318.1,  1275., 1432.4, 1425.6
    ,    1525.,    1680.,    1680.,    1820.,   1915.,   1915.,  1915.,  2025.,  2025.,  1691.
    ,    1691.,    1667.,    1776.,    1776.,   1854.,   1950.,  1950.,  1950.,  1950.,  2100.
    ,    2030.,    2030.,    2030.,    2127.,   2127.,   2252.,  2020.,  2020.,  2044.,  2045.
	,    2045.,    2297., 2170.272, 2171.565,  2464.,  2464., 3108.544,3111.13, 3402.272, 3403.565
    };
  //static G4int n[15]={-1, 0, 1,-1, 0,-2,-1, 0, 0, 1,-1, 0, 1,-1, 0}; 
  //static G4int z[15]={ 1, 0,-1, 0,-1, 0,-1,-2, 1, 0, 1, 0,-1, 0,-1};
  //static G4int s[15]={ 0, 0, 0, 1, 1, 2, 2, 2, 0, 0, 1, 1, 1, 2, 2};
  //
  static G4int n[15]={ 1, 0, 1, 0,-1, 0,-1, 2, 1, 0, 1, 0, 1, 0,-1}; 
  static G4int z[15]={ 0, 1,-1, 0, 1,-1, 0, 0, 1, 2, 0, 1,-1, 0, 1};
  static G4int s[15]={ 0, 0, 1, 1, 1, 2, 2, 0, 0, 0, 1, 1, 2, 2, 2};
  //...........................................................................
  G4int ab=theQCode;
  if(abs(thePDGCode)>80000000)             // Nuclear coding
  {
    G4int szn=thePDGCode-90000000;
    //if(szn==0) return m[4];                // mPi0   %%???? MK ????
    if(szn==0) return 0.;
    G4int ds=0;
    G4int dz=0;
    G4int dn=0;
    if(szn<-100000)
    {
      G4int ns=(-szn)/1000000+1;
      szn+=ns*1000000;
      ds+=ns;
    }
    else if(szn<-100)
    {
      G4int nz=(-szn)/1000+1;
      szn+=nz*1000;
      dz+=nz;
    }
    else if(szn<0)
    {
      G4int nn=-szn;
      szn=0;
      dn+=nn;
    }
    G4int sz =szn/1000;
    G4int n  =szn%1000;
    if(n>700)
    {
      n-=1000;
      dz--;
    }
    G4int z  =sz%1000-dz;
    if(z>700)
    {
      z-=1000;
      ds--;
    }
    G4int s  =sz/1000-ds;
    return GetNuclMass(z,n,s);
  }
  else if(ab<0||!thePDGCode)
  {
#ifdef debug
    if(thePDGCode!=10)
      G4cout<<"***G4QPDGCode::GetMass: m=100000., QCode="<<theQCode<<",PDGCode="<<thePDGCode<<G4endl;
#endif
    return 100000.;
  }
  if(ab<nM) return m[ab];
  G4int a=ab-66;
  G4int c=a%15;
  G4int b=a/15;      // b_base/2
  if (ab<89)
  {
    if     (ab==80) return GetNuclMass(0,1,0); //  n
    else if(ab==81) return GetNuclMass(1,0,0); //  p
    else if(ab==82) return GetNuclMass(0,0,1); //  L
    else if(ab==83) return GetNuclMass(0,2,0); // nn
    else if(ab==84) return GetNuclMass(1,1,0); //  d
    else if(ab==85) return GetNuclMass(2,0,0); // pp
    else if(ab==86) return GetNuclMass(0,1,1); // nL
    else if(ab==87) return GetNuclMass(1,0,1); // pL
    else if(ab==88) return GetNuclMass(0,0,2); // LL
  }
  return GetNuclMass(b+z[c],b+n[c],s[c]);
}

// Get the width value for the PDG
G4double G4QPDGCode::GetWidth()
//      =====================
{
  //static const int nW = 72;
  static const int nW = 80;
  static G4double width[nW] = {   0.,  70., 450., 112.,   0.,   0., .00118,  0.,   0., .203
                              ,   0.,   0.,   0.,   0.,   0.,   0.,   0.,    0., 160., 160.
                              , 8.41, 50.5, 50.8, 4.43, 120., 120., 120.,  120., 15.6,  39.
                              ,  36., 35.8,  10.,   9.,   0., 107., 107., 185.5, 109., 98.5
                              ,  76., 130., 130.,  80., 120., 120., 120.,   20.,  20., 160.
                              , 160., 168., 159., 159.,  87., 300., 300.,  300., 300., 200.
                              , 180., 180., 180.,  99.,  99.,  55., 387.,  387., 208., 198.
							  , 198., 149., 120., 120., 170., 170., 120.,  120., 170., 170.
                              };
  G4int ab=abs(theQCode);
  if(ab<nW) return width[ab];
  return 0.;                  // @@ May be real width should be implemented for nuclei e.g. pp
}

// Get a nuclear mass for Z (a#of protons), N (a#of neutrons), & S (a#of lambdas) 
G4double G4QPDGCode::GetNuclMass(G4int Z, G4int N, G4int S)
//      ===================================================
{
  static const G4double mP  = G4QPDGCode(2212).GetMass();
  static const G4double mN  = G4QPDGCode(2112).GetMass();
  static const G4double mL  = G4QPDGCode(3122).GetMass(); // Lambda
  //////////static const G4double mSm = G4QPDGCode(3112).GetMass(); // Sigma-
  //////////static const G4double mSp = G4QPDGCode(3222).GetMass(); // Sigma+
  //////////static const G4double mKm = G4QPDGCode(3312).GetMass(); // Ksi-
  //////////static const G4double mKz = G4QPDGCode(3322).GetMass(); // Ksi0
  static const G4double mK  = G4QPDGCode( 321).GetMass();
  static const G4double mK0 = G4QPDGCode( 311).GetMass();
  static const G4double mPi = G4QPDGCode( 211).GetMass();
  //////////static const G4double mPi0= G4QPDGCode( 111).GetMass();
  static const G4int    nSh = 164;
  static G4double sh[nSh] = {0.,                        // Fake element for C++ N=Z=0
                               -4.315548,   2.435504,  -1.170501,   3.950887,   5.425238,
                                13.342524,  15.547586,  22.583129,  23.983480,  30.561036,
                                33.761971,  41.471027,  45.532156,  53.835880,  58.495514,
                                65.693445,  69.903344,  76.899581,  81.329361,  88.979438,
                                92.908703, 100.316636, 105.013393, 113.081686, 118.622601,
                               126.979113, 132.714435, 141.413182, 146.433488, 153.746754,
                               158.665225, 165.988967, 170.952395, 178.473011, 183.471531,
                               191.231310, 196.504414, 204.617158, 210.251108, 218.373984,
                               223.969281, 232.168660, 237.925619, 246.400505, 252.392471,
                               260.938644, 267.191321, 276.107788, 282.722682, 291.881502,
                               296.998590, 304.236025, 309.562296, 316.928655, 322.240263,
                               329.927236, 335.480630, 343.233705, 348.923475, 356.911659,
                               362.785757, 370.920926, 376.929998, 385.130316, 391.197741,
                               399.451554, 405.679971, 414.101869, 420.346260, 428.832412,
                               435.067445, 443.526983, 449.880034, 458.348602, 464.822352,
                               473.313779, 479.744524, 488.320887, 495.025069, 503.841579,
                               510.716379, 519.451976, 525.036156, 532.388151, 537.899017,
                               545.252264, 550.802469, 558.402181, 564.101100, 571.963429,
                               577.980340, 586.063802, 592.451334, 600.518525, 606.832027,
                               614.831626, 621.205330, 629.237413, 635.489106, 643.434167,
                               649.691284, 657.516479, 663.812101, 671.715021, 678.061128,
                               686.002970, 692.343712, 700.360477, 706.624091, 714.617848,
                               721.100390, 729.294717, 735.887170, 744.216084, 751.017094,
                               759.551764, 766.377807, 775.080204, 781.965673, 790.552795,
                               797.572494, 806.088030, 813.158751, 821.655631, 828.867137,
                               836.860955, 842.183292, 849.195302, 854.731798, 861.898839,
                               867.783606, 875.313342, 881.443441, 889.189065, 895.680189,
                               903.679729, 910.368085, 918.579876, 925.543547, 933.790028,
                               940.811396, 949.122548, 956.170201, 964.466810, 971.516490,
                               979.766905, 986.844659, 995.113552,1002.212760,1010.418770,
                              1018.302560,1025.781870,1033.263560,1040.747880,1048.234460,
                              1055.723430,1063.214780,1070.708750,1078.204870,1085.703370,
                              1093.204260,1100.707530,1108.213070};
  static const G4double b1=8.09748564; // MeV
  static const G4double b2=-0.76277387;
  static const G4double b3=83.487332;  // MeV
  static const G4double b4=0.090578206;// 2*b4
  static const G4double b5=0.676377211;// MeV
  static const G4double b6=5.55231981; // MeV
  static const G4double b7=25.;        // MeV (Lambda binding energy predexponent)
  // even-odd difference is 3.7(MeV)/X
  // S(X>151)=-57.56-5.542*X**1.05
  static const G4double b8=10.5;       // (Lambda binding energy exponent)
  static const G4double b9=-1./3.;
  static const G4double a2=0.13;       // Lambda binding energy for the deutron+Lambda system (MeV)
  static const G4double a3=2.2;        // Lambda binding energy for (t or He3)+Lambda system (MeV)
  static const G4double um=931.49432;  // Umified atomic mass unit (MeV)
  static const G4double me =0.511;     // electron mass (MeV) :: n:8.071, p:7.289
  static G4double c[9][9]={// z=1     =2     =3     =4     =5     =6     =7     =8     =9
                            {13.136,14.931,25.320,38.000,45.000,55.000,65.000,75.000,85.000},  //n=1
				            {14.950, 2.425,11.680,18.374,27.870,35.094,48.000,60.000,72.000},  //n=2
				            {25.930,11.390,14.086,15.770,22.921,28.914,39.700,49.000,60.000},  //n=3
				            {36.830,17.594,14.908, 4.942,12.416,15.699,24.960,32.048,45.000},  //n=4
				            {41.860,26.110,20.946,11.348,12.051,10.650,17.338,23.111,33.610},  //n=5
				            {45.000,31.598,24.954,12.607, 8.668, 0.000, 5.345, 8.006,16.780},  //n=6
				            {50.000,40.820,33.050,20.174,13.369, 3.125, 2.863, 2.855,10.680},  //n=7
				            {55.000,48.810,40.796,25.076,16.562, 3.020, 0.101,-4.737,1.9520},  //n=8
				            {60.000,55.000,50.100,33.660,23.664, 9.873, 5.683,-0.809,0.8730}}; //n=9
#ifdef debug
  G4cout<<"G4QPDGCode::GetNuclMass called with Z="<<Z<<",N="<<N<<", S="<<S<<G4endl;
#endif
  if(!N&&!Z&&!S)   
  {
#ifdef debug
    G4cout<<"G4QPDGCode::GetNuclMass(0,0,0)="<<mPi0<<"#0"<<G4endl;
#endif
    //return mPi0;
    return 0.;
  }
  G4int A=Z+N;
  G4int Bn=A+S;
  //if((Z<0||N<0)&&!Bn)
  //{
  //  if     (N<0) return Bn*mL-Z*mK - N*mK0+0.001*   S ;
  //  else              return Bn*mL+N*mPi-A*mK +0.001*(N+S);
  //}
  if(A<0&&Bn>=0)                      // Bn*LAMBDA's + (-(N+Z))*Kaons
  {
    if     (N<0&&Z<0) return Bn*mL-Z*mK -N*mK0+0.001*   S ;
    else if(N<0)      return Bn*mL+Z*mPi-A*mK0+0.001*(Z+S);
    else              return Bn*mL+N*mPi-A*mK +0.001*(N+S);
  }
  if(!Bn)                      // => "GS Mesons - octet" case (without eta&pi0)
  {
    if     (N==1&&Z==-1||N==-1&&Z==1) return mPi;
    else if(N==1&&S==-1||N==-1&&S==1) return mK0;
    else if(S==1&&Z==-1||S==-1&&Z==1) return mK;
    else return 0.;                 // Exotic mesons (4q)
  }
  if(Bn==1)                    // => "GS Baryons - decuplet" case (withoutSigma0)
  {
    if     (Z== 1 && N== 0 && S== 0) return mP;
    else if(Z== 0 && N== 1 && S== 0) return mN;
    else if(Z== 0 && N== 0 && S== 1) return mL;
    // @@ For future improvement of Octet baryon production (@@change decays!)
    //else if(Z== 1 && N==-1 && S== 1) return mSp;  // Lower than LAMBDA + pi+
    //else if(Z==-1 && N== 1 && S== 1) return mSm;  // Lower than LAMBDA + pi-
    //else if(Z== 0 && N==-1 && S== 2) return mKz;  // Lower than LAMBDA + K0
    //else if(Z==-1 && N== 0 && S== 2) return mKm;  // Lower than LAMBDA + K-
  }
  // === Start mesonic extraction ===
  G4double k=0.;                      // Mass Sum of K+ mesons
  if(S<0)                             // @@ Can be improved by K0/K+ search of minimum
  {
    if(Bn<0) return 0.;               // @@ Make it for antinuclei @@ (Bn<0)
    if(Z>0)
	{
      if(Z>=-S)                       // => "Enough protons in nucleus" case
	  {
        k=-S*mK;
        Z+=S;
	  }
      else
	  {
        k=Z*mK-(S+Z)*mK0;
        N+=S+Z;
        if(N<0) return 0.;            // @@ Antiparticles aren't implemented @@
        Z=0;
	  }
	}
    else if(N>0)
	{
      if(N>=-S)                         // => "Enough neutrons in nucleus" case
	  {
        k-=S*mK0;
        N+=S;
	  }
      else
	  {
        k+=N*mK0-(S+N)*mK;
        Z+=S+N;
        if(Z<0) return 0.;            // @@ Antiparticles aren't implemented @@
        N=0;
	  }
	}
    else return 0.;                   // @@ Antiparticles aren't implemented @@
    S=0;
  }
  if(N<0)
  {
    k+=-N*mPi;
    Z+=N;                             // A=Z+N>=0
    N=0;
  }
  if(Z<0)
  {
    k+=-Z*mPi;
    N+=Z;                             // A=Z+N>=0
    Z=0;
  }
  A=Z+N;
  if (!A) return k+S*mL+S*.001;       // @@ multy LAMBDA states are not implemented
  G4double m=k+A*um;                  // Expected mass in atomic units
  G4double D=N-Z;                     // Isotopic shift of the nucleus
  if(A+S<1&&k==0.||Z<0||N<0)          // @@ Can be generalized to anti-nuclei
  {
#ifdef debug
    G4cerr<<"***G4QPDGCode::GetNuclMass: A="<<A<<"<1 || Z="<<Z<<"<0 || N="<<N<<"<0"<<G4endl;
    //@@G4Exception("***G4QPDGCode::GetNuclMass: Impossible nucleus");
#endif
    return 0.;                        // @@ Temporary
  }
  if     (!Z) return k+N*(mN+.1)+S*(mL+.1);  // @@ n+LAMBDA states are not implemented
  else if(!N) return k+Z*(mP+1.)+S*(mL+.1);  // @@ p+LAMBDA states are not implemented
  else if(N<=9&&Z<=9) m+=1.433e-5*pow(double(Z),2.39)-Z*me+c[N-1][Z-1];
  else 
  {
    if(G4NucleiPropertiesTable::IsInTable(Z,A)) m=k+G4NucleiProperties::GetNuclearMass(A,Z);
    else m+=-sh[Z]-sh[N]+b1*D*D*pow(A,b2)+b3*(1.-2./(1.+exp(b4*D)))+Z*Z*(b5*pow(A,b9)+b6/A);
  }
  G4double maxM= k+Z*mP+N*mN+S*mL+.001;       // @@ .001 ?? Wings of the Mass parabola
  if(m>maxM) m=maxM;
  if(S>0)
  {
    G4double bs=0.;
    if     (S==2) bs=a2;
    else if(S==3) bs=a3;
    else if(S>3)  bs=b7*exp(-b8/(A+1.));
    m+=S*(mL-bs);
  }  
#ifdef debug
  G4cout<<"G4QPDGCode::GetNuclMass: >>>OUT<<< m="<<m<<G4endl;
#endif
  return m;
}

// Get a Quark Content {d,u,s,ad,au,as} for the PDG
G4QContent G4QPDGCode::GetQuarkContent() const
//      ======================================
{
  G4int            a=0; // particle
  if(thePDGCode<0) a=1; // anti-particle
  G4int d=0;
  G4int u=0;
  G4int s=0;
  G4int ad=0;
  G4int au=0;
  G4int as=0;
  G4int ab=abs(thePDGCode);
  if     (!ab)
  {
    G4cerr<<"***G4QPDGCode::GetQuarkContent: PDG=0, return (0,0,0,0,0,0)"<<G4endl;
    return G4QContent(0,0,0,0,0,0);
  }
  else if(ab==22 || ab==10)
  {
#ifdef debug
    if     (ab==22) G4cout<<"***G4QPDGCode::GetQuarkContent: For the Photon? - Return 0"<<G4endl;
    else if(ab==10) G4cout<<"***G4QPDGCode::GetQuarkContent: For the Chipolino? - Return 0"<<G4endl;
#endif
    return G4QContent(0,0,0,0,0,0); // Photon
  }
  else if(ab<80000000) // Baryons & Mesons
  {
    G4int c=ab/10;     // temporary (quarks)
    G4int f=c%10;      // (1) low quark
    G4int v=c/10;      // (2,3) temporary(B) or (2) final(M) (high quarks, high quark)
    G4int t=0;         // (3)prototype of highest quark (B)
#ifdef debug
	G4cout<<"G4QPDGCode::GetQuarkContent: ab="<<ab<<", c="<<c<<", f="<<f<<", v="<<v<<G4endl;
#endif
    if(v>10)           // Baryons
	{
      t=v/10;          // (3) highest quark
      v%=10;           // (2) high quark
      if     (f==1)
	  {
        if(a) ad++;
        else   d++;
	  }
      else if(f==2)
	  {
        if(a) au++;
        else   u++;
	  }
      else if(f==3)
	  {
        if(a) as++;
        else   s++;
	  }
      else G4cerr<<"***G4QPDGCode::GetQContent:1 PDG="<<thePDGCode<<","<<f<<","<<v<<","<<t<<G4endl;
      if     (v==1)
	  {
        if(a) ad++;
        else   d++;
	  }
      else if(v==2)
	  {
        if(a) au++;
        else   u++;
	  }
      else if(v==3)
	  {
        if(a) as++;
        else   s++;
	  }
      else G4cerr<<"***G4QPDGCode::GetQContent:2 PDG="<<thePDGCode<<","<<f<<","<<v<<","<<t<<G4endl;
      if     (t==1)
	  {
        if(a) ad++;
        else   d++;
	  }
      else if(t==2)
	  {
        if(a) au++;
        else   u++;
	  }
      else if(t==3)
	  {
        if(a) as++;
        else   s++;
	  }
      else G4cerr<<"***G4QPDGCode::GetQCont:3 PDG="<<thePDGCode<<","<<f<<","<<v<<","<<t<<G4endl;
      return G4QContent(d,u,s,ad,au,as);
	}
    else        // Mesons
	{
      if(f==v)
	  {
        if     (f==1) return G4QContent(1,0,0,1,0,0);
        else if(f==2) return G4QContent(0,1,0,0,1,0);
        else if(f==3) return G4QContent(0,0,1,0,0,1);
        else G4cerr<<"***G4QPDGCode::GetQCont:4 PDG="<<thePDGCode<<",i="<<f<<","<<v<<","<<t<<G4endl;
	  }
      else
	  {
        if     (f==1 && v==2)
        {
          if(a)return G4QContent(1,0,0,0,1,0);
          else return G4QContent(0,1,0,1,0,0);
        }
        else if(f==1 && v==3)
        {
          if(a)return G4QContent(0,0,1,1,0,0);
          else return G4QContent(1,0,0,0,0,1);
        }
        else if(f==2 && v==3)
        {
          if(a)return G4QContent(0,0,1,0,1,0);
          else return G4QContent(0,1,0,0,0,1);
        }
        else G4cerr<<"***G4QPDGCode::GetQCont:5 PDG="<<thePDGCode<<","<<f<<","<<v<<","<<t<<G4endl;
	  }
	}
  }
  else                    
  {
    G4int szn=ab-90000000;
    G4int ds=0;
    G4int dz=0;
    G4int dn=0;
    if(szn<-100000)
    {
      G4int ns=(-szn)/1000000+1;
      szn+=ns*1000000;
      ds+=ns;
    }
    else if(szn<-100)
    {
      G4int nz=(-szn)/1000+1;
      szn+=nz*1000;
      dz+=nz;
    }
    else if(szn<0)
    {
      G4int nn=-szn;
      szn=0;
      dn+=nn;
    }
    G4int sz =szn/1000;
    G4int n  =szn%1000;
    if(n>700)
    {
      n-=1000;
      dz--;
    }
    G4int z  =sz%1000-dz;
    if(z>700)
    {
      z-=1000;
      ds--;
    }
    s  =sz/1000-ds;
    G4int b=z+n+s;
    d=n+b;
    u=z+b;
    if      (d<0&&u<0&&s<0) return G4QContent(0,0,0,-d,-u,-s);
    else if (u<0&&s<0)      return G4QContent(d,0,0,0,-u,-s);
    else if (d<0&&s<0)      return G4QContent(0,u,0,-d,0,-s);
    else if (d<0&&u<0)      return G4QContent(0,0,s,-d,-u,0);
    else if (u<0)           return G4QContent(d,0,s,0,-u,0);
    else if (s<0)           return G4QContent(d,u,0,0,0,-s);
    else if (d<0)           return G4QContent(0,u,s,-d,0,0);
    else                    return G4QContent(d,u,s,0,0,0);
  }
  return G4QContent(0,0,0,0,0,0);
}

// Quark Content of pseudo-meson for q_i->q_o Quark Exchange
G4QContent G4QPDGCode::GetExQContent(G4int i, G4int o)  const
{//        ==================================================
  G4QContent cQC(0,0,0,0,0,0);
  if     (!i)   cQC.IncAD();
  else if(i==1) cQC.IncAU();
  else if(i==2) cQC.IncAS();
  else G4cerr<<"***G4QPDGCode::GetExQContent: strange entering quark i="<<i<<G4endl;
  if     (!o)   cQC.IncD();
  else if(o==1) cQC.IncU();
  else if(o==2) cQC.IncS();
  else G4cerr<<"***G4QPDGCode::GetExQContent: strange exiting quark o="<<o<<G4endl;
  return cQC;
}


// Relative Cross Index for q_i->q_o (7 means there is no such cluster)
G4int G4QPDGCode::GetRelCrossIndex(G4int i, G4int o)  const
{//   =====================================================
  static const G4int b01[ 3]={ 1, 7, 7};
  static const G4int b02[ 3]={ 2, 7, 7};
  static const G4int b10[ 3]={ 7,-1, 7};                                     // Baryons
  static const G4int b12[ 3]={ 7, 1, 7};
  static const G4int b20[ 3]={ 7, 7,-2};
  static const G4int b21[ 3]={ 7, 7,-1};
  static const G4int d01[ 6]={ 1, 1, 7, 1, 7, 7};
  static const G4int d02[ 6]={ 3, 3, 7, 2, 7, 7};
  static const G4int d10[ 6]={ 7,-1,-1, 7,-1, 7};                            // Dibaryons
  static const G4int d12[ 6]={ 7, 2, 2, 7, 1, 7};
  static const G4int d20[ 6]={ 7, 7, 7,-3,-3,-2};
  static const G4int d21[ 6]={ 7, 7, 7,-2,-2,-1};
  static const G4int m01[15]={ 1, 7, 1, 1, 7, 1, 7, 1, 1, 7, 1, 7, 1, 1, 7}; // Multibaryons
  static const G4int m02[15]={ 3, 3, 3, 3, 7, 7, 7, 3, 3, 7, 3, 3, 7, 7, 7}; // @@ Regular !!!!
  static const G4int m10[15]={ 7,-1, 7,-1,-1, 7,-1, 7,-1,-1, 7,-1, 7,-1,-1}; // 01-> 1, 10-> -1
  static const G4int m12[15]={ 2, 2, 7, 2, 2, 7, 7, 7, 2, 2, 2, 2, 7, 7, 7}; // 12-> 2, 21-> -2
  static const G4int m20[15]={ 7, 7, 7,-3,-3,-3,-3, 7, 7, 7,-3,-3, 7,-3,-3}; // 02-> 3, 20-> -3 
  static const G4int m21[15]={ 7, 7,-2,-2, 7,-2,-2, 7, 7, 7,-2,-2,-2,-2, 7};
  // === I<->O Reversed version ===
  //static const G4int b01[ 3]={ 7,-1, 7};
  //static const G4int b02[ 3]={ 7, 7,-2};
  //static const G4int b10[ 3]={ 1, 7, 7};                                     // Baryons
  //static const G4int b12[ 3]={ 7, 7,-1};
  //static const G4int b20[ 3]={ 2, 7, 7};
  //static const G4int b21[ 3]={ 7, 1, 7};
  //static const G4int d01[ 6]={ 7,-1,-1, 7,-1, 7};
  //static const G4int d02[ 6]={ 7, 7, 7,-3,-3,-2};
  //static const G4int d10[ 6]={ 1, 1, 7, 1, 7, 7};                            // Dibaryons
  //static const G4int d12[ 6]={ 7, 7, 7,-2,-2,-1};
  //static const G4int d20[ 6]={ 3, 3, 7, 2, 7, 7};
  //static const G4int d21[ 6]={ 7, 2, 2, 7, 1, 7};
  //static const G4int m01[15]={ 7,-1, 7,-1,-1, 7,-1, 7,-1,-1, 7,-1, 7,-1,-1}; // Multibaryons
  //static const G4int m02[15]={ 7, 7, 7,-3,-3,-3,-3, 7, 7, 7,-3,-3, 7,-3,-3}; // @@ Regular !!!!
  //static const G4int m10[15]={ 1, 7, 1, 1, 7, 1, 7, 1, 1, 7, 1, 7, 1, 1, 7}; // 01-> 1, 10-> -1
  //static const G4int m12[15]={ 7, 7,-2,-2, 7,-2,-2, 7, 7, 7,-2,-2,-2,-2, 7}; // 12-> 2, 21-> -2
  //static const G4int m20[15]={ 3, 3, 3, 3, 7, 7, 7, 3, 3, 7, 3, 3, 7, 7, 7}; // 02-> 3, 20-> -3 
  //static const G4int m21[15]={ 2, 2, 7, 2, 2, 7, 7, 7, 2, 2, 2, 2, 7, 7, 7};
  //static const G4int fragmStart = 72;
  static const G4int fragmStart = 80;    // "Isonuclei are added"

  if(theQCode<fragmStart)
  {
#ifdef debug
    G4cerr<<"***G4QPDGCode::RelGetCrossIndex: not nucleus QCode="<<theQCode<<G4endl;
#endif
    return 7;
  }
  G4int sub=theQCode-fragmStart;
  G4int rel=sub;                         // case of nuclear baryons
  if     (sub>8)rel =(sub-9)%15;         // case of heavy fragments (BaryNum>2)
  else if(sub>2)rel = sub-3;             // case of nuclear di-baryon
#ifdef debug
	G4cout<<"G4QPDGCode::RelGetCrossIndex:i="<<i<<",o="<<o<<",sub="<<sub<<",rel="<<rel<<G4endl;
#endif
  if     (!i)
  {
    if     (!o)      return 0;
    else if(o==1)
    {
      if     (sub<3) return b01[rel];
      else if(sub<9) return d01[rel];
      else           return m01[rel];
    }
    else if(o==2)
    {
      if     (sub<3) return b02[rel];
      else if(sub<9) return d02[rel];
      else           return m02[rel];
    }
    else G4cerr<<"***G4QPDGCode::RelGetCrossIndex: strange exiting quark (i=0) o="<<o<<G4endl;
  }
  else if(i==1)
  {
    if     (!o)
    {
      if     (sub<3) return b10[rel];
      else if(sub<9) return d10[rel];
      else           return m10[rel];
    }
    else if(o==1) return 0;
    else if(o==2)
    {
      if     (sub<3) return b12[rel];
      else if(sub<9) return d12[rel];
      else           return m12[rel];
    }
    else G4cerr<<"***G4QPDGCode::RelGetCrossIndex: strange exiting quark (i=0) o="<<o<<G4endl;
  }
  else if(i==2)
  {
    if     (!o)
    {
      if     (sub<3) return b20[rel];
      else if(sub<9) return d20[rel];
      else           return m20[rel];
    }
    else if(o==1)
    {
      if     (sub<3) return b21[rel];
      else if(sub<9) return d21[rel];
      else           return m21[rel];
    }
    else if(o==2) return 0;
    else G4cerr<<"***G4QPDGCode::RelGetCrossIndex: strange exiting quark (i=0) o="<<o<<G4endl;
  }
  else G4cerr<<"***G4QPDGCode::RelGetCrossIndex: strange entering quark i="<<i<<G4endl;
  return 0;
}

// Get number of Combinations for q_i->q_o
G4int G4QPDGCode::GetNumOfComb(G4int i, G4int o) const
{//   ================================================
  if(i>-1&&i<3)
  {
    G4int shiftQ=GetRelCrossIndex(i, o);
    G4int sQCode=theQCode;                        // QCode of the parent cluster
    if     (shiftQ==7) return 0;                  // A parent cluster doesn't exist
    else if(!shiftQ) sQCode+=shiftQ;              // Shift QCode of son to QCode of his parent
    G4QPDGCode parent;                            // Create a temporary (fake) parent cluster
    parent.InitByQCode(sQCode);                   // Initialize it by Q-Code
    G4QContent parentQC=parent.GetQuarkContent(); // Quark Content of the parent cluster
    if     (!o)   return parentQC.GetD();
    else if(o==1) return parentQC.GetU();
    else if(o==2) return parentQC.GetS();
    else G4cerr<<"***G4QPDGCode:::GetNumOfComb: strange exiting quark o="<<o<<G4endl;
  }
  else G4cerr<<"***G4QPDGCode:::GetNumOfComb: strange entering quark i="<<i<<G4endl;
  return 0;
}

// Get a total number of Combinations for q_i
G4int G4QPDGCode::GetTotNumOfComb(G4int i) const
{//   ==========================================
  G4int tot=0;
  if(i>-1&&i<3) for(int j=0; j<3; j++) tot+=GetNumOfComb(i, j);
  else G4cerr<<"***G4QPDGCode:::GetTotNumOfComb: strange entering quark i="<<i<<G4endl;
  return tot;
}






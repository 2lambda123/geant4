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
// G4 Tools program: NuMu DIS (x,Q2) approximation is integrated over x
// .....................................................
// Created: M.V. Kossov, CERN/ITEP(Moscow), 30-Sept-05
// 
//=====================================================================
#include "globals.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include "G4ios.hh"
#include <CLHEP/GenericFunctions/LogGamma.hh>

void strucf(int A, double x, double ydME, double& f2, double& xf3, double& fL)
{
  const Genfun::LogGamma lGam;
  double N=3., D=0., Del=0., U2=0., U3=0., V=0.;
  double Q2=x*ydME;
  double r=0.;
  double max=1.;
  double H=1.22;
  if(A==1)                   // Proton
  {
    r=sqrt(Q2/1.66);
    max=.5;
  }
  else if(A<13)              // Light nuclei
  {
    double f=Q2/4.62;
    r=f*f;
    max=.3;
    if(A>2) H=1.;
  }
  else if(A>0)               // Heavy nuclei
  {
    double f=Q2/3.4;
    double ff=f*f;
    r=ff*ff;
    max=.5;
    H=1.;
  }
  else G4cout<<"strucf: A="<<A<<" <= 0"<< G4endl;
  //
  N=3.+.3581*log(1.+Q2/.04); // a#of partons in the nonperturbative phase space
  Del=(1.+r)/(12.5+r/max);
  double S=pow(1.+.6/Q2,-1.-Del);
  D=H*S*(1.-.5*S);
  V=3*(1.-D)*(N-1.);
  double cc=Q2/.08;
  double cc2=cc*cc;
  double C=(1.+cc2)/(1.+cc2/.24)/(1.+Q2/21.6);
  double c3=C+C+C;
  double uu=exp(lGam(N-Del)-lGam(N-1.)-lGam(1.-Del))/N;
  U2=(c3+N-3.)*uu;
  U3=c3*uu;
  // From here the Q2 coefficients are used
  double x1=pow(1.-x,N-2.);
  double pp=D*pow(x,-Del)*x1;
  double dir=V*x*x1;
  double per=U2*pp;
		f2 = per + dir;
  xf3= U3*pp+dir;
  fL = per/4.;
  return;
}

void getFun(int A, double lx, double ydME, double* f)
{
  double f2=0., xf3=0., fL=0.;
  if (lx>0.5) G4cerr<<"***getFun: ln(x)="<<lx<<">.5"<<G4endl;
  double x=exp(lx);
  strucf(A, x, ydME, f2, xf3, fL);
  f[0]=x*f2;          // direct part
  f[1]=x*(-f2+xf3);   // *y (neutrino) part
  f[2]=x*(-f2-xf3);   // *y (anti-neutrino) part
  f[3]=x*(f2-fL-xf3); // *y2 (neutrino) part
  f[4]=x*(f2-fL+xf3); // *y2 (anti-neutrino) part
}

int main()
{
  const double reps=.01;        // relative accuracy of the total Q2 integral calculation
  const double xeps=.0001;      // relative accuracy of the total X integral calculation
  //           =========
  const double GF=1.16637e-5;   // Fermi constant in GeV^-2
  const double GF2=GF*GF;       // Squared Fermi constant in GeV^-4
  const double MW=80.425;       // Mass of W-boson in GeV
  const double MW2=MW*MW;       // Squared mass of W-boson in GeV^2
  const double MW4=MW2*MW2;     // Quadro mass of W-boson in GeV^4
  const double hc2=38937932300.;// (hc)^2 in GeV^2*10^-38cm2 to convert GeV^-2 to 10^-38cm2
  const double pif=3.14159265*4;// 4pi
  const double sik=GF2*MW4*hc2/pif; // precalculated coefficient
  const double mpi=.1349766;    // pi0 meson mass in GeV
  const double mpi2=mpi*mpi;    // m_pi^2 in GeV^2
  const double me=.00051099892; // electron mass in GeV
  const double me2=me*me;       // m_e^2 in GeV^2
  const double hme2=me2/2;      // .5*m_e^2 in GeV^2
  const double mmu=.105658369;  // mu meson mass in GeV
  const double mmu2=mmu*mmu;    // m_mu^2 in GeV^2
  const double hmmu2=mmu2/2;    // .5*m_mu^2 in GeV^2
  const double mtau=1.777;      // tau meson mass in GeV
  const double mtau2=mtau*mtau; // m_tau^2 in GeV^2
  const double hmtau2=mtau2/2;  // .5*m_e^2 in GeV^2
  const double mp=.93827203;    // proton mass in GeV
  const double mn=.93956536;    // neutron mass in GeV
  const double md=1.87561282;   // deuteron mass in GeV
  const double MN=.931494043;   // Nucleon mass (inside nucleus) in GeV (atomic mass unit)
  const double mp2=mp*mp;       // m_p^2 in GeV^2
  const double MN2=MN*MN;       // M_N^2 in GeV^2
  const double dMN=MN+MN;       // 2*M_N in GeV
  const double EminE=me+me2/dMN;// Threshold for muon production
  const double EminMu=mmu+mmu2/dMN;  // Threshold for muon production
  const double EminTau=mmu+mmu2/dMN; // Threshold for muon production
  //std::ofstream fileNuMuX("NuMuXQ2.out", std::ios::out);
  //fileNuMuX.setf( std::ios::scientific, std::ios::floatfield );
  // _____ Begin of Test Area
  //Genfun::LogGamma logGamma;
  //double n=4.9;
		//double g=exp(logGamma(n));
  //G4cout<<"Gamma("<<n<<") = "<<g<<G4endl;
  // ^^^^^ End of Test Area
  //
  double f[5];                    // A working array
  int    A=12;                    // Neucleus for which calculations should be done
  double lEnuMin=log(EminMu);     // Log of Minimum energy of neutrino
  double lEnuMax=log(300.);       // Log of Maximum energy of neutrino
  int    nE=20;
  double dlE=(lEnuMax-lEnuMin)/nE;
  G4cout<<"Emin="<<EminMu<<",lEi="<<lEnuMin<<",lEa="<<lEnuMax<<",dlE="<<dlE<<G4endl;
  for(double lEnu=lEnuMin+dlE/2; lEnu<lEnuMax; lEnu+=dlE)
		{
    double Enu=exp(lEnu);         // Energy of neutrino/anti-neutrino
    double dEnu=Enu+Enu;          // doubled energy of nu/anu
    double Enu2=Enu*Enu;          // squared energy of nu/anu
    double ME=Enu*MN;             // M*E
    double dME=ME+ME;             // 2*M*E
    double DIStsig=1.;            // Total curent DIS cross-section to be integrated
    double DISmsig=1.e20;         // Total remembered DIS cross-section
    double dEMN=(dEnu+MN)*ME;
    double MEm=ME-hmmu2;
    double sqE=Enu*sqrt(MEm*MEm-mmu2*MN2);
    double E2M=MN*Enu2-(Enu+MN)*hmmu2;
    double ymax=(E2M+sqE)/dEMN;
    double ymin=(E2M-sqE)/dEMN;
    // The Q2min/max calculation is not necessary in this method
    //double rmin=1.-ymin;
    //double rhm2E=hmmu2/Enu2;
    //double Q2min=(Enu2+Enu2)*(rmin-rhm2E-sqrt(rmin*rmin-rhm2E-rhm2E));
    //double Q2max=dME*ymax;
    int    nY=8;
    //G4cout<<"*** E="<<Enu<<", N="<<nY<<", yi="<<ymin<<" < ya="<<ymax<<G4endl;
    while(fabs(DIStsig-DISmsig)/DIStsig>reps)
    {
      DISmsig=DIStsig;
      DIStsig=0.;
      nY*=2;
      double dY=(ymax-ymin)/nY;
      for(double y=ymin+dY/2; y<ymax; y+=dY)
				  {
        double y2=y*y;
        double DISxint=1.;        // Curent DIS x-integral
        double DISmint=1.e20;     // Remembered DIS x-integral
        double lXmin=log((sqrt(Enu2+mmu2)-Enu)/MN);
        double lXmax=0.;          // Temporary, while QES is in DIS
        double ydME=y*dME;
        int    nX=8;
        while(fabs(DISxint-DISmint)/DISxint>xeps)
        {
          DISmint=DISxint;
          DISxint=0.;
          nX*=2;
          double dlX=(lXmax-lXmin)/nX;
          for(double lX=lXmin+dlX/2; lX<lXmax; lX+=dlX)
				      {
            getFun(A, lX, ydME, f);
            double dik=exp(lX)*ydME+MW2;
            //DISxint+=(f[0]+f[0]+(y+y)*f[1]+y2*f[3])/dik/dik; // neutrino
            DISxint+=(f[0]+f[0]+(y+y)*f[2]+y2*f[4])/dik/dik; // anti-neutrino
            //G4cout<<f[0]<<","<<f[1]<<","<<f[2]<<","<<f[3]<<","<<f[4]<<G4endl;
          }
          DISxint*=dlX;
          //G4cout<<"--- E="<<Enu<<" --- Q2="<<Q2<<" --- nX="<<nX<<", iX="<<DISxint
          //      <<", mX="<<DISmint<<", rX="<<(DISxint-DISmint)/DISxint<<G4endl;
        }
        //G4cout<<"(E="<<Enu<<"), Q2="<<Q2<<", I="<<DISxint/dik/dik<<G4endl;
        DIStsig+=DISxint;
      }
      DIStsig*=dY;
      //G4cout<<"=== E="<<Enu<<" ===> nQ="<<nQ2<<", iQ="<<DIStsig<<", mQ="<<DISmsig
						//      <<", rQ="<<(DIStsig-DISmsig)/DIStsig<<G4endl;
    }
    DIStsig*=sik*dMN; // Jakobian is 2ME => instead of 1/E must be *2M
    G4cout<<"*** E="<<Enu<<",sig/E="<<DIStsig<<G4endl;
		} // End of the Enery LOOP
  // int np=0;
  //for(int m=0; m<2; m++)
  //{
  //  //fileNuMuX<<"  static const G4double SH"<<n<<"[nH]={"<<G4endl<<"    ";
  //  //G4cout<<"**** A_high="<<m<<G4endl;
  //  np=0;
  //  int nC=14;
	 //  for(G4int en=0; en<nC; en++)
	 //  {
  //    //G4double sig=1.;
  //    np++;
  //    //if(np==7)           // Write by 7 number in brackets
  //    //{
  //    //  if(en==nC-1) fileNuMuX<<sig<<"};"<<G4endl;
  //    //  else         fileNuMuX<<sig<<","<<G4endl<<"    ";
	 //    //}
  //    //else           fileNuMuX<<sig<<",";
  //    //if(np==7) np=0;
  //  } // End of the point LOOP
  //} // End of the isotop LOOP
  return EXIT_SUCCESS;
}

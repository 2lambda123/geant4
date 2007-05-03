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
// $Id: G4ElasticHadrNucleusHE.cc,v 1.59 2007-05-03 14:56:06 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//  The generator of high energy hadron-nucleus elastic scattering
//  The hadron kinetic energy T > 1 GeV
//  N.  Starkov 2003.
//
//  19.05.04 Variant for G4 6.1: The 'ApplyYourself' was changed
//  19.11.05 The HE elastic scattering on proton is added (N.Starkov)
//  16.11.06 The low energy boundary is shifted to T = 400 MeV (N.Starkov)
//  23.11.06 General cleanup, ONQ0=3, use pointer instead of particle name (VI)
//  02.05.07 Scale sampled t as p^2 (VI)
//

#include  "G4ElasticHadrNucleusHE.hh"
#include  "Randomize.hh"
#include  "G4ios.hh"
#include  "G4ParticleTable.hh"
#include  "G4IonTable.hh"

using namespace std;

//  ############################################################
ElasticData:: ElasticData(const G4ParticleDefinition* p, 
			  G4int A, G4double* eGeV)
{ 
  hadr     = p;
  massGeV  = p->GetPDGMass()/GeV;
  AtomicWeight = A;

  DefineNucleusParameters(A);

  limitQ2 = 35./(R1*R1);     //  (GeV/c)^2
  G4double dQ2 = limitQ2/(ONQ2 - 1.);

  TableQ2[0] = 0.0;
  for(G4int ii=1; ii<ONQ2; ii++) {
    TableQ2[ii] = TableQ2[ii-1]+dQ2;
  }

  for(G4int kk=0; kk<NENERGY; kk++) {
    dnkE[kk] = 0;
    G4double Q2m = 4.0*eGeV[kk]*(eGeV[kk] + 2.0*massGeV);
    maxQ2[kk] = std::min(limitQ2, Q2m);
    TableCrossSec[ONQ2*kk] = 0.0;
  }
}

//  ##########################################################
void ElasticData::DefineNucleusParameters(G4int Nucleus)
{
  if(Nucleus == 208)
    {
      R1       = 20.5;    //  26.09.06
      R2       = 15.74;
      Pnucl    = 0.4;
      Aeff     = 0.7;
    }

  else if(Nucleus == 90)
    {
      R1    = 16.5*1.1;
      R2    = 11.62;
      Pnucl = 0.4;
      Aeff  = 0.7;
    }

  else if(Nucleus == 58)
    {
      R1    = 15.0*1.05;
      R2    = 9.9;
      Pnucl = 0.45;
      Aeff  = 0.85;
    }
  else if(Nucleus == 16)
    {
      R1    = 10.50;
      R2    = 5.5;
      Pnucl = 0.7;
      Aeff  = 0.98;
    }

  else if(Nucleus == 9)
    {
      R1    = 9.0;
      R2    = 7.0;
      Pnucl = 0.190;
      Aeff  = 0.9;
    }

  else if(Nucleus == 4)
    {
      R1    = 6.0;   //  26.09.06
      R2    = 3.7;
      Pnucl = 0.4;
      Aeff  = 0.87;
   }

  else
    {
      R1    = 4.45*std::pow(static_cast<double>(Nucleus-1),0.309)*0.9;
      R2    = 2.3*std::pow(static_cast<double>(Nucleus),0.36);
      Pnucl = 0.176+0.00167*Nucleus+8.69E-6*Nucleus*Nucleus;
      Aeff  = 0.9;
    }
  //   G4cout<<" Nucl.Par. "<<Nucleus<<"  R1  "<<R1<<G4endl;
}

//  ####### The constructor for the generating of events #######
G4ElasticHadrNucleusHE::G4ElasticHadrNucleusHE()
{
  MbToGeV2  =  2.568;
  sqMbToGeV =  1.602;
  Fm2ToGeV2 =  25.68;
  GeV2      =  GeV*GeV;

  Binom();
  // energy in GeV
  Energy[0] = 0.4;
  Energy[1] = 0.6;
  Energy[2] = 0.8;
  LowEdgeEnergy[0] = 0.0;
  LowEdgeEnergy[1] = 0.5;
  LowEdgeEnergy[2] = 0.7;
  G4double e = 1.0;
  G4double f = std::pow(10.,0.1);
  for(G4int i=3; i<NENERGY; i++) {
    Energy[i] = e;
    LowEdgeEnergy[i] = e/f;
    e *= f*f;
  }
  verboselevel = 0;
}

//  ####### The destructor for the generating of events #######
G4ElasticHadrNucleusHE::~G4ElasticHadrNucleusHE()
{
  size_t  SizeData = SetOfElasticData.size();
  if( SizeData != 0)
    {
      for(size_t kk = 0; kk<SizeData; kk++)
	{
	   delete SetOfElasticData[kk];
	}
    }
}

//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
G4double G4ElasticHadrNucleusHE::SampleT(
                          const G4ParticleDefinition * p,
                          G4double inLabMom, 
                          G4int,  G4int N)
{
  G4double pTotLabMomentum = inLabMom/GeV; // (GeV/c)
  G4double Q2 = 0;

  HadrCode = p->GetPDGEncoding();

  if(N>1) {    

    ElasticData * ElD1 = 0;

    size_t  SizeData = SetOfElasticData.size();

    G4int NumberOfRecord = -1;

    if(verboselevel > 1)
      G4cout<<" SampleT: SizeData "<<SizeData<<G4endl;
//  .........................................
    if( SizeData > 0) {
      for(size_t kk = 0; kk<SizeData; kk++) {

	ElD1 = SetOfElasticData[kk];
	if( ElD1->AtomicWeight == N && ElD1->Hadron() == p) {
	  NumberOfRecord = kk;
	  break;
	}
      }
    }       // if SizeData!=0
//  ...........................................
    if(NumberOfRecord == -1) {
      ElD1 = new  ElasticData(p, N, Energy);
      SetOfElasticData.push_back(ElD1);
      if(verboselevel > 1)
	G4cout<< " G4ElasticHadrNucleusHE::SampleT:  new record " <<NumberOfRecord
	      << " for " << p->GetParticleName() << " A= " << N
	      << G4endl;
    }  

    Q2 = HadronNucleusQ2_2(p, ElD1, pTotLabMomentum);

  }  

  else  Q2 = HadronProtonQ2(p, pTotLabMomentum);
  
  if(verboselevel > 1)
    G4cout<<" SampleT: Q2(GeV^2)= "<<Q2<<G4endl;

  return  Q2*GeV2;
}

//  ########################################################
G4HadFinalState * G4ElasticHadrNucleusHE::ApplyYourself(
                          const  G4HadProjectile  &aHadron,
                                 G4Nucleus        &aNucleus)
{
  G4Nucleus * aNucl      = &aNucleus;

  theParticleChange.Clear();

//  ---------------  Hadron Definition  ---------------

  G4DynamicParticle    * dParticle = &aHad;
  G4ParticleDefinition * hadrDef =
    const_cast<G4ParticleDefinition *>(aHadron.GetDefinition());

  G4ThreeVector  hadrMomentum = aHadron.Get4Momentum().vect();
  dParticle->SetDefinition(hadrDef);
  dParticle->SetMomentum(hadrMomentum);

//  ---------------  Nucleus Definition  ---------------
  G4double A = aNucl->GetN();
  G4int nA   = G4int(A + 0.5);
  G4int nZ   = G4int(aNucl->GetZ());

  G4ParticleDefinition * secNuclDef = 0;

  if(nZ == 1 && nA == 1)      secNuclDef = G4Proton::Proton();
  else if(nZ == 1 && nA == 2) secNuclDef = G4Deuteron::Deuteron();
  else if(nZ == 1 && nA == 3) secNuclDef = G4Triton::Triton();
  else if(nZ == 2 && nA == 3) secNuclDef = G4He3::He3();
  else if(nZ == 2 && nA == 4) secNuclDef = G4Alpha::Alpha();
  else secNuclDef = G4ParticleTable::
                         GetParticleTable()->FindIon(nZ,nA,0,nZ);
//  ----------------------------------------------------
  G4double ranQ2;

  ranQ2 = SampleT(hadrDef, dParticle->GetTotalMomentum(), nZ, nA);

  Q2res = ranQ2;

//  ----------------  Hadron kinematics  ----------------
  G4double m1 = hadrDef->GetPDGMass();
  G4double m2 = secNuclDef->GetPDGMass();
  G4LorentzVector lv1 = dParticle->Get4Momentum();
  G4LorentzVector lv0(0.0,0.0,0.0,m2);
  G4LorentzVector lv1test = dParticle->Get4Momentum();
  G4LorentzVector lv0test(0.0,0.0,0.0,m2);

  G4LorentzVector lv  = lv0 + lv1;
  G4ThreeVector bst = lv.boostVector();
  lv1.boost(-bst);
  lv0.boost(-bst);
  G4ThreeVector p1 = lv1.vect();
  G4double ptot = p1.mag();

  // Sampling in CM system
  G4double phi  = G4UniformRand()*twopi;
  G4double cost = 1. - 0.5*ranQ2/(ptot*ptot);

  if(std::abs(cost) > 1.0) cost = 2.0*G4UniformRand() - 1.0;
  G4double sint = std::sqrt((1.0-cost)*(1.0+cost));

  G4ThreeVector v1(sint*std::cos(phi),sint*std::sin(phi),cost);
  p1 = p1.unit();
  v1.rotateUz(p1);
  v1 *= ptot;
  G4LorentzVector nlv1(v1.x(),v1.y(),v1.z(),
                            std::sqrt(ptot*ptot + m1*m1));
  G4LorentzVector nlv0 = lv0 + lv1 - nlv1;
  nlv0.boost(bst);
  nlv1.boost(bst);


  lv1 = nlv1-lv1test;
  lv0 = nlv0-lv0test;

  Q2res = -lv0.m2();

  G4double eFinal = nlv1.e() - m1;

  if(eFinal < 0.0) 
  {
    G4cout << "G4HadronElastic(Apply) WARNING ekin= " << eFinal
           << " after scattering of "
           << hadrDef->GetParticleName()
           << " p(GeV/c)= " << dParticle->GetTotalMomentum()/GeV
           << " on " << secNuclDef->GetParticleName()
           << G4endl;
    eFinal = 0.0;
  }

  theParticleChange.SetMomentumChange(nlv1.vect().unit());
  theParticleChange.SetEnergyChange(eFinal);

  G4DynamicParticle * aSec = new G4DynamicParticle(secNuclDef, nlv0);
  theParticleChange.AddSecondary(aSec);

  if(verboselevel > 1)
    G4cout<<G4endl<<"----------- End Applay ------------"<<G4endl;

  return &theParticleChange;
}

//  ########################################################
G4double G4ElasticHadrNucleusHE::HadronNucleusQ2_2(
			      const G4ParticleDefinition * aHadron,
                              ElasticData* pElD,
                              G4double LabMom)
{

  G4double Rand = G4UniformRand();

  G4int      iNumbQ2 = 0;
  G4double   Q2 = 0.0, Buf = 0.0;

  //  iContr = 2;

  G4double hadrMass = pElD->massGeV;
  G4double ptot2 = LabMom*LabMom;
  G4double ekin  = std::sqrt(hadrMass*hadrMass+ptot2)-hadrMass;

  G4int NumbOnE = 0; 
  for(; NumbOnE < NENERGY-1; NumbOnE++) {
    if(ekin <= LowEdgeEnergy[NumbOnE+1]) break;
  }

  G4double* dNumbQ2 = pElD->TableQ2;

  G4int index = NumbOnE*ONQ2;

  G4double Weight= 1.0;
  G4double rmax  = 1.0;

  G4double T     = Energy[NumbOnE];
  G4double P2    = T*(T + 2.*hadrMass);
  G4double Q2max = pElD->maxQ2[NumbOnE];
  G4int length   = pElD->dnkE[NumbOnE];

  // Build first part of the vector
  if(length == 0) {
   
    R1    = pElD->R1;
    R2    = pElD->R2;
    Aeff  = pElD->Aeff;
    Pnucl = pElD->Pnucl;
    G4double P = sqrt(P2);
    GetHadronValues(aHadron, P);
    G4int AWeight = pElD->AtomicWeight;
    Weight = GetLightFq2(AWeight, Q2max);

    if(verboselevel > 1)
      G4cout<<" HadrNucleusQ2_2: NumbOnE= " << NumbOnE 
	    << " length= " << length 
	    << " Weight "<<Weight
	    << " Q2max= " << Q2max 
	    << " ekin= " << ekin <<G4endl;

    pElD->TableCrossSec[index] = 0;

    for(G4int ii=1; ii<ONQ0; ii++)
      {
	Q2 = pElD->TableQ2[ii];

	if(Q2 < Q2max) Buf = GetLightFq2(AWeight, Q2)/Weight;
	else           Buf = 1.0;

	pElD->TableCrossSec[index+ii] = Buf;

	if(verboselevel > 1)
	  G4cout<<" HadrNucleusQ2_2: ii= " << ii << " Q2= "
		<<Q2 <<" p= " <<Buf<<" B*W "<<Buf*Weight<<G4endl;
      }   // for ii

    rmax    = Buf;
    length  = ONQ0;
    pElD->dnkE[NumbOnE] = ONQ0;

  } else {
    rmax = pElD->TableCrossSec[index+length-1];
  }

  G4double* dNumbFQ2 = &(pElD->TableCrossSec[index]);

  // No more vector needed
  if(rmax >= Rand) {

    for(iNumbQ2 = 1; iNumbQ2<length; iNumbQ2++) {
      if(Rand <= pElD->TableCrossSec[index+iNumbQ2]) break;
    }

  // Build second part of the vector
  } else {
    
    R1    = pElD->R1;
    R2    = pElD->R2;
    Aeff  = pElD->Aeff;
    Pnucl = pElD->Pnucl;
    G4double P = sqrt(P2);
    GetHadronValues(aHadron, P);
    G4int AWeight = pElD->AtomicWeight;
    Weight = GetLightFq2(AWeight, Q2max);
     
    // Stop building when find out the node
    for(iNumbQ2 = length; iNumbQ2<ONQ2; iNumbQ2++) {

      Q2 = pElD->TableQ2[iNumbQ2];

      if(Q2 < Q2max) Buf = GetLightFq2(AWeight, Q2)/Weight;
      else           Buf = 1.0;

      pElD->TableCrossSec[index+iNumbQ2] = Buf;
      if(verboselevel > 1)
	G4cout<<" HadrNucleusQ2_2: NumbOnE= " << NumbOnE 
	      << " iNumbQ2= " << iNumbQ2 << " Q2= "
	      <<Q2 <<" Buf= " <<Buf<<" B*W "<<Buf*Weight<<G4endl;
      if(Rand <= Buf) {
	pElD->dnkE[NumbOnE] = iNumbQ2+1;
	break;
      }
    }   
  }

  Q2 = GetQ2_2(iNumbQ2, dNumbQ2, dNumbFQ2, Rand);
  //  if(Q2max < pElD->limitQ2) 
  Q2 *= ptot2/P2;

  if(verboselevel > 1)
    G4cout<<" HadrNucleusQ2_2(2): Q2= "<<Q2<<" iNumbQ2= " << iNumbQ2 
	  << " rand= " << Rand << G4endl;

  return Q2;

}       

//  +++++++  The randomization of one dimensional array ++++++
G4double G4ElasticHadrNucleusHE::GetQ2_2(G4int kk, G4double * Q,
					 G4double * F, G4double ranUni)
{
  G4double ranQ2;
  G4double F2  = F[kk-1];
  G4double F3  = F[kk];
  G4double X2  = Q[kk-1];
  G4double X3  = Q[kk];

  if(kk <= 2)
    {
      ranQ2 = X2 + (ranUni - F2)*(X3 - X2)/(F3 - F2);
      return ranQ2;
    }

  G4double F1  = F[kk-2];

  G4double F12 = F1*F1;
  G4double F22 = F2*F2;
  G4double F32 = F3*F3;

  G4double X1  = Q[kk-2];          //  MeV^2

  G4double D0  = F12*F2+F1*F32+F3*F22-F32*F2-F22*F1-F12*F3;

  if(std::abs(D0) < 0.00000001)
    { 
      ranQ2 = X2 + (ranUni - F2)*(X3 - X2)/(F3 - F2);
    }
  else    
    {
      G4double DA = X1*F2+X3*F1+X2*F3-X3*F2-X1*F3-X2*F1;
      G4double DB = X2*F12+X1*F32+X3*F22-X2*F32-X3*F12-X1*F22;
      G4double DC = X3*F2*F12+X2*F1*F32+X1*F3*F22
	           -X1*F2*F32-X2*F3*F12-X3*F1*F22;
      ranQ2 = (DA*ranUni*ranUni + DB*ranUni + DC)/D0;
    }

  return ranQ2;         //  MeV^2
}
//  ==========================================================
G4double G4ElasticHadrNucleusHE::GetLightFq2(G4int Nucleus, G4double Q2)
{

// ----------------  The preparing of probability function  ------------

  G4double prec = Nucleus > 208  ?  1.0e-7 : 1.0e-6;

  G4double    Stot     = HadrTot*MbToGeV2;     //  Gev^-2
  G4double    Bhad     = HadrSlope;         //  GeV^-2
  G4double    Asq      = 1+HadrReIm*HadrReIm;
  G4double    Rho2     = std::sqrt(Asq);
  //    G4cout << "Stot= " << Stot << " Bhad= " << Bhad << " Asq= " << Asq << G4endl;
  G4double    R12      = R1*R1;
  G4double    R22      = R2*R2;
  G4double    R12B     = R12+2*Bhad;
  G4double    R22B     = R22+2*Bhad;

  G4double    Norm     = (R12*R1-Pnucl*R22*R2); //*HP->Aeff;

  G4double    R13      = R12*R1/R12B;
  G4double    R23      = Pnucl*R22*R2/R22B;
  G4double    Unucl    = Stot/twopi/Norm*R13;
  G4double    UnucRho2 = -Unucl*Rho2;

  G4double    FiH      = std::asin(HadrReIm/Rho2);
  G4double    NN2      = R23/R13;

  G4double    dddd;
 
  G4double    Prod0    = 0;
  G4double    N1       = -1.0;
  G4double    Tot0     = 0;
  G4double    exp1;

  G4double    Prod3 ;
  G4double    exp2  ;
  G4double    N4, N5, N2, Prod1, Prod2;

  for(G4int i1 = 1; i1<= Nucleus; i1++) {

    N1 *= UnucRho2*G4double(Nucleus-i1+1)/G4double(i1);
    Prod1 = 0;
    Tot0  = 0;
    N2    = -1;

    for(G4int i2 = 1; i2<=Nucleus; i2++) {

      N2 *= UnucRho2*G4double(Nucleus-i2+1)/G4double(i2);
      Prod2 = 0; //std::exp(-Q2/i2*R12B/4)/i2*R12B;
      N5    = -1/NN2;

      for(G4int m2=0; m2<= i2; m2++) {
	Prod3 = 0;
	exp2  = 1/(m2/R22B+(i2-m2)/R12B);
	N5   *= -NN2;
	N4    = -1/NN2;

	for(G4int m1=0; m1<=i1; m1++) {
	  exp1   = 1/(m1/R22B+(i1-m1)/R12B);
	  dddd   = 0.25*exp1+exp2;
	  N4    *= -NN2;
	  Prod3 += N4*exp1*exp2*SetBinom[i1][m1]*
	    (1-std::exp(-Q2*dddd))/dddd;
	}                                   // m1
	Prod2 += Prod3*N5*SetBinom[i2][m2];
      }                                      // m2
      Prod1 += Prod2*N2*std::cos(FiH*(i1-i2));

      if (std::abs(Prod2*N2/Prod1)<prec) break;
    }                                         // i2
    Prod0  += Prod1*N1;
    if(std::abs(N1*Prod1/Prod0) < prec) break;
  }                                           // i1
  Prod0 *= 0.25*pi/MbToGeV2;  //  This is in mb

  return Prod0;
}
//  ++++++++++++++++++++  The interpolation +++++++++++++++++++++
G4double G4ElasticHadrNucleusHE::InterPol(
                  G4double X1, G4double X2, G4double X3,
                  G4double Y1, G4double Y2, G4double Y3, 
                  G4double X)
{
  G4double ranQ2;
  G4double F12 = X1*X1;
  G4double F22 = X2*X2;
  G4double F32 = X3*X3;
  G4double D0  = F12*X2+X1*F32+X3*F22-F32*X2-F22*X1-F12*X3;

  if(std::fabs(D0) < 1e-8 || D0 == 0) 
    ranQ2 = (Y2+(X-X2)*(Y3-Y2) /(X3-X2));   //   MeV^2

  else    
    {
      G4double DA =  Y1*X2    +Y3*X1    +Y2*X3 -Y3*X2 -Y1*X3 -Y2*X1;
      G4double DB =  Y2*F12   +Y1*F32   +Y3*F22-Y2*F32-Y3*F12-Y1*F22;
      G4double DC =  Y3*X2*F12+Y2*X1*F32+Y1*X3*F22
	-Y1*X2*F32-Y2*X3*F12-Y3*X1*F22;
      ranQ2 = (DA*X*X+DB*X+DC)/D0;           //   MeV^2
    }
  return  ranQ2;
}

//  =====================================================
void  G4ElasticHadrNucleusHE::GetKinematics(const G4ParticleDefinition * aHadron,
					    G4double MomentumH)
{
  GetHadronValues(aHadron, MomentumH);

  SigTot = HadrTot;
  ReOnIm = HadrReIm;

  if(Slope==0.) Slope  = HadrSlope;

  IntConst = (1.-Coeff1)/Slope;

  ProtonM = proton_mass_c2/GeV;
  HadronM = aHadron->GetPDGMass()/GeV;      
  HadronE = std::sqrt(MomentumH*MomentumH + HadronM*HadronM);

  HdrE   = HadronE;

  PM2    = ProtonM*ProtonM;
  HM2    = HadronM*HadronM;
  Sh     = 2.0*ProtonM*HadronE+PM2+HM2;    // GeV
  SqrtS  = std::sqrt(Sh);
  ConstU = 2*PM2+2*HM2-Sh;

  EcmH   = 0.5*(Sh+HM2-PM2)/SqrtS;
  EcmP   = 0.5*(Sh-HM2+PM2)/SqrtS;
     
  Kcm    = std::sqrt(EcmH*EcmH-HM2);
  MaxT   = 4.*Kcm*Kcm;

  BoundaryP[0]=9.0; BoundaryTG[0]=5.0;BoundaryTL[0]=MaxT/2.0;
  BoundaryP[1]=20.0;BoundaryTG[1]=1.5;BoundaryTL[1]=MaxT;
  BoundaryP[2]=5.0; BoundaryTG[2]=1.0;BoundaryTL[2]=1.5;
  BoundaryP[3]=8.0; BoundaryTG[3]=3.0;BoundaryTL[3]=MaxT;
  BoundaryP[4]=7.0; BoundaryTG[4]=3.0;BoundaryTL[4]=MaxT;
  BoundaryP[5]=5.0; BoundaryTG[5]=2.0;BoundaryTL[5]=MaxT;
  BoundaryP[6]=5.0; BoundaryTG[6]=1.5;BoundaryTL[6]=3.0;

  HadrCodes[0] =  2212; HadrCodes[1] =  2112;
  HadrCodes[2] = -2212; HadrCodes[3] =  211;
  HadrCodes[4] = -211;  HadrCodes[5] =  321;
  HadrCodes[6] = -321;

  HadrCode = aHadron->GetPDGEncoding();

  G4int NumberH=0;

  while(HadrCode!=HadrCodes[NumberH]) NumberH++;

  if(MomentumH<BoundaryP[NumberH]) MaxTR = BoundaryTL[NumberH];
  else MaxTR = BoundaryTG[NumberH];

  GetParametersHP(aHadron, MomentumH);
}
//  +++++++++++++++++++++++++++++++++++++++
void  G4ElasticHadrNucleusHE::GetParametersHP(const G4ParticleDefinition * aHadron,
					      G4double HadrP)
  {
    G4int  iStep;

    HadrCode = aHadron->GetPDGEncoding();

    switch(HadrCode)
    {
      case 2212 :
      case 3122 :
      case 3222 :
      case 3112 :
      case 3212 :
      case 3312 :
      case 3322 :
      case 3334 :
        {
          G4double EnP[6]={1.5,3.0,5.0,9.0,14.0,19.0};
          G4double C0P[6]={0.15,0.02,0.06,0.08,0.0003,0.0002};
          G4double C1P[6]={0.05,0.02,0.03,0.025,0.0,0.0};
          G4double B0P[6]={1.5,2.5,3.0,4.5,1.4,1.25};
          G4double B1P[6]={5.0,1.0,3.5,4.0,4.8,4.8};

          if(BoundaryP[0]<HadrP)
          {
            Coeff0=Coeff1=Coeff2=0.0;
            Slope0=Slope1=Slope2=1.0;
          }
          else
          {
            iStep = 0;  while(HadrP>EnP[iStep])  iStep++;

            Coeff0 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  C0P[iStep], C0P[iStep-1],
                                  HadrP);

            Coeff1 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  C1P[iStep], C1P[iStep-1],
                                  HadrP);

            Slope0 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  B0P[iStep], B0P[iStep-1],
                                  HadrP);

            Slope1 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  B1P[iStep], B1P[iStep-1],
                                  HadrP);
          }       //  else
         Coeff2 = 0;
         Slope2 = 5;
        } break;  //  case 2212
      case  2112 :
      case -2112 :
        {
          G4double EnN[5]={1.5,5.0,10.0,14.0,20.0};
          G4double C0N[5]={0.0,0.0,0.02,0.02,0.01};
          G4double C1N[5]={0.06,0.008,0.0015,0.001,0.0003};
          G4double B0N[5]={1.5,2.5,3.8,3.8,3.5};
          G4double B1N[5]={1.5,2.2,3.6,4.5,4.8};

          if(BoundaryP[1]<HadrP)
          {
            Coeff0=Coeff1=0.0;
            Slope0=Slope1=Slope2=1.0;
          }
          else
          {
            iStep = 0;  while(HadrP>EnN[iStep])   iStep++;

            Coeff0 = LineInterpol(EnN[iStep], EnN[iStep-1],
                                  C0N[iStep], C0N[iStep-1],
                                  HadrP);

            Coeff1 = LineInterpol(EnN[iStep], EnN[iStep-1],
                                  C1N[iStep], C1N[iStep-1],
                                  HadrP);

            Slope0 = LineInterpol(EnN[iStep], EnN[iStep-1],
                                  B0N[iStep], B0N[iStep-1],
                                  HadrP);

            Slope1 = LineInterpol(EnN[iStep], EnN[iStep-1],
                                  B1N[iStep], B1N[iStep-1],
                                  HadrP);
            Coeff2 = 0.8/HadrP/HadrP;
            Slope2 = 5;
          }       //  else
        } break;  //  case 2112

      case -2212 :
      case -3122 :
      case -3222 :
      case -3112 :
      case -3212 :
      case -3312 :
      case -3322 :
      case -3334 :
        {
          G4double EnP[2]={1.5,4.0};
          G4double C0P[2]={0.001,0.0005};
          G4double C1P[2]={0.003,0.001};
          G4double B0P[2]={2.5,4.5};
          G4double B1P[2]={1.0,4.0};
          if(BoundaryP[2]<HadrP)
          {
            Coeff0=Coeff1=Coeff2=0.0;
            Slope0=Slope1=Slope2=1.0;
          }

          else
          {
            iStep = 0;  while(HadrP>EnP[iStep])  iStep++;

            Coeff0 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  C0P[iStep], C0P[iStep-1],
                                  HadrP);

            Coeff1 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  C1P[iStep], C1P[iStep-1],
                                  HadrP);

            Slope0 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  B0P[iStep], B0P[iStep-1],
                                  HadrP);

            Slope1 = LineInterpol(EnP[iStep], EnP[iStep-1],
                                  B1P[iStep], B1P[iStep-1],
                                  HadrP);
          }       //  else

            Coeff2=0.0;
            Slope2=5.0;
        } break;  //  case -2212

      case 211 :
        {
          G4double EnPP[4]={1.0,2.0,3.0,4.0};
          G4double C0PP[4]={0.0,0.0,0.0,0.0};
          G4double C1PP[4]={0.15,0.08,0.02,0.01};
          G4double B0PP[4]={1.5,2.8,3.8,3.8};
          G4double B1PP[4]={0.8,1.6,3.6,4.6};

          if(BoundaryP[3]<HadrP)
          {
            Coeff0=Coeff1=Coeff2=0.0;
            Slope0=Slope1=Slope2=1.0;
          }
          else
          {
            iStep = 0;  while(HadrP>EnPP[iStep])   iStep++;

            Coeff0 = LineInterpol(EnPP[iStep], EnPP[iStep-1],
                                  C0PP[iStep], C0PP[iStep-1],
                                  HadrP);

            Coeff1 = LineInterpol(EnPP[iStep], EnPP[iStep-1],
                                  C1PP[iStep], C1PP[iStep-1],
                                  HadrP);

            Slope0 = LineInterpol(EnPP[iStep], EnPP[iStep-1],
                                  B0PP[iStep], B0PP[iStep-1],
                                  HadrP);

            Slope1 = LineInterpol(EnPP[iStep], EnPP[iStep-1],
                                  B1PP[iStep], B1PP[iStep-1],
                                  HadrP);
            Coeff2 = 0.02/HadrP;
            Slope2 = 5;
          }       //  else
        } break;  //  case 211

      case -211 :
        {
          G4double EnPP[4]={1.0,2.0,3.0,4.0};
          G4double C0PP[4]={0.0,0.0,0.0,0.0};
//          G4double C1PP[4]={0.15,0.08,0.02,0.01};
//          G4double B0PP[4]={1.5,2.8,3.8,3.8};
          G4double B1PP[4]={0.8,1.6,3.6,4.6};

          if(BoundaryP[4]<HadrP)
          {
            Coeff0=Coeff1=Coeff2=0.0;
            Slope0=Slope1=Slope2=1.0;
          }
          else
          {
            iStep = 0;  while(HadrP>EnPP[iStep])    iStep++;

            Coeff0 = LineInterpol(EnPP[iStep], EnPP[iStep-1],
                                  C0PP[iStep], C0PP[iStep-1],
                                  HadrP);
            Slope1 = LineInterpol(EnPP[iStep], EnPP[iStep-1],
                                  B1PP[iStep], B1PP[iStep-1],
                                  HadrP);
            Coeff2 = 0.02/HadrP;
            Slope2 = 5;
          }       //  else
        } break;  //  case -211

      case 321 :
        {
          G4double EnK[4]={1.4,2.33,3.0,5.0};
          G4double C0K[4]={0.0,0.0,0.0,0.0};
          G4double C1K[4]={0.01,0.007,0.005,0.003};
          G4double B0K[4]={1.5,2.0,3.8,3.8};
          G4double B1K[4]={1.6,1.6,1.6,1.6};

          if(BoundaryP[5]<HadrP)
          {
            Coeff0=Coeff1=Coeff2=0.0;
            Slope0=Slope1=Slope2=1.0;
          }
          else
          {
            iStep = 0;  while(HadrP>EnK[iStep])   iStep++;

            Coeff0 = LineInterpol(EnK[iStep], EnK[iStep-1],
                                  C0K[iStep], C0K[iStep-1],
                                  HadrP);

            Coeff1 = LineInterpol(EnK[iStep], EnK[iStep-1],
                                  C1K[iStep], C1K[iStep-1],
                                  HadrP);

            Slope0 = LineInterpol(EnK[iStep], EnK[iStep-1],
                                  B0K[iStep], B0K[iStep-1],
                                  HadrP);

            Slope1 = LineInterpol(EnK[iStep], EnK[iStep-1],
                                  B1K[iStep], B1K[iStep-1],
                                  HadrP);
            Coeff2 = 0.34/HadrP/HadrP/HadrP;
            Slope2 = 5;
          }       //  else
        } break;  //  case 321
      case -321 :
        {
          G4double EnKM[2]={1.4,4.0};
          G4double C0KM[2]={0.006,0.002};
          G4double C1KM[2]={0.00,0.00};
          G4double B0KM[2]={2.5,3.5};
          G4double B1KM[2]={1.6,1.6};

          if(BoundaryP[6]<HadrP)
          {
            Coeff0=Coeff1=Coeff2=0.0;
            Slope0=Slope1=Slope2=0.0;
          }
          else
          {
            iStep = 0;  while(HadrP>EnKM[iStep])   iStep++;

            Coeff0 = LineInterpol(EnKM[iStep], EnKM[iStep-1],
                                  C0KM[iStep], C0KM[iStep-1],
                                  HadrP);

            Coeff1 = LineInterpol(EnKM[iStep], EnKM[iStep-1],
                                  C1KM[iStep], C1KM[iStep-1],
                                  HadrP);

            Slope0 = LineInterpol(EnKM[iStep], EnKM[iStep-1],
                                  B0KM[iStep], B0KM[iStep-1],
                                  HadrP);

            Slope1 = LineInterpol(EnKM[iStep], EnKM[iStep-1],
                                  B1KM[iStep], B1KM[iStep-1],
                                  HadrP);
            Coeff2 = 0.01/HadrP/HadrP/HadrP;
            Slope2 = 5;
          }       //  else
        } break;  //  case -321
    }
  }
//  ++++++++++++++++++++++++++++++++++++++++
G4double G4ElasticHadrNucleusHE::LineInterpol(G4double p1, G4double p2,
					      G4double c1, G4double c2,
					      G4double p)
{
  return c1+(p-p1)*(c2-c1)/(p2-p1);
}
//  ============================================================
G4float G4ElasticHadrNucleusHE::GetFt(G4double Q2)
{
  G4float Fdistr, SqrQ2 = std::sqrt(Q2);

  Fdistr = (1.-Coeff1-Coeff0)/Slope*(1.-std::exp(-Slope*Q2))

    + Coeff0*(1.-std::exp(-Slope0*Q2))

    + Coeff2/Slope2*std::exp(Slope2*ConstU)*(std::exp(Slope2*Q2)-1.)

    + 2.*Coeff1/Slope1*(1./Slope1-(1./Slope1+SqrQ2)*std::exp(-Slope1*SqrQ2));

  return Fdistr;
}
//  +++++++++++++++++++++++++++++++++++++++
G4float G4ElasticHadrNucleusHE::GetDistrFun(G4double Q2)
{
  G4float Fq2;

  Fq2   = GetFt(Q2);

  if(verboselevel>-1)
    G4cout<<" GetDistrFun: Q2= " << Q2 
	  << " Fq2=  "<<Fq2<<" FmaxT= " << FmaxT<<G4endl;

  return Fq2/FmaxT;
}
//  +++++++++++++++++++++++++++++++++++++++
G4double G4ElasticHadrNucleusHE::GetQ2(G4double Ran)
{
  G4double DDD0=MaxTR*0.5, DDD1=0.0, DDD2=MaxTR;

  FmaxT = GetFt(MaxTR);
  G4double delta = GetDistrFun(DDD0)-Ran;

  while(std::fabs(delta) > 0.0001)
    {
      if(delta>0) 
      {
        DDD2 = DDD0;
        DDD0 = (DDD0+DDD1)*0.5;
      }
      else if(delta<0)
      {
        DDD1 = DDD0; 
        DDD0 = (DDD0+DDD2)*0.5;
      }
      delta = GetDistrFun(DDD0)-Ran;
    }

  return DDD0;
}
//  ++++++++++++++++++++++++++++++++++++++++++
G4double G4ElasticHadrNucleusHE::HadronProtonQ2(const G4ParticleDefinition * aHadron,
						G4double inLabMom)
{
  G4double Ran = G4UniformRand();
  GetKinematics(aHadron, inLabMom);
  G4double Q2 = GetQ2(Ran);

  return Q2;
}
//  ++++++++++++++++++++++++++++++++++++++++++
void  G4ElasticHadrNucleusHE::GetHadronValues(const G4ParticleDefinition * aHadron,
					      G4double HadrMoment)
{
  G4double protM  = proton_mass_c2/GeV;
  G4double protM2 = protM*protM;

  G4int iHadron   = -1;
  G4int iHadrCode = aHadron->GetPDGEncoding();

   switch (iHadrCode)
     {
     case 2212:
     case 2112:
     case 3122:
     case 3222:
     case 3112:
     case 3212:
     case 3312:
     case 3322:
     case 3334:   
       iHadron = 0;
       break;

     case -2212:
     case -2112:
     case -3122:
     case -3222:
     case -3112:
     case -3212:
     case -3312:
     case -3322:
     case -3334:
       iHadron = 1;
       break;

     case  211:
       iHadron = 2;
       break;

     case -211:
       iHadron = 3;
       break;

     case  321:
     case  130:
     case  310:
     case  311:     
       iHadron = 4;
       break;
                 
     case -321:
     case -130:
     case -310:
     case -311:
       iHadron = 5;
       break;

     default:
       { 
	 G4cout<<" ElasticHE: For the hadron "
	       << aHadron->GetParticleName() 
	       <<" other method must be used."<<G4endl;
	 HadrTot   = 20;
	 HadrSlope = 7;
	 HadrReIm  = 0.2;
	 return;
       }
     }
   G4double mHadr      = aHadron->GetPDGMass()/GeV;  // In GeV
   G4double mHadr2     = mHadr*mHadr;

   G4double HadrEnergy = sqrt(mHadr2+HadrMoment*HadrMoment);

   G4double sHadr      = 2.*HadrEnergy*protM+protM2+mHadr2;
   G4double sqrS       = std::sqrt(sHadr);
   G4double Ecm        = 0.5*(sHadr-mHadr2+protM2)/sqrS;
   MomentumCM          = std::sqrt(Ecm*Ecm-protM2);

   if(HadrEnergy-mHadr<0.39)
   {
     G4cout<<"ElasticHE(GetHadronValues): The energy T = "
	   <<(HadrEnergy-mHadr)
	   <<" GeV is very low for this method! T = 0.400 GeV is set."
	   <<G4endl;
   }

   G4double TotP=0.0, TotN=0.0;
   G4double logE = std::log(HadrEnergy);

   switch (iHadron)
    {
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
     case 0:                  //  proton, neutron

       G4double A0, B0;
       if(HadrMoment>10.)
	 {
	   B0 = 7.5;
	   A0 = 100. - B0*17.2167;   // log(3.0e7)

	   TotP = TotN = A0 + B0*logE - 11. + 103*std::pow(sHadr,-0.165); //  mb
	 }
// ==================  neutron  ================
       else if(HadrMoment <= 10 && HadrMoment >1.4 /*iHadrCode == 2112*/)
	 {
              TotN = 33.3+
		15.2*(HadrMoment*HadrMoment-1.35)/
		(std::pow(HadrMoment,2.37)+0.95);
	 }

       else if(HadrMoment <= 1.4  && HadrMoment > 0.8 /*iHadrCode == 2112*/)
	 {

            TotN = 33+25.5*(logE+0.0513)*(logE+0.0513);  //  -log(0.95)
	 }

       else if(HadrMoment <= 0.8  /*&& iHadrCode == 2112*/)
	 {
            A0   = (logE-0.2634)*(logE-0.2634);  // log(1.3)
            TotN = 33+30*A0*A0;
	 }
//  =================  proton  ===============
       if(HadrMoment <= 10  && HadrMoment>1.05 /*iHadrCode == 2212*/)
	 {
	   TotP = 39.0+
	     75.*(HadrMoment-1.2)/(HadrMoment*HadrMoment*HadrMoment+0.15);
	 }

       else if(HadrMoment <= 1.05  && HadrMoment > 0.7 /*iHadrCode == 2212*/)
	 {
	   A0 = logE+0.3147;
	   TotP = 23+40*A0*A0;
	 }

       else if(HadrMoment <= 0.7  /*&& iHadrCode == 2212*/)
	 {
            TotP = 23+50*std::pow(std::log(0.73/HadrMoment),3.5);
	 }

       HadrTot = (TotP+TotN)/2.;

//  ============================================
       HadrSlope = 6.44+0.88*std::log(sHadr)-1;     //  GeV-2
       if(HadrMoment<2.) HadrSlope = 1.5;

       if(HadrMoment>1.2)
	 HadrReIm  = 0.13*std::log(sHadr/350)*std::pow(sHadr,-0.18);

       else if(HadrMoment <= 1.2 && HadrMoment > 0.6)
	 {
	   HadrReIm = -0.0-
             75.5*(std::pow(HadrMoment,0.25)-0.95)/
	     (std::pow(3*HadrMoment,2.2)+1);     
	 }
                                                                               
       else if(HadrMoment <= 0.6)
	 {
	   A0 = 9*HadrMoment*HadrMoment;
	   HadrReIm = -0.0 + 15.5*HadrMoment/(A0*3*HadrMoment+2);
	 }

       DDSect2   = 11;                              //mb*GeV-2
       DDSect3   = 3;                               //mb*GeV-2
//  ================== lambda  ==================
       if( iHadrCode == 3122)
	 {
	   HadrTot *= 0.88;
	   HadrSlope *=0.85;
	 }
//  ================== sigma +  ==================
       if( iHadrCode == 3222)
	 {
	   HadrTot   *=0.81;
	   HadrSlope *=0.85;
	 }
//  ================== sigma 0,-  ==================
       if(iHadrCode == 3112 || iHadrCode == 3212 )
	 {
	   HadrTot   *=0.88;
	   HadrSlope *=0.85;
	 }
//  ===================  xi  =================
       if( iHadrCode == 3312 || iHadrCode == 3322 )
	 {
	   HadrTot   *=0.77;
	   HadrSlope *=0.75;
	 }
//  =================  omega  =================
       if( iHadrCode == 3334)
	 {
	   HadrTot   *=0.78;
	   HadrSlope *=0.7;
	 }

       break;
//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    case 1:              //   antiproton

      sqrS      = std::sqrt(sHadr);
      HadrTot   = 5.2+5.2*std::log(HadrEnergy) + 123.2/sqrS;     //  mb
      HadrSlope = 8.32+0.57*std::log(sHadr); //GeV-2

      if(HadrEnergy<1000)
	HadrReIm  = 0.06*(sqrS-2.236)*(sqrS-14.14)*std::pow(sHadr,-0.8);
      else
	HadrReIm  = 0.6*std::log(sHadr/350)*std::pow(sHadr,-0.25);

      DDSect2   = 11;                                //mb*GeV-2
      DDSect3   = 3;                                 //mb*GeV-2
//  ================== lambda  ==================
      if( iHadrCode == -3122)
	{
	  HadrTot *= 0.88;
	  HadrSlope *=0.85;
	}
//  ================== sigma +  ==================
      if( iHadrCode == -3222)
	{
	  HadrTot   *=0.81;
	  HadrSlope *=0.85;
	}
//  ================== sigma 0,-  ==================
      if(iHadrCode == -3112 || iHadrCode == -3212 )
	{
	  HadrTot   *=0.88;
	  HadrSlope *=0.85;
	}
//  ===================  xi  =================
      if( iHadrCode == -3312 || iHadrCode == -3322 )
	{
	  HadrTot   *=0.77;
	  HadrSlope *=0.75;
	}
//  =================  omega  =================
      if( iHadrCode == -3334)
	{
	  HadrTot   *=0.78;
          HadrSlope *=0.7;
	}

      break;
//  -------------------------------------------
    case 2:             //   pi plus, pi minus
    case 3:

      if(HadrMoment>3.5)
	TotP = 10.6+2.*logE + 25.*std::pow(HadrEnergy,-0.43); // mb
      //  =========================================
      else if(HadrMoment <= 3.5  &&  HadrMoment >1.15)
	{
	  G4double Ex1 = 3.2*
	    std::exp(-(HadrMoment-2.55)*(HadrMoment-2.55)/0.55/0.55);

	  G4double Ex2 = 12.*
	    std::exp(-(HadrMoment-1.47)*(HadrMoment-1.47)/0.225/0.225);

	  TotP = Ex1+Ex2+27.5;
	}
//  =========================================
      else if(HadrMoment <= 1.15 &&  HadrMoment >0.4)
	TotP  = 88*(logE+0.2877)*(logE+0.2877)+14.0;
	
//  =========================================
      else if(HadrMoment <= 0.4)
	{
	  G4double x = (HadrMoment-0.29)/0.085;
	  TotP = 20. + 180.*std::exp(-x*x);
	}
//  =========================================
      HadrSlope = 7.28+0.245*std::log(sHadr);        //GeV-2
      HadrReIm  = 0.2*std::log(sHadr/100)*std::pow(sHadr,-0.15);
      DDSect2   = 4.6;                               //mb*GeV-2
      DDSect3   = 1.33;                              //mb*GeV-2
//  -------------------------------------------

      if(HadrMoment > 3.0 )
	TotN = 10.6 + 2.*std::log(HadrEnergy) + 30.*std::pow(HadrEnergy,-0.43); // mb

      else if(HadrMoment <= 3.0 && HadrMoment > 1.3)
	TotN = 36.1+0.079-4.313*logE+
	  3*std::exp(-(HadrMoment-2.1)*(HadrMoment-2.1)/0.4/0.4)+
	  1.5*std::exp(-(HadrMoment-1.4)*(HadrMoment-1.4)/0.12/0.12);

      else if(HadrMoment <= 1.3 && HadrMoment > 0.65)
	TotN = 36.1+
	  10*std::exp(-(HadrMoment-0.72)*(HadrMoment-0.72)/0.06/0.06)+
	  24*std::exp(-(HadrMoment-1.015)*(HadrMoment-1.015)/0.075/0.075);

      else if(HadrMoment <= 0.65 && HadrMoment > 0.37)
	{
	  G4double x = std::log(HadrMoment/0.48);
	  TotN = 26. + 110.*x*x;
	}

      else ///(HadrMoment<0.37)
	TotN = 28.0+
	  40.*std::exp(-(HadrMoment-0.29)*(HadrMoment-0.29)/0.07/0.07);

      HadrTot = (TotP+TotN)/2;

      HadrSlope = 7.28+0.245*std::log(sHadr);        // GeV-2
      HadrReIm  = 0.2*std::log(sHadr/100)*std::pow(sHadr,-0.15);

      DDSect2   = 4.6;                               //mb*GeV-2
      DDSect3   = 1.33;                              //mb*GeV-2

      break;
      // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    case 4:            //  K plus

      HadrTot   = 10.6+1.8*std::log(HadrEnergy)+
                          9.0*std::pow(HadrEnergy,-0.55);  // mb
      if(HadrEnergy>100) HadrSlope = 15.0;
      else HadrSlope = 1.0+1.76*std::log(sHadr) - 2.84/std::sqrt(sHadr);   // GeV-2

      HadrReIm  = 0.4*(sHadr-20)*(sHadr-150)*std::pow(sHadr+50,-2.1);
      DDSect2   = 3.5;                             //mb*GeV-2
      DDSect3   = 1.03;                            //mb*GeV-2
      break;
      //  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    case 5:              //   K minus

      HadrTot   = 10+1.8*std::log(HadrEnergy) + 25./std::sqrt(HadrEnergy); // mb
      HadrSlope = 6.98+0.127*std::log(sHadr);         // GeV-2
      HadrReIm  = 0.4*(sHadr-20)*(sHadr-20)*std::pow(sHadr+50,-2.1);
      DDSect2   = 3.5;                             //mb*GeV-2
      DDSect3   = 1.03;                            //mb*GeV-2
      break;
    }   //  switch
}
//  =========================================================

/*  End of file  */


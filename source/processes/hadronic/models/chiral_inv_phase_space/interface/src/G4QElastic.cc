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
// $Id: G4QElastic.cc,v 1.4 2006-04-27 16:39:58 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QElastic class -----------------
//                 by Mikhail Kossov, December 2003.
// G4QElastic class of the CHIPS Simulation Branch in GEANT4
// ---------------------------------------------------------------
// ****************************************************************************************
// ********** This CLASS is temporary moved from the photolepton_hadron directory *********
// ****************************************************************************************

//#define debug
//#define pdebug
//#define tdebug

#include "G4QElastic.hh"

// Initialization of static vectors
G4int    G4QElastic::nPartCWorld=152;        // The#of particles initialized in CHIPS World
std::vector<G4int> G4QElastic::ElementZ;              // Z of the element(i) in theLastCalc
std::vector<G4double> G4QElastic::ElProbInMat;        // SumProbabilityElements in Material
std::vector<std::vector<G4int>*> G4QElastic::ElIsoN;       // N of isotope(j) of Element(i)
std::vector<std::vector<G4double>*>G4QElastic::IsoProbInEl;//SumProbabIsotopes inElementI

// Constructor
G4QElastic::G4QElastic(const G4String& processName) : G4VDiscreteProcess(processName)
{
#ifdef debug
  G4cout<<"G4QElastic::Constructor is called processName="<<processName<<G4endl;
#endif
  if (verboseLevel>0) G4cout << GetProcessName() << " process is created "<< G4endl;

  G4QCHIPSWorld::Get()->GetParticles(nPartCWorld); // Create CHIPS World with 234 particles
}

// Destructor
G4QElastic::~G4QElastic() {}


G4LorentzVector G4QElastic::GetEnegryMomentumConservation() {return EnMomConservation;}

G4int G4QElastic::GetNumberOfNeutronsInTarget() {return nOfNeutrons;}

// output of the function must be in units of length! L=1/sig_V,sig_V=SUM(n(j,i)*sig(j,i)),
// where n(i,j) is a number of nuclei of the isotop j of the element i in V=1(lengtUnit^3)
// ********** All CHIPS cross sections are calculated in the surface units ************
G4double G4QElastic::GetMeanFreePath(const G4Track& aTrack,G4double Q,G4ForceCondition* Fc)
{
  *Fc = NotForced;
  const G4DynamicParticle* incidentParticle = aTrack.GetDynamicParticle();
  G4ParticleDefinition* incidentParticleDefinition=incidentParticle->GetDefinition();
  if( !IsApplicable(*incidentParticleDefinition))
    G4cout<<"*W*G4QElastic::GetMeanFreePath: is called for notImplementedParticle"<<G4endl;
  // Calculate the mean Cross Section for the set of Elements(*Isotopes) in the Material
  G4double Momentum = incidentParticle->GetTotalMomentum(); // 3-momentum of the Particle
  const G4Material* material = aTrack.GetMaterial();        // Get the current material
  const G4double* NOfNucPerVolume = material->GetVecNbOfAtomsPerVolume();
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int nE=material->GetNumberOfElements();
#ifdef debug
  G4cout<<"G4QElastic::GetMeanFreePath:"<<nE<<" Elem's in theMaterial"<<G4endl;
#endif
  G4VQCrossSection* CSmanager=0;
  G4int pPDG=0;
  if(incidentParticleDefinition == G4Proton::Proton())
  {
    CSmanager=G4QElasticCrossSection::GetPointer();
    pPDG=2212;
  }
  else if(incidentParticleDefinition == G4Neutron::Neutron())
  {
    CSmanager=G4QElasticCrossSection::GetPointer();
    pPDG=2112;
  }
  else G4cout<<"G4QElastic::GetMeanFreePath: np,pp,pd,pHe implemented in CHIPS"<<G4endl;
  
  G4QIsotope* Isotopes = G4QIsotope::Get(); // Pointer to the G4QIsotopes singleton
  G4double sigma=0.;                        // Sums over elements for the material
  G4int IPIE=IsoProbInEl.size();            // How many old elements?
  if(IPIE) for(G4int ip=0; ip<IPIE; ++ip)   // Clean up the SumProb's of Isotopes (SPI)
  {
    std::vector<G4double>* SPI=IsoProbInEl[ip]; // Pointer to the SPI vector
    SPI->clear();
    delete SPI;
    std::vector<G4int>* IsN=ElIsoN[ip];     // Pointer to the N vector
    IsN->clear();
    delete IsN;
  }
  ElProbInMat.clear();                      // Clean up the SumProb's of Elements (SPE)
  ElementZ.clear();                         // Clear the body vector for Z of Elements
  IsoProbInEl.clear();                      // Clear the body vector for SPI
  ElIsoN.clear();                           // Clear the body vector for N of Isotopes
  for(G4int i=0; i<nE; ++i)
  {
    G4Element* pElement=(*theElementVector)[i]; // Pointer to the current element
    G4int Z = static_cast<G4int>(pElement->GetZ()); // Z of the Element
    ElementZ.push_back(Z);                  // Remember Z of the Element
    G4int isoSize=0;                        // The default for the isoVectorLength is 0
    G4int indEl=0;                          // Index of non-trivial element or 0(default)
    G4IsotopeVector* isoVector=pElement->GetIsotopeVector(); // Get the predefined IsoVect
    if(isoVector) isoSize=isoVector->size();// Get size of the existing isotopeVector
#ifdef debug
    G4cout<<"G4QElastic::GetMeanFreePath: isovectorLength="<<isoSize<<G4endl; // Result
#endif
    if(isoSize)                             // The Element has non-trivial abumdance set
    {
      indEl=pElement->GetIndex();           // Index of the non-trivial element
      if(!Isotopes->IsDefined(Z,indEl))     // This index is not defined for this Z: define
      {
        std::vector<std::pair<G4int,G4double>*>* newAbund =
                                               new std::vector<std::pair<G4int,G4double>*>;
        G4double* abuVector=pElement->GetRelativeAbundanceVector();
        for(G4int j=0; j<isoSize; j++)      // Calculation of abundance vector for isotopes
        {
          G4int N=pElement->GetIsotope(j)->GetN()-Z; // N means A=N+Z !
          if(pElement->GetIsotope(j)->GetZ()!=Z)G4cerr<<"G4QCaptureAtRest::GetMeanFreePath"
																																	<<": Z="<<pElement->GetIsotope(j)->GetZ()<<"#"<<Z<<G4endl;
          G4double abund=abuVector[j];
								  std::pair<G4int,G4double>* pr= new std::pair<G4int,G4double>(N,abund);
#ifdef debug
          G4cout<<"G4QElastic::GetMeanFreePath:pair#="<<j<<", N="<<N<<",ab="<<abund<<G4endl;
#endif
          newAbund->push_back(pr);
						  }
#ifdef debug
        G4cout<<"G4QElastic::GetMeanFreePath: pairVectorLength="<<newAbund->size()<<G4endl;
#endif
        indEl=G4QIsotope::Get()->InitElement(Z,indEl,newAbund); // definition of the newInd
        for(G4int k=0; k<isoSize; k++) delete (*newAbund)[k];   // Cleaning temporary
        delete newAbund; // Was "new" in the beginning of the name space
      }
    }
    std::vector<std::pair<G4int,G4double>*>* cs= Isotopes->GetCSVector(Z,indEl);//CSPointer
    std::vector<G4double>* SPI = new std::vector<G4double>; // Pointer to the SPI vector
    IsoProbInEl.push_back(SPI);
    std::vector<G4int>* IsN = new std::vector<G4int>; // Pointer to the N vector
    ElIsoN.push_back(IsN);
    G4int nIs=cs->size();                   // A#Of Isotopes in the Element
    G4double susi=0.;                       // sum of CS over isotopes
    if(nIs) for(G4int j=0; j<nIs; j++)      // Calculate CS for eachIsotope of El
    {
      std::pair<G4int,G4double>* curIs=(*cs)[j]; // A pointer, which is used twice
      G4int N=curIs->first;                 // #of Neuterons in the isotope j of El i
      IsN->push_back(N);                    // Remember Min N for the Element
#ifdef debug
  G4cout<<"G4QElast::GMFP:*true*, P="<<Momentum<<",Z="<<Z<<",N="<<N<<",PDG="<<pPDG<<G4endl;
#endif
		    G4bool ccsf=true;
      if(Q==-27.) ccsf=false;
      G4double CSI=CSmanager->GetCrossSection(ccsf,Momentum,Z,N,pPDG);//CS(j,i) for isotope
      curIs->second = CSI;
      susi+=CSI;                            // Make a sum per isotopes
      SPI->push_back(susi);                 // Remember summed cross-section
    } // End of temporary initialization of the cross sections in the G4QIsotope singeltone
    sigma+=Isotopes->GetMeanCrossSection(Z,indEl)*NOfNucPerVolume[i];//SUM(MeanCS*NOfNperV)
    ElProbInMat.push_back(sigma);
  } // End of LOOP over Elements

  // Check that cross section is not zero and return the mean free path
  if(sigma > 0.) return 1./sigma;                 // Mean path [distance] 
  return DBL_MAX;
}


G4bool G4QElastic::IsApplicable(const G4ParticleDefinition& particle) 
{
  if      (particle == *(        G4Proton::Proton()        )) return true;
  else if (particle == *(       G4Neutron::Neutron()       )) return true;
  //else if (particle == *(     G4MuonMinus::MuonMinus()     )) return true; 
  //else if (particle == *(       G4TauPlus::TauPlus()       )) return true;
  //else if (particle == *(      G4TauMinus::TauMinus()      )) return true;
  //else if (particle == *(      G4Electron::Electron()      )) return true;
  //else if (particle == *(      G4Positron::Positron()      )) return true;
  //else if (particle == *(         G4Gamma::Gamma()         )) return true;
  //else if (particle == *(      G4MuonPlus::MuonPlus()      )) return true;
  //else if (particle == *(G4AntiNeutrinoMu::AntiNeutrinoMu())) return true;
  //else if (particle == *(   G4NeutrinoMu::NeutrinoMu()   )) return true;
  //else if (particle == *(     G4PionMinus::PionMinus()     )) return true;
  //else if (particle == *(      G4PionPlus::PionPlus()      )) return true;
  //else if (particle == *(      G4KaonPlus::KaonPlus()      )) return true;
  //else if (particle == *(     G4KaonMinus::KaonMinus()     )) return true;
  //else if (particle == *(  G4KaonZeroLong::KaonZeroLong()  )) return true;
  //else if (particle == *( G4KaonZeroShort::KaonZeroShort() )) return true;
  //else if (particle == *(        G4Lambda::Lambda()        )) return true;
  //else if (particle == *(     G4SigmaPlus::SigmaPlus()     )) return true;
  //else if (particle == *(    G4SigmaMinus::SigmaMinus()    )) return true;
  //else if (particle == *(     G4SigmaZero::SigmaZero()     )) return true;
  //else if (particle == *(       G4XiMinus::XiMinus()       )) return true;
  //else if (particle == *(        G4XiZero::XiZero()        )) return true;
  //else if (particle == *(    G4OmegaMinus::OmegaMinus()    )) return true;
  //else if (particle == *(   G4AntiNeutron::AntiNeutron()   )) return true;
  //else if (particle == *(    G4AntiProton::AntiProton()    )) return true;
#ifdef debug
  G4cout<<"***>>G4QElastic::IsApplicable: PDG="<<particle.GetPDGEncoding()<<G4endl;
#endif
  return false;
}

G4VParticleChange* G4QElastic::PostStepDoIt(const G4Track& track, const G4Step& step)
{
  //static const G4double mProt=G4Proton::Proton()->GetPDGMass()*GeV; // proton mass (in GeV)
  //static const G4double mProt= G4QPDGCode(2212).GetMass()*.001;   // CHIPS in GeV
  //static const G4double mP2=mProt*mProt;                            // squared proton mass
  //
  const G4DynamicParticle* projHadron = track.GetDynamicParticle();
  const G4ParticleDefinition* particle=projHadron->GetDefinition();
#ifdef debug
  G4cout<<"G4QElastic::PostStepDoIt: Before the GetMeanFreePath is called"<<G4endl;
#endif
  G4ForceCondition cond=NotForced;
  GetMeanFreePath(track, -27., &cond);                  // @@ ?? jus to update parameters?
#ifdef debug
  G4cout<<"G4QElastic::PostStepDoIt: After the GetMeanFreePath is called"<<G4endl;
#endif
  G4LorentzVector proj4M=(projHadron->Get4Momentum())*MeV; // Convert to MeV!
  G4LorentzVector scat4M=proj4M;                      // @@ Must be filled (?)
  G4double momentum = projHadron->GetTotalMomentum()*MeV; // 3-momentum of the Proj in MeV
  G4double Momentum = proj4M.rho();                   // Just for the test purposes
  if(std::fabs(Momentum-momentum)>.000001)
           G4cerr<<"*War*G4QElastic::PostStepDoIt:P(IU)="<<Momentum<<"="<<momentum<<G4endl;
  G4double pM2=proj4M.m2();        // in MeV^2
  G4double pM=std::sqrt(pM2);      // in MeV
#ifdef debug
  G4cout<<"G4QElastic::PostStepDoIt: pP(IU)="<<Momentum<<"="<<momentum<<",pM="<<pM<<G4endl;
#endif
  if (!IsApplicable(*particle))  // Check applicability
  {
    G4cerr<<"G4QElastic::PostStepDoIt:Only pp elastic is implemented."<<G4endl;
    return 0;
  }
  const G4Material* material = track.GetMaterial();      // Get the current material
  G4int Z=0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int nE=material->GetNumberOfElements();
#ifdef debug
  G4cout<<"G4QElastic::PostStepDoIt: "<<nE<<" elements in the material."<<G4endl;
#endif
  G4int projPDG=0;                           // PDG Code prototype for the captured hadron
  // Not all these particles are implemented yet (see Is Applicable)
  if      (particle ==          G4Proton::Proton()         ) projPDG= 2212;
  else if (particle ==         G4Neutron::Neutron()        ) projPDG= 2112;
  //else if (particle ==       G4PionMinus::PionMinus()      ) projPDG= -211;
  //else if (particle ==        G4PionPlus::PionPlus()       ) projPDG=  211;
  //else if (particle ==        G4KaonPlus::KaonPlus()       ) projPDG= 2112;
  //else if (particle ==       G4KaonMinus::KaonMinus()      ) projPDG= -321;
  //else if (particle ==    G4KaonZeroLong::KaonZeroLong()   ) projPDG=  130;
  //else if (particle ==   G4KaonZeroShort::KaonZeroShort()  ) projPDG=  310;
  //else if (particle ==        G4MuonPlus::MuonPlus()       ) projPDG=  -13;
  //else if (particle ==       G4MuonMinus::MuonMinus()      ) projPDG=   13;
  //else if (particle ==      G4NeutrinoMu::NeutrinoMu()     ) projPDG=   14;
  //else if (particle ==  G4AntiNeutrinoMu::AntiNeutrinoMu() ) projPDG=  -14;
  //else if (particle ==        G4Electron::Electron()       ) projPDG=   11;
  //else if (particle ==        G4Positron::Positron()       ) projPDG=  -11;
  //else if (particle ==       G4NeutrinoE::NeutrinoE()      ) projPDG=   12;
  //else if (particle ==   G4AntiNeutrinoE::AntiNeutrinoE()  ) projPDG=  -12;
  //else if (particle ==           G4Gamma::Gamma()          ) projPDG=   22;
  //else if (particle ==         G4TauPlus::TauPlus()        ) projPDG=  -15;
  //else if (particle ==        G4TauMinus::TauMinus()       ) projPDG=   15;
  //else if (particle ==     G4NeutrinoTau::NeutrinoTau()    ) projPDG=   16;
  //else if (particle == G4AntiNeutrinoTau::AntiNeutrinoTau()) projPDG=  -16;
  //else if (particle ==          G4Lambda::Lambda()         ) projPDG= 3122;
  //else if (particle ==       G4SigmaPlus::SigmaPlus()      ) projPDG= 3222;
  //else if (particle ==      G4SigmaMinus::SigmaMinus()     ) projPDG= 3112;
  //else if (particle ==       G4SigmaZero::SigmaZero()      ) projPDG= 3212;
  //else if (particle ==         G4XiMinus::XiMinus()        ) projPDG= 3312;
  //else if (particle ==          G4XiZero::XiZero()         ) projPDG= 3322;
  //else if (particle ==      G4OmegaMinus::OmegaMinus()     ) projPDG= 3334;
  //else if (particle ==     G4AntiNeutron::AntiNeutron()    ) projPDG=-2112;
  //else if (particle ==      G4AntiProton::AntiProton()     ) projPDG=-2212;
#ifdef debug
  G4int prPDG=particle->GetPDGEncoding();
		G4cout<<"G4QElastic::PostStepDoIt: projPDG="<<projPDG<<", stPDG="<<prPDG<<G4endl;
#endif
  if(!projPDG)
  {
    G4cerr<<"*Warning*G4QElastic::PostStepDoIt: Undefined interacting hadron"<<G4endl;
    return 0;
  }
  G4int EPIM=ElProbInMat.size();
#ifdef debug
		G4cout<<"G4QElastic::PostStDoIt:m="<<EPIM<<",n="<<nE<<",T="<<ElProbInMat[EPIM-1]<<G4endl;
#endif
  G4int i=0;
  if(EPIM>1)
  {
    G4double rnd = ElProbInMat[EPIM-1]*G4UniformRand();
    for(i=0; i<nE; ++i)
		  {
#ifdef debug
				  G4cout<<"G4QElastic::PostStepDoIt:EPM["<<i<<"]="<<ElProbInMat[i]<<",r="<<rnd<<G4endl;
#endif
      if (rnd<ElProbInMat[i]) break;
    }
    if(i>=nE) i=nE-1;                        // Top limit for the Element
  }
  G4Element* pElement=(*theElementVector)[i];
  Z=static_cast<G4int>(pElement->GetZ());
#ifdef debug
				G4cout<<"G4QElastic::PostStepDoIt: i="<<i<<", Z(element)="<<Z<<G4endl;
#endif
  if(Z<=0)
  {
    G4cerr<<"---Warning---G4QElastic::PostStepDoIt: Element with Z="<<Z<<G4endl;
    if(Z<0) return 0;
  }
  std::vector<G4double>* SPI = IsoProbInEl[i];// Vector of summedProbabilities for isotopes
  std::vector<G4int>* IsN = ElIsoN[i];     // Vector of "#of neutrons" in the isotope El[i]
  G4int nofIsot=SPI->size();               // #of isotopes in the element i
#ifdef debug
		G4cout<<"G4QElastic::PosStDoIt:n="<<nofIsot<<",T="<<(*SPI)[nofIsot-1]<<G4endl;
#endif
  G4int j=0;
  if(nofIsot>1)
  {
    G4double rndI=(*SPI)[nofIsot-1]*G4UniformRand(); // Randomize the isotop of the Element
    for(j=0; j<nofIsot; ++j)
    {
#ifdef debug
				  G4cout<<"G4QElastic::PostStepDoIt: SP["<<j<<"]="<<(*SPI)[j]<<", r="<<rndI<<G4endl;
#endif
      if(rndI < (*SPI)[j]) break;
    }
    if(j>=nofIsot) j=nofIsot-1;            // Top limit for the isotope
  }
  G4int N =(*IsN)[j]; ;                    // Randomized number of neutrons
#ifdef debug
		G4cout<<"G4QElastic::PostStepDoIt: j="<<i<<", N(isotope)="<<N<<", MeV="<<MeV<<G4endl;
#endif
  if(N<0)
  {
    G4cerr<<"*Warning*G4QElastic::PostStepDoIt: Isotope with Z="<<Z<<", 0>N="<<N<<G4endl;
    return 0;
  }
  nOfNeutrons=N;                           // Remember it for the energy-momentum check
#ifdef debug
  G4cout<<"G4QElastic::PostStepDoIt: N="<<N<<" for element with Z="<<Z<<G4endl;
#endif
  if(N<0)
  {
    G4cerr<<"*Warning*G4QElastic::PostStepDoIt:Element with N="<<N<< G4endl;
    return 0;
  }
  aParticleChange.Initialize(track);
  G4double localtime = track.GetGlobalTime();
  G4ThreeVector position = track.GetPosition();
  //
  G4int targPDG=90000000+Z*1000+N;         // CHIPS PDG Code of the target nucleus
  G4QPDGCode targQPDG(targPDG);
  G4double tM=targQPDG.GetMass();          // CHIPS target mass in MeV
  G4double kinEnergy= projHadron->GetKineticEnergy()*MeV; // Kin energy in MeV (Is *MeV n?)
  G4ParticleMomentum dir = projHadron->GetMomentumDirection();// It is a unit three-vector
  G4LorentzVector tot4M=proj4M+G4LorentzVector(0.,0.,0.,tM); // Total 4-mom of the reaction
#ifdef debug
  G4cout<<"G4QElastic::PostStepDoIt: tM="<<tM<<", p4M="<<proj4M<<", t4M="<<tot4M<<G4endl;
#endif
  EnMomConservation=tot4M;                 // Total 4-mom of reaction for E/M conservation
  G4VQCrossSection* CSmanager=G4QElasticCrossSection::GetPointer();
  // @@ Probably this is not necessary any more
#ifdef debug
  G4cout<<"G4QElas::PSDI:false,P="<<Momentum<<",Z="<<Z<<",N="<<N<<",PDG="<<projPDG<<G4endl;
#endif
  G4double xSec=CSmanager->GetCrossSection(false, Momentum, Z, N, projPDG);// Rec.CrossSect
#ifdef debug
  G4cout<<"G4QElast::PSDI:pPDG="<<projPDG<<",P="<<Momentum<<",CS="<<xSec/millibarn<<G4endl;
#endif
  // @@ check a possibility to separate p, n, or alpha (!)
  if(xSec <= 0.) // The cross-section iz 0 -> Do Nothing
  {
    G4cerr<<"*Warning*G4QElastic::PSDoIt: Zero cross-section"<<G4endl;
    //Do Nothing Action insead of the reaction
    aParticleChange.ProposeEnergy(kinEnergy);
    aParticleChange.ProposeLocalEnergyDeposit(0.);
    aParticleChange.ProposeMomentumDirection(dir) ;
    return G4VDiscreteProcess::PostStepDoIt(track,step);
  }
  G4double mint=CSmanager->GetExchangeT(Z,N,projPDG); // -t in MeV^2
  // @@ only for pp: M_1=M_2=M_p, (1-cost)=(-t)/T/M
  // G4double cost=1.-mint/kinEnergy/tM;      // cos(theta) in CMS
  // In general
  G4double tM2=tM*tM;
  G4double pEn=pM+kinEnergy;                  // tot projectile Energy in MeV
  G4double sM=(tM+tM)*pEn+tM2+pM2;            // Mondelstam s
  G4double twop2cm=(tM2+tM2)*(pEn*pEn-pM2)/sM;// Doubled squared momentum in CM system
  G4double cost=1.-mint/twop2cm;              // cos(theta) in CMS
  // 
#ifdef tdebug
  G4cout<<"G4QElastic::PoStDoI:t="<<mint<<",T="<<kinEnergy<<",M="<<tM<<",c="<<cost<<G4endl;
#endif
  if(cost>1. || cost<-1.)
  {
    if(cost>1.000001 || cost<-1.000001) G4cout<<"*Warning*G4QElastic::PostStepDoIt: cos="
        <<cost<<", t="<<mint<<",T="<<kinEnergy<<", M="<<tM<<",tm="<<2*kinEnergy*tM<<G4endl;
    if     (cost>1.)  cost=1.;
    else if(cost<-1.) cost=-1.;
  }
  G4LorentzVector reco4M=G4LorentzVector(0.,0.,0.,tM);      // 4mom of the recoil target
  G4LorentzVector dir4M=tot4M-G4LorentzVector(0.,0.,0.,(tot4M.e()-tM-pM)*.01);
  if(!G4QHadron(tot4M).RelDecayIn2(scat4M, reco4M, dir4M, cost, cost))
  {
    G4cerr<<"G4QElastic::PSD:t4M="<<tot4M<<",pM="<<pM<<",tM="<<tM<<",cost="<<cost<<G4endl;
    throw G4QException("G4QElastic::PostStepDoIt: Can't decay Elastic Compound");
  }
#ifdef tdebug
  G4cout<<"G4QElastic::PoStDoIt:s4M="<<scat4M<<"+r4M="<<reco4M<<"="<<scat4M+reco4M<<G4endl;
#endif
  // Update G4VParticleChange for the scattered muon
  G4double finE=scat4M.e()-pM;             // Final kinetic energy of the scattered proton
  if(finE>0) aParticleChange.ProposeEnergy(finE);
  else
  {
    G4cerr<<"*Warning*G4QElastic::PostStDoIt: Zero or negative scattered E="<<finE<<G4endl;
    aParticleChange.ProposeEnergy(0.) ;
    aParticleChange.ProposeTrackStatus(fStopAndKill);
  }
  G4ThreeVector findir=scat4M.vect()/scat4M.rho();  // Unit vector in new direction
  aParticleChange.ProposeMomentumDirection(findir); // new direction for the scattered part
  EnMomConservation-=scat4M;                        // It must be initialized by (pE+tM,pP)
  // This is how in general the secondary should be identified
		G4DynamicParticle* theSec = new G4DynamicParticle; // A secondary for the recoil hadron 
  //G4int targPDG=2212;                      // PDG for the recoil proton @@only for p targ
  //G4ParticleDefinition* theDefinition=G4Proton::Proton(); // @@ only for p target
  G4int aA = Z+N;
#ifdef pdebug
		G4cout<<"G4QElastic::PostStepDoIt: Ion Z="<<Z<<", A="<<aA<<G4endl;
#endif
  G4ParticleDefinition* theDefinition=G4ParticleTable::GetParticleTable()
                                                                       ->FindIon(Z,aA,0,Z);
  if(!theDefinition)G4cout<<"*Warning*G4QElastic::PostStepDoIt:drop PDG="<<targPDG<<G4endl;
#ifdef pdebug
  G4cout<<"G4QElastic::PostStepDoIt:Name="<<theDefinition->GetParticleName()<<G4endl;
#endif
  theSec->SetDefinition(theDefinition);

  EnMomConservation-=reco4M;
#ifdef tdebug
  G4cout<<"G4QElastic::PostSDoIt:"<<targPDG<<reco4M<<reco4M.m()<<EnMomConservation<<G4endl;
#endif
  theSec->Set4Momentum(reco4M);
#ifdef debug
  G4ThreeVector curD=theSec->GetMomentumDirection();
  G4double curM=theSec->GetMass();
  G4double curE=theSec->GetKineticEnergy()+curM;
  G4cout<<"G4QElastic::PSDoIt:p="<<curD<<curD.mag()<<",e="<<curE<<",m="<<curM<<G4endl;
#endif
  G4Track* aNewTrack = new G4Track(theSec, localtime, position );
  aParticleChange.AddSecondary( aNewTrack );
#ifdef debug
    G4cout<<"G4QElastic::PostStepDoIt: **** PostStepDoIt is done ****"<<G4endl;
#endif
  return G4VDiscreteProcess::PostStepDoIt(track, step);
}

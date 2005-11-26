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
// $Id: G4QCollision.cc,v 1.3 2005-11-26 07:53:25 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//      ---------------- G4QCollision class -----------------
//                 by Mikhail Kossov, December 2003.
// G4QCollision class of the CHIPS Simulation Branch in GEANT4
// ---------------------------------------------------------------
// ****************************************************************************************
// ********** This CLASS is temporary moved from the photolepton_hadron directory *********
// ****************************************************************************************

//#define debug
//#define pdebug

#include "G4QCollision.hh"

G4QCollision::G4QCollision(const G4String& processName) : G4VDiscreteProcess(processName)
{
#ifdef debug
  G4cout<<"G4QCollision::Constructor is called"<<G4endl;
#endif
  if (verboseLevel>0) G4cout << GetProcessName() << " process is created "<< G4endl;

  G4QCHIPSWorld::Get()->GetParticles(nPartCWorld); // Create CHIPS World with 234 particles
  G4QNucleus::SetParameters(freeNuc,freeDib,clustProb,mediRatio); // Clusterization param's
  G4Quasmon::SetParameters(Temperature,SSin2Gluons,EtaEtaprime);  // Hadronic parameters
  G4QEnvironment::SetParameters(SolidAngle); // SolAngle of pbar-A secondary mesons capture
  //@@ Initialize here the G4QuasmonString parameters
}

G4bool   G4QCollision::manualFlag=false; // If false then standard parameters are used
G4double G4QCollision::Temperature=180.; // Critical Temperature (sensitive at High En)
G4double G4QCollision::SSin2Gluons=0.3;  // Supression of s-quarks (in respect to u&d)
G4double G4QCollision::EtaEtaprime=0.3;  // Supression of eta mesons (gg->qq/3g->qq)
G4double G4QCollision::freeNuc=0.5;      // Percentage of free nucleons on the surface
G4double G4QCollision::freeDib=0.05;     // Percentage of free diBaryons on the surface
G4double G4QCollision::clustProb=5.;     // Nuclear clusterization parameter
G4double G4QCollision::mediRatio=10.;    // medium/vacuum hadronization ratio
G4int    G4QCollision::nPartCWorld=152;  // The#of particles initialized in CHIPS World
G4double G4QCollision::SolidAngle=0.5;   // Part of Solid Angle to capture (@@A-dep.)
G4bool   G4QCollision::EnergyFlux=false; // Flag for Energy Flux use (not MultyQuasmon)
G4double G4QCollision::PiPrThresh=141.4; // Pion Production Threshold for gammas
G4double G4QCollision::M2ShiftVir=20000.;// Shift for M2=-Q2=m_pi^2 of the virtualGamma
G4double G4QCollision::DiNuclMass=1880.; // DoubleNucleon Mass for VirtualNormalization
G4double G4QCollision::photNucBias=1.;   // BiasingParameter for photo(e,mu,tau)Nuclear
G4double G4QCollision::weakNucBias=1.;   // BiasingParameter for ChargedCurrents(nu,mu) 

void G4QCollision::SetManual()   {manualFlag=true;}
void G4QCollision::SetStandard() {manualFlag=false;}

// Fill the private parameters
void G4QCollision::SetParameters(G4double temper, G4double ssin2g, G4double etaetap,
                                     G4double fN, G4double fD, G4double cP, G4double mR,
                                     G4int nParCW, G4double solAn, G4bool efFlag,
                                     G4double piThresh, G4double mpisq, G4double dinum)
{//  =============================================================================
  Temperature=temper;
  SSin2Gluons=ssin2g;
  EtaEtaprime=etaetap;
  freeNuc=fN;
  freeDib=fD;
  clustProb=cP;
  mediRatio=mR;
  nPartCWorld = nParCW;
  EnergyFlux=efFlag;
  SolidAngle=solAn;
  PiPrThresh=piThresh;
  M2ShiftVir=mpisq;
  DiNuclMass=dinum;
  G4QCHIPSWorld::Get()->GetParticles(nPartCWorld); // Create CHIPS World with 234 particles
  G4QNucleus::SetParameters(freeNuc,freeDib,clustProb,mediRatio); // Clusterization param's
  G4Quasmon::SetParameters(Temperature,SSin2Gluons,EtaEtaprime);  // Hadronic parameters
  G4QEnvironment::SetParameters(SolidAngle); // SolAngle of pbar-A secondary mesons capture
}

void G4QCollision::SetPhotNucBias(G4double phnB) {photNucBias=phnB;}
void G4QCollision::SetWeakNucBias(G4double ccnB) {weakNucBias=ccnB;}

// Destructor

G4QCollision::~G4QCollision() {}


G4LorentzVector G4QCollision::GetEnegryMomentumConservation()
{
  return EnMomConservation;
}

G4int G4QCollision::GetNumberOfNeutronsInTarget()
{
  return nOfNeutrons;
}

G4double G4QCollision::GetMeanFreePath(const G4Track& aTrack,G4double,G4ForceCondition* Fc)
{
  *Fc = NotForced;
  const G4DynamicParticle* incidentParticle = aTrack.GetDynamicParticle();
  G4ParticleDefinition* incidentParticleDefinition=incidentParticle->GetDefinition();
  if( !IsApplicable(*incidentParticleDefinition))
    G4cout<<"-W-G4QCollision::GetMeanFreePath called for not implemented particle"<<G4endl;
  // Calculate the mean Cross Section for the set of Elements(*Isotopes) in the Material
  G4double Momentum = incidentParticle->GetTotalMomentum(); // 3-momentum of the Particle
  const G4Material* material = aTrack.GetMaterial();        // Get the current material
  const G4double* NOfNucPerVolume = material->GetVecNbOfAtomsPerVolume();
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int nE=material->GetNumberOfElements();
#ifdef debug
  G4cout<<"G4QCollision::GetMeanFreePath:"<<nE<<" Elem's in theMaterial"<<G4endl;
#endif
  G4bool leptoNuc=false;       // By default the reaction is not lepto-nuclear
  G4VQCrossSection* CSmanager=G4QProtonNuclearCrossSection::GetPointer();
  if(incidentParticleDefinition == G4Proton::Proton())
      CSmanager=G4QProtonNuclearCrossSection::GetPointer();
  else if(incidentParticleDefinition == G4Gamma::Gamma())
                                      CSmanager=G4QPhotonNuclearCrossSection::GetPointer();
  else if(incidentParticleDefinition == G4Electron::Electron() ||
          incidentParticleDefinition == G4Positron::Positron())
  {
    CSmanager=G4QElectronNuclearCrossSection::GetPointer();
    leptoNuc=true;
  }
  else if(incidentParticleDefinition == G4MuonPlus::MuonPlus() ||
          incidentParticleDefinition == G4MuonMinus::MuonMinus())
  {
    CSmanager=G4QMuonNuclearCrossSection::GetPointer();
    leptoNuc=true;
  }
  else if(incidentParticleDefinition == G4TauPlus::TauPlus() ||
          incidentParticleDefinition == G4TauMinus::TauMinus())
  {
    CSmanager=G4QTauNuclearCrossSection::GetPointer();
    leptoNuc=true;
  }
  else if(incidentParticleDefinition == G4NeutrinoMu::NeutrinoMu() )
  {
    CSmanager=G4QNuMuNuclearCrossSection::GetPointer();
    leptoNuc=true;
  }
  else if(incidentParticleDefinition == G4AntiNeutrinoMu::AntiNeutrinoMu() )
  {
    CSmanager=G4QANuMuNuclearCrossSection::GetPointer();
    leptoNuc=true;
  }
  else G4cout<<"G4QCollision::GetMeanFreePath:Particle isn't implemented in CHIPS"<<G4endl;
  
  G4QIsotope* Isotopes = G4QIsotope::Get(); // Pointer to the G4QIsotopes singelton
  G4double sigma=0.;
  for(G4int i=0; i<nE; ++i)
  {
    G4int Z = static_cast<G4int>((*theElementVector)[i]->GetZ()); // Z of the Element
    std::vector<std::pair<G4int,G4double>*>* cs= Isotopes->GetCSVector(Z); // Pointer to CS
    G4int nIs=cs->size();                         // A#Of Isotopes in the Element
    if(nIs) for(G4int j=0; j<nIs; j++)            // Calculate CS for eachIsotope of El
    {
      std::pair<G4int,G4double>* curIs=(*cs)[j];  // A pointer, which is used twice
      G4int N=curIs->first;                       // #ofNeuterons in the isotope
      curIs->second = CSmanager->GetCrossSection(Momentum, Z, N); // CS calculation
    } // End of temporary initialization of the cross sections in the G4QIsotope singeltone
    sigma+=Isotopes->GetMeanCrossSection(Z)*NOfNucPerVolume[i]; // SUM(MeanCS*NOFNperV)
  } // End of LOOP over Elements

  // Check that cross section is not zero and return the mean free path
  if(photNucBias!=1.) if(incidentParticleDefinition == G4MuonPlus::MuonPlus()   ||
                         incidentParticleDefinition == G4MuonMinus::MuonMinus() ||
                         incidentParticleDefinition == G4Electron::Electron()   ||
                         incidentParticleDefinition == G4Positron::Positron()   ||  
                         incidentParticleDefinition == G4TauMinus::TauMinus()   ||
                         incidentParticleDefinition == G4TauPlus::TauPlus()       )
                                                                        sigma*=photNucBias;
  if(photNucBias!=1.) if(incidentParticleDefinition==G4NeutrinoE::NeutrinoE()            ||
                         incidentParticleDefinition==G4AntiNeutrinoE::AntiNeutrinoE()    ||
                         incidentParticleDefinition==G4NeutrinoTau::NeutrinoTau()        ||
                         incidentParticleDefinition==G4AntiNeutrinoTau::AntiNeutrinoTau()||
                         incidentParticleDefinition==G4NeutrinoMu::NeutrinoMu()          ||
                         incidentParticleDefinition==G4AntiNeutrinoMu::AntiNeutrinoMu()   )
                                                                        sigma*=weakNucBias;
  if(sigma > 0.) return 1./sigma;                 // Mean path [distance] 
  return DBL_MAX;
}


G4bool G4QCollision::IsApplicable(const G4ParticleDefinition& particle) 
{
  if      (particle == *(      G4MuonPlus::MuonPlus()      )) return true;
  else if (particle == *(     G4MuonMinus::MuonMinus()     )) return true; 
  else if (particle == *(       G4TauPlus::TauPlus()       )) return true;
  else if (particle == *(      G4TauMinus::TauMinus()      )) return true;
  else if (particle == *(      G4Electron::Electron()      )) return true;
  else if (particle == *(      G4Positron::Positron()      )) return true;
  else if (particle == *(         G4Gamma::Gamma()         )) return true;
  else if (particle == *(        G4Proton::Proton()        )) return true;

  else if (particle == *(G4AntiNeutrinoMu::AntiNeutrinoMu())) return true;
  else if (particle == *(   G4NeutrinoMu::NeutrinoMu()   )) return true;
  //else if (particle == *(       G4Neutron::Neutron()       )) return true;
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
  G4cout<<"***G4QCollision::IsApplicable: PDG="<<particle.GetPDGEncoding()<<G4endl;
#endif
  return false;
}

G4VParticleChange* G4QCollision::PostStepDoIt(const G4Track& track, const G4Step& step)
{
  static const G4double mu=G4MuonMinus::MuonMinus()->GetPDGMass(); // muon mass
  static const G4double mu2=mu*mu;                                 // squared muon mass
  //static const G4double dpi=M_PI+M_PI;   // 2*pi (for Phi distr.) ***changed to twopi***
  static const G4double mNeut= G4QPDGCode(2112).GetMass();
  static const G4double mProt= G4QPDGCode(2212).GetMass();
  static const G4double dM=mProt+mNeut;                            // doubled nucleon mass
  //static const G4double mPi0 = G4QPDGCode(111).GetMass();
  //static const G4double mDeut= G4QPDGCode(2112).GetNuclMass(1,1,0);
  //static const G4double mPi  = G4QPDGCode(211).GetMass();
  //static const G4double mMu  = G4QPDGCode(13).GetMass();
  //static const G4double mTau = G4QPDGCode(15).GetMass();
  //static const G4double mEl  = G4QPDGCode(11).GetMass();
  //
  const G4DynamicParticle* projHadron = track.GetDynamicParticle();
  const G4ParticleDefinition* particle=projHadron->GetDefinition();
  G4bool scat=false;
  G4int  scatPDG=0;                                   // Must be filled if true
  G4LorentzVector proj4M=projHadron->Get4Momentum();
  G4LorentzVector scat4M=proj4M;                      // Must be filled if true
  G4double momentum = projHadron->GetTotalMomentum(); // 3-momentum of the Particle
  G4double Momentum=proj4M.rho();
  if(std::fabs(Momentum-momentum)>.001)
    G4cerr<<"G4QCollision::PostStepDoIt: P="<<Momentum<<"="<<momentum<<G4endl;
#ifdef debug
  G4double mp=proj4M.m();
  G4cout<<"G4QCollision::PostStepDoIt is called, P="<<Momentum<<"="<<momentum<<G4endl;
#endif
  if (!IsApplicable(*particle))  // Check applicability
  {
    G4cerr<<"G4QCollision::PostStepDoIt:Only gam,e+,e-,mu+,mu-,t+,t-,p are implemented."
          <<G4endl;
    return 0;
  }
  const G4Material* material = track.GetMaterial();      // Get the current material
  G4int Z=0;
  const G4ElementVector* theElementVector = material->GetElementVector();
  G4int i=0;
  G4double sum=0.;
  G4int nE=material->GetNumberOfElements();
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: "<<nE<<" elements in the material."<<G4endl;
#endif
  G4int projPDG=0;                           // PDG Code prototype for the captured hadron
  // Not all these particles are implemented yet (see Is Applicable)
  if      (particle ==        G4MuonPlus::MuonPlus()       ) projPDG=  -13;
  else if (particle ==       G4MuonMinus::MuonMinus()      ) projPDG=   13;
  else if (particle ==      G4NeutrinoMu::NeutrinoMu()     ) projPDG=   14;
  else if (particle ==  G4AntiNeutrinoMu::AntiNeutrinoMu() ) projPDG=  -14;
  else if (particle ==        G4Electron::Electron()       ) projPDG=   11;
  else if (particle ==        G4Positron::Positron()       ) projPDG=  -11;
  else if (particle ==       G4NeutrinoE::NeutrinoE()      ) projPDG=   12;
  else if (particle ==   G4AntiNeutrinoE::AntiNeutrinoE()  ) projPDG=  -12;
  else if (particle ==           G4Gamma::Gamma()          ) projPDG=   22;
  else if (particle ==          G4Proton::Proton()         ) projPDG= 2212;
  else if (particle ==         G4Neutron::Neutron()        ) projPDG= 2112;
  else if (particle ==       G4PionMinus::PionMinus()      ) projPDG= -211;
  else if (particle ==        G4PionPlus::PionPlus()       ) projPDG=  211;
  else if (particle ==        G4KaonPlus::KaonPlus()       ) projPDG= 2112;
  else if (particle ==       G4KaonMinus::KaonMinus()      ) projPDG= -321;
  else if (particle ==    G4KaonZeroLong::KaonZeroLong()   ) projPDG=  130;
  else if (particle ==   G4KaonZeroShort::KaonZeroShort()  ) projPDG=  310;
  else if (particle ==         G4TauPlus::TauPlus()        ) projPDG=  -15;
  else if (particle ==        G4TauMinus::TauMinus()       ) projPDG=   15;
  else if (particle ==     G4NeutrinoTau::NeutrinoTau()    ) projPDG=   16;
  else if (particle == G4AntiNeutrinoTau::AntiNeutrinoTau()) projPDG=  -16;
  else if (particle ==          G4Lambda::Lambda()         ) projPDG= 3122;
  else if (particle ==       G4SigmaPlus::SigmaPlus()      ) projPDG= 3222;
  else if (particle ==      G4SigmaMinus::SigmaMinus()     ) projPDG= 3112;
  else if (particle ==       G4SigmaZero::SigmaZero()      ) projPDG= 3212;
  else if (particle ==         G4XiMinus::XiMinus()        ) projPDG= 3312;
  else if (particle ==          G4XiZero::XiZero()         ) projPDG= 3322;
  else if (particle ==      G4OmegaMinus::OmegaMinus()     ) projPDG= 3334;
  else if (particle ==     G4AntiNeutron::AntiNeutron()    ) projPDG=-2112;
  else if (particle ==      G4AntiProton::AntiProton()     ) projPDG=-2212;
  G4int aProjPDG=std::abs(projPDG);
#ifdef debug
  G4int prPDG=particle->GetPDGEncoding();
		G4cout<<"G4QCollision::PostStepRestDoIt: projPDG="<<projPDG<<",stPDG="<<prPDG<<G4endl;
#endif
  if(!projPDG)
  {
    G4cerr<<"---Warning---G4QCollision::PostStepDoIt:Undefined interacting hadron"<<G4endl;
    return 0;
  }
  // @@ It's a standard randomization procedure, which can be placed in G4QMaterial class
  std::vector<G4double> sumfra;
  for(i=0; i<nE; ++i)
  {
	   G4double frac=material->GetFractionVector()[i];
    sum+=frac;
    sumfra.push_back(sum);             // remember the summation steps
  }
  G4double rnd = sum*G4UniformRand();
  for(i=0; i<nE; ++i) if (rnd<sumfra[i]) break;
  G4Element* pElement=(*theElementVector)[i];
  Z=static_cast<G4int>(pElement->GetZ());
  if(Z<=0)
  {
    G4cerr<<"---Warning---G4QCollision::PostStepDoIt:Element with Z="<<Z<< G4endl;
    if(Z<0) return 0;
  }
  G4int N = Z;
  G4int isoSize=0;                         // The default for the isoVectorLength is 0
  G4IsotopeVector* isoVector=pElement->GetIsotopeVector();
  if(isoVector) isoSize=isoVector->size(); // Get real size of the isotopeVector if exists
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: isovectorLength="<<isoSize<<G4endl;
#endif
  if(isoSize)                         // The Element has not trivial abumdance set
  {
    // @@ the following solution is temporary till G4Element can contain the QIsotopIndex
    G4int curInd=G4QIsotope::Get()->GetLastIndex(Z);
    if(!curInd)                       // The new artificial element must be defined 
				{
      std::vector<std::pair<G4int,G4double>*>* newAbund =
                                               new std::vector<std::pair<G4int,G4double>*>;
      G4double* abuVector=pElement->GetRelativeAbundanceVector();
      for(G4int j=0; j<isoSize; j++)
      {
        N=pElement->GetIsotope(j)->GetN()-Z;
        if(pElement->GetIsotope(j)->GetZ()!=Z) G4cerr<<"*G4QCaptureAtRest::AtRestDoIt: Z="
																																							 <<pElement->GetIsotope(j)->GetZ()<<"#"<<Z<<G4endl;
        G4double abund=abuVector[j];
								std::pair<G4int,G4double>* pr= new std::pair<G4int,G4double>(N,abund);
#ifdef debug
        G4cout<<"G4QCollision::PostStepDoIt:pair#="<<j<<", N="<<N<<",ab="<<abund<<G4endl;
#endif
        newAbund->push_back(pr);
						}
#ifdef debug
      G4cout<<"G4QCollision::PostStepDoIt: pairVectorLength="<<newAbund->size()<<G4endl;
#endif
      curInd=G4QIsotope::Get()->InitElement(Z,1,newAbund);
      for(G4int k=0; k<isoSize; k++) delete (*newAbund)[k];
      delete newAbund;
    }
    // @@ ^^^^^^^^^^ End of the temporary solution ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    N = G4QIsotope::Get()->GetNeutrons(Z,curInd);
  }
  else  N = G4QIsotope::Get()->GetNeutrons(Z);
  nOfNeutrons=N;                                       // Remember it for energy-mom. check
  G4double dd=0.025;
  G4double am=Z+N;
  G4double sr=std::sqrt(am);
  G4double dsr=0.01*(sr+sr);
  if(dsr<dd)dsr=dd;
  if(manualFlag) G4QNucleus::SetParameters(freeNuc,freeDib,clustProb,mediRatio); // ManualP
		else if(projPDG==-2212) G4QNucleus::SetParameters(1.-dsr-dsr,dd+dd,5.,10.);//aP ClustPars
  else if(projPDG==-211)  G4QNucleus::SetParameters(.67-dsr,.32-dsr,5.,9.);//Pi- ClustPars
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: N="<<N<<" for element with Z="<<Z<<G4endl;
#endif
  if(N<0)
  {
    G4cerr<<"---Warning---G4QCollision::PostStepDoIt:Element with N="<<N<< G4endl;
    return 0;
  }
		if(aProjPDG==11 || aProjPDG==13 || aProjPDG==15) // leptons with photonuclear
		{ // Lepto-nuclear case with the equivalent photon algorithm. @@InFuture + neutrino & QE
    G4double kinEnergy= projHadron->GetKineticEnergy();
    G4ParticleMomentum dir = projHadron->GetMomentumDirection();
    G4VQCrossSection* CSmanager=G4QElectronNuclearCrossSection::GetPointer();
    if(aProjPDG== 13) CSmanager=G4QMuonNuclearCrossSection::GetPointer();
    if(aProjPDG== 15) CSmanager=G4QTauNuclearCrossSection::GetPointer();
    G4double xSec=CSmanager->GetCrossSection(Momentum, Z, N);// Recalculate Cross Section
    // @@ check a possibility to separate p, n, or alpha (!)
    if(xSec <= 0.) // The cross-section iz 0 -> Do Nothing
    {
      //Do Nothing Action insead of the reaction
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    G4double photonEnergy = CSmanager->GetExchangeEnergy(); // Energy of EqivExchangePart
    if( kinEnergy < photonEnergy )
    {
      //Do Nothing Action insead of the reaction
      G4cerr<<"G4QCollision::PSDoIt: photE="<<photonEnergy<<">leptE="<<kinEnergy<<G4endl;
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    G4double photonQ2 = CSmanager->GetExchangeQ2(photonEnergy);// Q2(t) of EqivExchangePart
    G4double W=photonEnergy-photonQ2/dM;// HadronicEnergyFlow (W-energy) for virtual photon
    if(W<0.) 
    {
      //Do Nothing Action insead of the reaction
      G4cout << "G4QCollision::PostStepDoIt:(lN) negative equivalent energy W="<<W<<G4endl;
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    // Update G4VParticleChange for the scattered muon
    G4VQCrossSection* thePhotonData=G4QPhotonNuclearCrossSection::GetPointer();
    G4double sigNu=thePhotonData->GetCrossSection(photonEnergy, Z, N);// IntegratedCrossSec
    G4double sigK =thePhotonData->GetCrossSection(W, Z, N);           // Real CrossSec
    G4double rndFraction = CSmanager->GetVirtualFactor(photonEnergy, photonQ2);
    if(sigNu*G4UniformRand()>sigK*rndFraction) 
    {
      //Do NothingToDo Action insead of the reaction
      G4cout << "G4QCollision::PostStepDoIt: probability correction - DoNothing"<<G4endl;
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    G4double iniE=kinEnergy+mu;          // Initial total energy of the muon
    G4double finE=iniE-photonEnergy;     // Final total energy of the muon
    if(finE>0) aParticleChange.ProposeEnergy(finE) ;
    else
    {
      aParticleChange.ProposeEnergy(0.) ;
      aParticleChange.ProposeTrackStatus(fStopAndKill);
    }
    // Scatter the muon
    G4double EEm=iniE*finE-mu2;          // Just an intermediate value to avoid "2*"
    G4double iniP=std::sqrt(iniE*iniE-mu2);   // Initial momentum of the electron
    G4double finP=std::sqrt(finE*finE-mu2);   // Final momentum of the electron
    G4double cost=(EEm+EEm-photonQ2)/iniP/finP; // cos(theta) for the electron scattering
    if(cost>1.) cost=1.;                 // To avoid the accuracy of calculation problem
    //else if(cost>1.001)                // @@ error report can be done, but not necessary
    if(cost<-1.) cost=-1.;               // To avoid the accuracy of calculation problem
    //else if(cost<-1.001)               // @@ error report can be done, but not necessary
    // --- Example from electromagnetic physics --
    //G4ThreeVector newMuonDirection(dirx,diry,dirz);
    //newMuonDirection.rotateUz(dir);
    //aParticleChange.ProposeMomentumDirection(newMuonDirection1) ;
    // The scattering in respect to the derection of the incident muon is made impicitly:
    G4ThreeVector ort=dir.orthogonal();  // Not normed orthogonal vector (!) (to dir)
    G4ThreeVector ortx = ort.unit();     // First unit vector orthogonal to the direction
    G4ThreeVector orty = dir.cross(ortx);// Second unit vector orthoganal to the direction
    G4double sint=std::sqrt(1.-cost*cost);    // Perpendicular component
    G4double phi=twopi*G4UniformRand();  // phi of scattered electron
    G4double sinx=sint*std::sin(phi);         // x-component
    G4double siny=sint*std::cos(phi);         // y-component
    G4ThreeVector findir=cost*dir+sinx*ortx+siny*orty;
    aParticleChange.ProposeMomentumDirection(findir); // new direction for the muon
    const G4ThreeVector photon3M=iniP*dir-finP*findir;
    projPDG=22;
    proj4M=G4LorentzVector(photon3M,photon3M.mag()); //@@ photon is real?
  }
		if(aProjPDG==14) // *** neutrino nuclear interactions (only nu_mu & only CC) ***
		{ // Lepto-nuclear case with the equivalent photon algorithm. @@InFuture + neutrino & QE
    G4double kinEnergy= projHadron->GetKineticEnergy(); // For neutrino it is total energy
    G4ParticleMomentum dir = projHadron->GetMomentumDirection();
    G4VQCrossSection* CSmanager=G4QNuMuNuclearCrossSection::GetPointer();
    G4bool nuanu=true;
    scatPDG=13;                         // Prototype = secondary scattered mu-
    if(aProjPDG== 14)
    {
      nuanu=false;
      CSmanager=G4QANuMuNuclearCrossSection::GetPointer(); // @@ open
      scatPDG=-13;                    // secondary scattered mu+
    }
    G4double xSec=CSmanager->GetCrossSection(Momentum, Z, N);// Recalculate Cross Section
    // @@ check a possibility to separate p, n, or alpha (!)
    if(xSec <= 0.) // The cross-section iz 0 -> Do Nothing
    {
      //Do Nothing Action insead of the reaction
      aParticleChange.ProposeEnergy(kinEnergy);
      aParticleChange.ProposeLocalEnergyDeposit(0.);
      aParticleChange.ProposeMomentumDirection(dir) ;
      return G4VDiscreteProcess::PostStepDoIt(track,step);
    }
    scat=true;                                  // event with changed scattered projectile
    G4double totCS = CSmanager->GetLastTOTCS(); // the last total cross section (isotope?)
    G4double qelCS = CSmanager->GetLastQELCS(); // the last total cross section
    if(totCS - qelCS < 0.) totCS = qelCS;
    if(totCS*G4UniformRand()<qelCS)             // ***** Quasi Elastic interaction
    {
      G4double Q2=CSmanager->GetQEL_ExchangeQ2();
      G4double mIN=mProt;                       // @@ split from the nucleus
      G4double mOT=mNeut;
      if(nuanu)
      {
        mIN=mNeut;                              // @@ split from the nucleus
        mOT=mProt;
        projPDG=2212;                           // proton is going out
      }
      else projPDG=2112;                        // neutron is going out
      // make a new projectile from the scattered nucleon
      proj4M=G4LorentzVector(Q2,mIN,mOT,Q2);    // 4m of the pion
    }
    else                                        // ***** Non Quasi Elastic interaction
    {
      G4double Q2=CSmanager->GetNQE_ExchangeQ2();
      projPDG=CSmanager->GetExchangePDGCode();
      proj4M=G4LorentzVector(Q2,Q2,Q2,Q2);      // 4m of the pion
    }
    aParticleChange.ProposeEnergy(0.) ;
    aParticleChange.ProposeTrackStatus(fStopAndKill); // the initial neutrino is killed
  }
  G4int targPDG=90000000+Z*1000+N;       // PDG Code of the target nucleus
  G4QPDGCode targQPDG(targPDG);
  G4double tM=targQPDG.GetMass();
  EnMomConservation=proj4M+G4LorentzVector(0.,0.,0.,tM);    // Total 4-mom of the reaction
  G4QHadronVector* output=new G4QHadronVector; // Prototype of the output G4QHadronVector
  // @@@@@@@@@@@@@@ Temporary for the testing purposes --- Begin
  //G4bool elF=false; // Flag of the ellastic scattering is "false" by default
  //G4double eWei=1.;
  // @@@@@@@@@@@@@@ Temporary for the testing purposes --- End  
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: projPDG="<<projPDG<<", targPDG="<<targPDG<<G4endl;
#endif
  G4QHadron* pH = new G4QHadron(projPDG,proj4M);                // ---> DELETED -->--   -+
  if(momentum<1000.) // Condition for using G4QEnvironment (not G4QuasmonString)         |
		{ //                                                                                   |
    G4QHadronVector projHV;                                 //                           |
    projHV.push_back(pH);                                   // DESTROYED over 2 lines -+ |
    G4QEnvironment* pan= new G4QEnvironment(projHV,targPDG);// ---> DELETED --->-----+ | |
    std::for_each(projHV.begin(), projHV.end(), DeleteQHadron()); // <---<------<----+-+-+
    projHV.clear(); // <------------<---------------<-------------------<------------+-+ .
#ifdef debug
    G4cout<<"G4QCollision::PostStepDoIt: pPDG="<<projPDG<<", mp="<<mp<<G4endl; //    |   .
#endif
    try                                                           //                 |   .
	   {                                                             //                 |   .
	     delete output;                                              //                 |   .
      output = pan->Fragment();// DESTROYED in the end of the LOOP work space        |   .
    }                                                             //                 |   .
    catch (G4QException& error)//                                                    |   .
	   {                                                             //                 |   .
	     //#ifdef pdebug
      G4cerr<<"***G4QCollision::PostStepDoIt: G4QE Exception is catched"<<G4endl; // |   .
	     //#endif
      G4Exception("G4QCollision::PostStepDoIt:","27",FatalException,"CHIPS Crash");//|   .
    }                                                             //                 |   .
    delete pan;                              // Delete the Nuclear Environment <--<--+   .
  } //                                                                                   .
  else               // Use G4QuasmonString                                              .
		{ //                                                                                   ^
    G4QuasmonString* pan= new G4QuasmonString(pH,false,targPDG,false);//-> DELETED --+   |
    delete pH;                                                    // --------<-------+---+
#ifdef debug
    G4double mp=G4QPDGCode(projPDG).GetMass();   // Mass of the projectile particle  |
    G4cout<<"G4QCollision::PostStepDoIt: pPDG="<<projPDG<<", pM="<<mp<<G4endl; //    |
#endif
    //G4int tNH=0;                    // Prototype of the number of secondaries inOut|
    try                                                           //                 |
	   {                                                             //                 |
				  delete output;                                            //                   |
      output = pan->Fragment();// DESTROYED in the end of the LOOP work space        |
      // @@@@@@@@@@@@@@ Temporary for the testing purposes --- Begin                 |
      //tNH=pan->GetNOfHadrons();     // For the test purposes of the String         |
      //if(tNH==2)                    // At least 2 hadrons are in the Constr.Output |
				  //{//                                                                          |
      //  elF=true;                   // Just put a flag for the ellastic Scattering |
	     //  delete output;              // Delete a prototype of dummy G4QHadronVector |
      //  output = pan->GetHadrons(); // DESTROYED in the end of the LOOP work space |
      //}//                                                                          |
      //eWei=pan->GetWeight();        // Just an example for the weight of the event |
#ifdef debug
      //G4cout<<"=====>>G4QCollision::PostStepDoIt: elF="<<elF<<",n="<<tNH<<G4endl;//|
#endif
      // @@@@@@@@@@@@@@ Temporary for the testing purposes --- End                   |
    }                                                             //                 |
    catch (G4QException& error)//                                                    |
	   {                                                             //                 |
	     //#ifdef pdebug
      G4cerr<<"***G4QCollision::PostStepDoIt: GEN Exception is catched"<<G4endl; //  |
	     //#endif
      G4Exception("G4QCollision::AtRestDoIt:","27",FatalException,"QString Excep");//|
    }                                                             //                 |
    delete pan;                              // Delete the Nuclear Environment ---<--+
  }
  aParticleChange.Initialize(track);
  G4double localtime = track.GetGlobalTime();
  G4ThreeVector position = track.GetPosition();
  // --- the scattered hadron with changed nature can be added here ---
  if(scat)
  {
    G4QHadron* scatHadron = new G4QHadron(scatPDG,scat4M);
    output->push_back(scatHadron);
  }
  // ------------- From here the secondaries are filled -------------------------
  G4int tNH = output->size();       // A#of hadrons in the output
  aParticleChange.SetNumberOfSecondaries(tNH); 
  // Now add nuclear fragments
#ifdef debug
  G4cout<<"G4QCollision::PostStepDoIt: "<<tNH<<" particles are generated"<<G4endl;
#endif
  G4int nOut=output->size();               // Real length of the output @@ Temporary
  if(tNH==1 && !scat)                      // @@ Temporary. Find out why it happened!
  {
    G4cout<<"-Warning-G4QCollision::PostStepDoIt: only one secondary! Make 0."<<G4endl;
    tNH=0;
    delete output->operator[](0);          // delete the creazy hadron
    output->pop_back();                     // clean up the output vector
  }
  if(tNH==2&&2!=nOut) G4cout<<"--Warning--G4QCollision::PostStepDoIt: 2 # "<<nOut<<G4endl;
  // Deal with ParticleChange final state interface to GEANT4 output of the process
  //if(tNH==2) for(i=0; i<tNH; i++) // @@ Temporary tNH==2 instead of just tNH
  if(tNH) for(i=0; i<tNH; i++) // @@ Temporary tNH==2 instead of just tNH
  {
    // Note that one still has to take care of Hypernuclei (with Lambda or Sigma inside)
    // Hypernucleus mass calculation and ion-table interface upgrade => work for Hisaya @@
    // The decau process for hypernuclei must be developed in GEANT4 (change CHIPS body)
    G4QHadron* hadr=output->operator[](i);   // Pointer to the output hadron    
    G4int PDGCode = hadr->GetPDGCode();
    G4int nFrag   = hadr->GetNFragments();
#ifdef pdebug
    G4cout<<"G4QCollision::AtRestDoIt: H#"<<i<<",PDG="<<PDGCode<<",nF="<<nFrag<<G4endl;
#endif
    if(nFrag)                // Skip intermediate (decayed) hadrons
    {
#ifdef debug
	     G4cout<<"G4QCollision::PostStepDoIt: Intermediate particle is found i="<<i<<G4endl;
#endif
      delete hadr;
      continue;
    }
    G4DynamicParticle* theSec = new G4DynamicParticle;  
    G4ParticleDefinition* theDefinition;
    if     (PDGCode==90000001) theDefinition = G4Neutron::Neutron();
    else if(PDGCode==90001000) theDefinition = G4Proton::Proton();//While it can be in ions
    else if(PDGCode==91000000) theDefinition = G4Lambda::Lambda();
    else if(PDGCode==311 || PDGCode==-311)
    {
      if(G4UniformRand()>.5) theDefinition = G4KaonZeroLong::KaonZeroLong();   // K_L
						else                   theDefinition = G4KaonZeroShort::KaonZeroShort(); // K_S
    }
    else if(PDGCode==91000999) theDefinition = G4SigmaPlus::SigmaPlus();
    else if(PDGCode==90999001) theDefinition = G4SigmaMinus::SigmaMinus();
    else if(PDGCode==91999000) theDefinition = G4XiMinus::XiMinus();
    else if(PDGCode==91999999) theDefinition = G4XiZero::XiZero();
    else if(PDGCode==92998999) theDefinition = G4OmegaMinus::OmegaMinus();
	   else if(PDGCode >80000000) // Defines hypernuclei as normal nuclei (N=N+S Correction!)
    {
      G4int aZ = hadr->GetCharge();
      G4int aA = hadr->GetBaryonNumber();
#ifdef pdebug
						G4cout<<"G4QCollision::AtRestDoIt:Ion Z="<<aZ<<", A="<<aA<<G4endl;
#endif
      theDefinition = G4ParticleTable::GetParticleTable()->FindIon(aZ,aA,0,aZ);
    }
    //else theDefinition = G4ParticleTable::GetParticleTable()->FindParticle(PDGCode);
    else
    {
#ifdef pdebug
						G4cout<<"G4QCollision::PostStepDoIt:Define particle with PDG="<<PDGCode<<G4endl;
#endif
      theDefinition = G4QPDGToG4Particle::Get()->GetParticleDefinition(PDGCode);
#ifdef pdebug
						G4cout<<"G4QCollision::PostStepDoIt:AfterParticleDefinition PDG="<<PDGCode<<G4endl;
#endif
    }
    if(!theDefinition)
    {
      G4cout<<"---Warning---G4QCollision::PostStepDoIt: drop PDG="<<PDGCode<<G4endl;
      delete hadr;
      continue;
    }
#ifdef pdebug
    G4cout<<"G4QCollision::PostStepDoIt:Name="<<theDefinition->GetParticleName()<<G4endl;
#endif
    theSec->SetDefinition(theDefinition);
    G4LorentzVector h4M=hadr->Get4Momentum();
    EnMomConservation-=h4M;
#ifdef tdebug
    G4cout<<"G4QCollis::PSDI:"<<i<<","<<PDGCode<<h4M<<h4M.m()<<EnMomConservation<<G4endl;
#endif
#ifdef debug
    G4cout<<"G4QCollision::PostStepDoIt:#"<<i<<",PDG="<<PDGCode<<",4M="<<h4M<<G4endl;
#endif
    theSec->Set4Momentum(h4M); //                                                         ^
    delete hadr; // <-----<-----------<-------------<---------------------<---------<-----+
#ifdef debug
    G4ThreeVector curD=theSec->GetMomentumDirection();              //                    ^
    G4double curM=theSec->GetMass();                                //                    |
    G4double curE=theSec->GetKineticEnergy()+curM;                  //                    ^
    G4cout<<"G4QCollis::PSDoIt:p="<<curD<<curD.mag()<<",e="<<curE<<",m="<<curM<<G4endl;// |
#endif
    G4Track* aNewTrack = new G4Track(theSec, localtime, position ); //                    ^
    aParticleChange.AddSecondary( aNewTrack ); //                                         |
#ifdef debug
    G4cout<<"G4QCollision::PostStepDoIt:#"<<i<<" is done."<<G4endl; //                    |
#endif
  } //                                                                                    |
  delete output; // instances of the G4QHadrons from the output are already deleted above +
  aParticleChange.ProposeTrackStatus(fStopAndKill);        // Kill the absorbed particle
  //return &aParticleChange;                               // This is not enough (ClearILL)
#ifdef debug
    G4cout<<"G4QCollision::PostStepDoIt: **** PostStepDoIt is done ****"<<G4endl;
#endif
  return G4VDiscreteProcess::PostStepDoIt(track, step);
}

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
// $Id: G4MultipleScattering.cc,v 1.48 2006-03-07 15:56:41 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -----------------------------------------------------------------------------
// 16/05/01 value of cparm changed , L.Urban
// 18/05/01 V.Ivanchenko Clean up against Linux ANSI compilation
// 07/08/01 new methods Store/Retrieve PhysicsTable (mma)
// 23-08-01 new angle and z distribution,energy dependence reduced,
//          Store,Retrieve methods commented out temporarily, L.Urban
// 27-08-01 in BuildPhysicsTable:aParticleType.GetParticleName()=="mu+" (mma)
// 28-08-01 GetContinuousStepLimit and AlongStepDoIt moved from .icc file (mma)
// 03-09-01 value of data member factlim changed, L.Urban
// 10-09-01 small change in GetContinuousStepLimit, L.Urban
// 11-09-01 G4MultipleScatteringx put as default G4MultipleScattering
//          store/retrieve physics table reactivated (mma)
// 13-09-01 corr. in ComputeTransportCrossSection, L.Urban
// 14-09-01 protection in GetContinuousStepLimit, L.Urban
// 17-09-01 migration of Materials to pure STL (mma)
// 27-09-01 value of data member factlim changed, L.Urban
// 31-10-01 big fixed in PostStepDoIt,L.Urban
// 17-04-02 NEW angle distribution + boundary algorithm modified, L.Urban
// 22-04-02 boundary algorithm modified -> important improvement in timing 
// 24-04-02 some minor changes in boundary algorithm, L.Urban
// 06-05-02 bug fixed in GetContinuousStepLimit, L.Urban
// 24-05-02 changes in angle distribution and boundary algorithm, L.Urban
// 11-06-02 bug fixed in ComputeTransportCrossSection, L.Urban
// 12-08-02 bug fixed in PostStepDoIt (lateral displacement), L.Urban
// 15-08-02 new angle distribution, L.Urban
// 26-09-02 angle distribution + boundary algorithm modified, L.Urban
// 15-10-02 temporary fix for proton scattering
// 30-10-02 modified angle distribution,mods in boundary algorithm,
//          changes in data members, L.Urban
// 11-12-02 precision problem in ComputeTransportCrossSection
//          for small Tkin/for heavy particles cured from L.Urban
// 20-01-03 Migrade to cut per region (V.Ivanchenko)
// 05-02-03 changes in data members, new sampling for geom.
//          path length, step dependence reduced with new
//          method
// 28-03-03 Move to model design (V.Ivanchenko)
// 08-08-03 STD substitute standard  (V.Ivanchenko)
// 23-04-04 value of data member dtrl changed from 0.15 to 0.05 (L.Urban)
// 17-08-04 name of facxsi changed to factail (L.Urban)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 07-02-05 correction in order to have a working Setsamplez function (L.Urban)
// 15-04-05 optimize internal interface (V.Ivanchenko)
// 12-09-05 new TruePathLengthLimit - facrange works for every track from
//             start, geometry also influences the limit
// 02-10-05 conditions limiting the step are finalized + code cleaning (L.Urban)
// 03-10-05 weaker step limitation for Tkin > Tlimit (L.Urban)
// 05-10-05 value of data member tlimitmin has been changed (L.Urban)
// 06-10-05 correction in TruePathLengthLimit, timing improved.(L.Urban)
// 07-10-05 bug fixed in TruePathLengthLimit (L.Urban)
// 11-10-05 change in TruePathLengthLimit conditions,slightly better 
//          timing and much weaker cut dependence (L.Urban)
// 13-10-05 move SetFacrange(0.02) from InitialiseProcess to constructor
// 23-10-05 new Boolean data member prec (false ~ 7.1 like, true new step
//          limit in TruePathLengthLimit, L.Urban)
// 25-10-05 prec renamed to steppingAlgorithm, set function triggers
//          'default' facrange too, true - 0.02, false - 0.2 (L.Urban)
// 26-10-05 the above is put in the function MscStepLimitation() (mma)
// 05-11-05 tlimitmin = facrange*rungecut (instead of a fixed value)L.Urban
// 13-11-05 some code cleaning, slightly better timing (L.Urban)
// 01-12-05 add control on verbosity in SetMscStepLimitation
// 06-12-05 tlimitmin = facrange*rangecut(e-) for every particle
// 07-12-05 volume name World removed, rangecut computed using index
//          instead of particle name L.Urban
// 08-12-05 world is now: navigator->GetWorldVolume() L.Urban
// 11-12-05 data member rangecut removed, steplimit does not depend
//          on cut any more (L.Urban)
// 17-01-06 value of data member factail changed (1. --> 0.75),
//          value of facgeom is 3.5 instead of 4 (L.Urban)
// 19-01-07 tlimitmin = facrange*50*micrometer, i.e. it depends on the
//          value of facrange (L.Urban) 
// 16-02-06 value of factail changed, samplez = true (L.Urban)
//
// -----------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4MultipleScattering.hh"
#include "G4UrbanMscModel.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

G4MultipleScattering::G4MultipleScattering(const G4String& processName)
  : G4VMultipleScattering(processName)
{
  lowKineticEnergy  = 0.1*keV;
  highKineticEnergy = 100.*TeV;
  totBins           = 120;

  facrange          = 0.02;
  dtrl              = 0.05;
  tkinlimit         = 2.*MeV;
  facgeom           = 3.5;
  factail           = 0.75; 
  
  steppingAlgorithm = true;
  samplez           = true ;  
  isInitialized     = false;  

  SetBinning(totBins);
  SetMinKinEnergy(lowKineticEnergy);
  SetMaxKinEnergy(highKineticEnergy);

  SetLateralDisplasmentFlag(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4MultipleScattering::~G4MultipleScattering()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4MultipleScattering::IsApplicable (const G4ParticleDefinition& p)
{
  return (p.GetPDGCharge() != 0.0 && !p.IsShortLived());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4MultipleScattering::MscStepLimitation(G4bool algorithm, G4double factor) 
{
  steppingAlgorithm = algorithm;
  if (factor > 0.) SetFacrange(factor);
  else { if (algorithm) SetFacrange(0.02); else SetFacrange(0.2);}

  if(verboseLevel > 1)  
    G4cout << "Stepping algorithm is set to " << steppingAlgorithm 
	   << " with facrange = " << facrange << G4endl;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4MultipleScattering::InitialiseProcess(const G4ParticleDefinition* p)
{
  if(isInitialized) return;

  if (p->GetParticleType() == "nucleus") {
    steppingAlgorithm = false;
    SetLateralDisplasmentFlag(false);
    SetBuildLambdaTable(false);
  } else {
    SetBuildLambdaTable(true);
  }
  // compute Tlimit for particle
  G4UrbanMscModel* em = new G4UrbanMscModel(facrange,dtrl,tkinlimit,
					    facgeom,factail,
					    samplez,steppingAlgorithm);
  em->SetLateralDisplasmentFlag(LateralDisplasmentFlag());
  em->SetLowEnergyLimit(lowKineticEnergy);
  em->SetHighEnergyLimit(highKineticEnergy);
  AddEmModel(1, em);
  isInitialized = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4MultipleScattering::PrintInfo()
{
  G4cout << "      Boundary/stepping algorithm is active with facrange= "
	 << facrange
	 << "  Step limitation " << steppingAlgorithm
	 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


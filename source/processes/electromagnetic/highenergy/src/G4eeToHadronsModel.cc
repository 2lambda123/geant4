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
// $Id: G4eeToHadronsModel.cc,v 1.1 2004-11-19 18:46:14 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4eeToHadronsModel
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 12.08.2003
//
// Modifications:
//
//
// -------------------------------------------------------------------
//


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4eeToHadronsModel.hh"
#include "Randomize.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4Positron.hh"
#include "G4PionPlus.hh"
#include "Randomize.hh"
#include "G4Vee2hadrons.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsLogVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4eeToHadronsModel::G4eeToHadronsModel(const G4Vee2hadrons* m, 
				             G4int ver,
                                       const G4String& nam)
  : G4VEmModel(nam),
  model(m),
  crossPerElectron(0),
  crossBornPerElectron(0),
  isInitialised(false),
  nbins(100),
  verbose(ver)
{
  highKinEnergy = 0.1*TeV;
  lowKinEnergy  = 0.0;
  theGamma      = G4Gamma::Gamma();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4eeToHadronsModel::~G4eeToHadronsModel()
{
  delete model;
  delete crossPerElectron;
  delete crossBornPerElectron;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4eeToHadronsModel::HighEnergyLimit(const G4ParticleDefinition*)
{
  return highKinEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4eeToHadronsModel::LowEnergyLimit(const G4ParticleDefinition*)
{
  return lowKinEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToHadronsModel::SetHighEnergyLimit(G4double e)
{
  highKinEnergy = e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToHadronsModel::SetLowEnergyLimit(G4double e)
{
  lowKinEnergy = e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4eeToHadronsModel::IsInCharge(const G4ParticleDefinition* p)
{
  return (p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToHadronsModel::Initialise(const G4ParticleDefinition*,
                                    const G4DataVector&)
{
  if(isInitialised) return;
  isInitialised  = true;

  emin  = model->ThresholdEnergy();
  emax = 2.0*electron_mass_c2*sqrt(1.0 + 0.5*highKinEnergy/electron_mass_c2);
  if(emin > emax) emin = emax;

  lowKinEnergy  = 0.5*emin*emin/electron_mass_c2 - 2.0*electron_mass_c2;

  epeak = std::min(model->PeakEnergy(), emax);
  peakKinEnergy  = 0.5*epeak*epeak/electron_mass_c2 - 2.0*electron_mass_c2;

  if(verbose>0) {
    G4cout << "G4eeToHadronsModel::Initialise: " << G4endl;
    G4cout << "LabSystem: emin(GeV)= " << lowKinEnergy/GeV
           << " epeak(GeV)= " << peakKinEnergy/GeV
           << " emax(GeV)= " << highKinEnergy/GeV
           << G4endl;
    G4cout << "SM System: emin(MeV)= " << emin/MeV
           << " epeak(MeV)= " << epeak/MeV
           << " emax(MeV)= " << emax/MeV
           << G4endl;
  }

  if(lowKinEnergy < peakKinEnergy) {
    crossBornPerElectron = model->PhysicsVector(emin, emax); 
    crossPerElectron     = model->PhysicsVector(emin, emax);
    nbins = crossPerElectron->GetVectorLength();
    for(G4int i=0; i<nbins; i++) {
      G4double e  = crossPerElectron->GetLowEdgeEnergy(i);
      G4double cs = model->ComputeCrossSection(e);     
      crossBornPerElectron->PutValue(i, cs);
    }
    ComputeCMCrossSectionPerElectron();         
  }
  if(verbose>0) {
    G4cout << "G4eeToHadronsModel: Cross secsions per electron" 
           << " nbins= " << nbins
           << " emin(MeV)= " << emin/MeV
           << " emax(MeV)= " << emax/MeV
           << G4endl;
    G4bool b;   
    for(G4int i=0; i<nbins; i++) {
      G4double e  = crossPerElectron->GetLowEdgeEnergy(i);
      G4double s1 = crossPerElectron->GetValue(e, b);
      G4double s2 = crossBornPerElectron->GetValue(e, b);
      G4cout << "E(MeV)= " << e/MeV 
             << "  cross(nb)= " << s1/nanobarn 
             << "  crossBorn(nb)= " << s2/nanobarn 
	     << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4eeToHadronsModel::ComputeDEDX(const G4MaterialCutsCouple*,
                                        const G4ParticleDefinition*,
                                              G4double,
                                              G4double)
{
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4eeToHadronsModel::CrossSection(const G4MaterialCutsCouple* couple,
                                          const G4ParticleDefinition*,
                                                G4double kineticEnergy,
                                                G4double,
                                                G4double)
{
  G4double cross = 0.0;
  if(crossPerElectron) {
    G4bool b;   
    G4double e = 2.0*electron_mass_c2*
                 sqrt(1.0 + 0.5*kineticEnergy/electron_mass_c2);
    cross = (couple->GetMaterial()->GetElectronDensity())*
            (crossPerElectron->GetValue(e, b));
  }
  //  G4cout << "e= " << kineticEnergy << " cross= " << cross << G4endl;
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4DynamicParticle* G4eeToHadronsModel::SampleSecondary(
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle*,
                                   G4double,
                                   G4double)
{
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

std::vector<G4DynamicParticle*>* G4eeToHadronsModel::SampleSecondaries(
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle* dParticle,
                                   G4double,
                                   G4double)
{
  std::vector<G4DynamicParticle*>* newp = 0;
  if(crossPerElectron) {
    G4double t = dParticle->GetKineticEnergy();
    G4double e = 2.0*electron_mass_c2*sqrt(1.0 + 0.5*t/electron_mass_c2);
    G4LorentzVector inlv = dParticle->Get4Momentum();
    G4ThreeVector inBoost = inlv.boostVector();
    if(e > emin) {
      G4DynamicParticle* gamma = GenerateCMPhoton(e);
      G4LorentzVector gLv = gamma->Get4Momentum();
      G4LorentzVector lv(0.0,0.0,0.0,e);
      lv -= gLv;
      G4double m = lv.m();
      G4ThreeVector boost = lv.boostVector();
      const G4ThreeVector dir = gamma->GetMomentumDirection();
      newp = model->SampleSecondaries(m, dir);
      if(newp) {
        G4int np = newp->size();
        for(G4int j=0; j<np; j++) {
          G4DynamicParticle* dp = (*newp)[j];
          G4LorentzVector v = dp->Get4Momentum();
          v.boost(boost);
          v.boost(inBoost);
          dp->Set4Momentum(v);
	}
      } else {
        newp = new std::vector<G4DynamicParticle*>;
      }
      gLv.boost(inBoost);
      gamma->Set4Momentum(gLv);
      newp->push_back(gamma);
    }
  }
  return newp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToHadronsModel::ComputeCMCrossSectionPerElectron()
{
  G4bool b;   
  for(G4int i=0; i<nbins; i++) {
    G4double e  = crossPerElectron->GetLowEdgeEnergy(i);
    G4double cs = 0.0;
    if(i > 0) {
      G4double L   = 2.0*log(e/electron_mass_c2);
      G4double bt  = 2.0*fine_structure_const*(L - 1.0)/pi;
      G4double btm1= bt - 1.0;
      G4double del = 1. + fine_structure_const*(1.5*L + pi*pi/3. -2.)/pi;
      G4double s1  = crossBornPerElectron->GetValue(e, b);
      G4double e1  = crossPerElectron->GetLowEdgeEnergy(i-1);
      G4double x1  = 1. - e1/e;
      cs += s1*(del*pow(x1,bt) - bt*(x1 - 0.25*x1*x1));
      if(i > 1) {
	G4double e2  = e1;
	G4double x2  = x1;
	G4double s2  = crossBornPerElectron->GetValue(e2, b);
	G4double w2  = bt*(del*pow(x2,btm1) - 1.0 + 0.5*x2);
     
	for(G4int j=i-2; j>=0; j--) {
	  e1  = crossPerElectron->GetLowEdgeEnergy(j);
	  x1  = 1. - e1/e;
	  G4double s1 = crossBornPerElectron->GetValue(e1, b);
	  G4double w1 = bt*(del*pow(x1,btm1) - 1.0 + 0.5*x1);
	  cs += 0.5*(x1 - x2)*(w2*s2 + w1*s1);
	  e2 = e1;
	  x2 = x1;
	  s2 = s1;
	  w2 = w1;
	}
      }
    }
    crossPerElectron->PutValue(i, cs);
    //    G4cout << "e= " << e << "  cs= " << cs << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4DynamicParticle* G4eeToHadronsModel::GenerateCMPhoton(G4double e)
{
  G4bool b;   
  G4double x;
  G4DynamicParticle* gamma = 0;
  G4double L   = 2.0*log(e/electron_mass_c2);
  G4double bt  = 2.0*fine_structure_const*(L - 1.)/pi;
  G4double btm1= bt - 1.0;
  G4double del = 1. + fine_structure_const*(1.5*L + pi*pi/3. -2.)/pi;

  G4double s0 = crossBornPerElectron->GetValue(e, b);
  G4double de = (emax - emin)/(G4double)nbins;
  G4double x0 = std::min(de,e - emin)/e;
  G4double ds = crossBornPerElectron->GetValue(e, b)
              *(del*pow(x0,bt) - bt*(x0 - 0.25*x0*x0));
  G4double e1 = e*(1. - x0);

  if(e1 < emax && s0*G4UniformRand()<ds) { 
    x = x0*pow(G4UniformRand(),1./bt);
  } else {    

    x  = 1. - e1/e;
    G4double s1 = crossBornPerElectron->GetValue(e1, b);
    G4double w1 = bt*(del*pow(x,btm1) - 1.0 + 0.5*x);
    G4double grej = s1*w1;
    G4double f;
    //    G4cout << "e= " << e/GeV << " epeak= " << epeak/GeV 
    //       << " s1= " << s1 << " w1= " << w1 
    //       << " grej= " << grej << G4endl;
    // Above emax cross section is 0
    if(e1 > emax) {
      x  = 1. - emax/e;
      G4double s2 = crossBornPerElectron->GetValue(emax, b);
      G4double w2 = bt*(del*pow(x,btm1) - 1.0 + 0.5*x);
      grej = s2*w2;
      //  G4cout << "emax= " << emax << " s2= " << s2 << " w2= " << w2 
      //   << " grej= " << grej << G4endl;
    }

    if(e1 > epeak) {
      x  = 1. - epeak/e;
      G4double s2 = crossBornPerElectron->GetValue(epeak, b);
      G4double w2 = bt*(del*pow(x,btm1) - 1.0 + 0.5*x);
      grej = std::max(grej,s2*w2);
      //G4cout << "epeak= " << epeak << " s2= " << s2 << " w2= " << w2 
      //     << " grej= " << grej << G4endl;
    }
    G4double xmin = 1. - e1/e;
    if(e1 > emax) xmin = 1. - emax/e;
    G4double xmax = 1. - emin/e;
    do {
      x = xmin + G4UniformRand()*(xmax - xmin);
      G4double s2 = crossBornPerElectron->GetValue((1.0 - x)*e, b);
      G4double w2 = bt*(del*pow(x,btm1) - 1.0 + 0.5*x);
      //G4cout << "x= " << x << " xmin= " << xmin << " xmax= " << xmax
      //     << " s2= " << s2 << " w2= " << w2 
      //	   << G4endl;
      f = s2*w2;
      if(f > grej) {
	G4cout << "G4DynamicParticle* G4eeToHadronsModel:WARNING "
	       << f << " > " << grej << " majorant is`small!" 
	       << G4endl; 
      }
    } while (f < grej*G4UniformRand());
  }

  G4ThreeVector dir(0.0,0.0,1.0);
  gamma = new G4DynamicParticle(theGamma,dir,x*e);
  return gamma;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


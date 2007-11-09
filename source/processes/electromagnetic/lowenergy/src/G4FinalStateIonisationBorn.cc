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
// $Id: G4FinalStateIonisationBorn.cc,v 1.3 2007-11-09 16:38:13 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// Contact Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// Reference: TNS Geant4-DNA paper
// Reference for implementation model: NIM. 155, pp. 145-156, 1978

// History:
// -----------
// Date         Name              Modification
// 28 Apr 2007  M.G. Pia          Created in compliance with design described in TNS paper
//
// -------------------------------------------------------------------

// Class description:
// Reference: TNS Geant4-DNA paper
// S. Chauvie et al., Geant4 physics processes for microdosimetry simulation:
// design foundation and implementation of the first set of models,
// IEEE Trans. Nucl. Sci., vol. 54, no. 6, Dec. 2007.
// Further documentation available from http://www.ge.infn.it/geant4/dna

// -------------------------------------------------------------------


#include "G4FinalStateIonisationBorn.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4DynamicParticle.hh"
#include "Randomize.hh"

#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleMomentum.hh"


G4FinalStateIonisationBorn::G4FinalStateIonisationBorn()
{
  // NEW
  // Factor to scale microscopic/macroscopic cross section data in water
 
  G4double scaleFactor = (1.e-22 / 3.343) * m*m;

  // Energy limits
  G4ParticleDefinition* electronDef = G4Electron::ElectronDefinition();
  G4ParticleDefinition* protonDef = G4Proton::ProtonDefinition();

  G4String electron;
  G4String proton;

  // Default energy limits (defined for protection against anomalous behaviour only)
  lowEnergyLimitDefault = 25 * eV;
  highEnergyLimitDefault = 10 * MeV;

  char *path = getenv("G4LEDATA");
 
  if (!path)
    G4Exception("G4DNACrossSectionDataSet::FullFileName - G4LEDATA environment variable not set");

  // Data members for electrons

  if (electronDef != 0)
    {
      electron = electronDef->GetParticleName();
      lowEnergyLimit[electron] = 25. * eV;
      highEnergyLimit[electron] = 30. * keV;

      std::ostringstream eFullFileName;
      eFullFileName << path << "/dna/sigmadiff_ionisation_e_born.dat";
      std::ifstream eDiffCrossSection(eFullFileName.str().c_str());
      // eDiffCrossSection(eFullFileName.str().c_str());
      if(!eDiffCrossSection)
	{ 
	  G4cout<<"ERROR OPENING DATA FILE IN ELECTRON BORN IONIZATION !!! "<<G4endl;
	  while(1);
	}
      
      eTdummyVec.push_back(0.);
      while(!eDiffCrossSection.eof())
	{
	  double tDummy;
	  double eDummy;
	  eDiffCrossSection>>tDummy>>eDummy;
	  if(tDummy != eTdummyVec.back()) eTdummyVec.push_back(tDummy);
	  for (int j=0; j<5; j++)
	    {
	      eDiffCrossSection>>eDiffCrossSectionData[j][tDummy][eDummy];
	      eDiffCrossSectionData[j][tDummy][eDummy]*=scaleFactor;
	      eVecm[tDummy].push_back(eDummy);
	    }
	}

    }
  else
    {
      G4Exception("G4FinalStateIonisationBorn Constructor: electron is not defined");
    }

  // Data members for protons

  if (protonDef != 0)
    {
      proton = protonDef->GetParticleName();
      lowEnergyLimit[proton] = 500. * keV;
      highEnergyLimit[proton] = 10. * MeV;

      std::ostringstream pFullFileName;
      pFullFileName << path << "/dna/sigmadiff_ionisation_p_born.dat";
      std::ifstream pDiffCrossSection(pFullFileName.str().c_str());
      //     pDiffCrossSection(pFullFileName.str().c_str());
      if (!pDiffCrossSection)
	{ 
	  G4cout<<"ERROR OPENING DATA FILE IN PROTON BORN IONIZATION !!! "<<G4endl;
	  while(1);
	}
      
      pTdummyVec.push_back(0.);
      while(!pDiffCrossSection.eof())
	{
	  double tDummy;
	  double eDummy;
	  pDiffCrossSection>>tDummy>>eDummy;
	  if(tDummy != pTdummyVec.back()) pTdummyVec.push_back(tDummy);
	  for (int j=0; j<5; j++)
	    {
	      pDiffCrossSection>>pDiffCrossSectionData[j][tDummy][eDummy];
	      pDiffCrossSectionData[j][tDummy][eDummy]*=scaleFactor;
	      //G4cout << "j=" << j << " Tdum=" << tDummy << " Edum=" << eDummy << " pDiff=" << pDiffCrossSectionData[j][tDummy][eDummy] << G4endl;
	      pVecm[tDummy].push_back(eDummy);
	    }
	}

    }
  else
    {
      G4Exception("G4FinalStateIonisationBorn Constructor: proton is not defined");
    }

}


G4FinalStateIonisationBorn::~G4FinalStateIonisationBorn()
{
  eVecm.clear();
  pVecm.clear();
}


const G4FinalStateProduct& G4FinalStateIonisationBorn::GenerateFinalState(const G4Track& track, const G4Step& /* step */)
{
  // Clear previous secondaries, energy deposit and particle kill status
  product.Clear();

  const G4DynamicParticle* particle = track.GetDynamicParticle();

  G4double lowLim = lowEnergyLimitDefault;
  G4double highLim = highEnergyLimitDefault;

  G4double k = particle->GetKineticEnergy();

  const G4String& particleName = particle->GetDefinition()->GetParticleName();

  // Retrieve energy limits for the current particle type

  std::map< G4String,G4double,std::less<G4String> >::iterator pos1;
  pos1 = lowEnergyLimit.find(particleName);

  // Lower limit
  if (pos1 != lowEnergyLimit.end())
    {
      lowLim = pos1->second;
    }

  // Upper limit
  std::map< G4String,G4double,std::less<G4String> >::iterator pos2;
  pos2 = highEnergyLimit.find(particleName);

  if (pos2 != highEnergyLimit.end())
    {
      highLim = pos2->second;
    }

  // Verify that the current track is within the energy limits of validity of the cross section model

  if (k >= lowLim && k <= highLim)
    {
      // Kinetic energy of primary particle

      G4ParticleMomentum primaryDirection = particle->GetMomentumDirection();
      G4double particleMass = particle->GetDefinition()->GetPDGMass();
      G4double totalEnergy = k + particleMass;
      G4double pSquare = k * (totalEnergy + particleMass);
      G4double totalMomentum = std::sqrt(pSquare);

      const G4String& particleName = particle->GetDefinition()->GetParticleName();
  
      G4int ionizationShell = cross.RandomSelect(k,particleName);
  
      G4double secondaryKinetic = RandomizeEjectedElectronEnergy(particle->GetDefinition(),k,ionizationShell);
  
      G4double bindingEnergy = waterStructure.IonisationEnergy(ionizationShell);

      G4double cosTheta = 0.;
      G4double phi = 0.; 
      RandomizeEjectedElectronDirection(track.GetDefinition(), k,secondaryKinetic, cosTheta, phi);

      G4double sinTheta = std::sqrt(1.-cosTheta*cosTheta);
      G4double dirX = sinTheta*cos(phi);
      G4double dirY = sinTheta*sin(phi);
      G4double dirZ = cosTheta;
      G4ThreeVector deltaDirection(dirX,dirY,dirZ);
      deltaDirection.rotateUz(primaryDirection);

      G4double deltaTotalMomentum = std::sqrt(secondaryKinetic*(secondaryKinetic +
							   2.*electron_mass_c2 ));

      //Primary Particle Direction
      G4double finalPx = totalMomentum*primaryDirection.x()
	- deltaTotalMomentum*deltaDirection.x();
      G4double finalPy = totalMomentum*primaryDirection.y()
	- deltaTotalMomentum*deltaDirection.y();
      G4double finalPz = totalMomentum*primaryDirection.z()
	- deltaTotalMomentum*deltaDirection.z();
      G4double finalMomentum =
	std::sqrt(finalPx*finalPx+finalPy*finalPy+finalPz*finalPz);
      finalPx /= finalMomentum;
      finalPy /= finalMomentum;
      finalPz /= finalMomentum;

      product.ModifyPrimaryParticle(finalPx,finalPy,finalPz,k-bindingEnergy-secondaryKinetic);
      product.AddEnergyDeposit(bindingEnergy);

      G4DynamicParticle* aElectron = new G4DynamicParticle(G4Electron::Electron(),deltaDirection,secondaryKinetic);
      product.AddSecondary(aElectron);
    }

  return product;
}


G4double G4FinalStateIonisationBorn::RandomizeEjectedElectronEnergy(G4ParticleDefinition* particleDefinition, 
								    G4double k, 
								    G4int shell)
{

  if (particleDefinition == G4Electron::ElectronDefinition()) 
    {

      G4double maximumEnergyTransfer=0.;
      if ((k+waterStructure.IonisationEnergy(shell))/2. > k) maximumEnergyTransfer=k;
      else maximumEnergyTransfer = (k+waterStructure.IonisationEnergy(shell))/2.;
    
      G4double crossSectionMaximum = 0.;
      for(G4double value=waterStructure.IonisationEnergy(shell); value<=maximumEnergyTransfer; value+=0.1*eV)
	{
	  G4double differentialCrossSection = DifferentialCrossSection(particleDefinition, k/eV, value/eV, shell);
	  if(differentialCrossSection >= crossSectionMaximum) crossSectionMaximum = differentialCrossSection;
	}
 
      G4double secondaryElectronKineticEnergy=0.;
      do 
	{
	  secondaryElectronKineticEnergy = G4UniformRand() * (maximumEnergyTransfer-waterStructure.IonisationEnergy(shell));
	} while(G4UniformRand()*crossSectionMaximum >
	      DifferentialCrossSection(particleDefinition, k/eV,(secondaryElectronKineticEnergy+waterStructure.IonisationEnergy(shell))/eV,shell));

      return(secondaryElectronKineticEnergy);
 
    }
  
  if (particleDefinition == G4Proton::ProtonDefinition()) 
    {
      G4double maximumKineticEnergyTransfer = 4.* (electron_mass_c2 / proton_mass_c2) * k - (waterStructure.IonisationEnergy(shell));

      G4double crossSectionMaximum = 0.;
      for (G4double value = waterStructure.IonisationEnergy(shell); 
	   value<=4.*waterStructure.IonisationEnergy(shell) ; 
	   value+=0.1*eV)
	{
	  G4double differentialCrossSection = DifferentialCrossSection(particleDefinition, k/eV, value/eV, shell);
	  if (differentialCrossSection >= crossSectionMaximum) crossSectionMaximum = differentialCrossSection;
	}

      G4double secondaryElectronKineticEnergy = 0.;
      do
	{
	  secondaryElectronKineticEnergy = G4UniformRand() * maximumKineticEnergyTransfer;
	} while(G4UniformRand()*crossSectionMaximum >= 
	      DifferentialCrossSection(particleDefinition, k/eV,(secondaryElectronKineticEnergy+waterStructure.IonisationEnergy(shell))/eV,shell));

      return(secondaryElectronKineticEnergy);
    }

  return 0;
}


void G4FinalStateIonisationBorn::RandomizeEjectedElectronDirection(G4ParticleDefinition* particleDefinition, 
								   G4double k, 
								   G4double secKinetic, 
								   G4double & cosTheta, 
								   G4double & phi )
{
  if (particleDefinition == G4Electron::ElectronDefinition()) 
    {

      phi = twopi * G4UniformRand();
      if(secKinetic < 50.*eV) cosTheta = (2.*G4UniformRand())-1.;
      else if(secKinetic <= 200.*eV) 	
	{
	  if(G4UniformRand() <= 0.1) cosTheta = (2.*G4UniformRand())-1.;
	  else cosTheta = G4UniformRand()*(std::sqrt(2)/2);
	}
      else	
	{
	  G4double sin2O = (1.-secKinetic/k) / (1.+secKinetic/(2.*electron_mass_c2));
	  cosTheta = std::sqrt(1-sin2O);
	}
    }
 
  if (particleDefinition == G4Proton::ProtonDefinition()) 
    {
      G4double maxSecKinetic = 4.* (electron_mass_c2 / proton_mass_c2) * k;
      phi = twopi * G4UniformRand();
      cosTheta = std::sqrt(secKinetic / maxSecKinetic);
    }

				
}


double G4FinalStateIonisationBorn::DifferentialCrossSection(G4ParticleDefinition * particleDefinition, 
							    G4double k, 
							    G4double energyTransfer, 
							    G4int ionizationLevelIndex)
{
  G4double value_t1=0;
  G4double value_t2=0;
  G4double value_e21=0;
  G4double value_e22=0;
  G4double value_e12=0;
  G4double value_e11=0;

  if (particleDefinition == G4Electron::ElectronDefinition()) 
    {
      // k should be in eV and energy transfer eV also
      std::vector<double>::iterator t2 = std::upper_bound(eTdummyVec.begin(),eTdummyVec.end(), k);
      std::vector<double>::iterator t1 = t2-1;
      std::vector<double>::iterator e12 = std::upper_bound(eVecm[(*t1)].begin(),eVecm[(*t1)].end(), energyTransfer);
      std::vector<double>::iterator e11 = e12-1;

      std::vector<double>::iterator e22 = std::upper_bound(eVecm[(*t2)].begin(),eVecm[(*t2)].end(), energyTransfer);
      std::vector<double>::iterator e21 = e22-1;

      value_t1  = *t1;
      value_t2  = *t2;
      value_e21 = *e21;
      value_e22 = *e22;
      value_e12 = *e12;
      value_e11 = *e11;
    }
  
  if (particleDefinition == G4Proton::ProtonDefinition()) 
    {
      // k should be in eV and energy transfer eV also
      std::vector<double>::iterator t2 = std::upper_bound(pTdummyVec.begin(),pTdummyVec.end(), k);
      std::vector<double>::iterator t1 = t2-1;
      std::vector<double>::iterator e12 = std::upper_bound(pVecm[(*t1)].begin(),pVecm[(*t1)].end(), energyTransfer);
      std::vector<double>::iterator e11 = e12-1;

      std::vector<double>::iterator e22 = std::upper_bound(pVecm[(*t2)].begin(),pVecm[(*t2)].end(), energyTransfer);
      std::vector<double>::iterator e21 = e22-1;
 
      value_t1  = *t1;
      value_t2  = *t2;
      value_e21 = *e21;
      value_e22 = *e22;
      value_e12 = *e12;
      value_e11 = *e11;
    }
  
  G4double xs11=0;   
  G4double xs12=0; 
  G4double xs21=0; 
  G4double xs22=0; 
  
  if (particleDefinition == G4Electron::ElectronDefinition()) 
    {
      xs11 = eDiffCrossSectionData[ionizationLevelIndex][value_t1][value_e11];
      xs12 = eDiffCrossSectionData[ionizationLevelIndex][value_t1][value_e12];
      xs21 = eDiffCrossSectionData[ionizationLevelIndex][value_t2][value_e21];
      xs22 = eDiffCrossSectionData[ionizationLevelIndex][value_t2][value_e22];
    }

  if (particleDefinition == G4Proton::ProtonDefinition()) 
    {
      xs11 = pDiffCrossSectionData[ionizationLevelIndex][value_t1][value_e11];
      xs12 = pDiffCrossSectionData[ionizationLevelIndex][value_t1][value_e12];
      xs21 = pDiffCrossSectionData[ionizationLevelIndex][value_t2][value_e21];
      xs22 = pDiffCrossSectionData[ionizationLevelIndex][value_t2][value_e22];
    }

  if (xs11==0 || xs12==0 ||xs21==0 ||xs22==0) return (0.);
 
  G4double sigma = QuadInterpolator(value_e11, value_e12, 
				    value_e21, value_e22, 
				    xs11, xs12, 
				    xs21, xs22, 
				    value_t1, value_t2, 
				    k, energyTransfer);

  if(energyTransfer < waterStructure.IonisationEnergy(ionizationLevelIndex)) return(0.);

  return(sigma);
}


G4double G4FinalStateIonisationBorn::LogLogInterpolate(G4double e1, 
						       G4double e2, 
						       G4double e, 
						       G4double xs1, 
						       G4double xs2)
{
  G4double a = (log10(xs2)-log10(xs1)) / (log10(e2)-log10(e1));
  G4double b = log10(xs2) - a*log10(e2);
  G4double sigma = a*log10(e) + b;
  return (pow(10,sigma));
}


G4double G4FinalStateIonisationBorn::QuadInterpolator(G4double e11, G4double e12, 
						      G4double e21, G4double e22, 
						      G4double xs11, G4double xs12, 
						      G4double xs21, G4double xs22, 
						      G4double t1, G4double t2, 
						      G4double t, G4double e)
{
  G4double interpolatedvalue1 = LogLogInterpolate(e11, e12, e, xs11, xs12);
  G4double interpolatedvalue2 = LogLogInterpolate(e21, e22, e, xs21, xs22);
  G4double value = LogLogInterpolate(t1, t2, t, interpolatedvalue1, interpolatedvalue2);
  return (value);
}



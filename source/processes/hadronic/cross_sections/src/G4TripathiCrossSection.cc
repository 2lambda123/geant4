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
// Implementation of formulas in analogy to NASA technical paper 3621 by Tripathi, et al.

#include "G4TripathiCrossSection.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"

G4double G4TripathiCrossSection::
GetCrossSection(const G4DynamicParticle* aPart, 
                const G4Element*anEle, G4double )
{
  G4double result = 0;
  
  const G4double targetAtomicNumber = anEle->GetN();
  const G4double nTargetProtons = anEle->GetZ();
  
  const G4double kineticEnergy = aPart->GetKineticEnergy()/MeV;
  const G4double nProjProtons = aPart->GetDefinition()->GetPDGCharge();
  const G4double projectileAtomicNumber = aPart->GetDefinition()->GetBaryonNumber();

  const G4double nuleonRadius=1.1E-15;
  const G4double myNuleonRadius=1.36E-15;
  
  // needs target mass
  G4double targetMass = G4ParticleTable::GetParticleTable()
                                       ->GetIonTable()
				       ->GetIonMass(lrint(nTargetProtons), lrint(targetAtomicNumber));
  G4LorentzVector pTarget(0,0,0,targetMass); 
  G4LorentzVector pProjectile(aPart->Get4Momentum());
  pTarget = pTarget+pProjectile;
  G4double E_cm = (pTarget.mag()-targetMass-pProjectile.m())/MeV;
  
  // done
  G4double r_rms_p = 0.6 * myNuleonRadius * pow(projectileAtomicNumber, 1./3.);
  G4double r_rms_t = 0.6 * myNuleonRadius * pow(targetAtomicNumber, 1./3.);
  
  // done
  G4double r_p = 1.29*r_rms_p/nuleonRadius ;
  G4double r_t = 1.29*r_rms_t/nuleonRadius;
  
  // done
  G4double Radius = r_p + r_t + 
           1.2*(pow(targetAtomicNumber, 1./3.) + pow(projectileAtomicNumber, 1./3.))/
	   pow(E_cm, 1./3.);
  
  //done
  G4double B = 1.44*nProjProtons*nTargetProtons/Radius;
  
  // done
  G4double Energy = kineticEnergy/projectileAtomicNumber;

  // done
  G4double D = 2.77-(8.0E-3*targetAtomicNumber)+(1.8E-5*targetAtomicNumber*targetAtomicNumber)
                   - 0.8/(1+exp((250.-Energy)/75.));
  D = 1.75;
  // done
  G4double C_E = D * (1-exp(-Energy/40.)) - 0.292*exp(-Energy/792.)*cos(0.229*pow(Energy, 0.453));
  
  // done
  G4double S = pow(projectileAtomicNumber, 1./3.)*pow(targetAtomicNumber, 1./3.)/
               (pow(projectileAtomicNumber, 1./3.) + pow(targetAtomicNumber, 1./3.)); 
  
  // done
  G4double deltaE = 1.85*S + 0.16*S/pow(E_cm,1./3.) - C_E +
                    0.91*(targetAtomicNumber-2.*nTargetProtons)*nProjProtons/
		    (targetAtomicNumber*projectileAtomicNumber);
  
  // done 
  result = pi * nuleonRadius*nuleonRadius * 
           pow(( pow(targetAtomicNumber, 1./3.) + 
	         pow(projectileAtomicNumber, 1./3.) + deltaE),2.) * (1-B/E_cm);
  
  if(result < 0) result = 0;
  return result*m2;
  
}

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
// $Id: G4QuasiElasticChannel.cc,v 1.1 2007-03-30 15:25:54 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

// Author : Gunter Folger March 2007
// Class Description
// Final state production model for theoretical models of hadron inelastic
// quasi elastic scattering in geant4;
// Class Description - End
//
// Modified:

#include "G4QuasiElasticChannel.hh"

#include "G4HadTmpUtil.hh"		//lrint

G4QuasiElasticChannel::G4QuasiElasticChannel()
{
	theQuasiElastic=G4QuasiFreeRatios::GetPointer();
}

G4QuasiElasticChannel::~G4QuasiElasticChannel()
{}

G4double G4QuasiElasticChannel::GetFraction(G4Nucleus &theNucleus,
				const G4DynamicParticle & thePrimary)
{
	std::pair<G4double,G4double> ratios;
	ratios=theQuasiElastic->GetRatios(thePrimary.GetTotalMomentum(),
			thePrimary.GetDefinition()->GetPDGEncoding(),
			G4lrint(theNucleus.GetZ()),
			G4lrint(theNucleus.GetN()-theNucleus.GetZ()));
#ifdef debug_getFraction			
	G4cout << "G4QuasiElasticChannel::ratios " << ratios.first << " x " <<ratios.second
	       << "  = " << ratios.first*ratios.second << G4endl;
#endif
	       
	return ratios.first*ratios.second;       		
}

G4KineticTrackVector * G4QuasiElasticChannel::Scatter(G4Nucleus &theNucleus,
					const G4DynamicParticle & thePrimary)
{
	
	
	G4int A=G4lrint(theNucleus.GetN());
	G4int Z=G4lrint(theNucleus.GetZ());
	G4double an=G4UniformRand()*A;
	G4ParticleDefinition * pDef;
	if ( an < Z )
	{
	   pDef=G4Proton::Proton();
	} else 
	{
	   pDef=G4Neutron::Neutron(); 
	}          
	
#ifdef debug_scatter
	G4cout << " neutron - proton? A, Z, an, pdg" <<" "<< A <<" "<<Z 
			<< " "<<an <<" " << pDef->GetParticleName()<< G4endl;
#endif
	G4LorentzVector pNucleon(G4ThreeVector(0,0,0),pDef->GetPDGMass());
	
	std::pair<G4LorentzVector,G4LorentzVector> result;
	
	result=theQuasiElastic->Scatter(pDef->GetPDGEncoding(),pNucleon,
	                       thePrimary.GetDefinition()->GetPDGEncoding(),
			        thePrimary.Get4Momentum());
				
	if (result.first.e() == 0.)
	{
	   G4cout << "Warning - G4QuasiElasticChannel::Scatter no scattering" << G4endl;
	   return 0;       //no scatter
	}
	G4KineticTrackVector * ktv;
	ktv=new G4KineticTrackVector();
	G4KineticTrack * sPrim=new G4KineticTrack(thePrimary.GetDefinition(),
			0.,G4ThreeVector(0), result.second);
	ktv->push_back(sPrim);
	G4KineticTrack * sNuc=new G4KineticTrack(pDef,
			0.,G4ThreeVector(0), result.first);
	ktv->push_back(sNuc);
	
#ifdef debug_scatter
	G4cout << " scattered Nucleon : " << result.first << G4endl;
	G4cout << " scattered Project : " << result.second << G4endl;
#endif
	return ktv;			       
}

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
// $Id: HadrontherapyPhysicsList.cc,v 1.0
// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the National Institute for Nuclear Physics, Catania, Italy
// (b) National Institute for Nuclear Physics Section of Genova, genova, Italy
// 
// * cirrone@lns.infn.it
// ----------------------------------------------------------------------------

#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "HadrontherapyPhysicsList.hh"
#include "HadrontherapyPhysicsListMessenger.hh"
#include "HadrontherapyParticles.hh"
#include "HadrontherapyPhotonStandard.hh"
#include "HadrontherapyPhotonEPDL.hh"
#include "HadrontherapyPhotonPenelope.hh"
#include "HadrontherapyElectronStandard.hh"
#include "HadrontherapyElectronEEDL.hh"
#include "HadrontherapyElectronPenelope.hh"
#include "HadrontherapyPositronStandard.hh"
#include "HadrontherapyPositronPenelope.hh"
#include "HadrontherapyIonLowE.hh"
#include "HadrontherapyIonLowEZiegler1977.hh"
#include "HadrontherapyIonLowEZiegler1985.hh"
#include "HadrontherapyIonLowEZiegler2000.hh"
#include "HadrontherapyIonStandard.hh"
#include "HadrontherapyProtonPrecompound.hh"
#include "HadrontherapyProtonBertini.hh"
#include "HadrontherapyProtonBinary.hh"
#include "HadrontherapyProtonLEP.hh"
#include "HadrontherapyMuonStandard.hh"
#include "HadrontherapyDecay.hh"
#include "HadrontherapyParticles.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "HadrontherapyHadronLElastic.hh"
#include "HadrontherapyHadronBertiniElastic.hh"
#include "HadrontherapyHadronUHadronElastic.hh"
#include "HadrontherapyHadronHadronElastic.hh"
#include "HadrontherapyHadronCHIPSElastic.hh"
#include "HadrontherapyProtonPrecompoundGEM.hh"
#include "HadrontherapyProtonPrecompoundFermi.hh"
#include "HadrontherapyProtonPrecompoundGEMFermi.hh"

HadrontherapyPhysicsList::HadrontherapyPhysicsList(): G4VModularPhysicsList(),
						      electronIsRegistered(false), 
						      positronIsRegistered(false), 
						      photonIsRegistered(false), 
						      ionIsRegistered(false),
						      protonHadronicIsRegistered(false),
						      muonIsRegistered(false),
						      decayIsRegistered(false),
						      hadronicElasticScatteringIsRegistered(false)
{
  // The secondary production threshold is set to 10. mm
  // for all the particles in all the experimental set-up
  // The phantom is defined as a Geant4 Region. Here the cut is fixed to 0.001 mm
  defaultCutValue = 0.01 * mm;

  // Messenger: it is possible to activate physics processes and models interactively 
  messenger = new HadrontherapyPhysicsListMessenger(this);

  SetVerboseLevel(1);

  // Register all the particles involved in the experimental set-up
  RegisterPhysics( new HadrontherapyParticles("particles") );
}

HadrontherapyPhysicsList::~HadrontherapyPhysicsList()
{ 
  delete messenger;
}

void HadrontherapyPhysicsList::AddPhysicsList(const G4String& name)
{
  G4cout << "Adding PhysicsList component " << name << G4endl;
  
  //*************************//
  // Electromagnetic physics //
  //*************************//
  //
  // The user can choose three alternative approaches for electrons and photons:
  // Standard, Low Energy based on the Livermore libraries and Low Energy Penelope
  //

  // ******** PHOTONS ********//
  
  // Register standard processes for photons

  if (name == "photon-standard") 
    {
      if (photonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- photon List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPhotonStandard(name) );
	  photonIsRegistered = true;
	}
    }

  // Register LowE-EPDL processes for photons
  if (name == "photon-epdl") 
    {
      if (photonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- photon List already existing"
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPhotonEPDL(name) );
	  photonIsRegistered = true;
	}
    } 

  // Register processes a' la Penelope for photons
  if (name == "photon-penelope")
    {
      if (photonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- photon List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPhotonPenelope(name) );
	  photonIsRegistered = true;
	}
    }

   // ******** Electrons ********//

  // Register standard processes for electrons
  if (name == "electron-standard") 
    {
      if (electronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- electron List already existing" 
		 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyElectronStandard(name) );	  
	  electronIsRegistered = true;
	}
    }

  // Register LowE-EEDL processes for electrons
  if (name == "electron-eedl") 
    {
      if (electronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- electron List already existing"                  
		 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyElectronEEDL(name) );
	  electronIsRegistered = true;
	}
    } 

  // Register processes a' la Penelope for electrons
  if (name == "electron-penelope")
    {
      if (electronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
		 << " cannot be registered ---- electron List already existing"                  
		 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyElectronPenelope(name) );
	  electronIsRegistered = true;
	}
    }

  // ******** Positrons ********//
  // Register standard processes for positrons
  if (name == "positron-standard") 
    {
      if (positronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- positron List already existing"                  
		 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPositronStandard(name) );
	  positronIsRegistered = true;
	}
    }
  // Register penelope processes for positrons
  if (name == "positron-penelope") 
    {
      if (positronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- positron List already existing"   << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPositronPenelope(name) );
	  positronIsRegistered = true;
	}
    }
 
  // ******** HADRONS AND IONS ********//
  // Register Low Energy  processes for protons and ions
  // Stopping power parameterisation: ICRU49 (default model)
  
  if (name == "ion-LowE") 
    {
      if (ionIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- proton List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyIonLowE(name) );
	  ionIsRegistered = true;
	}
    }

// Register Low Energy processes for protons and ions
// Stopping power parameterisation: Ziegler 1977
 if (name == "ion-LowE-ziegler1977") 
    {
      if (ionIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- proton List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyIonLowEZiegler1977(name) );
	  ionIsRegistered = true;
	}
    }


// Register Low Energy processes for protons and ions
// Stopping power parameterisation: Ziegler 1985
 if (name == "ion-LowE-ziegler1985") 
    {
      if (ionIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- proton List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyIonLowEZiegler1985(name) );
	  ionIsRegistered = true;
	}
    }


// Register Low Energy processes for protons and ions
// Stopping power parameterisation: SRIM2000
 if (name == "ion-LowE-ziegler2000") 
    {
      if (ionIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- proton List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyIonLowEZiegler2000(name) );
	  ionIsRegistered = true;
	}
    }

  // Register Standard processes for protons and ions

  if (name == "ion-standard") 
    {
      if (ionIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- ion List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyIonStandard(name) );
	  ionIsRegistered = true;
	}
    }

  // Register the Standard processes for muons
  if (name == "muon-standard") 
    {
      if (muonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyMuonStandard(name) );
	  muonIsRegistered = true;
	}
    }

  // Register the decay process
  if (name == "decay") 
    {
      if (decayIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyDecay(name) );
	  decayIsRegistered = true;
	}
    }


  // ------------------------------------------------------------------------------------
  //------------------------------
  // HADRONIC ELASTIC MODELS
  // -----------------------------

  // ------------------------------------------------------
  // GROUP I: PROCESS: G4HadronElasticProcess


  // Activate the LElastic model with G4HadronElasticProcess

  if (name == "hadron-LElastic") 
    {
      if (hadronicElasticScatteringIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- hadronic Elastic Scattering List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyHadronLElastic(name) );
	  hadronicElasticScatteringIsRegistered = true;
	}
    }
  

// Activate the Bertini Elastic model with G4HadronElasticProcess

 if (name == "hadron-bertiniElastic") 
    {
      if (hadronicElasticScatteringIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- hadronic Elastic Scattering List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyHadronBertiniElastic(name) );
	  hadronicElasticScatteringIsRegistered = true;
	}
    }


// Activate the elastic G4HadronElastic model with G4HadronElasticProcess 

   if (name == "hadron-hadronElastic") 
    {
      if (hadronicElasticScatteringIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- hadronic Elastic Scattering List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyHadronHadronElastic(name) );
	  hadronicElasticScatteringIsRegistered = true;
	}
    }

   // -----------------------------------------------
   // GROUP II: PROCESS: G4UHadronElasticProcess


   // Activate the LElastic model with G4UHadronElasticProcess 
   
   
   // Activate the elastic G4HadronElastic model with G4UHadronElasticProces

   if (name == "hadron-UHadronElastic") 
     {
       if (hadronicElasticScatteringIsRegistered) 
	 {
	   G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		  << " cannot be registered ---- hadronic Elastic Scattering List already existing" 
		  << G4endl;
	 } 
       else 
	 {
	   G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
		  << " is registered" << G4endl;
	   RegisterPhysics( new HadrontherapyHadronUHadronElastic(name) );
	   hadronicElasticScatteringIsRegistered = true;
	 }
     }
   

   // ------------------------------------------------
   // GROUP #3 Process: CHIPS Elastic

   // Activate the elastic CHIPS elastic scattering

   if (name == "hadron-CHIPSElastic") 
     {
       if (hadronicElasticScatteringIsRegistered) 
	 {
	   G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		  << " cannot be registered ---- hadronic Elastic Scattering List already existing" 
		  << G4endl;
	 } 
       else 
	 {
	   G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	   RegisterPhysics( new HadrontherapyHadronCHIPSElastic(name) );
	   hadronicElasticScatteringIsRegistered = true;
	 }
     }
   
   


   // ********************
   // INELASTIC PROCESS
   // ********************

   //--------------------------------------------------------------------------------------------
   // HADRONIC PRECOMPUND + DEFAULT EVAPORATION
   //--------------------------------------------------------------------------------------------
   //
   // Register the hadronic physics for protons, neutrons, ions
   // 
  
   //Precompound Default Evaporation

   if (name == "proton-precompound") 
    {
      if (protonHadronicIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonPrecompound(name) );
	  protonHadronicIsRegistered = true;
	}
    }
   
   //--------------------------------------------------------------------------------------------
   // HADRONIC PRECOMPOUND + GEM EVAPORATION 
   //--------------------------------------------------------------------------------------------
   //
   // Register the hadronic physics for protons, neutrons, ions
   // 
  

   if (name == "proton-precompoundGEM") 
    {
      if (protonHadronicIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonPrecompoundGEM(name) );
	  protonHadronicIsRegistered = true;
	}
    }

   //--------------------------------------------------------------------------------------------
   // HADRONIC PRECOMPOUND +  DEFAULT EVAPORATION + FERMI BREAK-UP
   //--------------------------------------------------------------------------------------------
   //
   // Register the hadronic physics for protons, neutrons, ions
   // 
  

   if (name == "proton-precompoundFermi") 
    {
      if (protonHadronicIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonPrecompoundFermi(name) );
	  protonHadronicIsRegistered = true;
	}
    }



   //--------------------------------------------------------------------------------------------
   // HADRONIC PRECOMPOUND +  GEM EVAPORATION + FERMI BREAK-UP
   //--------------------------------------------------------------------------------------------
   //
   // Register the hadronic physics for protons, neutrons, ions
   // 
  

   if (name == "proton-precompoundGEMFermi") 
    {
      if (protonHadronicIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonPrecompoundGEMFermi(name) );
	  protonHadronicIsRegistered = true;
	}
    }



//--------------------------------------------------------------------------------------------
// Begin Hadronic Bertini model
//--------------------------------------------------------------------------------------------

//  Bertini model for protons, pions and neutrons

if (name == "proton-bertini") 
    {
      if (protonHadronicIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonBertini(name) );
	  protonHadronicIsRegistered = true;
	}

    }
//--------------------------------------------------------------------------------------------
// Begin Hadronic LEP model
//--------------------------------------------------------------------------------------------

//  LEP model for protons, pions and neutrons

if (name == "proton-LEP") 
    {
      if (protonHadronicIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonLEP(name) );
	  protonHadronicIsRegistered = true;
	}

    }


//--------------------------------------------------------------------------------------------
// HADRONIC INELASTIC  BINARY  MODEL
//--------------------------------------------------------------------------------------------

// Binary cascade model with the default precompound

if (name == "proton-precompound-binary") 
    {
      if (protonHadronicIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonBinary(name) );
	  protonHadronicIsRegistered = true;
	}
  	}  
//--------------------------------------------------------------------------------------------
// End Hadronic models
//--------------------------------------------------------------------------------------------
  




  if (electronIsRegistered && positronIsRegistered && photonIsRegistered &&
      ionIsRegistered) 
    {
      G4cout << 
	"Electromagnetic physics is registered for electron, positron, photons, protons" 
	     << G4endl;
    }
  
  if (protonHadronicIsRegistered && muonIsRegistered && decayIsRegistered)
    {
      G4cout << " Hadronic physics is registered" << G4endl;
    }     
}

void HadrontherapyPhysicsList::SetCuts()
{  
  // Set the threshold of production equal to the defaultCutValue
  // in the experimental set-up
  G4VUserPhysicsList::SetCutsWithDefault();
     
  G4double lowlimit=250*eV;
  G4ProductionCutsTable::GetProductionCutsTable() ->SetEnergyRange(lowlimit, 100.*GeV);
  // Definition of a smaller threshold of production in the phantom region
  // where high accuracy is required in the energy deposit calculation

  G4String regionName = "PhantomLog";
  G4Region* region = G4RegionStore::GetInstance()->GetRegion(regionName);
  G4ProductionCuts* cuts = new G4ProductionCuts ;
  G4double regionCut = 0.01*mm;
  cuts -> SetProductionCut(regionCut,G4ProductionCuts::GetIndex("gamma"));
  cuts -> SetProductionCut(regionCut,G4ProductionCuts::GetIndex("e-"));
  cuts -> SetProductionCut(regionCut,G4ProductionCuts::GetIndex("e+"));
  cuts -> SetProductionCut(regionCut,G4ProductionCuts::GetIndex("proton"));
  cuts -> SetProductionCut(regionCut,G4ProductionCuts::GetIndex("genericIons"));
  region -> SetProductionCuts(cuts);

  if (verboseLevel>0) DumpCutValuesTable();
}



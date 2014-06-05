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
// $Id$
//
// Author: Mathieu Karamitros (kara@cenbg.in2p3.fr)
//
// WARNING : This class is released as a prototype.
// It might strongly evolve or even disapear in the next releases.
//
// History:
// -----------
// 10 Oct 2011 M.Karamitros created
//
// -------------------------------------------------------------------

#include "G4DNAChemistryManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Molecule.hh"
#include "G4ITTrackHolder.hh"
#include "G4H2O.hh"
#include "G4DNAMolecularReactionTable.hh"
#include "G4DNAWaterExcitationStructure.hh"
#include "G4DNAWaterIonisationStructure.hh"
#include "G4Electron_aq.hh"
#include "G4ITManager.hh"
#include "G4MolecularConfiguration.hh"
#include "G4MoleculeCounter.hh"
#include "G4ITTimeStepper.hh"
#include "G4VUserChemistryList.hh"
#include "G4AutoLock.hh"
#include "G4UIcmdWithABool.hh"
#include "G4GeometryManager.hh"
#include "G4StateManager.hh"

using namespace std;

G4DNAChemistryManager* G4DNAChemistryManager::fgInstance ;
G4ThreadLocal std::ofstream*  G4DNAChemistryManager::fpgOutput_tl = 0;
G4ThreadLocal G4bool*  G4DNAChemistryManager::fpgThreadInitialized_tl = 0;
G4Mutex chemManExistence;

G4DNAChemistryManager::G4DNAChemistryManager() :
				G4UImessenger(), G4VStateDependent(),
				fActiveChemistry(false)
{
	fpExcitationLevel   = 0;
	fpIonisationLevel   = 0;
	fWriteFile          = false;
	fpUserChemistryList = 0;
	fMasterInitialized  = false;
	fpChemDNADirectory  = new G4UIdirectory("/process/em/dna/chem/");
	fpActivateChem 	    = new G4UIcmdWithABool("/process/em/dna/chem/activate", this);
	fBuildPhysicsTable  = false;
	fGeometryClosed     = false;
	fPhysicsTableBuilt  = false;
	fForceThreadReinitialization = false;
}

G4DNAChemistryManager* G4DNAChemistryManager::Instance()
{
	if (fgInstance == 0)
	{
		G4AutoLock lock(&chemManExistence);
		if(fgInstance == 0) // MT : double check at initialisation
		{
			fgInstance = new G4DNAChemistryManager();
		}
		lock.unlock();
	}
	return fgInstance;
}

G4DNAChemistryManager* G4DNAChemistryManager::GetInstanceIfExists()
{
	return fgInstance;
}

G4DNAChemistryManager::~G4DNAChemistryManager()
{
//	G4cout << "Deleting G4DNAChemistryManager" << G4endl;
	Clear();
	fgInstance = 0;
	// assert(G4StateManager::GetStateManager()->DeregisterDependent(this) == true);
}

void G4DNAChemistryManager::Clear()
{
	if(fpIonisationLevel)
	{
		delete fpIonisationLevel;
		fpIonisationLevel = 0;

	}
	if(fpExcitationLevel)
	{
		delete fpExcitationLevel;
		fpExcitationLevel = 0;
	}
	if(fpUserChemistryList)
	{
		delete fpUserChemistryList;
		fpUserChemistryList = 0;
	}

	if(fpChemDNADirectory)
	{
		delete fpChemDNADirectory;
		fpChemDNADirectory  = 0;
	}
	if(fpActivateChem)
	{
		delete fpActivateChem;
		fpActivateChem  = 0;
	}

	G4DNAMolecularReactionTable::DeleteInstance();
	//G4MoleculeHandleManager::DeleteInstance();
	G4MolecularConfiguration::DeleteManager();
	G4MoleculeCounter::DeleteInstance();
}

void G4DNAChemistryManager::DeleteInstance()
{
	G4cout << "G4DNAChemistryManager::DeleteInstance" << G4endl;
	G4AutoLock lock(&chemManExistence);

	if(fgInstance)
	{
		G4DNAChemistryManager* deleteMe = fgInstance;
		fgInstance = 0;
		lock.unlock();
		delete deleteMe;
	}
	else
	{
		G4cout << "G4DNAChemistryManager already deleted" << G4endl;
	}
	lock.unlock();
}

G4bool G4DNAChemistryManager::Notify(G4ApplicationState requestedState)
{
	if(requestedState == G4State_Quit)
	{
		G4cout << "G4DNAChemistryManager::Notify ---> received G4State_Quit" << G4endl;
		//DeleteInstance();
		Clear();
	}

	else if(requestedState == G4State_GeomClosed)
	{
		fGeometryClosed = true;
	}

	return true;
}


void G4DNAChemistryManager::SetNewValue(G4UIcommand* command,
		G4String value)
{
	if(command == fpActivateChem)
	{
		SetChemistryActivation(fpActivateChem->GetNewBoolValue(value));
	}
}

void G4DNAChemistryManager::Run()
{
	if(fActiveChemistry)
	{
		InitializeThread();

		if(fMasterInitialized == false)
		{
			G4ExceptionDescription description;
			description << "Global components were not initialized.";
			G4Exception("G4DNAChemistryManager::Run", "MASTER_INIT", FatalException, description);
		}

		if(fpgThreadInitialized_tl == 0)
		{
			G4ExceptionDescription description;
			description << "Thread local components were not initialized.";
			G4Exception("G4DNAChemistryManager::Run", "THREAD_INIT", FatalException, description);
		}

		G4ITTimeStepper::Instance() -> Process();
		CloseFile();
	}
}

void G4DNAChemistryManager::Gun(G4ITGun* gun, bool physicsTableToBuild)
{
	fBuildPhysicsTable = physicsTableToBuild;
	G4ITTimeStepper::Instance() -> SetGun(gun);
}

void G4DNAChemistryManager::InitializeMaster()
{
	if(fMasterInitialized == false)
	{
		if(fpUserChemistryList)
		{
			fpUserChemistryList->ConstructDissociationChannels();
			fpUserChemistryList->ConstructReactionTable(G4DNAMolecularReactionTable::GetReactionTable());
			fMasterInitialized = true;
		}
		else
		{
			if(fActiveChemistry)
			{
				G4ExceptionDescription description;
				description << "No user chemistry list has been provided.";
				G4Exception("G4DNAChemistryManager::InitializeMaster", "NO_CHEM_LIST", FatalException, description);
			}
		}
	}
}


void G4DNAChemistryManager::InitializeThread()
{
	if(fpgThreadInitialized_tl == 0 || fForceThreadReinitialization == true)
	{
		if(fpUserChemistryList)
		{
			if(fBuildPhysicsTable && fPhysicsTableBuilt == false)
			{
				fpUserChemistryList->BuildPhysicsTable();
				if(fGeometryClosed == false)
				{
					G4GeometryManager* geomManager = G4GeometryManager::GetInstance();
					G4cout << "Start closing geometry." << G4endl;
					geomManager->OpenGeometry();
					geomManager->CloseGeometry(true, true);
					fGeometryClosed = true;
				}

				fPhysicsTableBuilt = true;
			}
			fpUserChemistryList->ConstructTimeStepModel(G4DNAMolecularReactionTable::GetReactionTable());
			G4ITTimeStepper::Instance()-> Initialize();
			fpgThreadInitialized_tl = new G4bool(true);
		}
		else
		{
			G4ExceptionDescription description;
			description << "No user chemistry list has been provided.";
			G4Exception("G4DNAChemistryManager::InitializeThread", "NO_CHEM_LIST", FatalException, description);
		}

		G4MoleculeCounter::Instance()->Initialize();
	}

	InitializeFile();
}

void G4DNAChemistryManager::InitializeFile()
{
	if(fpgOutput_tl == 0 || fWriteFile == false) return;


	*fpgOutput_tl << std::setprecision(6) << std::scientific;
	*fpgOutput_tl << setw(11) << left << "#Parent ID"
			<< setw(10) << "Molecule"
			<< setw(14) << "Elec Modif"
			<< setw(13) << "Energy (eV)"
			<< setw(22) << "X pos of parent [nm]"
			<< setw(22) << "Y pos of parent [nm]"
			<< setw(22) << "Z pos of parent [nm]"
			<< setw(14) << "X pos [nm]"
			<< setw(14) << "Y pos [nm]"
			<< setw(14) << "Z pos [nm]"
			<< G4endl
			<< setw(21) << "#"
			<< setw(13) << "1)io/ex=0/1"
			<< G4endl
			<< setw(21) << "#"
			<< setw(13) << "2)level=0...5"
			<< G4endl;
}

void G4DNAChemistryManager::WriteInto(const G4String& output,
		ios_base::openmode mode)
{
	fpgOutput_tl =  new std::ofstream();
	fpgOutput_tl->open(output.data(), mode);
	fWriteFile = true;
}

void G4DNAChemistryManager::CloseFile()
{

	if(fpgOutput_tl == 0 || fWriteFile == false) return;


	if (fpgOutput_tl->is_open())
	{
		fpgOutput_tl->close();
	}

	fWriteFile = false;
}

G4DNAWaterExcitationStructure* G4DNAChemistryManager::GetExcitationLevel()
{
	if(!fpExcitationLevel)
	{
		fpExcitationLevel = new G4DNAWaterExcitationStructure;
	}
	return fpExcitationLevel;
}

G4DNAWaterIonisationStructure* G4DNAChemistryManager::GetIonisationLevel()
{
	if(!fpIonisationLevel)
	{
		fpIonisationLevel = new G4DNAWaterIonisationStructure;
	}
	return fpIonisationLevel;
}

void G4DNAChemistryManager::CreateWaterMolecule(ElectronicModification modification,
		G4int electronicLevel,
		const G4Track* theIncomingTrack)
{
	if(fWriteFile)
	{
		G4double energy = -1.;

		switch (modification)
		{
		case eDissociativeAttachment:
			energy = -1;
			break;
		case eExcitedMolecule :
			energy = GetExcitationLevel()->ExcitationEnergy(electronicLevel);
			break;
		case eIonizedMolecule :
			energy = GetIonisationLevel()->IonisationEnergy(electronicLevel);
			break;
		}

		*fpgOutput_tl << setw(11) << left << theIncomingTrack->GetTrackID()
                										<< setw(10) << "H2O"
                										<< left << modification
                										<< internal <<":"
                										<< right <<electronicLevel
                										<< left
                										<< setw(11) << ""
                										<< std::setprecision(2) << std::fixed
                										<< setw(13) << energy/eV
                										<< std::setprecision(6) << std::scientific
                										<< setw(22) << (theIncomingTrack->GetPosition().x())/nanometer
                										<< setw(22) << (theIncomingTrack->GetPosition().y())/nanometer
                										<< setw(22) << (theIncomingTrack->GetPosition().z())/nanometer
                										<< G4endl;
	}

	if(fActiveChemistry)
	{
		G4Molecule * H2O = new G4Molecule (G4H2O::Definition());

		switch (modification)
		{
		case eDissociativeAttachment:
			H2O -> AddElectron(5,1);
			break;
		case eExcitedMolecule :
			H2O -> ExciteMolecule(electronicLevel);
			break;
		case eIonizedMolecule :
			H2O -> IonizeMolecule(electronicLevel);
			break;
		}

		G4Track * H2OTrack = H2O->BuildTrack(1*picosecond,
				theIncomingTrack->GetPosition());

		H2OTrack -> SetParentID(theIncomingTrack->GetTrackID());
		H2OTrack -> SetTrackStatus(fStopButAlive);
		H2OTrack -> SetKineticEnergy(0.);

		G4ITTrackHolder::Instance()->PushTrack(H2OTrack);
	}
}

void G4DNAChemistryManager::CreateSolvatedElectron(const G4Track* theIncomingTrack,
		G4ThreeVector* finalPosition)
// finalPosition is a pointer because this argument is optional
{
	if(fWriteFile)
	{
		*fpgOutput_tl << setw(11)<< theIncomingTrack->GetTrackID()
                										<< setw(10)<< "e_aq"
                										<< setw(14)<< -1
                										<< std::setprecision(2) << std::fixed
                										<< setw(13)<< theIncomingTrack->GetKineticEnergy()/eV
                										<< std::setprecision(6) << std::scientific
                										<< setw(22)<< (theIncomingTrack->GetPosition().x())/nanometer
                										<< setw(22)<< (theIncomingTrack->GetPosition().y())/nanometer
                										<< setw(22)<< (theIncomingTrack->GetPosition().z())/nanometer  ;

		if(finalPosition != 0)
		{
			*fpgOutput_tl<< setw(14)<< (finalPosition->x())/nanometer
					<< setw(14)<< (finalPosition->y())/nanometer
					<< setw(14)<< (finalPosition->z())/nanometer ;
		}

		*fpgOutput_tl << G4endl;
	}

	if(fActiveChemistry)
	{
		G4Molecule* e_aq = new G4Molecule(G4Electron_aq::Definition());
		G4Track * e_aqTrack(0);
		if(finalPosition)
		{
			e_aqTrack  = e_aq->BuildTrack(picosecond,*finalPosition);
		}
		else
		{
			e_aqTrack  = e_aq->BuildTrack(picosecond,theIncomingTrack->GetPosition());
		}
		e_aqTrack -> SetTrackStatus(fAlive);
		e_aqTrack -> SetParentID(theIncomingTrack->GetTrackID());
		G4ITTrackHolder::Instance()->PushTrack(e_aqTrack);
		G4ITManager<G4Molecule>::Instance()->Push(e_aqTrack);
	}
}


void G4DNAChemistryManager::PushMolecule(G4Molecule*& molecule, double time,
		const G4ThreeVector& position, int parentID)
{
	if(fWriteFile)
	{
		*fpgOutput_tl << setw(11)<< parentID
				<< setw(10)<< molecule->GetName()
				<< setw(14)<< -1
				<< std::setprecision(2) << std::fixed
				<< setw(13)<< -1
				<< std::setprecision(6) << std::scientific
				<< setw(22)<< (position.x())/nanometer
				<< setw(22)<< (position.y())/nanometer
				<< setw(22)<< (position.z())/nanometer;
		*fpgOutput_tl << G4endl;
	}

	if(fActiveChemistry)
	{
		G4Track* track = molecule->BuildTrack(time,position);
		track -> SetTrackStatus(fAlive);
		track -> SetParentID(parentID);
		G4ITTrackHolder::Instance()->PushTrack(track);
		G4ITManager<G4Molecule>::Instance()->Push(track);
	}
	else
	{
		delete molecule;
		molecule = 0;
	}
}

void G4DNAChemistryManager::PushMoleculeAtParentTimeAndPlace(G4Molecule*& molecule,
		const G4Track* theIncomingTrack)
{
	if(fWriteFile)
	{
		*fpgOutput_tl << setw(11)<< theIncomingTrack->GetTrackID()
                										<< setw(10)<< molecule->GetName()
                										<< setw(14)<< -1
                										<< std::setprecision(2) << std::fixed
                										<< setw(13)<< theIncomingTrack->GetKineticEnergy()/eV
                										<< std::setprecision(6) << std::scientific
                										<< setw(22)<< (theIncomingTrack->GetPosition().x())/nanometer
                										<< setw(22)<< (theIncomingTrack->GetPosition().y())/nanometer
                										<< setw(22)<< (theIncomingTrack->GetPosition().z())/nanometer  ;
		*fpgOutput_tl << G4endl;
	}

	if(fActiveChemistry)
	{
		G4Track* track = molecule->BuildTrack(theIncomingTrack->GetGlobalTime(),theIncomingTrack->GetPosition());
		track -> SetTrackStatus(fAlive);
		track -> SetParentID(theIncomingTrack->GetTrackID());
		G4ITTrackHolder::Instance()->PushTrack(track);
		G4ITManager<G4Molecule>::Instance()->Push(track);
	}
	else
	{
		delete molecule;
		molecule = 0;
	}
}

void G4DNAChemistryManager::AddEmptyLineInOuputFile()
{
	if(fWriteFile)
	{
		*fpgOutput_tl << G4endl;
	}
}

/*
 * G4VUserChemistryList.cc
 *
 *  Created on: 23 oct. 2013
 *      Author: kara
 */

#include "G4VUserChemistryList.hh"
#include "G4ITStepManager.hh"
#include "G4MoleculeTable.hh"
#include "G4MoleculeDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4DNAChemistryManager.hh"

G4VUserChemistryList::G4VUserChemistryList() {
	// TODO Auto-generated constructor stub

	verboseLevel = 1;
}

G4VUserChemistryList::~G4VUserChemistryList() {
	// TODO Auto-generated destructor stub

	G4DNAChemistryManager* chemMan = G4DNAChemistryManager::GetInstanceIfExists();
	if(chemMan)
	{
		chemMan->Deregister(this);
	}
}

void G4VUserChemistryList::RegisterTimeStepModel(G4VITModel* timeStepModel,
		double startingTime)
{
	G4ITStepManager::Instance()->GetModelHandler()->RegisterModel(timeStepModel, startingTime);
}


void G4VUserChemistryList::BuildPhysicsTable()
{
	G4MoleculeTable* theMoleculeTable = G4MoleculeTable::Instance();

	G4MoleculeDefinitionIterator* iterator = theMoleculeTable ->GetDefintionIterator();

	//Prepare Physics table for all particles
	//	iterator->reset();
	//	while( (*iterator)() ){
	//		G4MoleculeDefinition* moleculeDef = iterator->value();
	//PreparePhysicsTable(particle);
	//	}

	iterator->reset();
	while( (*iterator)() ){
		G4MoleculeDefinition* moleculeDef = iterator->value();
		G4cout << moleculeDef->GetName() << G4endl;
		BuildPhysicsTable(moleculeDef);
	}
}

void G4VUserChemistryList::BuildPhysicsTable(G4MoleculeDefinition* moleculeDef)
{
	//Get processes from master thread;
	G4ProcessManager* pManager =  moleculeDef->GetProcessManager();

	if (!pManager) {
#ifdef G4VERBOSE
		if (verboseLevel>0){
			G4cout << "G4VUserPhysicsList::BuildPhysicsTable "
					<<" : No Process Manager for "
					<< moleculeDef->GetParticleName() << G4endl;
			G4cout << moleculeDef->GetParticleName()
	    				   << " should be created in your PhysicsList" <<G4endl;
		}
#endif
		G4Exception("G4VUserChemistryList::BuildPhysicsTable",
				"Run0271", FatalException,
				"No process manager");
		return;
	}

	G4ProcessManager* pManagerShadow = moleculeDef->GetMasterProcessManager();
	G4ProcessVector* pVector = pManager->GetProcessList();
	if (!pVector) {
#ifdef G4VERBOSE
		if (verboseLevel>0){
			G4cout << "G4VUserChemistryList::BuildPhysicsTable  "
					<<" : No Process Vector for "
					<< moleculeDef->GetParticleName() <<G4endl;
		}
#endif
		G4Exception("G4VUserChemistryList::BuildPhysicsTable",
				"Run0272", FatalException,
				"No process Vector");
		return;
	}
#ifdef G4VERBOSE
	if (verboseLevel>2){
		G4cout << "G4VUserChemistryList::BuildPhysicsTable %%%%%% " << moleculeDef->GetParticleName() << G4endl;
		G4cout << " ProcessManager : " << pManager << " ProcessManagerShadow : " << pManagerShadow << G4endl;
		for(G4int iv1=0;iv1<pVector->size();iv1++)
		{ G4cout << "  " << iv1 << " - " << (*pVector)[iv1]->GetProcessName() << G4endl; }
		G4cout << "--------------------------------------------------------------" << G4endl;
		G4ProcessVector* pVectorShadow = pManagerShadow->GetProcessList();

		for(G4int iv2=0;iv2<pVectorShadow->size();iv2++)
		{ G4cout << "  " << iv2 << " - " << (*pVectorShadow)[iv2]->GetProcessName() << G4endl; }
	}
#endif
	for (G4int j=0; j < pVector->size(); ++j) {
		//Andrea July 16th 2013 : migration to new interface...
		//Infer if we are in a worker thread or master thread
		//Master thread is the one in which the process manager
		// and process manager shadow pointers are the same
		if ( pManagerShadow == pManager )
		{
			(*pVector)[j]->BuildPhysicsTable(*moleculeDef);
		}
		else
		{
			(*pVector)[j]->BuildWorkerPhysicsTable(*moleculeDef);
		}

	}
}

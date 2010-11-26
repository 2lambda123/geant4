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
// The code was written by :
//	^Claudio Andenna  claudio.andenna@ispesl.it, claudio.andenna@iss.infn.it
//      *Barbara Caccia barbara.caccia@iss.it
//      with the support of Pablo Cirrone (LNS, INFN Catania Italy)
//	with the contribute of Alessandro Occhigrossi*
//
// ^INAIL DIPIA - ex ISPESL and INFN Roma, gruppo collegato Sanità, Italy
// *Istituto Superiore di Sanità and INFN Roma, gruppo collegato Sanità, Italy
//  Viale Regina Elena 299, 00161 Roma (Italy)
//  tel (39) 06 49902246
//  fax (39) 06 49387075
//
// more information:
// http://g4advancedexamples.lngs.infn.it/Examples/medical-linac
//
//*******************************************************//


#include "ML2PhantomConstructionMessenger.hh"
#include "ML2PhantomConstruction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"


CML2PhantomConstructionMessenger::CML2PhantomConstructionMessenger(CML2PhantomConstruction *phantomConstructor) : pPhantomConstructor (phantomConstructor)
{
	this->PhantomName=new G4UIcmdWithAString("/phantom/PhantomName",this);
	this->PhantomName->SetDefaultValue("fullWater");
	this->PhantomName->SetGuidance("phantom name to select among those implemented  fullWater, boxInBox, Dicom1");
	this->pPhantomConstructor->setPhantomName("fullWater");

	this->PhantomFileName =new G4UIcmdWithAString("/phantom/PhantomFileName",this);
	this->PhantomFileName ->SetDefaultValue("");
	this->PhantomFileName->SetGuidance("full path and macro file name containing specific setup data for the phantom chosen");
	this->pPhantomConstructor->setPhantomFileName ("");

	this->phantomCentre=new G4UIcmdWith3VectorAndUnit("/phantom/centre", this);
	this->phantomCentre->SetDefaultUnit("mm");
	this->phantomCentre->SetGuidance("phantom centre coordinates in the world [mm]");
	this->phantomCentre->SetDefaultValue(G4ThreeVector(0.,0.,0.));
}

CML2PhantomConstructionMessenger::~CML2PhantomConstructionMessenger(void)
{
	delete PhantomName;
	delete phantomCentre;
}
void CML2PhantomConstructionMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
	if (cmd==this->PhantomName)
	{this->pPhantomConstructor->setPhantomName(newValue);}

	if (cmd==this->PhantomFileName )
	{this->pPhantomConstructor->setPhantomFileName (newValue);}

	if (cmd==this->phantomCentre )
	{
		if (this->bOnlyVisio)
		{
			this->pPhantomConstructor->applyNewCentre(this->phantomCentre->GetNew3VectorRawValue(newValue));
// what follows seems to be necessary to have a good refresh
			G4UImanager* UI = G4UImanager::GetUIpointer();
			G4String command;
			command = "/run/beamOn 0";
			UI->ApplyCommand(command); 
			command = "/vis/viewer/flush";
			UI->ApplyCommand(command); 
		}
		else
		{
			this->pPhantomConstructor->addNewCentre(this->phantomCentre->GetNew3VectorRawValue(newValue));
		}
	}
}


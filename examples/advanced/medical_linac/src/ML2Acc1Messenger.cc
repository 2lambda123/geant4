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
//	^Claudio Andenna claudio.andenna@iss.infn.it, claudio.andenna@ispesl.it
//      *Barbara Caccia barbara.caccia@iss.it
//      with the support of Pablo Cirrone (LNS, INFN Catania Italy)
//
// ^ISPESL and INFN Roma, gruppo collegato Sanità, Italy
// *Istituto Superiore di Sanità and INFN Roma, gruppo collegato Sanità, Italy
//  Viale Regina Elena 299, 00161 Roma (Italy)
//  tel (39) 06 49902246
//  fax (39) 06 49387075
//
// more information:
// http://g4advancedexamples.lngs.infn.it/Examples/medical-linac
//
//*******************************************************//


#include "ML2Acc1Messenger.hh"
#include "ML2Acc1.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

CML2Acc1Messenger::CML2Acc1Messenger(CML2Acc1 *acc1) : pAcc1(acc1)
{
	this->idEnergy=new G4UIcmdWithAnInteger("/accelerator/idEnergy",this);
	this->idEnergy->SetDefaultValue(6);

	this->leavesA=new G4UIcmdWithADoubleAndUnit("/accelerator/leavesA", this);
	this->leavesA->SetDefaultUnit("mm");
	this->leavesA->SetDefaultValue(300.);

	this->leavesB=new G4UIcmdWithADoubleAndUnit("/accelerator/leavesB", this);
	this->leavesB->SetDefaultUnit("mm");
	this->leavesB->SetDefaultValue(300.);

	this->aperture1X=new G4UIcmdWithADoubleAndUnit("/accelerator/aperture1X", this);
	this->aperture1X->SetDefaultUnit("mm");
	this->aperture1X->SetDefaultValue(100.);

	this->aperture1Y=new G4UIcmdWithADoubleAndUnit("/accelerator/aperture1Y", this);
	this->aperture1Y->SetDefaultUnit("mm");
	this->aperture1Y->SetDefaultValue(100.);

	this->aperture2X=new G4UIcmdWithADoubleAndUnit("/accelerator/aperture2X", this);
	this->aperture2X->SetDefaultUnit("mm");
	this->aperture2X->SetDefaultValue(-100.);

	this->aperture2Y=new G4UIcmdWithADoubleAndUnit("/accelerator/aperture2Y", this);
	this->aperture2Y->SetDefaultUnit("mm");
	this->aperture2Y->SetDefaultValue(-100.);

	this->SSD=new G4UIcmdWithADoubleAndUnit("/accelerator/SSD", this);
	this->SSD->SetDefaultUnit("mm");
	this->SSD->SetDefaultValue(1000.);

}

CML2Acc1Messenger::~CML2Acc1Messenger(void)
{
	delete idEnergy;
	delete aperture1X;
	delete aperture2X;
	delete aperture1Y;
	delete aperture2Y;
	delete SSD;
	delete leavesA;
	delete leavesB;
}
void CML2Acc1Messenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
	if (cmd==this->aperture1X)
	{
		this->aperture1X->GetNewUnitValue(newValue);
		this->pAcc1->setJaw1X(this->aperture1X->GetNewDoubleValue(newValue));
	}

	if (cmd==this->aperture1Y)
	{
		this->aperture1Y->GetNewUnitValue(newValue);
		this->pAcc1->setJaw1Y(this->aperture1Y->GetNewDoubleValue(newValue));
	}

	if (cmd==this->aperture2X)
	{
		this->aperture2X->GetNewUnitValue(newValue);
		this->pAcc1->setJaw2X(this->aperture2X->GetNewDoubleValue(newValue));
	}
	if (cmd==this->aperture2Y)
	{
		this->aperture2Y->GetNewUnitValue(newValue);
		this->pAcc1->setJaw2Y(this->aperture2Y->GetNewDoubleValue(newValue));
	}

	if (cmd==this->SSD)
	{
		this->SSD->GetNewUnitValue(newValue);
		this->pAcc1->setSSD(this->SSD->GetNewDoubleValue(newValue));
	}

	if (cmd==this->leavesA)
	{this->pAcc1->setLeavesAx(this->leavesA->GetNewDoubleValue(newValue));}

	if (cmd==this->leavesB)
	{this->pAcc1->setLeavesBx(this->leavesA->GetNewDoubleValue(newValue));}


	if (cmd==this->idEnergy)
	{this->pAcc1->setidEnergy(this->idEnergy->GetNewIntValue(newValue));}

}

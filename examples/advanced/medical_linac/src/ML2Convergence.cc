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


#include "ML2Convergence.hh"

CML2Convergence::CML2Convergence(void)
{}

CML2Convergence::CML2Convergence(G4int seed, G4int saving_in_Selected_Voxels_every_events, G4String FileExperimentalData, G4String FileExperimentalDataOut, G4bool bCompareExp, G4int maxNumberOfEvents, G4int nRecycling, G4int nMaxLoops)
:ML2ExpVoxels(0)
{
	this->nGeometry=0;
	this->nMaxLoops=nMaxLoops;
	this->idCurrentLoop=this->nMaxLoops;
	this->bCompareExp=bCompareExp;
	if (this->bCompareExp){this->nMaxLoops=-1;};
	this->fileExperimentalData=FileExperimentalData;

// if the flag compareExp if true and the experimental data is given create the class CML2ExpVoxels
	if (this->bCompareExp && this->fileExperimentalData!="")
	{
		this->ML2ExpVoxels=new CML2ExpVoxels(this->bCompareExp, saving_in_Selected_Voxels_every_events, seed, FileExperimentalData, FileExperimentalDataOut);
		if (!this->ML2ExpVoxels->loadData())
		{
			this->ML2ExpVoxels=0;
		std::cout <<"I don't have any convergence criteria set, I'll do " << this->nMaxLoops << " loop(s) for each rotation"<< G4endl;
		}
		else
		{
			this->ML2ExpVoxels->setRecycling(nRecycling);
		}
	}
	this->maxNumberOfEvents=maxNumberOfEvents;
}

CML2Convergence::~CML2Convergence(void)
{
	if (this->ML2ExpVoxels!=0)
	{delete this->ML2ExpVoxels;}

}
void CML2Convergence::add(const G4Step* aStep)
{
// accumulate events in the CML2ExpVoxels class (if created)
	if (this->ML2ExpVoxels!=0)
	{
		if (aStep->GetTotalEnergyDeposit()>0.)
		{this->ML2ExpVoxels->add(aStep);}
	}
}
G4bool CML2Convergence::stopRun()
{
	G4bool bStopRun=false;
	if (this->ML2ExpVoxels!=0) // true if the experimental data file exists and is used to check the convergence
	{
		bStopRun=this->convergenceCriteria();
		return bStopRun;
	}
	else // true if no experiemental data file is used. In this case it runs "nMaxLoops" loops.
	{
		this->idCurrentLoop--;
		if (this->idCurrentLoop==0)
		{
			bStopRun=true;
		}
	}
	return bStopRun;
}
G4bool CML2Convergence::convergenceCriteria()
{
	G4bool bStopRun=true;
	if (this->bCompareExp)
	{
// It checks if the maximum number of events is reached at least in one voxel. Having more rotations the limits is incremented each rotation
		if (this->ML2ExpVoxels->getMaxNumberOfEvents() >= this->maxNumberOfEvents*this->nGeometry)
		{bStopRun = true;}
		else
		{bStopRun = false;}
	}
	std::cout << "max n. of events accumulated:"<<this->ML2ExpVoxels->getMaxNumberOfEvents() <<"; max n. of events to be accumulated:" <<this->maxNumberOfEvents<<"  current geometry: " << this->nGeometry<<G4endl;
	return bStopRun;
}

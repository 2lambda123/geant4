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


#ifndef CML2SensitiveDetectorParticleH
#define CML2SensitiveDetectorParticleH

#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4VTouchable.hh"
#include "ML2SinputData.hh"

class CML2ReadOutGeometryVoxels;

class CML2SDWithParticle : public G4VSensitiveDetector
{
public:
	CML2SDWithParticle();
	CML2SDWithParticle(G4int idType, G4int max_N_particles_in_PhSp_File, G4int seed, G4int nMaxParticlesInRamPhaseSpace, G4String name, G4String PhaseSpaceOutFile, G4bool bSavePhaseSpace, G4bool bStopAtVolatilePhaseSpace, SPrimaryParticle *primaryParticleData);
	~CML2SDWithParticle(void);
	G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist);
//	inline void setbStopAtPhaseSpace(G4bool bStopAtPhaseSpace){this->bStopAtPhaseSpace=bStopAtPhaseSpace;};
	//inline void Initialize(G4HCofThisEvent *){};
	//inline void EndOfEvent(G4HCofThisEvent*){};
	G4int getTotalNumberOfParticles(){return this->nTotalParticles;};
	inline CML2SDWithParticle* getCML2SensitiveDetectorParticle(){return this;};
	inline Sparticle getParticle(int i){return this->particles[i];};
	inline void setActive(G4bool bActive){this->bActive=bActive;};
	inline bool getBContinueRun(){return this->bContinueRun;};
private:
	void saveDataParticles(G4int nParticle);
	void saveHeaderParticles();

	G4ThreeVector halfSize;
	G4ThreeVector pos;
	SPrimaryParticle *primaryParticleData;
	Sparticle *particles;
	G4String fullOutFileData;
	G4int nTotalParticles, nParticle;
	G4int idType, nMaxParticlesInRamPhaseSpace, max_N_particles_in_PhSp_File;
	G4bool bStopAtPhaseSpace, bSavePhaseSpace, bActive, bContinueRun;
};

#endif

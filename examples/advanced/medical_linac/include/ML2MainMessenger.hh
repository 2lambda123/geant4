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


#ifndef CML2MainMessengerH
#define CML2MainMessengerH

#include "ML2Main.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "ML2CInputData.hh"


class CML2Main;
class CML2CInputData;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithAInteger;

class CML2MainMessenger : public G4UImessenger 
{
public:
	CML2MainMessenger(CML2CInputData *CInputData);
	~CML2MainMessenger(void);
	void SetNewValueOLD(G4UIcommand* cmd, G4String newValue);
	void SetNewValue(G4UIcommand* cmd, G4String newValue);

private:

	CML2CInputData *CInputData;
	CML2Main *ML2Main;

	G4UIcmdWith3VectorAndUnit *phaseSpaceCentre, *phaseSpaceHalfSize;
	G4UIcmdWithAString *phaseSPaceOutFile, *ROGOutFile;
	G4UIcmdWithABool *bSavePhaseSpace, *bStopAtPhaseSpace, *bSaveROG;

	G4UIcmdWithAnInteger *nBeam, *nMaxParticlesInRamPlanePhaseSpace, *minNumberOfEvents;
	G4UIcmdWithABool *bCompareExp, *bOnlyVisio;
	G4UIcmdWithAString * fileExperimentalData;
	G4UIcmdWithAnInteger *saving_in_Selected_Voxels_every_events;
	G4UIcmdWithAnInteger *saving_in_ROG_Voxels_every_events;
	G4UIcmdWithAnInteger *max_N_particles_in_PhSp_File; 
};

#endif

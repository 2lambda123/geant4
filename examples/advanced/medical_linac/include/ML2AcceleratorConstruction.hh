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

#ifndef CML2AcceleratorConstructionH
#define CML2AcceleratorConstructionH

#include "ML2SinputData.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4UImanager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"

#include "ML2PrimaryGenerationAction.hh"

#include "ML2Acc1.hh"

class CML2AcceleratorConstructionMessenger;
class CML2PrimaryGenerationAction;

class CML2AcceleratorConstruction
{
public:
	CML2AcceleratorConstruction(void);
	~CML2AcceleratorConstruction(void);
	static CML2AcceleratorConstruction* GetInstance(void);
	bool Construct(G4VPhysicalVolume *PVWorld, G4bool bOnlyVisio);
	inline G4VPhysicalVolume *getPhysicalVolume(void){return this->PVAccWorld;};
	void resetAccelerator();

	inline void setAcceleratorName(G4String val){this->AcceleratorName=val;};
	inline void setAcceleratorMacFileName(G4String val){this->AcceleratorMacFileName=val;};

	G4String getCurrentRotationString();

	inline G4String getNextAcceleratorXRotationName(){return this->nextAcceleratorXRotationName;};
	inline void setIsoCentre(G4double val){this->isoCentre=val;};
	inline void setRotation90Y(G4bool val){this->bRotate90Y=val;};

	inline void addAcceleratorRotationsX(G4double val){this->rotationsX.push_back(val);};

	inline G4double getAcceleratorIsoCentre(){return this->isoCentre;};
	inline G4String getAcceleratorName(){return this->AcceleratorName;};
	inline G4String getAcceleratorMacFileName(){return this->AcceleratorMacFileName;};
	inline G4double getZ_Value_PhaseSpaceBeforeJaws(){return this->Z_Value_PhaseSpaceBeforeJaws;};
	inline G4bool getRotation90Y(){return this->bRotate90Y;};
	void writeInfo();

	G4RotationMatrix * rotateAccelerator();
	G4RotationMatrix * rotateAccelerator(G4double angleX);
private:
	bool design(void);

	CML2AcceleratorConstructionMessenger *acceleratorConstructionMessenger;
	static CML2AcceleratorConstruction * instance;
	G4String AcceleratorName, AcceleratorMacFileName, nextAcceleratorXRotationName;

	G4VPhysicalVolume *PVAccWorld;
	G4RotationMatrix *rotation;
	G4int idCurrentRotationX;
	G4double currentRotationX, isoCentre, Z_Value_PhaseSpaceBeforeJaws;
	std::vector <G4double> rotationsX;
	G4ThreeVector initialCentre;
	G4bool bRotate90Y, bOnlyVisio;
	

	CML2Acc1 *accelerator1;
};

#endif

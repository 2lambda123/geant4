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


#ifndef CML2PhantomConstructionH
#define CML2PhantomConstructionH

#include "globals.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "ML2SinputData.hh"
#include "ML2Ph_FullWater.hh"
#include "ML2Ph_BoxInBox.hh"

class CML2PhantomConstructionMessenger;
class CPh_HalfWaterAir;
class CPh_FullWater;
class CML2PhantomConstruction
{
public:
	CML2PhantomConstruction(void);
	~CML2PhantomConstruction(void);
	static CML2PhantomConstruction* GetInstance(void);
	void Construct(G4VPhysicalVolume *PVWorld, G4int saving_in_ROG_Voxels_every_events, G4int seed, G4String ROGOutFile, G4bool bSaveROG);
	inline G4int getTotalNumberOfEvents()
	{
		if (this->phantomName="fullWater")
		{return this->Ph_fullWater->getTotalNumberOfEvents();}
		else if (this->phantomName="BoxInBox")
		{return this->Ph_BoxInBox->getTotalNumberOfEvents();}
		return 0;
	};
	inline void setPhantomName(G4String val){this->phantomName=val;};
	inline void setPhantom_nVoxelsX(G4int val){this->nVoxelsX=val;};
	inline void setPhantom_nVoxelsY(G4int val){this->nVoxelsY=val;};
	inline void setPhantom_nVoxelsZ(G4int val){this->nVoxelsZ=val;};
	inline void setPhantomSpecficationsFileName(G4String val){this->PhantomSpecficationsFileName=val;};
	inline void setPhantomRotationX(G4double val){this->rotationX=val;};
	inline void setPhantomRotationY(G4double val){this->rotationY=val;};
	inline void setPhantomRotationZ(G4double val){this->rotationZ=val;};
private:
	void design(void);
	CML2PhantomConstructionMessenger *phantomContstructionMessenger;
	static CML2PhantomConstruction * instance;
	G4int nVoxelsX, nVoxelsY, nVoxelsZ;
	G4String phantomName, PhantomSpecficationsFileName;

	G4RotationMatrix *rotation;
	G4VPhysicalVolume *PVPhmWorld;

	G4double rotationX, rotationY, rotationZ;
	CML2Ph_FullWater *Ph_fullWater;
	CML2Ph_BoxInBox *Ph_BoxInBox;
};
#endif


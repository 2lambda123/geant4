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
//
// FredVoxelTestMessenger.hh
//
// Declaration of Fred's voxel test control
//

#ifndef FredVoxelTestMessenger_hh
#define FredVoxelTestMessenger_hh

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

class FredVoxelTest;
class G4VSolid;

class FredVoxelTestMessenger : public G4UImessenger {
	public:
	FredVoxelTestMessenger();
	~FredVoxelTestMessenger();
	
	void SetNewValue( G4UIcommand *command, G4String newValues );
        G4String GetCurrentValue( G4UIcommand *command );

	inline void SetTestVolume( const G4VSolid *newTestVolume ) { testVolume = newTestVolume; }
	inline const G4VSolid *GetTestVolume() { return testVolume; }

	protected:
	const G4VSolid		*testVolume;
	
	FredVoxelTest	*voxelTest;
	
	G4UIdirectory			*voxelTestDirectory;
	G4UIcmdWith3VectorAndUnit	*xMinMaxCmd;
	G4UIcmdWith3VectorAndUnit	*yMinMaxCmd;
	G4UIcmdWith3VectorAndUnit	*zMinMaxCmd;
	G4UIcmdWith3VectorAndUnit	*posCmd;
	G4UIcmdWithADouble		*xRotateCmd;
	G4UIcmdWithADouble		*yRotateCmd;
	G4UIcmdWithADouble		*zRotateCmd;
	G4UIcmdWithoutParameter		*levelCmd;
	G4UIcmdWithAString		*drawCmd;
	G4UIcmdWithAString		*testCmd;
	G4UIcmdWithoutParameter		*resetCmd;
	
	void InvokeTest( G4String request );
	void Draw();
};
	

#endif

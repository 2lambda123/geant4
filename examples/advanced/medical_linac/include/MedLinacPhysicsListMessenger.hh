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
// $Id: MedLinacPhysicsListMessenger.hh,v 1.1 2005-07-03 23:27:37 mpiergen Exp $
//
//
// Code developed by: M. Piergentili

#ifndef MedLinacPhysicsListMessenger_h
#define MedLinacPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MedLinacPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//****************************************************************************

class MedLinacPhysicsListMessenger: public G4UImessenger
{
  public:
    MedLinacPhysicsListMessenger(MedLinacPhysicsList* );
   ~MedLinacPhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    MedLinacPhysicsList* pMedLinacPhysicsList;

    G4UIdirectory*             PhysicsDir;
    G4UIcmdWithADoubleAndUnit* CutCmd;
   
  //G4UIcmdWithoutParameter*   UpdateCmd;
};

//****************************************************************************
#endif


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
// $Id: F03FieldMessenger.cc,v 1.6 2004-03-23 15:40:39 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#include "F03FieldMessenger.hh"

#include "F03FieldSetup.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//////////////////////////////////////////////////////////////////////////////

F03FieldMessenger::F03FieldMessenger(F03FieldSetup* pEMfield)
  : fEMfieldSetup(pEMfield)
{ 
  F03detDir = new G4UIdirectory("/field/");
  F03detDir->SetGuidance("F03 field tracking control.");

  StepperCmd = new G4UIcmdWithAnInteger("/field/setStepperType",this);
  StepperCmd->SetGuidance("Select stepper type for magnetic field");
  StepperCmd->SetParameterName("choice",true);
  StepperCmd->SetDefaultValue(4);
  StepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 
  UpdateCmd = new G4UIcmdWithoutParameter("/field/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/field/setFieldZ",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false,false);
  MagFieldCmd->SetDefaultUnit("tesla");
  MagFieldCmd->AvailableForStates(G4State_Idle); 
 
  MinStepCmd = new G4UIcmdWithADoubleAndUnit("/field/setMinStep",this);  
  MinStepCmd->SetGuidance("Define minimal step");
  MinStepCmd->SetGuidance("Magnetic field will be in Z direction.");
  MinStepCmd->SetParameterName("min step",false,false);
  MinStepCmd->SetDefaultUnit("mm");
  MinStepCmd->AvailableForStates(G4State_Idle);  
       
  AbsMaterCmd = new G4UIcmdWithAString("/field/setAbsMat",this);
  AbsMaterCmd->SetGuidance("Select Material of the Absorber.");
  AbsMaterCmd->SetParameterName("choice",true);
  AbsMaterCmd->SetDefaultValue("Xe");
  AbsMaterCmd->AvailableForStates(G4State_Idle);


}

///////////////////////////////////////////////////////////////////////////////

F03FieldMessenger::~F03FieldMessenger()
{
  delete StepperCmd;
  delete MagFieldCmd;
  delete MinStepCmd;
  delete F03detDir;
  delete UpdateCmd;

  delete AbsMaterCmd; 
}

////////////////////////////////////////////////////////////////////////////
//
//

void F03FieldMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 
  if( command == StepperCmd )
  { 
    fEMfieldSetup->SetStepperType(StepperCmd->GetNewIntValue(newValue));
  }  
  if( command == UpdateCmd )
  { 
    fEMfieldSetup->UpdateField(); 
  }
  if( command == MagFieldCmd )
  { 
    fEMfieldSetup->SetFieldValue(MagFieldCmd->GetNewDoubleValue(newValue));
    // Check the value
    // fEMfieldSetup->GetConstantFieldValue();
      // GetLocalFieldManager()
    G4cout << "Set field value to " <<     
      fEMfieldSetup->GetConstantFieldValue() / gauss << " Gauss " << G4endl;

  }
  if( command == MinStepCmd )
  { 
    fEMfieldSetup->SetMinStep(MinStepCmd->GetNewDoubleValue(newValue));
  }
}

//
//
/////////////////////////////////////////////////////////////////////////

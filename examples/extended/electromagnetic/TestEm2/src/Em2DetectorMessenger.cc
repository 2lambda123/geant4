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
// $Id: Em2DetectorMessenger.cc,v 1.3 2001-07-11 09:57:37 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "Em2DetectorMessenger.hh"

#include "Em2DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em2DetectorMessenger::Em2DetectorMessenger(Em2DetectorConstruction * Det)
:Em2Detector(Det)
{ 
  Em2detDir = new G4UIdirectory("/calor/");
  Em2detDir->SetGuidance("Em2 detector control.");
      
  MaterCmd = new G4UIcmdWithAString("/calor/setMat",this);
  MaterCmd->SetGuidance("Select Material.");
  MaterCmd->SetParameterName("material",false);
  MaterCmd->AvailableForStates(PreInit,Idle);
  
  LBinCmd = new G4UIcmdWith3Vector("/calor/setLbin",this);
  LBinCmd->SetGuidance("set longitudinal bining");
  LBinCmd->SetGuidance("nb of bins; bin thickness (in radl)");
  LBinCmd->SetParameterName("nLtot","dLradl"," ",true);
  LBinCmd->SetRange("nLtot>=1 && dLradl>0");
  LBinCmd->AvailableForStates(PreInit,Idle);
  
  RBinCmd = new G4UIcmdWith3Vector("/calor/setRbin",this);
  RBinCmd->SetGuidance("set radial bining");
  RBinCmd->SetGuidance("nb of bins; bin thickness (in radl)");
  RBinCmd->SetParameterName("nRtot","dRradl"," ",true);
  RBinCmd->SetRange("nRtot>=1 && dRradl>0");
  RBinCmd->AvailableForStates(PreInit,Idle);
  
  FieldCmd = new G4UIcmdWithADoubleAndUnit("/calor/setField",this);  
  FieldCmd->SetGuidance("Define magnetic field.");
  FieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  FieldCmd->SetParameterName("Bz",false);
  FieldCmd->SetUnitCategory("Magnetic flux density");
  FieldCmd->AvailableForStates(PreInit,Idle);
  
  UpdateCmd = new G4UIcmdWithoutParameter("/calor/update",this);
  UpdateCmd->SetGuidance("Update geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(Idle);          
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em2DetectorMessenger::~Em2DetectorMessenger()
{
  delete MaterCmd;
  delete LBinCmd;
  delete RBinCmd;
  delete FieldCmd;  
  delete UpdateCmd;
  delete Em2detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em2DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == MaterCmd )
   { Em2Detector->SetMaterial(newValue);}
   
  if( command == LBinCmd )
   { Em2Detector->SetLBining(LBinCmd->GetNew3VectorValue(newValue));}
   
  if( command == RBinCmd )
   { Em2Detector->SetRBining(RBinCmd->GetNew3VectorValue(newValue));}
      
  if( command == FieldCmd )
   { Em2Detector->SetMagField(FieldCmd->GetNewDoubleValue(newValue));}
     
  if( command == UpdateCmd )
   { Em2Detector->UpdateGeometry();}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

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
// $Id: Tst14DetectorMessenger.cc,v 1.7 2003-11-06 12:14:06 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#include "Tst14DetectorMessenger.hh"

#include "Tst14DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


Tst14DetectorMessenger::Tst14DetectorMessenger(Tst14DetectorConstruction * Tst14Det)
:Tst14Detector(Tst14Det)
{ 
  Tst14detDir = new G4UIdirectory("/calor/");
  Tst14detDir->SetGuidance("Tst14 detector control.");
      
  AbsMaterCmd = new G4UIcmdWithAString("/calor/setAbsMat",this);
  AbsMaterCmd->SetGuidance("Select Material of the Absorber.");
  AbsMaterCmd->SetParameterName("choice",true);
  AbsMaterCmd->SetDefaultValue("Lead");
  AbsMaterCmd->AvailableForStates(G4State_Idle);
  
  WorldMaterCmd = new G4UIcmdWithAString("/calor/setWorldMat",this);
  WorldMaterCmd->SetGuidance("Select Material of the World.");
  WorldMaterCmd->SetParameterName("wchoice",true);
  WorldMaterCmd->SetDefaultValue("Air");
  WorldMaterCmd->AvailableForStates(G4State_Idle);
  
  AbsThickCmd = new G4UIcmdWithADoubleAndUnit("/calor/setAbsThick",this);
  AbsThickCmd->SetGuidance("Set Thickness of the Absorber");
  AbsThickCmd->SetParameterName("SizeZ",false,false);
  AbsThickCmd->SetDefaultUnit("mm");
  AbsThickCmd->SetRange("SizeZ>0.");
  AbsThickCmd->AvailableForStates(G4State_Idle);
  
  AbsRadCmd = new G4UIcmdWithADoubleAndUnit("/calor/setAbsRad",this);
  AbsRadCmd->SetGuidance("Set radius of the Absorber");
  AbsRadCmd->SetParameterName("SizeR",false,false);
  AbsRadCmd->SetDefaultUnit("mm");
  AbsRadCmd->SetRange("SizeR>0.");
  AbsRadCmd->AvailableForStates(G4State_Idle);
  
  AbsZposCmd = new G4UIcmdWithADoubleAndUnit("/calor/setAbsZpos",this);
  AbsZposCmd->SetGuidance("Set Z pos. of the Absorber");
  AbsZposCmd->SetParameterName("Zpos",false,false);
  AbsZposCmd->SetDefaultUnit("mm");
  AbsZposCmd->AvailableForStates(G4State_Idle);
  
  WorldZCmd = new G4UIcmdWithADoubleAndUnit("/calor/setWorldZ",this);
  WorldZCmd->SetGuidance("Set Z size of the World");
  WorldZCmd->SetParameterName("WSizeZ",false,false);
  WorldZCmd->SetDefaultUnit("mm");
  WorldZCmd->SetRange("WSizeZ>0.");
  WorldZCmd->AvailableForStates(G4State_Idle);
  
  WorldRCmd = new G4UIcmdWithADoubleAndUnit("/calor/setWorldR",this);
  WorldRCmd->SetGuidance("Set R size of the World");
  WorldRCmd->SetParameterName("WSizeR",false,false);
  WorldRCmd->SetDefaultUnit("mm");
  WorldRCmd->SetRange("WSizeR>0.");
  WorldRCmd->AvailableForStates(G4State_Idle);
  
  UpdateCmd = new G4UIcmdWithoutParameter("/calor/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/calor/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false,false);
  MagFieldCmd->SetDefaultUnit("tesla");
  MagFieldCmd->AvailableForStates(G4State_Idle);  

}


Tst14DetectorMessenger::~Tst14DetectorMessenger()
{
  delete AbsMaterCmd; 
  delete AbsThickCmd; 
  delete AbsRadCmd;  
  delete AbsZposCmd; 
  delete WorldMaterCmd;
  delete WorldZCmd;
  delete WorldRCmd;
  delete UpdateCmd;
  delete MagFieldCmd;
  delete Tst14detDir;
}


void Tst14DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == AbsMaterCmd )
   { Tst14Detector->SetAbsorberMaterial(newValue);}
   
  if( command == WorldMaterCmd )
   { Tst14Detector->SetWorldMaterial(newValue);}
   
  if( command == AbsThickCmd )
   { Tst14Detector->SetAbsorberThickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == AbsRadCmd )
   { Tst14Detector->SetAbsorberRadius(AbsRadCmd->GetNewDoubleValue(newValue));}
   
  if( command == AbsZposCmd )
   { Tst14Detector->SetAbsorberZpos(AbsZposCmd->GetNewDoubleValue(newValue));}
   
  if( command == WorldZCmd )
   { Tst14Detector->SetWorldSizeZ(WorldZCmd->GetNewDoubleValue(newValue));}
   
  if( command == WorldRCmd )
   { Tst14Detector->SetWorldSizeR(WorldRCmd->GetNewDoubleValue(newValue));}
   
  if( command == UpdateCmd )
   { Tst14Detector->UpdateGeometry(); }

  if( command == MagFieldCmd )
   { Tst14Detector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}

}

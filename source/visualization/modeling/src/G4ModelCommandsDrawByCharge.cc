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
// $Id: G4ModelCommandsDrawByCharge.cc,v 1.1 2005-11-21 05:44:44 tinslay Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// Jane Tinslay, John Allison, Joseph Perl November 2005

#include "G4ModelCommandsDrawByCharge.hh"
#include "G4TrajectoryDrawByCharge.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UICommand.hh"

//Set colour with string
G4ModelCommandDrawByChargeSet::G4ModelCommandDrawByChargeSet(G4TrajectoryDrawByCharge* model, const G4String& placement)
  :G4VModelCommand<G4TrajectoryDrawByCharge>(model) 
{
  G4String name = model->Name();
  G4String myCommand = placement+"/"+name+"/set";

  fpCommand = new G4UIcmdWithAString(myCommand, this);      
  fpCommand->SetGuidance("Set trajectory colour through a string.");
  fpCommand->SetParameterName("parameters", false); 
}

G4ModelCommandDrawByChargeSet::~G4ModelCommandDrawByChargeSet() {} 

void G4ModelCommandDrawByChargeSet::SetNewValue(G4UIcommand* cmd, G4String newValue) 
{
  G4int charge(-999);
  G4String colour;
  std::istringstream is (newValue);
  is >> charge >> colour;  

  G4TrajectoryDrawByCharge::Charge myCharge = G4TrajectoryDrawByCharge::Positive;

  switch (charge) {
  case 1:
    myCharge = G4TrajectoryDrawByCharge::Positive;
    break;
  case 0:
    myCharge = G4TrajectoryDrawByCharge::Neutral;
    break;
  case -1:
    myCharge = G4TrajectoryDrawByCharge::Negative;
    break;
  default:
    std::ostringstream o;
    o << "Unknown charge "<<charge;
    G4Exception
      ("G4ModelCommandDrawByChargeSet::SetNewValue (G4UIcommand* cmd, G4String newValue) ",
       "UnknownCharge", FatalErrorInArgument, o.str().c_str());
  }
  
  // Configure model as requested
  Model()->Set(myCharge, colour);
}

//Set colour by red, green, blue and alpha components
G4ModelCommandDrawByChargeSetRGBA::G4ModelCommandDrawByChargeSetRGBA(G4TrajectoryDrawByCharge* model, const G4String& placement) 
  :G4VModelCommand<G4TrajectoryDrawByCharge>(model)
{
  G4String name = model->Name();
  G4String myCommand = placement+"/"+name+"/setRGBA";

  fpCommand = new G4UIcmdWithAString(myCommand, this);      
  fpCommand->SetGuidance("Set trajectory colour through red, green, blue and alpha components.");
  fpCommand->SetParameterName("parameters", false); 
}

G4ModelCommandDrawByChargeSetRGBA::~G4ModelCommandDrawByChargeSetRGBA() {} 

void G4ModelCommandDrawByChargeSetRGBA::SetNewValue(G4UIcommand* cmd, G4String newValue) 
{
  G4int charge(-999);
  G4double red(0), green(0), blue(0), alpha(0);
  std::istringstream is (newValue);
  is >> charge >> red >> green >> blue >> alpha;  
  
  G4TrajectoryDrawByCharge::Charge myCharge = G4TrajectoryDrawByCharge::Positive;
  
  switch (charge) {
  case 1:
    myCharge = G4TrajectoryDrawByCharge::Positive;
    break;
  case 0:
    myCharge = G4TrajectoryDrawByCharge::Neutral;
    break;
  case -1:
    myCharge = G4TrajectoryDrawByCharge::Negative;
    break;
  default:
    std::ostringstream o;
    o << "Unknown charge "<<charge;
    G4Exception
      ("G4ModelCommandDrawByChargeSetRGBA::SetNewValue (G4UIcommand* cmd, G4String newValue) ",
       "UnknownCharge", FatalErrorInArgument, o.str().c_str());
  }

  G4Colour myColour(red, green, blue, alpha);
  
  // Configure model as requested
  Model()->Set(myCharge, myColour);
}

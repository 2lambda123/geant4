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
// $Id: G4UIcmdWithABool.cc,v 1.5 2003-06-16 16:55:38 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

#include "G4UIcmdWithABool.hh"
#include <strstream>

G4UIcmdWithABool::G4UIcmdWithABool
(const char * theCommandPath,G4UImessenger * theMessenger)
:G4UIcommand(theCommandPath,theMessenger)
{
  G4UIparameter * blParam = new G4UIparameter('b');
  SetParameter(blParam);
}

G4bool G4UIcmdWithABool::GetNewBoolValue(const char* paramString)
{
  G4String v = paramString;
  v.toUpper();
  G4bool vl = false;
  if( v=="Y" || v=="YES" || v=="1" || v=="T" || v=="TRUE" )
  { vl = true; }
  return vl;
}

G4String G4UIcmdWithABool::ConvertToString(G4bool blValue)
{
  G4String vl = "0";
  if(blValue) vl = "1";
  return vl;
}

void G4UIcmdWithABool::SetParameterName
(const char * theName,G4bool omittable,G4bool currentAsDefault)
{
  G4UIparameter * theParam = GetParameter(0);
  theParam->SetParameterName(theName);
  theParam->SetOmittable(omittable);
  theParam->SetCurrentAsDefault(currentAsDefault);
}

void G4UIcmdWithABool::SetDefaultValue(G4bool defVal)
{
  G4UIparameter * theParam = GetParameter(0);
  theParam->SetDefaultValue(defVal);
}


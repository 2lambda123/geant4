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
// $Id: G4UIcmdWith3VectorAndUnit.cc,v 1.5 2003-06-16 16:55:37 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

#include "G4UIcmdWith3VectorAndUnit.hh"
#include <strstream>

G4UIcmdWith3VectorAndUnit::G4UIcmdWith3VectorAndUnit
(const char * theCommandPath,G4UImessenger * theMessenger)
:G4UIcommand(theCommandPath,theMessenger)
{
  G4UIparameter * dblParamX = new G4UIparameter('d');
  SetParameter(dblParamX);
  G4UIparameter * dblParamY = new G4UIparameter('d');
  SetParameter(dblParamY);
  G4UIparameter * dblParamZ = new G4UIparameter('d');
  SetParameter(dblParamZ);
  G4UIparameter * untParam = new G4UIparameter('s');
  SetParameter(untParam);
  untParam->SetParameterName("Unit");
}

G4ThreeVector G4UIcmdWith3VectorAndUnit::GetNew3VectorValue(const char* paramString)
{
  G4double vx;
  G4double vy;
  G4double vz;
  char unts[30];
  std::istrstream is((char*)paramString);
  is >> vx >> vy >> vz >> unts;
  G4String unt = unts;
  G4double uv = ValueOf(unt);
  return G4ThreeVector(vx*uv,vy*uv,vz*uv);
}

G4ThreeVector G4UIcmdWith3VectorAndUnit::GetNew3VectorRawValue(const char* paramString)
{
  G4double vx;
  G4double vy;
  G4double vz;
  char unts[30];
  std::istrstream is((char*)paramString);
  is >> vx >> vy >> vz >> unts;
  return G4ThreeVector(vx,vy,vz);
}

G4double G4UIcmdWith3VectorAndUnit::GetNewUnitValue(const char* paramString)
{
  G4double vx;
  G4double vy;
  G4double vz;
  char unts[30];
  std::istrstream is((char*)paramString);
  is >> vx >> vy >> vz >> unts;
  G4String unt = unts;
  return ValueOf(unt);
}

G4String G4UIcmdWith3VectorAndUnit::ConvertToString
(G4ThreeVector vec,const char * unitName)
{
  G4String unt = unitName;
  G4double uv = ValueOf(unitName);
  
  char st[100];
  std::ostrstream os(st,100);
  os << vec.x()/uv << " " << vec.y()/uv << " " << vec.z()/uv
     << " " << unitName << '\0';
  G4String vl = st;
  return vl;
}

void G4UIcmdWith3VectorAndUnit::SetParameterName
(const char * theNameX,const char * theNameY,const char * theNameZ,
G4bool omittable,G4bool currentAsDefault)
{
  G4UIparameter * theParamX = GetParameter(0);
  theParamX->SetParameterName(theNameX);
  theParamX->SetOmittable(omittable);
  theParamX->SetCurrentAsDefault(currentAsDefault);
  G4UIparameter * theParamY = GetParameter(1);
  theParamY->SetParameterName(theNameY);
  theParamY->SetOmittable(omittable);
  theParamY->SetCurrentAsDefault(currentAsDefault);
  G4UIparameter * theParamZ = GetParameter(2);
  theParamZ->SetParameterName(theNameZ);
  theParamZ->SetOmittable(omittable);
  theParamZ->SetCurrentAsDefault(currentAsDefault);
}

void G4UIcmdWith3VectorAndUnit::SetDefaultValue(G4ThreeVector vec)
{
  G4UIparameter * theParamX = GetParameter(0);
  theParamX->SetDefaultValue(vec.x());
  G4UIparameter * theParamY = GetParameter(1);
  theParamY->SetDefaultValue(vec.y());
  G4UIparameter * theParamZ = GetParameter(2);
  theParamZ->SetDefaultValue(vec.z());
}

void G4UIcmdWith3VectorAndUnit::SetUnitCategory(const char * unitCategory)
{
  SetUnitCandidates(UnitsList(unitCategory));
}

void G4UIcmdWith3VectorAndUnit::SetUnitCandidates(const char * candidateList)
{
  G4UIparameter * untParam = GetParameter(3);
  G4String canList = candidateList;
  untParam->SetParameterCandidates(canList);
}

void G4UIcmdWith3VectorAndUnit::SetDefaultUnit(const char * defUnit)
{
  G4UIparameter * untParam = GetParameter(3);
  untParam->SetOmittable(true);
  untParam->SetDefaultValue(defUnit);
  SetUnitCategory(CategoryOf(defUnit));
}


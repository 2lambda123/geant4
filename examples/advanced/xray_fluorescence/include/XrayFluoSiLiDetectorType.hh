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
// $Id: XrayFluoSiLidetectorType.hh
// GEANT4 tag $Name:
//
// Author: Alfonso Mantero (Alfonso.Mantero@ge.infn.it)
//
// History:
// -----------
//  19 Jun 2003  Alfonso Mantero Created
//
// -------------------------------------------------------------------

#ifndef XrayFluoSiLidetectorType_hh
#define XrayFluoSiLidetectorType_hh 1

#include "globals.hh"
#include "g4std/map"
#include "G4DataVector.hh"
#include "XrayFluoDataSet.hh"
#include "XrayFluoVDetectorType.hh"

//class XrayFluoDataSet;
//class G4DataVector;

class XrayFluoSiLiDetectorType : public XrayFluoVDetectorType
{
public:
  
  ~XrayFluoSiLiDetectorType();
  
  //returns an unique instance of this class (singleton)
  static XrayFluoSiLiDetectorType* GetInstance();

  //gives the material (a string) of wich is made the detector
  G4String GetDetectorMaterial();
  
  // Given the energy depositon in the detector, returns the measured value,
  // according the energy definition power of the detector
  G4double ResponseFunction(G4double);
  
  //returns a random value of the energy measured according to the tabulated
  //peack just lower of the energy deposited
  G4double GetInfData(G4double,G4double,G4int);
  
  //returns a random value of the energy measured according to the tabulated
  //peack just higher of the energy deposited
  G4double GetSupData(G4double,G4double,G4int);

  //loads the data tabulated for the response and the efficiency data from file
  void LoadResponseData(G4String);
  void LoadEfficiencyData(G4String);
  
private:

  
  XrayFluoSiLiDetectorType();

  static XrayFluoSiLiDetectorType* instance;

  G4String detectorMaterial;

  //stores the data of the efficience of the detector
  const XrayFluoDataSet* efficiencySet;

  //stores the energy data (first column of the file) of the 
  //response function 
  G4std::map<G4int,G4DataVector*,G4std::less<G4int> > energyMap;
  
  //stores the values (second column of the file) of the 
  //response function 
  G4std::map<G4int,G4DataVector*,G4std::less<G4int> > dataMap;

};
#endif

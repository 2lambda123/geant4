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
// $Id: G4NeutronHPIsoData.hh,v 1.10 2002-12-12 19:18:13 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4NeutronHPIsoData_h
#define G4NeutronHPIsoData_h 1

 // Hadronic Process: Very Low Energy Neutron X-Sections
 // original by H.P. Wellisch, TRIUMF, 14-Feb-97
 // Has the Cross-section data for on isotope.
 
#include "globals.hh"
#include "G4ios.hh"
#include "g4std/fstream"
#include "g4std/strstream"
#include <stdlib.h>
#include "G4NeutronHPVector.hh"
#include "G4NeutronHPNames.hh"

class G4NeutronHPIsoData
{
public:

  G4NeutronHPIsoData()
  {
    theChannelData = NULL;
    theFissionData = NULL;
    theCaptureData = NULL;
    theElasticData = NULL;
    theInelasticData = NULL;
  }
  
  ~G4NeutronHPIsoData(){if(theChannelData!=NULL) delete theChannelData;}
  
  inline G4double GetXsec(G4double energy)
  {
    return G4std::max(0., theChannelData->GetXsec(energy));
  }
  G4bool Init(G4int A, G4int Z, G4double abun, G4String dirName, G4String aFSType);
  
  void Init(G4int A, G4int Z, G4double abun); //fill PhysicsVector for this Isotope
  
  inline G4NeutronHPVector * MakeElasticData()
    {return theElasticData;}
  inline G4NeutronHPVector * MakeFissionData()
    {return theFissionData;}
  inline G4NeutronHPVector * MakeCaptureData()
    {return theCaptureData;}
  inline G4NeutronHPVector * MakeInelasticData()
    {return theInelasticData;}
  inline G4NeutronHPVector * MakeChannelData()
    {return theChannelData;}

  G4String GetName(G4int A, G4int Z, G4String base, G4String rest);
  
  inline void FillChannelData(G4NeutronHPVector * aBuffer)
  {
    if(theChannelData!=NULL) G4Exception("IsoData has channel full already!!!");
    theChannelData = new G4NeutronHPVector;
    for(G4int i=0; i<aBuffer->GetVectorLength(); i++)
    {
      theChannelData->SetPoint(i, aBuffer->GetPoint(i));
    }
  }
  
  inline void ThinOut(G4double precision)
  {
    if(theFissionData) theFissionData->ThinOut(precision);
    if(theCaptureData) theCaptureData->ThinOut(precision);
    if(theElasticData) theElasticData->ThinOut(precision);
    if(theInelasticData) theInelasticData->ThinOut(precision);
  }
  
private:

  G4NeutronHPVector * theFissionData;
  G4NeutronHPVector * theCaptureData;
  G4NeutronHPVector * theElasticData;
  G4NeutronHPVector * theInelasticData;
  G4NeutronHPVector * theChannelData;

  G4String theFileName;
  G4NeutronHPNames theNames;
};

#endif

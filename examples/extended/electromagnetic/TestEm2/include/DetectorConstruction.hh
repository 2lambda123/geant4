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
// $Id: DetectorConstruction.hh,v 1.4 2004-05-21 18:21:49 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"

class G4Tubs;
class G4LogicalVolume;
class G4UniformMagField;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
   ~DetectorConstruction();

  public:

     void SetMaterial(const G4String&);
     void SetLBining (G4ThreeVector);
     void SetRBining (G4ThreeVector);
     void SetMagField(G4double);

     G4VPhysicalVolume* Construct();

     void UpdateGeometry();

     const
     G4VPhysicalVolume* GetEcal() {return physiEcal;};
     G4Material*    GetMaterial() {return myMaterial;};

     // Subdivision of absorber
     G4int    GetnLtot()          {return nLtot;};
     G4int    GetnRtot()          {return nRtot;};
     G4double GetdLradl()         {return dLradl;};
     G4double GetdRradl()         {return dRradl;};
     G4double GetfullLength()     {return EcalLength;};
     G4double GetfullRadius()     {return EcalRadius;};

     // Acceptance parameters
     void     SetEdepAndRMS(G4ThreeVector);
     G4double GetAverageEdep() const    {return edeptrue;};
     G4double GetRMSEdep() const        {return rmstrue;};
     G4double GetLimitEdep() const      {return limittrue;};

     // Histogram name and type
     void SetHistoName(G4String& val)   {histoName = val;};
     void SetHistoType(G4String& val)   {histoType = val;};
     const G4String& HistoName() const  {return histoName;};
     const G4String& HistoType() const  {return histoType;};

  private:

     void DefineMaterials();
     G4VPhysicalVolume* ConstructVolumes();

     G4int    nLtot,  nRtot;          // nb of bins: longitudinal and radial
     G4double dLradl, dRradl;         // bin thickness (in radl unit)

     G4Material* myMaterial;          //pointer to the material
     G4UniformMagField* magField;     //pointer to the mag field

     G4double EcalLength;             //full length of the Calorimeter
     G4double EcalRadius;             //radius  of the Calorimeter

     G4Tubs*            solidEcal;    //pointer to the solid calorimeter
     G4LogicalVolume*   logicEcal;    //pointer to the logical calorimeter
     G4VPhysicalVolume* physiEcal;    //pointer to the physical calorimeter

     G4Tubs*            solidSlice;   //pointer to the solid  L-slice
     G4LogicalVolume*   logicSlice;   //pointer to the logical L-slide
     G4VPhysicalVolume* physiSlice;   //pointer to the physical L-slide

     G4Tubs*            solidRing;    //pointer to the solid  R-slice
     G4LogicalVolume*   logicRing;    //pointer to the logical R-slide
     G4VPhysicalVolume* physiRing;    //pointer to the physical R-slide

     DetectorMessenger* detectorMessenger;  //pointer to the Messenger

     G4double           edeptrue;
     G4double           rmstrue;
     G4double           limittrue;
     G4String           histoName;
     G4String           histoType;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


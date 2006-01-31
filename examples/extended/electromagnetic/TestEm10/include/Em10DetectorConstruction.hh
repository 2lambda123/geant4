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
// $Id: Em10DetectorConstruction.hh,v 1.9 2006-01-31 10:06:25 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#ifndef Em10DetectorConstruction_h
#define Em10DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ios.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
class G4UniformMagField;
class Em10DetectorMessenger;
class Em10CalorimeterSD;
class G4Region;
class Em10Materials;


class Em10DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    Em10DetectorConstruction();
   ~Em10DetectorConstruction();

  public:
       
     
     void SetAbsorberMaterial (G4String);     
     void SetAbsorberThickness(G4double);     
     void SetAbsorberRadius(G4double);                
     void SetAbsorberZpos(G4double);

     void SetRadiatorMaterial (G4String);     
     void SetRadiatorThickness(G4double);
 
     void SetGasGapThickness(G4double);     
   
     void SetFoilNumber (G4int    i)  {fFoilNumber = i;  };     
     void SetAlphaPlate (G4double val){fAlphaPlate = val;};     
     void SetAlphaGas   (G4double val){fAlphaGas   = val;};     

     void SetWorldMaterial(G4String);
     void SetWorldSizeZ(G4double);
     void SetWorldSizeR(G4double);

  //  void SetGammaCut(G4double    cut){fGammaCut    = cut;};
  // void SetElectronCut(G4double cut){fElectronCut = cut;};
  //  void SetPositronCut(G4double cut){fPositronCut = cut;};



     void SetMagField(G4double);
     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     
  public:
  
     void PrintCalorParameters(); 
                    
     G4Material* GetWorldMaterial()    {return WorldMaterial;};
     G4double GetWorldSizeZ()          {return WorldSizeZ;}; 
     G4double GetWorldSizeR()          {return WorldSizeR;};
     
     G4double GetAbsorberZpos()        {return zAbsorber;}; 
     G4double GetzstartAbs()           {return zstartAbs;};
     G4double GetzendAbs()             {return zendAbs;};

     G4Material* GetAbsorberMaterial()  {return AbsorberMaterial;};
     G4double    GetAbsorberThickness() {return AbsorberThickness;};      
     G4double GetAbsorberRadius()       {return AbsorberRadius;};
     
     const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           
     const G4VPhysicalVolume* GetAbsorber()   {return physiAbsorber;};
     G4LogicalVolume* GetLogicalAbsorber()    {return logicAbsorber;};

     G4LogicalVolume* GetLogicalRadiator()    {return logicRadiator;};
     G4double         GetFoilThick()          {return fRadThickness;};      
     G4double         GetGasThick()           {return fGasGap;};      
     G4int            GetFoilNumber()         {return fFoilNumber;};      
     G4Material* GetFoilMaterial()  {return fFoilMat;};
     G4Material* GetGasMaterial()  {return fGasMat;};
      
private:
    
  void ComputeCalorParameters();
  G4VPhysicalVolume* ConstructDetectorXTR(); 

    
  G4VPhysicalVolume* SimpleSetUpALICE();     
                
private:
     
  G4bool             worldchanged;
  G4Material*        AbsorberMaterial;
  G4double           AbsorberThickness;
  G4double           AbsorberRadius;

  G4Material*        fWindowMat ;
  G4double           fWindowThick ;

  G4Material*        fElectrodeMat ;
  G4double           fElectrodeThick ;

  G4Material*        fGapMat ;
  G4double           fGapThick ;

  G4double           fAlphaPlate ;
  G4double           fAlphaGas ;

 
  G4double           zAbsorber ;
  G4double           zstartAbs , zendAbs ;
     
  G4Material*        WorldMaterial;
  G4double           WorldSizeR;
  G4double           WorldSizeZ;
            
  G4Box*             solidWorld;    //pointer to the solid World 
  G4LogicalVolume*   logicWorld;    //pointer to the logical World
  G4VPhysicalVolume* physiWorld;    //pointer to the physical World

  // TR radiator volumes and dimensions
          
  G4Box*             fSolidRadSlice;   // pointer to the solid  z-slice 
  G4LogicalVolume*   fLogicRadSlice;   // pointer to the logical z-slide
  G4VPhysicalVolume* fPhysicRadSlice;  // pointer to the physical z-slide

  G4Box*             fSolidRadRing;    // pointer to the solid  R-slice 
  G4LogicalVolume*   fLogicRadRing;    // pointer to the logical R-slide
  G4VPhysicalVolume* fPhysicRadRing;   // pointer to the physical R-slide

  G4Box*             solidRadiator;
  G4LogicalVolume*   logicRadiator; 
  G4VPhysicalVolume* physiRadiator;

  G4Material* fRadiatorMat;        // pointer to the mixed TR radiator material
  G4Material* fFoilMat;            // pointer to the TR foil radiator material
  G4Material* fGasMat;             // pointer to the TR gas radiator material

  G4double fRadThickness ;
  G4double fGasGap       ;
  G4double foilGasRatio  ;

  G4int fFoilNumber ;
  G4int fModelNumber ; // selection of parametrisation model1-10

  G4double fDetThickness ;
  G4double fDetLength    ;
  G4double fDetGap       ;

  G4double fStartR       ;
  G4double fStartZ       ;

  G4int fModuleNumber ;   // the number of Rad-Det modules

  G4Box*             solidAbsorber; //pointer to the solid Absorber
  G4LogicalVolume*   logicAbsorber; //pointer to the logical Absorber
  G4VPhysicalVolume* physiAbsorber; //pointer to the physical Absorber
     
  G4UniformMagField* magField;      //pointer to the magnetic field

  // G4double fElectronCut, fGammaCut, fPositronCut;
       
  Em10DetectorMessenger* detectorMessenger;  //pointer to the Messenger
  Em10CalorimeterSD*     calorimeterSD;  //pointer to the sensitive detector
  G4Region*             fRegGasDet;
  G4Region*             fRadRegion;
  Em10Materials*        fMat;  

};

////////////////////////////////////////////////////////////////////////

inline void Em10DetectorConstruction::ComputeCalorParameters()
{
  // Compute derived parameters of the calorimeter
     if(!worldchanged)
     {
       //  WorldSizeR=2.*AbsorberRadius ;
       //  WorldSizeZ=2.*AbsorberThickness ;
     }
     
     zstartAbs = zAbsorber-0.5*AbsorberThickness; 
     zendAbs   = zAbsorber+0.5*AbsorberThickness; 

}

#endif








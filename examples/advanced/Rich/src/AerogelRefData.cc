// Rich advanced example for Geant4
// AerogelRefData.cc for Rich of LHCb
// History:
// Created: Sajan Easo (Sajan.Easo@cern.ch)
// Revision and changes: Patricia Mendez (Patricia.Mendez@cern.ch)
/////////////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <fstream.h>
#include "AerogelRefData.hh"
#include "RichTbGeometryParameters.hh"
#include "RichTbMaterialParameters.hh" 
AerogelRefData::AerogelRefData(G4String AgelRefIndFile)
   :NumberOfRefIndBins(NumAerogelRefIndexPhotonEnergyBins),
    StdAerogelRefphotE(vector<G4double>(NumAerogelRefIndexPhotonEnergyBins)),
    StdAerogelRefIndexValue(vector<G4double>(NumAerogelRefIndexPhotonEnergyBins)),
    AerogelRefIndShift(vector<G4double>( MaxNumberOfAerogelTypes)),
 AerogelWavelengthRef(vector<G4double>( MaxNumberOfAerogelTypes)) {
  
  AerogelRefIndexFileName=AgelRefIndFile;

  StdAerogelNominalRefractiveIndex= StdAerogelNominalRefIndex;

  ReadStdAerogelRefIndex();
  G4double AerogelRefEnergy,AerogelReferenceRefInd;
  for(G4int ityp=0; ityp< MaxNumberOfAerogelTypes; ityp++ ){
  AerogelWavelengthRef[ityp]=AerogelReferenceRefIndWavelength[ityp];
  AerogelRefEnergy= 
          (PhotMomWaveConv/(AerogelWavelengthRef[ityp]/nanometer))*eV;

  AerogelReferenceRefInd = GetStdAgelRefIndexE(AerogelRefEnergy);
  AerogelRefIndShift[ityp] =  AerogelReferenceRefInd -
    StdAerogelNominalRefractiveIndex ;
  }

}
G4double AerogelRefData::GetStdAgelRefIndexE(G4double  StdAgelRefEnergy) {

  G4double refint= StdAerogelNominalRefractiveIndex;
  if(StdAgelRefEnergy <= StdAerogelRefphotE[0] ) {
    refint= StdAerogelRefphotE[0];
    G4cout<<"Ref index reference value outside bounds "<<G4endl;

  } 
  if(StdAgelRefEnergy > StdAerogelRefphotE[ NumberOfRefIndBins-1 ] ) {
    refint= StdAerogelRefphotE[ NumberOfRefIndBins-1 ];
    G4cout<<"Ref index reference value outside bounds "<<G4endl;

  } 

  for(G4int iabin=0; iabin < NumberOfRefIndBins-1 ; iabin++ ){

    G4double E1= StdAerogelRefphotE[iabin];
    G4double E2= StdAerogelRefphotE[iabin+1];

    if(StdAgelRefEnergy >  E1 && StdAgelRefEnergy <=  E2 && E1 != E2 ){
       
      //Now interpolate
      G4double r1= StdAerogelRefIndexValue[iabin];
      G4double r2= StdAerogelRefIndexValue[iabin+1];
      G4double slp=(r2-r1)/(E2-E1);
      G4double ain= r1-slp*E1;
      refint= slp*StdAgelRefEnergy + ain;
      break; 
    }
   
  } 

  return refint;

}
void AerogelRefData::ReadStdAerogelRefIndex() {
 
      const char* StdAerogelRefIndFile= AerogelRefIndexFileName.c_str();
      if( StdAerogelRefIndFile == 0 ){ 
	G4cout<<" Aerogel RefIndex Input file missing" 
              << " Please provide the correct file name "
              <<G4endl; 
      }else { 

      G4cout<<"Now reading Std Aerogel Ref Index from "
	    <<StdAerogelRefIndFile<<G4endl; 
      ifstream finpga(StdAerogelRefIndFile);
      G4double ephot,rind;
      for (G4int fbin=0; fbin< NumberOfRefIndBins  ; fbin++){
      finpga>>ephot;
      finpga>>rind;
      StdAerogelRefphotE[fbin]=ephot*eV;
      StdAerogelRefIndexValue[fbin]=rind;
      //for test avoid the variation in ref index. SE 18-10-01
      // StdAerogelRefIndexValue[fbin]= StdAerogelNominalRefractiveIndex;

      }
      
      }

}

vector<G4double> AerogelRefData::GetCurAerogelRefIndValueVect (G4int AerogelTnum ) {
  // no  test for tdr rich. now put back to TB conditions.
  vector<G4double>CAgel(NumberOfRefIndBins);
  AerogelType Ctype;
  G4double CRn = GetRefnominal( AerogelTnum );
  for (G4int ib=0; ib < NumberOfRefIndBins; ib++ ){

        CAgel[ib]= StdAerogelRefIndexValue[ib] 
      - StdAerogelNominalRefractiveIndex-AerogelRefIndShift[ AerogelTnum] + CRn;

  }

  return CAgel;

}

G4double AerogelRefData:: GetCurAerogelRefIndValue( G4int rbinw , G4int AerogelTnum ){
  // no test for tdr. now in TB conditions.
 G4double crn=  GetRefnominal( AerogelTnum );
  return StdAerogelRefIndexValue[rbinw]
       - StdAerogelNominalRefractiveIndex-AerogelRefIndShift[ AerogelTnum] +crn;
  //   - StdAerogelNominalRefractiveIndex-AerogelRefIndShift[ AerogelTnum] +crn;
  // return StdAerogelRefIndexValue[rbinw]-1.03269+crn;
}

G4double AerogelRefData::GetRefnominal(G4int AgelTnum ) {

  G4double CRnominal = StdAerogelNominalRefractiveIndex;
  if(AgelTnum == 0 ){
  
    CRnominal= AerogelTypeANominalRefIndex;
  }else if (AgelTnum == 1 ) {
   
    CRnominal= AerogelTypeBNominalRefIndex;
  }else if  (AgelTnum == 2 ) {
 
    CRnominal= AerogelTypeCNominalRefIndex;
  }else if  (AgelTnum == 3 ) {
  
    CRnominal= AerogelTypeDNominalRefIndex;
  }else if  (AgelTnum == 4 ) {

    CRnominal= AerogelTypeENominalRefIndex;

  } else {G4cout<<"Unknown Aerogel Type "<<G4endl; }

}
AerogelType AerogelRefData::GetAerogelType(G4int AgelTnum ) {
  AerogelType Ctype;

  if(AgelTnum == 0 ){
    Ctype=AerogelTypeA;

  }else if (AgelTnum == 1 ) {
    Ctype=AerogelTypeB;

  }else if  (AgelTnum == 2 ) {
    Ctype=AerogelTypeC;

  }else if  (AgelTnum == 3 ) {
    Ctype=AerogelTypeD;

  }else if  (AgelTnum == 4 ) {
    Ctype=AerogelTypeE;


  } else {G4cout<<"Unknown Aerogel Type "<<G4endl; }

}
AerogelRefData::~AerogelRefData(){ ; }










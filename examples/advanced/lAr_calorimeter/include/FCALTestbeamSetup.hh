//----------------------------------------------------------------------------
//
//   Name of file:      FCALFrontVolumes.hh
//   Author:            Mathieu Fontaine           Rachid Mazini
//                      fontaine@lps.umontreal.ca  Rachid.Mazini@cern.ch
//   Language:          C++
//   Tested on:         g++
//   Prerequisites:     None
//   Purpose:           Header file for FCALFrontVolume.cc, which defines
//                      the  volumes in the testbeam front.
//   Developped:        10-March-2000   M.F.
//
//----------------------------------------------------------------------------


#ifndef FCALTestbeamSetup_h
#define FCALTestbeamSetup_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "FCALTestbeamSetupSD.hh"

//class FCALFrontVolumes;
//class FCALTailVolumes;
//class FCALCryostatVolumes;

class G4LogicalVolume;
class G4VPhysicalVolume;

class FCALTestbeamSetup : public G4VUserDetectorConstruction
{

public:

  FCALTestbeamSetup();
  ~FCALTestbeamSetup();

public:

  G4VPhysicalVolume* Construct();

private:

  G4double MotherSizeX, MotherSizeY, MotherSizeZ;

  G4double MWPCSizeX, MWPCSizeY, MWPCSizeZ;
  G4double MWPCPosX, MWPCPosY, MWPCPosZ[5];

  G4double ScintS1andS3SizeX, ScintS1andS3SizeY, ScintS1andS3SizeZ;
  G4double ScintS2SizeX, ScintS2SizeY, ScintS2SizeZ;
  G4double ScintS1_S3PosX, ScintS1_S3PosY, ScintS1PosZ,ScintS2PosZ, ScintS3PosZ;

  G4double HoleCntrSizeX, HoleCntrSizeY, HoleCntrScintSizeZ, HoleCntrAbsrbSizeZ;
  G4double HoleCntrScintPosX, HoleCntrScintPosY, HoleCntrScintPosZ;
  G4double HoleCntrPbPosX, HoleCntrPbPosY, HoleCntrPbPosZ;
  G4double HoleCntrAlPosX, HoleCntrAlPosY, HoleCntrAlPosZ;
  G4double ScintHoleRmin, ScintHoleRmax, ScintHoleLength;
  G4double AbsrbHoleRmin, AbsrbHoleRmax, AbsrbHoleLength;
  G4double HoleStartPhi, HoleDPhi;
  G4double HolePosX, HolePosY, HolePosZ;

  G4double LeadWallSizeX, LeadWallSizeY, LeadWallSizeZ;
  G4double LeadWallSlitSizeX, LeadWallSlitSizeY, LeadWallSlitSizeZ;
  G4double LeadWallPosX,LeadWallPosY, LeadWallPosZ;

  G4double IronWallSizeX, IronWallSizeY, IronWallSizeZ;
  G4double IronWallSlitSizeX, IronWallSlitSizeY, IronWallSlitSizeZ;
  G4double IronWallPosX,IronWallPosY, IronWallPosZ;

  G4int NBigScint, NSmallScint, NBigIron, NSmallIron;
  G4double BigScintSizeX, BigScintSizeY, SmallScintSizeX, SmallScintSizeY, ScintSizeZ;
  G4double ScintPosX, ScintPosY, ScintPosZ[7];
  G4double BigIronSizeX, BigIronSizeY, SmallIronSizeX, SmallIronSizeY, IronSizeZ;
  G4double IronPosX, IronPosY, IronPosZ[6];

  G4double ConcWallSizeX, ConcWallSizeY, ConcWallSizeZ; 
  G4double ConcWallPosX, ConcWallPosY, ConcWallAPosZ, ConcWallBPosZ;
  G4double ConcWallInsSizeX, ConcWallInsSizeY, ConcWallInsSizeZ;
  G4double ConcWallInsPosX, ConcWallInsPosY, ConcWallInsPosZ;

  G4double MuCntrSIzeX, MuCntrSIzeY, MuCntrSIzeZ;
  G4double MuCntrPosX, MuCntrPosY, MuCntrPosZ;
  

  G4double CryostatPosX, CryostatPosY, CryostatPosZ;


  FCALTestbeamSetupSD* FcalTBSetupSD;  // Senstive detector

  /* 
  G4double TailPosX;
  G4double TailPosY;
  G4double TailPosZ;

  G4double FrontPosX;
  G4double FrontPosY;
  G4double FrontPosZ;
  */
  /*
  G4double EMModulePosX;
  G4double EMModulePosY;
  G4double EMModulePosZ;

  G4double HadModulePosX;
  G4double HadModulePosY;
  G4double HadModulePosZ;
  */

};

#endif   /* FCALTestbeamSetup.hh */

// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// **********************************************************************
// *                                                                    *
// *                    GEANT 4 xray_telescope advanced example         *
// *                                                                    *
// * MODULE:            exrdmPrimaryGeneratorAction.hh                *
// * -------                                                            *
// *                                                                    *
// * Version:           0.5                                             *
// * Date:              15/11/00                                        *
// * Author:            F.Lei                                           *
// * Organisation:      DERA, UK                                        *
// *                                                                    *
// **********************************************************************
// 
// CHANGE HISTORY
// --------------
//
// 15.11.2000 F.Lei
// - New version of PrimaryGeneratorAction using the GPS insead of the
//   standard particle gun.  
// - The PrimaryGeneratorMessenger is no longer needed.
//
// 06.11.2000 R.Nartallo
// - First implementation of X-ray Telescope advanced example.
// - Based on Chandra and XMM models
//
//
// **********************************************************************

#ifndef ZIIIPrimaryGeneratorAction_h
#define ZIIIPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;
class ZIIIDetectorConstruction;

class ZIIIPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  ZIIIPrimaryGeneratorAction(ZIIIDetectorConstruction*);
  ~ZIIIPrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event* anEvent);

private:
  G4GeneralParticleSource* particleGun;
    ZIIIDetectorConstruction*    ZIIIDetector;  //pointer to the geometry
    G4String                      rndmFlag;	  //flag for a random impact point         
};

#endif




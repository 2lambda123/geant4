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
// $Id: G4PhotoClusterModel.hh,v 1.2 2004-11-30 10:27:37 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
///////////////////////////////////////////////////////////////////////////
// 
// Class for 'fast' parametrisation model describing X-ray ionisation clusters
// created in some G4Envelope. 
// 
// History:
// 14.07.00 V. Grichine first version 
//


#ifndef G4PhotoClusterModel_h
#define G4PhotoClusterModel_h 1


#include "globals.hh"

#include "G4VClusterModel.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"


class G4PhotoClusterModel : public G4VClusterModel
{
public:

   G4PhotoClusterModel (G4LogicalVolume* anEnvelope);

  ~G4PhotoClusterModel ();

  // Pure virtual functions from base class

  G4bool IsApplicable(const G4ParticleDefinition&);
 
  G4bool ModelTrigger(const G4FastTrack &);
 
  void DoIt(const G4FastTrack&, G4FastStep&)  ;

protected:

  void     ComputePhotoAbsCof() ;
  G4double GetLinearPhotoAbs(G4double) ;



private:

  G4int      fMatIndex         ;
  G4double** fPhotoAbsCof    ;
  G4int      fIntervalNumber ;
  
};

#endif






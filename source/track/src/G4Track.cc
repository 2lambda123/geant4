//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4Track.cc,v 1.35 2010-09-21 00:33:05 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//---------------------------------------------------------------
//
//  G4Track.cc
//
//---------------------------------------------------------------
//   Add copy constructor            Hisaya Feb. 07 01
//   Fix GetVelocity                 Hisaya Feb. 17 01
//   Modification for G4TouchableHandle             22 Oct. 2001  R.Chytracek//
//   Fix GetVelocity (bug report #741)   Horton-Smith Apr 14 2005
//   Remove massless check in  GetVelocity   02 Apr. 09 H.Kurashige

#include "G4Track.hh"
#include "G4ParticleTable.hh"

#include <iostream>
#include <iomanip>

G4Allocator<G4Track> aTrackAllocator;

G4PhysicsLogVector* G4Track::velTable = 0;

G4double G4Track::maxT = 1000.0;
G4double G4Track::minT = 0.0001;
G4int    G4Track::NbinT = 500;

///////////////////////////////////////////////////////////
G4Track::G4Track(G4DynamicParticle* apValueDynamicParticle,
                 G4double aValueTime,
                 const G4ThreeVector& aValuePosition)
///////////////////////////////////////////////////////////
  : fCurrentStepNumber(0),    fPosition(aValuePosition),
    fGlobalTime(aValueTime),  fLocalTime(0.),
    fTrackLength(0.),
    fParentID(0),             fTrackID(0),
    fpDynamicParticle(apValueDynamicParticle),
    fTrackStatus(fAlive),
    fBelowThreshold(false),   fGoodForTracking(false),
    fStepLength(0.0),         fWeight(1.0),
    fpStep(0),
    fVtxKineticEnergy(0.0),
    fpLVAtVertex(0),          fpCreatorProcess(0),
    fpUserInformation(0),
    prev_mat(0),  groupvel(0),
    prev_velocity(0.0), prev_momentum(0.0),
    is_OpticalPhoton(false)
{    
  static G4bool isFirstTime = true;
  static G4ParticleDefinition* fOpticalPhoton =0;
  if ( isFirstTime ) {
    isFirstTime = false;
    // set  fOpticalPhoton
    fOpticalPhoton = G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");
  }
  // check if the particle type is Optical Photon
  is_OpticalPhoton = (fpDynamicParticle->GetDefinition() == fOpticalPhoton);

  if (velTable==0) PrepareVelocityTable();
}

//////////////////
G4Track::G4Track()
//////////////////
  : fCurrentStepNumber(0),    
    fGlobalTime(0),           fLocalTime(0.),
    fTrackLength(0.),
    fParentID(0),             fTrackID(0),
    fpDynamicParticle(0),
    fTrackStatus(fAlive),
    fBelowThreshold(false),   fGoodForTracking(false),
    fStepLength(0.0),         fWeight(1.0),
    fpStep(0),
    fVtxKineticEnergy(0.0),
    fpLVAtVertex(0),          fpCreatorProcess(0),
    fpUserInformation(0),
    prev_mat(0),  groupvel(0),
    prev_velocity(0.0), prev_momentum(0.0),
    is_OpticalPhoton(false) 
{
}
//////////////////
G4Track::G4Track(const G4Track& right)
//////////////////
  : fCurrentStepNumber(0),    
    fGlobalTime(0),           fLocalTime(0.),
    fTrackLength(0.),
    fParentID(0),             fTrackID(0),
    fpDynamicParticle(0),
    fTrackStatus(fAlive),
    fBelowThreshold(false),   fGoodForTracking(false),
    fStepLength(0.0),         fWeight(1.0),
    fpStep(0),
    fVtxKineticEnergy(0.0),
    fpLVAtVertex(0),          fpCreatorProcess(0),
    fpUserInformation(0),
    prev_mat(0),  groupvel(0),
    prev_velocity(0.0), prev_momentum(0.0),
    is_OpticalPhoton(false) 
{
  *this = right;
}

///////////////////
G4Track::~G4Track()
///////////////////
{
   delete fpDynamicParticle;
   delete fpUserInformation;
}

//////////////////
G4Track & G4Track::operator=(const G4Track &right)
//////////////////
{
  if (this != &right) {
   fPosition = right.fPosition;
   fGlobalTime = right.fGlobalTime;
   fLocalTime = right.fLocalTime;
   fTrackLength = right.fTrackLength;
   fWeight = right.fWeight;
   fStepLength = right.fStepLength;

   // Track ID (and Parent ID) is not copied and set to zero for new track
   fTrackID = 0;
   fParentID =0;

   // CurrentStepNumber is set to be 0
   fCurrentStepNumber = 0;

   // dynamic particle information 
   fpDynamicParticle = new G4DynamicParticle(*(right.fpDynamicParticle));
 
   // track status and flags for tracking  
   fTrackStatus = right.fTrackStatus;
   fBelowThreshold = right.fBelowThreshold;
   fGoodForTracking = right.fGoodForTracking;
   
   // Step information (Step Length, Step Number, pointer to the Step,) 
   // are not copied
   fpStep=0;

   // vertex information
   fVtxPosition = right.fVtxPosition;
   fpLVAtVertex = right.fpLVAtVertex;
   fVtxKineticEnergy = right.fVtxKineticEnergy;
   fVtxMomentumDirection = right.fVtxMomentumDirection;

   // CreatorProcess and UserInformation are not copied 
   fpCreatorProcess = 0;
   fpUserInformation = 0;

   prev_mat = right.prev_mat;
   groupvel = right.groupvel;
   prev_velocity = right.prev_velocity;
   prev_momentum = right.prev_momentum;

   is_OpticalPhoton = right.is_OpticalPhoton; 

  }
  return *this;
}

///////////////////
void G4Track::CopyTrackInfo(const G4Track& right)
//////////////////
{
  *this = right;
}

///////////////////
G4double G4Track::GetVelocity() const
///////////////////
{ 
    
  G4double velocity = c_light ;
  
  G4double mass = fpDynamicParticle->GetMass();

  // special case for photons
  if ( is_OpticalPhoton ) {

    G4Material* mat=0; 
    G4bool update_groupvel = false;
    if ( this->GetStep() ){
      mat= this->GetMaterial();         //   Fix for repeated volumes
    }else{
      if (fpTouchable!=0){ 
        mat=fpTouchable->GetVolume()->GetLogicalVolume()->GetMaterial();
      }
    }
    // check if previous step is in the same volume
    //  and get new GROUPVELOCITY table if necessary 
    if ((mat != 0) && ((mat != prev_mat)||(groupvel==0))) {
	groupvel = 0;
	if(mat->GetMaterialPropertiesTable() != 0)
	  groupvel = mat->GetMaterialPropertiesTable()->GetProperty("GROUPVEL");
	update_groupvel = true;
    }
    prev_mat = mat;
      
    if  (groupvel != 0 ) {
	// light velocity = c/(rindex+d(rindex)/d(log(E_phot)))
	// values stored in GROUPVEL material properties vector
	velocity =  prev_velocity;
 	 
	// check if momentum is same as in the previous step
        //  and calculate group velocity if necessary 
	G4double current_momentum = fpDynamicParticle->GetTotalMomentum();
 	if( update_groupvel || (current_momentum != prev_momentum) ) {
	  velocity =
	    groupvel->GetProperty(current_momentum);
	  prev_velocity = velocity;
	  prev_momentum = current_momentum;
	}
    }

  } else {
    // particles other than optical photon
    if (mass<DBL_MIN) {
      // Zero Mass
      velocity = c_light;
    } else {
      G4double T = (fpDynamicParticle->GetKineticEnergy())/mass;
      if (T > maxT) {
	velocity = c_light;
      } else if (T<DBL_MIN) {
	velocity =0.;
      } else if (T<minT) {
	velocity = c_light*std::sqrt(T*(T+2.))/(T+1.0);
      } else {	
	velocity = velTable->Value(T);
      }
    }
      
  }
                                                                                
  return velocity ;
}

///////////////////
void G4Track::PrepareVelocityTable()
///////////////////
{
  velTable = new G4PhysicsLogVector(minT, maxT, NbinT);
  for (G4int i=0; i<=NbinT; i++){
    G4double T = velTable->Energy(i);
    velTable->PutValue(i, c_light*std::sqrt(T*(T+2.))/(T+1.0) );    
  }

  return;
} 


///////////////////
void G4Track::SetVelocityTableProperties(G4double t_max, G4double t_min, G4int nbin)
///////////////////
{
  if (nbin > 100 )                NbinT = nbin;
  if ((t_min < t_max)&&(t_min>0.))  {
    minT = t_min; 
    maxT = t_max;
  } 
  if (velTable !=0)               delete velTable;
  velTable = 0;
}

///////////////////
G4double G4Track::GetMaxTOfVelocityTable()
///////////////////
{ return maxT; }

///////////////////
G4double G4Track::GetMinTOfVelocityTable() 
///////////////////
{ return minT; }

///////////////////
G4double G4Track::GetNbinOfVelocityTable() 
///////////////////
{ return NbinT; }


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
// $Id: G4CoupledTransportation.cc,v 1.9 2006-10-16 17:47:57 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// ------------------------------------------------------------
//  GEANT 4 class implementation
// =======================================================================
// Modified:   
//
// Created:   13 May  2006, J. Apostolakis, based on G4Transportation
// =======================================================================

#include "G4CoupledTransportation.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ParticleTable.hh"
#include "G4ChordFinder.hh"
class G4VSensitiveDetector;

//////////////////////////////////////////////////////////////////////////
//
// Constructor

G4CoupledTransportation::G4CoupledTransportation( G4int verboseLevel )
  : G4VProcess( G4String("CoupledTransportation"), fTransportation ),
    fParticleIsLooping( false ),
    fPreviousSftOrigin (0.,0.,0.),
    fPreviousSafety    ( 0.0 ),
    // fThreshold_Warning_Energy( 100 * MeV ),  
    // fThreshold_Important_Energy( 250 * MeV ), 
    // fThresholdTrials( 10 ), 
    // fUnimportant_Energy( 1 * MeV ), 
    // fNoLooperTrials(0),
    // fSumEnergyKilled( 0.0 ), fMaxEnergyKilled( 0.0 ), 
    fVerboseLevel( verboseLevel )
{
  G4TransportationManager* transportMgr ; 

  transportMgr = G4TransportationManager::GetTransportationManager() ; 

  fMassNavigator = transportMgr->GetNavigatorForTracking() ; 
  fFieldPropagator = transportMgr->GetPropagatorInField() ;
  // fGlobalFieldMgr = transportMgr->GetFieldManager() ;
  fNavigatorId= transportMgr->ActivateNavigator( fMassNavigator ); 
  if( fVerboseLevel > 0 ){
    G4cout << " G4CoupledTransportation constructor: ----- " << G4endl;
    G4cout << " Verbose level is " << fVerboseLevel << G4endl;
    G4cout << " Navigator Id obtained in G4CoupledTransportation constructor " 
	   << fNavigatorId << G4endl;
  }
  fPathFinder=  G4PathFinder::GetInstance(); 

  fCurrentTouchableHandle = G4TouchableHandle( 0 );  // new G4TouchableHistory();

  fStartedNewTrack= true; 
  
  // fEndGlobalTimeComputed  = false;
  // fCandidateEndGlobalTime = 0;
}

//////////////////////////////////////////////////////////////////////////

G4CoupledTransportation::~G4CoupledTransportation()
{
  // fCurrentTouchableHandle is a data member - no deletion required

}

//////////////////////////////////////////////////////////////////////////
//
// Responsibilities:
//    Find whether the geometry limits the Step, and to what length
//    Calculate the new value of the safety and return it.
//    Store the final time, position and momentum.

G4double G4CoupledTransportation::
AlongStepGetPhysicalInteractionLength( const G4Track&  track,
                                             G4double, //  previousStepSize
                                             G4double  currentMinimumStep,
                                             G4double& currentSafety,
                                             G4GPILSelection* selection )
{
  G4double geometryStepLength; 
  fParticleIsLooping = false ;

  // Initial actions moved to  StartTrack()   
  // --------------------------------------
  // Note: in case another process changes touchable handle
  //    it will be necessary to add here (for all steps)   
  // fCurrentTouchableHandle = aTrack->GetTouchableHandle();

  // GPILSelection is set to defaule value of CandidateForSelection
  // It is a return value
  //
  *selection = CandidateForSelection ;

  // Get initial Energy/Momentum of the track
  //
  const G4DynamicParticle*    pParticle  = track.GetDynamicParticle() ;
  const G4ParticleDefinition* pParticleDef   = pParticle->GetDefinition() ;
  G4ThreeVector startMomentumDir       = pParticle->GetMomentumDirection() ;
  G4ThreeVector startPosition          = track.GetPosition() ;

  // G4double   theTime        = track.GetGlobalTime() ;

  if( fStartedNewTrack || track.GetCurrentStepNumber()==1 ) {
#if 0
    if( fVerboseLevel > 0 ){
      G4cout << " Calling PathFinder::Locate() from " 
	     << " G4CoupledTransportation::AlongStepGPIL " 
	     << " with startedNewTrack= " << fStartedNewTrack
	     << "  and step number= " << track.GetCurrentStepNumber()
	     << G4endl;
    }
    //  fPathFinder->Locate( startPosition, startMomentumDir );   // For now -- out in G480
#endif
    fCurrentTouchableHandle = track.GetTouchableHandle(); 
    fStartedNewTrack= false; 
  }

  // The Step Point safety can be limited by other geometries and/or the 
  // assumptions of any process - it's not always the geometrical safety.
  // We calculate the starting point's isotropic safety here.
  //
  G4ThreeVector OriginShift = startPosition - fPreviousSftOrigin ;
  G4double      MagSqShift  = OriginShift.mag2() ;
  if( MagSqShift >= sqr(fPreviousSafety) )
  {
     currentSafety = 0.0 ;
  }
  else
  {
     currentSafety = fPreviousSafety - std::sqrt(MagSqShift) ;
  }

  // Is the particle charged ?
  //
  G4double              particleCharge = pParticle->GetCharge() ; 

  // fEndGlobalTimeComputed = false ;

  // There is no need to locate the current volume. It is Done elsewhere:
  //   On track construction 
  //   By the tracking, after all AlongStepDoIts, in "Relocation"

  // Check whether the particle have an (EM) field force exerting upon it
  //
  G4FieldManager* fieldMgr=0;
  G4bool          fieldExertsForce = false ;
  if( (particleCharge != 0.0) ) // ||  (magneticMoment != 0.0 ) )
  {
     fieldMgr= fFieldPropagator->FindAndSetFieldManager( track.GetVolume() ); 
     if (fieldMgr != 0) {
        // If the field manager has no field, there is no field !
        fieldExertsForce = (fieldMgr->GetDetectorField() != 0);
     } 
  }
  
  //  A field could be exerting force  --  the PathFinder will take care!
  
  G4double       momentumMagnitude = pParticle->GetTotalMomentum() ;
  G4ThreeVector  EndUnitMomentum ;
  G4double       lengthAlongCurve ;
  G4double       restMass = pParticleDef->GetPDGMass() ;
 
  fFieldPropagator->SetChargeMomentumMass( particleCharge,    // in e+ units
					   momentumMagnitude, // in Mev/c 
					   restMass           ) ;  
  
  // Message the field Manager, to configure it for this track
  if (fieldMgr != 0) {
     fieldMgr->ConfigureForTrack( &track );
  }

  G4ThreeVector spin        = track.GetPolarization() ;
  G4FieldTrack  theFieldTrack = G4FieldTrack( startPosition, 
					    track.GetMomentumDirection(),
					    0.0, 
					    track.GetKineticEnergy(),
					    restMass,
					    0.0,    // UNUSED: track.GetVelocity(),
					    track.GetGlobalTime(), // Lab.
					    track.GetProperTime(), // Part.
					    &spin                  ) ;
  theFieldTrack.SetChargeAndMoments( particleCharge ); // EM moments -- future extension 

  G4int stepNo= track.GetCurrentStepNumber(); 

  ELimited limitedStep; 
  G4FieldTrack endTrackState('a');  //  Default values

  fGeometryLimitedStep = false ;    //  default 
  if( currentMinimumStep > 0 )  {
      // Do the Transport in the field (non recti-linear)
      //
      lengthAlongCurve = fPathFinder->ComputeStep( theFieldTrack,
						   currentMinimumStep, 
						   fNavigatorId,
						   stepNo,
						   currentSafety,
						   limitedStep,
						   endTrackState ) ;
      // G4cout << " PathFinder ComputeStep returns " << lengthAlongCurve << G4endl; 

      if( limitedStep == kUnique || limitedStep == kSharedTransport ) {
	 fGeometryLimitedStep = true ;
      }
      if( lengthAlongCurve < currentMinimumStep){
         geometryStepLength   = lengthAlongCurve ;
         // Old code:   fGeometryLimitedStep = true ;
      } else {
         geometryStepLength   = currentMinimumStep ;
      }

      // Momentum:  Magnitude and direction can be changed too now ...
      //
      fMomentumChanged         = true ; 
      fTransportEndMomentumDir = endTrackState.GetMomentumDir() ;

      // Remember last safety origin & value.
      fPreviousSftOrigin = startPosition ;
      fPreviousSafety    = currentSafety ;         

      // Get the End-Position and End-Momentum (Dir-ection)
      fTransportEndPosition = endTrackState.GetPosition() ;
      fTransportEndKineticEnergy  = endTrackState.GetKineticEnergy() ; 
  } else {
      geometryStepLength   = lengthAlongCurve= 0.0 ;
      fMomentumChanged         = false ; 
      // fGeometryLimitedStep = false ;   //  --- ???
      fTransportEndMomentumDir = track.GetMomentumDirection();
      fTransportEndKineticEnergy  = track.GetKineticEnergy();
  }

  // G4FieldTrack aTrackState(endTrackState);  

  if( fVerboseLevel > 1 ){
    G4cout << " G4CT::CS End Position = "  << fTransportEndPosition << G4endl; 
    G4cout << " G4CT::CS End Direction = " << fTransportEndMomentumDir << G4endl; 
  }

#if 0   
  if( fFieldPropagator->GetCurrentFieldManager()->DoesFieldChangeEnergy() )
    {
      // If the field can change energy, then the time must be integrated
      //    - so this should have been updated
      //
      fCandidateEndGlobalTime   = endTrackState.GetLabTimeOfFlight();
      fEndGlobalTimeComputed    = true;
      
      // was ( fCandidateEndGlobalTime != track.GetGlobalTime() );
      // a cleaner way is to have FieldTrack knowing whether time is updated.
    }
  else
#endif
    {
      // The energy should be unchanged by field transport,
      //    - so the time changed will be calculated elsewhere
      //
      fEndGlobalTimeComputed = false;
      
      // Check that the integration preserved the energy 
      //     -  and if not correct this!
      G4double  startEnergy= track.GetKineticEnergy();
      G4double  endEnergy= fTransportEndKineticEnergy; 
      
      static G4int no_inexact_steps=0; // , no_large_ediff;
      G4double absEdiff = std::fabs(startEnergy- endEnergy);
      if( absEdiff > perMillion * endEnergy )  {
	 no_inexact_steps++;
	 // Possible statistics keeping here ...
      }
      if( (fVerboseLevel > 1) && ( absEdiff > perThousand * endEnergy) )
        {
	  ReportInexactEnergy(startEnergy, endEnergy); 
      
        }  // end of if (fVerboseLevel)

      // Correct the energy for fields that conserve it
      //  This - hides the integration error
      //       - but gives a better physical answer
      fTransportEndKineticEnergy= track.GetKineticEnergy(); 
    }

  endpointDistance   = (fTransportEndPosition - startPosition).mag() ;
  // fParticleIsLooping = fFieldPropagator->IsParticleLooping() ;

  fTransportEndSpin = endTrackState.GetSpin();

  // If we are asked to go a step length of 0, and we are on a boundary
  //   then a boundary will also limit the step -> we must flag this.
  //
  if( currentMinimumStep == 0.0 ) {
     //   TODO:  Add explicit logical status for being at a boundary
     if( currentSafety == 0.0 )  fGeometryLimitedStep = true ;
  }

  // Update safety for the end-point, if becomes negative at the end-point.
  //   TODO:  No safety update if at a boundary
  // 
  if( currentSafety < endpointDistance ) 
  {
      G4double endSafety =
               fMassNavigator->ComputeSafety( fTransportEndPosition) ;
      fPreviousSftOrigin = fTransportEndPosition ;
      fPreviousSafety    = endSafety ; 

      // The convention (Stepping Manager's)  is safety from the start point, 
      //
      currentSafety = endSafety + endpointDistance ;

#ifdef G4DEBUG_TRANSPORT 
      G4cout.precision(16) ;
      G4cout << "***CoupledTransportation::AlongStepGPIL ** " << G4endl  ;
      G4cout << "  Revised Safety at endpoint "  << fTransportEndPosition
             << "   give safety value= " << endSafety << G4endl ; 
      G4cout << "  Adding endpoint distance " << endpointDistance 
             << "   to obtain pseudo-safety= " << currentSafety << G4endl ; 
#endif
  }            

  fParticleChange.ProposeTrueStepLength(geometryStepLength) ;

  return geometryStepLength ;
}

//////////////////////////////////////////////////////////////////////////

G4VParticleChange* G4CoupledTransportation::AlongStepDoIt( const G4Track& track,
                                                    const G4Step&  stepData )
{
  static G4int noCalls=0;
  static const G4ParticleDefinition* fOpticalPhoton =
           G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");

  noCalls++;

  fParticleChange.Initialize(track) ;
      // sets all its members to the value of corresponding members in G4Track

  //  Code specific for Transport
  //
  fParticleChange.ProposePosition(fTransportEndPosition) ;
  // G4cout << " G4CoupledTransportation::AlongStepDoIt" 
  //     << " proposes position = " << fTransportEndPosition  
  //     << " and end momentum direction  = " << fTransportEndMomentumDir <<  G4endl;
  fParticleChange.ProposeMomentumDirection(fTransportEndMomentumDir) ;
  fParticleChange.ProposeEnergy(fTransportEndKineticEnergy) ;
  fParticleChange.SetMomentumChanged(fMomentumChanged) ;

  fParticleChange.ProposePolarization(fTransportEndSpin);
  
  G4double deltaTime = 0.0 ;

  // Calculate  Lab Time of Flight (ONLY if field Equations used it!)
     // G4double endTime   = fCandidateEndGlobalTime;
     // G4double delta_time = endTime - startTime;

  G4double startTime = track.GetGlobalTime() ;
  
  if (!fEndGlobalTimeComputed)
  {
     G4double finalInverseVel= DBL_MAX, initialInverseVel=DBL_MAX; 

     // The time was not integrated .. make the best estimate possible
     //
     G4double finalVelocity   = track.GetVelocity() ;
     if( finalVelocity > 0.0 ) { finalInverseVel= 1.0 / finalVelocity; }
     G4double initialVelocity = stepData.GetPreStepPoint()->GetVelocity() ;
     if( initialVelocity > 0.0 ) { initialInverseVel= 1.0 / initialVelocity; }
     G4double stepLength      = track.GetStepLength() ;

     const G4DynamicParticle* fpDynamicParticle = track.GetDynamicParticle();
     if (fpDynamicParticle->GetDefinition()== fOpticalPhoton)
     {
        //  A photon is in the medium of the final point
        //  during the step, so Peter says it has the final velocity.
        deltaTime = stepLength * finalInverseVel ;
	// G4cout << " dt = s * initV "  << "  s = "   << stepLength
	//        << " finalV= " << finalInverseVel << G4endl; 
     }
     else if (finalVelocity > 0.0)
     {
        // deltaTime = stepLength/finalVelocity ;
        G4double meanInverseVelocity = 0.5 * ( initialInverseVel + finalInverseVel );
        deltaTime = stepLength * meanInverseVelocity ;
	// G4cout << " dt = s * mean(1/v) , with " << "  s = " << stepLength
	//     << "  mean(1/v)= "  << meanInverseVelocity << G4endl;
     }
     else
     {
        deltaTime = stepLength * initialInverseVel ;
	// G4cout << " dt = s * initV "  << "  s = "   << stepLength
	//        << " initV= " << initialInverseVel << G4endl; 
     }  //  Could do with better estimate for final step (finalVelocity = 0) ?

     fCandidateEndGlobalTime   = startTime + deltaTime ;

     // G4cout << " Calculated global time from start = " << startTime << " and "
     //        << " delta time = " << deltaTime << G4endl;
  }
  else
  {
     deltaTime = fCandidateEndGlobalTime - startTime ;
     // G4cout << " Calculated global time from candidate end time = "
     //    << fCandidateEndGlobalTime << " and start time = " << startTime << G4endl;
  }

  // G4cout << " G4CoupledTransportation::AlongStepDoIt  "
  // << " flag whether computed time = " << fEndGlobalTimeComputed  << " and " 
  // << " is proposes end time " << fCandidateEndGlobalTime << G4endl; 
  fParticleChange.ProposeGlobalTime( fCandidateEndGlobalTime ) ;

  // Now Correct by Lorentz factor to get "proper" deltaTime
  
  G4double  restMass       = track.GetDynamicParticle()->GetMass() ;
  G4double deltaProperTime = deltaTime*( restMass/track.GetTotalEnergy() ) ;

  fParticleChange.ProposeProperTime(track.GetProperTime() + deltaProperTime) ;
  //fParticleChange. ProposeTrueStepLength( track.GetStepLength() ) ;

#if 0 
  // If the particle is caught looping or is stuck (in very difficult
  // boundaries) in a magnetic field (doing many steps) 
  //   THEN this kills it ...
  //
  if ( fParticleIsLooping )
  {
     G4double endEnergy= fTransportEndKineticEnergy;

if( (endEnergy < fThreshold_Important_Energy) 
	  || (fNoLooperTrials >= fThresholdTrials ) ){
	// Kill the looping particle 
	//
	fParticleChange.ProposeTrackStatus( fStopAndKill )  ;

        // 'Bare' statistics
        fSumEnergyKilled += endEnergy; 
	if( endEnergy > fMaxEnergyKilled) { fMaxEnergyKilled= endEnergy; }

#ifdef G4VERBOSE
	if( (fVerboseLevel > 1) || 
	    ( endEnergy > fThreshold_Warning_Energy )  ) { 
	  G4cout << " G4CoupledTransportation is killing track that is looping or stuck "
		 << G4endl
		 << "   This track has " << track.GetKineticEnergy() / MeV
		 << " MeV energy." << G4endl;
	}
#endif
	fNoLooperTrials=0; 
      } else{ 
	fNoLooperTrials ++; 
      }
  }else{ fNoLooperTrials=0; }
#endif

  // Another (sometimes better way) is to use a user-limit maximum Step size
  // to alleviate this problem .. 

  // Add smooth curved trajectories to particle-change
  //
  // fParticleChange.SetPointerToVectorOfAuxiliaryPoints
  //   (fFieldPropagator->GimmeTrajectoryVectorAndForgetIt() );

  return &fParticleChange ;
}

//////////////////////////////////////////////////////////////////////////
//
//  This ensures that the PostStep action is always called,
//  so that it can do the relocation if it is needed.
// 

G4double G4CoupledTransportation::
PostStepGetPhysicalInteractionLength( const G4Track&,
                                            G4double, // previousStepSize
                                            G4ForceCondition* pForceCond )
{ 
  // Must act as PostStep action -- to relocate particle
  *pForceCond = Forced ;    
  return DBL_MAX ;
}

static 
void ReportMove( G4ThreeVector OldVector, G4ThreeVector NewVector, G4String& Quantity )
{
    G4ThreeVector moveVec = ( NewVector - OldVector );

    G4cerr << G4endl
	   << "**************************************************************" << G4endl;
    G4cerr << "Endpoint has moved between value expected from TransportEndPosition "
	   << " and value from Track in PostStepDoIt. " << G4endl
	   << "Change of " << Quantity << " is " << moveVec.mag() / mm << " mm long, "
	   << " and its vector is " << (1.0/mm) * moveVec << " mm " << G4endl
	   << "Endpoint of ComputeStep was " << OldVector
	   << " and current position to locate is " << NewVector << G4endl;
}

/////////////////////////////////////////////////////////////////////////////

G4VParticleChange* G4CoupledTransportation::PostStepDoIt( const G4Track& track,
                                                   const G4Step& )
{
  G4TouchableHandle retCurrentTouchable ;   // The one to return

  // Initialize ParticleChange  (by setting all its members equal
  //                             to corresponding members in G4Track)
  // fParticleChange.Initialize(track) ;  // To initialise TouchableChange

  fParticleChange.ProposeTrackStatus(track.GetTrackStatus()) ;

  // Check that the end position and direction are preserved 
  //  between 
  if( (fTransportEndPosition  - track.GetPosition()).mag2() >= 1.0e-16 ){
     static G4String EndLabelString("End of Step Position");  
     ReportMove( track.GetPosition(), fTransportEndPosition, EndLabelString ); 
     G4cerr << " Problem in G4CoupledTransportation::PostStepDoIt " << G4endl; 
  }

  // If the Step was determined by the volume boundary, relocate the particle
  // The pathFinder will know that the geometry limited the step (!?)

  if( fVerboseLevel > 0 ){
     G4cout << " Calling PathFinder::Locate() from " 
	    << " G4CoupledTransportation::PostStepDoIt " << G4endl;
  }
  fPathFinder->Locate( track.GetPosition(), 
		       track.GetMomentumDirection(),
		       true); 

  if(fGeometryLimitedStep)
  {  
    // fCurrentTouchable will now become the previous touchable, 
    // and what was the previous will be freed.
    // (Needed because the preStepPoint can point to the previous touchable)
    if( fVerboseLevel > 0 )
      G4cout << "G4CoupledTransportation::PostStepDoIt --- fNavigatorId = " 
	     << fNavigatorId << G4endl;

    fCurrentTouchableHandle= 
      fPathFinder->CreateTouchableHandle( fNavigatorId );

    // Check whether the particle is out of the world volume 
    // If so it has exited and must be killed.
    //
    if( fVerboseLevel > 1 ){
       G4cout << "CHECK !!!!!!!!!!! fCurrentTouchableHandle->GetVolume() = " 
	      << fCurrentTouchableHandle->GetVolume() << G4endl;
    }
    if( fCurrentTouchableHandle->GetVolume() == 0 )
    {
       fParticleChange.ProposeTrackStatus( fStopAndKill ) ;
    }
    retCurrentTouchable = fCurrentTouchableHandle ;
    fParticleChange.SetTouchableHandle( fCurrentTouchableHandle ) ;
  }
  else                 // fGeometryLimitedStep  is false
  { 
    if( fVerboseLevel > 1 ){
       G4cout << "G4CoupledTransportation::PostStepDoIt -- fGeometryLimitedStep = false !!" 
	      << G4endl;
    }
    // This serves only to move the Navigator's location
    //
    // fLinearNavigator->LocateGlobalPointWithinVolume( track.GetPosition() ) ;

    // Keep the value of the track's current Touchable is retained,
    //  and use it to overwrite the (unset) one in particle change.
    // In general this is fCurrentTouchable, 
    //  - yet could it be different at the start of a step ?
    //
    fParticleChange.SetTouchableHandle( track.GetTouchableHandle() ) ;
    retCurrentTouchable = track.GetTouchableHandle() ;
  }         // endif ( fGeometryLimitedStep ) 

  const G4VPhysicalVolume* pNewVol = retCurrentTouchable->GetVolume() ;
  const G4Material* pNewMaterial   = 0 ;
  const G4VSensitiveDetector* pNewSensitiveDetector   = 0 ;
                                                                                       
  if( pNewVol != 0 )
  {
    pNewMaterial= pNewVol->GetLogicalVolume()->GetMaterial();
    pNewSensitiveDetector= pNewVol->GetLogicalVolume()->GetSensitiveDetector();
  }

  // ( const_cast<G4Material *> pNewMaterial ) ;
  // ( const_cast<G4VSensitiveDetetor *> pNewSensitiveDetector) ;

  fParticleChange.SetMaterialInTouchable( (G4Material *) pNewMaterial ) ;
  fParticleChange.SetSensitiveDetectorInTouchable( (G4VSensitiveDetector *) pNewSensitiveDetector ) ;
             // "temporarily" until Get/Set Material of ParticleChange, 
             // and StepPoint can be made const. 

  const G4MaterialCutsCouple* pNewMaterialCutsCouple = 0;
  if( pNewVol != 0 )
  {
    pNewMaterialCutsCouple=pNewVol->GetLogicalVolume()->GetMaterialCutsCouple();
    if( pNewMaterialCutsCouple!=0 
	&& pNewMaterialCutsCouple->GetMaterial()!=pNewMaterial )
      {
	// for parametrized volume
	//
	pNewMaterialCutsCouple =
	  G4ProductionCutsTable::GetProductionCutsTable()
                       ->GetMaterialCutsCouple(pNewMaterial,
					       pNewMaterialCutsCouple->GetProductionCuts());
      }
  }
  fParticleChange.SetMaterialCutsCoupleInTouchable( pNewMaterialCutsCouple );

  // Must always set the touchable in ParticleChange, whether relocated or not
  fParticleChange.SetTouchableHandle(retCurrentTouchable) ;

  return &fParticleChange ;
}

// New method takes over the responsibility to reset the state of G4CoupledTransportation
//   object at the start of a new track or the resumption of a suspended track. 

void 
G4CoupledTransportation::StartTracking(G4Track* aTrack) // G48
{

  static G4TransportationManager* transportMgr=  
      G4TransportationManager::GetTransportationManager(); 

  // G4VProcess::StartTracking(aTrack);

  //  Here are the 'initialising' actions
  //     once taken in AlongStepGPIL -- if ( track.GetCurrentStepNumber()==1 )

  fStartedNewTrack= true; 

  fMassNavigator = transportMgr->GetNavigatorForTracking() ; 
  fNavigatorId= transportMgr->ActivateNavigator( fMassNavigator );  // Confirm it!

  if( fVerboseLevel > 1 ){
    G4cout << " Navigator Id obtained in StartTracking " << fNavigatorId << G4endl;
  }
  G4ThreeVector position = aTrack->GetPosition(); 
  G4ThreeVector direction = aTrack->GetMomentumDirection(); // G48

  if( fVerboseLevel > 1 ){
    G4cout << " Calling PathFinder::PrepareNewTrack from    " 
	   << " G4CoupledTransportation::StartTracking -- which calls Locate()" << G4endl;
  }
  fPathFinder->PrepareNewTrack( position, direction); 
                // track.GetPosition(), track.GetMomentumDirection() );  //G48
  // This implies a call to 
  //  fPathFinder->Locate( position, direction ); 

  // reset safety value and center
  //
  fPreviousSafety    = 0.0 ; 
  fPreviousSftOrigin = G4ThreeVector(0.,0.,0.) ;
  
#if 0
  // Put back in for 8.0   -- G480
  // reset looping counter -- for motion in field
  if( aTrack->GetCurrentStepNumber()==1 ) {
     fNoLooperTrials= 0; 
  }
#endif

  // ChordFinder reset internal state
  //
  if( DoesGlobalFieldExist() ) {
     G4ChordFinder* chordF= fFieldPropagator->GetChordFinder();
     if( chordF ) chordF->ResetStepEstimate();
  }
  
  // Update the current touchable handle  (from the track's)
  //
  fCurrentTouchableHandle = aTrack->GetTouchableHandle();  // G480
}

void
G4CoupledTransportation::
ReportInexactEnergy(G4double startEnergy, G4double endEnergy)
{
  static G4int no_warnings= 0, warnModulo=1,  moduloFactor= 10, no_large_ediff= 0; 

  if( std::fabs(startEnergy- endEnergy) > perThousand * endEnergy )
    {
      no_large_ediff ++;
      if( (no_large_ediff% warnModulo) == 0 )
	{
	  no_warnings++;
	  G4cout << "WARNING - G4CoupledTransportation::AlongStepGetPIL() " 
		 << "   Energy change in Step is above 1^-3 relative value. " << G4endl
		 << "   Relative change in 'tracking' step = " 
		 << std::setw(15) << (endEnergy-startEnergy)/startEnergy << G4endl
		 << "     Starting E= " << std::setw(12) << startEnergy / MeV << " MeV " << G4endl
		 << "     Ending   E= " << std::setw(12) << endEnergy   / MeV << " MeV " << G4endl;       
	  G4cout << " Energy has been corrected -- however, review"
		 << " field propagation parameters for accuracy."  << G4endl;
	  if( (fVerboseLevel > 2 ) || (no_warnings<4) || (no_large_ediff == warnModulo * moduloFactor) ){
	    G4cout << " These include EpsilonStepMax(/Min) in G4FieldManager "
		   << " which determine fractional error per step for integrated quantities. " << G4endl
		   << " Note also the influence of the permitted number of integration steps."
		   << G4endl;
	  }
	  G4cerr << "ERROR - G4CoupledTransportation::AlongStepGetPIL()" << G4endl
		 << "        Bad 'endpoint'. Energy change detected"
		 << " and corrected. " 
		 << " Has occurred already "
		 << no_large_ediff << " times." << G4endl;
	  if( no_large_ediff == warnModulo * moduloFactor )
	    {
	      warnModulo *= moduloFactor;
	    }
	}
    }
}

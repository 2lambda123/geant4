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
// $Id: G4VParticleChange.cc,v 1.12 2003-05-19 15:16:25 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//	GEANT 4 class implementation file 
//
//
// ------------------------------------------------------------
//   Implemented for the new scheme                 23 Mar. 1998  H.Kurahige
// --------------------------------------------------------------

#include "G4VParticleChange.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TrackFastVector.hh"

const G4double G4VParticleChange::accuracyForWarning = 1.0e-9;
const G4double G4VParticleChange::accuracyForException = 0.001;

G4VParticleChange::G4VParticleChange():
   theNumberOfSecondaries(0),
   theSizeOftheListOfSecondaries(G4TrackFastVectorSize),
   theStatusChange(fAlive),
   theSteppingControlFlag(NormalCondition),     
   theLocalEnergyDeposit(0.0),
   verboseLevel(1),
   theParentWeight(1.0),
   fSetSecondaryWeightByProcess(false),  
   fSetParentWeightByProcess(true)  
{
   debugFlag = false;
#ifdef G4VERBOSE
  // activate CHeckIt if in VERBOSE mode
  debugFlag = true;
#endif
   theListOfSecondaries = new G4TrackFastVector();
}

G4VParticleChange::~G4VParticleChange() {
  // check if tracks still exist in theListOfSecondaries
  if (theNumberOfSecondaries>0) {
#ifdef G4VERBOSE
    if (verboseLevel>0) {
      G4cerr << "G4VParticleChange::~G4VParticleChange() Warning  ";
      G4cerr << "theListOfSecondaries is not empty ";
    }
#endif
    for (G4int index= 0; index<theNumberOfSecondaries; index++){
      if ( (*theListOfSecondaries)[index] ) delete (*theListOfSecondaries)[index] ;
    }
  }
  delete theListOfSecondaries; 
}

// copy and assignment operators are implemented as "shallow copy"
G4VParticleChange::G4VParticleChange(const G4VParticleChange &right):
   theNumberOfSecondaries(0),
   theSizeOftheListOfSecondaries(G4TrackFastVectorSize),
   theStatusChange(fAlive),
   theSteppingControlFlag(NormalCondition),     
   theLocalEnergyDeposit(0.0),
   verboseLevel(1),
   theParentWeight(1.0),
   fSetSecondaryWeightByProcess(false)
{
   debugFlag = false;
#ifdef G4VERBOSE
  // activate CHeckIt if in VERBOSE mode
  debugFlag = true;
#endif

  theListOfSecondaries = right.theListOfSecondaries;
  theSizeOftheListOfSecondaries = right.theSizeOftheListOfSecondaries;
  theNumberOfSecondaries = right.theNumberOfSecondaries;
  theStatusChange = right.theStatusChange;
  theTrueStepLength = right.theTrueStepLength;
  theLocalEnergyDeposit = right.theLocalEnergyDeposit;
  theSteppingControlFlag = right.theSteppingControlFlag;
  fSetParentWeightByProcess = right.fSetParentWeightByProcess;    
}


G4VParticleChange & G4VParticleChange::operator=(const G4VParticleChange &right)
{
   debugFlag = false;
#ifdef G4VERBOSE
  // activate CHeckIt if in VERBOSE mode
  debugFlag = true;
#endif
   if (this != &right)
   {
      theListOfSecondaries = right.theListOfSecondaries;
      theSizeOftheListOfSecondaries = right.theSizeOftheListOfSecondaries;
      theNumberOfSecondaries = right.theNumberOfSecondaries;
      theStatusChange = right.theStatusChange;
      theTrueStepLength = right.theTrueStepLength;
      theLocalEnergyDeposit = right.theLocalEnergyDeposit;
      theSteppingControlFlag = right.theSteppingControlFlag;
      fSetParentWeightByProcess = right.fSetParentWeightByProcess;    
   }
   return *this;
}

G4bool G4VParticleChange::operator==(const G4VParticleChange &right) const
{
   return (this == (G4VParticleChange *) &right);
}


G4bool G4VParticleChange::operator!=(const G4VParticleChange &right) const
{
   return (this != (G4VParticleChange *) &right);
}

//----------------------------------------------------------------
// methods for printing messages  
//
 
void G4VParticleChange::DumpInfo() const
{

// Show header
  G4cout.precision(3);
  G4cout << "      -----------------------------------------------" 
       << G4endl;
  G4cout << "        G4ParticleChange Information  " << G4std::setw(20) << G4endl;
  G4cout << "      -----------------------------------------------" 
       << G4endl;

  G4cout << "        # of 2ndaries       : " 
       << G4std::setw(20) << theNumberOfSecondaries
       << G4endl;

  if (theNumberOfSecondaries >0) {
    G4cout << "        Pointer to 2ndaries : " 
         << G4std::setw(20) << GetSecondary(0)
         << G4endl;
    G4cout << "        (Showed only 1st one)"
         << G4endl;
  }
  G4cout << "      -----------------------------------------------" 
       << G4endl;

  G4cout << "        Energy Deposit (MeV): " 
       << G4std::setw(20) << theLocalEnergyDeposit/MeV
       << G4endl;

  G4cout << "        Track Status        : " 
       << G4std::setw(20);
       if( theStatusChange == fAlive ){
         G4cout << " Alive";
       } else if( theStatusChange == fStopButAlive ){
           G4cout << " StopButAlive";
       } else if( theStatusChange == fStopAndKill ){
           G4cout << " StopAndKill";
       } else if( theStatusChange  == fKillTrackAndSecondaries ){
           G4cout << " KillTrackAndSecondaries";
       } else if( theStatusChange  == fSuspend ){
           G4cout << " Suspend";
       } else if( theStatusChange == fPostponeToNextEvent ){
           G4cout << " PostponeToNextEvent";
       }
       G4cout << G4endl;
  G4cout << "        True Path Length (mm) : " 
       << G4std::setw(20) << theTrueStepLength/mm
       << G4endl;
  G4cout << "        Stepping Control     : " 
       << G4std::setw(20) << theSteppingControlFlag
       << G4endl;   
  G4cout << G4endl;      
}

G4bool G4VParticleChange::CheckIt(const G4Track& )
{

  G4bool    exitWithError = false;
  G4double  accuracy;

  // Energy deposit should not be negative
  G4bool itsOKforEnergy = true;
  accuracy = -1.0*theLocalEnergyDeposit/MeV;
  if (accuracy > accuracyForWarning) {
    G4cout << "  G4VParticleChange::CheckIt    : ";
    G4cout << "the energy deposit  is negative  !!" << G4endl;
    G4cout << "  Difference:  " << accuracy  << "[MeV] " <<G4endl;
    itsOKforEnergy = false;
    if (accuracy > accuracyForException) exitWithError = true;
  }
 
  // true path length should not be negative
  G4bool itsOKforStepLength = true;
  accuracy = -1.0*theTrueStepLength/mm;
  if (accuracy > accuracyForWarning) {
    G4cout << "  G4VParticleChange::CheckIt    : ";
    G4cout << "the true step length is negative  !!" << G4endl;
    G4cout << "  Difference:  " << accuracy  << "[MeV] " <<G4endl;
    itsOKforStepLength = false;
    if (accuracy > accuracyForException) exitWithError = true;
  }

  G4bool itsOK = itsOKforStepLength && itsOKforEnergy ;
  // dump out information of this particle change
  if (! itsOK ){
    G4cout << " G4VParticleChange::CheckIt " <<G4endl;
    DumpInfo();
  }

  // Exit with error
  if (exitWithError) G4Exception("G4VParticleChange::CheckIt");

  // correction 
  if ( !itsOKforStepLength ) {
    theTrueStepLength =  (1.e-12)*mm;
  } 
  if ( !itsOKforEnergy ) {
    theLocalEnergyDeposit = 0.0;
  }
  return itsOK;
}

const G4double G4VParticleChange::GetAccuracyForWarning() const
{
  return accuracyForWarning;
}

const G4double G4VParticleChange::GetAccuracyForException() const
{
  return accuracyForException;
}

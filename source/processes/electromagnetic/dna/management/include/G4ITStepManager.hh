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
// $Id: G4ITStepManager.hh 60427 2012-07-11 16:34:35Z matkara $
//
// Author: Mathieu Karamitros, kara@cenbg.in2p3.fr

// The code is developed in the framework of the ESA AO7146
//
// We would be very happy hearing from you, so do not hesitate to send us your feedback!
//
// In order for Geant4-DNA to be maintained and still open-source, article citations are crucial. 
// If you use Geant4-DNA chemistry and you publish papers about your software, in addition to the general paper on Geant4-DNA:
//
// The Geant4-DNA project, S. Incerti et al., Int. J. Model. Simul. Sci. Comput. 1 (2010) 157–178
//
// we ask that you please cite the following papers reference papers on chemistry:
//
// Diﬀusion-controlled reactions modelling in Geant4-DNA, M. Karamitros et al., 2014 (submitted)
// Modeling Radiation Chemistry in the Geant4 Toolkit, M. Karamitros et al., Prog. Nucl. Sci. Tec. 2 (2011) 503-508


#ifndef G4ITStepManager_h
#define G4ITStepManager_h

#include <vector>
#include <map>
#include <memory>

#include "globals.hh"

#include "G4TrackList.hh"
#include "G4ITModelHandler.hh"
#include "G4ITStepStatus.hh"
#include "G4ITTrackHolder.hh"
#include "G4VStateDependent.hh"

class G4ITTrackingManager;
class G4ITModelProcessor;
class G4ITStepProcessor;
class G4Track ;
class G4UserTimeStepAction;
class G4ITSteppingMessenger;
class G4ITTrackingInteractivity;
class G4ITGun;


/**
 * G4ITStepManager enables to synchronize in time
 * the step of tracks.
 */
class G4ITStepManager : public G4ITTrackHolder, public G4VStateDependent
{
protected:
	virtual ~G4ITStepManager();

public :
	static G4ITStepManager* Instance();
	/** DeleteInstance should be used instead
	 * of the destructor
	 */
	static void DeleteInstance();
	virtual G4bool Notify(G4ApplicationState requestedState) ;

	void Initialize();
	void ForceReinitialization() ;
	inline bool IsInitialized();
	void Reset();
	void Process() ;
	virtual void PushTrack(G4Track*);
	void ClearList();

	inline void SetGun(G4ITGun*);

	inline void Stop();
	void Clear();

	// To be called only in UserReactionAction::EndProcessing()
	// after fRunning flag has been turned off.
	// This is not done automatically before UserReactionAction::EndProcessing()
	// is called in case one would like to access some track information
	void EndTracking();

	void SetEndTime(const double);

	inline void SetTimeTolerance(double);
	// Two tracks below the time tolerance are supposed to be
	// in the same time slice
	inline double GetTimeTolerance() const;

	inline void SetMaxZeroTimeAllowed(int);
	inline int GetMaxZeroTimeAllowed() const;

	inline G4ITModelHandler* GetModelHandler();
	inline void     SetTimeSteps(std::map<double,double>*);
	inline void		SetDefaultTimeStep(double);
	double GetLimitingTimeStep() const;
	inline G4int    GetNbSteps() const;
	inline void     SetMaxNbSteps(G4int);
	inline G4int    GetMaxNbSteps() const;
	inline G4double GetStartTime() const;
	inline G4double GetEndTime() const;
	virtual inline G4double GetTimeStep() const;
	inline G4double GetPreviousTimeStep() const;
	inline G4double GetGlobalTime() const;
	inline void     SetUserAction(G4UserTimeStepAction*);
	inline G4UserTimeStepAction* GetUserReactionAction() const;

	inline G4ITStepStatus GetStatus() const;

	inline void SetVerbose(int);
	// 1 : Reaction information
	// 2 : (1) + time step information
	// 3 : (2) + step info for individual tracks
	// 4 : (2) + trackList processing info + pushed and killed track info
	inline int GetVerbose() const;

	void SetInteractivity(G4ITTrackingInteractivity*);
	inline G4ITTrackingInteractivity* GetInteractivity();

protected:

	void DoProcess();
	void SynchronizeTracks();
	void Stepping();

	void FindUserPreDefinedTimeStep();
	void CalculateMinTimeStep() ;
	void ComputeInteractionLength();
	void DoIt();
	void ComputeTrackReaction();
	void MergeSecondariesWithMainList();

	void PushSecondaries(G4ITStepProcessor*);
	void _PushTrack(G4Track*);
	void PushDelayed(G4Track*, const G4double&);

	void EndTracking(G4Track*);
	void KillTracks();

	void ExtractTimeStepperData(G4ITModelProcessor*);
	void ExtractILData(G4ITStepProcessor*);
	void ExtractDoItData(G4ITStepProcessor*);

	void AddTrackID(G4Track*);

	void ResetLeadingTracks();

private:
	G4ITStepManager();
	void Create();
	G4ITStepManager(const G4ITStepManager&);
	G4ITStepManager& operator=(const G4ITStepManager&);

	G4ITSteppingMessenger* fSteppingMsg;

	static G4ThreadLocal G4ITStepManager* fgStepManager ;
	int fVerbose;
	bool fInitialized;
	bool fRunning ;

	int fNbTracks ;
	int fNbSteps ;
	int fMaxSteps;

	G4ITStepStatus fITStepStatus;

	// Time members
	double fTimeTolerance;
	double fGlobalTime;
	double fTmpGlobalTime ;
	double fStartTime ;       // fEndTime : stores the biggest global time steps here (for synchronizing tracks)
	double fEndTime ;       // fEndTime : stores the biggest global time steps here (for synchronizing tracks)
	double fTmpEndTime ;    // fTmpEndTime : Stores the biggest end time here (for synchronizing tracks)
	double fPreviousTimeStep;
	int    fZeroTimeCount;
	int    fMaxNZeroTimeStepsAllowed;

	// Flags
	bool fComputeTimeStep;
	bool fComputeReaction;

	double fTimeStep ; // The selected minimum time step

	// User steps
	bool fUsePreDefinedTimeSteps ;
	double fDefaultMinTimeStep;
	std::map<double, double>* fpUserTimeSteps ; // One can give time steps in respect to the global time
	mutable double fUserUpperTimeLimit;
	double fDefinedMinTimeStep  ; // selected user time step in respect to the global time
	bool fReachedUserTimeLimit ; // if fMinTimeStep == the user time step

	double fTSTimeStep; // Time calculated by the time stepper in CalculateMinTimeStep()
	double fILTimeStep; // Time calculated by the interaction length methods in ComputeInteractionLength()

	std::map<G4Track*, G4TrackVectorHandle> fReactingTracks ;
	std::vector<G4Track*> fLeadingTracks ;

	bool fInteractionStep ;
	// Flag : if the step is lead by the interaction with the matter and NOT by tracks' reaction

	G4TrackList*                    fpMainList ;
	G4TrackList                     fSecondaries ; //to merge with fpMainList
	G4TrackList*                    fpWaitingList ; // Waiting queue of currentList
	std::map<double,G4TrackList* >  fDelayedList ;
	G4TrackList                     fToBeKilledList ;

	G4ITStepProcessor* fpStepProcessor ;
	G4ITModelProcessor* fpModelProcessor;

	G4ITModelHandler* fpModelHandler;

	G4ITTrackingManager* fpTrackingManager;
	G4UserTimeStepAction* fpUserTimeStepAction;
	G4ITTrackingInteractivity* fpTrackingInteractivity;

	G4ITGun* fpGun;
	G4bool fContinue;
};

inline bool G4ITStepManager::IsInitialized()
{
	return fInitialized;
}

inline G4ITModelHandler* G4ITStepManager::GetModelHandler() {return fpModelHandler;}

inline void G4ITStepManager::SetEndTime(const double __endtime) { fEndTime = __endtime ;}

inline void G4ITStepManager::SetTimeSteps(std::map<double,double>* steps)
{
	fUsePreDefinedTimeSteps = true;
	fpUserTimeSteps = steps ;
}

inline G4int G4ITStepManager::GetNbSteps() const
{
	return fNbSteps;
}

inline void G4ITStepManager::SetMaxNbSteps(G4int maxSteps)
{
	fMaxSteps = maxSteps;
}

inline G4int G4ITStepManager::GetMaxNbSteps() const
{
	return fMaxSteps ;
}

inline G4double G4ITStepManager::GetStartTime() const
{
	return fStartTime;
}

inline G4double G4ITStepManager::GetEndTime() const
{
	return fEndTime;
} 

inline G4double G4ITStepManager::GetTimeStep() const
{
	return fTimeStep ;
}

inline void G4ITStepManager::SetDefaultTimeStep(double timeStep)
{
	fDefaultMinTimeStep = timeStep;
}

inline G4double G4ITStepManager::GetGlobalTime() const
{
	return fGlobalTime ;
}

inline void G4ITStepManager::SetUserAction(G4UserTimeStepAction* userITAction)
{
	fpUserTimeStepAction = userITAction;
}

inline G4UserTimeStepAction* G4ITStepManager::GetUserReactionAction() const
{
	return fpUserTimeStepAction;
}

inline void G4ITStepManager::SetVerbose(int verbose)
{
	fVerbose = verbose;
}

inline int G4ITStepManager::GetVerbose() const
{
	return fVerbose ;
}

inline void G4ITStepManager::SetMaxZeroTimeAllowed(int maxTimeStepAllowed)
{
	fMaxNZeroTimeStepsAllowed = maxTimeStepAllowed;
}

inline int G4ITStepManager::GetMaxZeroTimeAllowed() const
{
	return fMaxNZeroTimeStepsAllowed ;
}

inline void G4ITStepManager::SetTimeTolerance(double time)
{
	fTimeTolerance = time;
}

inline double G4ITStepManager::GetTimeTolerance() const
{
	return fTimeTolerance;
}

inline G4double G4ITStepManager::GetPreviousTimeStep() const
{
	return fPreviousTimeStep;
}

inline G4ITStepStatus G4ITStepManager::GetStatus() const
{
	return fITStepStatus;
}

inline void G4ITStepManager::Stop()
{
	fContinue = false;
}

inline G4ITTrackingInteractivity* G4ITStepManager::GetInteractivity()
{
	return fpTrackingInteractivity ;
}

inline void G4ITStepManager::SetGun(G4ITGun* gun)
{
	fpGun = gun;
}

#endif

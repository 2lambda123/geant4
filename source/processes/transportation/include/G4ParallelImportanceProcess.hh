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
// $Id: G4ParallelImportanceProcess.hh,v 1.10 2003-08-19 16:37:23 dressel Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4ParallelImportanceProcess
//
// Class description:
//
// Used internally by importance sampling in a "parallel" geometry.
// This is a G4ParallelTransport that also does importance
// sampling in the "parallel" geometry.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4ParallelImportanceProcess_hh
#define G4ParallelImportanceProcess_hh G4ParallelImportanceProcess_hh

#include "G4ParallelTransport.hh"
#include "G4VTrackTerminator.hh"

class G4SplittingAndRussianRouletePostStepDoIt;
class G4VImportanceSplitExaminer;
class G4Nsplit_Weight;


class G4ParallelImportanceProcess : public G4ParallelTransport, public G4VTrackTerminator
{

public:  // with description

  G4ParallelImportanceProcess(const G4VImportanceSplitExaminer &aImportanceSplitExaminer,
			      G4VPGeoDriver &pgeodriver, 
			      G4VParallelStepper &aStepper,
			      const G4VTrackTerminator *TrackTerminator,
			      const G4String &aName = "ParallelImportanceProcess");  
    // initialise G4ParallelTransport and members

  virtual ~G4ParallelImportanceProcess();

  virtual G4VParticleChange *PostStepDoIt(const G4Track&,
					  const G4Step&);
    // do the "parallel transport" and importance sampling.

  virtual void KillTrack() const;
    // used in case no scoring process follows that does the killing

  virtual const G4String &GetName() const;


private:

  G4ParallelImportanceProcess(const G4ParallelImportanceProcess &);
  G4ParallelImportanceProcess &operator=(const G4ParallelImportanceProcess &);
  
  virtual void Error(const G4String &m);

private:

  G4ParticleChange *fParticleChange;
  const G4VImportanceSplitExaminer &fImportanceSplitExaminer;  
  G4SplittingAndRussianRouletePostStepDoIt *fSplittingAndRussianRouletePostStepDoIt;
};

#endif





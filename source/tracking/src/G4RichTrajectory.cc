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
// $Id: G4RichTrajectory.cc,v 1.2 2006-05-24 11:43:26 tsasaki Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Last checked in at $Date: 2006-05-24 11:43:26 $ by $Author: tsasaki $
//
// ---------------------------------------------------------------
//
// G4RichTrajectory.cc
//
// Contact:
//   Questions and comments on G4Trajectory, on which this is based,
//   should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Makoto  Asai   (e-mail: asai@kekvax.kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//   and on the extended code to:
//     John Allison   (e-mail: John.Allison@manchester.ac.uk)
//     Joseph Perl    (e-mail: perl@slac.stanford.edu)
//
// ---------------------------------------------------------------

#include "G4RichTrajectory.hh"
#include "G4RichTrajectoryPoint.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4VProcess.hh"

//#define G4ATTDEBUG
#ifdef G4ATTDEBUG
#include "G4AttCheck.hh"
#endif

G4Allocator<G4RichTrajectory> aRichTrajectoryAllocator;

G4RichTrajectory::G4RichTrajectory() {}

G4RichTrajectory::G4RichTrajectory(const G4Track* aTrack):
  G4Trajectory(aTrack)  // Note: this initialises the base class data
			// members and, unfortunately but never mind,
			// creates a G4TrajectoryPoint in
			// TrajectoryPointContainer that we cannot
			// access because it's private.  We store the
			// same information (plus more) in a
			// G4RichTrajectoryPoint in the
			// RichTrajectoryPointsContainer
{
  fpInitialVolume = aTrack->GetVolume();
  fpInitialNextVolume = aTrack->GetNextVolume();
  fpCreatorProcess = aTrack->GetCreatorProcess();
  fpRichPointsContainer = new RichTrajectoryPointsContainer;
  // Insert the first rich trajectory point (see note above)...
  fpRichPointsContainer->push_back(new G4RichTrajectoryPoint(aTrack));
}

G4RichTrajectory::G4RichTrajectory(G4RichTrajectory & right):
  G4Trajectory(right)
{
  fpInitialVolume = right.fpInitialVolume;
  fpInitialNextVolume = right.fpInitialNextVolume;
  fpCreatorProcess = right.fpCreatorProcess;
  fpRichPointsContainer = new RichTrajectoryPointsContainer;
  for(size_t i=0;i<right.fpRichPointsContainer->size();i++)
  {
    G4RichTrajectoryPoint* rightPoint =
      (G4RichTrajectoryPoint*)((*(right.fpRichPointsContainer))[i]);
    fpRichPointsContainer->push_back(new G4RichTrajectoryPoint(*rightPoint));
  }
}

G4RichTrajectory::~G4RichTrajectory()
{
  //  fpRichPointsContainer->clearAndDestroy();
  size_t i;
  for(i=0;i<fpRichPointsContainer->size();i++){
    delete  (*fpRichPointsContainer)[i];
  }
  fpRichPointsContainer->clear();

  delete fpRichPointsContainer;
}

void G4RichTrajectory::AppendStep(const G4Step* aStep)
{
  fpRichPointsContainer->push_back(new G4RichTrajectoryPoint(aStep));
}
  
void G4RichTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  G4Trajectory::MergeTrajectory(secondTrajectory);

  G4RichTrajectory* seco = (G4RichTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(G4int i=1;i<ent;i++) {
    // initial point of the second trajectory should not be merged
    fpRichPointsContainer->push_back((*(seco->fpRichPointsContainer))[i]);
    //    fpRichPointsContainer->push_back(seco->fpRichPointsContainer->removeAt(1));
  }
  delete (*seco->fpRichPointsContainer)[0];
  seco->fpRichPointsContainer->clear();
}

const std::map<G4String,G4AttDef>* G4RichTrajectory::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("G4RichTrajectory",isNew);
  if (isNew) {

    *store = *(G4Trajectory::GetAttDefs());

    G4String ID;

    ID = "IVN";
    (*store)[ID] = G4AttDef(ID,"Initial Volume Name",
			    "Bookkeeping","","G4String");

    ID = "INVN";
    (*store)[ID] = G4AttDef(ID,"Initial Next Volume Name",
			    "Bookkeeping","","G4String");

    ID = "CPN";
    (*store)[ID] = G4AttDef(ID,"Creator Process Name",
			    "Bookkeeping","","G4String");

    ID = "CPTN";
    (*store)[ID] = G4AttDef(ID,"Creator Process Type Name",
			    "Bookkeeping","","G4String");

  }

  return store;
}

std::vector<G4AttValue>* G4RichTrajectory::CreateAttValues() const
{
  std::vector<G4AttValue>* values = G4Trajectory::CreateAttValues();

  values->push_back(G4AttValue("IVN",fpInitialVolume->GetName(),""));

  values->push_back(G4AttValue("INVN",fpInitialNextVolume->GetName(),""));

  if (fpCreatorProcess) {
    values->push_back(G4AttValue("CPN",fpCreatorProcess->GetProcessName(),""));
    G4ProcessType type = fpCreatorProcess->GetProcessType();
    values->push_back(G4AttValue("CPTN",G4VProcess::GetProcessTypeName(type),""));
  }

#ifdef G4ATTDEBUG
  G4cout << G4AttCheck(values,GetAttDefs());
#endif

  return values;
}

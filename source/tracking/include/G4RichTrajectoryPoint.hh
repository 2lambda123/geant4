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
// $Id: G4RichTrajectoryPoint.hh,v 1.1 2005-11-24 12:47:36 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//---------------------------------------------------------------
//
// G4RichTrajectoryPoint.hh
//
// class description:
//   This class extends G4TrajectoryPoint.
//   From G4Trajectory, the following information is included:
//     1) Position (end of step).
//   The extended information, only publicly accessible through AttValues,
//   includes:
//     2) Total energy deposit.
//     3) Procees defining end of step.
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Makoto  Asai   (e-mail: asai@kekvax.kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//   and on the extended code to:
//     John Allison   (e-mail: John.Allison@manchester.ac.uk)
//     Joseph Perl    (e-mail: perl@slac.stanford.edu)
//
// ---------------------------------------------------------------

#ifndef G4RICHTRAJECTORYPOINT_HH
#define G4RICHTRAJECTORYPOINT_HH

#include "G4TrajectoryPoint.hh"

class G4Track;
class G4Step;
class G4VProcess;

class G4RichTrajectoryPoint : public G4TrajectoryPoint
{

public: // without description

  // Constructor/Destructor
  G4RichTrajectoryPoint();
  G4RichTrajectoryPoint(const G4Track*);
  G4RichTrajectoryPoint(const G4Step*);
  G4RichTrajectoryPoint(const G4RichTrajectoryPoint &right);
  virtual ~G4RichTrajectoryPoint();

  // Operators
  inline void *operator new(size_t);
  inline void operator delete(void *aRichTrajectoryPoint);
  inline int operator==(const G4RichTrajectoryPoint& right) const
  { return (this==&right); };

  // Get methods for HepRep style attributes
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;

private:

  // Extended member data
  G4double fTotEDep;
  const G4VProcess* fpProcess;
};

#if defined G4TRACKING_ALLOC_EXPORT
extern G4DLLEXPORT G4Allocator<G4RichTrajectoryPoint>
aRichTrajectoryPointAllocator;
#else
extern G4DLLIMPORT G4Allocator<G4RichTrajectoryPoint>
aRichTrajectoryPointAllocator;
#endif

inline void* G4RichTrajectoryPoint::operator new(size_t)
{
  void *aRichTrajectoryPoint;
  aRichTrajectoryPoint =
    (void *) aRichTrajectoryPointAllocator.MallocSingle();
  return aRichTrajectoryPoint;
}

inline void G4RichTrajectoryPoint::operator delete
(void *aRichTrajectoryPoint)
{
  aRichTrajectoryPointAllocator.FreeSingle
    ((G4RichTrajectoryPoint *) aRichTrajectoryPoint);
}

#endif


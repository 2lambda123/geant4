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
// $Id: G4CollisionNNToDeltaDeltastar.hh,v 1.1 2003-10-07 12:37:26 hpw Exp $ //

#ifndef G4CollisionNNToDeltaDeltastar_h
#define G4CollisionNNToDeltaDeltastar_h

#include "globals.hh"
#include "G4GeneralNNCollision.hh"
#include "G4VCrossSectionSource.hh"
#include "G4VAngularDistribution.hh"
#include "G4KineticTrackVector.hh"
#include <vector>

class G4KineticTrack;

class G4CollisionNNToDeltaDeltastar : public G4GeneralNNCollision
{

public:

  G4CollisionNNToDeltaDeltastar();
  virtual ~G4CollisionNNToDeltaDeltastar() {};

  virtual G4String GetName() const { return "NN -> Delta Delta* Collision"; }
  

protected:

  std::vector<G4String> result;
  virtual const std::vector<G4String>& GetListOfColliders(G4int ) const
  {
    G4Exception("Tried to call G4CollisionNNToNDeltastar::GetListOfColliders. Please find out why!");
    return result;
  } 

};

#endif

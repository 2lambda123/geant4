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

#ifndef G4CollisionNNToDeltaDelta1950_h
#define G4CollisionNNToDeltaDelta1950_h

#include "globals.hh"
#include "G4GeneralNNCollision.hh"
#include "G4VCrossSectionSource.hh"
#include "G4VAngularDistribution.hh"
#include "G4KineticTrackVector.hh"
#include <vector>

class G4CollisionNNToDeltaDelta1950 : public G4GeneralNNCollision
{

public:

  G4CollisionNNToDeltaDelta1950();

  virtual ~G4CollisionNNToDeltaDelta1950() {}


  virtual G4String GetName() const { return "NN -> Delta Delta(1950) Collision"; }
  virtual const std::vector<G4String>& GetListOfColliders(G4int ) const
  {
    throw G4HadronicException(__FILE__, __LINE__, "Tried to call G4CollisionNNToDeltaDelta1950::GetListOfColliders. Please find out why!");
    std::vector<G4String> * aList = new std::vector<G4String>;
    return *aList;
  } 
  

protected:
  
  virtual const G4CollisionVector* GetComponents() const { return components; } 

private:  

  G4CollisionVector* components;

};

#endif

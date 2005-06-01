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
// $Id: G4VDigi.hh,v 1.3 2005-06-01 17:15:35 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4VDigi_h
#define G4VDigi_h 1

#include "globals.hh"
#include <vector>
#include <map>

class G4AttDef;
class G4AttValue;

// class description:
//
//  This is the base class of digi object. The user should derive this
// base class to make his/her own digi class. Two virtual method Draw()
// and Print() can be implemented if the user wants these functionarities.
//  If a concrete digi class is used as a transient class, G4Allocator
// must be used.

class G4VDigi 
{

  public:
      G4VDigi();
      virtual ~G4VDigi();

      G4int operator==(const G4VDigi &right) const;

      virtual void Draw();
      virtual void Print();

      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const
      { return 0; }
      // If implemented by a derived class, returns a pointer to a map
      // of attribute definitions for the attribute values below.  The
      // user must test the validity of this pointer.  See
      // G4Trajectory for an example of a concrete implementation of
      // this method.
      virtual std::vector<G4AttValue>* CreateAttValues() const
      { return 0; }
      // If implemented by a derived class, returns a pointer to a
      // list of attribute values suitable, e.g., for picking.  Each
      // must refer to an attribute definition in the above map; its
      // name is the key.  The user must test the validity of this
      // pointer (it must be non-zero and conform to the G4AttDefs,
      // which may be checked with G4AttCheck) and delete the list
      // after use.  See G4Trajectory for an example of a concrete
      // implementation of this method and
      // G4VTrajectory::ShowTrajectory for an example of its use.

};

#endif


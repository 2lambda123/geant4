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
// $Id: G4VUserTrackInformation.hh,v 1.4 2003-05-09 20:42:27 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//---------------------------------------------------------------
//
// G4VUserTrackInformation
//
// Class Description:
//
//  Abstract class which the user can derive his/her own concrete
// class for toring user's information associating with a G4Track
// class object.
//
//  It is user's responsibility 
//   1) Construct a concrete class object and set the pointer to
//     proper G4Track object
//   2) Concrete class derived from this class is expected to use G4Allocator
//     for memory management or something equivarent for performance reason
//
//  To set a pointer of a concrete class object to G4Track in
// G4UserTrackingAction concrete implementation, given the G4Track
// object is available only by "pointer to const", SetUserTrackInformation()
// method of G4TrackingManager is available.
//
//  The concrete class object is deleted by Geant4 kernel when
// associated G4Track object is deleted.


#ifndef G4VUserTrackInformation_H
#define G4VUserTrackInformation_H 1

class G4VUserTrackInformation
{
  public:
    G4VUserTrackInformation() {;}
    virtual ~G4VUserTrackInformation() {;}

  public:
    virtual void Print() const = 0;
};

#endif


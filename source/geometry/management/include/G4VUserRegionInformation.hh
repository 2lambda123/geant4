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
// $Id: G4VUserRegionInformation.hh,v 1.3 2003-11-02 14:01:22 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// --------------------------------------------------------------------
//
// G4VUserRegionInformation
//
// Class Description:
//
// Abstract class, the user can subclass for storing information
// associated with a G4Region class object.
//
// It is user's responsibility to construct a concrete class object 
// and set the pointer to proper G4Region object.
// The concrete class object is deleted by Geant4 kernel when
// associated G4Region object is deleted.

// History:
// 21/10/2003 - M.Asai, Created.
// --------------------------------------------------------------------
#ifndef G4VUserRegionInformation_H
#define G4VUserRegionInformation_H 1

class G4VUserRegionInformation
{
  public:  // with description
  
    G4VUserRegionInformation() {}
    virtual ~G4VUserRegionInformation() {}

    virtual void Print() const = 0;
};

#endif


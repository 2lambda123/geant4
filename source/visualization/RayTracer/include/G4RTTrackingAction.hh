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
// $Id: G4RTTrackingAction.hh,v 1.5 2003-05-30 13:01:20 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//

// class description:
//
//  This is a concrete class of G4UserTrackingAction. This class is used
// by G4RayTracer for managing a ray tracked through volumes. An object
// of this class is constructed by G4RayTracer and set to G4TrackingManager
// with replacement of user defined tracking action during the period of
// ray tracing.
//

///////////////////////
//G4RTTrackingAction.hh
///////////////////////


#ifndef G4RTTrackingAction_h
#define G4RTTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class G4Track;

///////////////////////////
class G4RTTrackingAction : public G4UserTrackingAction
///////////////////////////
{

//--------
   public:
//--------

// Constructor & Destructor
   G4RTTrackingAction(){;}
   virtual ~G4RTTrackingAction(){;}

// Member functions
   virtual void PreUserTrackingAction(const G4Track* aTrack);
   virtual void PostUserTrackingAction(const G4Track*){;}


};

#endif



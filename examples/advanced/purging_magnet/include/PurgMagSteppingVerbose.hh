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
// ********************************************************************
// Code developed by:
//  S.Larsson
//
//    ************************************
//    *                                  *
//    *    PurgMagSteppingVerbose.hh     *
//    *                                  *
//    ************************************
//
// $Id: PurgMagSteppingVerbose.hh,v 1.2 2004-06-18 09:17:51 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

class PurgMagSteppingVerbose;

#ifndef PurgMagSteppingVerbose_h
#define PurgMagSteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

class PurgMagSteppingVerbose : public G4SteppingVerbose {

public:   
  
  //Constructor/Destructor
  PurgMagSteppingVerbose();
  ~PurgMagSteppingVerbose();
  
  void StepInfo();
  void TrackingStarted();

};

#endif

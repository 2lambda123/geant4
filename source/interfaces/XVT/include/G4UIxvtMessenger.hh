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
// $Id: G4UIxvtMessenger.hh,v 1.4 2001-07-11 10:01:19 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// The original code (written by Makoto Asai) has been adapted for use      //
// with the XVT Geant GUI                                                   //
//                                                                          //
// By: Simon Prior 11/08/97                                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#ifndef G4UIxvtMessenger_h
#define G4UIxvtMessenger_h 1

#if defined(G4UI_BUILD_XVT_SESSION) || defined(G4UI_USE_XVT)

#include "G4UImessenger.hh"
#include "G4UImanager.hh"

class G4UIxvt;

class G4UIxvtMessenger : public G4UImessenger 
{
  public:
      G4UIxvtMessenger(G4UIxvt*);
      void SetNewValue(G4UIcommand * command,G4String newValue);
  private:
      G4UIxvt* xvtptr;
};

#endif

#endif


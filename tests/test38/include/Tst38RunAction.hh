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
// $Id: Tst38RunAction.hh,v 1.1 2006-05-03 07:09:59 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef Tst38RunAction_h
#define Tst38RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4ios.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"

class G4Run;

class Tst38RunAction : public G4UserRunAction
{
  public:
    Tst38RunAction();
    ~Tst38RunAction();

  public:
    void BeginOfRunAction(const G4Run* aRun);
    void EndOfRunAction(const G4Run* aRun);

};

#endif


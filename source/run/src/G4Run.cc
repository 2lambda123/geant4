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
// $Id: G4Run.cc,v 1.6 2004-06-09 07:34:18 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

// Export extern symbols for DLLs build
//
#define G4RUN_EXPORT 1

#include "G4Run.hh"

G4Allocator<G4Run> aRunAllocator;

G4Run::G4Run()
:runID(0),numberOfEvent(0),numberOfEventToBeProcessed(0),HCtable(0),DCtable(0)
{;}

G4Run::~G4Run()
{;}


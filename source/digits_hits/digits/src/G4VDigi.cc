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
// $Id: G4VDigi.cc,v 1.1 2003-10-03 10:15:39 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

// G4VDigi
#include "G4VDigi.hh"
#include "globals.hh"

G4VDigi::G4VDigi()
{;}

G4VDigi::~G4VDigi()
{;}

G4int G4VDigi::operator==(const G4VDigi &right) const
{ return (this==&right); }

void G4VDigi::Draw() 
{;}

void G4VDigi::Print() 
{;}


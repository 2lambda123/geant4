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
// $Id: G4VKineticNucleon.cc,v 1.1 2003-10-07 12:38:35 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#include "G4VKineticNucleon.hh"

G4VKineticNucleon::G4VKineticNucleon()
{
}

G4VKineticNucleon::G4VKineticNucleon(const G4VKineticNucleon &)
{
}


G4VKineticNucleon::~G4VKineticNucleon()
{
}


//const G4VKineticNucleon & G4VKineticNucleon::operator=(const G4VKineticNucleon &right)
//{}


int G4VKineticNucleon::operator==(const G4VKineticNucleon &right) const
{
	return this == &right;
}

int G4VKineticNucleon::operator!=(const G4VKineticNucleon &right) const
{
	return this != &right;

}





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
// $Id: G4VParticipants.cc,v 1.2 2005-06-04 13:47:01 jwellisc Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      ---------------- G4VParticipants ----------------
//             by Gunter Folger, May 1998.
//      abstract class finding participants in a hadron Nucleus collision
//       in Parton String Models.
// ------------------------------------------------------------

#include "G4VParticipants.hh"
#include "Randomize.hh"

G4VParticipants::G4VParticipants() : theNucleus(NULL)
{}



G4VParticipants::~G4VParticipants()
{
// G4cout << "G4VParticipants::~G4VParticipants()" << G4endl;
	if ( theNucleus != NULL ) delete theNucleus;
}








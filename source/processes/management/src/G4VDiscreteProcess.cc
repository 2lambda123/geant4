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
// $Id: G4VDiscreteProcess.cc,v 1.4 2003-11-03 03:27:28 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//	GEANT 4 class implementation file 
//
//	History: first implementation, based on object model of
//	2nd December 1995, G.Cosmo
// --------------------------------------------------------------
//   New Physics scheme           8 Jan. 1997  H.Kurahige
// ------------------------------------------------------------

#include "G4VDiscreteProcess.hh"
G4VDiscreteProcess::G4VDiscreteProcess()
                   :G4VProcess("No Name Discrete Process") 
{
  G4Exception("G4VDiscreteProcess::G4VDiscreteProcess()","Illegal operation",
	      JustWarning,"Default constructor is called");
}

G4VDiscreteProcess::G4VDiscreteProcess(const G4String& aName , G4ProcessType aType)
                  : G4VProcess(aName, aType)
{
}

G4VDiscreteProcess::~G4VDiscreteProcess()
{
}

G4VDiscreteProcess::G4VDiscreteProcess(G4VDiscreteProcess& right)
                  : G4VProcess(right)
{
}











